#include "stdafx.h"
#include "eventlog.h"
#include <vector>
#include <fstream>
#include <Shlwapi.h>
#include "messages.h"

HANDLE eventlog::hlog;

eventlog::eventlog()
= default;

eventlog::~eventlog()
{
	if (hlog)
	{
		DeregisterEventSource(hlog);
	}
}

void eventlog::init()
{
	try {
		int count = 0;

		while (getHandle() == NULL)
		{
			if (count > 30)
			{
				break;
			}

			Sleep(1000);
			count++;
		}

		if (hlog != NULL)
		{
			ReportEvent(hlog, EVENTLOG_INFORMATION_TYPE, 0, MSG_AGENT_INITIALIZED, NULL, 0, 0, NULL, NULL);
		}
	}
	catch (std::exception const& e)
	{
		std::string message = "Event log initialization error\r\n";
		message += e.what();
		OutputDebugStringA(message.c_str());
	}
	catch (...)
	{
		std::string message = "Event log initialization error\r\n";
		message += "No exception information was available";
		OutputDebugStringA(message.c_str());
	}
}

void eventlog::writeToFileLog(const std::string& message)
{
	std::ofstream outfile;
	const LPCWSTR sourcepath = L"%windir%\\logs\\lpp.log";

	const int bufSize = ExpandEnvironmentStrings(sourcepath, NULL, 0);

	if (bufSize == 0)
	{
		// error
		return;
	}

	std::vector<wchar_t> buf(bufSize);
	const int result = ExpandEnvironmentStrings(sourcepath, &buf[0], bufSize);

	if (bufSize == 0 || result != bufSize)
	{
		// error
		return;
	}

	outfile.open(buf.data(), std::ios_base::app);
	outfile << message;
	outfile.close();
}

void eventlog::writeToFileLog(const std::wstring& message)
{
	std::wofstream outfile;
	const LPCWSTR sourcepath = L"%windir%\\logs\\lpp.log";

	const int bufSize = ExpandEnvironmentStrings(sourcepath, NULL, 0);

	if (bufSize == 0)
	{
		// error
		return;
	}

	std::vector<wchar_t> buf(bufSize);
	const int result = ExpandEnvironmentStrings(sourcepath, &buf[0], bufSize);

	if (bufSize == 0 || result != bufSize)
	{
		// error
		return;
	}

	outfile.open(buf.data(), std::ios_base::app);
	outfile << message;
	outfile.close();
}

HANDLE eventlog::getHandle()
{
	if (eventlog::hlog == NULL)
	{
		eventlog::hlog = RegisterEventSource(NULL, L"LithnetPasswordProtection");

		if (eventlog::hlog == NULL)
		{
			const std::wstring message = L"Could not register event source LithnetPasswordProtection:" + std::to_wstring(GetLastError()) + L"\r\n";
			writeToFileLog(message);
		}
	}

	return eventlog::hlog;
}

void eventlog::logw(const WORD &severity, const DWORD &eventID, const int argCount, ...)
{
	auto* pInsertStrings = new LPCWSTR[argCount];

	va_list(arglist);
	va_start(arglist, argCount);

	for (int i = 0; i < argCount; i++)
	{
		pInsertStrings[i] = va_arg(arglist, LPCWSTR);
	}

	va_end(arglist);

	const HANDLE hevent = getHandle();

	if (hevent)
	{
		ReportEvent(hevent, severity, 0, eventID, NULL, argCount, 0, pInsertStrings, NULL);
	}

	delete[] pInsertStrings;
}

void eventlog::log(const WORD &severity, const DWORD &eventID, const int argCount, ...)
{
	auto* pInsertStrings = new LPCSTR[argCount];

	va_list(arglist);
	va_start(arglist, argCount);

	for (int i = 0; i < argCount; i++)
	{
		pInsertStrings[i] = va_arg(arglist, LPCSTR);
	}

	va_end(arglist);

	const HANDLE hevent = getHandle();

	if (hevent)
	{
		ReportEventA(hevent, severity, 0, eventID, NULL, argCount, 0, pInsertStrings, NULL);
	}

	delete[] pInsertStrings;
}
