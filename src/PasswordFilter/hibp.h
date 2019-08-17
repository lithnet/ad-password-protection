#pragma once
#include "SecureArrayT.h"
#include "registry.h"
#include <winhttp.h>

static LPCWSTR DefaultHibpSha1ApiUrl = L"https://api.pwnedpasswords.com/range/{range}";
static LPCWSTR UserAgent = L"Lithnet Password Protection for Active Directory/1.0";

bool IsInHibpSha1Api(const SecureArrayT<WCHAR> &password);
std::wstring GetHttpResponse(const std::wstring &host, const INTERNET_PORT port, const std::wstring &path, bool usetls);
bool IsInVariableWidthRangeData(const std::wstring &rangeData, const std::wstring &value);
void CrackUrl(const std::wstring& url, URL_COMPONENTS &urlComponents);