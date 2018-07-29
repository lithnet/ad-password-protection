#include "stdafx.h"
#include "hasher.h"
#include <fstream>
#include "registry.h"
#include "hashevaluator.h"
#include "shlwapi.h"

bool IsHashInStore(std::wstring hash)
{
	std::wstring range = hash.substr(0, 5);
	return IsHashInStore(hash, range);
}

bool IsHashInStore(std::wstring hash, std::wstring range)
{
	std::wstring path = GetStoreFileName(range);

	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	return IsHashInFile(path, hash);
}

std::wstring GetStoreFileName(std::wstring range)
{
	std::wstring path = GetRegValue(L"SHA1Store", L"");

	if (path == L"")
	{
		throw new std::invalid_argument("No store path was specified");
	}

	if (!path.empty() && *path.rbegin() != '\\')
		path += '\\';

	path += range;
	path += L".txt";

	return path;
}

bool IsHashInFile(std::wstring filename, std::wstring hash)
{
	std::wifstream file(filename.c_str());
	std::wstring line;

	while (std::getline(file, line))
	{
		if (line.compare(hash) == 0)
		{
			return true;
		}
	}

	return false;
}
