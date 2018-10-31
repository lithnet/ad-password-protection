using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;
using StoreInterface;
using DSInternals;
using DSInternals.Replication;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsDiagnostic.Test, "ADUserPassword")]
    public class TestADUserPassword : PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string AccountName { get; set; }

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
            DSInternals.Replication.DirectoryReplicationClient client = new DirectoryReplicationClient("fim-dev1.local", RpcProtocol.TCP);
            var x = client.GetAccountByUPN(this.AccountName);

            if (Global.Store.IsInStore(x.NTHash, StoreType.Password))
            {
                this.WriteObject($"{this.AccountName}: password has been pwned");
            }
        }
    }
}
