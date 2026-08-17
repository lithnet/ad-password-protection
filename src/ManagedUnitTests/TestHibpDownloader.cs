using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;
using Lithnet.ActiveDirectory.PasswordProtection;

namespace ManagedUnitTests
{
    internal class TestHibpDownloader : HibpDownloader
    {
        private readonly Func<string, string, HttpResponseMessage> responseFactory;

        public TestHibpDownloader(Store store, Func<string, string, HttpResponseMessage> responseFactory)
            : base(store)
        {
            this.responseFactory = responseFactory;
            this.RequestEtags = new Dictionary<string, string>();
        }

        public Dictionary<string, string> RequestEtags { get; }

        protected override Task<HttpResponseMessage> GetRangeFromApiAsync(string range, CancellationToken ct, string etag = null)
        {
            this.RequestEtags[range] = etag;
            return Task.FromResult(this.responseFactory(range, etag));
        }
    }
}
