using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public class HashRangeGroup : IGrouping<string, IEnumerable<byte[]>>
    {
        IEnumerable<IEnumerable<byte[]>> items;

        public HashRangeGroup(string key, IEnumerable<IEnumerable<byte[]>> items)
        {
            this.items = items;
        }

        public string Key { get; set; }

        public IEnumerator<IEnumerable<byte[]>> GetEnumerator()
        {
            return this.items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
    }
}
