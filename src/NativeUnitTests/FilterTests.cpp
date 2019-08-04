#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/filter.h"
#include "passwordevaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(FilterTests)
	{
		TEST_METHOD_INITIALIZE(Cleanup)
		{
			DeleteUnitTestPolicyKey(L"", L"Default");
		}

		TEST_METHOD(PasswordFilterSetNormalizedBannedWord)
		{
			std::wstring username_raw = L"username";
			std::wstring fullname_raw = L"fullname";
			std::wstring bad_password_normalize = L"Password345!";

			UNICODE_STRING username = GetUnicodeString(username_raw);
			UNICODE_STRING fullname = GetUnicodeString(fullname_raw);
			UNICODE_STRING password = GetUnicodeString(bad_password_normalize);

			const auto result = PasswordFilter(&username, &fullname, &password, TRUE);

			Assert::IsFalse(result);
		}

		TEST_METHOD(PasswordFilterChangeNormalizedBannedWord)
		{
			std::wstring username_raw = L"username";
			std::wstring fullname_raw = L"fullname";
			std::wstring bad_password_normalize = L"Password345!";
			UNICODE_STRING username = GetUnicodeString(username_raw);
			UNICODE_STRING fullname = GetUnicodeString(fullname_raw);
			UNICODE_STRING password = GetUnicodeString(bad_password_normalize);

			const auto result = PasswordFilter(&username, &fullname, &password, FALSE);

			Assert::IsFalse(result);
		}

		TEST_METHOD(PasswordFilterExSetNormalizedBannedWord)
		{
			std::wstring username_raw = L"username";
			std::wstring fullname_raw = L"fullname";
			std::wstring bad_password_normalize = L"Password345!";

			const auto result = PasswordFilterEx(username_raw.c_str(), fullname_raw.c_str(), bad_password_normalize.c_str(), TRUE);
			Assert::AreEqual(PASSWORD_REJECTED_BANNED_NORMALIZED_WORD, result);
		}

		TEST_METHOD(PasswordFilterExChangeNormalizedBannedWord)
		{
  			std::wstring username_raw = L"username";
			std::wstring fullname_raw = L"fullname";
			std::wstring bad_password_normalize = L"Password345!";
			const auto result = PasswordFilterEx(username_raw.c_str(), fullname_raw.c_str(), bad_password_normalize.c_str(), FALSE);
			Assert::AreEqual(PASSWORD_REJECTED_BANNED_NORMALIZED_WORD, result);
		}
	};
}