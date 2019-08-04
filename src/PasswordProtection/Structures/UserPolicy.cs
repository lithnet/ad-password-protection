using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [StructLayout(LayoutKind.Sequential)]
    public struct UserPolicy
    {
        [MarshalAs(UnmanagedType.I4)]
        public int Version;

        public GeneralPolicy GeneralPolicy;

        public StorePolicy StorePolicy;

        public ComplexityPointsPolicy ComplexityPoints;

        public ComplexityThresholdPolicy ComplexityThreshold1;

        public ComplexityThresholdPolicy ComplexityThreshold2;

        public ComplexityThresholdPolicy ComplexityThreshold3;
    }
}
