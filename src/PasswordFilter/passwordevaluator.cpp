#include "stdafx.h"
#include "messages.h"
#include "eventlog.h"
#include "stringnormalization.h"
#include "passwordevaluator.h"
#include <regex>
#include <Shlwapi.h>
#include "utils.h"
#include "complexityevaluator.h"
#include "v3store.h"
#include "hibp.h"
#include "policy.h"

int ProcessPassword(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation)
{
	eventlog::getInstance().logw(EVENTLOG_INFORMATION_TYPE, MSG_PROCESSING_REQUEST, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	const auto policySetName = policy::GetPolicySetNameForUser(accountName);
	user_policy pol;
	policy::PopulatePolicySetObject(policySetName, &pol);

	if (!ProcessPasswordLength(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_LENGTH;
	}

	if (!ProcessPasswordComplexityThreshold(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_COMPLEXITY_THRESHOLD;
	}

	if (!ProcessPasswordComplexityPoints(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_COMPLEXITY_POINTS;
	}

	if (!ProcessPasswordRegexApprove(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_REGEX_APPROVE;
	}

	if (!ProcessPasswordRegexReject(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_REGEX_REJECT;
	}

	if (!ProcessPasswordDoesntContainAccountName(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_CONTAINS_ACCOUNT_NAME;
	}

	if (!ProcessPasswordDoesntContainFullName(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_CONTAINS_FULL_NAME;
	}

	if (!ProcessPasswordRaw(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_BANNED;
	}

	if (!ProcessPasswordNormalizedWordStore(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_BANNED_NORMALIZED_WORD;
	}

	if (!ProcessPasswordNormalizedPasswordStore(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD;
	}

	if (!ProcessPasswordHibp(password, accountName, fullName, setOperation, pol))
	{
		return PASSWORD_REJECTED_HIBP_API;
	}

	OutputDebugString(L"Password was approved by all modules");

	eventlog::getInstance().logw(EVENTLOG_SUCCESS, MSG_PASSWORD_APPROVED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());

	return PASSWORD_APPROVED;
}


BOOLEAN ProcessPasswordHibp(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if ((setOperation && pol.StorePolicy.CheckPasswordNotInHibpOnSet != 0) ||
		(!setOperation && pol.StorePolicy.CheckPasswordNotInHibpOnChange != 0))
	{
		OutputDebugString(L"Checking password in HIBP API");
		registry reg;

		try
		{
			if (IsInHibpSha1Api(password))
			{
				OutputDebugString(L"Rejected password as it was found in the HIBP API");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_HIBP_API, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
				return FALSE;
			}

			OutputDebugString(L"Password was not found in the HIBP API");
		}
		catch (std::system_error const& e)
		{
			OutputDebugString(L"Win32 error caught when trying to read HIBP API");

			eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_WIN32_HIBP_ERROR, 2, std::to_string(e.code().value()).c_str(), e.what());

			if (reg.GetRegValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0) != 0)
			{
				OutputDebugString(L"Rejected password because RejectPasswordOnHibpError was non-zero");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
				return FALSE;
			}
			else
			{
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_PASSED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
			}
		}
		catch (std::exception const& e)
		{
			OutputDebugString(L"Error caught trying to read the HIBP API");

			eventlog::getInstance().log(EVENTLOG_ERROR_TYPE, MSG_OTHER_HIBP_ERROR, 1, e.what());

			if (reg.GetRegValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0) != 0)
			{
				OutputDebugString(L"Rejected password because RejectPasswordOnHibpError was non-zero");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
				return FALSE;
			}
			else
			{
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_PASSED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
			}
		}
		catch (...)
		{
			OutputDebugString(L"Unexpected error caught");

			eventlog::getInstance().logw(EVENTLOG_ERROR_TYPE, MSG_OTHER_HIBP_ERROR, 1, L"No exception information was available");

			if (reg.GetRegValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0) != 0)
			{
				OutputDebugString(L"Rejected password because RejectPasswordOnHibpError was non-zero");
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
				return FALSE;
			}
			else
			{
				eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_PASSED_ON_HIBP_ERROR, 1, setOperation ? L"set" : L"change");
			}
		}
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRaw(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if ((setOperation && pol.StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnSet /*reg.GetRegValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 0)*/ != 0) || 
		(!setOperation && pol.StorePolicy.CheckPasswordNotInCompromisedPasswordStoreOnChange /*reg.GetRegValue(REG_VALUE_CHECKBANNEDPASSWORDONCHANGE, 0)*/ != 0))
	{
		OutputDebugString(L"Checking raw password");

		if (v3store::getInstance().IsPasswordInPasswordStore(password))
		{
			OutputDebugString(L"Rejected password as it was found in the banned store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Raw password did not match any existing hashes");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordNormalizedPasswordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if (setOperation &&  pol.StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnSet /*reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 0)*/ != 0 ||
		!setOperation && pol.StorePolicy.CheckNormalizedPasswordNotInCompromisedPasswordStoreOnChange /*reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONCHANGE, 0)*/ != 0 )
	{
		bool result = TRUE;

		const SecureArrayT<WCHAR> normalizedPassword = NormalizePassword(password);
		OutputDebugString(L"Checking normalized password");

		result = v3store::getInstance().IsPasswordInPasswordStore(normalizedPassword);
		if (result)
		{
			OutputDebugString(L"Rejected normalized password as it was found in the banned password store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
		}
		else
		{
			OutputDebugString(L"Normalized password did not match any existing hashes in the password store");
		}

		return !result;
	}

	return TRUE;
}


BOOLEAN ProcessPasswordNormalizedWordStore(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if (setOperation && pol.StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnSet /*reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 0)*/ != 0 ||
		!setOperation && pol.StorePolicy.CheckNormalizedPasswordNotInBannedWordStoreOnChange /* reg.GetRegValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 0)*/ != 0)
	{
		bool result = TRUE;

		const SecureArrayT<WCHAR> normalizedPassword = NormalizePassword(password);
		OutputDebugString(L"Checking normalized password");

		result = v3store::getInstance().IsPasswordInWordStore(normalizedPassword);
		if (result)
		{
			OutputDebugString(L"Rejected normalized password as it was found in the banned word store");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_WORD, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
		}
		else
		{
			OutputDebugString(L"Normalized password did not match any existing hashes in the word store");
		}

		return !result;
	}

	return TRUE;
}


BOOLEAN ProcessPasswordLength(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	const unsigned int minLength = pol.GeneralPolicy.MinimumLength; // reg.GetRegValue(REG_VALUE_MINIMUMLENGTH, 0);

	if (minLength > 0)
	{
		OutputDebugString(L"Checking minimum length");

		if (wcslen(password) < minLength)
		{
			OutputDebugString(L"Rejected password as it did not meet the minimum length requirements");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_MINLENGTH, 5, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str(), std::to_wstring(wcslen(password)).c_str(), std::to_wstring(minLength).c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the minimum length requirements");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordDoesntContainAccountName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	int flag = pol.GeneralPolicy.ValidatePasswordDoesntContainAccountName; //reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 0);

	if (flag != 0 && accountName.length() > 3)
	{
		OutputDebugString(L"Checking to see if password contains account name");

		for each (std::wstring substring in SplitString(accountName, L' '))
		{
			if (substring.length() > 3)
			{
				if (StrStrI(password, substring.c_str()) != NULL)
				{
					OutputDebugString(L"Rejected password as it contained part of the account name");
					eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_CONTAINS_ACCOUNTNAME, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
					return FALSE;
				}
			}
		}

		OutputDebugString(L"Password did not contain part of the account name");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordDoesntContainFullName(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	int flag = pol.GeneralPolicy.ValidatePasswordDoesntContainFullName; //reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 0);

	if (flag != 0 && fullName.length() > 3)
	{
		OutputDebugString(L"Checking to see if password contains full name");

		for each (std::wstring substring in SplitString(fullName, L' '))
		{
			if (substring.length() > 3)
			{
				if (StrStrI(password, substring.c_str()) != NULL)
				{
					OutputDebugString(L"Rejected password as it contained part of the full name");
					eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_CONTAINS_DISPLAYNAME, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
					return FALSE;
				}
			}
		}

		OutputDebugString(L"Password did not contain any part of the full name");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexApprove(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if (pol.GeneralPolicy.RegexApprove == 0)
	{
		return TRUE;
	}

	std::wstring regex(pol.GeneralPolicy.RegexApprove);

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression approval match");

		const std::wregex e(regex);

		if (!std::regex_match(password.get(), e))
		{
			OutputDebugString(L"Password did not match the approval regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_APPROVAL_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password met the approval regular expression");
	}

	return TRUE;
}

BOOLEAN ProcessPasswordRegexReject(const SecureArrayT<WCHAR> &password, const std::wstring &accountName, const std::wstring &fullName, const BOOLEAN &setOperation, const user_policy &pol)
{
	if (pol.GeneralPolicy.RegexReject == 0)
	{
		return TRUE;
	}

	std::wstring regex(pol.GeneralPolicy.RegexReject);

	if (regex.length() > 0)
	{
		OutputDebugString(L"Checking for regular expression rejection match");

		const std::wregex e(regex);

		if (std::regex_match(password.get(), e))
		{
			OutputDebugString(L"Password matched the rejection regular expression");
			eventlog::getInstance().logw(EVENTLOG_WARNING_TYPE, MSG_PASSWORD_REJECTED_REJECTION_REGEX, 3, setOperation ? L"set" : L"change", accountName.c_str(), fullName.c_str());
			return FALSE;
		}

		OutputDebugString(L"Password not did match the rejection regular expression");
	}

	return TRUE;
}
