#include "stdafx.h"
#include "CppUnitTest.h"
#include <iomanip>
#include "FilterTests.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/policy.h"
#include "registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(Storev3Tests)
	{
		user_policy pol;

	public:
		TEST_METHOD(TestBannedPasswordOnSetRaw)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalizedAgainstPasswordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalized2AgainstPasswordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"Password345!");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalizedAgainstWordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnSetNormalized2AgainstWordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"Password345!");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnSetRaw)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());
			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnSetNormalizedAgainstPasswordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());

			Assert::IsTrue(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnSetNormalizedAgainstWordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());

			Assert::IsTrue(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnChangeRaw)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"password");
			Assert::IsFalse(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnChangeNormalizedAgainstPasswordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"!!$P@s sw_o+rd$#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestBannedPasswordOnChangeNormalizedAgainstWordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"!!abacus#%^$");
			Assert::IsFalse(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnChangeRaw)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());
			Assert::IsTrue(ProcessPasswordRaw(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnChangeNormalizedAgainstPasswordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());
			Assert::IsTrue(ProcessPasswordNormalizedPasswordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordOnChangeNormalizedAgainstWordStore)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(GetGuid());
			Assert::IsTrue(ProcessPasswordNormalizedWordStore(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		private:
		TEST_METHOD_INITIALIZE(CleanupRegistry)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONCHANGE, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONCHANGE, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDCOMPROMISEDPASSWORDONSET, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONCHANGE, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKNORMALIZEDBANNEDWORDONSET, 0);
		}
	};
}
