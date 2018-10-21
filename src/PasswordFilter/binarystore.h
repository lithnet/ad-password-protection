#pragma once
#include "SecureArrayT.h"

const static int SHA1_HASH_LENGTH = 20;

class binarystore
{
private:
	int hashSize;
	int hashOffset;
	std::wstring storeSubPathPasswordStore;
	std::wstring storeSubPathWordStore;
	std::wstring storeBasePath;
	std::wstring storePathPasswordStore;
	std::wstring storePathWordStore;

public:
	binarystore(const std::wstring& storeBasePath, const std::wstring& storeSubPathPasswordStore, const std::wstring& storeSubPathWordStore, int hashSize, int hashOffset);
	virtual ~binarystore();
	bool IsPasswordInPasswordStore(const SecureArrayT<WCHAR> &password);

	bool IsPasswordInWordStore(const SecureArrayT<WCHAR>& password);

protected:
	virtual std::wstring GetRangeFromHash(const SecureArrayT<BYTE> &hash) = 0;
	virtual SecureArrayT<BYTE> GetHashFromPassword(const SecureArrayT<WCHAR> &password) = 0;

private:
	bool IsHashInPasswordStore(const SecureArrayT<BYTE> &hash);
	bool IsHashInWordStore(const SecureArrayT<BYTE>& hash);
	std::wstring GetPasswordStoreFileName(const std::wstring &range) const;
	std::wstring GetWordStoreFileName(const std::wstring & range) const;
	bool IsHashInBinaryFile(const std::wstring &filename, const SecureArrayT<BYTE> & hashBytes) const;
};
