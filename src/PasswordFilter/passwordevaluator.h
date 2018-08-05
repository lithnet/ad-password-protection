#pragma once

__declspec(dllexport) BOOLEAN ProcessPassword(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRaw(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordNormalized(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordLength(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRegexApprove(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordRegexReject(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordComplexityThreshold(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

__declspec(dllexport) BOOLEAN ProcessPasswordComplexityPoints(LPWSTR password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);
