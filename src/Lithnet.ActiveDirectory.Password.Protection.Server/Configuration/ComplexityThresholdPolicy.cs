using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ComplexityThresholdPolicy
    {
        public int Threshold { get; set; }

        public int CharsetsRequired { get; set; }

        public bool RequiresLower { get; set; }

        public bool RequiresNumber { get; set; }

        public bool RequiresSymbol { get; set; }

        public bool RequiresSymbolOrNumber { get; set; }

        public bool RequiresUpper { get; set; }
    }
}