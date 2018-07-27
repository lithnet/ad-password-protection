#include "stdafx.h"
#include "hasher.h"
#include <fstream>
#include "utils.h"
#include "registry.h"
#include "hashevaluator.h"
#include "shlwapi.h"

BOOL checkHash(std::wstring hash)
{
	//DebugBreak();
	std::wstring range = hash.substr(0, 5);
	odprintf(L"Range: %s", range.c_str());

	return checkHash(hash, range);
}

BOOL checkHash(std::wstring hash, std::wstring range)
{
	std::wstring path = GetStoreFile(range);

	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	return hasHash(path, hash);
}

std::wstring GetStoreFile(std::wstring range)
{
	std::wstring path = GetRegValueString(L"StorePath", L"");

	if (path == L"")
	{
		throw new std::invalid_argument("No store path was specified");
	}

	if (!path.empty() && *path.rbegin() != '\\')
		path += '\\';

	path = path.append(range);
	path = path.append(L".txt");

	odprintf(L"Store file: %s", path.c_str());
	
	return path;
}

BOOL hasHash(std::wstring filename, std::wstring hash)
{
	std::wifstream hFile(filename.c_str());
	std::wstring line;
	while (std::getline(hFile, line))
	{
		if (line.compare(hash) == 0)
		{
			std::wstring message = L"Matched: ";
			message.append(line);
			OutputDebugString(message.c_str());
			return TRUE;
		}
	}

	return FALSE;
}
