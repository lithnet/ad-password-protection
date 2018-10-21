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
		registry reg = registry(L"UnitTests");

		TEST_METHOD(PasswordLengthNotMetOnSet)
		{
			SetValue(REG_VALUE_MINIMUMLENGTH, 9);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(PasswordLengthNotMetOnChange)
		{
			SetValue(REG_VALUE_MINIMUMLENGTH, 9);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(PasswordLengthMetOnSet)
		{
			SetValue(REG_VALUE_MINIMUMLENGTH, 8);
			TestString password(L"password");
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(PasswordLengthMetOnChange)
		{
			SetValue(REG_VALUE_MINIMUMLENGTH, 8);
			TestString password(L"password");
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}
	};
}