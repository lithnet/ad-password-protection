#pragma once

BOOLEAN ProcessPassword(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordRaw(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordNormalized(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordLength(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordRegexApprove(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordRegexReject(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordComplexityThreshold(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);

BOOLEAN ProcessPasswordComplexityPoints(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation);
