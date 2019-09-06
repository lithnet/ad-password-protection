#include "pch.h"
#include "registry.h"
#include "sddl.h"

registry::registry()
{
	this->settingsKeyName = REG_BASE_SETTINGS_KEY;
	this->policyKeyName = REG_BASE_POLICY_KEY;
}

registry::registry(std::wstring policySetName)
{
	this->policySetName = policySetName;
	this->settingsKeyName = GetKeyName(REG_BASE_SETTINGS_KEY);
	this->policyKeyName = GetKeyName(REG_BASE_POLICY_KEY);
}

std::wstring registry::GetRegValue(const std::wstring & valueName, const std::wstring & defaultValue) const
{
	return GetPolicyOrSettingsValue(valueName, defaultValue);
}

DWORD registry::GetRegValue(const std::wstring & valueName, DWORD defaultValue) const
{
	return GetPolicyOrSettingsValue(valueName, defaultValue);
}

DWORD registry::GetPolicyOrSettingsValue(const std::wstring & valueName, DWORD defaultValue) const
{
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD value(0);

	long result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->policyKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_DWORD,
		NULL,
		&value,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return value;
	}

	result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->settingsKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_DWORD,
		NULL,
		&value,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return value;
	}

	return defaultValue;
}

std::wstring registry::GetKeyName(LPCWSTR & key) const
{
	std::wstring name(key);
	if (!this->policySetName.empty())
	{
		name += L"\\";
		name += this->policySetName;
	}

	return name;
}

std::wstring registry::GetPolicyOrSettingsValue(const std::wstring & valueName, const std::wstring & defaultValue) const
{
	DWORD dwBufferSize = 0;

	long result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->policyKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		NULL,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return GetValueString(dwBufferSize, this->policyKeyName, valueName, defaultValue);
	}

	result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->settingsKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		NULL,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return GetValueString(dwBufferSize, this->settingsKeyName, valueName, defaultValue);
	}

	return defaultValue;
}

std::wstring registry::GetValueString(DWORD & dwBufferSize, const std::wstring & keyName, const std::wstring & valueName, const std::wstring & defaultValue) const
{
	std::unique_ptr<WCHAR[]> szBuffer = std::make_unique<WCHAR[]>(dwBufferSize / sizeof(WCHAR));

	const long result = RegGetValue(HKEY_LOCAL_MACHINE,
		keyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		&szBuffer[0],
		&dwBufferSize);

	if (result != ERROR_SUCCESS)
	{
		return defaultValue;
	}

	return std::wstring(szBuffer.get());
}
