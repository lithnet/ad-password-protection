using System.Collections.Generic;
using System.Linq;
using System.Threading;
using Lithnet.ActiveDirectory.PasswordProtection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace ManagedUnitTests
{
    public class StoreTestBase
    {
        private readonly Store store;

        public StoreTestBase(Store store)
        {
            this.store = store;
        }

        public void TestAddPasswordToStore()
        {
            const string password = "password";
            Assert.IsFalse(this.store.IsInStore(password, StoreType.Password));
            this.store.AddToStore(password, StoreType.Password);
            Assert.IsTrue(this.store.IsInStore(password, StoreType.Password));
        }

        public void TestAddHashToStore()
        {
            const string hash = "000000005AD76BD555C1D6D771DE417A4B87E4B4";
            byte[] hashBytes = hash.HexStringToBytes();
            Assert.IsFalse(this.store.IsInStore(hashBytes, StoreType.Password));
            this.store.AddToStore(hashBytes, StoreType.Password);
            Assert.IsTrue(this.store.IsInStore(hashBytes, StoreType.Password));
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

            List<byte[]> hashBytes = hashes.Select(t => t.HexStringToBytes()).ToList();

            this.store.AddToStore(
                new HashSet<byte[]>(hashBytes, new ByteArrayComparer()), StoreType.Password, new OperationProgress(), CancellationToken.None);

            foreach (string hash in hashes)
            {
                Assert.IsTrue(this.store.IsInStore(hash.HexStringToBytes(), StoreType.Password));
            }

            Assert.IsFalse(this.store.IsInStore("000000000000000000000000000000000000000B".HexStringToBytes(), StoreType.Password));
        }
    }
}