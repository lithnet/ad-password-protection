using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidationResult
    {
        public PasswordTestResult Result { get; set; }

        public object FailureContext { get; set; }
    }
}
