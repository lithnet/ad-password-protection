#include "stdafx.h"
#include "hasher.h"
#include <wincrypt.h>
#include "malloc.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bcrypt.h>
#include <locale>

template<typename T>
std::wstring ToHexString(T first, T last, bool use_uppercase = true, bool insert_spaces = false)
{
	std::wstringstream ss;
	ss << std::hex << std::setfill(L'0');

	if (use_uppercase)
	{
		ss << std::uppercase;
	}

	while (first != last)
	{
		ss << std::setw(2) << static_cast<int>(*first++);

		if (insert_spaces && first != last)
		{
			ss << " ";
		}
	}

	return ss.str();
}

//
//std::wstring GetSha1HashString(const std::wstring input)
//{
//	std::string result;
//	int lenW = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.length(), NULL, 0, NULL, NULL);
//
//	if (lenW > 0)
//	{
//		char* output = new char[lenW];
//
//		if (WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.length(), output, lenW, NULL, NULL) > 0)
//		{
//			return GetSha1HashString(std::string(output, lenW));
//		}
//	}
//
//	throw std::system_error(GetLastError(), std::system_category(), "Sha1Hash/WideCharToMultiByte failed");
//}
//
//std::wstring GetSha1HashString(const std::string input)
//{
//	BYTE *pbHash = NULL;
//
//	try 
//	{
//		pbHash = new BYTE[20];
//
//		GetSha1HashBytes(input, pbHash, 20);
//
//		std::wstring hashedStringResult = ToHexString(pbHash, pbHash + 20);
//
//		if (hashedStringResult.length() != 40)
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "hashedStringResult returned an invalid hash length");
//		}
//
//		if (pbHash)
//		{
//			delete[] pbHash;
//		}
//
//		return hashedStringResult;
//	}
//	catch (...)
//	{
//		if (pbHash)
//		{
//			delete[] pbHash;
//		}
//
//		throw;
//	}
//}

void GetSha1HashBytes(const LPWSTR &input, BYTE* hashBytes, const int &hashBytesLength)
{
	int lenW = WideCharToMultiByte(CP_UTF8, 0, input, -1, NULL, 0, NULL, NULL);

	if (lenW > 0)
	{
		char* output = new char[lenW];

		if (WideCharToMultiByte(CP_UTF8, 0, input, -1, output, lenW, NULL, NULL) > 0)
		{
			GetSha1HashBytes(output, hashBytes, hashBytesLength);
			if (output)
			{
				SecureZeroMemory(output, lenW);
				delete[] output;
			}

			return;
		}

		if (output)
		{
			SecureZeroMemory(output, lenW);
			delete[] output;
		}
	}

	throw std::system_error(GetLastError(), std::system_category(), "Sha1Hash/WideCharToMultiByte failed");
}

void GetSha1HashBytes(const LPSTR &input, BYTE* hashBytes, const int &hashBytesLength)
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

		if (!CryptHashData(hHash, (BYTE*)input, static_cast<DWORD>(strlen(input)), 0))
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

		if (hashBytesLength != dwHashLen)
		{
			throw std::invalid_argument("An insufficient buffer was allocated for the hash");
		}

		if (!CryptGetHashParam(hHash, HP_HASHVAL, hashBytes, &dwHashLen, 0))
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
