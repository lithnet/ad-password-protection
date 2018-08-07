using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using StoreInterface;

namespace ManagedUnitTests
{
    [TestClass]
    public class EseStoreTests
    {
        private static EsentStore store;
        private static StoreTestBase tester;

        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            string path = Path.Combine(TestHelpers.TestStorePath, "eseUnitTest");
            EsentStore.CreateNewDB(path);
            store = new EsentStore(path);
            tester = new StoreTestBase(store);
        }

        [ClassCleanup]
        public static void Cleanup()
        {
            store.Dispose();
        }

        [TestInitialize()]
        public void Initialize()
        {
            store.ClearStore();
        }

        [TestMethod]
        public void TestAddPasswordToStore()
        {
            tester.TestAddHashToStore();
        }

        [TestMethod]
        public void TestAddHashToStore()
        {
            tester.TestAddHashToStore();
        }

        [TestMethod]
        public void TestAllHashesAreFoundInStore()
        {
            tester.TestAllHashesAreFoundInStore();
        }
    }
}
