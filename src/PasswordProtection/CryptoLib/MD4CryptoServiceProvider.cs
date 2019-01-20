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
    /// <inheritdoc />
    /// <summary>
    /// A MD4 implementation using windows native security api
    /// </summary>
    public class MD4CryptoServiceProvider : MD4
    {
        private IntPtr providerHandle;
        private IntPtr hashHandle;
        private bool disposed;

        public MD4CryptoServiceProvider()
        {
            this.providerHandle = IntPtr.Zero;
            this.hashHandle = IntPtr.Zero;
            this.HashSizeValue = 128;

            bool status = NativeMethods.CryptAcquireContext(ref this.providerHandle, null, null, ProviderType.PROV_RSA_FULL, CryptAcquireContextTypes.CRYPT_VERIFYCONTEXT | CryptAcquireContextTypes.CRYPT_SILENT);

            if (!status)
            {
                status = NativeMethods.CryptAcquireContext(ref this.providerHandle, null, null, ProviderType.PROV_RSA_FULL, CryptAcquireContextTypes.CRYPT_VERIFYCONTEXT | CryptAcquireContextTypes.CRYPT_SILENT | CryptAcquireContextTypes.CRYPT_NEWKEYSET);

                if (!status)
                {
                    throw new CryptographicException(Marshal.GetLastWin32Error());
                }
            }

            this.Setup();
        }

        /// <inheritdoc />
        public override void Initialize()
        {
            this.Setup();
        }

        private void Setup()
        {
            bool status = NativeMethods.CryptCreateHash(this.providerHandle, AlgId.CALG_MD4, IntPtr.Zero, 0, ref this.hashHandle);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }
        }

        /// <inheritdoc />
        protected override void HashCore(byte[] array, int ibStart, int cbSize)
        {
            byte[] input = new byte[cbSize];

            Array.Copy(array, ibStart, input, 0, cbSize);
            bool status = NativeMethods.CryptHashData(this.hashHandle, input, (uint)cbSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }
        }

        /// <inheritdoc />
        protected override byte[] HashFinal()
        {
            byte[] hashSizeBuffer = new byte[sizeof(int)];
            uint hashSize = (uint)hashSizeBuffer.Length;

            bool status = NativeMethods.CryptGetHashParam(this.hashHandle, HashParameters.HP_HASHSIZE, hashSizeBuffer, ref hashSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }

            hashSize = BitConverter.ToUInt32(hashSizeBuffer, 0);
            byte[] hashValue = new byte[hashSize];

            status = NativeMethods.CryptGetHashParam(this.hashHandle, HashParameters.HP_HASHVAL, hashValue, ref hashSize, 0);

            if (!status)
            {
                throw new CryptographicException(Marshal.GetLastWin32Error());
            }

            if (this.hashHandle != IntPtr.Zero)
            {
                NativeMethods.CryptDestroyHash(this.hashHandle);
                this.hashHandle = IntPtr.Zero;
            }

            return hashValue;
        }

        /// <inheritdoc />
        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (!this.disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.

                if (disposing)
                {
                    // Free managed resources & other reference types
                }

                if (this.hashHandle != IntPtr.Zero)
                {
                    NativeMethods.CryptDestroyHash(this.hashHandle);
                    this.hashHandle = IntPtr.Zero;
                }
                if (this.providerHandle != IntPtr.Zero)
                {
                    NativeMethods.CryptReleaseContext(this.providerHandle, 0);
                    this.providerHandle = IntPtr.Zero;
                }
            }

            this.disposed = true;
        }
    }
}