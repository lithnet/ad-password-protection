using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ConfigurationMetadata
    {
        public static readonly Version SupportedConfigSchemaVersion = new Version("1.0");

        public int Usn { get; set; } = 0;

        public Version SchemaVersion { get; set; } = new Version("1.0");

        public void ValidateMetadata()
        {
            if (SupportedConfigSchemaVersion.Major < this.SchemaVersion.Major)
            {
                throw new ConfigurationException("The configuration file version is not supported by this application. Please upgrade the application to use this configuration file");
            }
        }
    }
}
