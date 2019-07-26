#pragma once
#include "SecureArrayT.h"
#include "policy.h"

BOOLEAN ProcessPasswordComplexityThreshold(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordComplexityPoints(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);


