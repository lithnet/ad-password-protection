#include "stdafx.h"
#include "filter.h"
#include "utils.h"
#include "registry.h"
#include "eventlog.h"
#include "messages.h"
#include "passwordevaluator.h"
#include "SecureArrayT.h"
#include <thread>

extern "C" __declspec(dllexport)  BOOLEAN __stdcall InitializeChangeNotify(void)
{
	try {
		std::thread t1(eventlog::init);
		t1.detach();
		return TRUE;
	}
	catch (std::exception const& e)
	{
		OutputDebugString(L"Other error caught");
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, e.what());
	}
	catch (...)
	{
		OutputDebugString(L"Unexpected error caught");
		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, L"No exception information was available");
	}

	return FALSE;
}

extern "C" __declspec(dllexport) NTSTATUS __stdcall PasswordChangeNotify(
	PUNICODE_STRING UserName,
	ULONG RelativeId,
	PUNICODE_STRING NewPassword
)
{
	return 0L;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(
	PUNICODE_STRING AccountName,
	PUNICODE_STRING FullName,
	PUNICODE_STRING Password,
	BOOLEAN SetOperation
)
{
	bool simulate = false;
	registry reg;

	try {

		if (reg.GetRegValue(REG_VALUE_DISABLED, 0) != 0)
		{
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_AGENT_DISABLED, 0);
			return TRUE;
		}

		if (Password->Length <= 0)
		{
			return FALSE;
		}

		simulate = reg.GetRegValue(REG_VALUE_AUDITONLY, 0) != 0;

		std::wstring accountName(AccountName->Buffer, AccountName->Length / sizeof(WCHAR));

		if (!IsUserInScope(accountName))
		{
			return TRUE;
		}

		std::wstring fullName(FullName->Buffer, FullName->Length / sizeof(WCHAR));

		SecureArrayT<WCHAR> password = UnicodeStringToWcharArray(*Password);

		int result = ProcessPassword(password, accountName, fullName, SetOperation);

		if (simulate)
		{
			return TRUE;
		}
		else
		{
			return result == 0;
		}
	}
	catch (std::system_error const& e)
	{
		OutputDebugString(L"Win32 error caught");

		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_WIN32ERROR, 2, std::to_string(e.code().value()).c_str(), e.what());

		if ((SetOperation && reg.GetRegValue(L"AllowPasswordSetOnError", 1) == 0) || (!SetOperation && reg.GetRegValue(L"AllowPasswordChangeOnError", 1) == 0))
		{
			OutputDebugString(L"Rejected password because AllowPasswordSetOnError or AllowPasswordChangeOnError was non-zero");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_ERROR, 1, SetOperation ? L"set" : L"change");
			return simulate ? TRUE : FALSE;
		}
	}
	catch (std::exception const& e)
	{
		OutputDebugString(L"Other error caught");

		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, e.what());

		if ((SetOperation && reg.GetRegValue(L"AllowPasswordSetOnError", 1) == 0) || (!SetOperation && reg.GetRegValue(L"AllowPasswordChangeOnError", 1) == 0))
		{
			OutputDebugString(L"Rejected password because AllowPasswordSetOnError or AllowPasswordChangeOnError was non-zero");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_ERROR, 1, SetOperation ? L"set" : L"change");
			return simulate ? TRUE : FALSE;
		}
	}
	catch (...)
	{
		OutputDebugString(L"Unexpected error caught");

		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, L"No exception information was available");

		if ((SetOperation && reg.GetRegValue(L"AllowPasswordSetOnError", 1) == 0) || (!SetOperation && reg.GetRegValue(L"AllowPasswordChangeOnError", 1) == 0))
		{
			OutputDebugString(L"Rejected password because AllowPasswordSetOnError or AllowPasswordChangeOnError was non-zero");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_ERROR, 1, SetOperation ? L"set" : L"change");
			return simulate ? TRUE : FALSE;
		}
	}

	OutputDebugString(L"Approved password");
	return TRUE;
}

extern "C" __declspec(dllexport) int __stdcall PasswordFilterEx(
	LPCWSTR AccountName,
	LPCWSTR FullName,
	LPCWSTR Password,
	BOOLEAN SetOperation
)
{
	registry reg;

	try {

		if (reg.GetRegValue(REG_VALUE_DISABLED, 0) != 0)
		{
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_AGENT_DISABLED, 0);
			return PASSWORD_APPROVED;
		}

		if (wcslen(Password) <= 0)
		{
			return PASSWORD_REJECTED_BLANK;
		}

		std::wstring accountName = AccountName;

		if (!IsUserInScope(accountName))
		{
			return PASSWORD_APPROVED;
		}

		std::wstring fullName = FullName;
		SecureArrayT<WCHAR> password = StringToWcharArray(Password);

		return ProcessPassword(password, accountName, fullName, SetOperation);
	}
	catch (std::system_error const& e)
	{
		OutputDebugString(L"Win32 error caught");
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_WIN32ERROR, 2, std::to_string(e.code().value()).c_str(), e.what());
	}
	catch (std::exception const& e)
	{
		OutputDebugString(L"Other error caught");
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, e.what());
	}
	catch (...)
	{
		OutputDebugString(L"Unexpected error caught");
		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, L"No exception information was available");
	}

	return FILTER_ERROR;
}

extern "C" __declspec(dllexport) void __stdcall GetUserPolicySettings(
	LPCWSTR AccountName, user_policy* policy)
{
	*policy = policy::getpolicy(AccountName);
}
