#include "stdafx.h"
#include "eventlog.h"
#include "messages.h"
#include <vector>

HANDLE hlog;

eventlog::eventlog()
{
	hlog = RegisterEventSource(NULL, L"LithnetPasswordFilter");
}

eventlog::~eventlog()
{
	if (hlog)
	{
		DeregisterEventSource(hlog);
	}
}

void eventlog::logw(const WORD &severity, const DWORD &eventID, const int argCount, ...)
{
	LPCWSTR* pInsertStrings = new LPCWSTR[argCount];

	va_list(arglist);
	va_start(arglist, argCount);
	
	for (int i = 0; i < argCount; i++)
	{
		pInsertStrings[i] = va_arg(arglist, LPCWSTR);
	}

	va_end(arglist);

	if (hlog)
	{
		ReportEvent(hlog, severity, 0, eventID, NULL, argCount, 0, pInsertStrings, NULL);
	}

	delete[] pInsertStrings;
}


void eventlog::log(const WORD &severity, const DWORD &eventID, const int argCount, ...)
{
	LPCSTR* pInsertStrings = new LPCSTR[argCount];

	va_list(arglist);
	va_start(arglist, argCount);

	for (int i = 0; i < argCount; i++)
	{
		pInsertStrings[i] = va_arg(arglist, LPCSTR);
	}

	va_end(arglist);

	if (hlog)
	{
		ReportEventA(hlog, severity, 0, eventID, NULL, argCount, 0, pInsertStrings, NULL);
	}

	delete[] pInsertStrings;
}
