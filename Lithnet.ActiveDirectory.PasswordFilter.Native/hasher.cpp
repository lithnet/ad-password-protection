#include "stdafx.h"
#include "hasher.h"
#include <wincrypt.h>
#include "malloc.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bcrypt.h>
#include <locale>

template<typename TInputIter>
std::wstring ToHexString(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false)
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

std::wstring Sha1Hash(const std::wstring input)
{
	std::string result;
	int lenW = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.length(), NULL, 0, NULL, NULL);
	if (lenW > 0)
	{
		char* output = new char[lenW];
		WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.length(), output, lenW, NULL, NULL);

		return Sha1Hash(std::string(output, lenW));
	}

	throw std::system_error(GetLastError(), std::system_category(), "WideCharToMultiByte failed");
}

std::wstring Sha1Hash(const std::string input)
{
	BYTE *pbHash = NULL;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	try
	{
		DWORD dwHashLen;
		DWORD dwCount = sizeof(DWORD);

		std::wstring hashedStringResult;

		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptAcquireContext failed");
		}

		if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptCreateHash failed");
		}
		
		if (!CryptHashData(hHash, (BYTE*)(input.c_str()), static_cast<DWORD>(input.size()), 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptHashData failed");
		}

		if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen, &dwCount, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptGetHashParam failed");
		}

		pbHash = new BYTE[dwHashLen];

		if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "CryptGetHashParam failed");
		}

		hashedStringResult = ToHexString(pbHash, pbHash + dwHashLen);

		if (pbHash)
		{
			delete[] pbHash;
		}

		if (hHash)
		{
			CryptDestroyHash(hHash);
		}

		if (hProv)
		{
			CryptReleaseContext(hProv, 0);
		}

		return hashedStringResult;
	}
	catch (...)
	{
		if (pbHash)
		{
			delete[] pbHash;
		}

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
