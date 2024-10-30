using System;
using System.Management.Automation;
using System.Threading;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsData.Sync, "HashesFromHibp")]
    public class SyncHashesFromHibp : ImportPSCmdlet
    {
        [Parameter(Mandatory = false, Position = 0)]
        public int Threads { get; set; } = -1;

        [Parameter]
        public SwitchParameter Reset { get; set; }

        [Parameter]
        public string ProxyAddress { get; set; }

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.InitializeProgressUpdate("Synchronizing password hashes from api.haveibeenpwned.com");

            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Uri proxyUri = null;
            if (!string.IsNullOrWhiteSpace(this.ProxyAddress))
            {
                if (!Uri.TryCreate(this.ProxyAddress, UriKind.Absolute, out proxyUri))
                {
                    throw new ArgumentException("The proxy address is not a valid URI");
                }

                this.WriteVerbose($"Using proxy server {this.ProxyAddress}");
            }

            HibpDownloader downloader = new HibpDownloader(Global.Store, proxyUri);

            if (this.MyInvocation.BoundParameters.ContainsKey(nameof(this.Reset)) && this.Reset)
            {
                this.WriteVerbose("Removing saved state");
                downloader.DeleteSavedState();
            }

            var task = Task.Run(() =>
            {
                try
                {
                    downloader.Execute(this.Progress, this.Threads, this.token.Token);
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

            var endTime = DateTime.Now;
            var results = new PSObject();

            results.Properties.Add(new PSNoteProperty("OperationStart", this.Progress.HibpStartTime));
            results.Properties.Add(new PSNoteProperty("OperationFinish", endTime));
            results.Properties.Add(new PSNoteProperty("Duration", endTime - this.Progress.HibpStartTime));
            results.Properties.Add(new PSNoteProperty("PagesRetrieved", this.Progress.HibpCurrentHash));
            results.Properties.Add(new PSNoteProperty("PagesWithChanges", this.Progress.HibpRangesChanged));
            results.Properties.Add(new PSNoteProperty("PagesUnchanged", this.Progress.HibpRangesUnchanged));
            results.Properties.Add(new PSNoteProperty("NewHashesImported", this.Progress.HashesAdded));
            results.Properties.Add(new PSNoteProperty("ExistingHashesDiscarded", this.Progress.HashesDiscarded));
            results.Properties.Add(new PSNoteProperty("TotalHashesProcessed", this.Progress.HashesAdded + this.Progress.HashesDiscarded));

            this.WriteObject(results);
            task.GetAwaiter().GetResult();
        }
    }
}
