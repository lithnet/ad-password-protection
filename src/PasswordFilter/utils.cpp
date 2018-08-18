#include "utils.h"
#include "stdafx.h"
#include "filter.h"
#include "registry.h"
#include <iostream>
#include "eventlog.h"
#include "messages.h"
#include "esestore.h"
#include <vector>

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

LPWSTR UnicodeStringToWcharArray(const UNICODE_STRING& str)
{
	int charCount = str.Length / sizeof(wchar_t);
	int len = charCount + 1;

	wchar_t* ar = new wchar_t[len];

	wcsncpy_s(ar, len, str.Buffer, charCount);

	return ar;
}

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