using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public static class NativeMethods
    {
        [DllImport("Netapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern int NetServerGetInfo(string serverName, int level, out IntPtr pServerInfo);

        [DllImport("NetApi32.dll")]
        private static extern int NetApiBufferFree(IntPtr buffer);

        public static bool IsDc()
        {
            var info = GetServerInfo();
            return info.Type.HasFlag(ServerTypes.DomainCtrl) || info.Type.HasFlag(ServerTypes.BackupDomainCtrl);
        }

        internal static ServerInfo101 GetServerInfo()
        {
            return GetServerInfo(null);
        }

        internal static ServerInfo101 GetServerInfo(string server)
        {
            IntPtr pServerInfo = IntPtr.Zero;

            try
            {
                int result = NetServerGetInfo(server, 101, out pServerInfo);

                if (result != 0)
                {
                    throw new Win32Exception(result);
                }

                var info = Marshal.PtrToStructure<ServerInfo101>(pServerInfo);

                return info;
            }
            finally
            {
                if (pServerInfo != IntPtr.Zero)
                {
                    _ = NetApiBufferFree(pServerInfo);
                }
            }
        }
    }
}
