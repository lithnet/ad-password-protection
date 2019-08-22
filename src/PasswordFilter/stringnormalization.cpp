#include "stdafx.h"
#include "utils.h"
#include "stringnormalization.h"
#include <cwctype>
#include "Shlwapi.h"
#include "SecureArrayT.h"

SecureArrayT<WCHAR> ToLowerInvariant(const SecureArrayT<WCHAR> &s)
{
	int sizeRequired = 0;

	sizeRequired = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, s.get(), -1, 0, 0);

	if (sizeRequired == 0)
	{
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}

	SecureArrayT<WCHAR> buf(sizeRequired);

	const int sizeAllocated = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, s.get(), -1, buf.get(), sizeRequired);

	if (sizeAllocated == 0)
	{
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}

	return buf;
}

SecureArrayT<WCHAR> NormalizePassword(const SecureArrayT<WCHAR> &password)
{
	if (password.getSize() == 0)
	{
		return SecureArrayT<WCHAR>(0);
	}

	if (!HasLetter(password))
	{
		return password;
	}

	SecureArrayT<WCHAR> newPassword = ToLowerInvariant(password);

	RemoveWhiteSpace(newPassword);
	StrTrim(newPassword.get(), CHARS_TO_TRIM);
	ReplaceChars(newPassword, CHARS_TO_REPLACE);
	RemoveChars(newPassword, CHARS_TO_DELETE);

	return newPassword;
}

bool HasLetter(const SecureArrayT<WCHAR> &s)
{
	for (wchar_t* c = s.get(); *c; ++c)
	{
		if (std::iswalpha(*c))
		{
			return true;
		}
	}

	return false;
}

void RemoveWhiteSpace(SecureArrayT<WCHAR> &s)
{
	LPWSTR cpy = s.get();
	LPWSTR temp = s.get();

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

void RemoveChars(SecureArrayT<WCHAR> &s, const WCHAR *charsToRemove)
{
	LPWSTR cpy = s.get();
	LPWSTR temp = s.get();

	for (size_t i = 0; i < wcslen(s); i++)
	{
		bool matched = false;

		for (size_t j = 0; i < wcslen(charsToRemove); j++)
		{
			if (charsToRemove[j] == *cpy)
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

void ReplaceChars(SecureArrayT<WCHAR> &s, const WCHAR *charPairsToReplace)
{
	LPWSTR cpy = s.get();
	LPWSTR temp = s.get();

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