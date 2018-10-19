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
    public abstract class Sha1StoreTests
    {
        protected BinaryStore Store { get; set; }

        protected int HashSize { get; set; }

        protected Sha1StoreTests()
        {
        }

        protected abstract string GetFileNameFromHash(string hash);

        protected abstract string GetPrefixFromHash(string hash);


        [TestInitialize]
        public void Initialize()
        {
            //this.Store.ClearStore();
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

            this.Store.AddPasswordToStore(password, false);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("5BAA61E4C9B93F3F0682250B6CF8331B7EE68FD8"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize);

            this.Store.AddPasswordToStore(password, false);

            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize);

            Assert.IsTrue(this.Store.IsPasswordInStore(password));
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            string hash = "000000005AD76BD555C1D6D771DE417A4B87E4B4";
            byte[] hashBytes = hash.HexStringToBytes();

            this.Store.AddHashToStore(hashBytes);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash(hash));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize);

            this.Store.AddHashToStore(hashBytes);
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize);
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

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).ToList();
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash(hashes[0]));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize * hashes.Length);

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize * hashes.Length);
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

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).ToList();
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.Store.IsHashInStore(hash.HexStringToBytes()));
            }

            Assert.IsFalse(this.Store.IsHashInStore("000000000000000000000000000000000000000B".HexStringToBytes()));
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

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).Reverse().ToList();
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("00000"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize * 10);

            rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("10000"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.HashSize * 10);
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

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Select(t => t.HexStringToBytes()), new ByteArrayComparer());

            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.GetHexString()).ToList());
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

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Reverse().Select(t => t.HexStringToBytes()), new ByteArrayComparer());

            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.GetHexString()).ToList());

            this.Store.AddHashToStore("0000000000000000000000000000000000000006".HexStringToBytes());
            this.Store.AddHashToStore("0000000000000000000000000000000000000007".HexStringToBytes());


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

            CollectionAssert.AreEqual(expectedHashes, this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.GetHexString()).ToList());
        }

    }
}