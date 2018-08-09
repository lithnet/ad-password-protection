using System;
using System.Text;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using StoreInterface;
using System.Linq;

namespace ManagedUnitTests
{
    public class StoreTestBase
    {
        private Store store;

        public StoreTestBase(Store store)
        {
            this.store = store;
        }

        public void TestAddPasswordToStore()
        {
            string password = "password";
            Assert.IsFalse(this.store.IsPasswordInStore(password));
            this.store.AddPasswordToStore(password, false);
            Assert.IsTrue(this.store.IsPasswordInStore(password));
        }

        public void TestAddHashToStore()
        {
            string hash = "000000005AD76BD555C1D6D771DE417A4B87E4B4";
            byte[] hashBytes = hash.GetSha1HashBytes();
            Assert.IsFalse(this.store.IsHashInStore(hashBytes));
            this.store.AddHashToStore(hashBytes);
            Assert.IsTrue(this.store.IsHashInStore(hashBytes));
        }

        public void TestAllHashesAreFoundInStore()
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

            this.store.AddHashesToStore(
                new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), 
                ref added, 
                ref discarded);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.store.IsHashInStore(hash.GetSha1HashBytes()));
            }

            Assert.IsFalse(this.store.IsHashInStore("000000000000000000000000000000000000000B".GetSha1HashBytes()));
        }
    }
}