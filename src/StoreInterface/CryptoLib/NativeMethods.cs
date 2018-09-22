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
using System.Diagnostics.CodeAnalysis;
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
        CRYPT_VERIFYCONTEXT = 0xF0000000,
        CRYPT_NEWKEYSET = 0x00000008,
        CRYPT_DELETEKEYSET = 0x00000010,
        CRYPT_MACHINE_KEYSET = 0x00000020,
        CRYPT_SILENT = 0x00000040,
        CRYPT_DEFAULT_CONTAINER_OPTIONAL = 0x00000080,
    }


    /// <summary>
    /// The ALG_ID data type specifies an algorithm identifier. 
    /// Parameters of this data type are passed to most of the functions in CryptoAPI.
    /// please refer: http://msdn.microsoft.com/en-us/library/aa375549(v=VS.85).aspx
    /// </summary>
    [SuppressMessage("Microsoft.Design", "CA1028:EnumStorageShouldBeInt32"),
    SuppressMessage("Microsoft.Design", "CA1008:EnumsShouldHaveZeroValue")]
    public enum AlgId : uint
    {
        /// <summary>
        /// Triple DES encryption algorithm.
        /// </summary>
        CALG_3DES = 0x00006603,

        /// <summary>
        /// Two-key triple DES encryption with effective key length equal to 112 bits.
        /// </summary>
        CALG_3DES_112 = 0x00006609,

        /// <summary>
        /// Advanced Encryption Standard (AES). This algorithm is supported by the Microsoft AES Cryptographic
        /// Provider.
        /// </summary>
        CALG_AES = 0x00006611,

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
        /// An algorithm to create a 40-bit DES key that has parity bits and zeroed key bits to make its key 
        /// length 64 bits. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_CYLINK_MEK = 0x0000660c,

        /// <summary>
        /// DES encryption algorithm.
        /// </summary>
        CALG_DES = 0x00006601,

        /// <summary>
        /// DESX encryption algorithm.
        /// </summary>
        CALG_DESX = 0x00006604,

        /// <summary>
        /// Diffie-Hellman ephemeral key exchange algorithm.
        /// </summary>
        CALG_DH_EPHEM = 0x0000aa02,

        /// <summary>
        /// Diffie-Hellman store and forward key exchange algorithm.
        /// </summary>
        CALG_DH_SF = 0x0000aa01,

        /// <summary>
        /// DSA public key signature algorithm.
        /// </summary>
        CALG_DSS_SIGN = 0x00002200,

        /// <summary>
        /// Elliptic curve Diffie-Hellman key exchange algorithm.
        /// </summary>
        CALG_ECDH = 0x0000aa05,

        /// <summary>
        /// Elliptic curve digital signature algorithm.
        /// </summary>
        CALG_ECDSA = 0x00002203,

        /// <summary>
        /// Elliptic curve Menezes, Qu, and Vanstone (MQV) key exchange algorithm. This algorithm is not supported.
        /// </summary>
        CALG_ECMQV = 0x0000a001,

        /// <summary>
        /// One way function hashing algorithm.
        /// </summary>
        CALG_HASH_REPLACE_OWF = 0x0000800b,

        /// <summary>
        /// Hughes MD5 hashing algorithm.
        /// </summary>
        CALG_HUGHES_MD5 = 0x0000a003,

        /// <summary>
        /// HMAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_HMAC = 0x00008009,

        /// <summary>
        /// KEA key exchange algorithm (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_KEA_KEYX = 0x0000aa04,

        /// <summary>
        /// MAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_MAC = 0x00008005,

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
        /// No signature algorithm.
        /// </summary>
        CALG_NO_SIGN = 0x00002000,

        /// <summary>
        /// The algorithm is only implemented in CNG. The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to 
        /// determine whether a cryptography algorithm is only supported by using the CNG functions.
        /// </summary>
        CALG_OID_INFO_CNG_ONLY = 0xffffffff,

        /// <summary>
        /// The algorithm is defined in the encoded parameters. The algorithm is only supported by using CNG.
        /// The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to determine whether a cryptography algorithm is
        /// only supported by using the CNG functions.
        /// </summary>
        CALG_OID_INFO_PARAMETERS = 0xfffffffe,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_PCT1_MASTER = 0x00004c04,

        /// <summary>
        /// RC2 block encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_RC2 = 0x00006602,

        /// <summary>
        /// RC4 stream encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_RC4 = 0x00006801,

        /// <summary>
        /// RC5 block encryption algorithm.
        /// </summary>
        CALG_RC5 = 0x0000660d,

        /// <summary>
        /// RSA public key exchange algorithm. This algorithm is supported by the Microsoft Base Cryptographic
        /// Provider.
        /// </summary>
        CALG_RSA_KEYX = 0x0000a400,

        /// <summary>
        /// RSA public key signature algorithm. This algorithm is supported by the Microsoft Base Cryptographic
        /// Provider.
        /// </summary>
        CALG_RSA_SIGN = 0x00002400,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_ENC_KEY = 0x00004c07,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_MAC_KEY = 0x00004c03,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SCHANNEL_MASTER_HASH = 0x00004c02,

        /// <summary>
        /// SEAL encryption algorithm. This algorithm is not supported.
        /// </summary>
        CALG_SEAL = 0x00006802,

        /// <summary>
        /// SHA hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_SHA = 0x00008004,

        /// <summary>
        /// Same as CALG_SHA. This algorithm is supported by the Microsoft Base Cryptographic Provider.
        /// </summary>
        CALG_SHA1 = 0x00008004,

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
        /// Skipjack block encryption algorithm (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_SKIPJACK = 0x0000660a,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL2_MASTER = 0x00004c05,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL3_MASTER = 0x00004c01,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_SSL3_SHAMD5 = 0x00008008,

        /// <summary>
        /// TEK (FORTEZZA). This algorithm is not supported.
        /// </summary>
        CALG_TEK = 0x0000660b,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_TLS1_MASTER = 0x00004c06,

        /// <summary>
        /// Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
        /// </summary>
        CALG_TLS1PRF = 0x0000800a,
    }

    internal enum HashParameters
    {
        HP_ALGID = 0x0001,   // Hash algorithm
        HP_HASHVAL = 0x0002, // Hash value
        HP_HASHSIZE = 0x0004 // Hash value size
    }

    [Flags()]
    internal enum CryptGenKeyTypes
    {
        CRYPT_EXPORTABLE = 0x00000001,
        CRYPT_USER_PROTECTED = 0x00000002,
        CRYPT_CREATE_SALT = 0x00000004,
        CRYPT_UPDATE_KEY = 0x00000008,
        CRYPT_NO_SALT = 0x00000010,
        CRYPT_PREGEN = 0x00000040,
        CRYPT_RECIPIENT = 0x00000010,
        CRYPT_INITIATOR = 0x00000040,
        CRYPT_ONLINE = 0x00000080,
        CRYPT_SF = 0x00000100,
        CRYPT_CREATE_IV = 0x00000200,
        CRYPT_KEK = 0x00000400,
        CRYPT_DATA_KEY = 0x00000800,
        CRYPT_VOLATILE = 0x00001000,
        CRYPT_SGCKEY = 0x00002000,
        CRYPT_ARCHIVABLE = 0x00004000,
        CRYPT_FORCE_KEY_PROTECTION_HIGH = 0x00008000,
    }

    internal enum CryptGetKeyParamParamType
    {
        KP_IV = 1,      // Initialization vector
        KP_SALT = 2,       // Salt value
        KP_PADDING = 3,       // Padding values
        KP_MODE = 4,       // Mode of the cipher
        KP_MODE_BITS = 5,       // Number of bits to feedback
        KP_PERMISSIONS = 6,       // Key permissions DWORD
        KP_ALGID = 7,       // Key algorithm
        KP_BLOCKLEN = 8,       // Block size of the cipher
        KP_KEYLEN = 9,       // Length of key in bits
        KP_SALT_EX = 10,      // Length of salt in bytes
        KP_P = 11,      // DSS/Diffie-Hellman P value
        KP_G = 12,      // DSS/Diffie-Hellman G value
        KP_Q = 13,     // DSS Q value
        KP_X = 14,      // Diffie-Hellman X value
        KP_Y = 15,      // Y value
        KP_RA = 16,      // Fortezza RA value
        KP_RB = 17,      // Fortezza RB value
        KP_INFO = 18,      // for putting information into an RSA envelope
        KP_EFFECTIVE_KEYLEN = 19,      // setting and getting RC2 effective key length
        KP_SCHANNEL_ALG = 20,      // for setting the Secure Channel algorithms
        KP_CLIENT_RANDOM = 21,      // for setting the Secure Channel client random data
        KP_SERVER_RANDOM = 22,      // for setting the Secure Channel server random data
        KP_RP = 23,
        KP_PRECOMP_MD5 = 24,
        KP_PRECOMP_SHA = 25,
        KP_CERTIFICATE = 26,      // for setting Secure Channel certificate data (PCT1)
        KP_CLEAR_KEY = 27,      // for setting Secure Channel clear key data (PCT1)
        KP_PUB_EX_LEN = 28,
        KP_PUB_EX_VAL = 29,
        KP_KEYVAL = 30,
        KP_ADMIN_PIN = 31,
        KP_KEYEXCHANGE_PIN = 32,
        KP_SIGNATURE_PIN = 33,
        KP_PREHASH = 34,
        KP_ROUNDS = 35,
        KP_OAEP_PARAMS = 36,      // for setting OAEP params on RSA keys
        KP_CMS_KEY_INFO = 37,
        KP_CMS_DH_KEY_INFO = 38,
        KP_PUB_PARAMS = 39,      // for setting public parameters
        KP_VERIFY_PARAMS = 40,      // for verifying DSA and DH parameters
        KP_HIGHEST_VERSION = 41,      // for TLS protocol version setting
        KP_GET_USE_COUNT = 42,      // for use with PP_CRYPT_COUNT_KEY_USE contexts
    }

    internal enum CryptExportKeyBlobType
    {
        SIMPLEBLOB = 0x1,
        PUBLICKEYBLOB = 0x6,
        PRIVATEKEYBLOB = 0x7,
        PLAINTEXTKEYBLOB = 0x8,
        OPAQUEKEYBLOB = 0x9,
        PUBLICKEYBLOBEX = 0xA,
        SYMMETRICWRAPKEYBLOB = 0xB,
        KEYSTATEBLOB = 0xC,
    }

    internal enum CryptExportKeyTypes
    {
        CRYPT_Y_ONLY = 0x00000001,
        CRYPT_SSL2_FALLBACK = 0x00000002,
        CRYPT_DESTROYKEY = 0x00000004,
        CRYPT_OAEP = 0x00000040,  // used with RSA encryptions/decryptions
        CRYPT_BLOB_VER3 = 0x00000080,  // export version 3 of a blob type
        CRYPT_IPSEC_HMAC_KEY = 0x00000100,  // CryptImportKey only
    }


    /// <summary>
    /// The flags defined by CryptSignHash function of CryptAPI.
    /// </summary>
    [Flags]
    public enum CryptSignHashFlag
    {
        /// <summary>
        /// No value to set.
        /// </summary>
        None = 0x00,

        /// <summary>
        /// Used with RSA providers. The hash object identifier (OID) is not placed in the RSA public key encryption. 
        /// If this flag is not set, the hash OID in the default signature is as specified in the definition of 
        /// DigestInfo in PKCS #1.
        /// </summary>
        NoHashOid = 0x01,

        /// <summary>
        /// This flag is not used.
        /// </summary>
        Type2Format = 0x02,

        /// <summary>
        /// Use the RSA signature padding method specified in the ANSI X9.31 standard. 
        /// Windows 2000:  CRYPT_X931_FORMAT is not supported.
        /// </summary>
        X931Format = 0x04,
    }

    /// <summary>
    /// Hash type of RSA signature.
    /// </summary>
    public enum CryptRsaSignHashType
    {
        /// <summary>
        /// MD5 hash algorithm
        /// </summary>
        MD5,

        /// <summary>
        /// MD5 hash algorithm
        /// </summary>
        SHA1,
    }

    internal static class NativeMethods
    {
        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptAcquireContext(ref IntPtr hProv, string pszContainer,
        string pszProvider, ProviderType dwProvType, CryptAcquireContextTypes dwFlags);


        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptCreateHash(IntPtr hProv, AlgId algId, IntPtr hKey,
            uint dwFlags, ref IntPtr phHash);


        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptHashData(IntPtr hHash, byte[] pbData, uint dataLen, uint flags);


        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptDestroyHash(IntPtr hHash);


        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptGetHashParam(IntPtr hHash,
                                             HashParameters dwParam,
                                             [Out] byte[] pbData,
                                             ref uint pdwDataLen,
                                             uint dwFlags);


        [DllImport("advapi32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptReleaseContext(IntPtr hProv, uint dwFlags);


        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptGenKey(IntPtr hProv, AlgId Algid, int dwFlags, ref IntPtr phKey);


        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptDestroyKey(IntPtr phKey);


        [DllImport(@"advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptExportKey(IntPtr hKey, IntPtr hExpKey,
            CryptExportKeyBlobType dwBlobType,
            CryptExportKeyTypes dwFlags, [In, Out] byte[] pbData, ref uint dwDataLen);


        [DllImport(@"advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptImportKey(IntPtr hProv, byte[] pbKeyData, Int32 dwDataLen,
            IntPtr hPubKey, UInt32 dwFlags, ref IntPtr hKey);


        [DllImport(@"advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool CryptEncrypt(IntPtr hKey, IntPtr hHash, int Final,
            uint dwFlags, byte[] pbData, ref int pdwDataLen, int dwBufLen);


        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool CryptDuplicateKey(IntPtr hKey, IntPtr pdwReserved,
                uint dwFlags, ref IntPtr phKey);


        internal const string BCRYPT_OBJECT_LENGTH = "ObjectLength";
        internal const string BCRYPT_ALGORITHM_NAME = "AlgorithmName";
        internal const string BCRYPT_PROVIDER_HANDLE = "ProviderHandle";
        internal const string BCRYPT_CHAINING_MODE = "ChainingMode";
        internal const string BCRYPT_BLOCK_LENGTH = "BlockLength";
        internal const string BCRYPT_KEY_LENGTH = "KeyLength";
        internal const string BCRYPT_KEY_OBJECT_LENGTH = "KeyObjectLength";
        internal const string BCRYPT_KEY_STRENGTH = "KeyStrength";
        internal const string BCRYPT_KEY_LENGTHS = "KeyLengths";
        internal const string BCRYPT_BLOCK_SIZE_LIST = "BlockSizeList";
        internal const string BCRYPT_EFFECTIVE_KEY_LENGTH = "EffectiveKeyLength";
        internal const string BCRYPT_HASH_LENGTH = "HashDigestLength";
        internal const string BCRYPT_HASH_OID_LIST = "HashOIDList";
        internal const string BCRYPT_PADDING_SCHEMES = "PaddingSchemes";
        internal const string BCRYPT_SIGNATURE_LENGTH = "SignatureLength";
        internal const string BCRYPT_HASH_BLOCK_LENGTH = "HashBlockLength";
        internal const string BCRYPT_INITIALIZATION_VECTOR = "IV";


        internal const string BCRYPT_CHAIN_MODE_NA = "ChainingModeN/A";
        internal const string BCRYPT_CHAIN_MODE_CBC = "ChainingModeCBC";
        internal const string BCRYPT_CHAIN_MODE_ECB = "ChainingModeECB";
        internal const string BCRYPT_CHAIN_MODE_CFB = "ChainingModeCFB";
        internal const string BCRYPT_CHAIN_MODE_CCM = "ChainingModeCCM";
        internal const string BCRYPT_CHAIN_MODE_GCM = "ChainingModeGCM";

        internal const uint AT_KEYEXCHANGE = 1;
        internal const uint AT_SIGNATURE = 2;

        [DllImport("bcrypt.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        internal static extern uint BCryptOpenAlgorithmProvider(
            out IntPtr phAlgorithm,
            string pszAlgId,
            string pszImplementation,
            uint dwFlags);


        [DllImport("bcrypt.dll")]
        internal static extern uint BCryptCloseAlgorithmProvider(
            IntPtr hAlgorithm,
            uint flags);


        [DllImport("bcrypt.dll")]
        internal static extern uint BCryptGenerateSymmetricKey(
            IntPtr hAlgorithm,
            [Out]
            out IntPtr phKey,
            [Out, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbKeyObject,
            uint cbKeyObject,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbSecret,
            uint cbSecret,
            uint dwFlags);


        [DllImport("bcrypt.dll")]
        internal static extern uint BCryptDestroyKey(IntPtr hKey);


        [DllImport("bcrypt.dll", CharSet = CharSet.Unicode)]
        internal static extern uint BCryptGetProperty(
            IntPtr hObject,
            string pszProperty,
            [Out, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbOutput,
            uint cbOutput,
            [Out]
            out uint pcbResult,
            uint flags);


        [DllImport("bcrypt.dll", CharSet = CharSet.Unicode)]
        internal static extern uint BCryptSetProperty(
            IntPtr hObject,
            string pszProperty,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbInput,
            uint cbInput,
            uint dwFlags);


        [DllImport("bcrypt.dll")]
        internal static extern uint BCryptEncrypt(
            IntPtr hKey,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbInput,
            uint cbInput,
            IntPtr pPaddingInfo,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbIV,
            uint cbIV,
            [Out, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbOutput,
            uint cbOutput,
            [Out]
            out uint pcbResult,
            uint dwFlags);


        [DllImport("bcrypt.dll")]
        internal static extern uint BCryptDecrypt(
            IntPtr hKey,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbInput,
            uint cbInput,
            IntPtr pPaddingInfo,
            [In, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbIV,
            uint cbIV,
            [Out, MarshalAs(UnmanagedType.LPArray)]
            byte[] pbOutput,
            uint cbOutput,
            [Out]
            out uint pcbResult,
            uint dwFlags);


        internal const uint BCRYPT_CIPHER_OPERATION = 0x00000001;
        internal const uint BCRYPT_HASH_OPERATION = 0x00000002;
        internal const uint BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION = 0x00000004;
        internal const uint BCRYPT_SECRET_AGREEMENT_OPERATION = 0x00000008;
        internal const uint BCRYPT_SIGNATURE_OPERATION = 0x00000010;
        internal const uint BCRYPT_RNG_OPERATION = 0x00000020;


#pragma warning disable 649
        internal struct BCRYPT_ALGORITHM_IDENTIFIER
        {
            [MarshalAs(UnmanagedType.LPWStr)]
            public string pszName;
            public uint dwClass;
            public uint dwFlags;
        }
#pragma warning restore 649


        [DllImport("bcrypt.dll", CharSet = CharSet.Unicode)]
        internal static extern uint BCryptEnumAlgorithms(
            uint dwAlgOperations,
            [Out]
            out uint pAlgCount,
            [Out]
            out IntPtr ppAlgList,
            uint dwFlags);


        [DllImport("bcrypt.dll", CharSet = CharSet.Unicode)]
        internal static extern uint BCryptFreeBuffer(IntPtr pvBuffer);


        [DllImport("Crypt32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptAcquireCertificatePrivateKey(
            IntPtr pCertContext,
            uint dwFlags,
            IntPtr pvReserved,
            out IntPtr phCryptProvOrNCryptKey,
            out uint pdwkeySpec,
            [MarshalAs(UnmanagedType.Bool)]
            out bool pfCallerFreeProvOrNCryptKey);


        [DllImport("Advapi32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptSetHashParam(
            IntPtr hHash,
            HashParameters param,
            IntPtr pbData,
            uint flags);


        [DllImport("Advapi32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool CryptSignHash(
            IntPtr hHash,
            uint keySpec,
            IntPtr sDescription,
            CryptSignHashFlag flags,
            IntPtr pbSignature,
            ref uint pdwSignLen);
    }
}