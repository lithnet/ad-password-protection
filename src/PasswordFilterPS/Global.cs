using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StoreInterface;
using Microsoft.Win32;

namespace PasswordFilterPS
{
    internal static class Global
    {
        public static Store Store { get; set; }

        public static void OpenStore(string path)
        {
            Global.Store = new V2Store(path);
        }

        public static void OpenStore()
        {
            RegistryKey hklm = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64);
            RegistryKey appKey = hklm.OpenSubKey("Software\\Policies\\Lithnet\\PasswordFilter");
            string storePath = (string)appKey?.GetValue("Store", null);

            if (storePath == null)
            {
                appKey = hklm.OpenSubKey("Software\\Lithnet\\PasswordFilter");
                storePath = (string)appKey?.GetValue("Store", null);
            }

            if (storePath == null)
            {
                throw new StoreNotOpenException("The store path was not found in the registry. Open the store manually using the Open-Store cmdlet and specifying the path to the store");
            }

            OpenStore(storePath);
        }

        public static void OpenExistingDefaultOrThrow()
        {
            if (Global.IsOpen)
            {
                return;
            }

            Global.OpenStore();
        }

        public static bool IsOpen => Global.Store != null;
    }
}
