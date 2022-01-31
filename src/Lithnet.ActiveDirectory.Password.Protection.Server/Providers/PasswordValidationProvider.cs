using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Providers
{
    public class PasswordValidationProvider : IPasswordValidationProvider
    {
        private readonly IPolicyProvider policyProvider;

        private readonly ILogger<PasswordValidationProvider> logger;

        private List<IPasswordValidator> validators;

        public PasswordValidationProvider(IPolicyProvider policyProvider, ILogger<PasswordValidationProvider> logger)
        {
            this.policyProvider = policyProvider;
            this.logger = logger;

            validators = new IPasswordValidator[]
            {
                new PasswordValidatorLength(logger),
                new PasswordValidatorBlankPassword(logger),
                new PasswordValidatorContainsAccountName(logger),
            }.OrderBy(t => t.Weight).ToList();
        }

        public PasswordValidationResponse ValidatePassword(PasswordValidationRequest request)
        {
            PasswordValidationResponse response = new PasswordValidationResponse();

            var policy = this.policyProvider.GetPolicy(request.Username);

            foreach (IPasswordValidator validator in this.validators)
            {
                var result = validator.ValidatePassword(request, policy);

                if (result.Result != PasswordTestResult.Approved)
                {
                    response.Result = result.Result;
                    response.Context = result.FailureContext;
                }
            }

            return response;
        }
    }
}
