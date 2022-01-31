using Lithnet.ActiveDirectory.PasswordProtection.Server;
using Lithnet.Licensing.Core;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using System;
using System.DirectoryServices.ActiveDirectory;
using Lithnet.ActiveDirectory.PasswordProtection.Server.Providers;

namespace Lithnet.ActiveDirectory.PasswordProtection.Service
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers();

            services.TryAddSingleton<ILicenseDataProvider, OptionsMonitorLicenseDataProvider>();
            services.TryAddSingleton<IPolicyProvider, PolicyProvider>();
            services.TryAddSingleton<IPasswordValidationProvider, PasswordValidationProvider>();
            services.TryAddSingleton<IAuthZContextProvider, AuthZContextProvider>();

            services.Configure<ConfigurationMetadata>(Configuration.GetSection("Metadata"));
            services.Configure<RateLimitOptions>(Configuration.GetSection("RateLimits"));
            services.Configure<EmailOptions>(Configuration.GetSection("Email"));
            services.Configure<ForwardedHeadersAppOptions>(Configuration.GetSection("ForwardedHeaders"));
            services.Configure<LicensingOptions>(Configuration.GetSection("Licensing"));
            services.Configure<AdminNotificationOptions>(Configuration.GetSection("AdminNotifications"));
            services.Configure<PolicyOptions>(Configuration.GetSection("Policies"));

            LppLicenseManager licenseManager = this.CreateLicenseManager(services);
            services.AddSingleton(licenseManager);
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseHttpsRedirection();

            app.UseRouting();

            app.UseAuthorization();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllers();
            });
        }

        private LppLicenseManager CreateLicenseManager(IServiceCollection services)
        {
            var provider = services.BuildServiceProvider();
            var licenseDataProvider = provider.GetService<ILicenseDataProvider>();
            var logger = provider.GetService<ILogger<Startup>>();
            LppLicenseManager licenseManager = new LppLicenseManager(licenseDataProvider, Forest.GetCurrentForest().Name);

            try
            {
                var license = licenseManager.GetLicense();
                if (license != null)
                {
                    logger.LogTrace("License information\r\n{licenseData}", license.ToString());
                }
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "An error occurred performing the license check. Enterprise edition features will not be available");
            }

            if (licenseManager.IsSpecificSku(ProductSkus.Enterprise))
            {
                logger.LogInformation("Starting Lithnet Password Protection Enterprise Edition");
            }
            else
            {
                logger.LogInformation("Starting Lithnet Password Protection Standard Edition");
            }

            return licenseManager;
        }
    }
}
