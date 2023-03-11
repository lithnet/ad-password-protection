using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using Lithnet.ActiveDirectory.PasswordProtection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace ManagedUnitTests
{
    [TestClass]
    public class V3StoreTests : BinaryStoreTests
    {
        public V3StoreTests()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "v3UnitTest");
            Directory.CreateDirectory(path);

            this.Store = new V3Store(path);
            this.StoredHashSize = 14;
        }

        [TestMethod]
        public void TestHibp()
        {
            HibpDownloader downloader = new HibpDownloader(this.Store);
            var progress = new OperationProgress();

            downloader.Execute(progress, 1, CancellationToken.None);
        }
        
        protected override string GetFileNameFromHash(string hash)
        {
            return $"{hash.Substring(0, 4)}.db";
        }

        protected override string GetPrefixFromHash(string hash)
        {
            return hash.Substring(0, 4);
        }

        [TestMethod]
        public void TestGoodHashTypes()
        {
            Lithnet.ActiveDirectory.PasswordProtection.Store.ImportHexHashesFromSortedFile(this.Store, StoreType.Password, @"TestData\test-good-hash.txt", new CancellationToken());
        }

        [TestMethod]
        public void TestHashTooLong()
        {
            try
            {
                Lithnet.ActiveDirectory.PasswordProtection.Store.ImportHexHashesFromSortedFile(this.Store, StoreType.Password, @"TestData\test-hash-too-long.txt", new CancellationToken());
                Assert.Fail("Did not throw the expected exception");
            }
            catch (InvalidDataException)
            {
            }
        }

        [TestMethod]
        public void TestHashTooShort()
        {
            try
            {
                Lithnet.ActiveDirectory.PasswordProtection.Store.ImportHexHashesFromSortedFile(this.Store, StoreType.Password, @"TestData\test-hash-too-short.txt", new CancellationToken());
                Assert.Fail("Did not throw the expected exception");
            }
            catch (InvalidDataException)
            {
            }
        }

        [TestMethod]
        public void TestAddNormalizedWordToStore()
        {
            const string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Word);

            string rawFile = Path.Combine(this.Store.StorePathWordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddToStore(password, StoreType.Word);

            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Word));
            Assert.IsTrue(this.Store.IsInStore("Password1234!", StoreType.Word));
        }

        [TestMethod]
        public void AddEnglishDictionaryToNewStoreAndValidate()
        {
            const string file = @"TestData\english.txt";
            string path = Path.Combine(TestHelpers.TestStorePath, Guid.NewGuid().ToString());
            Directory.CreateDirectory(path);
            CancellationToken ct = new CancellationToken();

            try
            {
                var store = new V3Store(path);
                Lithnet.ActiveDirectory.PasswordProtection.Store.ImportPasswordsFromFile(store, StoreType.Word, file, ct);

                using (StreamReader reader = new StreamReader(file))
                {
                    while (!reader.EndOfStream)
                    {
                        string line = reader.ReadLine();

                        if (string.IsNullOrEmpty(line))
                        {
                            continue;
                        }

                        Assert.IsTrue(store.IsInStore(line, StoreType.Word));
                    }
                }
            }
            finally
            {
                try
                {
                    Directory.Delete(path, true);
                }
                catch
                {
                }
            }
        }

        [TestMethod]
        public void TestAddPasswordToStore()
        {
            const string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Password);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddToStore(password, StoreType.Password);

            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Password));
        }

        [TestMethod]
        public void TestRemovePasswordFromStore()
        {
            const string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Password);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Password));

            this.Store.RemoveFromStore(password, StoreType.Password);
            TestHelpers.AssertFileIsExpectedSize(rawFile, 0);
            Assert.IsFalse(this.Store.IsInStore(password, StoreType.Password));
        }

        [TestMethod]
        public void TestRemoveWordFromStore()
        {
            const string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Word);

            string rawFile = Path.Combine(this.Store.StorePathWordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Word));

            this.Store.RemoveFromStore(password, StoreType.Word);
            TestHelpers.AssertFileIsExpectedSize(rawFile, 0);
            Assert.IsFalse(this.Store.IsInStore(password, StoreType.Word));
        }

        [TestMethod]
        public void TestRemoveNormalizedWordFromStore()
        {
            const string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Word);

            string rawFile = Path.Combine(this.Store.StorePathWordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Word));

            this.Store.RemoveFromStore("password1", StoreType.Word);
            TestHelpers.AssertFileIsExpectedSize(rawFile, 0);
            Assert.IsFalse(this.Store.IsInStore(password, StoreType.Word));
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            const string hash = "8846F7EAEE8FB117AD06BDD830B7586C";
            byte[] hashBytes = hash.HexStringToBytes();

            this.Store.AddToStore(hashBytes, StoreType.Password);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash(hash));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddToStore(hashBytes, StoreType.Password);
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);
        }

        [TestMethod]
        public void TestAddHashesToStore()
        {
            string[] hashes = new string[]
            {
                "00000000000000000000000000000001",
                "00000000000000000000000000000002",
                "00000000000000000000000000000003",
                "00000000000000000000000000000004",
                "00000000000000000000000000000005",
                "00000000000000000000000000000006",
                "00000000000000000000000000000007",
                "00000000000000000000000000000008",
                "00000000000000000000000000000009",
                "0000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).ToList();
            CancellationToken ct = new CancellationToken();

            OperationProgress progress = new OperationProgress();

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, progress, ct);

            Assert.AreEqual(0, progress.HashesDiscarded);
            Assert.AreEqual(hashes.Length, progress.HashesAdded);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash(hashes[0]));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * hashes.Length);

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, new OperationProgress(), ct);
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * hashes.Length);
        }

        [TestMethod]
        public void TestAllHashesAreFoundInStore()
        {
            string[] hashes = new string[]
            {
                "00000000000000000000000000000001",
                "00000000000000000000000000000002",
                "00000000000000000000000000000003",
                "00000000000000000000000000000004",
                "00000000000000000000000000000005",
                "00000000000000000000000000000006",
                "00000000000000000000000000000007",
                "00000000000000000000000000000008",
                "00000000000000000000000000000009",
                "0000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).ToList();
            CancellationToken ct;

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, new OperationProgress(), ct);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.Store.IsInStore(hash.HexStringToBytes(), StoreType.Password));
            }

            Assert.IsFalse(this.Store.IsInStore("0000000000000000000000000000000B".HexStringToBytes(), StoreType.Password));
        }

        [TestMethod]
        public void TestAddHashesToStoreIn2Ranges()
        {
            string[] hashes = new string[]
            {
                "10000000000000000000000000000001",
                "00000000000000000000000000000002",
                "10000000000000000000000000000003",
                "00000000000000000000000000000004",
                "10000000000000000000000000000005",
                "00000000000000000000000000000006",
                "10000000000000000000000000000007",
                "00000000000000000000000000000008",
                "10000000000000000000000000000009",
                "0000000000000000000000000000000A",
                "00000000000000000000000000000001",
                "10000000000000000000000000000002",
                "00000000000000000000000000000003",
                "10000000000000000000000000000004",
                "00000000000000000000000000000005",
                "10000000000000000000000000000006",
                "00000000000000000000000000000007",
                "10000000000000000000000000000008",
                "00000000000000000000000000000009",
                "1000000000000000000000000000000A",
            };

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).Reverse().ToList();
            OperationProgress progress = new OperationProgress();
            CancellationToken ct;
            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, progress, ct);

            Assert.AreEqual(0, progress.HashesDiscarded);
            Assert.AreEqual(hashes.Length, progress.HashesAdded);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash("00000"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * 10);

            rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash("10000"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * 10);
        }

        [TestMethod]
        public void TestHashOrder()
        {
            string[] hashes = new string[]
            {
                "00000000000000000000000000000002",
                "00000000000000000000000000000004",
                "00000000000000000000000000000006",
                "00000000000000000000000000000008",
                "0000000000000000000000000000000A",
                "00000000000000000000000000000001",
                "00000000000000000000000000000003",
                "00000000000000000000000000000005",
                "00000000000000000000000000000007",
                "00000000000000000000000000000009",
            };

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Select(t => t.HexStringToBytes()), new ByteArrayComparer());
            OperationProgress progress = new OperationProgress();
            CancellationToken ct;

            this.Store.AddToStore(hashBytes, StoreType.Password, progress, ct);

            Assert.AreEqual(0, progress.HashesDiscarded);
            Assert.AreEqual(hashes.Length, progress.HashesAdded);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashes(this.GetPrefixFromHash("00000"), StoreType.Password).Select(t => t.ToHexString()).ToList());
        }

        [TestMethod]
        public void TestHashOrderAfterInsert()
        {
            string[] hashes = new string[]
            {
                "00000000000000000000000000000001",
                "00000000000000000000000000000002",
                "00000000000000000000000000000003",
                "00000000000000000000000000000004",
                "00000000000000000000000000000005",
                "00000000000000000000000000000008",
                "00000000000000000000000000000009",
                "0000000000000000000000000000000A",
                "0000000000000000000000000000000B",
                "0000000000000000000000000000000C",
            };

            HashSet<byte[]> hashBytes = new HashSet<byte[]>(hashes.Reverse().Select(t => t.HexStringToBytes()), new ByteArrayComparer());
            OperationProgress progress = new OperationProgress();
            CancellationToken ct;
            this.Store.AddToStore(hashBytes, StoreType.Password, progress, ct);

            Assert.AreEqual(0, progress.HashesDiscarded);
            Assert.AreEqual(hashes.Length, progress.HashesAdded);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashes(this.GetPrefixFromHash("00000"), StoreType.Password).Select(t => t.ToHexString()).ToList());

            this.Store.AddToStore("00000000000000000000000000000006".HexStringToBytes(), StoreType.Password);
            this.Store.AddToStore("00000000000000000000000000000007".HexStringToBytes(), StoreType.Password);

            string[] expectedHashes = new string[]
            {
                "00000000000000000000000000000001",
                "00000000000000000000000000000002",
                "00000000000000000000000000000003",
                "00000000000000000000000000000004",
                "00000000000000000000000000000005",
                "00000000000000000000000000000006",
                "00000000000000000000000000000007",
                "00000000000000000000000000000008",
                "00000000000000000000000000000009",
                "0000000000000000000000000000000A",
                "0000000000000000000000000000000B",
                "0000000000000000000000000000000C",
            };

            CollectionAssert.AreEqual(expectedHashes, this.Store.GetHashes(this.GetPrefixFromHash("00000"), StoreType.Password).Select(t => t.ToHexString()).ToList());
        }
    }
}