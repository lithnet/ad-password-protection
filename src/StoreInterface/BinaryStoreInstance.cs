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
    public class BinaryStoreInstance
    {
        public bool IsInBatch { get; private set; }

        public string StorePath { get; }

        protected int StoredHashLength { get; }

        protected int HashLength { get; }

        protected int HashOffset { get; }

        internal BinaryStoreInstance(string path, int hashLength, int hashOffset)
        {
            this.StorePath = path;
            this.HashLength = hashLength;
            this.HashOffset = hashOffset;
            this.StoredHashLength = hashLength - hashOffset;

            if (!Directory.Exists(this.StorePath))
            {
                Directory.CreateDirectory(this.StorePath);
            }
        }

        public void ClearStore()
        {
            DirectoryInfo directory = new DirectoryInfo(this.StorePath);

            foreach (FileInfo file in directory.EnumerateFiles("*.db", SearchOption.TopDirectoryOnly))
            {
                file.Delete();
            }

            foreach (FileInfo file in directory.EnumerateFiles("*.db.bin", SearchOption.TopDirectoryOnly))
            {
                file.Delete();
            }
        }

        public HashSet<byte[]> GetHashes(string range)
        {
            HashSet<byte[]> items = new HashSet<byte[]>(ByteArrayComparer.Comparer);
            string file = Path.Combine(this.StorePath, $"{range}.db");

            this.LoadHashesFromStoreFile(file, items, null);

            return items;
        }

        public bool IsHashInStore(byte[] hash)
        {
            string file = Path.Combine(this.StorePath, $"{this.GetRangeFromHash(hash)}.db");
            return this.IsHashInStoreFile(file, hash);
        }

        public void AddHashRangeToStore(HashSet<byte[]> hashes, string range, OperationProgress progress)
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
                this.LoadHashesFromStoreFile(file, hashes, progress);
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

            progress?.IncrementHashesAdded(uniqueNewHashes);
            progress?.IncrementHashesDiscarded(hashesAlreadyInStore);

            if (writeFile)
            {
                this.WriteStoreFile(file, false, hashes);
            }
        }

        public string GetRangeFromHash(string hash)
        {
            return hash.Substring(0, this.HashOffset * 2);
        }

        public string GetRangeFromHash(byte[] hash)
        {
            return hash.ToHexString(0, this.HashOffset);
        }

        public void StartBatch()
        {
            this.IsInBatch = true;
        }

        public void EndBatch(OperationProgress progress)
        {
            this.ConsolidateAndSort(progress);
            this.IsInBatch = false;
        }

        private void AddHashRangeToTempStore(HashSet<byte[]> hashes, string range)
        {
            string file = Path.Combine(this.StorePath, $"{range}.db.bin");
            this.WriteStoreFile(file, true, hashes);
            this.IsInBatch = true;
        }

        public void ConsolidateAndSort(OperationProgress progress)
        {
            Trace.WriteLine("Sorting store");

            Parallel.ForEach(Directory.EnumerateFiles(this.StorePath, "*.db.bin"),
                tempFile =>
                {
                    HashSet<byte[]> hashes = new HashSet<byte[]>(ByteArrayComparer.Comparer);

                    string realFile = tempFile.Substring(0, tempFile.Length - 4);
                    Trace.WriteLine($"Consolidating {realFile}");

                    this.LoadHashesFromStoreFile(realFile, hashes, progress);
                    int originalCount = hashes.Count;

                    this.LoadHashesFromStoreFile(tempFile, hashes, progress);

                    progress?.IncrementHashesAdded(hashes.Count - originalCount);

                    this.WriteStoreFile(realFile, false, hashes);
                    hashes.Clear();

                    File.Delete(tempFile);
                });
        }

        private void LoadHashesFromStoreFile(string file, HashSet<byte[]> hashes, OperationProgress progress)
        {
            foreach (byte[] hash in this.LoadHashesFromStoreFile(file))
            {
                if (!hashes.Add(hash))
                {
                    progress?.IncrementHashesDiscarded();
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

            if (size % this.StoredHashLength != 0)
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
                    byte[] raw = reader.ReadBytes(this.StoredHashLength);

                    if (rangeBytes != null)
                    {
                        byte[] hash = new byte[this.HashLength];
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

            using (BinaryReader reader = new BinaryReader(File.Open(file, FileMode.Open, FileAccess.Read)))
            {
                long length = reader.BaseStream.Length;

                if (length % this.StoredHashLength != 0)
                {
                    throw new DataMisalignedException($"The store file {file} was corrupted");
                }

                byte[] toFind;

                if (this.HashOffset > 0)
                {
                    toFind = new byte[this.StoredHashLength];
                    Buffer.BlockCopy(hash, 2, toFind, 0, this.StoredHashLength);
                }
                else
                {
                    toFind = hash;
                }

                long lastRow = length / this.StoredHashLength;

                while (firstRow <= lastRow)
                {
                    long currentRow = (firstRow + lastRow) / 2;
                    reader.BaseStream.Position = currentRow * this.StoredHashLength;

                    byte[] data = reader.ReadBytes(this.StoredHashLength);

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

        public IEnumerable<byte[]> GetHashes()
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
                        if (item.Length != this.HashLength)
                        {
                            throw new InvalidDataException("The hash was not of the correct length");
                        }

                        writer.Write(item, this.HashOffset, this.StoredHashLength);
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