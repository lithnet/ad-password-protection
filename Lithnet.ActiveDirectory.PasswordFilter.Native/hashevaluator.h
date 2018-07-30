#pragma once

bool IsHashInStore(std::wstring hash);

bool IsHashInStore(std::wstring hash, std::wstring range);

std::wstring GetStoreFileName(std::wstring range);

bool IsHashInFile(std::wstring filename, std::wstring hash);

bool IsHashInFileBS(std::wstring filename, std::wstring hash);

const int SHA1_HASH_LENGTH = 40;

const int ROW_TERMINATOR_LENGTH = 2;