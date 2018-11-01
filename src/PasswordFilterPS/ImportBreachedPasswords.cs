using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsData.Import, "BreachedPasswords")]
    public class ImportBreachedPasswords : ImportPSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]
        public int BatchSize { get; set; } = -1;

        private CancellationTokenSource token = new CancellationTokenSource();

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.InitializeProgressUpdate($"Importing plain-text passwords from {this.Filename}");
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
                    StoreInterface.Store.ImportPasswordsFromFile(Global.Store, StoreInterface.StoreType.Password, this.Filename, this.token.Token, this.BatchSize, this.Progress);
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

            this.EndProgressUpdate();
        }
    }
}
