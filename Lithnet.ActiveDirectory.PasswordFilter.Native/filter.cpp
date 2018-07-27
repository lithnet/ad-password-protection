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
	return 0;
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
		odprintf(L"Account name: %s", accountName.c_str());
		odprintf(L"Full name: %s", fullName.c_str());

		if (GetRegValueDWORD(L"CheckRawPassword", 1) != 0)
		{
			OutputDebugString(L"Checking raw password");

			std::wstring hash;
			make_hash(password.c_str(), &hash);

			if (checkHash(hash))
			{
				OutputDebugString(L"Raw password matched existing hash");
				return FALSE;
			}

			OutputDebugString(L"Raw password did not match any existing hashes");
		}

		if (GetRegValueDWORD(L"CheckNormalizedPassword", 1) != 0)
		{
			OutputDebugString(L"Checking normalized password");

			std::wstring normalizedPassword;
			normalize(password, &normalizedPassword);

			std::wstring normalizedHash;
			make_hash(normalizedPassword.c_str(), &normalizedHash);

			if (checkHash(normalizedHash))
			{
				OutputDebugString(L"Normalized password matched existing hash");
				return FALSE;
			}

			OutputDebugString(L"Normalized password did not match any existing hashes");
		}

		OutputDebugString(L"Password validated");
	}
	catch (const std::invalid_argument&)
	{
		OutputDebugString(L"Unexpected Error");
		if (GetRegValueDWORD(L"AllowPasswordOnError", 1) == 0)
		{
			OutputDebugString(L"AllowPasswordOnError is zero, denying password change request");
			return FALSE;
		}
	}

	OutputDebugString(L"*******PasswordFilter Exit");

	return TRUE;
}

