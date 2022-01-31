using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IPasswordValidator
    {
        PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy);

        int Weight { get; }
    }
}
