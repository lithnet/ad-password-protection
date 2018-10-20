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
    public class V3StoreTests : BinaryStoreTests
    {
        public V3StoreTests()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "v3UnitTest");
            Directory.CreateDirectory(path);

            this.Store = new V3Store(path);
            this.StoredHashSize = 14;
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
        public void BuildPwnStore()
        {
            return;
            string file = @"D:\pwnedpwds\raw\pwned-passwords-ntlm-ordered-by-hash.txt";
            StoreInterface.Store.ImportHexHashesFromSortedFile(this.Store, file);
        }


        private void BuildStore(string sourceFile)
        {
            string filename = Path.GetFileNameWithoutExtension(sourceFile);

            string path = Path.Combine(TestHelpers.TestStorePath, filename + "-f");
            Directory.CreateDirectory(path);

            var store = new V3Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, true, true, 2000000);

            path = Path.Combine(TestHelpers.TestStorePath, filename + "-n");
            Directory.CreateDirectory(path);

            store = new V3Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, false, true, 2000000);

            path = Path.Combine(TestHelpers.TestStorePath, filename + "-r");
            Directory.CreateDirectory(path);

            store = new V3Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, true, false, 2000000);
        }

        [TestMethod]
        public void MergeStore()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V3Store(path);

            path = Path.Combine(TestHelpers.TestStorePath, "top1000000-f");
            var source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);

            path = Path.Combine(TestHelpers.TestStorePath, "english-f");
            source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);

            path = Path.Combine(TestHelpers.TestStorePath, "words-f");
            source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);

            path = Path.Combine(TestHelpers.TestStorePath, "rockyou-f");
            source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);

            path = Path.Combine(TestHelpers.TestStorePath, "breachcompilationuniq-f");
            source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);

            path = Path.Combine(TestHelpers.TestStorePath, "hibp");
            source = new V3Store(path);

            StoreInterface.Store.ImportFromStore(target, source);


        }

        [TestMethod]
        public void TestBadPassword()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V3Store(path);

            Assert.IsTrue(target.IsPasswordInStore("monash!!!!", true));
        }

        [TestMethod]
        public void TestBadPassword2()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V3Store(path);

            Assert.IsTrue(target.IsPasswordInStore("Password345!", true));
        }

        [TestMethod]
        public void BuildStoreEnglish()
        {
            this.BuildStore(@"D:\pwnedpwds\raw\english.txt");
        }

        [TestMethod]
        public void BuildStoreWords()
        {
            this.BuildStore(@"D:\pwnedpwds\raw\words.txt");
        }

        [TestMethod]
        public void BuildStoreRockyou()
        {
            this.BuildStore(@"D:\pwnedpwds\raw\rockyou.txt");
        }

        [TestMethod]
        public void BuildStoreTop1000000()
        {
            this.BuildStore(@"D:\pwnedpwds\raw\top1000000.txt");
        }

        [TestMethod]
        public void BuildStoreBreachCompilationUniq()
        {
            this.BuildStore(@"D:\pwnedpwds\raw\breachcompilationuniq.txt");
        }

        [TestMethod]
        public void BuildStoreHibp()
        {
            string file = @"D:\pwnedpwds\raw\pwned-passwords-ntlm-ordered-by-hash.txt";
            string path = Path.Combine(TestHelpers.TestStorePath, "hibp");
            Directory.CreateDirectory(path);

            var store = new V3Store(path);
            StoreInterface.Store.ImportHexHashesFromSortedFile(store, file);
        }

        [TestMethod]
        public void AddEnglishDictionaryToNewStoreAndValidate()
        {
            string file = @"D:\pwnedpwds\raw\english.txt";
            string path = Path.Combine(TestHelpers.TestStorePath, Guid.NewGuid().ToString());
            Directory.CreateDirectory(path);

            try
            {
                var store = new V3Store(path);
                StoreInterface.Store.ImportPasswordsFromFile(store, file, true, false, 2000000);

                using (StreamReader reader = new StreamReader(file))
                {
                    while (!reader.EndOfStream)
                    {
                        string line = reader.ReadLine();

                        if (line == null || line.Length <= 0)
                        {
                            continue;
                        }

                        Assert.IsTrue(store.IsPasswordInStore(line));
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
        public void ConsolidateStore()
        {
            int add = 0;
            int del = 0;

            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V3Store(path);
            target.ConsolidateAndSort(ref add, ref del);
        }

        [TestMethod]
        public void TestAddPasswordToStore()
        {
            string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddPasswordToStore(password, false);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddPasswordToStore(password, false);

            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsPasswordInStore(password));
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            string hash = "8846F7EAEE8FB117AD06BDD830B7586C";
            byte[] hashBytes = hash.HexStringToBytes();

            this.Store.AddHashToStore(hashBytes);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash(hash));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddHashToStore(hashBytes);
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
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash(hashes[0]));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * hashes.Length);

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * hashes.Length);
        }

        [TestMethod]
        public void TestAllHashesAreFonudInStore()
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
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.Store.IsHashInStore(hash.HexStringToBytes()));
            }

            Assert.IsFalse(this.Store.IsHashInStore("0000000000000000000000000000000B".HexStringToBytes()));
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
            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()),
                ref added,
                ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            string rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("00000"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * 10);

            rawFile = Path.Combine(this.Store.StorePath, this.GetFileNameFromHash("10000"));
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

            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.ToHexString()).ToList());
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

            int added = 0;
            int discarded = 0;

            this.Store.AddHashesToStore(hashBytes, ref added, ref discarded);

            Assert.AreEqual(0, discarded);
            Assert.AreEqual(hashes.Length, added);

            CollectionAssert.AreEqual(hashes.OrderBy(t => t).ToList(), this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.ToHexString()).ToList());

            this.Store.AddHashToStore("00000000000000000000000000000006".HexStringToBytes());
            this.Store.AddHashToStore("00000000000000000000000000000007".HexStringToBytes());


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

            CollectionAssert.AreEqual(expectedHashes, this.Store.GetHashesFromStore(this.GetPrefixFromHash("00000")).Select(t => t.ToHexString()).ToList());
        }
    }
}