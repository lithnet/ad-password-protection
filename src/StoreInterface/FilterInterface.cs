using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public static class FilterInterface
    {
        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int PasswordFilterEx(string accountName, string fullName, string password, bool isSetOperation);

        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern void GetUserPolicySettings(string accountName, out UserPolicy policy);

        public static PasswordTestResult TestPassword(string username, string fullname, string password, bool isSetOperation)
        {
            return (PasswordTestResult)FilterInterface.PasswordFilterEx(username, fullname, password, isSetOperation);
        }

        public static void GetUserPolicy(string username, out UserPolicy policy)
        {
            FilterInterface.GetUserPolicySettings(username, out policy);
        }
    }
}
