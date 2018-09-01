#include "stdafx.h"
#include "v3store.h"
#include "utils.h"
#include "hasher.h"

v3store::v3store(const std::wstring &basePath)
	: binarystore(basePath, L"v3\\p\\", 14, 2)
{
}

v3store::~v3store()
{
}

std::wstring v3store::GetRangeFromHash(const SecureArrayT<BYTE> &hash)
{
	return ToHexString(hash.get(), hash.get() + 2);
}

SecureArrayT<BYTE> v3store::GetHashFromPassword(const SecureArrayT<WCHAR> &password)
{
	return GetNtlmHashBytes(password);
}