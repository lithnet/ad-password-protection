using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential)]
    public struct StorePolicy
    {
        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedPasswordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedPasswordStoreOnSet;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedWordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedWordStoreOnSet;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInBannedPasswordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInBannedPasswordStoreOnSet;
    }
}