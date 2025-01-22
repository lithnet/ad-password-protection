#include "stdafx.h"
#include "registry.h"
#include <memory>
#include <vector>
#include "eventlog.h"
#include "messages.h"

registry::registry()
{
	this->settingsKeyName = REG_BASE_SETTINGS_KEY;
	this->policyKeyName = REG_BASE_POLICY_KEY;
}

registry::registry(std::wstring policyGroup)
{
	this->policyGroup = policyGroup;
	this->settingsKeyName = GetKeyName(REG_BASE_SETTINGS_KEY);
	this->policyKeyName = GetKeyName(REG_BASE_POLICY_KEY);
}

registry::~registry()
= default;

std::wstring registry::GetRegValue(const std::wstring& valueName, const std::wstring& defaultValue) const
{
	return GetPolicyOrSettingsValue(valueName, defaultValue);
}

std::vector<std::wstring> registry::GetRegValue(const std::wstring& valueName, DWORD maxItems, const std::vector<std::wstring>& defaultValue) const
{
	return GetPolicyOrSettingsValue(maxItems, valueName, defaultValue);
}

DWORD registry::GetRegValue(const std::wstring& valueName, DWORD defaultValue) const
{
	return GetPolicyOrSettingsValue(valueName, defaultValue);
}

registry registry::GetRegistryForUser(const std::wstring& user)
{
	return registry(L"default");
}

DWORD registry::GetPolicyOrSettingsValue(const std::wstring& valueName, DWORD defaultValue) const
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

const std::wstring registry::GetKeyName(LPCWSTR& key) const
{
	std::wstring name(key);
	if (!this->policyGroup.empty())
	{
		name += L"\\";
		name += this->policyGroup;
	}

	return name;
}

const std::wstring registry::GetPolicyOrSettingsValue(const std::wstring& valueName, const std::wstring& defaultValue) const
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

const std::vector<std::wstring> registry::GetPolicyOrSettingsValue(DWORD maxItems, const std::wstring& valueName, const std::vector<std::wstring>& defaultValue) const
{
	DWORD dwBufferSize = 0;

	long result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->policyKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_MULTI_SZ,
		NULL,
		NULL,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return GetValueMultiString(dwBufferSize, maxItems, this->policyKeyName, valueName, defaultValue);
	}

	result = RegGetValue(HKEY_LOCAL_MACHINE,
		this->settingsKeyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_MULTI_SZ,
		NULL,
		NULL,
		&dwBufferSize);

	if (result == ERROR_SUCCESS)
	{
		return GetValueMultiString(dwBufferSize, maxItems, this->settingsKeyName, valueName, defaultValue);
	}

	return defaultValue;
}

const std::vector<std::wstring> registry::GetValueMultiString(DWORD& dwBufferSize, DWORD maxItems, const std::wstring& keyName, const std::wstring& valueName, const std::vector<std::wstring>& defaultValue) const
{
	std::vector<std::wstring> result;

	std::vector<wchar_t> data;
	data.resize(dwBufferSize / sizeof(wchar_t));

	DWORD lResult = RegGetValue(HKEY_LOCAL_MACHINE,
		keyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_MULTI_SZ,
		NULL,
		&data[0],
		&dwBufferSize
	);

	if (lResult != ERROR_SUCCESS)
	{
        eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_REG_READ_ERROR, 4, std::to_wstring(lResult).c_str(), keyName.c_str(), valueName.c_str(), L"REG_MULTI_SZ");
		return defaultValue;
	}

	data.resize(dwBufferSize / sizeof(wchar_t));

	std::wstring current;
	for (DWORD i = 0; i < dwBufferSize / sizeof(WCHAR); i++)
	{
		if (data[i] == L'\0')
		{
			if (!current.empty())
			{
				if (result.size() >= maxItems)
				{
					break;
				}

				result.push_back(current);
				current.clear();
			}
		}
		else
		{
			current += data[i];
		}
	}

	return result;
}

const std::wstring registry::GetValueString(DWORD& dwBufferSize, const std::wstring& keyName, const std::wstring& valueName, const std::wstring& defaultValue) const
{
	std::unique_ptr<WCHAR[]> szBuffer = std::make_unique<WCHAR[]>(dwBufferSize / sizeof(WCHAR));

	long result = RegGetValue(HKEY_LOCAL_MACHINE,
		keyName.c_str(),
		valueName.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		&szBuffer[0],
		&dwBufferSize);

	if (result != ERROR_SUCCESS)
	{
		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_REG_READ_ERROR, 4, std::to_wstring(result).c_str(), keyName.c_str(), valueName.c_str(), L"REG_SZ");
		return defaultValue;
	}

	return std::wstring(szBuffer.get());
}
