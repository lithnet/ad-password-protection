using System;
using System.Management.Automation;
using System.Security.Principal;
using Lithnet.ActiveDirectory.PasswordProtection;
using DSInternals.Replication;
using DSInternals.Common.Data;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsDiagnostic.Test, "IsADUserPasswordCompromised", DefaultParameterSetName = "AccountName")]
    public class TestIsADUserPasswordCompromised : PSCmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false, ParameterSetName = "AccountName"), ValidateNotNullOrEmpty]
        public string AccountName { get; set; }

        [Parameter(Mandatory = true, Position = 2, ValueFromPipeline = false, ParameterSetName = "AccountName"), ValidateNotNullOrEmpty]
        public string DomainName { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false, ParameterSetName = "Upn"), ValidateNotNullOrEmpty]
        public string Upn { get; set; }

        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false, ParameterSetName = "Sid"), ValidateNotNullOrEmpty]
        public string Sid { get; set; }

        [Parameter(Mandatory = false, Position = 3, ValueFromPipeline = false), ValidateNotNullOrEmpty]
        public string Server { get; set; }

        [Parameter(Mandatory = false, Position = 4, ValueFromPipeline = false), ValidateNotNullOrEmpty]
        public PSCredential Credential { get; set; }

        [Parameter(Mandatory = false, Position = 5, ValueFromPipeline = false)]
        public SwitchParameter OutBadHashOnMatch { get; set; }

        private DirectoryReplicationClient client;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            base.BeginProcessing();
            this.client = new DirectoryReplicationClient(this.Server ?? Environment.GetEnvironmentVariable("UserDNSDomain"), RpcProtocol.TCP, this.Credential?.GetNetworkCredential());
        }

        protected override void ProcessRecord()
        {
            DSAccount account = null;

            switch (this.ParameterSetName)
            {
                case "AccountName":
                    account = this.client.GetAccount(new NTAccount(this.DomainName, this.AccountName));
                    break;

                case "Upn":
                    account = this.client.GetAccountByUPN(this.Upn);
                    break;

                case "Sid":
                    account = this.client.GetAccount(new SecurityIdentifier(this.Sid));
                    break;
            }

            if (account == null)
            {
                throw new InvalidOperationException("The account could not be found");
            }

            bool result = Global.Store.IsInStore(account.NTHash, StoreType.Password);
            if (this.OutBadHashOnMatch.IsPresent)
            {
                if (result)
                {
                    this.WriteObject(account.NTHash.ToHexString());
                }
            }
            else
            {
                this.WriteObject(result);
            }
        }
    }
}