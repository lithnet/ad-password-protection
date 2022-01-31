using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Providers
{
    public class PasswordValidatorLength : IPasswordValidator
    {
        private ILogger logger;

        public PasswordValidatorLength(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (request.Password.Length >= policy.MinimumLength)
            {
                logger.LogTrace("The password met the minimum length requirements");
                return new PasswordValidationResult();
            }
            else
            {
                logger.LogWarning("The user {user} attempted to set a password that did not meet the length requirements. Required length was {requiredLength}. Actual length {actualLength}", request.Username, policy.MinimumLength, request.Password.Length);
                
                return new PasswordValidationResult
                {
                    Result = PasswordTestResult.LengthRequirementsNotMet,
                };
            }
        }

        public int Weight => 1;
    }
}
