using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Get, "PasswordFilterConfig")]
    public class GetPasswordFilterConfig : PasswordFilterConfigCmdletBase
    {
        protected override void EndProcessing()
        {
            this.WriteSettings();
        }
    }
}
