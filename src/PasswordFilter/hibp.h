#pragma once
#include "SecureArrayT.h"
#include "registry.h"
#include <winhttp.h>

static LPCWSTR DefaultHibpAddress = L"api.pwnedpasswords.com";
static LPCWSTR UserAgent = L"Lithnet Password Protection for Active Directory/1.0";

bool IsInSha1HibpApi(const SecureArrayT<WCHAR> &password);
std::wstring GetHttpResponse(const std::wstring &host, const INTERNET_PORT port, const std::wstring &path);
bool IsInVariableWidthRangeData(const std::wstring &rangeData, const std::wstring &value);
