using System;
using System.Collections.Generic;
using System.Text;
using Lithnet.Licensing.Core;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class LppLicenseManager : LicenseManager<ProductFeatures, ProductSkus>
    {
        public const string LppProductId = "ED6BC89C-74FA-4581-BD09-256B2050B254";

        public LppLicenseManager(ILicenseDataProvider dataProvider, string audience)
            : base(dataProvider, audience, LppProductId)
        {
        }
    }
}
