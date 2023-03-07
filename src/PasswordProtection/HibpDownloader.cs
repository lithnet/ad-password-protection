using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Polly;
using Polly.Extensions.Http;
using Polly.Retry;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public class HibpDownloader
    {
        private static readonly Version AssemblyVersion = Assembly.GetExecutingAssembly().GetName().Version;

        private const string MetadataItemName = "hibp-state";
        private const int totalHashes = 0xFFFFF;

        private readonly Store store;
        private readonly HttpClient httpClient;
        private readonly AsyncRetryPolicy<HttpResponseMessage> policy;

        private ConcurrentDictionary<string, string> hibpState;
        private int hashCounter = -1;
        private int writeLock = 0;

        public HibpDownloader(Store store)
        {
            this.store = store;
            this.httpClient = this.InitializeHttpClient();
            this.policy = HttpPolicyExtensions.HandleTransientHttpError().RetryAsync(10, OnRequestError);
        }

        public void DeleteSavedState()
        {
            this.store.DeleteStoreMetadata(MetadataItemName);
        }

        private void LoadHibpState(int expectedThreadCount)
        {
            this.hibpState = new ConcurrentDictionary<string, string>(expectedThreadCount, totalHashes);

            var indexMetadata = this.store.GetStoreMetadata(MetadataItemName);
            if (indexMetadata != null)
            {
                var splitLines = indexMetadata.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

                foreach (var line in splitLines)
                {
                    var items = line.Split(new string[] { ":" }, StringSplitOptions.RemoveEmptyEntries);
                    if (items.Length == 2)
                    {
                        this.hibpState.TryAdd(items[0], items[1]);
                    }
                }
            }
        }

        private void WriteHibpState()
        {
            if (Interlocked.CompareExchange(ref this.writeLock, 1, 0) == 1)
            {
                return;
            }

            try
            {
                StringBuilder builder = new StringBuilder();

                foreach (var item in this.hibpState.ToArray())
                {
                    builder.Append(item.Key).Append(":").AppendLine(item.Value);
                }

                this.store.SetStoreMetadata(MetadataItemName, builder.ToString());
            }
            finally
            {
                this.writeLock = 0;
            }
        }

        private HttpClient InitializeHttpClient()
        {
            var handler = new HttpClientHandler();

            if (handler.SupportsAutomaticDecompression)
            {
                handler.AutomaticDecompression = DecompressionMethods.GZip;
            }

            handler.SslProtocols = System.Security.Authentication.SslProtocols.Tls12;
            handler.MaxConnectionsPerServer = 256;

            HttpClient client = new HttpClient(handler);
            client.BaseAddress = new Uri("https://api.pwnedpasswords.com/range/");
            client.DefaultRequestHeaders.UserAgent.Add(new ProductInfoHeaderValue("LithnetPasswordProtection", AssemblyVersion.ToString()));

            return client;
        }

        private async Task<HttpResponseMessage> GetRangeFromApi(string range, CancellationToken ct, string etag = null)
        {
            string requestUri = range + "?mode=ntlm";

            return await this.policy.ExecuteAsync(async () =>
            {
                var request = new HttpRequestMessage(HttpMethod.Get, requestUri);

                if (etag != null)
                {
                    request.Headers.Add("If-None-Match", etag);
                }

                return await this.httpClient.SendAsync(request, ct);
            }).ConfigureAwait(false);
        }

        private async Task ProcessRanges(OperationProgress progress, int threads, CancellationToken ct)
        {
            progress.HibpReadInProgress = true;
            progress.HibpHashTotal = totalHashes;
            progress.HibpStartTime = DateTime.Now;

            Task[] downloadTasks = new Task[threads];

            for (int i = 0; i < downloadTasks.Length; i++)
            {
                downloadTasks[i] = this.DownloadRanges(progress, ct);
            }

            await Task.WhenAll(downloadTasks).ConfigureAwait(false);
        }

        private async Task DownloadRanges(OperationProgress progress, CancellationToken ct)
        {
            int currentHash = Interlocked.Increment(ref this.hashCounter);

            while (currentHash < totalHashes && !ct.IsCancellationRequested)
            {
                string range = currentHash.ToHexPrefixString();
                Trace.WriteLine($"Hash GET {range} {currentHash}/{totalHashes}");
                progress.IncrementCurrentHash();

                this.hibpState.TryGetValue(range, out string etag);

                var response = await this.GetRangeFromApi(range, ct, etag).ConfigureAwait(false);

                if (response.StatusCode == HttpStatusCode.NotModified)
                {
                    progress.IncrementUnchangedRange();
                }
                else
                {
                    response.EnsureSuccessStatusCode();
                    progress.IncrementChangedRange();

                    this.hibpState[range] = response.Headers.ETag.ToString();
                    var lines = await response.Content.ReadAsStringAsync();

                    Store.ImportHexHashesFromString(this.store, lines, range, StoreType.Password, progress);
                }

                if (currentHash % 10000 == 0)
                {
                    this.WriteHibpState();
                }

                currentHash = Interlocked.Increment(ref this.hashCounter);
            }
        }

        public void Execute(OperationProgress progress, int threads, CancellationToken ct)
        {
            if (threads <= 0)
            {
                threads = Environment.ProcessorCount * 8;
            }

            this.hashCounter = -1;
            this.LoadHibpState(threads);

            try
            {
                this.ProcessRanges(progress, threads, ct).GetAwaiter().GetResult();
            }
            finally
            {
                this.WriteHibpState();
            }
        }

        private static void OnRequestError(DelegateResult<HttpResponseMessage> arg1, int arg2)
        {
            string requestUri = arg1.Result?.RequestMessage?.RequestUri?.ToString() ?? "";
            if (arg1.Exception != null)
            {
                Trace.WriteLine($"[yellow]Failed request #{arg2} while fetching {requestUri}. Exception message: {arg1.Exception.Message}.[/]");
            }
            else
            {
                Trace.WriteLine($"[yellow]Failed attempt #{arg2} fetching {requestUri}. Response contained HTTP Status code {arg1?.Result?.StatusCode}.[/]");
            }
        }
    }
}
