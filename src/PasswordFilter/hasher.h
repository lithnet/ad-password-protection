#pragma once
#include "SecureArrayT.h"

SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<WCHAR> &input);
SecureArrayT<BYTE> GetSha1HashBytes(const SecureArrayT<char> &input);
SecureArrayT<BYTE> GetNtlmHashBytes(const SecureArrayT<WCHAR> &input);