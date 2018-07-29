#include "stdafx.h"
#include "hasher.h"
#include "filter.h"
#include "utils.h"
#include "stringnormalization.h"
#include "hashevaluator.h"
#include "registry.h"
#include "eventlog.h"
#include "messages.h"

extern "C" __declspec(dllexport)  BOOLEAN __stdcall InitializeChangeNotify(void)
{
	return TRUE;
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
	try {
		if (GetRegValue(L"Disabled", 0) != 0)
		{
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_AGENT_DISABLED, 0);
			return TRUE;
		}

		std::wstring password(Password->Buffer, Password->Length / sizeof(WCHAR));
		std::wstring accountName(AccountName->Buffer, AccountName->Length / sizeof(WCHAR));
		std::wstring fullName(FullName->Buffer, FullName->Length / sizeof(WCHAR));
		
		eventlog::getInstance().logw(EVENTLOG_INFORMATION_TYPE, MSG_PROCESSING_REQUEST, 3, SetOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

		if ((SetOperation && GetRegValue(L"ValidateRawPasswordOnSet", 1) != 0) || (!SetOperation && GetRegValue(L"ValidateRawPasswordOnChange", 1) != 0))
		{
			std::wstring hash = Sha1Hash(password.c_str());
			std::wstring message = std::wstring(L"Checking raw password in range ");
			message.append(hash.substr(0, 5));
			OutputDebugString(message.c_str());

			if (IsHashInStore(hash))
			{
				OutputDebugString(L"Rejected password as it was found in the banned store");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED, 4, SetOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), L"The password was found in the banned password store");
				return FALSE;
			}

			OutputDebugString(L"Raw password did not match any existing hashes");
		}

		if ((SetOperation && GetRegValue(L"ValidateNormalizedPasswordOnSet", 1) != 0) || (!SetOperation && GetRegValue(L"ValidateNormalizedPasswordOnChange", 1) != 0))
		{
			std::wstring normalizedPassword = NormalizePassword(password);
			std::wstring normalizedHash = Sha1Hash(normalizedPassword.c_str());
			std::wstring message = std::wstring(L"Checking normalized password in range ");
			message.append(normalizedHash.substr(0, 5));
			OutputDebugString(message.c_str());

			if (IsHashInStore(normalizedHash))
			{
				OutputDebugString(L"Rejected normalized password as it was found in the banned store");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED, 4, SetOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), L"The normalized password was found in the banned password store");
				return FALSE;
			}

			OutputDebugString(L"Normalized password did not match any existing hashes");
		}

		eventlog::getInstance().logw(EVENTLOG_SUCCESS, MSG_PASSWORD_APPROVED, 3, SetOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
	}
	catch (std::exception const& e)
	{
		eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, e.what());

		if ((SetOperation && GetRegValue(L"AllowPasswordSetOnError", 1) == 0) || (!SetOperation && GetRegValue(L"AllowPasswordChangeOnError", 1) == 0))
		{
			OutputDebugString(L"Rejected password because AllowPasswordSetOnError or AllowPasswordChangeOnError was non-zero");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_ERROR, 1, SetOperation ? L"set" : L"change");
			return FALSE;
		}
	}
	catch (...)
	{
		eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_UNEXPECTEDERROR, 1, L"No exception information was available");

		if ((SetOperation && GetRegValue(L"AllowPasswordSetOnError", 1) == 0) || (!SetOperation && GetRegValue(L"AllowPasswordChangeOnError", 1) == 0))
		{
			OutputDebugString(L"Rejected password because AllowPasswordSetOnError or AllowPasswordChangeOnError was non-zero");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_ERROR, 1, SetOperation ? L"set" : L"change");
			return FALSE;
		}
	}

	OutputDebugString(L"Approved password");
	return TRUE;
}