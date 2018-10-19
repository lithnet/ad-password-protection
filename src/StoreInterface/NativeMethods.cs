using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace StoreInterface
{
    internal static class NativeMethods
    {
        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int PasswordFilterEx(string accountName, string fullName, string password, bool isSetOperation);

        public static PasswordTestResult TestPassword(string accountName, string fullName, string password, bool isSetOperation)
        {
            return (PasswordTestResult)PasswordFilterEx(accountName, fullName, password, isSetOperation);
        }
    }
}
