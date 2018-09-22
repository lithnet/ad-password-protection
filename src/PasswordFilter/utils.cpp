#include "utils.h"
#include "stdafx.h"
#include "filter.h"
#include "registry.h"
#include <iostream>
#include "eventlog.h"
#include "messages.h"
#include "esestore.h"
#include <vector>
#include "SecureArrayT.h"

std::vector<std::wstring> SplitString(const std::wstring &text, const wchar_t sep)
{
	std::vector<std::wstring> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::wstring::npos) {
		if (end != start) {
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(text.substr(start));
	}

	return tokens;
}

SecureArrayT<WCHAR> StringToWcharArray(const LPCWSTR str)
{
	int charCount = wcslen(str);
	int len = charCount + 1;

	SecureArrayT<WCHAR> ar(len);

	wcsncpy_s(ar.get(), len, str, charCount);

	return ar;
}

SecureArrayT<WCHAR> UnicodeStringToWcharArray(const UNICODE_STRING& str)
{
	int charCount = str.Length / sizeof(wchar_t);
	int len = charCount + 1;

	SecureArrayT<WCHAR> ar (len);

	wcsncpy_s(ar.get(), len, str.Buffer, charCount);

	return ar;
}

bool DirectoryExists(const std::wstring& dirName_in)
{
	DWORD attributes = GetFileAttributes(dirName_in.c_str());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}

	if (attributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;  
	}

	return false;
}