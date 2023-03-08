using System.Text;
using Microsoft.Protocols.TestTools.StackSdk.Security.Cryptographic;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public class V3Store : BinaryStore
    {
        public const int BinaryHashLength = 16;

        public const int BinaryHashOffset = 2;

        public V3Store(string storeBasePath)
        : base(storeBasePath, "v3\\p", "v3\\w", MD4.Create(), V3Store.BinaryHashLength, V3Store.BinaryHashOffset)
        {
        }

        public override byte[] ComputeHash(string text)
        {
            return this.Encoder.ComputeHash(Encoding.Unicode.GetBytes(text));
        }

        protected override string GetRangeFromHash(string hash)
        {
            return hash.Substring(0, V3Store.BinaryHashOffset * 2);
        }

        protected override string GetRangeFromHash(byte[] hash)
        {
            return hash.ToHexString(0, V3Store.BinaryHashOffset);
        }
    }
}
