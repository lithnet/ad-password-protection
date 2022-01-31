using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Providers
{
    public class PasswordValidatorRepeatingCharacters : IPasswordValidator
    {
        private ILogger logger;

        public PasswordValidatorRepeatingCharacters(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.MaximumRepeatedCharacters > 0)
            {
                int dupCount = 0;
                for (int i = 0; i < request.Password.Length - 1; i++)
                {
                    if (request.Password[i] == request.Password[i + 1])
                    {
                        dupCount++;
                    }
                    else
                    {
                        dupCount = 0;
                    }

                    if (dupCount >= policy.MaximumRepeatedCharacters)
                    {
                        return new PasswordValidationResult
                        {
                            Result = PasswordTestResult.RepeatedCharacters
                        };
                    }
                }
            }

            return new PasswordValidationResult();

        }

        public int Weight => 15;
    }
}
