using System.Collections.Generic;
using System.Threading;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public interface IStore
    {
        void ClearStore(StoreType storeType);

        void RemoveFromStore(string password, StoreType storeType);

        void RemoveFromStore(byte[] hash, StoreType storeType);

        void RemoveFromStore(HashSet<byte[]> hashes, StoreType storeType, OperationProgress progress, CancellationToken ct);

        void AddToStore(string password, StoreType storeType);

        void AddToStore(byte[] hash, StoreType storeType);

        void AddToStore(HashSet<byte[]> hashes, StoreType storeType, OperationProgress progress, CancellationToken ct);

        bool IsInStore(string password, StoreType storeType);

        bool IsInStore(byte[] hash, StoreType storeType);

        string GetStoreMetadata(string metadataItemName);

        void SetStoreMetadata(string metadataItemName, string data);

        void DeleteStoreMetadata(string metadataItemName);

        string GetPath();
    }
}
