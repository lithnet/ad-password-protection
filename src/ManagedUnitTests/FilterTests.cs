using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using StoreInterface;

namespace ManagedUnitTests
{
    [TestClass]
    public class FilterTests
    {
        [TestMethod]
        public void TestMethod1()
        {
            Assert.AreEqual(8, FilterInterface.TestPassword("accountname", "fullname", "password", true));
        }

        [TestMethod]
        public void GetUserPolicy()
        {
            FilterInterface.GetUserPolicy("test", out UserPolicy policy);
        }
    }
}
