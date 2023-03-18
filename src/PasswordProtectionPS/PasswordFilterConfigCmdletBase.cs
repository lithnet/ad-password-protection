using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    public class PasswordFilterConfigCmdletBase : PasswordProtectionCmdletBase
    {
        protected void WriteSettings()
        {
            Settings settings = new Settings();

            var result = new PSObject();
            result.Properties.Add(new PSNoteProperty("StorePath", settings.StorePath));
            result.Properties.Add(new PSNoteProperty("IsFilterRegistered", settings.IsFilterRegistered));
            result.Properties.Add(new PSNoteProperty("IsFilterEnabled", settings.IsFilterRegistered && settings.Enabled));

            this.WriteObject(result);
        }
    }
}
