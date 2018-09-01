using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsCommon.Open, "Store")]
    public class OpenStore : Cmdlet
    {
        [Parameter(Mandatory = false, Position = 1, ValueFromPipeline = false)]
        public string Path { get; set; }

        protected override void BeginProcessing()
        {
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            base.EndProcessing();
        }

        protected override void StopProcessing()
        {
            base.StopProcessing();
        }

        protected override void ProcessRecord()
        {
            if (this.Path == null)
            {
                Global.OpenStore();
            }
            else
            {
                Global.OpenStore(this.Path);
            }
        }
    }
}
