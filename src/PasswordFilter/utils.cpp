#include "utils.h"
#include "stdafx.h"
#include "filter.h"
#include "registry.h"
#include <iostream>
#include "eventlog.h"
#include "messages.h"
#include "esestore.h"
#include <vector>

std::vector<std::wstring> SplitString(const std::wstring &text, wchar_t sep) {
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

wchar_t* UnicodeStringToWcharArray(UNICODE_STRING str)
{
	int len = str.Length + 1;

	wchar_t* ar = new wchar_t[len];

	wcsncpy_s(ar, len, str.Buffer, str.Length);

	return ar;
}

void __cdecl odprintf(const wchar_t *format, ...)
{
	wchar_t buf[4096], *p = buf;
	va_list args;
	int n;
	int size = 4096 - 3;

	va_start(args, format);
	n = _vsnwprintf_s(p, size, size - 1, format, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf && iswspace(p[-1]))
		*--p = L'\0';

	*p++ = L'\r';
	*p++ = L'\n';
	*p = L'\0';

	OutputDebugString(buf);
}