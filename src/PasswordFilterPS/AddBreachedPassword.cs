using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Add, "BreachedPassword")]
    public class AddBreachedPassword : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        private PasswordProtection.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new PasswordProtection.OperationProgress();

            Global.Store.StartBatch(PasswordProtection.StoreType.Password);
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Global.Store.EndBatch(PasswordProtection.StoreType.Password, new System.Threading.CancellationToken(), this.progress);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            Global.Store.AddToStore(this.Value, PasswordProtection.StoreType.Password);
        }
    }
}
