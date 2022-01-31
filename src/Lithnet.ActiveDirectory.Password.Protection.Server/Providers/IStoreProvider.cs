namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public interface IStoreProvider
    {
        IStore Store { get; }
    }
}