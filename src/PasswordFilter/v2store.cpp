#include "stdafx.h"
#include "v2store.h"
#include "utils.h"
#include "hasher.h"

v2store::v2store(const std::wstring &basePath)
	: binarystore(basePath, L"v2\\p\\", 18, 2)
{
}

v2store::~v2store()
= default;

std::wstring v2store::GetRangeFromHash(const SecureArrayT<BYTE> &hash)
{
	return ToHexString(hash.get(), hash.get() + 2);
}

SecureArrayT<BYTE> v2store::GetHashFromPassword(const SecureArrayT<WCHAR> &password)
{
	return GetSha1HashBytes(password);
}