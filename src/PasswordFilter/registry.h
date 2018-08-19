#pragma once
#include "stdafx.h"

static const LPWSTR BASE_SETTINGS_KEY_NAME = L"SOFTWARE\\Lithnet\\PasswordFilter";
static const LPWSTR BASE_POLICY_KEY_NAME = L"SOFTWARE\\Policies\\Lithnet\\PasswordFilter";

class registry
{
private:
	std::wstring policyGroup;
	std::wstring settingsKeyName;
	std::wstring policyKeyName;

public:
	registry();
	registry(std::wstring policyGroup);
	~registry();
	std::wstring GetRegValue(const std::wstring & valueName, const std::wstring & defaultValue) const;
	DWORD GetRegValue(const std::wstring & valueName, DWORD defaultValue) const;
	static registry GetRegistryForUser(const std::wstring & user);

private:
	DWORD GetPolicyOrSettingsValue(const std::wstring & strValueName, DWORD defaultValue) const;
	const std::wstring GetKeyName(const LPWSTR& key) const;
	const std::wstring GetPolicyOrSettingsValue(const std::wstring & strValueName, const std::wstring & defaultValue) const;
	const std::wstring GetValueString(DWORD & dwBufferSize, const std::wstring & keyName, const std::wstring & strValueName, const std::wstring & defaultValue) const;
};


