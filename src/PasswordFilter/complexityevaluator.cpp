#include "stdafx.h"
#include "complexityevaluator.h"
#include "SecureArrayT.h"
#include "registry.h"
#include <cwctype>
#include  "eventlog.h"
#include "messages.h"
#include "policy.h"

BOOLEAN ProcessPasswordComplexityThreshold(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	const unsigned int threshold1 = pol.ComplexityThreshold1.ComplexityThreshold; // reg.GetRegValue(REG_VALUE_CT1, 0);
	const unsigned int threshold2 = pol.ComplexityThreshold2.ComplexityThreshold; // reg.GetRegValue(REG_VALUE_CT2, 0);

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
		const WCHAR c = password[i];

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

	complexity_threshold threshold{};
	std::wstring thresholdName;

	if (pwdlength < threshold1 || threshold2 <= 0)
	{
		threshold = pol.ComplexityThreshold1;
		thresholdName = L"level 1";
	}
	else if (pwdlength < threshold2)
	{
		threshold = pol.ComplexityThreshold2;
		thresholdName = L"level 2";
	}
	else
	{
		threshold = pol.ComplexityThreshold3;
		thresholdName = L"level 3";
	}

	const bool requiresLower = (threshold.ComplexityThresholdRequiresLower /*reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESLOWER, 0)*/ != 0);
	const bool requiresUpper = (threshold.ComplexityThresholdRequiresUpper /*reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESUPPER, 0)*/ != 0);
	const bool requiresNumber = (threshold.ComplexityThresholdRequiresNumber /*reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESNUMBER, 0)*/ != 0);
	const bool requiresSymbol = (threshold.ComplexityThresholdRequiresSymbol /*reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESSYMBOL, 0)*/ != 0);
	const bool requiresSymbolOrNumber = (threshold.ComplexityThresholdRequiresSymbolOrNumber /*reg.GetRegValue(thresholdID + REG_VALUE_CTREQUIRESSYMBOLORNUMBER, 0)*/ != 0);

	const int charSetsRequired = min(threshold.ComplexityThresholdCharsetsRequired /*reg.GetRegValue(thresholdID + REG_VALUE_CTCHARSETSREQUIRED, 0)*/, 4);
	const int charSetsPresent = (hasLower ? 1 : 0) + (hasUpper ? 1 : 0) + (hasSymbol ? 1 : 0) + (hasNumber ? 1 : 0);

	if ((charSetsPresent < charSetsRequired) || (requiresLower && !hasLower) || (requiresUpper && !hasUpper) || (requiresNumber && !hasNumber) || (requiresSymbol && !hasSymbol) || (requiresSymbolOrNumber && !(hasSymbol || hasNumber)))
	{
		OutputDebugString(std::wstring(L"Password did not meet the " + thresholdName + L" complexity requirements").c_str());
		eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY, 4, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(pwdlength).c_str());
		return FALSE;
	}

	OutputDebugString(std::wstring(L"Password met the " + thresholdName + L" complexity requirements").c_str());

	return true;
}


BOOLEAN ProcessPasswordComplexityPoints(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	const int requiredPoints = pol.ComplexityPoints.Required; //reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 0);

	if (requiredPoints > 0)
	{
		OutputDebugString(L"Checking for complexity points");
		
		bool hasLower = false;
		bool hasUpper = false;
		bool hasSymbol = false;
		bool hasNumber = false;

		int pointCount = 0;

		for (size_t i = 0; i < wcslen(password); i++)
		{
			const WCHAR c = password[i];
			pointCount += pol.ComplexityPoints.PerCharacter;

			if (std::iswdigit(c))
			{
				hasNumber = true;
				pointCount += pol.ComplexityPoints.PerNumber;
				continue;
			}

			if (std::iswupper(c))
			{
				pointCount += pol.ComplexityPoints.PerUpper;
				hasUpper = true;
				continue;
			}

			if (std::iswlower(c))
			{
				pointCount += pol.ComplexityPoints.PerLower;
				hasLower = true;
				continue;
			}

			pointCount += pol.ComplexityPoints.PerSymbol;
			hasSymbol = true;
		}

		if (hasLower)
		{
			pointCount += pol.ComplexityPoints.UseOfLower;
		}

		if (hasUpper)
		{
			pointCount += pol.ComplexityPoints.UseOfUpper;
		}

		if (hasSymbol)
		{
			pointCount += pol.ComplexityPoints.UseOfSymbol;
		}

		if (hasNumber)
		{
			pointCount += pol.ComplexityPoints.UseOfNumber;
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
