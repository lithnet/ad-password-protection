#include "stdafx.h"
#include "v2store.h"
#include "utils.h"

v2store::v2store(const std::wstring &basePath)
	: binarystore(basePath, L"v2\\", 18)
{
}

v2store::~v2store()
{
}

std::wstring v2store::GetRangeFromHash(const BYTE * hash)
{
	return ToHexString(hash, hash + 2);
}