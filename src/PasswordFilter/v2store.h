#pragma once
#include "binarystore.h"
#include "SecureArrayT.h"

class v2store :
	public binarystore
{
protected:
	std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash);
public:
	v2store(const std::wstring &basePath);
	~v2store();
};

