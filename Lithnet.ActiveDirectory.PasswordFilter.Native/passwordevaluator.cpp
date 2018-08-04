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

BOOLEAN ProcessPassword(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	eventlog::getInstance().logw(EVENTLOG_INFORMATION_TYPE, MSG_PROCESSING_REQUEST, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	if (!ProcessPasswordLength(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordComplexityThreshold(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordComplexityPoints(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordRegexApprove(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordRegexReject(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordRaw(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	if (!ProcessPasswordNormalized(password, accountName, fullName, setOperation))
	{
		return FALSE;
	}

	OutputDebugString(L"Password was approved by all modules");

	eventlog::getInstance().logw(EVENTLOG_SUCCESS, MSG_PASSWORD_APPROVED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	return TRUE;
}

BOOLEAN ProcessPasswordRaw(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	if ((setOperation && GetRegValue(L"ValidateRawPasswordOnSet", 1) != 0) || (!setOperation && GetRegValue(L"ValidateRawPasswordOnChange", 1) != 0))
	{
		std::wstring message = std::wstring(L"Checking raw password");
		OutputDebugString(message.c_str());

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

BOOLEAN ProcessPasswordNormalized(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	if ((setOperation && GetRegValue(L"ValidateNormalizedPasswordOnSet", 1) != 0) || (!setOperation && GetRegValue(L"ValidateNormalizedPasswordOnChange", 1) != 0))
	{
		std::wstring normalizedPassword = NormalizePassword(password);
		std::wstring normalizedHash = GetSha1HashString(normalizedPassword.c_str());
		std::wstring message = std::wstring(L"Checking normalized password in range ");
		message.append(normalizedHash.substr(0, 5));
		OutputDebugString(message.c_str());

		if (IsHashInStore(normalizedHash))
		{
			OutputDebugString(L"Rejected normalized password as it was found in the banned store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Normalized password did not match any existing hashes");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordLength(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	int minLength = GetRegValue(L"MinimumLength", 0);

	if (minLength > 0)
	{
		OutputDebugString(L"Checking minimum length");

		if (password.length() < minLength)
		{
			OutputDebugString(L"Rejected password as it did not meet the minimum length requirements");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_MINLENGTH, 5, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(password.length()).c_str(), std::to_wstring(minLength).c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the minimum length requirements");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexApprove(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	std::wstring regex = GetRegValue(L"RegexApprove", L"");

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

BOOLEAN ProcessPasswordRegexReject(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	std::wstring regex = GetRegValue(L"RegexReject", L"");

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

BOOLEAN ProcessPasswordComplexityThreshold(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	int threshold = GetRegValue(L"ComplexityThreshold", 0);

	if (threshold > 0)
	{
		OutputDebugString(L"Checking for complexity threshold");
		bool hasLower = false;
		bool hasUpper = false;
		bool hasSymbol = false;
		bool hasNumber = false;

		for (wchar_t& c : password) {

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

		if (password.length() < threshold)
		{
			bool requiresLower = (GetRegValue(L"BelowThresholdRequiresLower", 0) != 0);
			bool requiresUpper = (GetRegValue(L"BelowThresholdRequiresUpper", 0) != 0);
			bool requiresNumber = (GetRegValue(L"BelowThresholdRequiresNumber", 0) != 0);
			bool requiresSymbol = (GetRegValue(L"BelowThresholdRequiresSymbol", 0) != 0);
			bool requiresSymbolOrNumber = (GetRegValue(L"BelowThresholdRequiresSymbolOrNumber", 0) != 0);

			if ((requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
			{
				OutputDebugString(L"Password did not meet the below-threshold complexity requirements");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BELOW_THRESHOLD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
				return FALSE;
			}

			OutputDebugString(L"Password met the below threshold complexity requirements");

		}
		else
		{
			bool requiresLower = (GetRegValue(L"AboveThresholdRequiresLower", 0) != 0);
			bool requiresUpper = (GetRegValue(L"AboveThresholdRequiresUpper", 0) != 0);
			bool requiresNumber = (GetRegValue(L"AboveThresholdRequiresNumber", 0) != 0);
			bool requiresSymbol = (GetRegValue(L"AboveThresholdRequiresSymbol", 0) != 0);
			bool requiresSymbolOrNumber = (GetRegValue(L"AboveThresholdRequiresSymbolOrNumber", 0) != 0);

			if ((requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
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

BOOLEAN ProcessPasswordComplexityPoints(std::wstring password, std::wstring accountName, std::wstring fullName, BOOLEAN setOperation)
{
	int requiredPoints = GetRegValue(L"ComplexityPointsRequired", 0);

	if (requiredPoints > 0)
	{
		OutputDebugString(L"Checking for complexity points");

		int perChar = GetRegValue(L"ComplexityPointsPerCharacter", 2);

		int perNumber = GetRegValue(L"ComplexityPointsPerNumber", 0);
		int perSymbol = GetRegValue(L"ComplexityPointsPerSymbol", 0);
		int perUpper = GetRegValue(L"ComplexityPointsPerUpper", 0);
		int perLower = GetRegValue(L"ComplexityPointsPerLower", 0);

		int useNumber = GetRegValue(L"ComplexityPointsUseOfNumber", 1);
		int useSymbol = GetRegValue(L"ComplexityPointsUseOfSymbol", 1);
		int useUpper = GetRegValue(L"ComplexityPointsUseOfUpper", 1);
		int useLower = GetRegValue(L"ComplexityPointsUseOfLower", 1);

		bool hasLower = false;
		bool hasUpper = false;
		bool hasSymbol = false;
		bool hasNumber = false;

		int pointCount = 0;

		for (wchar_t& c : password) {
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
