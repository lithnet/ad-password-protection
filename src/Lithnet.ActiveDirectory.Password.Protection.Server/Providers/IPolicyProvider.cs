using System.Security.Principal;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IPolicyProvider
    {
        Policy GetPolicy(string username);
        Policy GetPolicy(SecurityIdentifier sid);
    }
}