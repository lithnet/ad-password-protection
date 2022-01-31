using Lithnet.Licensing.Core;
using Microsoft.Extensions.Options;
using System;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class OptionsMonitorLicenseDataProvider : ILicenseDataProvider
    {
        private readonly IOptionsMonitor<LicensingOptions> options;

        public OptionsMonitorLicenseDataProvider(IOptionsMonitor<LicensingOptions> options)
        {
            this.options = options;
            this.options.OnChange((x, y) => this.OnLicenseDataChanged?.Invoke(this, new EventArgs()));
        }

        public void LicenseDataChanged()
        {
            this.OnLicenseDataChanged?.Invoke(this, new EventArgs());
        }

        public event EventHandler OnLicenseDataChanged;

        public string GetRawLicenseData()
        {
            try
            {
                string data = this.options.CurrentValue.Data;
                return string.IsNullOrWhiteSpace(data) ? EmbeddedResourceProvider.GetResourceString("license.dat") : data;
            }
            catch
            {
                return null;
            }
        }
    }
}
