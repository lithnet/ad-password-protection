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

        [TestMethod]
        public void AddEnglishDictionary()
        {
            string file = @"D:\pwnedpwds\raw\breachcompilationuniq.txt";

            StoreInterface.Store.ImportPasswordsFromFile(this.Store, file, true);
        }
    }
}