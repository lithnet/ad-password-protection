#include "stdafx.h"
#include "hasher.h"
#include <wincrypt.h>
#include "malloc.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bcrypt.h>
#include <locale>
#include "utils.h"
#include "SecureArrayT.h"

SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<WCHAR> &input)
{
	int lenW = WideCharToMultiByte(CP_UTF8, 0, input, -1, NULL, 0, NULL, NULL);

	if (lenW > 0)
	{
		SecureArrayT<char> output(lenW);

		if (WideCharToMultiByte(CP_UTF8, 0, input, -1, output.get(), lenW, NULL, NULL) > 0)
		{
			SecureArrayT<BYTE> result = GetSha1HashBytes(output);

			return result;
		}
	}

	throw std::system_error(GetLastError(), std::system_category(), "Sha1Hash/WideCharToMultiByte failed");
}

SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<char> &input)
{
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	try
	{
		DWORD dwHashLen;
		DWORD dwCount = sizeof(DWORD);

		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
		{
			DWORD result = GetLastError();
			if (result == NTE_BAD_KEYSET)
			{
				// No default container was found. Attempt to create it.
				if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				{
					throw std::system_error(GetLastError(), std::system_category(), "CryptAcquireContext create container failed");
				}
			}
			else
			{
				throw std::system_error(GetLastError(), std::system_category(), "CryptAcquireContext failed");
			}
		}

		if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptCreateHash failed");
		}

		if (!CryptHashData(hHash, (BYTE*)input.get(), static_cast<DWORD>(strlen(input.get())), 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptHashData failed");
		}

		if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen, &dwCount, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptGetHashParam failed");
		}

		if (dwHashLen != 20)
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptGetHashParam returned an invalid hash length");
		}

		SecureArrayT<BYTE> hashBytes(20);

		if (!CryptGetHashParam(hHash, HP_HASHVAL, hashBytes.get(), &dwHashLen, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptGetHashParam failed");
		}

		if (hHash)
		{
			CryptDestroyHash(hHash);
		}

		if (hProv)
		{
			CryptReleaseContext(hProv, 0);
		}

		return hashBytes;
	}
	catch (...)
	{
		if (hHash)
		{
			CryptDestroyHash(hHash);
		}

		if (hProv)
		{
			CryptReleaseContext(hProv, 0);
		}

		throw;
	}
}
