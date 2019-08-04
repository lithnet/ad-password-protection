#pragma once

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

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

int ClearStandbyCache();

void ClearCache();

void SetUnitTestPolicyValue(std::wstring key, DWORD value);
void SetUnitTestPolicyValue(std::wstring policySetName, std::wstring key, DWORD value);


void SetUnitTestPolicyValue(std::wstring key, std::wstring value);
void SetUnitTestPolicyValue(std::wstring policySetName, std::wstring key, std::wstring value);

void DeleteUnitTestPolicyValue(std::wstring key);
void DeleteUnitTestPolicyValue(std::wstring policySetName, std::wstring key);
void DeleteUnitTestPolicyKey(const std::wstring policySetName, const std::wstring key);
UNICODE_STRING GetUnicodeString(std::wstring &str);

std::wstring GetGuid();

HKEY OpenSettingsKeyWritable(std::wstring policySetName);
