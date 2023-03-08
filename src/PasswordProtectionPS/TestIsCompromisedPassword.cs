using System.Management.Automation;
using System.Security;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsDiagnostic.Test, "IsCompromisedPassword", DefaultParameterSetName = "String")]
    public class TestIsCompromisedPassword : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "String"), ValidateNotNullOrEmpty]
        public string Value { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "Hash"), ValidateNotNullOrEmpty]
        public byte[] Hash { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "SecureString"), ValidateNotNullOrEmpty]
        public SecureString SecureString { get; set; }

        [Parameter(Mandatory = false, Position = 2, ParameterSetName = "String")]
        [Parameter(Mandatory = false, Position = 2, ParameterSetName = "SecureString")]
        public SwitchParameter Normalize { get; set; }

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            if (this.ParameterSetName == "String")
            {
                string password = this.Normalize.IsPresent ? StringNormalizer.Normalize(this.Value) : this.Value;
                this.WriteObject(Global.Store.IsInStore(password, StoreType.Password));
            }
            else if (this.ParameterSetName == "SecureString")
            {
                string password = this.SecureString.SecureStringToString();
                password = this.Normalize.IsPresent ? StringNormalizer.Normalize(password) : password;
                this.WriteObject(Global.Store.IsInStore(password, StoreType.Password));
            }
            else
            {
                this.WriteObject(Global.Store.IsInStore(this.Hash, StoreType.Password));
            }
        }
    }
}
