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

namespace Microsoft.Protocols.TestTools.StackSdk.Security.Cryptographic
{
    internal enum ProviderType
    {
        PROV_RSA_FULL = 1,
        PROV_RSA_SIG = 2,
        PROV_DSS = 3,
        PROV_FORTEZZA = 4,
        PROV_MS_EXCHANGE = 5,
        PROV_SSL = 6,
        PROV_RSA_SCHANNEL = 12,
        PROV_DSS_DH = 13,
        PROV_EC_ECDSA_SIG = 14,
        PROV_EC_ECNRA_SIG = 15,
        PROV_EC_ECDSA_FULL = 16,
        PROV_EC_ECNRA_FULL = 17,
        PROV_DH_SCHANNEL = 18,
        PROV_SPYRUS_LYNKS = 20,
        PROV_RNG = 21,
        PROV_INTEL_SEC = 22,
        PROV_REPLACE_OWF = 23,
        PROV_RSA_AES = 24,
    }

    [Flags()]
    internal enum CryptAcquireContextTypes : uint
    {
        CRYPT_NEWKEYSET = 0x00000008,
        CRYPT_DELETEKEYSET = 0x00000010,
        CRYPT_MACHINE_KEYSET = 0x00000020,
        CRYPT_SILENT = 0x00000040,
        CRYPT_DEFAULT_CONTAINER_OPTIONAL = 0x00000080,
        CRYPT_VERIFYCONTEXT = 0xF0000000,
    }

    internal enum AlgId : uint
    {
        /// <summary>
        /// No signature algorithm.
        /// </summary>
        CALG_NO_SIGN = 0x00002000,

        /// <summary>
        /// DSA public key signature algorithm.
        /// </summary>
        CALG_DSS_SIGN = 0x00002200,

        /// <summary>
        /// Elliptic curve digital signature algorithm.
        /// </summary>
        CALG_ECDSA = 0x00002203,

        /// <summary>
        /// RSA public key signature algorithm. This algorithm is supported by the Microsoft Base Cryptographic
        /// Provider.
        /// </summary>
        CALG_RSA_SIGN = 0x00002400,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL3_MASTER = 0x00004c01,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_MASTER_HASH = 0x00004c02,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_MAC_KEY = 0x00004c03,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_PCT1_MASTER = 0x00004c04,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL2_MASTER = 0x00004c05,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_TLS1_MASTER = 0x00004c06,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_ENC_KEY = 0x00004c07,

        /// <summary>
        /// DES encryption algorithm.
        /// </summary>
        CALG_DES = 0x00006601,

        /// <summary>
        /// RC2 block encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_RC2 = 0x00006602,

        /// <summary>
        /// Triple DES encryption algorithm.
        /// </summary>
        CALG_3DES = 0x00006603,

        /// <summary>
        /// DESX encryption algorithm.
        /// </summary>
        CALG_DESX = 0x00006604,

        /// <summary>
        /// Two-key triple DES encryption with effective key length equal to 112 bits.
        /// </summary>
        CALG_3DES_112 = 0x00006609,

        /// <summary>
        /// Skipjack block encryption algorithm (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_SKIPJACK = 0x0000660a,

        /// <summary>
        /// TEK (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_TEK = 0x0000660b,

        /// <summary>
        /// An algorithm to create a 40-bit DES key that has parity bits and zeroed key bits to make its key
        /// length 64 bits. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_CYLINK_MEK = 0x0000660c,

        /// <summary>
        /// RC5 block encryption algorithm.
        /// </summary>
        CALG_RC5 = 0x0000660d,

        /// <summary>
        /// 128 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
        /// </summary>
        CALG_AES_128 = 0x0000660e,

        /// <summary>
        /// 192 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
        /// </summary>
        CALG_AES_192 = 0x0000660f,

        /// <summary>
        /// 256 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
        /// </summary>
        CALG_AES_256 = 0x00006610,

        /// <summary>
        /// Advanced Encryption Standard (AES). This algorithm is supported by the Microsoft AES Cryptographic
        /// Provider.
        /// </summary>
        CALG_AES = 0x00006611,

        /// <summary>
        /// RC4 stream encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_RC4 = 0x00006801,

