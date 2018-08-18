#pragma once
#include "stdafx.h"
#include <vector>

std::vector<std::wstring> SplitString(const std::wstring &text, const wchar_t sep);

LPWSTR UnicodeStringToWcharArray(const UNICODE_STRING &str);

bool DirectoryExists(const std::wstring& dirName_in);

template<typename T>
std::wstring ToHexString(T first, T last, bool use_uppercase = true, bool insert_spaces = false);