using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace StoreInterface
{
    internal class NtlmHash
    {
        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: System.Runtime.InteropServices.MarshalAs(UnmanagedType.Bool)]
        private static extern bool CryptAcquireContext(ref IntPtr hProv, string pszContainer, string pszProvider, uint dwProvType, uint dwFlags);

        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool CryptCreateHash(IntPtr hProv, uint algId, IntPtr hKey, uint dwFlags, ref IntPtr phHash);


        [DllImport("advapi32.dll", SetLastError = true)]
        private static extern bool CryptHashData(IntPtr hHash, byte[] pbData, uint dataLen, uint flags);

        [DllImport("advapi32.dll", SetLastError = true)]
        private static extern long CryptGetHashParam(IntPtr hHash, uint dwParam, byte[] pbData, uint dwDataLen, uint dwFlags);

        [DllImport("advapi32.dll", SetLastError = true)]
        private static extern bool CryptDestroyHash(IntPtr hHash);

        [DllImport("Advapi32.dll", EntryPoint = "CryptReleaseContext", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern bool CryptReleaseContext(IntPtr hProv, Int32 dwFlags);

        private static uint PROV_RSA_FULL = 1;
        private static uint CRYPT_NEWKEYSET = 0x00000008;
        private static int NTE_BAD_KEYSET = -2146893802;
        private static uint CALG_MD4 = 32770;
        private static uint HP_HASHSIZE = 0x0004;
        private static uint HP_HASHVAL = 0x0002;


        private byte[] Hash(byte[] input)
        {
            IntPtr hProv = IntPtr.Zero;
            IntPtr hHash = IntPtr.Zero;

            try
            {
                uint dwCount = sizeof(uint);

                if (!CryptAcquireContext(ref hProv, null, null, PROV_RSA_FULL, 0))
                {
                    int result = Marshal.GetLastWin32Error();

                    if (result == NTE_BAD_KEYSET)
                    {
                        // No default container was found. Attempt to create it.
                        if (!CryptAcquireContext(ref hProv, null, null, PROV_RSA_FULL, CRYPT_NEWKEYSET))
                        {
                            throw new Win32Exception(result);
                        }
                    }
                    else
                    {
                        throw new Win32Exception(result);
                    }
                }

                if (!CryptCreateHash(hProv, CALG_MD4, IntPtr.Zero, 0, ref hHash))
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error());
                }

                if (!CryptHashData(hHash, input, (uint)input.Length, 0))
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error());
                }

                uint hashLen = 0;

                //if (!CryptGetHashParam(hHash, HP_HASHSIZE,  hashLen, dwCount, 0))
                //{
                //    throw new Win32Exception(Marshal.GetLastWin32Error());
                //}

                if (hashLen != 16)
                {
                    throw new Exception("CryptGetHashParam returned an invalid hash length");
                }

                byte[] hashBytes = new byte[16];

                //if (!CryptGetHashParam(hHash, HP_HASHVAL,  hashBytes, (uint)hashBytes.Length, 0))
                //{
                //    throw new Win32Exception(Marshal.GetLastWin32Error());
                //}

                return hashBytes;
            }
            finally
            {
                if (hHash != IntPtr.Zero)
                {
                    CryptDestroyHash(hHash);
                }

                if (hProv != IntPtr.Zero)
                {
                    CryptReleaseContext(hProv, 0);
                }
            }
        }
    }
}