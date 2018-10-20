#pragma once
#include "authz.h"

class groups
{
public:
	groups();
	~groups();
};

HRESULT GetSid(LPCWSTR wszAccName, PSID * ppSid);
BOOL GetGroupsFromContext(AUTHZ_CLIENT_CONTEXT_HANDLE hClientContext);
DWORD GetUserContext(PSID pSid, AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext);