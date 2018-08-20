#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorNameTests)
	{
	public:
		registry reg;

		TEST_METHOD(ContainsAccountNamePass)
		{
			SetValue(L"ValidatePasswordDoesntContainAccountName", 1);
			TestString password(L"doesn't contain the word");
			Assert::IsTrue(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsAccountNameFail)
		{
			SetValue(L"ValidatePasswordDoesntContainAccountName", 1);
			TestString password(L"does contain the word accountName");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsAccountNameMixedCaseFail)
		{
			SetValue(L"ValidatePasswordDoesntContainAccountName", 1);
			TestString password(L"does contain the word ACCOUNTname");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNamePass)
		{
			SetValue(L"ValidatePasswordDoesntContainFullName", 1);
			TestString password(L"doesn't contain any of the words");
			Assert::IsTrue(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNameFail)
		{
			SetValue(L"ValidatePasswordDoesntContainFullName", 1);
			TestString password(L"does contain the word full");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNameMixedCaseFail)
		{
			SetValue(L"ValidatePasswordDoesntContainFullName", 1);
			TestString password(L"does contain the word as a substring accountname");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}
	};
}