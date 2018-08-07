using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public class ByteArrayComparer : IComparer<byte[]>, IEqualityComparer<byte[]>
    {
        public static ByteArrayComparer Comparer { get; } = new ByteArrayComparer();

        [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int memcmp(byte[] b1, byte[] b2, long count);

        public int Compare(byte[] x, byte[] y)
        {
            if (x == null && y == null)
            {
                return 0;
            }

            if (x == null)
            {
                return -1;
            }

            if (y == null)
            {
                return 1;
            }

            int bytesToCompare = Math.Min(x.Length, y.Length);

            int result = memcmp(x, y, bytesToCompare);
            if (x.Length == y.Length)
            {
                return result;
            }

            return x.Length < y.Length ? -1 : 1;
        }

        public bool Equals(byte[] x, byte[] y)
        {
            return this.Compare(x, y) == 0;
        }

        public int GetHashCode(byte[] obj)
        {
            unchecked
            {
                int hash = 17;

                foreach (byte b in obj)
                {
                    hash = (hash * 31) + b.GetHashCode();
                }

                return hash;
            }
        }
    }
}
