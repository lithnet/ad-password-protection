using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Open, "Store")]
    public class OpenStore : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = false, Position = 1, ValueFromPipeline = false)]
        public string Path { get; set; }

        protected override void EndProcessing()
        {
            if (this.Path == null)
            {
                Global.OpenStore();
            }
            else
            {
                Global.OpenStore(this.Path);
            }

            var result = new PSObject();

            result.Properties.Add(new PSNoteProperty("StorePath", Global.Store.GetPath()));
            this.WriteObject(result);
        }
    }
}
