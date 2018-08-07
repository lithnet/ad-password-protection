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
        void ClearStore();

        void AddPasswordToStore(string password, bool normalize);

        void AddHashToStore(byte[] hash);

        void AddHashesToStore(HashSet<byte[]> hashes, ref int hashesAdded, ref int hashesDiscarded);

        bool IsPasswordInStore(string password, bool normalize = false);

        bool IsHashInStore(byte[] hash);
    }
}
