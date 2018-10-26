#pragma once
#include "policy.h"

extern "C" __declspec(dllexport)  BOOLEAN __stdcall InitializeChangeNotify(void);

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(
	PUNICODE_STRING AccountName,
	PUNICODE_STRING FullName,
	PUNICODE_STRING Password,
	BOOLEAN SetOperation);

extern "C" __declspec(dllexport) NTSTATUS __stdcall PasswordChangeNotify(
	PUNICODE_STRING UserName,
	ULONG RelativeId,
	PUNICODE_STRING NewPassword);

extern "C" __declspec(dllexport) int __stdcall PasswordFilterEx(
	LPCWSTR AccountName,
	LPCWSTR FullName,
	LPCWSTR Password,
	BOOLEAN SetOperation);

extern "C" __declspec(dllexport) void __stdcall GetUserPolicySettings(
	LPCWSTR AccountName, user_policy* policy);