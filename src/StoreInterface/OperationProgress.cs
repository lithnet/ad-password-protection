using System;
using System.Collections.Generic;
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

        public int HashesAdded => this.hashesAdded;

        public int HashesDiscarded => this.hashesDiscarded;

        public int TotalProcessed => this.totalProcessed;

        public string Status { get; internal set; }

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
    }
}
