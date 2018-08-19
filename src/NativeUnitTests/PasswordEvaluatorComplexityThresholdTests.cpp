#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorComplexityThresholdTests)
	{
	public:
		registry reg;
		TEST_METHOD(AboveThresholdRequiresNumberPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 1);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password 0";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresNumberFail)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 1);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresLowerPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 1);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"PASSWORD p";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE,reg));
		}

		TEST_METHOD(AboveThresholdRequiresLowerFail)
		{
			SetValue(L"AboveThresholdRequiresLower", 1);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"PASSWORD";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE,reg));
		}

		TEST_METHOD(AboveThresholdRequiresSymbolPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 1);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password !";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresSymbolFail)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 1);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresSymbolOrNumberNumberPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 1);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password 1";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresSymbolOrNumberSymbolPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 1);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password !";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresSymbolOrNumberFail)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 1);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresUpperPass)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 1);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password P";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(AboveThresholdRequiresUpperFail)
		{
			SetValue(L"AboveThresholdRequiresLower", 0);
			SetValue(L"AboveThresholdRequiresNumber", 0);
			SetValue(L"AboveThresholdRequiresSymbol", 0);
			SetValue(L"AboveThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"AboveThresholdRequiresUpper", 1);
			SetValue(L"ComplexityThreshold", 7);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

// Below Threshold

		TEST_METHOD(BelowThresholdRequiresNumberPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 1);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password 0";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresNumberFail)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 1);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresLowerPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 1);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"PASSWORD p";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresLowerFail)
		{
			SetValue(L"BelowThresholdRequiresLower", 1);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"PASSWORD";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresSymbolPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 1);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password !";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresSymbolFail)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 1);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresSymbolOrNumberNumberPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 1);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password 1";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresSymbolOrNumberSymbolPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 1);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password !";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresSymbolOrNumberFail)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 1);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 0);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresUpperPass)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 1);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password P";
			Assert::IsTrue(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(BelowThresholdRequiresUpperFail)
		{
			SetValue(L"BelowThresholdRequiresLower", 0);
			SetValue(L"BelowThresholdRequiresNumber", 0);
			SetValue(L"BelowThresholdRequiresSymbol", 0);
			SetValue(L"BelowThresholdRequiresSymbolOrNumber", 0);
			SetValue(L"BelowThresholdRequiresUpper", 1);
			SetValue(L"ComplexityThreshold", 17);

			LPWSTR password = L"password";
			Assert::IsFalse(ProcessPasswordComplexityThreshold(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}
	};
}