/*
Windows-Protocol-Test-Suites

Copyright (c) Microsoft Corporation

All rights reserved. 

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ""Software""), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

using System;
using System.Runtime.InteropServices;
using System.Security.Cryptography;

namespace Microsoft.Protocols.TestTools.StackSdk.Security.Cryptographic
{
    /// <summary>
    /// A MD4 implementation using windows native security api
    /// </summary>
    public class MD4CryptoServiceProvider : MD4
    {
        private IntPtr providerHandle;
        private IntPtr hashHandle;
        private bool disposed;

        /// <summary>
        /// Initialize MD4CryptoServiceProvider
        /// </summary>
        public MD4CryptoServiceProvider()
        {
            providerHandle = IntPtr.Zero;
            hashHandle = IntPtr.Zero;
            //MD4's hash size is always 128, in bit;
            this.HashSizeValue = 128;

            bool status = NativeMethods.CryptAcquireContext(ref providerHandle, "MD4", null,
                ProviderType.PROV_RSA_AES, 0);

            if (!status)
            {
                status = NativeMethods.CryptAcquireContext(ref providerHandle, "MD4", null,
                    ProviderType.PROV_RSA_AES, CryptAcquireContextTypes.CRYPT_NEWKEYSET);
                if (!status)
                {
                    throw new CryptographicException(Marshal.GetLastWin32Error());
                }
            }

            this.Initialize();
        }


        /// <summary>
        /// Initialize the hash
        /// </summary>
        public override void Initialize()
        {
            bool status = NativeMethods.CryptCreateHash(providerHandle, AlgId.CALG_MD4, IntPtr.Zero,
                0, ref hashHandle);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }
        }


        /// <summary>
        /// routes data written to the object into
        /// the hash algorithm for computing the hash.
        /// </summary>
        /// <param name="array">The input to compute the hash code for.</param>
        /// <param name="ibStart">The offset into the byte array from which to begin using data.</param>
        /// <param name="cbSize">The number of bytes in the byte array to use as data.</param>
        protected override void HashCore(byte[] array, int ibStart, int cbSize)
        {
            bool status;
            byte[] input = new byte[cbSize];

            Array.Copy(array, ibStart, input, 0, cbSize);
            status = NativeMethods.CryptHashData(hashHandle, input, (uint)cbSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }
        }


        /// <summary>
        /// finalizes the hash computation after
        /// the last data is processed by the cryptographic stream object.
        /// </summary>
        /// <returns>The computed hash code.</returns>
        protected override byte[] HashFinal()
        {
            byte[] hashSizeBuffer = new byte[sizeof(int)];
            uint hashSize = (uint)hashSizeBuffer.Length;

            bool status = NativeMethods.CryptGetHashParam(hashHandle, HashParameters.HP_HASHSIZE, hashSizeBuffer,
                ref hashSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }

            hashSize = (uint)BitConverter.ToUInt32(hashSizeBuffer, 0);
            byte[] hashValue = new byte[hashSize];

            status = NativeMethods.CryptGetHashParam(hashHandle, HashParameters.HP_HASHVAL,
                hashValue, ref hashSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }

            if (hashHandle != IntPtr.Zero)
            {
                NativeMethods.CryptDestroyHash(hashHandle);
                hashHandle = IntPtr.Zero;
            }

            return hashValue;
        }


        /// <summary>
        /// release resources
        /// </summary>
        /// <param name="disposing">indicates GC or user calling this function</param>
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (!disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.

                if (disposing)
                {
                    // Free managed resources & other reference types
                }

                // Call the appropriate methods to clean up unmanaged resources.
                // If disposing is false, only the following code is executed.
                if (hashHandle != IntPtr.Zero)
                {
                    NativeMethods.CryptDestroyHash(hashHandle);
                    hashHandle = IntPtr.Zero;
                }
                if (providerHandle != IntPtr.Zero)
                {
                    NativeMethods.CryptReleaseContext(providerHandle, 0);
                    providerHandle = IntPtr.Zero;
                }
            }
            disposed = true;
        }
    }
}