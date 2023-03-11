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
            result.Properties.Add(new PSNoteProperty("Enabled", settings.Enabled));
            result.Properties.Add(new PSNoteProperty("IsFilterRegistered", settings.IsFilterRegistered));

            this.WriteObject(result);
        }
    }
}
