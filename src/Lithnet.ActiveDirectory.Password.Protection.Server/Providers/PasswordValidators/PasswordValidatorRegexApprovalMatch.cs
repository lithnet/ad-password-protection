using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorRegexApprovalMatch : IPasswordValidator
    {
        private readonly ILogger logger;

        public PasswordValidatorRegexApprovalMatch(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.RegexApprovedCompiled.Count == 0)
            {
                return new PasswordValidationResult();
            }

            foreach (var regex in policy.RegexApprovedCompiled)
            {
                if (regex.IsMatch(request.Password))
                {
                    return new PasswordValidationResult();
                }
            }

            logger.LogWarning("User {user} attempted to set a password that did not match any of the approval regular expressions", request.Username);
            return new PasswordValidationResult
            {
                Result = PasswordTestResult.DidNotMatchApprovalRegex
            };
        }

        public int Weight => 20;
    }
}
