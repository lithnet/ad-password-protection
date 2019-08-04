#include "stdafx.h"
#include "policy.h"
#include "registry.h"
#include "utils.h"
#include <AuthZ.h>
#include "policysetmap.h"
#include <sddl.h>
#include "eventlog.h"
#include "messages.h"

std::wstring policy::GetPolicySetNameForUser(const std::wstring &accountName)
{
	return policy::GetPolicySetNameForUser(accountName, L"");
}

std::wstring policy::GetPolicySetNameForUser(const std::wstring &accountName, const std::wstring baseKey)
{
	std::wstring effectivePolicySetName = L"Default";

	auto policySetMap = policy::GetActivePolicySetMap(baseKey);

	if (policySetMap.empty())
	{
		eventlog::getInstance().debugw(EVENTLOG_INFORMATION_TYPE, MSG_POLICY_MAPPING_NOT_CONFIGURED, 1, accountName.c_str());
		return effectivePolicySetName;
	}

	AUTHZ_RESOURCE_MANAGER_HANDLE hResourceManager = 0;
	const LUID unusedId = { 0, 0 };
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
		bool matched = false;

		for (const PolicySetMap &map : policySetMap)
		{
			for (DWORD i = 0; i < static_cast<PTOKEN_GROUPS>(pvTokenGroupsBuf)->GroupCount; i++)
			{
				if (EqualSid(map.Sid, static_cast<PTOKEN_GROUPS>(pvTokenGroupsBuf)->Groups[i].Sid))
				{
					eventlog::getInstance().debugw(EVENTLOG_INFORMATION_TYPE, MSG_POLICY_MAPPING_COMPLETE, 3, accountName.c_str(), map.PolicySetName.c_str(), map.GroupName.c_str());
					effectivePolicySetName = map.PolicySetName;
					matched = true;
					break;
				}
			}

			if (matched)
			{
				break;
			}
		}

		if (!matched)
		{
			eventlog::getInstance().debugw(EVENTLOG_INFORMATION_TYPE, MSG_POLICY_MAPPING_DEFAULTED, 1, accountName.c_str());
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

		return effectivePolicySetName;
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
//
//user_policy& policy::GetPolicySetForUser(const std::wstring &accountName)
//{
//	const std::wstring policyName = policy::GetPolicySetNameForUser(accountName);
//	return GetPolicySetByName(policyName);
//}
//
//user_policy& policy::GetPolicySetByName(const std::wstring &policySetName)
//{
//	auto* policy = new user_policy();
//	PopulatePolicySetObject(policySetName, policy);
//	return *policy;
//}

void policy::PopulatePolicySetObject(const std::wstring &policySetName, user_policy *policy)
{
	const registry reg = registry(policySetName);
	PopulatePolicySetObject(reg, policy);
}

void policy::PopulatePolicySetObject(const registry &reg, user_policy *policy)
{
	policy->ComplexityPoints.PerCharacter = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
	policy->ComplexityPoints.PerLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
	policy->ComplexityPoints.PerNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
	policy->ComplexityPoints.PerSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
	policy->ComplexityPoints.PerUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);
	policy->ComplexityPoints.Required = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 0);
	policy->ComplexityPoints.UseOfLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
	policy->ComplexityPoints.UseOfNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
	policy->ComplexityPoints.UseOfSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
	policy->ComplexityPoints.UseOfUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

	policy->ComplexityThreshold1.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT1, 0);
	policy->ComplexityThreshold1.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT1CHARSETSREQUIRED, 0);
	policy->ComplexityThreshold1.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT1REQUIRESLOWER, 0) != 0;
	policy->ComplexityThreshold1.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESNUMBER, 0) != 0;
	policy->ComplexityThreshold1.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOL, 0) != 0;
	policy->ComplexityThreshold1.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy->ComplexityThreshold1.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT1REQUIRESUPPER, 0) != 0;

	policy->ComplexityThreshold2.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT2, 0);
	policy->ComplexityThreshold2.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT2CHARSETSREQUIRED, 0);
	policy->ComplexityThreshold2.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT2REQUIRESLOWER, 0) != 0;
	policy->ComplexityThreshold2.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESNUMBER, 0) != 0;
	policy->ComplexityThreshold2.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOL, 0) != 0;
	policy->ComplexityThreshold2.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy->ComplexityThreshold2.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT2REQUIRESUPPER, 0) != 0;

	policy->ComplexityThreshold3.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT3, 0);
	policy->ComplexityThreshold3.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT3CHARSETSREQUIRED, 0);
	policy->ComplexityThreshold3.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT3REQUIRESLOWER, 0) != 0;
	policy->ComplexityThreshold3.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESNUMBER, 0) != 0;
	policy->ComplexityThreshold3.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOL, 0) != 0;
	policy->ComplexityThreshold3.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy->ComplexityThreshold3.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT3REQUIRESUPPER, 0) != 0;

	policy->GeneralPolicy.MinimumLength = reg.GetRegValue(REG_VALUE_MINIMUMLENGTH, 0);
	policy->GeneralPolicy.ValidatePasswordDoesntContainAccountName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 0) != 0;
	policy->GeneralPolicy.ValidatePasswordDoesntContainFullName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 0) != 0;

	auto rxapprove = reg.GetRegValue(REG_VALUE_REGEXAPPROVE, L"");
	if (rxapprove.empty())
	{
		policy->GeneralPolicy.RegexApprove = 0;
	}
	else
	{
		policy->GeneralPolicy.RegexApprove = new wchar_t[rxapprove.length() + 1]{ 0 };
		rxapprove.copy(policy->GeneralPolicy.RegexApprove, rxapprove.length(), 0);
	}

	auto rxreject = reg.GetRegValue(REG_VALUE_REGEXREJECT, L"");

	if (rxreject.empty())
	{
		policy->GeneralPolicy.RegexReject = 0;
	}
	else
	{
		policy->GeneralPolicy.RegexReject = new wchar_t[rxreject.length() + 1]{ 0 };
		rxreject.copy(policy->GeneralPolicy.RegexReject, rxreject.length(), 0);
	}

	policy->StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 0) != 0;
	policy->StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnSet = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 0) != 0;

	policy->StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONCHANGE, 0) != 0;
	policy->StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnSet = reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 0) != 0;

	policy->StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnChange = reg.GetRegValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONCHANGE, 0) != 0;
	policy->StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnSet = reg.GetRegValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 0) != 0;
}

