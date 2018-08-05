#include "stdafx.h"
#include "TestUtils.h"

HKEY OpenSettingsKeyWritable()
{
	HKEY hKey;
	LSTATUS result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Lithnet\\PasswordFilter", 0, NULL, NULL, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKey, NULL);

	if (result == ERROR_FILE_NOT_FOUND)
	{
		return 0;
	}

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(result, std::system_category(), "Could not open key");
	}

	return hKey;
}

void SetValue(std::wstring key, DWORD value)
{
	HKEY hkey = OpenSettingsKeyWritable();

	LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void SetValue(std::wstring key, std::wstring value)
{
	HKEY hkey = OpenSettingsKeyWritable();

	LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), (value.size() + 1) * sizeof(wchar_t));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void DeleteValue(std::wstring key)
{
	HKEY hkey = OpenSettingsKeyWritable();

	LSTATUS result = RegDeleteValue(hkey, key.c_str());

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not delete value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

