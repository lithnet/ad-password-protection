#include "stdafx.h"
#include "binarystore.h"
#include <fstream>
#include "registry.h"
#include "shlwapi.h"
#include <sstream>
#include "hasher.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "utils.h"
#include <atlconv.h>
#include "SecureArrayT.h"

binarystore::binarystore(std::wstring storeBasePath, std::wstring storeSubPath, int hashSize, int hashOffset)
{
	this->hashSize = hashSize;
	this->storeSubPath = storeSubPath;
	this->storeBasePath = storeBasePath;
	this->hashOffset = hashOffset;

	WCHAR path[MAX_PATH] = L"";

	PathCombine(path, storeBasePath.c_str(), storeSubPath.c_str());

	if (!DirectoryExists(storeBasePath))
	{
		USES_CONVERSION;

		std::stringstream ss;
		ss << "There was no store found at ";
		ss << W2A(path);
		throw std::system_error(ERROR_PATH_NOT_FOUND, std::system_category(), ss.str().c_str());
	}

	if (!DirectoryExists(path))
	{
		CreateDirectory(path, NULL);
	}

	this->storePath = path;
}

binarystore::~binarystore()
{
}

bool binarystore::IsPasswordInStore(const SecureArrayT<WCHAR> &password)
{
	SecureArrayT<BYTE> hash = GetHashFromPassword(password);

	return IsHashInStore(hash);
}

bool binarystore::IsHashInStore(const SecureArrayT<BYTE> &hash)
{
	std::wstring path = GetStoreFileName(GetRangeFromHash(hash));

	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}

	return IsHashInBinaryFile(path, hash);
}

std::wstring binarystore::GetStoreFileName(const std::wstring &range)
{
	std::wstring path = this->storePath;
	path += range;
	path += L".db";
	return path;
}

bool binarystore::IsHashInBinaryFile(const std::wstring &filename, const SecureArrayT<BYTE> &hashBytes)
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
