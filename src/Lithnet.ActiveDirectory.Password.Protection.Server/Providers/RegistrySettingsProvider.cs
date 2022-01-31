using System;
using System.Collections.Generic;
using System.Security;
using System.Text;
using Microsoft.Win32;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class RegistrySettingsProvider : IRegistrySettingsProvider
    {
        static string REG_BASE_SETTINGS_KEY = "SOFTWARE\\Lithnet\\PasswordFilter";
        static string REG_BASE_POLICY_KEY = "SOFTWARE\\Policies\\Lithnet\\PasswordFilter";

        public RegistrySettingsProvider()
        {
        }

        public string StorePath
        {
            get => this.GetPolicyOrSetting<string>("Store", null);
            set => this.SetSetting<string>("Store", value);
        }


        private T GetPolicyOrSetting<T>(string name, T defaultValue)
        {
            using (var key = Registry.LocalMachine.OpenSubKey(REG_BASE_POLICY_KEY, false))
            {
                object result = key?.GetValue(name, null);

                if (result != null)
                {
                    return (T)result;
                }
            }

            using (var key = this.TryOpenWritableSettingsKey())
            {
                object result = key?.GetValue(name, null);

                if (result != null)
                {
                    return (T)result;
                }
            }

            return defaultValue;
        }

        private void SetSetting<T>(string name, T value)
        {
            using (var key = Registry.LocalMachine.CreateSubKey(REG_BASE_SETTINGS_KEY, true))
            {
                key.SetValue(name, value);
            }
        }
        private RegistryKey TryOpenWritableSettingsKey()
        {
            try
            {
                return Registry.LocalMachine.CreateSubKey(REG_BASE_SETTINGS_KEY, true);
            }
            catch (SecurityException)
            {
                return Registry.LocalMachine.OpenSubKey(REG_BASE_SETTINGS_KEY, false);
            }
        }
    }

}
