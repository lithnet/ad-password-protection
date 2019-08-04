using System;
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
    }
}
