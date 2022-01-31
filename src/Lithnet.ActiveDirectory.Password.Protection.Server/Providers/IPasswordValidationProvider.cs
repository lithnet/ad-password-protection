namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Providers
{
    public interface IPasswordValidationProvider
    {
        PasswordValidationResponse ValidatePassword(PasswordValidationRequest request);
    }
}