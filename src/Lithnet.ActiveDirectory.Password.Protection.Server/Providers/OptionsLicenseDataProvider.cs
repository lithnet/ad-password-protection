using Lithnet.Licensing.Core;
using System;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class OptionsLicenseDataProvider : ILicenseDataProvider
    {
        private readonly LicensingOptions options;

        public OptionsLicenseDataProvider(LicensingOptions options)
        {
            this.options = options;
        }

        public string GetRawLicenseData()
        {
            try
            {
                string data = this.options.Data;
                return string.IsNullOrWhiteSpace(data) ? EmbeddedResourceProvider.GetResourceString("license.dat") : data;
            }
            catch
            {
                return null;
            }
        }

        public void LicenseDataChanged()
        {
            this.OnLicenseDataChanged?.Invoke(this, new EventArgs());
        }

        public event EventHandler OnLicenseDataChanged;
    }
}