using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
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
        private int writeLock;

        public HibpDownloader(Store store)
        {
            this.store = store;
            this.httpClient = InitializeHttpClient();
            this.policy = HttpPolicyExtensions.HandleTransientHttpError().WaitAndRetryAsync(10, retryAttempt => TimeSpan.FromSeconds(Math.Pow(2, retryAttempt)),
                (response, delay, retryCount, context) => Trace.WriteLine($"Retry {retryCount}/10: {delay}: {response.Result.StatusCode}."));

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
                    builder.Append(item.Key).Append(':').AppendLine(item.Value);
                }

                this.store.SetStoreMetadata(MetadataItemName, builder.ToString());
            }
            finally
            {
                this.writeLock = 0;
            }
        }

        private static HttpClient InitializeHttpClient()
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
            client.Timeout = TimeSpan.FromMinutes(15);
            
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

                try
                {
                    return await this.httpClient.SendAsync(request, ct);
                }
                catch (Exception ex)
                {
                    Trace.WriteLine(ex.ToString());
                    throw;
                }
            }).ConfigureAwait(false);
        }

        private async Task ProcessRanges2(OperationProgress progress, int threads, CancellationToken ct)
        {
            progress.HibpReadInProgress = true;
            progress.HibpHashTotal = totalHashes;
            progress.HibpStartTime = DateTime.Now;

            ParallelOptions op = new ParallelOptions()
            {
                CancellationToken = ct,
                MaxDegreeOfParallelism = threads
            };

            var result = Parallel.For(0, 0xFFFF + 1, op, i => this.DownloadRange(progress, i, ct).GetAwaiter().GetResult());

            if (!result.IsCompleted)
            {
                throw new ApplicationException("The operation did not complete successfully");
            }
        }

        private async Task DownloadRange(OperationProgress progress, int range, CancellationToken ct)
        {
            try
            {
                HashSet<byte[]> items = new HashSet<byte[]>(15000);

                var baseCount = range * (0xF + 1);

                for (int i = 0; i <= 0xF; i++)
                {
                    var currentHash = baseCount + i;
                    string hibpRange = currentHash.ToHexHibpPrefixString();
                    //Trace.WriteLine($"Hash GET {hibpRange} {currentHash}/{totalHashes}");
                    progress.IncrementCurrentHash();

                    this.hibpState.TryGetValue(hibpRange, out string etag);

                    var response = await this.GetRangeFromApi(hibpRange, ct, etag).ConfigureAwait(false);

                    if (response.StatusCode == HttpStatusCode.NotModified)
                    {
                        progress.IncrementUnchangedRange();
                    }
                    else
                    {
                        response.EnsureSuccessStatusCode();
                        progress.IncrementChangedRange();

                        this.hibpState[hibpRange] = response.Headers.ETag.ToString();
                        var lines = await response.Content.ReadAsStringAsync().ConfigureAwait(false);

                        ExtractHashesFromResponse(items, hibpRange, lines);
                    }
                }

                this.store.AddSingleRangeToStore(items, range.ToHexLppPrefixString(), StoreType.Password, progress);

                if (range % 10000 == 0)
                {
                    this.WriteHibpState();
                }
            }
            catch (Exception ex)
            {
                Trace.WriteLine(ex.ToString());
                throw;
            }
        }

        private static void ExtractHashesFromResponse(HashSet<byte[]> items, string hibpRange, string lines)
        {
            var incompleteLines = lines.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries);

            int lineCount = 0;

            foreach (var line in incompleteLines)
            {
                lineCount++;
                var res = hibpRange + line;
                byte[] hash = Store.GetBinaryHashFromLine(res, Constants.NtlmHashLengthString, lineCount);
                items.Add(hash);
            }
        }

        private HashSet<byte[]> GetHexHashesFromString(string lines, string range)
        {
            var splitLines = lines.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);

            var hash = this.GetHexHashesFromLines(splitLines, range);
            return new HashSet<byte[]>(hash, ByteArrayComparer.Comparer);
        }

        private IEnumerable<byte[]> GetHexHashesFromLines(IEnumerable<string> lines, string prefix)
        {
            int hashStringLength = Constants.NtlmHashLengthString;
            int lineCount = 0;

            foreach (string line in lines)
            {
                lineCount++;

                var reconstructedLine = prefix + line;
                yield return Store.GetBinaryHashFromLine(reconstructedLine, hashStringLength, lineCount);
            }
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
                string range = currentHash.ToHexHibpPrefixString();
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

                    //Store.ImportHexHashesFromString(this.store, lines, range, StoreType.Password, progress, ct);
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
                this.ProcessRanges2(progress, threads, ct).GetAwaiter().GetResult();
            }
            finally
            {
                this.WriteHibpState();
            }
        }
    }
}
