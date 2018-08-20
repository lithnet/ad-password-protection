#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(StoreEseTests)
	{
	public:
		registry reg;

		TEST_METHOD(TestBannedPasswordOnSetRaw)
		{
			SetValue(L"ValidateRawPasswordOnSet", 1);

			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalized)
		{
			SetValue(L"ValidateNormalizedPasswordOnSet", 1);
			TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalized(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnSetRaw)
		{
			SetValue(L"ValidateRawPasswordOnSet", 1);

			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnSetNormalized)
		{
			SetValue(L"ValidateNormalizedPasswordOnSet", 1);
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalized(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnChangeRaw)
		{
			SetValue(L"ValidateRawPasswordOnChange", 1);

			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBannedPasswordOnChangeNormalized)
		{
			SetValue(L"ValidateNormalizedPasswordOnChange", 1);
			TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalized(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnChangeRaw)
		{
			SetValue(L"ValidateRawPasswordOnChange", 1);

			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestGoodPasswordOnChangeNormalized)
		{
			SetValue(L"ValidateNormalizedPasswordOnChange", 1);
			GUID gidReference;
			HRESULT hCreateGuid = CoCreateGuid(&gidReference);
			WCHAR* wszUuid = NULL;
			UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
			TestString password(wszUuid);

			Assert::IsTrue(ProcessPasswordNormalized(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_CLASS_CLEANUP(Cleanup)
		{

		}

		TEST_CLASS_INITIALIZE(Initialize)
		{
			SetValue(L"HashCheckMode", 0);
		}
	};
}
