using System;
using System.IO;
using System.Linq;
using Microsoft.Win32;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public class Settings
    {
        private readonly RegistryKey hklm = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64);

        public string StorePath
        {
            get => this.GetPolicyOrSetting<string>("Store", null);
            set
            {
                if (!Directory.Exists(value))
                {
                    throw new DirectoryNotFoundException($"The path {value} was not found");
                }

                var settingsKey = this.hklm.CreateSubKey("Software\\Lithnet\\PasswordFilter", true);
                settingsKey.SetValue("Store", value);
            }
        }

        public bool Enabled
        {
            get => this.GetPolicyOrSetting<int>("Disabled", 0) == 0;
            set
            {
                var settingsKey = this.hklm.CreateSubKey("Software\\Lithnet\\PasswordFilter", true);
                if (value)
                {
                    settingsKey.DeleteValue("Disabled", false);
                }
                else
                {
                    settingsKey.SetValue("Disabled", 1);
                }
            }
        }

        public bool IsFilterRegistered
        {
            get
            {
                RegistryKey key = this.hklm.OpenSubKey("SYSTEM\\CurrentControlSet\\Control\\Lsa");

                var values = key?.GetValue("Notification Packages") as string[];

                return values?.Contains("lithnetpwdf", StringComparer.OrdinalIgnoreCase) ?? false;
            }
        }

        public bool IsFilterEnabledStatePolicyControlled()
        {
            return this.IsPolicyEnforced("Disabled");
        }

        public bool IsStorePathPolicyControlled()
        {
            return this.IsPolicyEnforced("Store");
        }

        private bool IsPolicyEnforced(string setting)
        {
            var policyKey = this.hklm.OpenSubKey("Software\\Policies\\Lithnet\\PasswordFilter", false);

            return policyKey?.GetValue(setting, null) != null;
        }

        private T GetPolicyOrSetting<T>(string name, T defaultValue)
        {
            var settingsKey = this.hklm.OpenSubKey("Software\\Lithnet\\PasswordFilter", false);
            var policyKey = this.hklm.OpenSubKey("Software\\Policies\\Lithnet\\PasswordFilter", false);

            var result = policyKey?.GetValue(name);

            if (result == null)
            {
                result = settingsKey?.GetValue(name);
            }

            if (result == null)
            {
                return default;
            }

            return (T)result;
        }
    }
}
