#pragma once

bool IsPasswordInStore(const LPWSTR &password);

bool IsHashInEseStore(const BYTE *hash);

//bool IsHashInStore(std::wstring hash);

bool IsHashInStorev1(const BYTE * hash);

bool IsHashInStorev2(const BYTE * hash);

//bool IsHashInStore(std::wstring hash, std::wstring range);

bool IsHashInStorev1(const BYTE * hash, const std::wstring &range);

bool IsHashInStorev2(const BYTE * hash, const std::wstring &range);

std::wstring GetStoreFileName(const std::wstring &range);

std::wstring GetStoreFileNamev1(const std::wstring &range);

std::wstring GetStoreFileNamev2(const std::wstring &range);

//bool IsHashInFileTextFile(std::wstring filename, std::wstring hash);

//bool IsHashInTextFileBinarySearch(std::wstring filename, std::wstring hash);

bool IsHashInBinaryFilev1(const std::wstring &filename, const BYTE * hashBytes);

bool IsHashInBinaryFilev2(const std::wstring &filename, const BYTE * hashBytes);

//const int SHA1_HASH_LENGTH = 40;

//const int SHA1_HASH_ROW_TERMINATOR_LENGTH = 2;

const int SHA1_BINARY_HASH_LENGTH = 20;

const int SHA1_PARTIAL_BINARY_HASH_LENGTH = 18;

const int SHA1_BINARY_HASH_ROW_TERMINATOR_LENGTH = 0;

