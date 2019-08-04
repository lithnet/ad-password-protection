using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public static class FilterInterface
    {
        private const uint ERROR_UNSUPPORTEDVERSION = 0xA0010001;

        private const int SupportedApiVersion = 1;

        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private static extern int PasswordFilterEx(string accountName, string fullName, string password, bool isSetOperation);

        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern uint GetUserPolicySettings(string accountName, int version, out IntPtr ppolicy);

        [DllImport("lithnetpwdf.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern void FreeUserPolicySettings(IntPtr ppolicy);

        public static PasswordTestResult TestPassword(string username, string fullname, string password, bool isSetOperation)
        {
            return (PasswordTestResult)FilterInterface.PasswordFilterEx(username, fullname, password, isSetOperation);
        }

        public static UserPolicy GetUserPolicy(string username)
        {
            uint result = FilterInterface.GetUserPolicySettings(username, SupportedApiVersion, out IntPtr ppolicy);

            if (result == ERROR_UNSUPPORTEDVERSION)
            {
                throw new ApiVersionMismatchException($"The API indicated a version mismatch. This library supports version {SupportedApiVersion}");
            }

            UserPolicy policy = Marshal.PtrToStructure<UserPolicy>(ppolicy);

            FilterInterface.FreeUserPolicySettings(ppolicy);

            return policy;
        }
    }
}
