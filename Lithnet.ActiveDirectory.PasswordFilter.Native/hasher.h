#pragma once
#include "stdio.h"

std::wstring GetSha1HashString(const std::wstring data);
std::wstring GetSha1HashString(const std::string data);

void GetSha1HashBytes(const std::wstring input, BYTE * hashBytes, int hashBytesLength);

void GetSha1HashBytes(const std::string input, BYTE * hashBytes, int hashBytesLength);
