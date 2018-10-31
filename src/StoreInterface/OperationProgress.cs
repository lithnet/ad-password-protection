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
        private int totalProcessed;
        private string status;

        private long progressCurrentValue;
        private long progressTotalValue;

        public int HashesAdded => this.hashesAdded;

        public int HashesDiscarded => this.hashesDiscarded;

        public int TotalProcessed => this.totalProcessed;

        public long ProgressCurrentValue
        {
            get => this.progressCurrentValue;
            set => this.progressCurrentValue = value;
        }

        public long ProgressTotalValue
        {
            get => this.progressTotalValue;
            set => this.progressTotalValue = value;
        }

        public int ProgressPercent
        {
            get
            {
                if (this.ProgressTotalValue <= 0)
                {
                    return 0;
                }

                return Convert.ToInt32(((double)this.ProgressCurrentValue / this.ProgressTotalValue) * 100);
            }
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

        internal void IncrementProgressCurrentValue(int count = 1)
        {
            Interlocked.Add(ref this.progressCurrentValue, count);
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
            Interlocked.Add(ref this.totalProcessed, count);
        }

        public string GetProgressText()
        {
            var progress = this.ProgressTotalValue > 0 ? ((double)this.ProgressCurrentValue / this.ProgressTotalValue) : 0;

            if (progress > 0)
            {
                return $"Processed {this.totalProcessed:n0} objects ({progress:p1}). Added {this.hashesAdded:n0} new records and discarded {this.hashesDiscarded:n0} duplicates";
            }
            else
            {
                return $"Processed {this.totalProcessed:n0} objects. Added {this.hashesAdded:n0} new records and discarded {this.hashesDiscarded:n0} duplicates";
            }
        }
    }
}
