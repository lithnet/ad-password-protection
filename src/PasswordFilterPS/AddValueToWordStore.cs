using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsCommon.Add, "ValueToWordStore")]
    public class AddValueToWordStore : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        private StoreInterface.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new StoreInterface.OperationProgress();

            Global.Store.StartBatch(StoreInterface.StoreType.Word);
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Global.Store.EndBatch(StoreInterface.StoreType.Word, this.progress);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            Global.Store.AddToStore(this.Value, StoreInterface.StoreType.Word);
        }
    }
}
