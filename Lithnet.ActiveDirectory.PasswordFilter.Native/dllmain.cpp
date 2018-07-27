#include "stdafx.h"
#include "hasher.h"
#include "dllmain.h"
#include <iostream>
#include "pathcch.h"
#include <vector>
#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <locale>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;

	OutputDebugString(L"*******DLLMain");
}

extern "C" __declspec(dllexport)  BOOLEAN __stdcall InitializeChangeNotify(void)
{
	OutputDebugString(L"*******InitializeChangeNotify");
	//Lithnet::ActiveDirectory::PasswordFilter::Managed::Filter::Test("this is a test1");
	return TRUE;
}

extern "C" __declspec(dllexport) NTSTATUS __stdcall PasswordChangeNotify(
	PUNICODE_STRING UserName,
	ULONG RelativeId,
	PUNICODE_STRING NewPassword
)
{
	OutputDebugString(L"*******PasswordChangeNotify");
	//Lithnet::ActiveDirectory::PasswordFilter::Managed::Filter::Test("this is a test2");
	/*LPWSTR ConvertedUserName = ConvertWideChar(UserName);
	LPWSTR ConvertedNewPassword = ConvertWideChar(NewPassword);
	WriteToLog(ConvertedUserName, ConvertedNewPassword);*/
	return 0;
}

extern "C" __declspec(dllexport) void CALLBACK Test(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
	OutputDebugString(L"*******Test Enter");

	UNICODE_STRING username;
	RtlInitUnicodeString(&username, L"test");

	UNICODE_STRING fullname;
	RtlInitUnicodeString(&fullname, L"fullname");
	wchar_t* output;

	int lenW = ::MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, NULL, 0);
	if (lenW > 0)
	{
		output = new wchar_t[lenW];
		::MultiByteToWideChar(CP_ACP, 0, lpszCmdLine, -1, output, lenW);
	}

	UNICODE_STRING password;
	RtlInitUnicodeString(&password, output);

	PasswordFilter(&username, &fullname, &password, FALSE);

	delete[] output;
}

BOOL isInFile(std::wstring path, std::wstring hash)
{
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			// We don't have a hash store for that file
			return TRUE;
		}

		// If we got an error, what we do next depends on failopen/failclosed setting
	}



	if (hFile)
	{
		CloseHandle(hFile);
	}

	return FALSE;
}

BOOL hasHash(std::wstring filename, std::wstring hash)
{
	std::wifstream hFile(filename.c_str());
	std::wstring line;
	while (std::getline(hFile, line))
	{
		if (line.compare(hash) == 0)
		{
			OutputDebugString(L"Matched");
			OutputDebugString(line.c_str());
			return TRUE;
		}
	}

	return FALSE;
}

static inline void ltrim(std::wstring &s, std::wstring charsToRemove)
{
	int length = charsToRemove.length();

	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](wchar_t ch) {
		for (size_t i = 0; i < length; i++)
		{
			if (charsToRemove[i] == ch)
			{
				return false;
			}
		}

		return true;
	}));
}

static inline void rtrim(std::wstring &s, std::wstring charsToRemove)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [&](wchar_t ch) {
		OutputDebugString(std::wstring(1, ch).c_str());

		for (size_t i = 0; i < charsToRemove.length(); i++)
		{
			OutputDebugString(L"Testing char");
			//OutputDebugString(std::wstring(1, charsToRemove[i]).c_str());

			if (charsToRemove[i] == ch)
			{
				OutputDebugString(L"Match on char");
				return false;
			}
			else
			{
				OutputDebugString(L"No match on char");
			}
		}

		return true;
	}).base(), s.end());
}

