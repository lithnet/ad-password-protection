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

        public static void ImportPasswordsFromFile(Store store, string sourceFile, bool addExact, bool addNormalized, int batchSize = 5000000)
        {
            long totalProcesssed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;
            long hashesCreated = 0;
            bool batched = false;

            if (!(addExact || addNormalized))
            {
                throw new ArgumentException("You must specify at least one option of adding exact passwords or adding normalized passwords");
            }

            Stopwatch timer = new Stopwatch();
            timer.Start();

            Trace.WriteLine($"Loading plain-text passwords from {sourceFile}");

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            foreach (string line in GetLinesFromFile(sourceFile))
            {
                if (line.Length <= 0)
                {
                    continue;
                }

                byte[] hash = null;
                totalProcesssed++;

                if (addExact)
                {
                    hash = store.Encoder.ComputeHash(line);

                    if (hashes.Add(hash))
                    {
                        hashesCreated++;
                    }
                }

                if (addNormalized)
                {
                    string normalized = StringNormalizer.Normalize(line);
                    if (!string.IsNullOrEmpty(normalized))
                    {
                        byte[] newhash = store.Encoder.ComputeHash(normalized);

                        if (!ByteArrayComparer.Comparer.Equals(newhash, hash))
                        {
                            if (hashes.Add(newhash))
                            {
                                hashesCreated++;
                            }
                        }
                    }
                }

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {totalProcesssed} passwords into {hashesCreated} hashes");
                    timer.Restart();
                }

                if (batchSize > 0 && hashes.Count >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch();
                    }

                    Trace.WriteLine("Flushing batch to store");
                    store.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
                    hashes.Clear();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
            }

            if (batched)
            {
                Trace.WriteLine("Sorting hashes into store");
                store.EndBatch(ref hashesAdded, ref hashesDiscarded);
            }

            Trace.WriteLine($"Processed {totalProcesssed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine("Done");
        }

        public static void ImportHexHashesFromSortedFile(Store store, string sourceFile)
        {
            string lastRange = null;
            int totalProcessed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;
            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            Stopwatch timer = new Stopwatch();
            timer.Start();

            Trace.WriteLine($"Loading hexadecimal hashes from {sourceFile}");

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile))
            {
                totalProcessed++;

                string range = store.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    store.AddHashRangeToStore(hashes, lastRange, ref hashesAdded, ref hashesDiscarded);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddHashRangeToStore(hashes, lastRange, ref hashesAdded, ref hashesDiscarded);
                hashes.Clear();
            }

            Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine($"Done");
        }

        public static void ImportFromStore(Store targetStore, Store sourceStore)
        {
            string lastRange = null;
            int totalProcessed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;
            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            Stopwatch timer = new Stopwatch();
            timer.Start();

            Trace.WriteLine($"Merging hashes from source store");

            foreach (byte[] hash in sourceStore.GetHashes())
            {
                totalProcessed++;

                string range = targetStore.GetRangeFromHash(hash);

                if (range != lastRange && hashes.Count > 0)
                {
                    targetStore.AddHashRangeToStore(hashes, lastRange, ref hashesAdded, ref hashesDiscarded);
                    hashes.Clear();
                }

                hashes.Add(hash);

                lastRange = range;

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                targetStore.AddHashRangeToStore(hashes, lastRange, ref hashesAdded, ref hashesDiscarded);
                hashes.Clear();
            }

            Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine($"Done");
        }

        public static void ImportHexHashesFromFile(Store store, string sourceFile, int batchSize = 100000)
        {
            int totalProcessed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;
            bool batched = false;

            Trace.WriteLine($"Loading hexadecimal hashes from {sourceFile}");

            Stopwatch timer = new Stopwatch();
            timer.Start();

            HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

            foreach (byte[] hash in Store.GetHexHashesFromFile(sourceFile))
            {
                totalProcessed++;

                hashes.Add(hash);

                if (batchSize > 0 && hashes.Count >= batchSize)
                {
                    if (!batched)
                    {
                        batched = true;
                        store.StartBatch();
                    }

                    store.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
                    hashes.Clear();
                }

                if (timer.Elapsed.TotalSeconds > 5)
                {
                    Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
                    timer.Restart();
                }
            }

            if (hashes.Count > 0)
            {
                store.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
            }

            if (batched)
            {
                Trace.WriteLine("Sorting hashes into store");
                store.EndBatch(ref hashesAdded, ref hashesDiscarded);
            }

            Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}");
            Trace.WriteLine("Done");
        }

        private static IEnumerable<byte[]> GetHexHashesFromFile(string sourceFile)
        {
            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            foreach (string line in Store.GetLinesFromFile(sourceFile))
            {
                if (line.Length < 40)
                {
                    // not a SHA1 hash
                    continue;
                }

                yield return line.Substring(0, 40).GetSha1HashBytes();
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

        public abstract void ClearStore();

        public void AddPasswordToStore(string password, bool normalize)
        {
            byte[] hash = this.Encoder.ComputeHash(password);

            this.AddHashToStore(hash);

            if (normalize)
            {
                byte[] newhash = this.Encoder.ComputeHash(StringNormalizer.Normalize(password));

                if (ByteArrayComparer.Comparer.Equals(newhash, hash))
                {
                    return;
                }

                this.AddHashToStore(newhash);
            }
        }

        public void AddHashToStore(byte[] hash)
        {
            HashSet<byte[]> hashset = new HashSet<byte[]>(ByteArrayComparer.Comparer);
            hashset.Add(hash);
            int added = 0;
            int discarded = 0;
            this.AddHashesToStore(hashset, ref added, ref discarded);
        }

        public void AddHashesToStore(HashSet<byte[]> hashes, ref int hashesAdded, ref int hashesDiscarded)
        {
            int added = 0;
            int discarded = 0;

            Parallel.ForEach(
                hashes
                  .OrderBy(t => t, ByteArrayComparer.Comparer)
                 .GroupBy(this.GetRangeFromHash, StringComparer.OrdinalIgnoreCase),
                (group) =>
                {
                    this.AddHashesToStore(
                        group,
                        ref added,
                        ref discarded);
                });

            hashesAdded += added;
            hashesDiscarded += discarded;
        }

        private void AddHashesToStore(IGrouping<string, byte[]> group, ref int hashesAdded, ref int hashesDiscarded)
        {
            string range = group.Key;
            HashSet<byte[]> set = new HashSet<byte[]>(group, ByteArrayComparer.Comparer);
            this.AddHashRangeToStore(set, range, ref hashesAdded, ref hashesDiscarded);
        }

        protected abstract IEnumerable<byte[]> GetHashes();

        protected abstract void AddHashRangeToStore(HashSet<byte[]> hashes, string range, ref int hashesAdded, ref int hashesDiscarded);

        public abstract void StartBatch();

        public abstract void EndBatch(ref int hashesAdded, ref int hashesDiscarded);

        public bool IsPasswordInStore(string password, bool normalize = false)
        {
            if (normalize)
            {
                password = StringNormalizer.Normalize(password);
            }

            byte[] hash = this.Encoder.ComputeHash(password);

            return this.IsHashInStore(hash);
        }

        public abstract bool IsHashInStore(byte[] hash);
    }
}
