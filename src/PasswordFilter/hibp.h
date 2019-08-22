#pragma once
#include "SecureArrayT.h"
#include "registry.h"
#include <winhttp.h>

static LPCWSTR DefaultHibpSha1ApiUrl = L"https://api.pwnedpasswords.com/range/{range}";
static LPCWSTR DefaultHibpNtlmHexApiUrl = L"http://localhost/ntlm/hex/range/{range}";
static LPCWSTR DefaultHibpNtlmBinaryApiUrl = L"http://localhost/ntlm/binary-nopwncount/range/{range}";
static LPCWSTR DefaultHibpNtlmBinaryPwnCountApiUrl = L"http://localhost/ntlm/binary-pwncount/range/{range}";

static LPCWSTR UserAgent = L"Lithnet Password Protection for Active Directory/1.0";

int IsInHibpSha1Api(const SecureArrayT<WCHAR> &password, const registry &reg);
int IsInHibpNtlmHexApi(const SecureArrayT<WCHAR> &password, const registry &reg);
int IsInHibpNtlmBinaryApi(const SecureArrayT<WCHAR> &password, const registry &reg);
int IsInHibpNtlmBinaryPwnCountApi(const SecureArrayT<WCHAR> &password, const registry &reg);
int IsInFixedWidthRangeData(std::vector<BYTE>& rangeData, const SecureArrayT<BYTE> &hashBytes, const size_t& record_size);
int IsInVariableWidthRangeData(const std::wstring &rangeData, const std::wstring &value);
void CrackUrl(const std::wstring& url, URL_COMPONENTS &urlComponents);
std::vector<BYTE> GetHttpResponseBinary(const std::wstring &url);
std::wstringstream GetHttpResponseString(const std::wstring &url);