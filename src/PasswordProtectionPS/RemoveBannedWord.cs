using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using System.Security;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Remove, "BannedWord", DefaultParameterSetName = "String")]
    public class RemoveBannedWord : PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "String"), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "SecureString"), ValidateNotNullOrEmpty]
        public SecureString SecureString { get; set; }

        private PasswordProtection.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new PasswordProtection.OperationProgress();

            Global.Store.StartBatch(PasswordProtection.StoreType.Word);
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Global.Store.EndBatch(PasswordProtection.StoreType.Word, new System.Threading.CancellationToken(), this.progress);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            Global.Store.RemoveFromStore(this.ParameterSetName == "String" ? this.Value : this.SecureString.SecureStringToString(), PasswordProtection.StoreType.Word);
        }
    }
}
