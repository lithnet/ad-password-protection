using System;
using System.Runtime.InteropServices;
using System.Security;
using System.Threading.Tasks;
using Microsoft.Win32;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    internal static class Global
    {
        public static Store Store { get; set; }

        public static void OpenStore(string path)
        {
            Store = new V3Store(path);
        }

        public static void OpenStore()
        {
            Settings settings = new Settings();

            var storePath = settings.StorePath ?? GetLastOpenStorePath();

            if (storePath == null)
            {
                throw new StoreNotOpenException("The store path was not found in the registry. Open the store manually using the Open-Store cmdlet and specifying the path to the store");
            }

            OpenStore(storePath);

            SetLastOpenStorePath(storePath);
        }

        private static string GetLastOpenStorePath()
        {
            RegistryKey baseKey = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Registry64);
            return baseKey.OpenSubKey("Software\\Lithnet\\PasswordFilter", false)?.GetValue("LastOpenStore", null) as string;
        }

        private static void SetLastOpenStorePath(string path)
        {
            RegistryKey baseKey = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Registry64);
            baseKey.CreateSubKey("Software\\Lithnet\\PasswordFilter", true).SetValue("LastOpenStore", path);
        }

        public static void OpenExistingDefaultOrThrow()
        {
            if (IsOpen)
            {
                return;
            }

            OpenStore();
        }

        public static bool IsOpen => Store != null;

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
