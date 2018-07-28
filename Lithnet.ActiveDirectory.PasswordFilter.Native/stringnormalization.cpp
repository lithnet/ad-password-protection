#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "utils.h"
#include "stringnormalization.h"
#include "registry.h"
#include <cwctype>

std::wstring ToLowerInvariant(const std::wstring& s)
{
	// on Windows use LCMapString to handle internationalized-aware lowercasing
	if (s.empty())
	{
		return s;
	}

	const wchar_t* inBuf = s.c_str();
	int result = 0;

	// Find the size of the buffer we need.
	result = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, inBuf, s.size(), 0, 0);

	if (result == 0)
	{
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}

	wchar_t* buf = new wchar_t[result];

	// Get lowercased string.
	result = LCMapString(LOCALE_INVARIANT, LCMAP_LOWERCASE, inBuf, s.size(), buf, result);

	if (result == 0)
	{
		delete[](buf);
		throw std::system_error(GetLastError(), std::system_category(), "LCMapString failed");
	}
	
	std::wstring transformed = std::wstring(buf, result);

	delete[](buf);

	return transformed;
}

std::wstring NormalizePassword(std::wstring password)
{
	if (password.length() == 0)
	{
		return password;
	}

	std::wstring newPassword = ToLowerInvariant(password);

	odprintf(L"ToLower: %s", newPassword.c_str());

	newPassword = RemoveWhiteSpace(newPassword);

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

	return newPassword;
}


void ltrim(std::wstring &s, std::wstring charsToRemove)
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

void rtrim(std::wstring &s, std::wstring charsToRemove)
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

void ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) 
	{
		return !std::iswspace(ch);
	}));
}

void rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) 
	{
		return !std::iswspace(ch);
	}).base(), s.end());
}

void trim(std::wstring &s) {
	ltrim(s);
	rtrim(s);
}

std::wstring RemoveWhiteSpace(const std::wstring &str)
{
	std::wstring str_no_ws;

	for (wchar_t c : str)
	{
		if (!std::iswspace(c))
		{
			str_no_ws += c;
		}
	}

	return str_no_ws;
}