using System.Collections.Generic;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IApplicationConfig
    {
        string Path { get; }
        string Hash { get; }
        ConfigurationMetadata Metadata { get; set; }
        LicensingOptions Licensing { get; set; }
        EmailOptions Email { get; set; }
        AdminNotificationOptions AdminNotifications { get; set; }
        RateLimitOptions RateLimits { get; set; }
        ForwardedHeadersAppOptions ForwardedHeaders { get; set; }
        IDictionary<string, object> OtherData { get; set; }
        void Save(string file, bool forceOverwrite);
        bool HasFileBeenModified();
    }
}