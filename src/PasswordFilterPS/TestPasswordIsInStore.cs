using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsDiagnostic.Test, "PasswordIsInStore")]
    public class TestPasswordIsInStore : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Password { get; set; }

        [Parameter]
        public bool Normalize { get; set; }

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
            string password = this.Normalize ? StringNormalizer.Normalize(this.Password) : this.Password;

            this.WriteObject(Global.Store.IsInStore(password, StoreInterface.StoreType.Password));
        }
    }
}
