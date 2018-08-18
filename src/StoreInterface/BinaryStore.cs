using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace StoreInterface
{
    public abstract class BinaryStore : Store
    {
        public bool IsInBatch { get; private set; }

        public string StorePath { get; }

        protected BinaryStore(string storeBasePath, string storeSubPath, int hashSize)
        : base(hashSize)
        {
            if (!Directory.Exists(storeBasePath))
            {
                throw new DirectoryNotFoundException($"There was no store found at {storeBasePath}");
            }

            this.StorePath = Path.Combine(storeBasePath, storeSubPath);

            if (!Directory.Exists(this.StorePath))
            {
                Directory.CreateDirectory(this.StorePath);
            }
        }

        public override void ClearStore()
        {
            DirectoryInfo directory = new DirectoryInfo(this.StorePath);

            foreach (FileInfo file in directory.EnumerateFiles("*.db", SearchOption.TopDirectoryOnly))
            {
                file.Delete();
            }
        }

        public HashSet<byte[]> GetHashesFromStore(string range)
        {
            HashSet<byte[]> items = new HashSet<byte[]>(ByteArrayComparer.Comparer);
            int discarded = 0;
            string file = Path.Combine(this.StorePath, $"{range}.db");

            this.LoadHashesFromStoreFile(file, items, ref discarded);

            return items;
        }

        public override bool IsHashInStore(byte[] hash)
        {
            string file = Path.Combine(this.StorePath, $"{this.GetRangeFromHash(hash)}.db");
            return this.IsHashInStoreFile(file, hash);
        }

        protected override void AddHashRangeToStore(HashSet<byte[]> hashes, string range, ref int hashesAdded, ref int hashesDiscarded)
        {
            if (this.IsInBatch)
            {
                this.AddHashRangeToTempStore(hashes, range);
                return;
            }

            string file = Path.Combine(this.StorePath, $"{range}.db");

            bool writeFile = false;
            int hashesAlreadyInStore = 0;
            int newHashes = hashes.Count;

            if (File.Exists(file))
            {
                this.LoadHashesFromStoreFile(file, hashes, ref hashesAlreadyInStore);
            }
            else
            {
                writeFile = true;
            }

            int uniqueNewHashes = newHashes - hashesAlreadyInStore;

            if (uniqueNewHashes > 0)
            {
                writeFile = true;
            }
            else
            {
                //Trace.WriteLine($"No new hashes for store {range}.db");
            }

            Interlocked.Add(ref hashesAdded, uniqueNewHashes);
            Interlocked.Add(ref hashesDiscarded, hashesAlreadyInStore);

            if (writeFile)
            {
                this.WriteStoreFile(file, false, hashes);
            }
        }

        protected override void StartBatch()
        {
            this.IsInBatch = true;
        }

        protected override void EndBatch(ref int hashesAdded, ref int hashesDiscarded)
        {
            this.ConsolidateAndSort(ref hashesAdded, ref hashesDiscarded);
            this.IsInBatch = false;
        }

        private void AddHashRangeToTempStore(HashSet<byte[]> hashes, string range)
        {
            string file = Path.Combine(this.StorePath, $"{range}.db.bin");
            this.WriteStoreFile(file, true, hashes);
            this.IsInBatch = true;
        }

        public void ConsolidateAndSort(ref int hashesAdded, ref int hashesDiscarded)
        {
            Trace.WriteLine("Sorting store");

            int added = 0;
            int discarded = 0;

            Parallel.ForEach(Directory.EnumerateFiles(this.StorePath, "*.db.bin"),
                tempFile =>
                {
                    HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

                    string realFile = tempFile.Substring(0, tempFile.Length - 4);
                    Trace.WriteLine($"Consolidating {realFile}");

                    this.LoadHashesFromStoreFile(realFile, hashes, ref discarded);
                    int originalCount = hashes.Count;

                    this.LoadHashesFromStoreFile(tempFile, hashes, ref discarded);

                    added += hashes.Count - originalCount;

                    this.WriteStoreFile(realFile, false, hashes);
                    hashes.Clear();

                    File.Delete(tempFile);
                });

            hashesAdded += added;
            hashesDiscarded += discarded;
        }

        private void LoadHashesFromStoreFile(string file, HashSet<byte[]> hashes, ref int hashesDiscarded)
        {
            foreach (byte[] hash in this.LoadHashesFromStoreFile(file))
            {
                if (!hashes.Add(hash))
                {
                    hashesDiscarded++;
                }
            }
        }

        private IEnumerable<byte[]> LoadHashesFromStoreFile(string file)
        {
            FileInfo fileInfo = new FileInfo(file);

            if (!fileInfo.Exists)
            {
                yield break;
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

                    yield return raw;
                }
            }
        }

        private bool IsHashInStoreFile(string file, byte[] hash)
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

                    int result = ByteArrayComparer.Comparer.Compare(data, toFind);

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

        protected override IEnumerable<byte[]> GetHashes()
        {
            DirectoryInfo directory = new DirectoryInfo(this.StorePath);

            foreach (FileInfo file in directory.EnumerateFiles("*.db", SearchOption.TopDirectoryOnly))
            {
                foreach (byte[] item in this.LoadHashesFromStoreFile(file.FullName))
                {
                    yield return item;
                }
            }
        }

        private void WriteStoreFile(string file, bool append, IEnumerable<byte[]> hashes)
        {
            FileMode mode;
            string originalFile = file;
            string fileToOpen;

            if (append)
            {
                mode = FileMode.Append;
                fileToOpen = file;
            }
            else
            {
                mode = FileMode.Create;
                fileToOpen = file + ".tmp";
            }

            bool success = false;

            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Open(fileToOpen, mode, FileAccess.Write, FileShare.None)))
                {
                    foreach (byte[] item in hashes.OrderBy(t => t, ByteArrayComparer.Comparer))
                    {
                        if (item.Length != 20)
                        {
                            throw new InvalidDataException("The hash was not of the correct length");
                        }

                        writer.Write(item, this.HashOffset, this.HashSize);
                    }
                }

                success = true;
            }
            finally
            {
                if (mode == FileMode.Create)
                {
                    if (success)
                    {
                        if (File.Exists(originalFile))
                        {
                            File.Replace(fileToOpen, originalFile, null);
                        }
                        else
                        {
                            File.Move(fileToOpen, originalFile);
                        }
                    }
                    else
                    {
                        File.Delete(fileToOpen);
                    }
                }
            }
        }
    }
}