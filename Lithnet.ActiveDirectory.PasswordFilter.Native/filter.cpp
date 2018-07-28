#include "stdafx.h"
#include "hasher.h"
#include "filter.h"
#include "utils.h"
#include "stringnormalization.h"
#include "hashevaluator.h"
#include "registry.h"

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
	OutputDebugString(L"*******PasswordFilter Enter");

	try {
		std::wstring password(Password->Buffer, Password->Length / sizeof(WCHAR));
		std::wstring accountName(AccountName->Buffer, AccountName->Length / sizeof(WCHAR));
		std::wstring fullName(FullName->Buffer, FullName->Length / sizeof(WCHAR));

		odprintf(L"Password: %s", password.c_str());
		odprintf(L"Processing password %s for user %s (%s)", SetOperation ? L"set" : L"changed", accountName.c_str(), fullName.c_str());

		if ((SetOperation && GetRegValue(L"ValidateRawPasswordOnSet", 1) != 0) || (!SetOperation && GetRegValue(L"ValidateRawPasswordOnChange", 1) != 0))
		{
			OutputDebugString(L"Checking raw password");

			std::wstring hash = Sha1Hash(password.c_str());

			if (IsHashInStore(hash))
			{
				OutputDebugString(L"Raw password matched existing hash");
				return FALSE;
			}

			OutputDebugString(L"Raw password did not match any existing hashes");
		}

		if ((SetOperation && GetRegValue(L"ValidateNormalizedPasswordOnSet", 1) != 0) || (!SetOperation && GetRegValue(L"ValidateNormalizedPasswordOnChange", 1) != 0))
		{
			OutputDebugString(L"Checking normalized password");

			std::wstring normalizedPassword = NormalizePassword(password);
			std::wstring normalizedHash = Sha1Hash(normalizedPassword.c_str());

			if (IsHashInStore(normalizedHash))
			{
				OutputDebugString(L"Normalized password matched existing hash");
				return FALSE;
			}

			OutputDebugString(L"Normalized password did not match any existing hashes");
		}

		OutputDebugString(L"Password validated");
	}
	catch (std::exception const& e) 
	{
		OutputDebugString(L"Unexpected Error");
		OutputDebugStringA(e.what());

		if (GetRegValue(L"AllowPasswordOnError", 1) == 0)
		{
			OutputDebugString(L"AllowPasswordOnError is zero, denying password change request");
			return FALSE;
		}
	}
	catch (...)
	{
		OutputDebugString(L"Unexpected Error");
		if (GetRegValue(L"AllowPasswordOnError", 1) == 0)
		{
			OutputDebugString(L"AllowPasswordOnError is zero, denying password change request");
			return FALSE;
		}
	}

	OutputDebugString(L"*******PasswordFilter Exit");

	return TRUE;
}

