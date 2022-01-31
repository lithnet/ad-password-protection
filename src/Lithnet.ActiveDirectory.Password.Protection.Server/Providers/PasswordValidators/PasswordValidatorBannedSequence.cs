using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorBannedSequence : IPasswordValidator
    {
        private readonly ILogger logger;

        public PasswordValidatorBannedSequence(ILogger logger)
        {
            this.logger = logger;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.BannedSequences?.Count > 0)
            {
                foreach (var s in policy.BannedSequences)
                {
                    if (request.Password.Contains(s, StringComparison.OrdinalIgnoreCase))
                    {
                        return new PasswordValidationResult { Result = PasswordTestResult.BannedSequence };
                    }
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 200;
    }
}
