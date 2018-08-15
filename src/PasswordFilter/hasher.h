#pragma once
#include "stdio.h"

void GetSha1HashBytes(const LPWSTR &input, BYTE* hashBytes, const int &hashBytesLength);

void GetSha1HashBytes(const LPSTR &input, BYTE* hashBytes, const int &hashBytesLength);
