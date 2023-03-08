using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct ServerInfo101
    {
        public ServerPlatform PlatformId;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string Name;

        public int VersionMajor;

        public int VersionMinor;

        public ServerTypes Type;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string Comment;
    }
}
