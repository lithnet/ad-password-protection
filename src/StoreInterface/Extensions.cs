using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace StoreInterface
{
    public static class Extensions
    {
        public static byte[] ComputeHash(this HashAlgorithm hash, string text)
        {
            return hash.ComputeHash(Encoding.UTF8.GetBytes(text));
        }

        public static string ComputeHashString(this HashAlgorithm hash, string text)
        {
            return hash.ComputeHash(text).GetHexString();
        }


        public static string GetHexString(this byte[] hash)
        {
            return hash.GetHexString(0, hash.Length);
        }

        public static string GetHexString(this byte[] hash, int offset, int count)
        {
            StringBuilder sb = new StringBuilder(hash.Length * 2);

            for (int i = offset; i < count; i++)
            {
                sb.Append(hash[i].ToString("X2"));
            }

            return sb.ToString();
        }

        public static byte[] GetSha1HashBytes(this string hexHash)
        {
            if (hexHash.Length != 40)
            {
                throw new ArgumentException("The value supplied must be a 40-character long hexadecimal representation of a SHA1 hash");
            }

            byte[] hash = new byte[20];

            for (int i = 0; i < 20; i++)
            {
                hash[i] = Convert.ToByte(hexHash.Substring((i * 2), 2), 16);
            }

            return hash;
        }
    }
}
