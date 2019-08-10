using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lithnet.ActiveDirectory.PasswordProtection;

namespace ManagedUnitTests
{
    [TestClass]
    public class FilterTests
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool SetDllDirectory(string lpPathName);

        [TestMethod]
        public void GetUserPolicy()
        {
            SetDllDirectory(@"D:\github\lithnet\ad-password-protection\src\x64\Debug");

            UserPolicy policy = FilterInterface.GetUserPolicy("test");

            //GeneralPolicy gp = Marshal.PtrToStructure<GeneralPolicy>(policy.GeneralPolicy);


        }

        //[TestMethod]
        public void BuildMockApi()
        {
            return;
            StringBuilder builder = new StringBuilder();
            string currentRange = "00000";

            foreach (var line in File.ReadLines(@"D:\pwnedpwds\raw\pwned-passwords-sha1-ordered-by-hash-v5\pwned-passwords-sha1-ordered-by-hash-v5.txt"))
            {
                string lineRange = line.Substring(0, 5);

                if (lineRange != currentRange)
                {
                    File.WriteAllText(Path.Combine(@"D:\pwnedpwds\mock-api\range", currentRange), builder.ToString());
                    builder.Clear();
                    currentRange = lineRange;
                }

                builder.AppendLine(line.Substring(5, line.Length - 5));
            }

            File.WriteAllText(Path.Combine(@"D:\pwnedpwds\mock-api\range", currentRange), builder.ToString());
        }
    }
}
