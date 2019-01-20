using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ComplexityThresholdPolicy
    {
        [MarshalAs(UnmanagedType.I4)]
        public int ComplexityThreshold;

        [MarshalAs(UnmanagedType.I4)]
        public int ComplexityThresholdCharsetsRequired;

        [MarshalAs(UnmanagedType.I1)]
        public bool ComplexityThresholdRequiresLower;

        [MarshalAs(UnmanagedType.I1)]
        public bool ComplexityThresholdRequiresNumber;

        [MarshalAs(UnmanagedType.I1)]
        public bool ComplexityThresholdRequiresSymbol;

        [MarshalAs(UnmanagedType.I1)]
        public bool ComplexityThresholdRequiresSymbolOrNumber;

        [MarshalAs(UnmanagedType.I1)]
        public bool ComplexityThresholdRequiresUpper;
    }
}