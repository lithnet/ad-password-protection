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
		registry reg = registry(L"UnitTests");

		TEST_METHOD(ContainsAccountNamePass)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			TestString password(L"doesn't contain the word");
			Assert::IsTrue(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsAccountNameFail)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			TestString password(L"does contain the word accountName");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsAccountNameMixedCaseFail)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 1);
			TestString password(L"does contain the word ACCOUNTname");
			Assert::IsFalse(ProcessPasswordDoesntContainAccountName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNamePass)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			TestString password(L"doesn't contain any of the words");
			Assert::IsTrue(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNameFail)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			TestString password(L"does contain the word full");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ContainsFullNameMixedCaseFail)
		{
			SetValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 1);
			TestString password(L"does contain the word as a substring accountname");
			Assert::IsFalse(ProcessPasswordDoesntContainFullName(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}
	};
}