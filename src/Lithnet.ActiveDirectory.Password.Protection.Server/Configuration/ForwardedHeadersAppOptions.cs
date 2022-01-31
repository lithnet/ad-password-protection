using System.Collections.Generic;
using System.Net;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.HttpOverrides;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class ForwardedHeadersAppOptions
    {
        private static readonly ForwardedHeadersOptions defaultOptions = new ForwardedHeadersOptions();

        public string ForwardedForHeaderName { get; set; } = defaultOptions.ForwardedForHeaderName;

        public string ForwardedHostHeaderName { get; set; } = defaultOptions.ForwardedHostHeaderName;

        public string ForwardedProtoHeaderName { get; set; } = defaultOptions.ForwardedProtoHeaderName;

        public string OriginalForHeaderName { get; set; } = defaultOptions.OriginalForHeaderName;

        public string OriginalHostHeaderName { get; set; } = defaultOptions.OriginalHostHeaderName;

        public string OriginalProtoHeaderName { get; set; } = defaultOptions.OriginalProtoHeaderName;

        [JsonConverter(typeof(JsonStringEnumConverter))]
        public ForwardedHeaders ForwardedHeaders { get; set; } = defaultOptions.ForwardedHeaders;

        public int? ForwardLimit { get; set; } = defaultOptions.ForwardLimit;

        public IList<string> AllowedHosts { get; set; } =  new List<string>(defaultOptions.AllowedHosts);

        public bool RequireHeaderSymmetry { get; set; } = defaultOptions.RequireHeaderSymmetry;

        public IList<string> KnownProxies { get; set; }

        public IList<string> KnownNetworks { get; set; }

        public ForwardedHeadersOptions ToNativeOptions()
        {
            var f = new ForwardedHeadersOptions
            {
                AllowedHosts = this.AllowedHosts,
                ForwardedForHeaderName = this.ForwardedForHeaderName,
                ForwardedHeaders = this.ForwardedHeaders,
                ForwardedHostHeaderName = this.ForwardedHostHeaderName,
                ForwardedProtoHeaderName = this.ForwardedProtoHeaderName,
                ForwardLimit = this.ForwardLimit,
                OriginalForHeaderName = this.OriginalForHeaderName,
                OriginalHostHeaderName = this.OriginalHostHeaderName,
                OriginalProtoHeaderName = this.OriginalProtoHeaderName,
                RequireHeaderSymmetry = this.RequireHeaderSymmetry
            };

            this.AddKnownNetworks(f);
            this.AddKnownProxies(f);

            return f;
        }

        private void AddKnownProxies(ForwardedHeadersOptions o)
        {
            if (this.KnownProxies == null)
            {
                return;
            }

            o.KnownProxies.Clear();

            foreach (var s in this.KnownProxies)
            {
                o.KnownProxies.Add(IPAddress.Parse(s));
            }
        }


        private void AddKnownNetworks(ForwardedHeadersOptions o)
        {
            if (this.KnownNetworks == null)
            {
                return;
            }

            o.KnownNetworks.Clear();

            foreach (var s in this.KnownNetworks)
            {
                var items = s.Split("/");

                if (items.Length == 2)
                {
                    o.KnownNetworks.Add(new IPNetwork(IPAddress.Parse(items[0]), int.Parse(items[1])));
                }
            }
        }
    }
}