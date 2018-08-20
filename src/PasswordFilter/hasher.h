#pragma once
#include "stdio.h"
#include "SecureArrayT.h"

SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<WCHAR> &input);
SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<char> &input);