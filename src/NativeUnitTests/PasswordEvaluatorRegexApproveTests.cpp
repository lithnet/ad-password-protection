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
		registry reg;
		TEST_METHOD(RegexApprovePass)
		{
			SetValue(L"RegexApprove", L"approve.+");
			TestString password(L"approve me");
			Assert::IsTrue(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(L"RegexApprove");
		}

		TEST_METHOD(RegexApproveFail)
		{
			SetValue(L"RegexApprove", L"approve.+");
			TestString password(L"don't approve this");
			Assert::IsFalse(ProcessPasswordRegexApprove(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(L"RegexApprove");
		}

		TEST_METHOD(RegexRejectPass)
		{
			SetValue(L"RegexReject", L"reject.+");
			TestString password(L"don't reject me");
			Assert::IsTrue(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(L"RegexReject");
		}

		TEST_METHOD(RegexRejectFail)
		{
			SetValue(L"RegexReject", L"reject.+");
			TestString password(L"reject me");
			Assert::IsFalse(ProcessPasswordRegexReject(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
			DeleteValue(L"RegexReject");
		}
	};
}