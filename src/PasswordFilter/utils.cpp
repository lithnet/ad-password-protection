#include "stdafx.h"
#include "utils.h"
#include <vector>
#include "SecureArrayT.h"
#include <atlalloc.h>

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
	const int charCount = (int)wcslen(str);
	const int len = charCount + 1;

	SecureArrayT<WCHAR> ar(len);

	wcsncpy_s(ar.get(), len, str, charCount);

	return ar;
}

SecureArrayT<WCHAR> UnicodeStringToWcharArray(const UNICODE_STRING& str)
{
	const int charCount = str.Length / sizeof(wchar_t);
	const int len = charCount + 1;

	SecureArrayT<WCHAR> ar (len);

	wcsncpy_s(ar.get(), len, str.Buffer, charCount);

	return ar;
}

bool DirectoryExists(const std::wstring& dirName)
{
	const DWORD attributes = GetFileAttributes(dirName.c_str());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}

	return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}