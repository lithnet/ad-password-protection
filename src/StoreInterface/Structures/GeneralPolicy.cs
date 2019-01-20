using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct GeneralPolicy
    {
        public int MinimumLength;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string RegexApprove;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string RegexReject;

        [MarshalAs(UnmanagedType.I1)]
        public bool ValidatePasswordDoesntContainAccountName;

        [MarshalAs(UnmanagedType.I1)]
        public bool ValidatePasswordDoesntContainFullName;
    }
}