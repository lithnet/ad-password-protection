#pragma once
#include "SecureArrayT.h"
#include "registry.h"

static LPCWSTR DefaultHibpAddress = L"api.pwnedpasswords.com";
static LPCWSTR UserAgent = L"Lithnet Password Protection for Active Directory/1.0";

bool IsInHibp(const SecureArrayT<WCHAR> &password);
std::wstring GetHibpRangeData(const std::wstring &range);
bool IsInRangeData(const std::wstring &rangeData, const std::wstring &value);
