#pragma once
#include "SecureArrayT.h"
#include "registry.h"

static LPCWSTR DefaultHibpAddress = L"api.pwnedpasswords.com";
static LPCWSTR UserAgent = L"Lithnet Password Protection for Active Directory/1.0";

bool IsInHibp(const SecureArrayT<WCHAR> &password, const registry &reg);
std::wstring GetHibpRangeData(const std::wstring &range, const registry &reg);
