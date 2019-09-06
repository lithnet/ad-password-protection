#include "pch.h"
#include <tchar.h>
#include <strsafe.h>
#include "rpc_server.h"
#include "LppService.h"

rpc_server rpc;
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceControlHandler;


void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	serviceControlHandler = RegisterServiceCtrlHandler(serviceName, ServiceControlHandler);

	if (!serviceControlHandler)
	{
		throw std::system_error(GetLastError(), std::system_category(), "RegisterServiceCtrlHandler failed");
	}

	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	// Report initial status to the SCM
	ReportStatusToServiceManager(SERVICE_START_PENDING, NO_ERROR, 3000);

	// Perform service-specific initialization and work.
	InitializeService(dwArgc, lpszArgv);

	ReportStatusToServiceManager(SERVICE_RUNNING, NO_ERROR, 0);
}

void InitializeService(DWORD dwArgc, LPTSTR *lpszArgv)
{
	rpc.InitializeRpcServer();
	rpc.Start(false);
}

void WaitForExit()
{
	rpc.WaitForEnd();
}

void StopListening()
{
	rpc.Stop();
}

void ReportStatusToServiceManager(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;

	serviceStatus.dwCurrentState = dwCurrentState;
	serviceStatus.dwWin32ExitCode = dwWin32ExitCode;
	serviceStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING)
	{
		serviceStatus.dwControlsAccepted = 0;
	}
	else
	{
		serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}

	if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
	{
		serviceStatus.dwCheckPoint = 0;
	}
	else
	{
		serviceStatus.dwCheckPoint = dwCheckPoint++;
	}

	if (!SetServiceStatus(serviceControlHandler, &serviceStatus))
	{
		throw std::system_error(GetLastError(), std::system_category(), "SetServiceStatus failed");
	}
}

void WINAPI ServiceControlHandler(DWORD dwCtrl)
{
	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP:
		ReportStatusToServiceManager(SERVICE_STOP_PENDING, NO_ERROR, 0);
		StopListening();
		ReportStatusToServiceManager(SERVICE_STOPPED, NO_ERROR, 0);
		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}
}