using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public interface IStore
    {
        void ClearStore(StoreType storeType);

        void AddToStore(string password, StoreType storeType);
        
        void AddToStore(byte[] hash, StoreType storeType);
        
        void AddToStore(HashSet<byte[]> hashes, StoreType storeType, OperationProgress progress);
        
        bool IsInStore(string password, StoreType storeType);

        bool IsInStore(byte[] hash, StoreType storeType);
    }
}
