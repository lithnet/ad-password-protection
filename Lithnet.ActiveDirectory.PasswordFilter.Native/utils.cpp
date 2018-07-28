#include "utils.h"
#include "stdafx.h"
#include "filter.h"
#include "registry.h"
#include <iostream>

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

extern "C" __declspec(dllexport) void CALLBACK CheckPasswordSet(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******Check password enter");

	UNICODE_STRING username;
	RtlInitUnicodeString(&username, L"test");

	UNICODE_STRING fullname;
	RtlInitUnicodeString(&fullname, L"fullname");
	wchar_t* commandLineArgument;

	int lenW = MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		commandLineArgument = new wchar_t[lenW];
		MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, commandLineArgument, lenW);
	}

	UNICODE_STRING password;
	//RtlInitUnicodeString(&password, L"Pass\u1E9Eword");
	RtlInitUnicodeString(&password, commandLineArgument);

	BOOL result = PasswordFilter(&username, &fullname, &password, TRUE);

	if (result == TRUE)
	{
		OutputDebugString(L"Password filter passed");
	}
	else 
	{
		OutputDebugString(L"Password filter denied the password change");
	}

	delete[] commandLineArgument;

	OutputDebugString(L"*******Check password leave");
}

extern "C" __declspec(dllexport) void CALLBACK CheckPasswordChange(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******Check password enter");

	UNICODE_STRING username;
	RtlInitUnicodeString(&username, L"test");

	UNICODE_STRING fullname;
	RtlInitUnicodeString(&fullname, L"fullname");
	wchar_t* commandLineArgument;

	int lenW = MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		commandLineArgument = new wchar_t[lenW];
		MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, commandLineArgument, lenW);
	}

	UNICODE_STRING password;
	//RtlInitUnicodeString(&password, L"Pass\u1E9Eword");
	RtlInitUnicodeString(&password, commandLineArgument);

	BOOL result = PasswordFilter(&username, &fullname, &password, FALSE);

	if (result == TRUE)
	{
		OutputDebugString(L"Password filter passed");
	}
	else
	{
		OutputDebugString(L"Password filter denied the password change");
	}

	delete[] commandLineArgument;

	OutputDebugString(L"*******Check password leave");
}

extern "C" __declspec(dllexport) void CALLBACK GetRegString(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******GetRegString Enter");

	wchar_t* commandLineArgument;

	int lenW = MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		commandLineArgument = new wchar_t[lenW];
		MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, commandLineArgument, lenW);
	}

	std::wstring result = GetRegValue(commandLineArgument, L"This is a default value");

	odprintf(L"Got value: %s", result.c_str());
}

extern "C" __declspec(dllexport) void CALLBACK GetRegDword(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******GetRegDword Enter");

	wchar_t* commandLineArgument;

	int lenW = MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		commandLineArgument = new wchar_t[lenW];
		MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, commandLineArgument, lenW);
	} 

	DWORD result = GetRegValue(commandLineArgument, 1);

	odprintf(L"Got value: %d", result);
}