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
    public class ImportBreachedPasswords : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]
        public int BatchSize { get; set; } = -1;

        private StoreInterface.OperationProgress progress;

        private CancellationTokenSource token = new CancellationTokenSource();

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new StoreInterface.OperationProgress();
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
                    StoreInterface.Store.ImportPasswordsFromFile(Global.Store, StoreInterface.StoreType.Password, this.Filename, this.token.Token, this.BatchSize, this.progress);
                }
                catch (OperationCanceledException)
                {
                }
            }, this.token.Token);

            var p = new ProgressRecord(1, "Importing passwords", "Starting...");

            DateTime startTime = DateTime.Now;

            while (!(task.IsCompleted || task.IsCanceled || task.IsFaulted))
            {
                p.StatusDescription = this.progress.Status ?? "Processing";
                p.CurrentOperation = this.progress.GetProgressText();
                p.PercentComplete = Math.Min(100, this.progress.ProgressPercent);

                double totalSeconds = (DateTime.Now - startTime).TotalSeconds;
                int secondsRemaining = 0;

                if (totalSeconds > 0)
                {
                    double bytesPerSecond = this.progress.ProgressCurrentValue / totalSeconds;
                    long remainingBytes = this.progress.ProgressTotalValue - this.progress.ProgressCurrentValue;

                    if (bytesPerSecond > 0)
                    {
                        double result = remainingBytes / bytesPerSecond;

                        if (result < Int32.MaxValue && result > 0)
                        {
                            secondsRemaining = Convert.ToInt32(result);
                        }
                    }
                }

                p.SecondsRemaining = secondsRemaining;

                this.WriteProgress(p);
                Thread.Sleep(1000);
            }
        }
    }
}
