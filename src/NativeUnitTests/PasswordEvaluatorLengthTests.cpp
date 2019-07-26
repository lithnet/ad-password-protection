#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorLengthTests)
	{
	public:
		TEST_METHOD(PasswordLengthNotMetOnSet)
		{
			SetUnitTestPolicyValue(REG_VALUE_MINIMUMLENGTH, 9);
			const user_policy pol = policy::GetPolicyForGroup(L"UnitTests");
			const TestString password(L"password");
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(PasswordLengthNotMetOnChange)
		{
			SetUnitTestPolicyValue(REG_VALUE_MINIMUMLENGTH, 9);
			const user_policy pol = policy::GetPolicyForGroup(L"UnitTests");
			const TestString password(L"password");
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(PasswordLengthMetOnSet)
		{
			SetUnitTestPolicyValue(REG_VALUE_MINIMUMLENGTH, 8);
			const user_policy pol = policy::GetPolicyForGroup(L"UnitTests");
			const TestString password(L"password");
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(PasswordLengthMetOnChange)
		{
			SetUnitTestPolicyValue(REG_VALUE_MINIMUMLENGTH, 8);
			const user_policy pol = policy::GetPolicyForGroup(L"UnitTests");
			const TestString password(L"password");
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}
	};
}