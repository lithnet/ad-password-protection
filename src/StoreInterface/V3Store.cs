using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using Microsoft.Protocols.TestTools.StackSdk.Security.Cryptographic;
using System.Text;

namespace StoreInterface
{
    public class V3Store : BinaryStore
    {
        public V3Store(string storeBasePath)
        : base(storeBasePath, "v3\\p", "v3\\w", MD4.Create(), 16, 2)
        {
        }

        public override byte[] ComputeHash(string text)
        {
            return this.Encoder.ComputeHash(Encoding.Unicode.GetBytes(text));
        }
        
        protected override string GetRangeFromHash(string hash)
        {
            return hash.Substring(0, 4);
        }

        protected override string GetRangeFromHash(byte[] hash)
        {
            return hash.ToHexString(0, 2);
        }
    }
}
