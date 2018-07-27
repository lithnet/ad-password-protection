#pragma once
#include "stdafx.h"

std::wstring GetRegValueString(const std::wstring &valueName, const std::wstring &defaultValue);

DWORD GetRegValueDWORD(const std::wstring &valueName, DWORD defaultValue);