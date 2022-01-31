using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidatorBannedWord : IPasswordValidator
    {
        private readonly ILogger logger;

        private readonly IStore store;

        public PasswordValidatorBannedWord(ILogger logger, IStoreProvider store)
        {
            this.logger = logger;
            this.store = store.Store;
        }

        public PasswordValidationResult ValidatePassword(PasswordValidationRequest request, Policy policy)
        {
            if (policy.NormalizedBannedWordChecking)
            {
                if (store.IsInStore(StringNormalizer.Normalize(request.Password), StoreType.Word))
                {
                    return new PasswordValidationResult
                    {
                        Result = PasswordTestResult.BannedNormalizedWord
                    };
                }
            }

            return new PasswordValidationResult();
        }

        public int Weight => 20;
    }
}
