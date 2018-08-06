using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public interface IStore
    {
        void ClearStore();

        void ImportPasswordsFromFile(string sourceFile, bool normalize, int batchSize = 100000, int reportingBatchSize = 10000);

        void ImportHexHashesFromFile(string sourceFile, int reportingBatchSize = 10000);

        void AddPasswordToStore(string password, bool normalize);

        void AddHashToStore(byte[] hash);

        void AddHashesToStore(HashSet<byte[]> hashes, ref int hashesAdded, ref int hashesDiscarded, int reportingBatchSize = 10000);

        bool IsPasswordInStore(string password, bool normalize = false);

        bool IsHashInStore(byte[] hash);
    }
}
