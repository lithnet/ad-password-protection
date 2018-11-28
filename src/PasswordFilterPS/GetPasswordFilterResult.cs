using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsCommon.Get, "PasswordFilterResult")]
    public class GetPasswordFilterResult : PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Password { get; set; }

        [Parameter(Mandatory = true, Position = 2, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Username { get; set; }

        [Parameter(Mandatory = true, Position = 3, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Fullname { get; set; }

        [Parameter(Mandatory = false, Position = 4, ValueFromPipeline = false), ValidateNotNullOrEmpty]
        public bool IsSetOperation { get; set; }

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
            this.WriteObject(FilterInterface.TestPassword(this.Username, this.Fullname, this.Password, this.IsSetOperation));
        }
    }
}
