#pragma once
#include "stdafx.h"
#include <vector>

std::vector<std::wstring> SplitString(const std::wstring &text, const wchar_t sep);

LPWSTR UnicodeStringToWcharArray(const UNICODE_STRING &str);