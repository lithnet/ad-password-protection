// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"
#include <windows.h>
#include <winnt.h>
//#include <NTSecAPI.h>
#include <stdio.h>
#include <winternl.h>
#include <string>
#include "TestUtils.h"
#include "../PasswordFilter/SecureArrayT.h"

class TestString : public SecureArrayT<WCHAR>
{
public:

	TestString(std::wstring str)
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

	TestString(LPWSTR str)
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
};