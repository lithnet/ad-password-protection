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
        public bool CheckNormalizedPasswordNotInCompromisedPasswordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInCompromisedPasswordStoreOnSet;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedWordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckNormalizedPasswordNotInBannedWordStoreOnSet;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInCompromisedPasswordStoreOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInCompromisedPasswordStoreOnSet;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInHibpOnChange;

        [MarshalAs(UnmanagedType.I1)]
        public bool CheckPasswordNotInHibpOnSet;
    }
}