#pragma once
#include "stdafx.h"
#include <vector>

std::vector<std::wstring> SplitString(const std::wstring & text, wchar_t sep);

wchar_t * UnicodeStringToWcharArray(UNICODE_STRING str);

void odprintf(const wchar_t * format, ...);