using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using Lithnet.ActiveDirectory.PasswordProtection;
using Microsoft.Win32;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    internal static class Global
    {
        public static Store Store { get; set; }

        public static void OpenStore(string path)
        {
            Global.Store = new V3Store(path);
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

        public static string SecureStringToString(this SecureString value)
        {
            IntPtr valuePtr = IntPtr.Zero;

            try
            {
                valuePtr = Marshal.SecureStringToGlobalAllocUnicode(value);
                return Marshal.PtrToStringUni(valuePtr);
            }
            finally
            {
                Marshal.ZeroFreeGlobalAllocUnicode(valuePtr);
            }
        }

        internal static void ThrowIfFaulted(this Task t)
        {
            if (t.IsFaulted)
            {
                if (t.Exception == null)
                {
                    throw new InvalidOperationException("A faulted task had not exception details");
                }

                UnwrapAndThrow(t.Exception);
            }
        }

        internal static void UnwrapAndThrow(Exception ex)
        {
            if (ex is AggregateException e)
            {
                if (e.InnerExceptions.Count == 1)
                {
                    throw e.InnerExceptions[0];
                }
            }

            throw ex;
        }
    }
}
