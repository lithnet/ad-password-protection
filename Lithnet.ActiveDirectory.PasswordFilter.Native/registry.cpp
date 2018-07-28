#include "stdafx.h"
#include "registry.h"
#include "settings.h"
DWORD GetPolicyOrSettingsValue(HKEY hKeyPolicy, HKEY hKeySettings, const std::wstring &strValueName, DWORD defaultValue)
{
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD value(0);
	LONG result;

	if (hKeyPolicy)
	{
		result = RegQueryValueEx(hKeyPolicy, strValueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value), &dwBufferSize);

		if (result == ERROR_SUCCESS)
		{
			return value;
		}
	}
	
	if (hKeySettings)
	{
		result = RegQueryValueEx(hKeySettings, strValueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value), &dwBufferSize);

		if (result == ERROR_SUCCESS)
		{
			return value;
		}
	}
	
	return defaultValue;
}

const std::wstring GetPolicyOrSettingsValue(HKEY hKeyPolicy, HKEY hKeySettings, const std::wstring &strValueName, const std::wstring &defaultValue)
{
	DWORD dwBufferSize;
	LONG result;

	if (hKeyPolicy)
	{
		result = RegQueryValueEx(hKeyPolicy, strValueName.c_str(), NULL, NULL, NULL, &dwBufferSize);

		if (result == ERROR_SUCCESS)
		{
			return GetValueString(dwBufferSize, hKeyPolicy, strValueName, defaultValue);
		}
	}

	if (hKeySettings)
	{
		result = RegQueryValueEx(hKeySettings, strValueName.c_str(), NULL, NULL, NULL, &dwBufferSize);

		if (result == ERROR_SUCCESS)
		{
			return GetValueString(dwBufferSize, hKeySettings, strValueName, defaultValue);
		}
	}

	return defaultValue;
}

const std::wstring GetValueString(DWORD &dwBufferSize, const HKEY &hKeyPolicy, const std::wstring & strValueName, const std::wstring & defaultValue)
{
	wchar_t* szBuffer = new wchar_t[dwBufferSize];

	LONG result = RegQueryValueEx(hKeyPolicy, strValueName.c_str(), NULL, NULL, (LPBYTE)szBuffer, &dwBufferSize);

	if (result != ERROR_SUCCESS)
	{
		delete[] szBuffer;
		return defaultValue;
	}

	std::wstring value = std::wstring(szBuffer);
	delete[] szBuffer;
	return value;
}

std::wstring GetRegValue(const std::wstring &valueName, const std::wstring &defaultValue)
{
	HKEY hKeySettings = OpenSettingsKey();
	HKEY hKeyPolicy = OpenPolicyKey();
	
	std::wstring value = GetPolicyOrSettingsValue(hKeyPolicy, hKeySettings, valueName, defaultValue);

	if (hKeyPolicy)
	{
		RegCloseKey(hKeyPolicy);
	}

	if (hKeySettings)
	{
		RegCloseKey(hKeySettings);
	}

	return value;
}

DWORD GetRegValue(const std::wstring &valueName, DWORD defaultValue)
{
	HKEY hKeySettings = OpenSettingsKey();
	HKEY hKeyPolicy = OpenPolicyKey();

	DWORD value = GetPolicyOrSettingsValue(hKeyPolicy, hKeySettings, valueName, defaultValue);

	if (hKeyPolicy)
	{
		RegCloseKey(hKeyPolicy);
	}

	if (hKeySettings)
	{
		RegCloseKey(hKeySettings);
	}

	return value;
}

HKEY OpenSettingsKey()
{
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lithnet\\PasswordFilter", 0, KEY_READ, &hKey);

	if (result == ERROR_FILE_NOT_FOUND)
	{
		return 0;
	}

	if (result != ERROR_SUCCESS)
	{
		return 0;
	}

	return hKey;
}

HKEY OpenPolicyKey()
{
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Lithnet\\PasswordFilter", 0, KEY_READ, &hKey);

	if (result == ERROR_FILE_NOT_FOUND)
	{
		return 0;
	}

	if (result != ERROR_SUCCESS)
	{
		return 0;
	}

	return hKey;
}