using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;

namespace StoreInterface
{
    public class V3Store : BinaryStore
    {
        public V3Store(string storeBasePath)
        : base(storeBasePath, "v3", SHA1.Create(), 16, 2)
        {
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
