using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography;

namespace StoreInterface
{
    public class V2Store : BinaryStoreBase
    {
        public V2Store(string store)
        : base(store, "v2", 18)
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
