using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsData.Import, "BannedWordHashes")]
    public class ImportBannedWordHashes : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]

        public bool Sorted { get; set; }

        private StoreInterface.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            //Global.Store.StartBatch(StoreInterface.StoreType.Password);
            this.progress = new StoreInterface.OperationProgress();
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            //Global.Store.EndBatch(StoreInterface.StoreType.Password, null);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            if (this.Sorted)
            {
                StoreInterface.Store.ImportHexHashesFromSortedFile(Global.Store, StoreInterface.StoreType.Word, this.Filename, this.progress);
            }
            else
            {
                StoreInterface.Store.ImportHexHashesFromFile(Global.Store, StoreInterface.StoreType.Word, this.Filename, progress: this.progress);

            }
        }
    }
}
