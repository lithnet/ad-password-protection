using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Get, "StorePath")]
    public class GetStorePath : PasswordProtectionCmdletBase
    {
        protected override void EndProcessing()
        {
            if (Global.IsOpen)
            {
                this.WriteObject(Global.Store.GetPath());
            }
            else
            {
                Global.OpenStore();
                this.WriteObject(Global.Store.GetPath());
            }
        }
    }
}
