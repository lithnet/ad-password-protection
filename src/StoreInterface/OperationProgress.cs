using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace StoreInterface
{
    public class OperationProgress
    {
        private int hashesAdded;
        private int hashesDiscarded;
        private int linesReadFromFile;
        private int consolidateStorePosition;
        private int flushStorePosition;

        private string status;

        public int HashesAdded => this.hashesAdded;

        public int HashesDiscarded => this.hashesDiscarded;

        public int LinesReadFromFile => this.linesReadFromFile;

        public long FileReadPosition { get; set; }

        public long FileSizeTotal { get; set; }

        public int FileReadSecondsRemaining
        {
            get
            {
                double totalSeconds = (DateTime.Now - this.FileReadStartTime).TotalSeconds;

                if (totalSeconds > 0)
                {
                    double bytesPerSecond = this.FileReadPosition / totalSeconds;
                    long remainingBytes = this.FileSizeTotal - this.FileReadPosition;

                    if (bytesPerSecond > 0)
                    {
                        double result = remainingBytes / bytesPerSecond;

                        if (result < Int32.MaxValue && result > 0)
                        {
                            return Convert.ToInt32(result);
                        }
                    }
                }

                return 0;
            }
        }

        public bool FileReadInProgress { get; set; }

        public DateTime FileReadStartTime { get; set; }


        public int FileProgressPercent
        {
            get
            {
                if (this.FileSizeTotal <= 0)
                {
                    return 0;
                }

                return Convert.ToInt32(((double)this.FileReadPosition / this.FileSizeTotal) * 100);
            }
        }

        public int FlushStoreTotal { get; set; }

        public int FlushStorePosition
        {
            get => this.flushStorePosition;
            set => this.flushStorePosition = value;
        }

        public int FlushStorePercent => this.FlushStoreTotal > 0 ? (int)(((double)this.FlushStorePosition / this.FlushStoreTotal) * 100) : 0;

        public bool FlushStoreInProgress { get; set; }

        public DateTime FlushStoreStartTime { get; set; }

        public int FlushStoreSecondsRemaining
        {
            get
            {
                double totalSeconds = (DateTime.Now - this.FlushStoreStartTime).TotalSeconds;

                if (totalSeconds > 0)
                {
                    double itemsPerSecond = this.FlushStorePosition / totalSeconds;
                    long remaining = this.FlushStoreTotal - this.FlushStorePosition;

                    if (itemsPerSecond > 0)
                    {
                        double result = remaining / itemsPerSecond;

                        if (result < Int32.MaxValue && result > 0)
                        {
                            return Convert.ToInt32(result);
                        }
                    }
                }

                return 0;
            }
        }


        public int ConsolidateStoreTotal { get; set; }

        public int ConsolidateStorePosition
        {
            get => this.consolidateStorePosition;
            set => this.consolidateStorePosition = value;
        }

        public int ConsolidateStorePercent => this.ConsolidateStoreTotal > 0 ? (int)(((double)this.ConsolidateStorePosition / this.ConsolidateStoreTotal) * 100) : 0;

        public bool ConsolidateStoreInProgress { get; set; }

        public DateTime ConsolidateStoreStartTime { get; set; }
        
        public int ConsolidateStoreSecondsRemaining
        {
            get
            {
                double totalSeconds = (DateTime.Now - this.ConsolidateStoreStartTime).TotalSeconds;

                if (totalSeconds > 0)
                {
                    double itemsPerSecond = this.ConsolidateStorePosition / totalSeconds;
                    long remaining = this.ConsolidateStoreTotal - this.ConsolidateStorePosition;

                    if (itemsPerSecond > 0)
                    {
                        double result = remaining / itemsPerSecond;

                        if (result < Int32.MaxValue && result > 0)
                        {
                            return Convert.ToInt32(result);
                        }
                    }
                }

                return 0;
            }
        }

        public void IncrementConsolidateStorePosition()
        {
            Interlocked.Increment(ref this.consolidateStorePosition);
        }

        public void IncrementFlushStorePosition()
        {
            Interlocked.Increment(ref this.flushStorePosition);
        }

        public string Status
        {
            get => this.status;
            set
            {
                if (this.status != value)
                {
                    this.status = value;
                    Trace.WriteLine(this.status);
                }
            }
        }

        internal void IncrementHashesAdded(int count = 1)
        {
            Interlocked.Add(ref this.hashesAdded, count);
        }

        internal void IncrementHashesDiscarded(int count = 1)
        {
            Interlocked.Add(ref this.hashesDiscarded, count);
        }

        internal void IncrementTotalProcessed(int count = 1)
        {
            Interlocked.Add(ref this.linesReadFromFile, count);
        }

        public string GetProgressText()
        {
            return $"Found {this.linesReadFromFile:n0} objects. Committed {this.hashesAdded:n0} new records and discarded {this.hashesDiscarded:n0} duplicates";
        }
    }
}
