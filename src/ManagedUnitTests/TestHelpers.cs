using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace ManagedUnitTests
{
    public static class TestHelpers
    {
        public static string TestStorePath = @"D:\dev\lppstore";

        public static void AssertFileIsExpectedSize(string rawFile, int size)
        {
            FileInfo file = new FileInfo(rawFile);

            if (!file.Exists)
            {
                Assert.Fail("The file was not created");
            }

            Assert.AreEqual(size, file.Length);
        }
    }
}
