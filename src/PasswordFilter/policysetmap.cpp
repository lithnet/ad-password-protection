#include "stdafx.h"
#include "policysetmap.h"

PolicySetMap::PolicySetMap(const PSID &sid, const std::wstring &groupName, const std::wstring &policyName)
{
	this->Sid = sid;
	this->PolicySetName = policyName;
	this->GroupName = groupName;
}

PolicySetMap::~PolicySetMap()
{
	if (Sid)
	{
		LocalFree(Sid);
		Sid = 0;
	}
}

PolicySetMap::PolicySetMap(PolicySetMap&& other) noexcept
{
	this->Sid = other.Sid;
	other.Sid = 0;
	this->GroupName = other.GroupName;
	this->PolicySetName = other.PolicySetName;
}