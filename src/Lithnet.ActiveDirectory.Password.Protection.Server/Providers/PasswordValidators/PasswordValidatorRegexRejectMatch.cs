using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorRegexRejectMatch : IPasswordValidator
    {
        private readonly ILogger logger;

        public PasswordValidatorRegexRejectMatch(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.RegexRejectCompiled.Count == 0)
            {
                return new PasswordValidationResult();
            }

            foreach (var regex in policy.RegexRejectCompiled)
            {
                if (regex.IsMatch(request.Password))
                {
                    logger.LogWarning("User {user} attempted to set a password that matched a rejection regular expression", request.Username);

                    return new PasswordValidationResult
                    {
                        Result = PasswordTestResult.MatchedRejectRegex
                    };
                }
            }
            
            return new PasswordValidationResult();
        }

        public int Weight => 20;
    }
}
