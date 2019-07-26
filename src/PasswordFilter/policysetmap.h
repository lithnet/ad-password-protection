#pragma once

class PolicySetMap
{
public:
	PSID Sid;
	std::wstring PolicySetName;
	std::wstring GroupName;

	PolicySetMap(const PSID &sid, const std::wstring &groupName, const std::wstring &policyName);
	~PolicySetMap();
	PolicySetMap(PolicySetMap&& other) noexcept;
	PolicySetMap(const PolicySetMap& other) = delete;
	PolicySetMap& operator=(const PolicySetMap& other) = delete;
	PolicySetMap& operator=(const PolicySetMap&& other) = delete;
};

