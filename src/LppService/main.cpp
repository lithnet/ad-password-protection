#include "pch.h"
#include <tchar.h>
#include <strsafe.h>
#include "eventlog.h"
#include "../PasswordFilter/messages.h"
#include "LppService.h"

int __cdecl wmain(__in long argc, __in_ecount(argc) TCHAR* argv[])
{
	try
	{
		if (argc > 1)
		{
			if (lstrcmpi(argv[1], TEXT("-foreground")) == 0)
			{
				InitializeService(argc, argv);
				WaitForExit();
				return 0;
			}
		}

		SERVICE_TABLE_ENTRY DispatchTable[] =
		{
			{ LPWSTR(serviceName), LPSERVICE_MAIN_FUNCTION(ServiceMain) }
		};

		if (!StartServiceCtrlDispatcher(DispatchTable))
		{
			throw std::system_error(GetLastError(), std::system_category(), "StartServiceCtrlDispatcher failed");
		}

		return 0;
	}
	catch (std::system_error const& e)
	{
		OutputDebugString(L"Win32 error caught");
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_WIN32_RPC_INIT_ERROR, 2, std::to_string(e.code().value()).c_str(), e.what());
		return e.code().value();
	}
	catch (std::exception const& e)
	{
		OutputDebugString(L"Other error caught");
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_OTHER_RPC_INIT_ERROR, 1, e.what());
	}
	catch (...)
	{
		OutputDebugString(L"Unexpected error caught");
		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_OTHER_RPC_INIT_ERROR, 1, L"No exception information was available");
	}

	return -1;
}
