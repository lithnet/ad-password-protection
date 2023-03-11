#include "stdafx.h"
#include "CppUnitTest.h"
#include <rpcdce.h>
#include <iomanip>
#include "FilterTests.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	/// <summary>
	/// Add the following words and passwords to complete these unit tests
	/// 
	/// Add-CompromisedPassword "password"
	/// Add-CompromisedPassword "abacus"
	/// Add-BannedWord "abacus"
	/// Add-BannedWord "password"
	/// </summary>

	TEST_CLASS(Storev3Tests)
	{
	public:
		registry reg = registry(L"UnitTests");

		TEST_METHOD(TestBannedPasswordOnSetRaw)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 1);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalizedAgainstPasswordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 1);
			TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalized2AgainstPasswordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 1);
			TestString password(L"Password345!");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalizedAgainstWordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalized2AgainstWordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			TestString password(L"Password345!");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnSetRaw)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 1);

			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnSetNormalizedAgainstPasswordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 1);
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnSetNormalizedAgainstWordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnChangeRaw)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONCHANGE, 1);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnChangeNormalizedAgainstPasswordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONCHANGE, 1);
			TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnChangeNormalizedAgainstWordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 1);
			TestString password(L"!!abacus#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnChangeRaw)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONCHANGE, 1);

			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnChangeNormalizedAgainstPasswordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONCHANGE, 1);
			
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnChangeNormalizedAgainstWordStore)
		{
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 1);
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		private:
		TEST_METHOD_INITIALIZE(CleanupRegistry)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONCHANGE, 0);
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 0);
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONCHANGE, 0);
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDPASSWORDONSET, 0);
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 0);
			SetValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 0);
		}
	};
}
