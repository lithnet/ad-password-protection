#pragma once
#include "binarystore.h"
class v2store :
	public binarystore
{
protected:
	std::wstring GetRangeFromHash(const BYTE * hash);
public:
	v2store(const std::wstring &basePath);
	~v2store();
};

