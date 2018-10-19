using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public static class FilterInterface
    {
        public static PasswordTestResult TestPassword(string username, string fullname, string password, bool isSetOperation)
        {
            return NativeMethods.TestPassword(username, fullname, password, isSetOperation);
        }
    }
}
