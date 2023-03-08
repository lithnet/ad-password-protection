using System;
using System.Text;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lithnet.ActiveDirectory.PasswordProtection;
using System.Linq;

namespace ManagedUnitTests
{
    [TestClass]
    public abstract class BinaryStoreTests
    {
        protected BinaryStore Store { get; set; }

        protected int StoredHashSize { get; set; }

        protected BinaryStoreTests()
        {
        }

        protected abstract string GetFileNameFromHash(string hash);

        protected abstract string GetPrefixFromHash(string hash);

        [TestInitialize]
        public void Initialize()
        {
            this.Store.ClearStore(StoreType.Password);
            this.Store.ClearStore(StoreType.Word);
        }

        [TestCleanup]
        public void Cleanup()
        {
            this.Store.ClearStore(StoreType.Password);
            this.Store.ClearStore(StoreType.Word);
        }
    }
}