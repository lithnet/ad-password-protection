using System.IO;
using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Set, "PasswordFilterConfig")]
    public class SetPasswordFilterConfig : PasswordFilterConfigCmdletBase
    {
        [Parameter(Mandatory = false, Position = 0)]
        public string StorePath { get; set; }

        [Parameter(Mandatory = false)]
        public SwitchParameter Enabled { get; set; }

        protected override void EndProcessing()
        {
            Settings settings = new Settings();

            if (this.MyInvocation.BoundParameters.ContainsKey(nameof(this.StorePath)))
            {
                if (!Directory.Exists(this.StorePath))
                {
                    throw new DirectoryNotFoundException($"The directory {this.StorePath} does not exist");
                }

                if (settings.IsStorePathPolicyControlled())
                {
                    this.WriteWarning("The password filter enabled store path is currently controlled by a Group Policy setting. The value set will be ignored while the policy is applied.");
                }

                settings.StorePath = this.StorePath;
            }

            if (this.MyInvocation.BoundParameters.ContainsKey(nameof(this.Enabled)))
            {
                settings.Enabled = this.Enabled;

                if (settings.IsFilterEnabledStatePolicyControlled())
                {
                    this.WriteWarning("The password filter enabled state is currently controlled by a Group Policy setting. The value set will be ignored while the policy is applied.");
                }
            }

            this.WriteSettings();
        }
    }
}
