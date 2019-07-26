#pragma once
#include "SecureArrayT.h"
#include "policy.h"

static const int PASSWORD_APPROVED = 0;
static const int PASSWORD_REJECTED_LENGTH = 1;
static const int PASSWORD_REJECTED_COMPLEXITY_THRESHOLD = 2;
static const int PASSWORD_REJECTED_COMPLEXITY_POINTS = 3;
static const int PASSWORD_REJECTED_REGEX_APPROVE = 4;
static const int PASSWORD_REJECTED_REGEX_REJECT = 5;
static const int PASSWORD_REJECTED_CONTAINS_ACCOUNT_NAME = 6;
static const int PASSWORD_REJECTED_CONTAINS_FULL_NAME = 7;
static const int PASSWORD_REJECTED_BANNED = 8;
static const int PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD = 9;
static const int PASSWORD_REJECTED_BANNED_NORMALIZED_WORD = 10;
static const int PASSWORD_REJECTED_BLANK = 11;
static const int FILTER_ERROR = 100;

int ProcessPassword(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation);

BOOLEAN ProcessPasswordRaw(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordNormalizedPasswordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordNormalizedWordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordLength(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordDoesntContainAccountName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordDoesntContainFullName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordRegexApprove(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);

BOOLEAN ProcessPasswordRegexReject(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol);