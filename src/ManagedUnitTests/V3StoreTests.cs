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
        public void BuildUsablev3Store()
        {
            return;
            string path = Path.Combine(TestHelpers.TestStorePath, "v3Build");
            Directory.CreateDirectory(path);
            V3Store store = new V3Store(path);

            // Start with HIBP
            string file = @"D:\pwnedpwds\raw\pwned-passwords-ntlm-ordered-by-hash.txt";
            StoreInterface.Store.ImportHexHashesFromSortedFile(store, StoreType.Password, file);

            // add english dictionary to word store
            file = @"D:\pwnedpwds\raw\english.txt";
            StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Word, file);

            // add more english words to word store
            file = @"D:\pwnedpwds\raw\words.txt";
            StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Word, file);

            // add rockyou breach
            file = @"D:\pwnedpwds\raw\rockyou.txt";
            StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Password, file);

            // add top 100000 
            file = @"D:\pwnedpwds\raw\top1000000.txt";
            StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Password, file);

            // add breach compilation
            file = @"D:\pwnedpwds\raw\breachcompilationuniq.txt";
            StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Password, file);
        }

        [TestMethod]
        public void TestBadPassword()
        {
            V3Store store = new V3Store(@"D:\pwnedpwds\store");
            Assert.IsTrue(store.IsInStore("monash!!!!", StoreType.Word));
        }

        [TestMethod]
        public void TestBadPassword2()
        {
            V3Store store = new V3Store(@"D:\pwnedpwds\store");
            Assert.IsTrue(store.IsInStore("Password345!", StoreType.Word));
        }

        //[TestMethod]
        //public void BuildStoreEnglish()
        //{
        //    this.BuildStore(@"D:\pwnedpwds\raw\english.txt");
        //}

        //[TestMethod]
        //public void BuildStoreWords()
        //{
        //    this.BuildStore(@"D:\pwnedpwds\raw\words.txt");
        //}

        //[TestMethod]
        //public void BuildStoreRockyou()
        //{
        //    this.BuildStore(@"D:\pwnedpwds\raw\rockyou.txt");
        //}

        //[TestMethod]
        //public void BuildStoreTop1000000()
        //{
        //    this.BuildStore(@"D:\pwnedpwds\raw\top1000000.txt");
        //}

        //[TestMethod]
        //public void BuildStoreBreachCompilationUniq()
        //{
        //    this.BuildStore(@"D:\pwnedpwds\raw\breachcompilationuniq.txt");
        //}

        //[TestMethod]
        //public void BuildStoreHibp()
        //{
        //    string file = @"D:\pwnedpwds\raw\pwned-passwords-ntlm-ordered-by-hash.txt";
        //    string path = Path.Combine(TestHelpers.TestStorePath, "hibp");
        //    Directory.CreateDirectory(path);

        //    var store = new V3Store(path);
        //    StoreInterface.Store.ImportHexHashesFromSortedFile(store, file);
        //}

        [TestMethod]
        public void AddEnglishDictionaryToNewStoreAndValidate()
        {
            string file = @"D:\pwnedpwds\raw\english.txt";
            string path = Path.Combine(TestHelpers.TestStorePath, Guid.NewGuid().ToString());
            Directory.CreateDirectory(path);

            try
            {
                var store = new V3Store(path);
                StoreInterface.Store.ImportPasswordsFromFile(store, StoreType.Word, file);

                using (StreamReader reader = new StreamReader(file))
                {
                    while (!reader.EndOfStream)
                    {
                        string line = reader.ReadLine();

                        if (line == null || line.Length <= 0)
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
            string password = "password"; //8846F7EAEE8FB117AD06BDD830B7586C

            this.Store.AddToStore(password, StoreType.Password);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash("8846F7EAEE8FB117AD06BDD830B7586C"));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            this.Store.AddToStore(password, StoreType.Password);

            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize);

            Assert.IsTrue(this.Store.IsInStore(password, StoreType.Password));
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            string hash = "8846F7EAEE8FB117AD06BDD830B7586C";
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

            OperationProgress progress = new OperationProgress();

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, progress);

            Assert.AreEqual(0, progress.HashesDiscarded);
            Assert.AreEqual(hashes.Length, progress.HashesAdded);

            string rawFile = Path.Combine(this.Store.StorePathPasswordStore, this.GetFileNameFromHash(hashes[0]));
            TestHelpers.AssertFileIsExpectedSize(rawFile, this.StoredHashSize * hashes.Length);

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password);
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

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password);

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

            this.Store.AddToStore(new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, progress);

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


            this.Store.AddToStore(hashBytes, StoreType.Password, progress);

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

            this.Store.AddToStore(hashBytes, StoreType.Password, progress);

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