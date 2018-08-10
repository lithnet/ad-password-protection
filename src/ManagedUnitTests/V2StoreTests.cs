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
    public class V2StoreTests : BinaryStoreTests
    {
        public V2StoreTests()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "v2UnitTest");
            Directory.CreateDirectory(path);

            this.Store = new V2Store(path);
            this.HashSize = 18;
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
            string file = @"D:\pwnedpwds\raw\pwned-passwords-ordered-by-hash.txt";

            StoreInterface.Store.ImportHexHashesFromSortedFile(this.Store, file);
        }


        private void BuildStore(string sourceFile)
        {
            string filename = Path.GetFileNameWithoutExtension(sourceFile);

            string path = Path.Combine(TestHelpers.TestStorePath, filename + "-f");
            Directory.CreateDirectory(path);

            var store = new V2Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, true, true, 2000000);

            path = Path.Combine(TestHelpers.TestStorePath, filename + "-n");
            Directory.CreateDirectory(path);

            store = new V2Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, false, true, 2000000);

            path = Path.Combine(TestHelpers.TestStorePath, filename + "-r");
            Directory.CreateDirectory(path);

            store = new V2Store(path);
            StoreInterface.Store.ImportPasswordsFromFile(store, sourceFile, true, false, 2000000);
        }

        [TestMethod]
        public void MergeStore()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V2Store(path);

            path = Path.Combine(TestHelpers.TestStorePath, "top1000000-f");
            var source = new V2Store(path);


            StoreInterface.Store.ImportFromStore(target, source);


        }

        [TestMethod]
        public void TestBadPassword()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V2Store(path);

            Assert.IsTrue(target.IsPasswordInStore("monash!!!!", true));
        }

        [TestMethod]
        public void TestBadPassword2()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "merged");
            var target = new V2Store(path);

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
            string file = @"D:\pwnedpwds\raw\pwned-passwords-ordered-by-hash.txt";
            string path = Path.Combine(TestHelpers.TestStorePath, "hibp");
            Directory.CreateDirectory(path);

            var store = new V2Store(path);
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
                var store = new V2Store(path);
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
            this.Store.ConsolidateAndSort(ref add, ref del);
        }
    }
}