        /// <summary>
        /// SEAL encryption algorithm. This algorithm is not supported.
        /// </summary>
        CALG_SEAL = 0x00006802,

        /// <summary>
        /// MD2 hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_MD2 = 0x00008001,

        /// <summary>
        /// MD4 hashing algorithm.
        /// </summary>
        CALG_MD4 = 0x00008002,

        /// <summary>
        /// MD5 hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_MD5 = 0x00008003,

        /// <summary>
        /// Same as CALG_SHA. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_SHA1 = 0x00008004,

        /// <summary>
        /// SHA hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_SHA = CALG_SHA1,

        /// <summary>
        /// MAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_MAC = 0x00008005,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL3_SHAMD5 = 0x00008008,

        /// <summary>
        /// HMAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_HMAC = 0x00008009,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_TLS1PRF = 0x0000800a,

        /// <summary>
        /// One way function hashing algorithm.
        /// </summary>
        CALG_HASH_REPLACE_OWF = 0x0000800b,

        /// <summary>
        /// 256 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES
        /// Cryptographic Provider.
        /// </summary>
        CALG_SHA_256 = 0x0000800c,

        /// <summary>
        /// 384 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES
        /// Cryptographic Provider.
        /// </summary>
        CALG_SHA_384 = 0x0000800d,

        /// <summary>
        /// 512 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES
        /// Cryptographic Provider.
        /// </summary>
        CALG_SHA_512 = 0x0000800e,

        /// <summary>
        /// Elliptic curve Menezes, Qu, and Vanstone (MQV) key exchange algorithm. This algorithm is not supported.
        /// </summary>
        CALG_ECMQV = 0x0000a001,

        /// <summary>
        /// Hughes MD5 hashing algorithm.
        /// </summary>
        CALG_HUGHES_MD5 = 0x0000a003,

        /// <summary>
        /// RSA public key exchange algorithm. This algorithm is supported by the Microsoft Base Cryptographic
        /// Provider.
        /// </summary>
        CALG_RSA_KEYX = 0x0000a400,

        /// <summary>
        /// Diffie-Hellman store and forward key exchange algorithm.
        /// </summary>
        CALG_DH_SF = 0x0000aa01,

        /// <summary>
        /// Diffie-Hellman ephemeral key exchange algorithm.
        /// </summary>
        CALG_DH_EPHEM = 0x0000aa02,

        /// <summary>
        /// KEA key exchange algorithm (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_KEA_KEYX = 0x0000aa04,

        /// <summary>
        /// Elliptic curve Diffie-Hellman key exchange algorithm.
        /// </summary>
        CALG_ECDH = 0x0000aa05,

        /// <summary>
        /// The algorithm is defined in the encoded parameters. The algorithm is only supported by using CNG.
        /// The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to determine whether a cryptography algorithm is
        /// only supported by using the CNG functions.
        /// </summary>
        CALG_OID_INFO_PARAMETERS = 0xfffffffe,

        /// <summary>
        /// The algorithm is only implemented in CNG. The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to
        /// determine whether a cryptography algorithm is only supported by using the CNG functions.
        /// </summary>
        CALG_OID_INFO_CNG_ONLY = 0xffffffff,
    }

    internal enum HashParameters
    {
        HP_ALGID = 0x0001,   // Hash algorithm
        HP_HASHVAL = 0x0002, // Hash value
        HP_HASHSIZE = 0x0004 // Hash value size
    }

    internal static class NativeMethods
    {
        [DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptAcquireContext(ref IntPtr hProv, string pszContainer, string pszProvider, ProviderType dwProvType, CryptAcquireContextTypes dwFlags);

        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptCreateHash(IntPtr hProv, AlgId algId, IntPtr hKey, uint dwFlags, ref IntPtr phHash);

        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptHashData(IntPtr hHash, byte[] pbData, uint dataLen, uint flags);

        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptDestroyHash(IntPtr hHash);

        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptGetHashParam(IntPtr hHash, HashParameters dwParam, [Out] byte[] pbData, ref uint pdwDataLen, uint dwFlags);

        [DllImport("advapi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptReleaseContext(IntPtr hProv, uint dwFlags);
    }
}