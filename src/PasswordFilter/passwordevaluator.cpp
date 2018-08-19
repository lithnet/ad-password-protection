#include "stdafx.h"
#include "registry.h"
#include "messages.h"
#include "eventlog.h"
#include "hasher.h"
#include "stringnormalization.h"
#include "hashevaluator.h"
#include "passwordevaluator.h"
#include <regex>
#include <cwctype>
#include <shlwapi.h>
#include "utils.h"

BOOLEAN ProcessPassword(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation)
{
	eventlog::getInstance().logw(EVENTLOG_INFORMATION_TYPE, MSG_PROCESSING_REQUEST, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	registry reg = registry::GetRegistryForUser(accountName);

	if (!ProcessPasswordLength(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordComplexityThreshold(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordComplexityPoints(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordRegexApprove(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordRegexReject(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordDoesntContainAccountName(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordDoesntContainFullName(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordRaw(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	if (!ProcessPasswordNormalized(password, accountName, fullName, setOperation, reg))
	{
		return FALSE;
	}

	OutputDebugString(L"Password was approved by all modules");

	eventlog::getInstance().logw(EVENTLOG_SUCCESS, MSG_PASSWORD_APPROVED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	return TRUE;
}

BOOLEAN ProcessPasswordRaw(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	if ((setOperation && reg.GetRegValue(L"ValidateRawPasswordOnSet", 1) != 0) || (!setOperation && reg.GetRegValue(L"ValidateRawPasswordOnChange", 1) != 0))
	{
		OutputDebugString(L"Checking raw password");

		if (IsPasswordInStore(password))
		{
			OutputDebugString(L"Rejected password as it was found in the banned store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Raw password did not match any existing hashes");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordNormalized(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	if ((setOperation && reg.GetRegValue(L"ValidateNormalizedPasswordOnSet", 1) != 0) || (!setOperation && reg.GetRegValue(L"ValidateNormalizedPasswordOnChange", 1) != 0))
	{
		LPWSTR normalizedPassword;
		bool result = TRUE;

		try
		{
			normalizedPassword = NormalizePassword(password);
			OutputDebugString(L"Checking normalized password");

			result = IsPasswordInStore(normalizedPassword);
			if (result)
			{
				OutputDebugString(L"Rejected normalized password as it was found in the banned store");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			}
			else
			{
				OutputDebugString(L"Normalized password did not match any existing hashes");
			}

			SecureZeroMemory(normalizedPassword, wcslen(normalizedPassword));
			delete normalizedPassword;

			return !result;
		}
		catch (...)
		{
			if (normalizedPassword)
			{
				SecureZeroMemory(normalizedPassword, wcslen(normalizedPassword));
				delete normalizedPassword;
			}

			throw;
		}
	}

	return TRUE;
}

BOOLEAN ProcessPasswordLength(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int minLength = reg.GetRegValue(L"MinimumLength", 0);

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

BOOLEAN ProcessPasswordDoesntContainAccountName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int flag = reg.GetRegValue(L"ValidatePasswordDoesntContainAccountName", 0);

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


BOOLEAN ProcessPasswordDoesntContainFullName(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int flag = reg.GetRegValue(L"ValidatePasswordDoesntContainFullName", 0);

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

BOOLEAN ProcessPasswordRegexApprove(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	std::wstring regex = reg.GetRegValue(L"RegexApprove", L"");

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression approval match");

		std::wregex e(regex);

		if (!std::regex_match(password, e))
		{
			OutputDebugString(L"Password did not match the approval regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_APPROVAL_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the approval regular expression");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexReject(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	std::wstring regex = reg.GetRegValue(L"RegexReject", L"");

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression rejection match");

		std::wregex e(regex);

		if (std::regex_match(password, e))
		{
			OutputDebugString(L"Password matched the rejection regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_REJECTION_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password not did match the rejection regular expression");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordComplexityThreshold(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int threshold = reg.GetRegValue(L"ComplexityThreshold", 0);

	if (threshold > 0)
	{
		OutputDebugString(L"Checking for complexity threshold");
		bool hasLower = false;
		bool hasUpper = false;
		bool hasSymbol = false;
		bool hasNumber = false;
		
		for (size_t i = 0; i < wcslen(password); i++)
		{
			WCHAR c = password[i];

			if (std::iswdigit(c))
			{
				hasNumber = true;
				continue;
			}

			if (std::iswupper(c))
			{
				hasUpper = true;
				continue;
			}

			if (std::iswlower(c))
			{
				hasLower = true;
				continue;
			}

			hasSymbol = true;
		}

		if (wcslen(password) < threshold)
		{
			bool requiresLower = (reg.GetRegValue(L"BelowThresholdRequiresLower", 0) != 0);
			bool requiresUpper = (reg.GetRegValue(L"BelowThresholdRequiresUpper", 0) != 0);
			bool requiresNumber = (reg.GetRegValue(L"BelowThresholdRequiresNumber", 0) != 0);
			bool requiresSymbol = (reg.GetRegValue(L"BelowThresholdRequiresSymbol", 0) != 0);
			bool requiresSymbolOrNumber = (reg.GetRegValue(L"BelowThresholdRequiresSymbolOrNumber", 0) != 0);

			int charSetsRequired = reg.GetRegValue(L"BelowThresholdCharSetsRequired", 0);
			int charSetsPresent = (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasSymbol ? 1 : 0) + (hasNumber ? 0 : 1);

			if ((charSetsPresent < charSetsRequired) || (requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
			{
				OutputDebugString(L"Password did not meet the below-threshold complexity requirements");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BELOW_THRESHOLD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
				return FALSE;
			}

			OutputDebugString(L"Password met the below threshold complexity requirements");
		}
		else
		{
			bool requiresLower = (reg.GetRegValue(L"AboveThresholdRequiresLower", 0) != 0);
			bool requiresUpper = (reg.GetRegValue(L"AboveThresholdRequiresUpper", 0) != 0);
			bool requiresNumber = (reg.GetRegValue(L"AboveThresholdRequiresNumber", 0) != 0);
			bool requiresSymbol = (reg.GetRegValue(L"AboveThresholdRequiresSymbol", 0) != 0);
			bool requiresSymbolOrNumber = (reg.GetRegValue(L"AboveThresholdRequiresSymbolOrNumber", 0) != 0);

			int charSetsRequired = reg.GetRegValue(L"AboveThresholdCharSetsRequired", 0);
			int charSetsPresent = (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasSymbol ? 1 : 0) + (hasNumber ? 0 : 1);

			if ((charSetsPresent < charSetsRequired) || (requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
			{
				OutputDebugString(L"Password did not meet the above-threshold complexity requirements");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ABOVE_THRESHOLD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
				return FALSE;
			}

			OutputDebugString(L"Password met the above threshold complexity requirements");
		}
	}

	return TRUE;
}

BOOLEAN ProcessPasswordComplexityPoints(const LPWSTR &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int requiredPoints = reg.GetRegValue(L"ComplexityPointsRequired", 0);

	if (requiredPoints > 0)
	{
		OutputDebugString(L"Checking for complexity points");

		int perChar = reg.GetRegValue(L"ComplexityPointsPerCharacter", 2);

		int perNumber = reg.GetRegValue(L"ComplexityPointsPerNumber", 0);
		int perSymbol = reg.GetRegValue(L"ComplexityPointsPerSymbol", 0);
		int perUpper = reg.GetRegValue(L"ComplexityPointsPerUpper", 0);
		int perLower = reg.GetRegValue(L"ComplexityPointsPerLower", 0);

		int useNumber = reg.GetRegValue(L"ComplexityPointsUseOfNumber", 1);
		int useSymbol = reg.GetRegValue(L"ComplexityPointsUseOfSymbol", 1);
		int useUpper = reg.GetRegValue(L"ComplexityPointsUseOfUpper", 1);
		int useLower = reg.GetRegValue(L"ComplexityPointsUseOfLower", 1);

		bool hasLower = false;
		bool hasUpper = false;
		bool hasSymbol = false;
		bool hasNumber = false;

		int pointCount = 0;

		for (size_t i = 0; i < wcslen(password); i++)
		{
			WCHAR c = password[i];
			pointCount += perChar;

			if (std::iswdigit(c))
			{
				hasNumber = true;
				pointCount += perNumber;
				continue;
			}

			if (std::iswupper(c))
			{
				pointCount += perUpper;
				hasUpper = true;
				continue;
			}

			if (std::iswlower(c))
			{
				pointCount += perLower;
				hasLower = true;
				continue;
			}

			pointCount += perSymbol;
			hasSymbol = true;
		}

		if (hasLower)
		{
			pointCount += useLower;
		}

		if (hasUpper)
		{
			pointCount += useUpper;
		}

		if (hasSymbol)
		{
			pointCount += useSymbol;
		}

		if (hasNumber)
		{
			pointCount += useNumber;
		}

		if (pointCount < requiredPoints)
		{
			OutputDebugString(L"Password did not meet the required number of points");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_NOT_ENOUGH_POINTS, 5, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(pointCount).c_str(), std::to_wstring(requiredPoints).c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the required number of points");
	}

	return TRUE;
}
