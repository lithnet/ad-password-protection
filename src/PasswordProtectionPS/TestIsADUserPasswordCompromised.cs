using System;
using System.DirectoryServices.ActiveDirectory;
using System.Management.Automation;
using System.Security.Principal;
using DSInternals.Common.Data;
using DSInternals.Replication;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsDiagnostic.Test, "IsADUserPasswordCompromised", DefaultParameterSetName = "AccountName")]
    public class TestIsADUserPasswordCompromised : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = false, ParameterSetName = "AccountName"), ValidateNotNullOrEmpty]
        public string AccountName { get; set; }

        [Parameter(Mandatory = false, Position = 2, ValueFromPipeline = false, ParameterSetName = "AccountName"), ValidateNotNullOrEmpty]
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
        public SwitchParameter OutputCompromisedHashOnMatch { get; set; }

        private DirectoryReplicationClient client;
        private bool disposedValue;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            base.BeginProcessing();

            string server = this.Server;

            if (server == null)
            {
                if (NativeMethods.IsDc())
                {
                    server = Environment.MachineName;
                }
                else
                {
                    server = Domain.GetComputerDomain().FindDomainController(LocatorOptions.WriteableRequired).Name;
                }
            }

            this.client = new DirectoryReplicationClient(server, RpcProtocol.TCP, this.Credential?.GetNetworkCredential());
        }

        protected override void ProcessRecord()
        {
            DSAccount account = null;

            switch (this.ParameterSetName)
            {
                case "AccountName":
                    this.DomainName ??= Environment.GetEnvironmentVariable("UserDomain");

                    account = this.client.GetAccount(new NTAccount(this.DomainName, this.AccountName));
                    break;

                case "Upn":
                    account = this.client.GetAccount(new NTAccount(this.Upn));
                    break;

                case "Sid":
                    account = this.client.GetAccount(new SecurityIdentifier(this.Sid));
                    break;
            }

            if (account == null)
            {
                throw new InvalidOperationException("The account could not be found");
            }

            if (account.NTHash == null)
            {
                if (!this.OutputCompromisedHashOnMatch.IsPresent)
                {
                    this.WriteObject(false);
                }

                return;
            }

            bool result = Global.Store.IsInStore(account.NTHash, StoreType.Password);
            if (this.OutputCompromisedHashOnMatch.IsPresent)
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

        protected override void Dispose(bool disposing)
        {
            if (!this.disposedValue)
            {
                if (disposing)
                {
                    this.client.Dispose();
                }

                this.disposedValue = true;
            }

            base.Dispose(disposing);
        }
    }
}