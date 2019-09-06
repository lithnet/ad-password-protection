#pragma once

static constexpr const wchar_t* serviceName = L"lppservice";
void WaitForExit();
void WINAPI ServiceMain(DWORD, LPTSTR *);
void InitializeService(DWORD, LPTSTR *);
void WINAPI ServiceControlHandler(DWORD);
void ReportStatusToServiceManager(DWORD, DWORD, DWORD);
void StopListening();
