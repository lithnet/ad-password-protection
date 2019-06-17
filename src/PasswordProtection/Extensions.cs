using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public static class Extensions
    {
       public static string ToHexString(this byte[] hash)
        {
            if (hash == null)
            {
                throw new ArgumentNullException(nameof(hash), "The binary has provided was null");
            }

            return hash.ToHexString(0, hash.Length);
        }

        public static string ToHexString(this byte[] hash, int offset, int count)
        {
            if (hash == null)
            {
                throw new ArgumentNullException(nameof(hash), "The binary has provided was null");
            }

            if (offset >= hash.Length)
            {
                throw new ArgumentException("The value for offset cannot exceed the length of the hash", nameof(offset));
            }

            if (count + offset > hash.Length)
            {
                throw new ArgumentException("The combined values of offset and count cannot exceed the length of the hash", nameof(offset));
            }

            StringBuilder sb = new StringBuilder(hash.Length * 2);

            for (int i = offset; i < count; i++)
            {
                sb.Append(hash[i].ToString("X2"));
            }

            return sb.ToString();
        }

        public static byte[] HexStringToBytes(this string hexHash)
        {
            if (hexHash == null)
            {
                throw new ArgumentNullException(nameof(hexHash));
            }

            if (hexHash.Length % 2 != 0)
            {
                throw new ArgumentException($"The value supplied must be a hexadecimal representation of the hash");
            }

            int binaryLength = hexHash.Length / 2;

            byte[] hash = new byte[binaryLength];

            for (int i = 0; i < binaryLength; i++)
            {
                hash[i] = Convert.ToByte(hexHash.Substring((i * 2), 2), 16);
            }

            return hash;
        }
    }
}
