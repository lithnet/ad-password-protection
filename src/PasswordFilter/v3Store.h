#pragma once
#include "binarystore.h"
#include "SecureArrayT.h"

class v3store : public binarystore
{
protected:
	std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash) override;
	SecureArrayT<BYTE> GetHashFromPassword(const SecureArrayT<WCHAR> &password) override;

public:
	v3store(const std::wstring &basePath);
	~v3store();
};

