#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "utils.h"
#include "stringnormalization.h"
#include "registry.h"
#include <cwctype>
#include <iterator>
#include "shlwapi.h"

LPWSTR ToLowerInvariant(const LPWSTR &s)
{
	int sizeRequired = 0;

	sizeRequired = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, s, -1, 0, 0);

	if (sizeRequired == 0)
	{
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}

	wchar_t* buf = new wchar_t[sizeRequired];

	int sizeAllocated = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, s, -1, buf, sizeRequired);

	if (sizeAllocated == 0)
	{
		SecureZeroMemory(buf, sizeRequired);
		delete[] buf;
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}

	return buf;
}

LPWSTR NormalizePassword(const LPWSTR &password)
{
	if (wcslen(password) == 0)
	{
		return (LPWSTR)password;
	}

	LPWSTR newPassword = ToLowerInvariant(password);

	RemoveWhiteSpace(newPassword);
	StrTrim(newPassword, CHARS_TO_TRIM);
	ReplaceChars(newPassword, CHARS_TO_REPLACE);
	RemoveChars(newPassword, CHARS_TO_DELETE);

	return newPassword;
}

void RemoveWhiteSpace(LPWSTR &s)
{
	LPWSTR cpy = s;
	LPWSTR temp = s;

	while (*cpy)
	{
		if (!std::iswspace(*cpy))
		{
			*temp++ = *cpy;
		}

		cpy++;
	}

	*temp = 0;
}

void RemoveChars(LPWSTR &s, const wchar_t *charsToRemove)
{
	LPWSTR cpy = s;
	LPWSTR temp = s;

	for (size_t i = 0; i < wcslen(s); i++)
	{
		bool matched = false;

		for (size_t i = 0; i < wcslen(charsToRemove); i++)
		{
			if (charsToRemove[i] == *cpy)
			{
				matched = true;
				break;
			}
		}

		if (!matched)
		{
			*temp++ = *cpy;
		}

		cpy++;
	}

	*temp = 0;
}

void ReplaceChars(LPWSTR &s, const wchar_t *charPairsToReplace)
{
	LPWSTR cpy = s;
	LPWSTR temp = s;

	for (size_t i = 0; i < wcslen(s); i++)
	{
		bool matched = false;

		for (size_t j = 0; j < wcslen(charPairsToReplace); j = j + 2)
		{
			if (charPairsToReplace[j] == *cpy)
			{
				*temp++ = charPairsToReplace[j + 1];
				matched = true;
				break;
			}
		}

		if (!matched)
		{
			*temp++ = *cpy;
		}

		cpy++;
	}
}