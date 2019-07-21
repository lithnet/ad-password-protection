#pragma once
#include <vector>
#include "stdafx.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include "SecureArrayT.h"

std::vector<std::wstring> SplitString(const std::wstring &text, const wchar_t sep);

SecureArrayT<WCHAR> UnicodeStringToWcharArray(const UNICODE_STRING &str);

SecureArrayT<WCHAR> StringToWcharArray(const LPCWSTR str);

bool DirectoryExists(const std::wstring& dirName);

template<typename T>
std::wstring ToHexWString(T first, T last, bool use_uppercase = true, bool insert_spaces = false)
{
	std::wstringstream ss;
	ss << std::hex << std::setfill(L'0');

	if (use_uppercase)
	{
		ss << std::uppercase;
	}

	while (first != last)
	{
		ss << std::setw(2) << static_cast<int>(*first++);

		if (insert_spaces && first != last)
		{
			ss << " ";
		}
	}

	return ss.str();
}

template<typename T>
std::string ToHexString(T first, T last, bool use_uppercase = true, bool insert_spaces = false)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');

	if (use_uppercase)
	{
		ss << std::uppercase;
	}

	while (first != last)
	{
		ss << std::setw(2) << static_cast<int>(*first++);

		if (insert_spaces && first != last)
		{
			ss << " ";
		}
	}

	return ss.str();
}


LPCWSTR GetInteropString(LPCWSTR value);