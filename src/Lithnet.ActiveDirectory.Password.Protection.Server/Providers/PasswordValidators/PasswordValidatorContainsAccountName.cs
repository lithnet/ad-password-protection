using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorContainsAccountName : IPasswordValidator
    {
        private readonly ILogger logger;

        public PasswordValidatorContainsAccountName(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.PreventUseOfUsername)
            {
                string username = request.Username;

                if (username.Contains('\\'))
                {
                    username = username.Split('\\')[1];
                }

                if (username.Contains('@'))
                {
                    username = username.Split('@')[0];
                }

                if (username.Length > 3)
                {
                    logger.LogTrace("Checking to see if the password contains any component of the account name");

                    if (request.Password.Contains(username, StringComparison.OrdinalIgnoreCase))
                    {
                        logger.LogWarning("User {user} attempted to set a password that contained their account name", request.Username);
                        return new PasswordValidationResult
                        {
                            Result = PasswordTestResult.ContainsAccountName
                        };
                    }
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 10;
    }
}
