#pragma once
const static int SHA1_BINARY_HASH_LENGTH = 20;

class binarystore
{
private:
	int hashSize;
	int hashOffset;
	std::wstring storeSubPath;
	std::wstring storeBasePath;
	std::wstring storePath;

public:
	binarystore(std::wstring storeBasePath, std::wstring storeSubPath, int hashSize);
	~binarystore();
	bool IsPasswordInStore(const LPWSTR & password);

protected:
	virtual std::wstring GetRangeFromHash(const BYTE * hash) = 0;

private:
	bool IsHashInStore(const BYTE * hash);
	std::wstring GetStoreFileName(const std::wstring & range);
	bool IsHashInBinaryFile(const std::wstring & filename, const BYTE * hashBytes);
};