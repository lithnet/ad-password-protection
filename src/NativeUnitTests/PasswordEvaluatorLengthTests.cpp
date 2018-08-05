#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorLengthTests)
	{
	public:

		TEST_METHOD(PasswordLengthNotMetOnSet)
		{
			SetValue(L"MinimumLength", 9);
			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE));
		}

		TEST_METHOD(PasswordLengthNotMetOnChange)
		{
			SetValue(L"MinimumLength", 9);
			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE));
		}

		TEST_METHOD(PasswordLengthMetOnSet)
		{
			SetValue(L"MinimumLength", 8);
			LPWSTR password = L"password";
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE));
		}

		TEST_METHOD(PasswordLengthMetOnChange)
		{
			SetValue(L"MinimumLength", 8);
			LPWSTR password = L"password";
			Assert::IsTrue(ProcessPasswordLength(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE));
		}
	};
}