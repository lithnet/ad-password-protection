using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsDiagnostic.Test, "HashIsInStore")]
    public class TestHashIsInStore : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
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
            this.WriteObject(Global.Store.IsInStore(this.Hash, StoreInterface.StoreType.Password));
        }
    }
}
