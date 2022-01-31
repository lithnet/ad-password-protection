using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorCompromised : IPasswordValidator
    {
        private readonly ILogger logger;

        private readonly IStore store;

        public PasswordValidatorCompromised(ILogger logger, IStoreProvider store)
        {
            this.logger = logger;
            this.store = store.Store;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.CompromisedPasswordChecking)
            {
                if (store.IsInStore(request.Password, StoreType.Password))
                {
                    return new PasswordValidationResult
                    {
                        Result = PasswordTestResult.Compromised
                    };
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 20;
    }
}
