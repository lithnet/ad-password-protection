using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Security.Cryptography;

namespace StoreInterface
{
    public abstract class BinaryStore : Store
    {
        public string StorePathPasswordStore { get; }

        public string StorePathWordStore { get; }

        private BinaryStoreInstance PasswordStoreInstance { get; }

        private BinaryStoreInstance WordStoreInstance { get; }

        protected BinaryStore(string storeBasePath, string storeSubPathPasswords, string storeSubPathWords, HashAlgorithm encoder, int hashLength, int hashOffset)
            : base(encoder, hashLength, hashOffset)
        {
            if (!Directory.Exists(storeBasePath))
            {
                throw new DirectoryNotFoundException($"There was no store found at {storeBasePath}");
            }

            this.StorePathPasswordStore = Path.Combine(storeBasePath, storeSubPathPasswords);

            this.PasswordStoreInstance = new BinaryStoreInstance(this.StorePathPasswordStore, hashLength, hashOffset);

            this.StorePathWordStore = Path.Combine(storeBasePath, storeSubPathWords);

            this.WordStoreInstance = new BinaryStoreInstance(this.StorePathWordStore, hashLength, hashOffset);
        }

        public override void ClearStore(StoreType storeType)
        {
            this.GetInstance(storeType).ClearStore();
        }

        protected override void AddToStore(HashSet<byte[]> hashes, string range, StoreType storeType, OperationProgress progress)
        {
            this.GetInstance(storeType).AddHashRangeToStore(hashes, range, progress);
        }

        public override bool IsInStore(byte[] hash, StoreType storeType)
        {
            return this.GetInstance(storeType).IsHashInStore(hash);
        }

        public override IEnumerable<byte[]> GetHashes(StoreType storeType)
        {
            return this.GetInstance(storeType).GetHashes();
        }

        public override void StartBatch(StoreType storeType)
        {
            this.GetInstance(storeType).StartBatch();
        }

        public override int GetHashCount(StoreType storeType, string range)
        {
            return this.GetInstance(storeType).GetHashCount(range);
        }

        public override void EndBatch(StoreType storeType, CancellationToken ct, OperationProgress progress)
        {
            this.GetInstance(storeType).EndBatch(progress,ct);
        }

        public override HashSet<byte[]> GetHashes(string range, StoreType storeType)
        {
            return this.GetInstance(storeType).GetHashes(range);
        }

        private BinaryStoreInstance GetInstance(StoreType storeType)
        {
            return storeType == StoreType.Password ? this.PasswordStoreInstance : this.WordStoreInstance;
        }
    }
}