using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public interface IStore
    {
        void ClearStore(StoreType storeType);

        void RemoveFromStore(string password, StoreType storeType);

        void RemoveFromStore(byte[] hash, StoreType storeType);

        void RemoveFromStore(HashSet<byte[]> hashes, StoreType storeType, CancellationToken ct, OperationProgress progress);

        void AddToStore(string password, StoreType storeType);

        void AddToStore(byte[] hash, StoreType storeType);

        void AddToStore(HashSet<byte[]> hashes, StoreType storeType, CancellationToken ct, OperationProgress progress);

        bool IsInStore(string password, StoreType storeType);

        bool IsInStore(byte[] hash, StoreType storeType);
    }
}
