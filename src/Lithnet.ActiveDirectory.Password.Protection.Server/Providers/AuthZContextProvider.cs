using System;
using System.Diagnostics;
using System.Security.Principal;
using Lithnet.Security.Authorization;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class AuthZContextProvider : IAuthZContextProvider
    {
        public AuthorizationContext GetAuthZContext(string username)
        {
            NTAccount account = new NTAccount(username);
            SecurityIdentifier sid = (SecurityIdentifier) account.Translate(typeof(SecurityIdentifier));

            return this.GetAuthZContext(sid);
        }

        public AuthorizationContext GetAuthZContext(SecurityIdentifier sid)
        {
            AuthorizationContext c = new AuthorizationContext(sid, null);
            return c;
        }

        [DebuggerStepThrough]
        public static bool TryParseAsSid(string s, out SecurityIdentifier sid)
        {
            sid = null;

            try
            {
                if (s == null || !s.StartsWith("S-", StringComparison.OrdinalIgnoreCase))
                {
                    return false;
                }

                sid = new SecurityIdentifier(s);
                return true;
            }
            catch
            {
                return false;
            }
        }
    }
}
