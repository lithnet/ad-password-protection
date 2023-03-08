using System;
using System.Management.Automation;
using System.Threading;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsData.Import, "CompromisedPasswords")]
    public class ImportCompromisedPasswords : ImportPSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]
        public int BatchSize { get; set; } = -1;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.InitializeProgressUpdate($"Importing plain-text passwords from {this.Filename}");
            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            var task = Task.Run(() =>
            {
                try
                {
                    Store.ImportPasswordsFromFile(Global.Store, StoreType.Password, this.Filename, this.token.Token, this.BatchSize, this.Progress);
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
