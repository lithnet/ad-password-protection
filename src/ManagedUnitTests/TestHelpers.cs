using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.Win32;

namespace ManagedUnitTests
{
    public static class TestHelpers
    {
        public static string TestStorePath = ResolveStorePath();

        private static string ResolveStorePath()
        {
            string path = Environment.GetEnvironmentVariable("LPP_TEST_STORE_PATH");
            if (!string.IsNullOrEmpty(path))
            {
                return path;
            }

            using (RegistryKey key = Registry.LocalMachine.OpenSubKey(@"Software\Lithnet\PasswordFilter"))
            {
                path = key?.GetValue("TestStorePath") as string;
                if (!string.IsNullOrEmpty(path))
                {
                    return path;
                }
            }

            return Path.Combine(Path.GetTempPath(), "lppstore-test");
        }

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
