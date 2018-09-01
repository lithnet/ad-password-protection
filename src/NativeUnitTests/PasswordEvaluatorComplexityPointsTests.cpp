#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/complexityevaluator.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorComplexityPointsTests)
	{
	public:
		registry reg;
		TEST_METHOD(ComplexityPointsPerCharacterTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 12);

			SetValue(L"ComplexityPointsPerCharacter", 1);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");

			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerCharacterTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 13);

			SetValue(L"ComplexityPointsPerCharacter", 1);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");

			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerLowerTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 9);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 1);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");

			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerLowerTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 10);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 1);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");

			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerNumberTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 1);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp3");

			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerNumberTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 1);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerSymbolTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 1);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp!");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerSymbolTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 1);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerUpperTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 1);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password(L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsPerUpperTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 2);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 1);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);


			TestString password (L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsUseOfLowerTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 100);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);


			TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsUseOfLowerTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 100);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);


			TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}


		TEST_METHOD(ComplexityPointsUseOfNumberTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 100);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);


			TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsUseOfNumberTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 100);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 0);


			TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}


		TEST_METHOD(ComplexityPointsUseOfSymbolTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 100);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsUseOfSymbolTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 100);
			SetValue(L"ComplexityPointsUseOfUpper", 0);

			TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}


		TEST_METHOD(ComplexityPointsUseOfUpperTestPass)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 100);

			TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ComplexityPointsUseOfUpperTestFail)
		{
			SetValue(L"ComplexityPointsRequired", 100);

			SetValue(L"ComplexityPointsPerCharacter", 0);
			SetValue(L"ComplexityPointsPerLower", 0);
			SetValue(L"ComplexityPointsPerNumber", 0);
			SetValue(L"ComplexityPointsPerSymbol", 0);
			SetValue(L"ComplexityPointsPerUpper", 0);

			SetValue(L"ComplexityPointsUseOfLower", 0);
			SetValue(L"ComplexityPointsUseOfNumber", 0);
			SetValue(L"ComplexityPointsUseOfSymbol", 0);
			SetValue(L"ComplexityPointsUseOfUpper", 100);

			TestString password (L"password");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}
	};
}