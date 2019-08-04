#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorNameTests)
	{
		user_policy pol;

	public:
		
		TEST_METHOD(ContainsAccountNamePass)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"doesn't contain the word");
			Assert::IsTrue(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ContainsAccountNameFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"does contain the word accountName");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ContainsAccountNameMixedCaseFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"does contain the word ACCOUNTname");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ContainsFullNamePass)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"doesn't contain any of the words");
			Assert::IsTrue(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ContainsFullNameFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"does contain the word full");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ContainsFullNameMixedCaseFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			const TestString password(L"does contain the word as a substring accountname");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}
	};
}