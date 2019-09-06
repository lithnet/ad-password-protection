#pragma once
#include "pch.h"
#include <vector>

static LPCWSTR REG_BASE_SETTINGS_KEY = L"SOFTWARE\\Lithnet\\PasswordFilter";
static LPCWSTR REG_BASE_POLICY_KEY = L"SOFTWARE\\Policies\\Lithnet\\PasswordFilter";

static LPCWSTR REG_VALUE_STOREPATH = L"Store";
static LPCWSTR REG_VALUE_DISABLED = L"Disabled";
static LPCWSTR REG_VALUE_SIMULATE = L"Disabled";

class registry
{
private:
	std::wstring policySetName;
	std::wstring settingsKeyName;
	std::wstring policyKeyName;

public:
	registry();
	registry(std::wstring policySetName);
	std::wstring GetRegValue(const std::wstring & valueName, const std::wstring & defaultValue) const;
	DWORD GetRegValue(const std::wstring & valueName, DWORD defaultValue) const;

private:
	DWORD GetPolicyOrSettingsValue(const std::wstring & valueName, DWORD defaultValue) const;
	std::wstring GetKeyName(LPCWSTR& key) const;
	std::wstring GetPolicyOrSettingsValue(const std::wstring & valueName, const std::wstring & defaultValue) const;
	std::wstring GetValueString(DWORD & dwBufferSize, const std::wstring & keyName, const std::wstring & valueName, const std::wstring & defaultValue) const;
};


