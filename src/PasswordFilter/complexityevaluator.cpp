#include "stdafx.h"
#include "complexityevaluator.h"
#include "SecureArrayT.h"
#include "registry.h"
#include <cwctype>
#include  "eventlog.h"
#include "messages.h"

BOOLEAN ProcessPasswordComplexityThreshold(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
{
	int threshold1 = reg.GetRegValue(L"ComplexityThreshold1", 0);
	int threshold2 = reg.GetRegValue(L"ComplexityThreshold2", 0);
	int threshold3 = reg.GetRegValue(L"ComplexityThreshold3", 0);

	if (threshold1 <= 0)
	{
		return true;
	}

	OutputDebugString(L"Checking for complexity based on length");
	bool hasLower = false;
	bool hasUpper = false;
	bool hasSymbol = false;
	bool hasNumber = false;
	size_t pwdlength = wcslen(password);

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
		thresholdID = L"Threshold1";
	}
	else if (pwdlength < threshold2 || threshold3 <= 0)
	{
		thresholdID = L"Threshold2";
	}
	else
	{
		thresholdID = L"Threshold3";
	}

	bool requiresLower = (reg.GetRegValue(thresholdID + L"RequiresLower", 0) != 0);
	bool requiresUpper = (reg.GetRegValue(thresholdID + L"RequiresUpper", 0) != 0);
	bool requiresNumber = (reg.GetRegValue(thresholdID + L"RequiresNumber", 0) != 0);
	bool requiresSymbol = (reg.GetRegValue(thresholdID + L"RequiresSymbol", 0) != 0);
	bool requiresSymbolOrNumber = (reg.GetRegValue(thresholdID + L"RequiresSymbolOrNumber", 0) != 0);

	int charSetsRequired = min(reg.GetRegValue(thresholdID + L"CharSetsRequired", 0), 4);
	int charSetsPresent = (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasSymbol ? 1 : 0) + (hasNumber ? 1 : 0);

	if ((charSetsPresent < charSetsRequired) || (requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
	{
		OutputDebugString(std::wstring(L"Password did not meet the " + thresholdID + L" complexity requirements").c_str());
		eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY, 4, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), pwdlength);
		return FALSE;
	}

	OutputDebugString(std::wstring(L"Password met the " + thresholdID + L" complexity requirements").c_str());

	return true;
}


BOOLEAN ProcessPasswordComplexityPoints(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const registry &reg)
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
