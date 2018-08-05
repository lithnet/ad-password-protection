#pragma once
//#include <windows.h>
#include <stdio.h>


//typedef enum _SYSTEM_INFORMATION_CLASS
//{
//	SystemFileCacheInformation = 21,
//	SystemMemoryListInformation = 80
//};

typedef enum _SYSTEM_MEMORY_LIST_COMMAND
{
	MemoryCaptureAccessedBits,
	MemoryCaptureAndResetAccessedBits,
	MemoryEmptyWorkingSets,
	MemoryFlushModifiedList,
	MemoryPurgeStandbyList,
	MemoryPurgeLowPriorityStandbyList,
	MemoryCommandMax
} SYSTEM_MEMORY_LIST_COMMAND;

typedef struct _SYSTEM_FILECACHE_INFORMATION
{
	SIZE_T CurrentSize;
	SIZE_T PeakSize;
	ULONG PageFaultCount;
	SIZE_T MinimumWorkingSet;
	SIZE_T MaximumWorkingSet;
	SIZE_T CurrentSizeIncludingTransitionInPages;
	SIZE_T PeakSizeIncludingTransitionInPages;
	ULONG TransitionRePurposeCount;
	ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION;

void DisplayError(DWORD Err)
{
	LPVOID lpMessageBuffer;
	HMODULE Hand = LoadLibrary(L"NTDLL.DLL");

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_FROM_HMODULE,
		Hand,
		Err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMessageBuffer,
		0,
		NULL);

	// Now display the string.
	printf("%ls\n", lpMessageBuffer);

	// Free the buffer allocated by the system.
	LocalFree(lpMessageBuffer);
	FreeLibrary(Hand);
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
			DisplayError(ret);
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
			DisplayError(ret);
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
