using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Providers
{
    public class PasswordValidatorBlankPassword : IPasswordValidator
    {
        private ILogger logger;

        public PasswordValidatorBlankPassword(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (string.IsNullOrWhiteSpace(request.Password))
            {
                logger.LogWarning("The user {user} attempted to set a empty password", request.Username);
                return new PasswordValidationResult { Result = PasswordTestResult.PasswordWasBlank };
            }

            return new PasswordValidationResult();
        }

        public int Weight => 0;
    }
}
