using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;

namespace StoreInterface
{
    public abstract class Store : IStore
    {
        private const int DefaultBatchSize = 5000000;

        protected internal HashAlgorithm Encoder { get; }

        protected int StoredHashLength { get; }

        protected int HashLength { get; }

        protected int HashOffset { get; }

        protected Store(HashAlgorithm encoder, int hashLength, int hashOffset)
        {
            this.StoredHashLength = hashLength - hashOffset;
            this.HashLength = hashLength;
            this.HashOffset = hashOffset;
            this.Encoder = encoder;
        }

        public static void ImportPasswordsFromFile(Store store, StoreType storeType, string sourceFile, CancellationToken ct, int batchSize = DefaultBatchSize, OperationProgress progress = null)
        {
            bool batched = false;

            if (progress == null)
            {
                progress = new OperationProgress();
            }

            if (batchSize < 0)
            {
                batchSize = DefaultBatchSize;
            }

            progress.Status = $"Loading plain-text passwords from {sourceFile}";

            Dictionary<string, HashSet<byte[]>> importData = new Dictionary<string, HashSet<byte[]>>(store.HashOffset ^ 16);

            int currentCount = 0;

            foreach (string line in GetLinesFromFile(sourceFile, progress))
            {
                ct.ThrowIfCancellationRequested();

                if (line.Length <= 0)
                {
                    continue;
                }
                progress.Status = "Reading lines from file";

                byte[] hash = null;
                progress?.IncrementTotalProcessed();

                if (storeType == StoreType.Word)
                {
                    hash = store.ComputeHash(StringNormalizer.Normalize(line));
                }
                else
                {
                    hash = store.ComputeHash(line);
                }

                string range = store.GetRangeFromHash(hash);

                if (!importData.TryGetValue(range, out HashSet<byte[]> rangeStore))
                {
                    rangeStore = new HashSet<byte[]>(ByteArrayComparer.Comparer);
                    importData.Add(range, rangeStore);
                }

                if (!rangeStore.Add(hash))
                {
                    progress?.IncrementHashesDiscarded();
                }
                else
                {
                    currentCount++;
                }

                if (batchSize > 0 && currentCount >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch(storeType);
                    }

                    progress.Status = "Flushing batch to store";
                    store.AddToStore(importData, storeType, ct, true, progress);
                    currentCount = 0;
                }
            }

            if (currentCount > 0)
            {
                progress.Status = "Flushing batch to store";
                store.AddToStore(importData, storeType, ct, true, progress);
            }

            if (batched)
            {
                progress.Status = "Sorting hashes into store";
                store.EndBatch(storeType, ct, progress);
            }

            progress.Status = "Done";
        }

