using System;
using System.IO;
using System.Text;
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
