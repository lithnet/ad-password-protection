#include "stdafx.h"
#include "TestUtils.h"
#include "../PasswordFilter/registry.h"

UNICODE_STRING GetUnicodeString(std::wstring &str)
{
	UNICODE_STRING us;
	us.Buffer = (wchar_t *)str.c_str();
	us.Length = str.size() * sizeof(wchar_t);
	us.MaximumLength = us.Length + 2;

	return us;
}

HKEY OpenGlobalSettingsKeyWritable()
{
	std::wstring key = REG_BASE_POLICY_KEY;

	HKEY hKey;
	const LSTATUS result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, key.c_str(), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKey, NULL);

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

HKEY OpenUnitTestSettingsKeyWritable(std::wstring policySetName)
{
	std::wstring key = REG_BASE_POLICY_KEY;
	key += L"\\UnitTests";
	
	if (!policySetName.empty())
	{
		key += L"\\" + policySetName;
	}

	HKEY hKey;
	const LSTATUS result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, key.c_str(), 0, NULL, NULL, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKey, NULL);

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

void SetUnitTestPolicyValue(std::wstring policySetName, std::wstring key, DWORD value)
{
	const HKEY hkey = OpenUnitTestSettingsKeyWritable(policySetName);

	const LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}


void SetGlobalPolicyValue(std::wstring key, DWORD value)
{
	const HKEY hkey = OpenGlobalSettingsKeyWritable();

	const LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}


void SetGlobalPolicyValue(const std::wstring key, const std::wstring value)
{
	const HKEY hkey = OpenGlobalSettingsKeyWritable();

	const LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), (value.size() + 1) * sizeof(wchar_t));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void SetUnitTestPolicyValue(std::wstring key, DWORD value)
{
	SetUnitTestPolicyValue(L"Default", key, value);
}

void SetUnitTestPolicyValue(const std::wstring key, const std::wstring value)
{
	SetUnitTestPolicyValue(L"Default", key, value);
}

void SetUnitTestPolicyValue(std::wstring policySetName, const std::wstring key, const std::wstring value)
{
	const HKEY hkey = OpenUnitTestSettingsKeyWritable(policySetName);

	const LSTATUS result = RegSetValueEx(hkey, key.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), (value.size() + 1) * sizeof(wchar_t));

	if (result != ERROR_SUCCESS)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not set value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void DeleteUnitTestPolicyValue(const std::wstring key)
{
	DeleteUnitTestPolicyValue(L"Default", key);
}

void DeleteUnitTestPolicyValue(const std::wstring policySetName, const std::wstring key)
{
	const HKEY hkey = OpenUnitTestSettingsKeyWritable(policySetName);

	const LSTATUS result = RegDeleteValue(hkey, key.c_str());

	if (result != ERROR_SUCCESS && result != 2)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not delete value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void DeleteGlobalPolicyValue(const std::wstring key)
{
	const HKEY hkey = OpenGlobalSettingsKeyWritable();

	const LSTATUS result = RegDeleteValue(hkey, key.c_str());

	if (result != ERROR_SUCCESS && result != 2)
	{
		throw std::system_error(GetLastError(), std::system_category(), "Could not delete value");
	}

	if (hkey)
	{
		RegCloseKey(hkey);
	}
}

void DeleteUnitTestPolicyKey(const std::wstring policySetName, const std::wstring key)
{
	const HKEY hkey = OpenUnitTestSettingsKeyWritable(policySetName);

	const LSTATUS result = RegDeleteKey(hkey, key.c_str());

	if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND)
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
	const HMODULE ntdll = LoadLibrary(L"ntdll.dll");
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
		const NTSTATUS ret = NtSetSystemInformation(80, &command, sizeof(command));
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

std::wstring GetGuid()
{
	GUID gidReference;
	HRESULT result = CoCreateGuid(&gidReference);

	if (result != S_OK)
	{
		throw std::system_error(result, std::system_category(), "CoCreateGuid failed");
	}

	RPC_WSTR uuidString = NULL;

	result = UuidToString(&gidReference, &uuidString);

	if (result != RPC_S_OK)
	{
		throw std::system_error(result, std::system_category(), "CoCreateGuid failed");
	}

	std::wstring value = (wchar_t*)uuidString;

	RpcStringFree(&uuidString);

	return value;
}
