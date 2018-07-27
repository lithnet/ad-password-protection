#pragma once

BOOL checkHash(std::wstring hash);

BOOL checkHash(std::wstring hash, std::wstring range);

std::wstring GetStoreFile(std::wstring range);

BOOL hasHash(std::wstring filename, std::wstring hash);