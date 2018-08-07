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
    public class V1StoreTests : BinaryStoreTests
    {
        public V1StoreTests()
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "v1UnitTest");
            Directory.CreateDirectory(path);

            this.Store = new V1Store(path);
            this.HashSize = 20;
        }

        protected override string GetFileNameFromHash(string hash)
        {
            return $"{hash.Substring(0, 5)}.db";
        }

        protected override string GetPrefixFromHash(string hash)
        {
            return hash.Substring(0, 5);
        }
    }
}