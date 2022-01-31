using System;
using System.Collections.Generic;
using System.Security.Principal;
using System.Text;
using System.Text.Json.Serialization;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class FineGrainedPolicy
    {
        private bool sidParsed;

        private SecurityIdentifier sid;

        public string Id { get; set; }

        public bool Disabled { get; set; }

        public string Target { get; set; }

        public string Description { get; set; }

        public Policy Policy { get; set; }

        public string LastModifiedBy { get; set; }

        public DateTime? LastModified { get; set; }

        [JsonIgnore]
        public SecurityIdentifier TargetSid
        {
            get
            {
                if (!sidParsed)
                {
                    try
                    {
                        sid = new SecurityIdentifier(this.Target);
                    }
                    catch
                    {
                        // Ignore
                    }

                    sidParsed = true;
                }

                return sid;
            }
        }
    }
}
