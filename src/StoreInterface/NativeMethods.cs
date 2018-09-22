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
        [DllImport("D:\\github\\lithnet\\ad-password-filter\\src\\x64\\Debug\\lithnetpwdf.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int PasswordFilterEx(string accountName, string fullName, string password, bool isSetOperation);

        public static int TestPassword(string accountName, string fullName, string password, bool isSetOperation)
        {
            return PasswordFilterEx(accountName, fullName, password, isSetOperation);
        }
    }
}
