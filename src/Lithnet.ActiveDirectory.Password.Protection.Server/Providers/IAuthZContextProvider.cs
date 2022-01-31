using System.Security.Principal;
using Lithnet.Security.Authorization;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IAuthZContextProvider
    {
        AuthorizationContext GetAuthZContext(string username);
        AuthorizationContext GetAuthZContext(SecurityIdentifier sid);
    }
}