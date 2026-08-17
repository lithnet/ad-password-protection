using System;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public class HibpEtagMissingException : Exception
    {
        public HibpEtagMissingException(string hibpRange)
            : base($"The HIBP API response for range {hibpRange} did not include the required ETag header. Incremental synchronization cannot continue.")
        {
            this.HibpRange = hibpRange;
        }

        public string HibpRange { get; }
    }
}
