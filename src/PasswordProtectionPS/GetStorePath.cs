using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Get, "StorePath")]
    public class GetStorePath : Cmdlet
    {
        protected override void ProcessRecord()
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
