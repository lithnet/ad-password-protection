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

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
		return FALSE;

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		return FALSE;

	return TRUE;
}

int ClearStandbyCache()
{
	//return 0;
	HMODULE ntdll = LoadLibrary(L"ntdll.dll");
	if (!ntdll)
	{
		fprintf(stderr, "Can't load ntdll.dll\n");
		return -1;
	}

	NTSTATUS(WINAPI *NtSetSystemInformation)(INT, PVOID, ULONG) = (NTSTATUS(WINAPI *)(INT, PVOID, ULONG))GetProcAddress(ntdll, "NtSetSystemInformation");
	if (!NtSetSystemInformation)
	{
		fprintf(stderr, "Can't get NtSetSystemInformation address\n");
		return -1;
	}

	HANDLE processToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken) == FALSE)
		return 3;

	if (SetPrivilege(processToken, L"SeIncreaseQuotaPrivilege", TRUE))
	{
		SYSTEM_FILECACHE_INFORMATION info;
		ZeroMemory(&info, sizeof(info));
		info.MinimumWorkingSet = -1;
		info.MaximumWorkingSet = -1;
		NTSTATUS ret = NtSetSystemInformation((SYSTEM_INFORMATION_CLASS)21, &info, sizeof(info));
		if (ret >= 0) {
			printf("Flush FileCache WorkingSet : ok\n");
		}
		else {
			return -1;
		}
	}
	else {
		printf("Failure to set required privileges\n");
		return -1;
	}

	if (SetPrivilege(processToken, L"SeProfileSingleProcessPrivilege", TRUE))
	{
		SYSTEM_MEMORY_LIST_COMMAND command = (SYSTEM_MEMORY_LIST_COMMAND)4;
		NTSTATUS ret = NtSetSystemInformation(80, &command, sizeof(command));
		if (ret >= 0) {
			printf("Purge Memory Standby : ok\n");
		}
		else {
			return -1;
		}
	}
	else {
		printf("Failure to set required privileges\n");
		return -1;
	}

	return 0;
}

void ClearCache()
{
	if (ClearStandbyCache() != 0)
	{
		throw std::exception("Cache clear failed");
	}
	else
	{
		OutputDebugString(L"Cleared OS file cache");
	}
}
