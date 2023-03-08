using System.Management.Automation;
using System.Security;
using System.Threading;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Add, "BannedWord", DefaultParameterSetName = "String")]
    public class AddBannedWord : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "String"), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "SecureString"), ValidateNotNullOrEmpty]
        public SecureString SecureString { get; set; }

        private OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new OperationProgress();

            Global.Store.StartBatch(StoreType.Word);
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            Global.Store.EndBatch(StoreType.Word, this.progress, CancellationToken.None);
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            Global.Store.AddToStore(this.ParameterSetName == "String" ? this.Value : this.SecureString.SecureStringToString(), StoreType.Word);
        }
    }
}
