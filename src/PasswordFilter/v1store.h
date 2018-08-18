#pragma once
#include "binarystore.h"
class v1store :
	public binarystore
{
protected: 
	std::wstring GetRangeFromHash(const BYTE * hash);
public:
	v1store(const std::wstring &basePath);
	~v1store();
};

