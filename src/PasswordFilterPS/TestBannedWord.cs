using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsDiagnostic.Test, "BannedWord", DefaultParameterSetName = "String")]
    public class TestBannedWord: PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "String"), ValidateNotNullOrEmpty]
        public string Word { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "Hash"), ValidateNotNullOrEmpty]
        public byte[] Hash { get; set; }

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            if (this.ParameterSetName == "String")
            {
                string password = StringNormalizer.Normalize(this.Word);

                this.WriteObject(Global.Store.IsInStore(password, StoreType.Word));
            }
            else
            {
                this.WriteObject(Global.Store.IsInStore(this.Hash, StoreType.Word));
            }
        }
    }
}
