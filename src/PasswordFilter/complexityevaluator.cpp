#include "stdafx.h"
#include "complexityevaluator.h"
#include "SecureArrayT.h"
#include "registry.h"
#include <cwctype>
#include "eventlog.h"
#include "messages.h"

BOOLEAN ProcessPasswordComplexityThreshold(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	const int threshold1 = reg.GetRegValue(REG_VALUE_CT1, 0);
	const int threshold2 = reg.GetRegValue(REG_VALUE_CT2, 0);

	if (threshold1 <= 0)
	{
		return true;
	}

	OutputDebugString(L"Checking for complexity based on length");
	bool hasLower = false;
	bool hasUpper = false;
	bool hasSymbol = false;
	bool hasNumber = false;
	const size_t pwdlength = wcslen(password);

	for (size_t i = 0; i < pwdlength; i++)
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

	std::wstring thresholdID;
	if (pwdlength < threshold1 || threshold2 <= 0)
	{
		thresholdID = REG_VALUE_CT1;
	}
	else if (pwdlength < threshold2)
	{
		thresholdID = REG_VALUE_CT2;
	}
	else
	{
		thresholdID = REG_VALUE_CT3;
	}

	const bool requiresLower = (reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESLOWER, 0) != 0);
	const bool requiresUpper = (reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESUPPER, 0) != 0);
	const bool requiresNumber = (reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESNUMBER, 0) != 0);
	const bool requiresSymbol = (reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESSYMBOL, 0) != 0);
	const bool requiresSymbolOrNumber = (reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESSYMBOLORNUMBER, 0) != 0);

	const int charSetsRequired = min(reg.GetRegValue(thresholdID + REG_VALUE_CTCHARSETSREQUIRED, 0), 4);
	const int charSetsPresent = (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasSymbol ? 1 : 0) + (hasNumber ? 1 : 0);

	if ((charSetsPresent < charSetsRequired) || (requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
	{
		OutputDebugString(std::wstring(L"Password did not meet the " + thresholdID + L" complexity requirements").c_str());
		eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY, 4, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(pwdlength).c_str());
		return FALSE;
	}

	OutputDebugString(std::wstring(L"Password met the " + thresholdID + L" complexity requirements").c_str());

	return true;
}


BOOLEAN ProcessPasswordComplexityPoints(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int requiredPoints = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 0);

	if (requiredPoints > 0)
	{
		OutputDebugString(L"Checking for complexity points");

		int perChar = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 2);

		int perNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
		int perSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
		int perUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);
		int perLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);

		int useNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 1);
		int useSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 1);
		int useUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 1);
		int useLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 1);

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
