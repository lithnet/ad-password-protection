using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Xml;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ApplicationConfig : IApplicationConfig
    {
        [JsonIgnore]
        public string Path { get; private set; }

        [JsonIgnore]
        public string Hash { get; private set; }

        public ConfigurationMetadata Metadata { get; set; } = new ConfigurationMetadata();

        public LicensingOptions Licensing { get; set; } = new LicensingOptions();

        //public AuthenticationOptions Authentication { get; set; } = new AuthenticationOptions();

        public EmailOptions Email { get; set; } = new EmailOptions();

        public AdminNotificationOptions AdminNotifications { get; set; } = new AdminNotificationOptions();

        public RateLimitOptions RateLimits { get; set; } = new RateLimitOptions();

        public ForwardedHeadersAppOptions ForwardedHeaders { get; set; } = new ForwardedHeadersAppOptions();

        [JsonExtensionData]
        public IDictionary<string, object> OtherData { get; set; }

        public void Save(string file, bool forceOverwrite)
        {
            if (!forceOverwrite)
            {
                if (this.HasFileBeenModified())
                {
                    throw new ConfigurationModifiedException("The configuration file has been modified outside of this editor session");
                }
            }

            JsonSerializerOptions settings = new JsonSerializerOptions
            {
                WriteIndented = true,
                IgnoreNullValues = true
            };

            this.Metadata.Usn++;

            string data = JsonSerializer.Serialize(this, settings);
            File.WriteAllText(file, data);
            this.Hash = ApplicationConfig.BuildHash(file);
        }

        public bool HasFileBeenModified()
        {
            var existingFile = ApplicationConfig.Load(this.Path);
            return existingFile.Hash != this.Hash;
        }

        public static IApplicationConfig Load(string file)
        {
            string data = File.ReadAllText(file);
            var result = JsonSerializer.Deserialize<ApplicationConfig>(data);
            result.Path = file;
            result.Metadata.ValidateMetadata();
            result.Hash = ApplicationConfig.BuildHash(file);

            return result;
        }

        private static string BuildHash(string file)
        {
            using (var hash = SHA1.Create())
            {
                using (var stream = File.OpenRead(file))
                {
                    return hash.ComputeHash(stream).ToHexString();
                }
            }
        }
    }
}