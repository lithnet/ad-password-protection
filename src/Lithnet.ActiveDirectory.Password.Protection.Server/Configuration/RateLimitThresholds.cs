namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class RateLimitThresholds 
    {
        public bool Enabled { get; set; } = true;

        public int RequestsPerMinute { get; set; } = 10;

        public int RequestsPerHour { get; set; } = 50;

        public int RequestsPerDay { get; set; } = 100;
    }
}