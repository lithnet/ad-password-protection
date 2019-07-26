#include "stdafx.h"
#include "policy.h"
#include "registry.h"
#include "utils.h"
#include <AuthZ.h>

PSID policy::ConvertNameToBinarySid(const std::wstring &pAccountName)
{
	LPTSTR szDomainName = NULL;
	DWORD cbDomainName = 0;
	PSID pSid = NULL;
	DWORD cbSid = 0;
	SID_NAME_USE sidType;

	while (!LookupAccountName(NULL, pAccountName.c_str(), pSid, &cbSid, szDomainName, &cbDomainName, &sidType))
	{
		const DWORD result = GetLastError();

		if (result != ERROR_INSUFFICIENT_BUFFER)
		{
			throw std::system_error(result, std::system_category(), "LookupAccountName failed");
		}

		pSid = LocalAlloc(LPTR, cbSid);
		szDomainName = static_cast<LPTSTR>(LocalAlloc(LPTR, cbDomainName * sizeof(TCHAR)));

		if (szDomainName == NULL || pSid == NULL)
		{
			throw std::system_error(GetLastError(), std::system_category(), "Out of memory");
		}
	}

	if (szDomainName != NULL)
	{
		LocalFree(szDomainName);
	}

	return pSid;
}

std::wstring policy::GetPolicyNameForUser(const std::wstring &accountName)
{
	std::wstring effectivePolicyName = L"Default";

	auto policySids = registry::GetActivePolicyGroupSids();

	if (policySids.empty())
	{
		return effectivePolicyName;
	}

	AUTHZ_RESOURCE_MANAGER_HANDLE hResourceManager = 0;
	LUID unusedId = { 0 };
	AUTHZ_CLIENT_CONTEXT_HANDLE hCtx = NULL;
	DWORD dwTokenGroupsSize = 0;
	PVOID pvTokenGroupsBuf = NULL;
	PSID pUserSid = NULL;

	try
	{
		if (!AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT, NULL, NULL, NULL, NULL, &hResourceManager))
		{
			throw std::system_error(GetLastError(), std::system_category(), "AuthzInitializeResourceManager failed");
		}

		pUserSid = ConvertNameToBinarySid(accountName);

		if (!AuthzInitializeContextFromSid(0, pUserSid, hResourceManager, NULL, unusedId, NULL, &hCtx))
		{
			throw std::system_error(GetLastError(), std::system_category(), "AuthzInitializeContextFromSid failed");
		}

		while (!AuthzGetInformationFromContext(hCtx, AuthzContextInfoGroupsSids, dwTokenGroupsSize, &dwTokenGroupsSize, pvTokenGroupsBuf))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				throw std::system_error(GetLastError(), std::system_category(), "AuthzGetInformationFromContext failed");
			}

			if (pvTokenGroupsBuf)
			{
				free(pvTokenGroupsBuf);
			}

			pvTokenGroupsBuf = malloc(dwTokenGroupsSize);
		}

		for (const SidGroupMap &map : policySids)
		{
			bool matched = false;

			for (DWORD i = 0; i < static_cast<PTOKEN_GROUPS>(pvTokenGroupsBuf)->GroupCount; i++)
			{
				if (EqualSid(map.Sid, static_cast<PTOKEN_GROUPS>(pvTokenGroupsBuf)->Groups[i].Sid))
				{
					effectivePolicyName = map.GroupName;
					matched = true;
					break;
				}
			}

			if (matched)
			{
				break;
			}
		}

		if (pvTokenGroupsBuf)
		{
			free(pvTokenGroupsBuf);
		}

		if (hCtx)
		{
			AuthzFreeContext(hCtx);
		}

		if (pUserSid)
		{
			LocalFree(pUserSid);
		}

		if (hResourceManager)
		{
			AuthzFreeResourceManager(hResourceManager);
		}

		return effectivePolicyName;
	}
	catch (...)
	{
		if (pvTokenGroupsBuf)
		{
			free(pvTokenGroupsBuf);
		}

		if (hCtx)
		{
			AuthzFreeContext(hCtx);
		}

		if (pUserSid)
		{
			LocalFree(pUserSid);
		}

		if (hResourceManager)
		{
			AuthzFreeResourceManager(hResourceManager);
		}

		throw;
	}
}

user_policy policy::GetPolicyForUser(const std::wstring &accountName)
{
	const std::wstring policyName = policy::GetPolicyNameForUser(accountName);

	return GetPolicyForGroup(policyName);
}

user_policy policy::GetPolicyForGroup(const std::wstring &groupName)
{
	registry reg = registry::GetRegistryForGroup(groupName);

	user_policy policy{};

	policy.ComplexityPoints.PerCharacter = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
	policy.ComplexityPoints.PerLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
	policy.ComplexityPoints.PerNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
	policy.ComplexityPoints.PerSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
	policy.ComplexityPoints.PerUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);
	policy.ComplexityPoints.Required = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 0);
	policy.ComplexityPoints.UseOfLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
	policy.ComplexityPoints.UseOfNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
	policy.ComplexityPoints.UseOfSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
	policy.ComplexityPoints.UseOfUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

	policy.ComplexityThreshold1.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT1, 0);
	policy.ComplexityThreshold1.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT1CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold1.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT1REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT1REQUIRESUPPER, 0) != 0;

	policy.ComplexityThreshold2.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT2, 0);
	policy.ComplexityThreshold2.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT2CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold2.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT2REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT2REQUIRESUPPER, 0) != 0;

	policy.ComplexityThreshold3.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT3, 0);
	policy.ComplexityThreshold3.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT3CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold3.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT3REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT3REQUIRESUPPER, 0) != 0;

	policy.GeneralPolicy.MinimumLength = reg.GetRegValue(REG_VALUE_MINIMUMLENGTH, 0);
	policy.GeneralPolicy.ValidatePasswordDoesntContainAccountName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 0) != 0;
	policy.GeneralPolicy.ValidatePasswordDoesntContainFullName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 0) != 0;
	policy.GeneralPolicy.RegexApprove = GetInteropString(reg.GetRegValue(REG_VALUE_REGEXAPPROVE, L"").c_str());
	policy.GeneralPolicy.RegexReject = GetInteropString(reg.GetRegValue(REG_VALUE_REGEXREJECT, L"").c_str());
	
	policy.StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 0) != 0;
	policy.StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnSet = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 0) != 0;

	policy.StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONCHANGE, 0) != 0;
	policy.StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnSet = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 0) != 0;

	policy.StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONCHANGE, 0) != 0;
	policy.StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnSet= reg.GetRegValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 0) != 0;

	return policy;
}

policy::policy()
= default;

policy::~policy()
= default;