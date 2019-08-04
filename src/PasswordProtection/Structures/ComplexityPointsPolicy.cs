using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ComplexityPointsPolicy
    {
        [MarshalAs(UnmanagedType.I4)]
        public int PerCharacter;

        [MarshalAs(UnmanagedType.I4)]
        public int PerLower;

        [MarshalAs(UnmanagedType.I4)]
        public int PerNumber;

        [MarshalAs(UnmanagedType.I4)]
        public int PerSymbol;

        [MarshalAs(UnmanagedType.I4)]
        public int PerUpper;

        [MarshalAs(UnmanagedType.I4)]
        public int Required;

        [MarshalAs(UnmanagedType.I4)]
        public int UseOfLower;

        [MarshalAs(UnmanagedType.I4)]
        public int UseOfNumber;

        [MarshalAs(UnmanagedType.I4)]
        public int UseOfSymbol;

        [MarshalAs(UnmanagedType.I4)]
        public int UseOfUpper;
    }
}