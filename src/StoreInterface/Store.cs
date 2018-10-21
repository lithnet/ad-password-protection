using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Threading.Tasks;

namespace StoreInterface
{
    public abstract class Store : IStore
    {
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

        public static void ImportPasswordsFromFile(Store store, StoreType storeType, string sourceFile, int batchSize = 5000000, OperationProgress progress = null)
        {
            bool batched = false;

            Stopwatch timer = new Stopwatch();
            timer.Start();

            Trace.WriteLine($"Loading plain-text passwords from {sourceFile}");
            if (progress == null)
            {
                progress = new OperationProgress();
            }

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            foreach (string line in GetLinesFromFile(sourceFile))
            {
                if (line.Length <= 0)
                {
                    continue;
                }

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

                if (!hashes.Add(hash))
                {
                    progress?.IncrementHashesDiscarded();
                }

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {progress.TotalProcessed} passwords");
                    timer.Restart();
                }

                if (batchSize > 0 && hashes.Count >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch(storeType);
                    }

                    Trace.WriteLine("Flushing batch to store");
                    store.AddToStore(hashes, storeType, progress);
                    hashes.Clear();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddToStore(hashes, storeType, progress);
            }

            if (batched)
            {
                Trace.WriteLine("Sorting hashes into store");
                store.EndBatch(storeType, progress);
            }

            Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine("Done");
        }

        public static void ImportHexHashesFromSortedFile(Store store, StoreType storeType, string sourceFile, OperationProgress progress = null)
        {
            string lastRange = null;
            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            Stopwatch timer = new Stopwatch();
            timer.Start();

            if (progress == null)
            {
                progress = new OperationProgress();
            }

            Trace.WriteLine($"Loading hexadecimal hashes from {sourceFile}");

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile, store.HashLength))
            {
                progress.IncrementTotalProcessed();

                string range = store.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    store.AddToStore(hashes, lastRange, storeType, progress);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddToStore(hashes, lastRange, storeType, progress);
                hashes.Clear();
            }

            Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine($"Done");
        }

        public static void ImportFromStore(Store targetStore, Store sourceStore, StoreType sourceStoreType, StoreType destinationStoreType, OperationProgress progress = null)
        {
            string lastRange = null;

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            Stopwatch timer = new Stopwatch();
            timer.Start();

            Trace.WriteLine($"Merging hashes from source store");
            if (progress == null)
            {
                progress = new OperationProgress();
            }

            foreach (byte[] hash in sourceStore.GetHashes(sourceStoreType))
            {
                progress.IncrementTotalProcessed();

                string range = targetStore.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    targetStore.AddToStore(hashes, lastRange, destinationStoreType, progress);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                targetStore.AddToStore(hashes, lastRange, destinationStoreType, progress);
                hashes.Clear();
            }

            Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine($"Done");
        }

        public static void ImportHexHashesFromFile(Store store, StoreType storeType, string sourceFile, int batchSize = 100000, OperationProgress progress = null)
        {
            bool batched = false;

            Trace.WriteLine($"Loading hexadecimal hashes from {sourceFile}");

            Stopwatch timer = new Stopwatch();
            timer.Start();
            if (progress == null)
            {
                progress = new OperationProgress();
            }

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile, store.HashLength))
            {
                progress.IncrementTotalProcessed();

                hashes.Add(hash);

                if (batchSize > 0 && hashes.Count >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch(storeType);
                    }

                    store.AddToStore(hashes, storeType, progress);
                    hashes.Clear();
                }

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddToStore(hashes, storeType, progress);
            }

            if (batched)
            {
                Trace.WriteLine("Sorting hashes into store");
                store.EndBatch(storeType, progress);
            }

            Trace.WriteLine($"Processed {progress.TotalProcessed} passwords. Added {progress.HashesAdded} new hashes. Discarded {progress.HashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine("Done");
        }

        private static IEnumerable<byte[]> GetHexHashesFromFile(string sourceFile, int hashBinaryLength)
        {
            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            int hashStringLength = hashBinaryLength * 2;

            foreach (string line in Store.GetLinesFromFile(sourceFile))
            {
                if (line.Length < hashStringLength)
                {
                    // not a SHA1 hash
                    continue;
                }

                yield return line.Substring(0, hashStringLength).HexStringToBytes();
            }
        }

        private static IEnumerable<string> GetLinesFromFile(string sourceFile)
        {
            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            using (StreamReader reader = new StreamReader(sourceFile))
            {
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();

                    if (line == null || line.Length <= 0)
                    {
                        continue;
                    }

                    yield return line;
                }
            }
        }

        protected abstract string GetRangeFromHash(string hash);

        protected abstract string GetRangeFromHash(byte[] hash);

        public abstract IEnumerable<byte[]> GetHashes(StoreType storeType);

        public abstract void ClearStore(StoreType storeType);

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
            this.AddToStore(hashset, storeType, null);
        }

        public void AddToStore(HashSet<byte[]> hashes, StoreType storeType, OperationProgress progress = null)
        {
            Parallel.ForEach(
                hashes
                    .OrderBy(t => t, ByteArrayComparer.Comparer)
                    .GroupBy(this.GetRangeFromHash, StringComparer.OrdinalIgnoreCase),
                (group) =>
                {
                    this.AddToStore(group, storeType, progress);
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

        public abstract bool IsInStore(byte[] hash, StoreType storeType);

        protected abstract void AddToStore(HashSet<byte[]> hashes, string range, StoreType storeType, OperationProgress progress);

        public abstract void StartBatch(StoreType storeType);

        public abstract byte[] ComputeHash(string text);

        public abstract void EndBatch(StoreType storeType, OperationProgress progress);

        public abstract HashSet<byte[]> GetHashes(string range, StoreType storeType);
    }
}
