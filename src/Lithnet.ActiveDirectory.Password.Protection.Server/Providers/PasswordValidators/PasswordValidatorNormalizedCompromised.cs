using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorNormalizedCompromised : IPasswordValidator
    {
        private readonly ILogger logger;

        private readonly IStore store;

        public PasswordValidatorNormalizedCompromised(ILogger logger, IStoreProvider store)
        {
            this.logger = logger;
            this.store = store.Store;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.NormalizedCompromisedPasswordChecking)
            {
                if (store.IsInStore(StringNormalizer.Normalize(request.Password), StoreType.Password))
                {
                    return new PasswordValidationResult
                    {
                        Result = PasswordTestResult.CompromisedNormalizedPassword
                    };
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 20;
    }
}
