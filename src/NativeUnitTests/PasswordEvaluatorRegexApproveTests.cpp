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
		registry reg = registry(L"UnitTests");

		TEST_METHOD(RegexApprovePass)
		{
			SetValue(REG_VALUE_REGEXAPPROVE, L"approve.+");
			TestString password(L"approve me");
			Assert::IsTrue(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(REG_VALUE_REGEXAPPROVE);
		}

		TEST_METHOD(RegexApproveFail)
		{
			SetValue(REG_VALUE_REGEXAPPROVE, L"approve.+");
			TestString password(L"don't approve this");
			Assert::IsFalse(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(REG_VALUE_REGEXAPPROVE);
		}

		TEST_METHOD(RegexRejectPass)
		{
			SetValue(REG_VALUE_REGEXREJECT, L"reject.+");
			TestString password(L"don't reject me");
			Assert::IsTrue(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(REG_VALUE_REGEXREJECT);
		}

		TEST_METHOD(RegexRejectFail)
		{
			SetValue(REG_VALUE_REGEXREJECT, L"reject.+");
			TestString password(L"reject me");
			Assert::IsFalse(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(REG_VALUE_REGEXREJECT);
		}
	};
}