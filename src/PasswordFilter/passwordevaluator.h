#pragma once
#include "registry.h"

BOOLEAN ProcessPassword(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

BOOLEAN ProcessPasswordRaw(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordNormalized(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordLength(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordDoesntContainAccountName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordDoesntContainFullName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordRegexApprove(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordRegexReject(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordComplexityThreshold(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);

BOOLEAN ProcessPasswordComplexityPoints(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg);
