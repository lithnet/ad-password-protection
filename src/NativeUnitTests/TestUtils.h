#pragma once
#include <stdio.h>

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

void DisplayError(DWORD Err);

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

int ClearStandbyCache();

void ClearCache();

void SetValue(std::wstring key, DWORD value);

void SetValue(std::wstring key, std::wstring value);

void DeleteValue(std::wstring key);
