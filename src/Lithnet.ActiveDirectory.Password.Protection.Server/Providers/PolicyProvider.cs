using System;
using System.Collections.Generic;
using System.Security.Principal;
using Lithnet.Security.Authorization;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PolicyProvider : IPolicyProvider
    {
        private LppLicenseManager licenseManager;

        private IAuthZContextProvider contextProvider;

        private IOptionsMonitor<PolicyOptions> policyOptions;

        private ILogger<PolicyProvider> logger;

        public PolicyProvider(LppLicenseManager licenseManager, IAuthZContextProvider contextProvider, IOptionsMonitor<PolicyOptions> policyOptions, ILogger<PolicyProvider> logger)
        {
            this.licenseManager = licenseManager;
            this.contextProvider = contextProvider;
            this.policyOptions = policyOptions;
            this.logger = logger;
        }

        public Policy GetPolicy(string username)
        {
            if (string.IsNullOrWhiteSpace(username) || !this.licenseManager.IsFeatureEnabled(ProductFeatures.UserPolicy))
            {
                return this.GetDefaultPolicy();
            }

            return this.GetUserPolicy(contextProvider.GetAuthZContext(username));
        }

        public Policy GetPolicy(SecurityIdentifier sid)
        {
            if (!this.licenseManager.IsFeatureEnabled(ProductFeatures.UserPolicy))
            {
                return this.GetDefaultPolicy();
            }

            return this.GetUserPolicy(contextProvider.GetAuthZContext(sid));
        }

        private Policy GetDefaultPolicy()
        {
            return this.policyOptions.CurrentValue.Default;
        }

        private Policy GetUserPolicy(AuthorizationContext context)
        {
            foreach (var policy in policyOptions.CurrentValue.FineGrainedPolicies)
            {
                var sid = policy.TargetSid;

                if (sid == null)
                {
                    logger.LogWarning("Policy {policyId} has an invalid SID {sid}", policy.Id, policy.Target);
                    continue;
                }

                if (sid == context.SecurityIdentifer || context.ContainsSid(sid))
                {
                    logger.LogInformation("Matched user {user} to policy {policyId}", context.SecurityIdentifer, policy.Id);
                    return policy.Policy;
                }
            }

            logger.LogInformation("Could not match user {user} to a fine-grained policy. The default policy will be used instead", context.SecurityIdentifer);

            return this.GetDefaultPolicy();
        }
    }
}
