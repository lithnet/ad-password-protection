using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace StoreInterface
{
    public class V1Store : BinaryStore
    {
        public V1Store(string storeBasePath)
        : base(storeBasePath, "v1", SHA1.Create(), 20, 0)
        {
        }

        public override byte[] ComputeHash(string text)
        {
            return this.Encoder.ComputeHash(Encoding.UTF8.GetBytes(text));
        }

        protected override string GetRangeFromHash(string hash)
        {
            return hash.Substring(0, 5);
        }

        protected override string GetRangeFromHash(byte[] hash)
        {
            return hash.GetHexString(0, 3).Substring(0, 5);
        }
    }
}
