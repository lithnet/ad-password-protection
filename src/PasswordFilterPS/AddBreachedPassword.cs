using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsCommon.Add, "BreachedPassword")]
    public class AddBreachedPassword : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Password { get; set; }

        private StoreInterface.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new StoreInterface.OperationProgress();

            Global.Store.StartBatch(StoreInterface.StoreType.Password);
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Global.Store.EndBatch(StoreInterface.StoreType.Password, new System.Threading.CancellationToken(), this.progress);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            Global.Store.AddToStore(this.Password, StoreInterface.StoreType.Password);
        }
    }
}