        public static void ImportHexHashesFromSortedFile(Store store, StoreType storeType, string sourceFile, CancellationToken ct, OperationProgress progress = null)
        {
            string lastRange = null;
            HashSet<byte[]> hashes = new HashSet<byte[]>(10000, ByteArrayComparer.Comparer);

            if (progress == null)
            {
                progress = new OperationProgress();
            }

            progress.Status = $"Loading sorted hexadecimal hashes from {sourceFile}";

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile, store.HashLength, progress))
            {
                ct.ThrowIfCancellationRequested();
                progress.IncrementTotalProcessed();

                string range = store.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    store.AddToStore(hashes, lastRange, storeType, progress);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;
            }

            if (hashes.Count > 0)
            {
                store.AddToStore(hashes, lastRange, storeType, progress);
                hashes.Clear();
            }

            progress.Status = "Done";
        }

        public static void ImportFromStore(Store targetStore, Store sourceStore, StoreType sourceStoreType, StoreType destinationStoreType, CancellationToken ct, OperationProgress progress = null)
        {
            string lastRange = null;

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            if (progress == null)
            {
                progress = new OperationProgress();
            }

            progress.Status = $"Merging hashes from source store";

            foreach (byte[] hash in sourceStore.GetHashes(sourceStoreType))
            {
                ct.ThrowIfCancellationRequested();
                progress.IncrementTotalProcessed();

                string range = targetStore.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    targetStore.AddToStore(hashes, lastRange, destinationStoreType, progress);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;
            }

            if (hashes.Count > 0)
            {
                targetStore.AddToStore(hashes, lastRange, destinationStoreType, progress);
                hashes.Clear();
            }

            progress.Status = "Done";
        }

        public static void ImportHexHashesFromFile(Store store, StoreType storeType, string sourceFile, CancellationToken ct, int batchSize = DefaultBatchSize, OperationProgress progress = null)
        {
            bool batched = false;

            if (progress == null)
            {
                progress = new OperationProgress();
            }

            if (batchSize < 0)
            {
                batchSize = DefaultBatchSize;
            }

            progress.Status = $"Loading unsorted hexadecimal hashes from {sourceFile}";

            HashSet<byte[]> hashes = new HashSet<byte[]>(batchSize, ByteArrayComparer.Comparer);

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile, store.HashLength, progress))
            {
                ct.ThrowIfCancellationRequested();
                progress.IncrementTotalProcessed();

                hashes.Add(hash);

                if (batchSize > 0 && hashes.Count >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch(storeType);
                    }

                    store.AddToStore(hashes, storeType, ct, progress);
                    hashes.Clear();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddToStore(hashes, storeType, ct, progress);
            }

            if (batched)
            {
                progress.Status = $"Sorting hashes into store";
                store.EndBatch(storeType, ct, progress);
            }

            progress.Status = "Done";
        }

        public static bool DoesHexHashFileAppearSorted(string sourceFile, int hashBinaryLength)
        {
            int count = 0;
            byte[] lastHash = null;
            ByteArrayComparer byteArrayComparer = new ByteArrayComparer();

            foreach (var item in Store.GetHexHashesFromFile(sourceFile, hashBinaryLength, new OperationProgress()))
            {
                count++;

                if (count > 1000)
                {
                    break;
                }

                if (byteArrayComparer.Compare(item, lastHash) < 0)
                {
                    return false;
                }

                lastHash = item;
            }

            return true;
        }

        private static IEnumerable<byte[]> GetHexHashesFromFile(string sourceFile, int hashBinaryLength, OperationProgress progress)
        {
            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            int hashStringLength = hashBinaryLength * 2;

            foreach (string line in Store.GetLinesFromFile(sourceFile, progress))
            {
                if (line.Length < hashStringLength)
                {
                    continue;
                }

                yield return line.Substring(0, hashStringLength).HexStringToBytes();
            }
        }

        private static IEnumerable<string> GetLinesFromFile(string sourceFile, OperationProgress progress)
        {
            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            using (StreamReader reader = new StreamReader(sourceFile))
            {
                progress.ProgressTotalValue = reader.BaseStream.Length;

                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    progress.ProgressCurrentValue = reader.BaseStream.Position;

                    if (line == null || line.Length <= 0)
                    {
                        continue;
                    }

                    yield return line;
                }
            }
        }

        public void AddToStore(string password, StoreType storeType)
        {
            if (storeType == StoreType.Word)
            {
                password = StringNormalizer.Normalize(password);
            }

            this.AddToStore(this.ComputeHash(password), storeType);
        }

        public void AddToStore(byte[] hash, StoreType storeType)
        {
            HashSet<byte[]> hashset = new HashSet<byte[]>(ByteArrayComparer.Comparer);
            hashset.Add(hash);
            this.AddToStore(hashset, storeType, new CancellationToken(), null);
        }

        public void AddToStore(HashSet<byte[]> hashes, StoreType storeType, CancellationToken ct, OperationProgress progress = null)
        {
            ParallelOptions o = new ParallelOptions();
            o.CancellationToken = ct;
            if (System.Diagnostics.Debugger.IsAttached)
            {
                o.MaxDegreeOfParallelism = 1;
            }

            Parallel.ForEach(
                hashes
                    .OrderBy(t => t, ByteArrayComparer.Comparer)
                    .GroupBy(this.GetRangeFromHash, StringComparer.OrdinalIgnoreCase),
                o,
                group => this.AddToStore(group, storeType, progress));
        }

        public void AddToStore(Dictionary<string, HashSet<byte[]>> hashes, StoreType storeType, CancellationToken ct, bool emptyAfterCommit, OperationProgress progress = null)
        {
            ParallelOptions o = new ParallelOptions();
            o.CancellationToken = ct;
            if (System.Diagnostics.Debugger.IsAttached)
            {
                o.MaxDegreeOfParallelism = 1;
            }

            Parallel.ForEach(hashes, o, group =>
            {
                this.AddToStore(group.Value, group.Key, storeType, progress);
                if (emptyAfterCommit)
                {
                    group.Value.Clear();
                }
            });
        }

        public bool IsInStore(string password, StoreType storeType)
        {
            if (storeType == StoreType.Word)
            {
                password = StringNormalizer.Normalize(password);
            }

            return this.IsInStore(this.ComputeHash(password), storeType);
        }

        private void AddToStore(IGrouping<string, byte[]> group, StoreType storeType, OperationProgress progress)
        {
            string range = group.Key;
            HashSet<byte[]> set = new HashSet<byte[]>(group, ByteArrayComparer.Comparer);
            this.AddToStore(set, range, storeType, progress);
        }

        protected abstract string GetRangeFromHash(string hash);

        protected abstract string GetRangeFromHash(byte[] hash);

        public abstract IEnumerable<byte[]> GetHashes(StoreType storeType);

        public abstract int GetHashCount(StoreType storeType, string range);

        public abstract void ClearStore(StoreType storeType);

        public abstract bool IsInStore(byte[] hash, StoreType storeType);

        protected abstract void AddToStore(HashSet<byte[]> hashes, string range, StoreType storeType, OperationProgress progress);

        public abstract void StartBatch(StoreType storeType);

        public abstract byte[] ComputeHash(string text);

        public abstract void EndBatch(StoreType storeType, CancellationToken ct, OperationProgress progress);

        public abstract HashSet<byte[]> GetHashes(string range, StoreType storeType);
    }
}