std::vector<PolicySetMap> policy::GetActivePolicySetMap()
{
	return policy::GetActivePolicySetMap(L"");
}

std::vector<PolicySetMap> policy::GetActivePolicySetMap(const std::wstring baseKey)
{
	std::vector<PolicySetMap> sids;
	registry baseReg;
	const unsigned int maxAllowedPolicies = baseReg.GetRegValue(L"PolicyCount", 10);

	for (size_t i = 0; i < maxAllowedPolicies; i++)
	{
		const std::wstring policySetName = std::to_wstring(i);
		std::wstring policySetKeyName = policySetName;
		if (!baseKey.empty())
		{
			policySetKeyName = baseKey + L"\\" + policySetName;
		}

		try
		{
			registry policyReg = registry(policySetKeyName);

			if (policyReg.GetRegValue(L"Enabled", 0) == 1)
			{
				std::wstring sid = policyReg.GetRegValue(L"GroupSid", L"");
				std::wstring groupName = policyReg.GetRegValue(L"GroupName", L"");

				if (sid.empty() && !groupName.empty())
				{
					PSID sidbytes = ConvertNameToBinarySid(groupName);
					sids.emplace_back(sidbytes, groupName, policySetName);
				}
				else if (!sid.empty())
				{
					PSID sidbytes;
					if (!ConvertStringSidToSid(sid.c_str(), &sidbytes))
					{
						throw std::system_error(GetLastError(), std::system_category(), "ConvertStringSidToSid failed for group");
					}

					sids.emplace_back(sidbytes, groupName, policySetName);
				}
			}
		}
		catch (std::system_error const& e)
		{
			eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_GROUP_MAPPING_ERROR, 3, std::to_string(e.code().value()).c_str(), e.what(), policySetName.c_str());
		}
		catch (std::exception const& e)
		{
			eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_GROUP_MAPPING_ERROR, 3, L"Unknown", e.what(), policySetName.c_str());
		}
		catch (...)
		{
			eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_GROUP_MAPPING_ERROR, 3, L"Unknown", L"No exception information was available", policySetName.c_str());
		}
	}

	return sids;
}
