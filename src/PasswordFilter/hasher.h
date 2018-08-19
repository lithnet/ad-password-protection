#pragma once
#include "stdio.h"
#include "SecureSharedPtr.h"

SecureArrayT<BYTE> GetSha1HashBytes(const LPWSTR &input);
SecureArrayT<BYTE> GetSha1HashBytes(const LPSTR &input);