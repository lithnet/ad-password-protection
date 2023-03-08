using System.Management.Automation;
using System.Security;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsDiagnostic.Test, "IsBannedWord", DefaultParameterSetName = "String")]
    public class TestIsBannedWord : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "String"), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "Hash"), ValidateNotNullOrEmpty]
        public byte[] Hash { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "SecureString"), ValidateNotNullOrEmpty]
        public SecureString SecureString { get; set; }

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            if (this.ParameterSetName == "String")
            {
                string password = StringNormalizer.Normalize(this.Value);
                this.WriteObject(Global.Store.IsInStore(password, StoreType.Word));
            }
            else if (this.ParameterSetName == "SecureString")
            {
                string password = StringNormalizer.Normalize(this.SecureString.SecureStringToString());
                this.WriteObject(Global.Store.IsInStore(password, StoreType.Word));
            }
            else
            {
                this.WriteObject(Global.Store.IsInStore(this.Hash, StoreType.Word));
            }
        }
    }
}
