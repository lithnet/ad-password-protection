#pragma once
#include "SecureArrayT.h"
#include "registry.h"

BOOLEAN ProcessPasswordComplexityThreshold(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordComplexityPoints(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);


