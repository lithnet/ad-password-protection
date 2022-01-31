using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PolicyOptions
    {
        public Policy Default { get; set; } = new Policy();

        public List<FineGrainedPolicy> FineGrainedPolicies { get; set; } = new List<FineGrainedPolicy>();
    }
}
