#include "stdafx.h"
#include "hasher.h"
#include <wincrypt.h>
#include "malloc.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#define BUFFER_SIZE 256

template<typename TInputIter>
std::wstring to_hex_string(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false)
{
	std::wstringstream ss;
	ss << std::hex << std::setfill(L'0');
	if (use_uppercase)
		ss << std::uppercase;
	while (first != last)
	{
		ss << std::setw(2) << static_cast<int>(*first++);
		if (insert_spaces && first != last)
			ss << " ";
	}
	return ss.str();
}

void make_hash(const WCHAR * data, std::wstring * hash)
{
	OutputDebugString(L"Start hasher");

	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE *pbHash = NULL;
	DWORD dwHashLen;

	BYTE * pbBuffer = NULL;
	DWORD dwCount;
	DWORD i;
	size_t bufLen = 0;

	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
		return;
	}
	if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
		return;
	}

	bufLen = wcslen(data);

	pbBuffer = (BYTE*)malloc(bufLen + 1);
	memset(pbBuffer, 0, bufLen + 1);

	// Fill the buffer with data.
	for (i = 0; i < bufLen; i++) {
		pbBuffer[i] = (BYTE)data[i];
	}

	if (!CryptHashData(hHash, pbBuffer, (DWORD)bufLen, 0)) {
		return;
	}

	dwCount = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLen, &dwCount, 0)) {
		return;
	}
	if ((pbHash = (unsigned char*)malloc(dwHashLen)) == NULL) {
		return;
	}

	// Set memory block to zero
	memset(pbHash, 0, dwHashLen);

	if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0)) {
		return;
	}
	
	std::wstring hasheds = to_hex_string(pbHash, pbHash + dwHashLen);
	OutputDebugString(hasheds.c_str());

	*hash = hasheds;

	if (hHash) CryptDestroyHash(hHash);
	if (hProv) CryptReleaseContext(hProv, 0);
}
