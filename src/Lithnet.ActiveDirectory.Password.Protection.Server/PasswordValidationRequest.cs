using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidationRequest
    {
        public string Username { get; set; }

        public string Password { get; set; }

        public string FullName { get; set; }

        public bool IsSet { get; set; }
    }
}
