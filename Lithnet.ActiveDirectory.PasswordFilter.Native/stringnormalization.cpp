#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "utils.h"
#include "stringnormalization.h"

void normalize(std::wstring password, std::wstring * normalized)
{
	std::wstring newPassword = password;

	std::transform(newPassword.begin(), newPassword.end(), newPassword.begin(), ::tolower);
	odprintf(L"ToLower: %s", newPassword.c_str());

	newPassword = remove_ws(newPassword);

	odprintf(L"Whitespace remove: %s", newPassword.c_str());

	std::wstring charsToRemove = L"0123456789!@#$%^&*()[]{};:'\"<>,.?\\/+=_-~`";

	ltrim(newPassword, charsToRemove);

	odprintf(L"Ltrim: %s", newPassword.c_str());

	rtrim(newPassword, charsToRemove);

	odprintf(L"Rtrim: %s", newPassword.c_str());

	std::wstring charsToReplace = L"$s0o4a3e@a^a(c6g1i7t8b2z!i";

	for (size_t i = 0; i < charsToReplace.length(); i++)
	{
		std::replace(newPassword.begin(), newPassword.end(), charsToReplace[i], charsToReplace[i + 1]);
		i++;
	}

	odprintf(L"char sub: %s", newPassword.c_str());


	std::wstring charsToDelete = L"_.+";

	for (size_t i = 0; i < charsToDelete.length(); i++)
	{
		newPassword.erase(std::remove(newPassword.begin(), newPassword.end(), charsToDelete[i]), newPassword.end());
	}

	odprintf(L"delete chars: %s", newPassword.c_str());

	*normalized = newPassword;
}


static inline void ltrim(std::wstring &s, std::wstring charsToRemove)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](wchar_t ch) {
		for (size_t i = 0; i < charsToRemove.length(); i++)
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
		for (size_t i = 0; i < charsToRemove.length(); i++)
		{
			if (charsToRemove[i] == ch)
			{
				return false;
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

std::wstring remove_ws(const std::wstring& str)
{
	std::wstring str_no_ws;
	for (WCHAR c : str) if (!std::isspace(c)) str_no_ws += c;
	return str_no_ws;
}