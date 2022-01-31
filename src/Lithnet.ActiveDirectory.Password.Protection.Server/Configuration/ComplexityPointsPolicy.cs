using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ComplexityPointsPolicy
    {
        public bool Enabled { get; set; }

        public int PerCharacter { get; set; }

        public int PerLower { get; set; }

        public int PerNumber { get; set; }

        public int PerSymbol { get; set; }

        public int PerUpper { get; set; }

        public int Required { get; set; }

        public int UseOfLower { get; set; }

        public int UseOfNumber { get; set; }

        public int UseOfSymbol { get; set; }

        public int UseOfUpper { get; set; }
    }
}