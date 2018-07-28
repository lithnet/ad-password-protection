#include "stdafx.h"


DWORD GetDWORDRegKey(HKEY hKey, const std::wstring &strValueName, DWORD nDefaultValue)
{
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD value(0);

	LONG result = RegQueryValueEx(hKey, strValueName.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value), &dwBufferSize);

	if (result != ERROR_SUCCESS)
	{
		return nDefaultValue;
	}

	return value;
}

const std::wstring GetStringRegKey(HKEY hKey, const std::wstring &strValueName, const std::wstring &strDefaultValue)
{
	std::wstring strValue = strDefaultValue;

	DWORD dwBufferSize;

	DWORD dwRet = RegQueryValueEx(hKey, strValueName.c_str(), NULL, NULL, NULL, &dwBufferSize);

	if (dwRet != ERROR_SUCCESS)
	{
		return strDefaultValue;
	}

	wchar_t* szBuffer = new wchar_t[dwBufferSize];

	dwRet = RegQueryValueEx(hKey, strValueName.c_str(), NULL, NULL, (LPBYTE)szBuffer, &dwBufferSize);

	if (dwRet != ERROR_SUCCESS)
	{
		delete[] szBuffer;
		return strDefaultValue;
	}

	std::wstring result = std::wstring(szBuffer);
	delete[] szBuffer;
	return result;
}

std::wstring GetRegValueString(const std::wstring &valueName, const std::wstring &defaultValue)
{
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lithnet\\PasswordFilter", 0, KEY_READ, &hKey);

	if (result == ERROR_FILE_NOT_FOUND)
	{
		return defaultValue;
	}

	if (result != ERROR_SUCCESS)
	{
		OutputDebugString(L"Registry key open failed");
		return defaultValue;
	}

	std::wstring value = GetStringRegKey(hKey, valueName, defaultValue);

	if (hKey)
	{
		RegCloseKey(hKey);
	}

	return value;
}

DWORD GetRegValueDWORD(const std::wstring &valueName, DWORD defaultValue)
{
	HKEY hKey;
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lithnet\\PasswordFilter", 0, KEY_READ, &hKey);

	if (result == ERROR_FILE_NOT_FOUND)
	{
		return defaultValue;
	}

	if (result != ERROR_SUCCESS)
	{
		OutputDebugString(L"Registry key open failed");
		return defaultValue;
	}

	DWORD value = GetDWORDRegKey(hKey, valueName, defaultValue);

	if (hKey)
	{
		RegCloseKey(hKey);
	}

	return value;
}