namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class RateLimitOptions
    {
        public RateLimitThresholds PerIP { get; set; } = new RateLimitThresholds();

        public RateLimitThresholds PerUser { get; set; } = new RateLimitThresholds();
    }
}