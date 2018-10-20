#include "stdafx.h"
#include "groups.h"
#include <sddl.h>

groups::groups()
{
}


groups::~groups()
{
}




DWORD GetUserContext(PSID pSid, AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext)
{
	AUTHZ_RESOURCE_MANAGER_HANDLE hManager = NULL;
	LUID unusedId = { 0 };
	//AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = NULL;
	DWORD dwRtnCode = ERROR_SUCCESS;
	DWORD ret = ERROR_SUCCESS;

	if (!AuthzInitializeResourceManager(AUTHZ_RM_FLAG_NO_AUDIT, NULL, NULL, NULL, NULL, &hManager))
	{
		return GetLastError();
	}

	// Pass AUTHZ_SKIP_TOKEN_GROUPS to the function to avoid querying user group
	// information for access check. This allows us to model POSIX permissions
	// on Windows, where a user can have less permissions than a group it
	// belongs to.
	if (!AuthzInitializeContextFromSid(NULL, pSid, hManager, NULL, unusedId, NULL, &hAuthzClientContext))
	{
		return GetLastError();
	}

	return 0;

}

HRESULT GetSid(LPCWSTR wszAccName, PSID * ppSid)
{

	// Validate the input parameters.  
	if (wszAccName == NULL || ppSid == NULL)
	{
		return ERROR_BAD_ARGUMENTS;
	}

	// Create buffers that may be large enough.  
	// If a buffer is too small, the count parameter will be set to the size needed.  
	const DWORD INITIAL_SIZE = 32;
	DWORD cbSid = 0;
	DWORD dwSidBufferSize = INITIAL_SIZE;
	DWORD cchDomainName = 0;
	DWORD dwDomainBufferSize = INITIAL_SIZE;
	WCHAR * wszDomainName = NULL;
	SID_NAME_USE eSidType;
	DWORD dwErrorCode = 0;
	HRESULT hr = 0;

	// Create buffers for the SID and the domain name.  
	*ppSid = (PSID) new BYTE[dwSidBufferSize];
	if (*ppSid == NULL)
	{
		return ERROR_OUTOFMEMORY;
	}
	memset(*ppSid, 0, dwSidBufferSize);
	wszDomainName = new WCHAR[dwDomainBufferSize];
	if (wszDomainName == NULL)
	{
		return ERROR_OUTOFMEMORY;
	}

	memset(wszDomainName, 0, dwDomainBufferSize * sizeof(WCHAR));

	// Obtain the SID for the account name passed.  
	for (; ; )
	{

		// Set the count variables to the buffer sizes and retrieve the SID.  
		cbSid = dwSidBufferSize;
		cchDomainName = dwDomainBufferSize;
		if (LookupAccountNameW(
			NULL,            // Computer name. NULL for the local computer  
			wszAccName,
			*ppSid,          // Pointer to the SID buffer. Use NULL to get the size needed,  
			&cbSid,          // Size of the SID buffer needed.  
			wszDomainName,   // wszDomainName,  
			&cchDomainName,
			&eSidType
		))
		{
			if (IsValidSid(*ppSid) == FALSE)
			{
				wprintf(L"The SID for %s is invalid.\n", wszAccName);
				dwErrorCode = ERROR_BAD_ARGUMENTS;
			}
			break;
		}
		dwErrorCode = GetLastError();

		// Check if one of the buffers was too small.  
		if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
		{
			if (cbSid > dwSidBufferSize)
			{

				// Reallocate memory for the SID buffer.  
				wprintf(L"The SID buffer was too small. It will be reallocated.\n");
				FreeSid(*ppSid);
				*ppSid = (PSID) new BYTE[cbSid];
				if (*ppSid == NULL)
				{
					return ERROR_OUTOFMEMORY;
				}
				memset(*ppSid, 0, cbSid);
				dwSidBufferSize = cbSid;
			}

			if (cchDomainName > dwDomainBufferSize)
			{

				// Reallocate memory for the domain name buffer.  
				wprintf(L"The domain name buffer was too small. It will be reallocated.\n");
				delete[] wszDomainName;
				wszDomainName = new WCHAR[cchDomainName];
				if (wszDomainName == NULL)
				{
					return ERROR_OUTOFMEMORY;
				}
				memset(wszDomainName, 0, cchDomainName * sizeof(WCHAR));
				dwDomainBufferSize = cchDomainName;
			}
		}
		else
		{
			wprintf(L"LookupAccountNameW failed. GetLastError returned: %d\n", dwErrorCode);
			hr = HRESULT_FROM_WIN32(dwErrorCode);
			break;
		}
	}

	delete[] wszDomainName;
	return hr;
}


BOOL GetGroupsFromContext(AUTHZ_CLIENT_CONTEXT_HANDLE hClientContext)
{

	DWORD                cbSize = 0;
	PTOKEN_GROUPS        pTokenGroups = NULL;
	LPTSTR                StringSid = NULL;
	BOOL                bResult = FALSE;
	int i = 0;

	//Call the AuthzGetInformationFromContext function with a NULL output buffer to get the required buffer size.
	AuthzGetInformationFromContext(hClientContext, AuthzContextInfoGroupsSids, 0, &cbSize, NULL);




	//Allocate the buffer for the TOKEN_GROUPS structure.
	pTokenGroups = (PTOKEN_GROUPS)malloc(cbSize);
	if (!pTokenGroups)
		return FALSE;

	//Get the SIDs of groups associated with the client context. 
	if (!AuthzGetInformationFromContext(hClientContext, AuthzContextInfoGroupsSids, cbSize, &cbSize, pTokenGroups))
	{
		printf_s("AuthzGetInformationFromContext failed with %d\n", GetLastError);
		free(pTokenGroups);
		return FALSE;
	}

	//Enumerate and display the group SIDs.
	for (i = pTokenGroups->GroupCount - 1; i >= 0; --i)
	{
		//Convert a SID to a string.
		if (!ConvertSidToStringSid(
			pTokenGroups->Groups[i].Sid,
			&StringSid
		))
		{
			LocalFree(StringSid);
			return FALSE;
		}


		wprintf_s(L"%s \n", StringSid);

	}

	free(pTokenGroups);

	return TRUE;
}