#include "stdafx.h"
#include "TestString.h"

TestString::TestString(std::wstring str)
	: SecureArrayT<WCHAR>((wcslen(str.c_str()) + 1) * sizeof(wchar_t))
{
	size_t i;
	WCHAR* p = get();

	for (i = 0; i < wcslen(str.c_str()); i++)
	{
		p[i] = str[i];
	}

	p[i] = '\0';
}

TestString::TestString(LPWSTR str)
	: SecureArrayT<WCHAR>((wcslen(str) + 1) * sizeof(wchar_t))
{
	size_t i;
	WCHAR* p = get();

	for (i = 0; i < wcslen(str); i++)
	{
		p[i] = str[i];
	}

	p[i] = '\0';
}
