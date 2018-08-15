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