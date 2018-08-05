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

extern "C" __declspec(dllexport) void CALLBACK Log(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******Log enter");
	wchar_t* commandLineArgument;

	int lenW = MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		commandLineArgument = new wchar_t[lenW];
		MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, commandLineArgument, lenW);
	}

	eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_PASSWORD_REJECTED_BANNED, 1, commandLineArgument);
	OutputDebugString(L"*******Log exit");
}

void CheckPassword(const LPSTR &lpszCmdLine, BOOL SetPassword)
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

	BOOL result = PasswordFilter(&username, &fullname, &password, SetPassword);

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

extern "C" __declspec(dllexport) void CALLBACK TestESE(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	DebugBreak();
	esestore::getInstance();
}


extern "C" __declspec(dllexport) void CALLBACK CheckPasswordSet(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	CheckPassword(lpszCmdLine, TRUE);
}

extern "C" __declspec(dllexport) void CALLBACK CheckPasswordChange(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	CheckPassword(lpszCmdLine, FALSE);
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