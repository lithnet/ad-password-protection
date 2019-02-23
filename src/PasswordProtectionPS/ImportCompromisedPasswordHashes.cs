using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Management.Automation;
using Lithnet.ActiveDirectory.PasswordProtection;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsData.Import, "CompromisedPasswordHashes")]
    public class ImportCompromisedPasswordHashes : ImportPSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]
        public bool? Sorted { get; set; }

        [Parameter(Mandatory = false, Position = 3)]
        public int BatchSize { get; set; } = -1;

        private CancellationTokenSource token = new CancellationTokenSource();

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.InitializeProgressUpdate($"Importing password hashes from {this.Filename}");

            base.BeginProcessing();
        }
        protected override void StopProcessing()
        {
            this.token.Cancel();
        }

        protected override void EndProcessing()
        {
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            var task = Task.Run(() =>
            {
                try
                {
                    bool isSorted;
                    if (this.Sorted.HasValue)
                    {
                        isSorted = this.Sorted.Value;
                    }
                    else
                    {
                        isSorted = Store.DoesHexHashFileAppearSorted(this.Filename, V3Store.BinaryHashLength);
                    }

                    if (isSorted)
                    {
                        Store.ImportHexHashesFromSortedFile(Global.Store, StoreType.Password, this.Filename, this.token.Token, this.Progress);
                    }
                    else
                    {
                        Store.ImportHexHashesFromFile(Global.Store, StoreType.Password, this.Filename, this.token.Token, this.BatchSize, this.Progress);
                    }
                }
                catch (OperationCanceledException)
                {
                }
            }, this.token.Token);

            while (!(task.IsCompleted || task.IsCanceled || task.IsFaulted))
            {
                this.WriteProgressUpdate();
                Thread.Sleep(1000);
            }

            task.ThrowIfFaulted();

            this.EndProgressUpdate();
        }
    }
}
