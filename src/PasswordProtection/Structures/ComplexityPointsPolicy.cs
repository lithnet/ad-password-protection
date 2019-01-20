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
        public int PerCharacter;
        public int PerLower;
        public int PerNumber;
        public int PerSymbol;
        public int PerUpper;
        public int Required;
        public int UseOfLower;
        public int UseOfNumber;
        public int UseOfSymbol;
        public int UseOfUpper;
    }
}