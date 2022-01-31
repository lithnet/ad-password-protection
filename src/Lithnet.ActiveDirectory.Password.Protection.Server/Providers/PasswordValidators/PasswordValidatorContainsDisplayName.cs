using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorContainsDisplayName : IPasswordValidator
    {
        private readonly ILogger logger;

        public PasswordValidatorContainsDisplayName(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (!policy.PreventUseOfFullName)
            {
                return new PasswordValidationResult();
            }

            string name = request.FullName;

            if (name.Length <= 3)
            {
                return new PasswordValidationResult();
            }

            this.logger.LogTrace("Checking to see if the password contains any component of the display name");

            foreach (var namePart in name.Split(' '))
            {
                if (namePart.Length > 3)
                {
                    if (request.Password.Contains(namePart, StringComparison.OrdinalIgnoreCase))
                    {
                        this.logger.LogWarning("User {user} attempted to set a password that contained part of their full name", request.Username);
                        return new PasswordValidationResult
                        {
                            Result = PasswordTestResult.ContainsFullName
                        };
                    }
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 11;
    }
}
