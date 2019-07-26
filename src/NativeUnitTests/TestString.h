#pragma once
#include "SecureArrayT.h"

class TestString : public SecureArrayT<WCHAR>
{
public:
	TestString(std::wstring str);
	TestString(LPWSTR str);
	~TestString() = default;
};
