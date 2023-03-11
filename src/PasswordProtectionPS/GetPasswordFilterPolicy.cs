using System.Management.Automation;
using Microsoft.Win32;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Cmdlet(VerbsCommon.Get, "PasswordFilterPolicy")]
    public class GetPasswordFilterPolicy : PasswordProtectionCmdletBase
    {
        [Parameter(Mandatory = false, Position = 0)]
        public string User { get; set; }

        protected override void EndProcessing()
        {
            RegistryKey hklm = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64);
            var userKey = GetPolicyKeyForUser(this.User);
            var policyKey = hklm.OpenSubKey($"Software\\Policies\\Lithnet\\PasswordFilter\\{userKey}", false);

            PSObject result = new PSObject();

            if (policyKey != null)
            {
                foreach (var valueName in policyKey.GetValueNames())
                {
                    result.Properties.Add(new PSNoteProperty(valueName, policyKey.GetValue(valueName)));
                }
            }
            else
            {
                this.WriteVerbose("There are no applicable policies configured on the system");
            }

            this.WriteObject(result);
        }

        private static string GetPolicyKeyForUser(string user)
        {
            return "Default";
        }
    }
}
