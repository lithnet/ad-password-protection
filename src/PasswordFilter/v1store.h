#pragma once
#include "binarystore.h"
#include "SecureArrayT.h"

class v1store :
	public binarystore
{
protected: 
	std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash);
public:
	v1store(const std::wstring &basePath);
	~v1store();
};

