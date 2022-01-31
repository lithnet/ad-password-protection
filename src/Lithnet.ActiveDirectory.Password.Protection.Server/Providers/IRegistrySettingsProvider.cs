namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IRegistrySettingsProvider
    {
        string StorePath { get; set; }
    }
}