using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lithnet.ActiveDirectory.PasswordProtection;

namespace ManagedUnitTests
{
    [TestClass]
    public class FilterTests
    {
        [TestMethod]
        public void GetUserPolicy()
        {
            FilterInterface.GetUserPolicy("test", out UserPolicy policy);
        }
    }
}
