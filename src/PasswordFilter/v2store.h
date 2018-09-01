#pragma once
#include "binarystore.h"
#include "SecureArrayT.h"

class v2store :
	public binarystore
{
protected:
	std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash);
	SecureArrayT<BYTE> GetHashFromPassword(const SecureArrayT<WCHAR> &password);

public:
	v2store(const std::wstring &basePath);
	~v2store();
};

