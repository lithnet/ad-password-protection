using System;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Lithnet.ActiveDirectory.PasswordProtection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace ManagedUnitTests
{
    [TestClass]
    public class HibpDownloaderTests
    {
        private const string MetadataItemName = "hibp-state";

        private string storePath;
        private V3Store store;

        [TestInitialize]
        public void TestInitialize()
        {
            this.storePath = Path.Combine(TestHelpers.TestStorePath, "hibp-" + Guid.NewGuid());
            Directory.CreateDirectory(this.storePath);
            this.store = new V3Store(this.storePath);
        }

        [TestCleanup]
        public void TestCleanup()
        {
            if (Directory.Exists(this.storePath))
            {
                Directory.Delete(this.storePath, true);
            }
        }

        [TestMethod]
        public async Task MissingEtagFailsAndPreservesSavedState()
        {
            this.store.SetStoreMetadata(MetadataItemName, "00000:\"old-etag\"\r\n");
            TestHibpDownloader downloader = new TestHibpDownloader(this.store, CreateResponseWithoutEtag);
            OperationProgress progress = new OperationProgress();

            HibpEtagMissingException exception = await Assert.ThrowsExceptionAsync<HibpEtagMissingException>(() => downloader.ExecuteAsync(progress, 1, CancellationToken.None, 0, 0));

            Assert.AreEqual("00000", exception.HibpRange);
            StringAssert.Contains(exception.Message, "did not include the required ETag header");
            StringAssert.Contains(this.store.GetStoreMetadata(MetadataItemName), "00000:\"old-etag\"");
            Assert.AreEqual("\"old-etag\"", downloader.RequestEtags["00000"]);
            Assert.AreEqual(0, this.store.GetHashes("0000", StoreType.Password).Count);
        }

        [TestMethod]
        public async Task ResponsesWithEtagsUpdateSavedState()
        {
            this.store.SetStoreMetadata(MetadataItemName, CreateSavedState());
            TestHibpDownloader downloader = new TestHibpDownloader(this.store, CreateResponseWithEtag);
            OperationProgress progress = new OperationProgress();

            await downloader.ExecuteAsync(progress, 1, CancellationToken.None, 0, 0);

            Assert.AreEqual(16, progress.HibpRangesChanged);
            Assert.AreEqual(16, progress.HashesAdded);
            Assert.AreEqual(16, this.store.GetHashes("0000", StoreType.Password).Count);
            string savedState = this.store.GetStoreMetadata(MetadataItemName);
            StringAssert.Contains(savedState, "00000:\"etag-00000\"");
            StringAssert.Contains(savedState, "00001:\"etag-00001\"");
            Assert.AreEqual(16, savedState.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries).Length);
        }

        [TestMethod]
        public async Task NotModifiedResponsesKeepSavedEtags()
        {
            string originalState = CreateSavedState();
            this.store.SetStoreMetadata(MetadataItemName, originalState);
            TestHibpDownloader downloader = new TestHibpDownloader(this.store, CreateNotModifiedResponse);
            OperationProgress progress = new OperationProgress();

            await downloader.ExecuteAsync(progress, 1, CancellationToken.None, 0, 0);

            string savedState = this.store.GetStoreMetadata(MetadataItemName);
            Assert.AreEqual(16, progress.HibpRangesUnchanged);
            Assert.AreEqual(0, progress.HibpRangesChanged);
            foreach (string line in originalState.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries))
            {
                StringAssert.Contains(savedState, line);
            }
        }

        [TestMethod]
        public async Task HttpErrorsAreTerminating()
        {
            TestHibpDownloader downloader = new TestHibpDownloader(this.store, CreateServerErrorResponse);
            OperationProgress progress = new OperationProgress();

            await Assert.ThrowsExceptionAsync<HttpRequestException>(() => downloader.ExecuteAsync(progress, 1, CancellationToken.None, 0, 0));
        }

        private static HttpResponseMessage CreateResponseWithoutEtag(string range, string etag)
        {
            return CreateResponse(range, false);
        }

        private static HttpResponseMessage CreateResponseWithEtag(string range, string etag)
        {
            return CreateResponse(range, true);
        }

        private static HttpResponseMessage CreateNotModifiedResponse(string range, string etag)
        {
            return new HttpResponseMessage(HttpStatusCode.NotModified);
        }

        private static HttpResponseMessage CreateServerErrorResponse(string range, string etag)
        {
            return new HttpResponseMessage(HttpStatusCode.InternalServerError);
        }

        private static HttpResponseMessage CreateResponse(string range, bool includeEtag)
        {
            HttpResponseMessage response = new HttpResponseMessage(HttpStatusCode.OK);
            response.Content = new StringContent(new string('0', 26) + "1:1");

            if (includeEtag)
            {
                response.Headers.ETag = new EntityTagHeaderValue("\"etag-" + range + "\"");
            }

            return response;
        }

        private static string CreateSavedState()
        {
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i <= 0xF; i++)
            {
                string range = i.ToString("X5");
                builder.Append(range).Append(":\"old-").Append(range).AppendLine("\"");
            }

            return builder.ToString();
        }
    }
}