// trim from start (in place)
static inline void ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::wstring &s) {
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::wstring ltrim_copy(std::wstring s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::wstring rtrim_copy(std::wstring s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::wstring trim_copy(std::wstring s) {
	trim(s);
	return s;
}

std::wstring remove_ws(const std::wstring& str)
{
	std::wstring str_no_ws;
	for (WCHAR c : str) if (!std::isspace(c)) str_no_ws += c;
	return str_no_ws;
}

void normalize(std::wstring password, std::wstring * normalized)
{
	/*
			password = password.ToLowerInvariant();
			password = new string(password.Where(t => !char.IsWhiteSpace(t)).ToArray());
			password = password.TrimEnd(digitsAndSymbols);
			password = password.TrimStart(digitsAndSymbols);
			password = password
				.Replace("_", "")
				.Replace(".", "")
				.Replace("+", "")
				.Replace('$', 's')
				.Replace('0', 'o')
				.Replace('4', 'a')
				.Replace('3', 'e')
				.Replace('@', 'a')
				.Replace('4', 'a')
				.Replace('^', 'a')
				.Replace('(', 'c')
				.Replace('6', 'g')
				.Replace('1', 'i')
				.Replace('7', 't')
				.Replace('8', 'b')
				.Replace('2', 'z')
				.Replace('!', 'i');

			return password;
	*/
	std::wstring newPassword = password;

	std::transform(newPassword.begin(), newPassword.end(), newPassword.begin(), ::tolower);
	OutputDebugString(L"toLower: ");
	OutputDebugString(newPassword.c_str());

	newPassword = remove_ws(newPassword);

	OutputDebugString(L"whitespace remove: ");
	OutputDebugString(newPassword.c_str());

	std::wstring charsToRemove = L"0123456789!@#$%^&*()[]{};:'\"<>,.?\\/+=_-~`";

	ltrim(newPassword, charsToRemove);

	OutputDebugString(L"ltrim chars: ");
	OutputDebugString(newPassword.c_str());

	charsToRemove = L"0123456789!@#$%^&*()[]{};:'\"<>,.?\\/+=_-~`";

	rtrim(newPassword, charsToRemove);

	OutputDebugString(L"rtrim chars: ");
	OutputDebugString(newPassword.c_str());

	std::wstring charsToReplace = L"$s0o4a3e@a^a(c6g1i7t8b2z!i";

	for (size_t i = 0; i < charsToReplace.length(); i++)
	{
		std::replace(newPassword.begin(), newPassword.end(), charsToReplace[i], charsToReplace[i + 1]);
		i++;
	}

	OutputDebugString(L"replace chars: ");
	OutputDebugString(newPassword.c_str());

	std::wstring charsToDelete = L"_.+";

	for (size_t i = 0; i < charsToDelete.length(); i++)
	{
		newPassword.erase(std::remove(newPassword.begin(), newPassword.end(), charsToDelete[i]), newPassword.end());
	}

	OutputDebugString(L"delete chars: ");
	OutputDebugString(newPassword.c_str());

	*normalized = newPassword;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(
	PUNICODE_STRING AccountName,
	PUNICODE_STRING FullName,
	PUNICODE_STRING Password,
	BOOLEAN SetOperation
)
{
	OutputDebugString(L"*******PasswordFilter 2 Enter");

	std::wstring passwordBuffer(Password->Buffer, Password->Length / sizeof(WCHAR));
	const wchar_t *password = passwordBuffer.c_str();

	OutputDebugString(L"Password: ");
	OutputDebugString(password);
	//DebugBreak();

	std::wstring newPassword;
	normalize(passwordBuffer, &newPassword);

	OutputDebugString(L"Normalized: ");
	OutputDebugString(newPassword.c_str());

	std::wstring accountNameBuffer(AccountName->Buffer, AccountName->Length / sizeof(WCHAR));
	const wchar_t *accountName = accountNameBuffer.c_str();

	OutputDebugString(L"Account Name: ");
	OutputDebugString(accountName);

	std::wstring fullNameBuffer(FullName->Buffer, FullName->Length / sizeof(WCHAR));
	const wchar_t *fullName = fullNameBuffer.c_str();

	OutputDebugString(L"Full Name: ");
	OutputDebugString(fullName);

	std::wstring hash;

	const wchar_t *np = newPassword.c_str();
	OutputDebugString(L"Normalized password length");
	OutputDebugString(std::to_wstring(newPassword.length()).c_str());
	make_hash(np, &hash);
	OutputDebugString(hash.c_str());

	std::wstring range = hash.substr(0, 5);
	OutputDebugString(range.c_str());

	std::wstring path = L"D:\\pwnedpwds\\store\\";
	path = path.append(range);
	path = path.append(L".txt");

	OutputDebugString(path.c_str());

	BOOL r = hasHash(path, hash);

	if (r == TRUE)
	{
		OutputDebugString(L"Has hash");
	}
	else
	{
		OutputDebugString(L"Does not have hash");
	}

	OutputDebugString(L"*******PasswordFilter Exit");

	return r;
}

