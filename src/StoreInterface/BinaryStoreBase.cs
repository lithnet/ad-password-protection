using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace StoreInterface
{
    public abstract class BinaryStoreBase : IStore
    {
        public string StorePath { get; }

        protected int HashSize { get; }

        protected int HashOffset => 20 - this.HashSize;

        protected internal static ByteArrayComparer Comparer = new ByteArrayComparer();

        protected SHA1 Encoder = SHA1.Create();
       
        protected BinaryStoreBase(string store, string storeSubPath, int hashSize)
        {
            if (!Directory.Exists(store))
            {
                throw new DirectoryNotFoundException($"There was no store found at {store}");
            }

            this.StorePath = Path.Combine(store, storeSubPath);

            if (!Directory.Exists(this.StorePath))
            {
                Directory.CreateDirectory(this.StorePath);
            }

            this.HashSize = hashSize;
        }

        public void ClearStore()
        {
            DirectoryInfo directory = new DirectoryInfo(this.StorePath);

            foreach (FileInfo file in directory.EnumerateFiles("*.db", SearchOption.TopDirectoryOnly))
            {
                file.Delete();
            }
        }

        public void ImportPasswordsFromFile(string sourceFile, bool normalize, int batchSize = 100000, int reportingBatchSize = 10000)
        {
            int totalProcesssed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();

            HashSet<byte[]> hashes = new HashSet<byte[]>(Comparer);

            using (StreamReader reader = new StreamReader(sourceFile))
            {
                string line;

                while ((line = reader.ReadLine()) != null)
                {
                    if (line.Length <= 0)
                    {
                        continue;
                    }

                    byte[] hash = this.Encoder.ComputeHash(line);

                    hashes.Add(hash);

                    if (normalize)
                    {
                        byte[] newhash = this.Encoder.ComputeHash(StringNormalizer.Normalize(line));

                        if (BinaryStoreBase.Comparer.Equals(newhash, hash))
                        {
                            return;
                        }

                        hashes.Add(newhash);
                    }

                    if (hashes.Count >= batchSize)
                    {
                        this.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
                        hashes.Clear();
                    }

                    if (totalProcesssed % reportingBatchSize == 0)
                    {
                        Trace.WriteLine($"Processed {totalProcesssed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                        Console.Write($"Processed {totalProcesssed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                        timer.Restart();
                    }
                }
            }

            if (hashes.Count > 0)
            {
                this.AddHashesToStore(hashes, ref hashesAdded, ref hashesDiscarded);
                hashes.Clear();
            }
        }

        public void ImportHexHashesFromFile(string sourceFile, int reportingBatchSize = 10000)
        {
            string lastRange = null;
            int totalProcessed = 0;
            int hashesAdded = 0;
            int hashesDiscarded = 0;

            Stopwatch timer = new Stopwatch();

            if (!File.Exists(sourceFile))
            {
                throw new FileNotFoundException("The source file was not found", sourceFile);
            }

            HashSet<byte[]> hashes = new HashSet<byte[]>(BinaryStoreBase.Comparer);

            using (StreamReader reader = new StreamReader(sourceFile))
            {
                string line;

                while ((line = reader.ReadLine()) != null)
                {
                    if (line.Length < 40)
                    {
                        // not a SHA1 hash
                        continue;
                    }

                    line = line.Substring(0, 40);
                    string range = this.GetRangeFromHash(line);

                    if (range != lastRange && hashes.Count > 0)
                    {
                        this.AddHashRangeToStore(hashes, range, ref hashesAdded, ref hashesDiscarded);
                        hashes.Clear();
                    }

                    byte[] hash = new byte[20];

                    for (int i = 0; i < 20; i++)
                    {
                        hash[i] = Convert.ToByte(line.Substring((i * 2), 2), 16);
                    }

                    hashes.Add(hash);

                    lastRange = range;
                    totalProcessed++;

                    if (totalProcessed % reportingBatchSize == 0)
                    {
                        Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                        Console.Write($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                        timer.Restart();
                    }
                }
            }

            if (hashes.Count > 0)
            {
                this.AddHashRangeToStore(hashes, lastRange, ref hashesAdded, ref hashesDiscarded);
                hashes.Clear();
            }
        }

        public void AddPasswordToStore(string password, bool normalize)
        {
            byte[] hash = this.Encoder.ComputeHash(password);

            this.AddHashToStore(hash);

            if (normalize)
            {
                byte[] newhash = this.Encoder.ComputeHash(StringNormalizer.Normalize(password));

                if (BinaryStoreBase.Comparer.Equals(newhash, hash))
                {
                    return;
                }

                this.AddHashToStore(newhash);
            }
        }
        
        public void AddHashToStore(byte[] hash)
        {
            string range = this.GetRangeFromHash(hash);
            HashSet<byte[]> hashset = new HashSet<byte[]>(new ByteArrayComparer());
            hashset.Add(hash);
            int added = 0;
            int discarded = 0;
            this.AddHashRangeToStore(hashset, range, ref added, ref discarded);
        }

        public void AddHashesToStore(HashSet<byte[]> hashes, ref int hashesAdded, ref int hashesDiscarded, int reportingBatchSize = 10000)
        {
            int totalProcessed = 0;

            Stopwatch timer = new Stopwatch();

            foreach (IGrouping<string, byte[]> group in hashes.OrderBy(t => t, Comparer).GroupBy<byte[], string>(this.GetRangeFromHash, StringComparer.OrdinalIgnoreCase))
            {
                string range = group.Key;

                HashSet<byte[]> set = new HashSet<byte[]>(group, Comparer);
                this.AddHashRangeToStore(set, range, ref hashesAdded, ref hashesDiscarded);

                totalProcessed += set.Count;

                if (totalProcessed % reportingBatchSize == 0)
                {
                    Trace.WriteLine($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                    Console.Write($"Processed {totalProcessed} passwords. Added {hashesAdded} new hashes. Discarded {hashesDiscarded} duplicates. Duration: {timer.Elapsed}\r");
                    timer.Restart();
                }
            }
        }

        public HashSet<byte[]> GetHashesFromStore(string range)
        {
            HashSet<byte[]> items = new HashSet<byte[]>(Comparer);
            int discarded = 0;
            string file = Path.Combine(this.StorePath, $"{range}.db");

            this.LoadHashesFromStore(file, items, ref discarded);

            return items;
        }

        public bool IsPasswordInStore(string password, bool normalize = false)
        {
            if (normalize)
            {
                password = StringNormalizer.Normalize(password);
            }

            byte[] hash = this.Encoder.ComputeHash(password);

            return this.IsHashInStore(hash);
        }

        public bool IsHashInStore(byte[] hash)
        {
            string file = Path.Combine(this.StorePath, $"{this.GetRangeFromHash(hash)}.db");
            return this.IsHashInStore(file, hash);
        }

        
        protected abstract string GetRangeFromHash(string hash);

        protected abstract string GetRangeFromHash(byte[] hash);


        private void LoadHashesFromStore(string file, HashSet<byte[]> hashes, ref int hashesDiscarded)
        {
            FileInfo fileInfo = new FileInfo(file);

            if (!fileInfo.Exists)
            {
                return;
            }

            long size = fileInfo.Length;

            if (size % this.HashSize != 0)
            {
                throw new DataMisalignedException($"The store file {file} was corrupted");
            }


            byte[] rangeBytes = null;

            if (this.HashOffset > 0)
            {
                rangeBytes = new byte[this.HashOffset];
                string range = Path.GetFileNameWithoutExtension(file);

                for (int i = 0; i < 2; i++)
                {
                    rangeBytes[i] = Convert.ToByte(range.Substring((i * 2), 2), 16);
                }
            }

            using (BinaryReader reader = new BinaryReader(File.Open(file, FileMode.Open)))
            {
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    byte[] raw = reader.ReadBytes(this.HashSize);

                    if (rangeBytes != null)
                    {
                        byte[] hash = new byte[20];
                        Buffer.BlockCopy(rangeBytes, 0, hash, 0, this.HashOffset);
                        Buffer.BlockCopy(raw, 0, hash, this.HashOffset, raw.Length);
                        raw = hash;
                    }

                    if (!hashes.Add(raw))
                    {
                        hashesDiscarded++;
                    }
                }
            }
        }

        private bool IsHashInStore(string file, byte[] hash)
        {

            if (!File.Exists(file))
            {
                return false;
            }

            long firstRow = 0;

            using (BinaryReader reader = new BinaryReader(File.Open(file, FileMode.Open)))
            {
                long length = reader.BaseStream.Length;
                if (length % this.HashSize != 0)
                {
                    throw new DataMisalignedException($"The store file {file} was corrupted");
                }

                byte[] toFind;

                if (this.HashOffset > 0)
                {
                    toFind = new byte[this.HashSize];
                    Buffer.BlockCopy(hash, 2, toFind, 0, this.HashSize);
                }
                else
                {
                    toFind = hash;
                }

                long lastRow = length / this.HashSize;

                while (firstRow <= lastRow)
                {
                    long currentRow = (firstRow + lastRow) / 2;
                    reader.BaseStream.Position = currentRow * this.HashSize;

                    byte[] data = reader.ReadBytes(this.HashSize);

                    int result = Comparer.Compare(data, toFind);

                    if (result < 0)
                    {
                        firstRow = currentRow + 1;
                    }
                    else if (result > 0)
                    {
                        lastRow = currentRow - 1;
                    }
                    else
                    {
                        Debug.WriteLine($"Hash found at row {currentRow}");
                        return true;
                    }
                }
            }

            Debug.WriteLine($"Hash not found");

            return false;
        }

        private void AddHashRangeToStore(HashSet<byte[]> hashes, string range, ref int hashesAdded, ref int hashesDiscarded)
        {
            string file = Path.Combine(this.StorePath, $"{range}.db");

            bool writeFile = false;

            if (File.Exists(file))
            {
                this.LoadHashesFromStore(file, hashes, ref hashesDiscarded);
            }
            else
            {
                writeFile = true;
            }

            if ((hashes.Count - hashesDiscarded) > 0)
            {
                writeFile = true;
            }

            hashesAdded += hashes.Count;

            if (writeFile)
            {
                this.WriteStore(file, hashes);
            }
        }

        private void WriteStore(string file, HashSet<byte[]> hashes)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(file, FileMode.Create, FileAccess.Write, FileShare.None)))
            {
                foreach (byte[] item in hashes.OrderBy(t => t, BinaryStoreBase.Comparer))
                {
                    if (item.Length != 20)
                    {
                        throw new InvalidDataException("The hash was not of the correct length");
                    }

                    writer.Write(item, this.HashOffset, this.HashSize);
                }
            }
        }
    }
}