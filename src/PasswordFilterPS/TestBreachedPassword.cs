using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsDiagnostic.Test, "BreachedPassword", DefaultParameterSetName = "BreachedPasswordString")]
    public class TestBreachedPassword : PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "BreachedPasswordString"), ValidateNotNullOrEmpty]
        public string Password { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true, ParameterSetName = "BreachedPasswordHash"), ValidateNotNullOrEmpty]
        public byte[] Hash { get; set; }

        [Parameter(Mandatory = false, Position = 2, ParameterSetName = "BreachedPasswordString")]
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
            if (this.ParameterSetName == "BreachedPasswordString")
                {
                string password = this.Normalize ? StringNormalizer.Normalize(this.Password) : this.Password;

                this.WriteObject(Global.Store.IsInStore(password, StoreType.Password));
            }
            else
            {
                this.WriteObject(Global.Store.IsInStore(this.Hash, StoreType.Password));
            }
        }
    }
}
