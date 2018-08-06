using System;
using System.Text;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using StoreInterface;
using System.Linq;

namespace ManagedUnitTests
{
    [TestClass]
    public class V2StoreTests
    {
        private V2Store store;

        public V2StoreTests()
        {
            string tempPath = Path.GetTempPath();
            string path = Path.Combine(tempPath, "v2UnitTest");
            Directory.CreateDirectory(path);

            this.store = new V2Store(path);
        }

        [TestInitialize()]
        public void Initialize()
        {
            this.store.ClearStore();
        }

        [TestCleanup]
        public void Cleaup()
        {
            //this.store.ClearStore();
        }

        [TestMethod]
        public void TestAddPasswordToStore()
        {
            string password = "password"; //5BAA61E4C9B93F3F0682250B6CF8331B7EE68FD8

            this.store.AddPasswordToStore(password, false);

            string rawFile = Path.Combine(this.store.StorePath, "5BAA.db");
            FileInfo file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(18, file.Length);

            this.store.AddPasswordToStore(password, false);

            file = new FileInfo(rawFile);
            Assert.AreEqual(18, file.Length);

            Assert.IsTrue(this.store.IsPasswordInStore(password));
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            string hash = "000000005AD76BD555C1D6D771DE417A4B87E4B4";
            byte[] hashBytes = hash.GetSha1HashBytes();

            this.store.AddHashToStore(hashBytes);

            string rawFile = Path.Combine(this.store.StorePath, "0000.db");
            FileInfo file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(18, file.Length);

            this.store.AddHashToStore(hashBytes);

            file = new FileInfo(rawFile);
            Assert.AreEqual(18, file.Length);
        }

        [TestMethod]
        public void TestAddHashesToStore()
        {
            string[] hashes = new string[]
            {
                "0000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000006",
                "0000000000000000000000000000000000000007",
                "0000000000000000000000000000000000000008",
                "0000000000000000000000000000000000000009",
                "000000000000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.GetSha1HashBytes()).ToList();
            int added = 0;
            int discarded = 0;

            this.store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.store.StorePath, "0000.db");
            FileInfo file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(18 * hashes.Length, file.Length);

            this.store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), ref added, ref discarded);

            file = new FileInfo(rawFile);

            Assert.AreEqual(18 * hashes.Length, file.Length);
        }

        [TestMethod]
        public void TestAllHashesAreFonudInStore()
        {
            string[] hashes = new string[]
            {
                "0000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000006",
                "0000000000000000000000000000000000000007",
                "0000000000000000000000000000000000000008",
                "0000000000000000000000000000000000000009",
                "000000000000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.GetSha1HashBytes()).ToList();
            int added = 0;
            int discarded = 0;

            this.store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), ref added, ref discarded);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.store.IsHashInStore(hash.GetSha1HashBytes()));
            }

            Assert.IsFalse(this.store.IsHashInStore("000000000000000000000000000000000000000B".GetSha1HashBytes()));
        }

        [TestMethod]
        public void TestAddHashesToStoreIn2Ranges()
        {
            string[] hashes = new string[]
            {
                "1000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000002",
                "1000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000004",
                "1000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000006",
                "1000000000000000000000000000000000000007",
                "0000000000000000000000000000000000000008",
                "1000000000000000000000000000000000000009",
                "000000000000000000000000000000000000000A",
                "0000000000000000000000000000000000000001",
                "1000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000003",
                "1000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000005",
                "1000000000000000000000000000000000000006",
                "0000000000000000000000000000000000000007",
                "1000000000000000000000000000000000000008",
                "0000000000000000000000000000000000000009",
                "100000000000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.GetSha1HashBytes()).Reverse().ToList();
            int added = 0;
            int discarded = 0;

            this.store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.store.StorePath, "0000.db");
            FileInfo file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(18 * 10, file.Length);

            rawFile = Path.Combine(this.store.StorePath, "1000.db");
            file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(18 * 10, file.Length);
        }

        [TestMethod]
        public void TestHashOrder()
        {
            string[] hashes = new string[]
            {
                "0000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000006",
                "0000000000000000000000000000000000000008",
                "000000000000000000000000000000000000000A",
                "0000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000007",
                "0000000000000000000000000000000000000009",
            };

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Select(t => t.GetSha1HashBytes()), new ByteArrayComparer());

            int added = 0;
            int discarded = 0;

            this.store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.store.GetHashesFromStore("0000").Select (t => t.GetHexString()).ToList());
        }

        [TestMethod]
        public void TestHashOrderAfterInsert()
        {
            string[] hashes = new string[]
            {
                "0000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000008",
                "0000000000000000000000000000000000000009",
                "000000000000000000000000000000000000000A",
                "000000000000000000000000000000000000000B",
                "000000000000000000000000000000000000000C",
            };

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Reverse().Select(t => t.GetSha1HashBytes()), new ByteArrayComparer());

            int added = 0;
            int discarded = 0;

            this.store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.store.GetHashesFromStore("0000").Select(t => t.GetHexString()).ToList());

            this.store.AddHashToStore("0000000000000000000000000000000000000006".GetSha1HashBytes());
            this.store.AddHashToStore("0000000000000000000000000000000000000007".GetSha1HashBytes());


            string[] expectedHashes = new string[]
            {
                "0000000000000000000000000000000000000001",
                "0000000000000000000000000000000000000002",
                "0000000000000000000000000000000000000003",
                "0000000000000000000000000000000000000004",
                "0000000000000000000000000000000000000005",
                "0000000000000000000000000000000000000006",
                "0000000000000000000000000000000000000007",
                "0000000000000000000000000000000000000008",
                "0000000000000000000000000000000000000009",
                "000000000000000000000000000000000000000A",
                "000000000000000000000000000000000000000B",
                "000000000000000000000000000000000000000C",
            };

            CollectionAssert.AreEqual(expectedHashes, this.store.GetHashesFromStore("0000").Select(t => t.GetHexString()).ToList());
        }
    }
}
