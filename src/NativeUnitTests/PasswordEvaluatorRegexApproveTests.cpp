#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorRegexApproveTests)
	{
	public:
		
		TEST_METHOD(RegexApprovePass)
		{
			SetUnitTestPolicyValue(REG_VALUE_REGEXAPPROVE, L"approve.+");
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"approve me");
			Assert::IsTrue(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
			DeleteUnitTestPolicyValue(REG_VALUE_REGEXAPPROVE);
		}

		TEST_METHOD(RegexApproveFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_REGEXAPPROVE, L"approve.+");
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"don't approve this");
			Assert::IsFalse(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
			DeleteUnitTestPolicyValue(REG_VALUE_REGEXAPPROVE);
		}

		TEST_METHOD(RegexRejectPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_REGEXREJECT, L"reject.+");
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"don't reject me");
			Assert::IsTrue(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
			DeleteUnitTestPolicyValue(REG_VALUE_REGEXREJECT);
		}

		TEST_METHOD(RegexRejectFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_REGEXREJECT, L"reject.+");
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"reject me");
			Assert::IsFalse(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
			DeleteUnitTestPolicyValue(REG_VALUE_REGEXREJECT);
		}
	};
}