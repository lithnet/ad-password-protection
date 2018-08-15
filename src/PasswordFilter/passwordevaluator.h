#pragma once

__declspec(dllexport) BOOLEAN ProcessPassword(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRaw(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordNormalized(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordLength(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordDoesntContainAccountName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordDoesntContainFullName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRegexApprove(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRegexReject(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordComplexityThreshold(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordComplexityPoints(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);
