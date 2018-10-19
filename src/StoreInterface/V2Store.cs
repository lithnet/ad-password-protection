using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace StoreInterface
{
    public class V2Store : BinaryStore
    {
        public V2Store(string storeBasePath)
        : base(storeBasePath, "v2", SHA1.Create(), 20, 2)
        {
        }

        public override byte[] ComputeHash(string text)
        {
            return this.Encoder.ComputeHash(Encoding.UTF8.GetBytes(text));
        }

        protected override string GetRangeFromHash(string hash)
        {
            return hash.Substring(0, 4);
        }

        protected override string GetRangeFromHash(byte[] hash)
        {
            return hash.GetHexString(0, 2);
        }
    }
}
