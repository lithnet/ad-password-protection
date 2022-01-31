using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Security.Policy;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ProtectedSecret
    {
        public string Data { get; set; }

        public string Salt { get; set; }

        public int Mode { get; set; } = 0;
    }
}
