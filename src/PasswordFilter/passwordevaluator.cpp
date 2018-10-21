#include "stdafx.h"
#include "registry.h"
#include "messages.h"
#include "eventlog.h"
#include "stringnormalization.h"
#include "passwordevaluator.h"
#include <regex>
#include <Shlwapi.h>
#include "utils.h"
#include "complexityevaluator.h"
#include "v3store.h"

static registry baseReg;
static v3store store(baseReg.GetRegValue(L"Store", L""));

int ProcessPassword(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation)
{
	eventlog::getInstance().logw(EVENTLOG_INFORMATION_TYPE, MSG_PROCESSING_REQUEST, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	const registry reg = registry::GetRegistryForUser(accountName);

	if (!ProcessPasswordLength(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_LENGTH;
	}

	if (!ProcessPasswordComplexityThreshold(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_COMPLEXITY_THRESHOLD;
	}

	if (!ProcessPasswordComplexityPoints(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_COMPLEXITY_POINTS;
	}

	if (!ProcessPasswordRegexApprove(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_REGEX_APPROVE;
	}

	if (!ProcessPasswordRegexReject(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_REGEX_APPROVE;
	}

	if (!ProcessPasswordDoesntContainAccountName(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_CONTAINS_ACCOUNT_NAME;
	}

	if (!ProcessPasswordDoesntContainFullName(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_CONTAINS_FULL_NAME;
	}

	if (!ProcessPasswordRaw(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_BANNED;
	}

	if (!ProcessPasswordNormalizedPasswordStore(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD;
	}

	if (!ProcessPasswordNormalizedWordStore(password, accountName, fullName, setOperation, reg))
	{
		return PASSWORD_REJECTED_BANNED_NORMALIZED_WORD;
	}

	OutputDebugString(L"Password was approved by all modules");

	eventlog::getInstance().logw(EVENTLOG_SUCCESS, MSG_PASSWORD_APPROVED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	return PASSWORD_APPROVED;
}

BOOLEAN ProcessPasswordRaw(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	if ((setOperation && reg.GetRegValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 1) != 0) || 
		(!setOperation && reg.GetRegValue(REG_VALUE_CHECKBANNEDPASSWORDONCHANGE, 1) != 0))
	{
		OutputDebugString(L"Checking raw password");

		if (store.IsPasswordInPasswordStore(password))
		{
			OutputDebugString(L"Rejected password as it was found in the banned store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Raw password did not match any existing hashes");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordNormalizedPasswordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	if (setOperation && reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 1) != 0 ||
		!setOperation && reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONCHANGE, 1) != 0 )
	{
		bool result = TRUE;

		const SecureArrayT<WCHAR> normalizedPassword = NormalizePassword(password);
		OutputDebugString(L"Checking normalized password");

		result = store.IsPasswordInPasswordStore(normalizedPassword);
		if (result)
		{
			OutputDebugString(L"Rejected normalized password as it was found in the banned password store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
		}
		else
		{
			OutputDebugString(L"Normalized password did not match any existing hashes in the password store");
		}

		return !result;
	}

	return TRUE;
}


BOOLEAN ProcessPasswordNormalizedWordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	if (setOperation && reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1) != 0 ||
		!setOperation && reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 1) != 0)
	{
		bool result = TRUE;

		const SecureArrayT<WCHAR> normalizedPassword = NormalizePassword(password);
		OutputDebugString(L"Checking normalized password");

		result = store.IsPasswordInWordStore(normalizedPassword);
		if (result)
		{
			OutputDebugString(L"Rejected normalized password as it was found in the banned word store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_WORD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
		}
		else
		{
			OutputDebugString(L"Normalized password did not match any existing hashes in the word store");
		}

		return !result;
	}

	return TRUE;
}


BOOLEAN ProcessPasswordLength(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	const int minLength = reg.GetRegValue(REG_VALUE_MINIMUMLENGTH, 0);

	if (minLength > 0)
	{
		OutputDebugString(L"Checking minimum length");

		if (wcslen(password) < minLength)
		{
			OutputDebugString(L"Rejected password as it did not meet the minimum length requirements");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_MINLENGTH, 5, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(wcslen(password)).c_str(), std::to_wstring(minLength).c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the minimum length requirements");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordDoesntContainAccountName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int flag = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 0);

	if (flag != 0 && accountName.length() > 3)
	{
		OutputDebugString(L"Checking to see if password contains account name");

		for each (std::wstring substring in SplitString(accountName, L' '))
		{
			if (substring.length() > 3)
			{
				if (StrStrI(password, substring.c_str()) != NULL)
				{
					OutputDebugString(L"Rejected password as it contained part of the account name");
					eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_CONTAINS_ACCOUNTNAME, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
					return FALSE;
				}
			}
		}

		OutputDebugString(L"Password did not contain part of the account name");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordDoesntContainFullName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int flag = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 0);

	if (flag != 0 && fullName.length() > 3)
	{
		OutputDebugString(L"Checking to see if password contains full name");

		for each (std::wstring substring in SplitString(fullName, L' '))
		{
			if (substring.length() > 3)
			{
				if (StrStrI(password, substring.c_str()) != NULL)
				{
					OutputDebugString(L"Rejected password as it contained part of the full name");
					eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_CONTAINS_DISPLAYNAME, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
					return FALSE;
				}
			}
		}

		OutputDebugString(L"Password did not contain any part of the full name");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexApprove(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	std::wstring regex = reg.GetRegValue(REG_VALUE_REGEXAPPROVE, L"");

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression approval match");

		std::wregex e(regex);

		if (!std::regex_match(password.get(), e))
		{
			OutputDebugString(L"Password did not match the approval regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_APPROVAL_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the approval regular expression");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexReject(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	std::wstring regex = reg.GetRegValue(REG_VALUE_REGEXREJECT, L"");

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression rejection match");

		std::wregex e(regex);

		if (std::regex_match(password.get(), e))
		{
			OutputDebugString(L"Password matched the rejection regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_REJECTION_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password not did match the rejection regular expression");
	}

	return TRUE;
}
