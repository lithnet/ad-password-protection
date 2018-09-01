#pragma once
#include "SecureArrayT.h"

const static int SHA1_HASH_LENGTH = 20;

class binarystore
{
private:
	int hashSize;
	int hashOffset;
	std::wstring storeSubPath;
	std::wstring storeBasePath;
	std::wstring storePath;

public:
	binarystore(std::wstring storeBasePath, std::wstring storeSubPath, int hashSize, int hashOffset);
	~binarystore();
	bool IsPasswordInStore(const SecureArrayT<WCHAR> &password);

protected:
	virtual std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash) = 0;
	virtual SecureArrayT<BYTE> GetHashFromPassword(const SecureArrayT<WCHAR> &password) = 0;

private:
	bool IsHashInStore(const SecureArrayT<BYTE> &hash);
	std::wstring GetStoreFileName(const std::wstring &range);
	bool IsHashInBinaryFile(const std::wstring &filename, const SecureArrayT<BYTE> &hash);
};
