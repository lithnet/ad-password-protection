#include "stdafx.h"
#include "binarystore.h"
#include <fstream>
#include "Shlwapi.h"
#include <sstream>
#include <iomanip>
#include "utils.h"
#include "SecureArrayT.h"
#include "eventlog.h"
#include "messages.h"

binarystore::binarystore(const std::wstring& storeBasePath, const std::wstring& storeSubPathPasswordStore, const std::wstring& storeSubPathWordStore, int hashSize, int hashOffset)
{
	this->hashSize = hashSize;
	this->storeSubPathPasswordStore = storeSubPathPasswordStore;
	this->storeSubPathWordStore = storeSubPathWordStore;
	this->storeBasePath = storeBasePath;
	this->hashOffset = hashOffset;

	WCHAR passwordStorePath[MAX_PATH] = L"";
	WCHAR wordStorePath[MAX_PATH] = L"";

	if (storeBasePath.empty())
	{
		throw std::exception("Store path was null");
	}

	if (!DirectoryExists(storeBasePath))
	{
		if (!CreateDirectory(storeBasePath.c_str(), NULL))
		{
			const DWORD error = GetLastError();
			eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_STOREERROR, 2, std::to_wstring(error).c_str(), storeBasePath.c_str());
			throw std::system_error(error, std::system_category(), "Failed to create the store folder");
		}
	}

	PathCombine(passwordStorePath, storeBasePath.c_str(), storeSubPathPasswordStore.c_str());
	PathCombine(wordStorePath, storeBasePath.c_str(), storeSubPathWordStore.c_str());

	this->storePathPasswordStore = passwordStorePath;
	this->storePathWordStore = wordStorePath;
}

binarystore::~binarystore()
= default;

bool binarystore::IsPasswordInPasswordStore(const SecureArrayT<WCHAR> &password)
{
	SecureArrayT<BYTE> hash = GetHashFromPassword(password);

	return IsHashInPasswordStore(hash);
}

bool binarystore::IsPasswordInWordStore(const SecureArrayT<WCHAR> &password)
{
	SecureArrayT<BYTE> hash = GetHashFromPassword(password);

	return IsHashInWordStore(hash);
}

bool binarystore::IsHashInPasswordStore(const SecureArrayT<BYTE> &hash)
{
	std::wstring path = GetPasswordStoreFileName(GetRangeFromHash(hash));

	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}

	return IsHashInBinaryFile(path, hash);
}

bool binarystore::IsHashInWordStore(const SecureArrayT<BYTE> &hash)
{
	std::wstring path = GetWordStoreFileName(GetRangeFromHash(hash));

	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}

	return IsHashInBinaryFile(path, hash);
}

std::wstring binarystore::GetPasswordStoreFileName(const std::wstring &range) const
{
	std::wstring path = this->storePathPasswordStore;
	path += range;
	path += L".db";
	return path;
}

std::wstring binarystore::GetWordStoreFileName(const std::wstring &range) const
{
	std::wstring path = this->storePathWordStore;
	path += range;
	path += L".db";
	return path;
}

bool binarystore::IsHashInBinaryFile(const std::wstring &filename, const SecureArrayT<BYTE> &hashBytes) const
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate | std::ios::in);

	long firstRow = 0, currentRow = 0;
	std::wstring message = L"Searching ";
	message += filename;
	OutputDebugString(message.c_str());

	long length = static_cast<long>(file.tellg());

	if (length % this->hashSize != 0)
	{
		throw std::invalid_argument("The hash store is corrupted");
	}

	file.seekg(0, std::ios::beg);

	const BYTE* toFind = hashBytes + this->hashOffset;

	long lastRow = length / this->hashSize;

	SecureArrayT<BYTE> rowData(this->hashSize);

	while (firstRow <= lastRow)
	{
		currentRow = (firstRow + lastRow) / 2;
		file.seekg((currentRow * this->hashSize), std::ios::beg);

		file.read((char*)rowData.get(), this->hashSize);

		int result = memcmp(rowData.get(), toFind, this->hashSize);

		if (result < 0)
		{
			firstRow = currentRow + 1;
		}
		else if (result > 0)
		{
			lastRow = currentRow - 1;
		}
		else
		{
			message = L"Hash found at row ";
			message += std::to_wstring(currentRow);
			OutputDebugString(message.c_str());
			file.close();
			return true;
		}
	}

	OutputDebugString(L"Hash not found");
	file.close();
	return false;
}
