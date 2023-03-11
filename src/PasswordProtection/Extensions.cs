using System;
using System.Globalization;
using System.Linq;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public static class Extensions
    {
        public static string ToHexLppPrefixString(this int i)
        {
            return i.ToString("X4", CultureInfo.InvariantCulture);
        }

        public static string ToHexHibpPrefixString(this int i)
        {
            return i.ToString("X5", CultureInfo.InvariantCulture);
        }

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
                sb.Append(hash[i].ToString("X2", CultureInfo.InvariantCulture));
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
                throw new ArgumentException("The value supplied must be a hexadecimal representation of the hash");
            }

            int binaryLength = hexHash.Length / 2;

            byte[] hash = new byte[binaryLength];

            for (int i = 0; i < binaryLength; i++)
            {
                hash[i] = Convert.ToByte(hexHash.Substring(i * 2, 2), 16);
            }

            return hash;
        }
    }
}
