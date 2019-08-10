#include "stdafx.h"
#include "v3store.h"
#include "utils.h"
#include "hasher.h"

v3store::v3store(const std::wstring &basePath)
	: binarystore(basePath, L"v3\\p\\", L"v3\\w\\", 14, 2)
{
}

v3store::~v3store()
= default;

std::wstring v3store::GetRangeFromHash(const SecureArrayT<BYTE> &hash)
{
	return ToHexWString(hash.get(), hash.get() + 2);
}

SecureArrayT<BYTE> v3store::GetHashFromPassword(const SecureArrayT<WCHAR> &password)
{
	return GetNtlmHashBytes(password);
}