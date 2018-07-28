#pragma once

bool IsHashInStore(std::wstring hash);

bool IsHashInStore(std::wstring hash, std::wstring range);

std::wstring GetStoreFile(std::wstring range);

bool IsHashInFile(std::wstring filename, std::wstring hash);