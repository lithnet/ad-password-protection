#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/complexityevaluator.h"
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PasswordEvaluatorComplexityThresholdTests)
	{
	public:
		registry reg;

		void ResetValues()
		{
			SetValue(L"Threshold1RequiresLower", 0);
			SetValue(L"Threshold1RequiresNumber", 0);
			SetValue(L"Threshold1RequiresSymbol", 0);
			SetValue(L"Threshold1RequiresSymbolOrNumber", 0);
			SetValue(L"Threshold1RequiresUpper", 0);
			SetValue(L"Threshold1CharsetsRequired", 0);
			SetValue(L"ComplexityThreshold1", 0);

			SetValue(L"Threshold2RequiresLower", 0);
			SetValue(L"Threshold2RequiresNumber", 0);
			SetValue(L"Threshold2RequiresSymbol", 0);
			SetValue(L"Threshold2RequiresSymbolOrNumber", 0);
			SetValue(L"Threshold2RequiresUpper", 0);
			SetValue(L"Threshold2CharsetsRequired", 0);
			SetValue(L"ComplexityThreshold2", 0);


			SetValue(L"Threshold3RequiresLower", 0);
			SetValue(L"Threshold3RequiresNumber", 0);
			SetValue(L"Threshold3RequiresSymbol", 0);
			SetValue(L"Threshold3RequiresSymbolOrNumber", 0);
			SetValue(L"Threshold3RequiresUpper", 0);
			SetValue(L"Threshold3CharsetsRequired", 0);
			SetValue(L"ComplexityThreshold3", 0);
		}

		TEST_METHOD_INITIALIZE(Init)
		{
			ResetValues();
		}


		void TestThreshold(std::wstring passwordPass, std::wstring passwordFail)
		{
			TestString pwdPass(passwordPass);
			Assert::IsTrue(ProcessPasswordComplexityThreshold(pwdPass, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));

			TestString pwdFail(passwordFail);
			Assert::IsFalse(ProcessPasswordComplexityThreshold(pwdFail, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(ThresholdRequiresNumber)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresNumber", 1);
			TestThreshold(L"passworddddd 0", L"passwordddd");


			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresNumber", 1);
			TestThreshold( L"passworddddd 0", L"passwordddd");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresNumber", 1);
			TestThreshold(L"passworddddd 0", L"passwordddd");
		}


		TEST_METHOD(ThresholdRequiresLower)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresLower", 1);
			TestThreshold(L"PASSWORDDDDDDd", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresLower", 1);
			TestThreshold(L"PASSWORDDDDDDd", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresLower", 1);		
			TestThreshold(L"PASSWORDDDDDDd", L"PASSWORDDDDDD");
		}

		TEST_METHOD(ThresholdRequiresSymbol)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresSymbol", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresSymbol", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresSymbol", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");
		}


		TEST_METHOD(ThresholdRequiresSymbolOrNumber_Number)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD1", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD1", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD1", L"PASSWORDDDDDD");
		}

		TEST_METHOD(ThresholdRequiresSymbolOrNumber_Symbol)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresSymbolOrNumber", 1);
			TestThreshold(L"PASSWORDDDDDD!", L"PASSWORDDDDDD");
		}

		TEST_METHOD(ThresholdRequiresUpper)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1RequiresUpper", 1);
			TestThreshold(L"passworddddd P", L"passwordddd");


			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2RequiresUpper", 1);
			TestThreshold(L"passworddddd P", L"passwordddd");


			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3RequiresUpper", 1);
			TestThreshold(L"passworddddd P", L"passwordddd");
		}

		TEST_METHOD(ThresholdRequires1Charset)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1CharsetsRequired", 1);
			TestThreshold(L"passworddddd", L"");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2CharsetsRequired", 1);
			TestThreshold(L"passworddddd", L"");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3CharsetsRequired", 1);
			TestThreshold(L"passworddddd", L"");
		}

		TEST_METHOD(ThresholdRequires2Charset)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1CharsetsRequired", 2);
			TestThreshold(L"passwordddddD", L"passworddddd");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2CharsetsRequired", 2);
			TestThreshold(L"passwordddddD", L"passworddddd");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3CharsetsRequired", 2);
			TestThreshold(L"passwordddddD", L"passworddddd");
		}

		TEST_METHOD(ThresholdRequires3Charset)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1CharsetsRequired", 3);
			TestThreshold(L"passwordddddD1", L"passwordddddD");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2CharsetsRequired", 3);
			TestThreshold(L"passwordddddD1", L"passwordddddD");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3CharsetsRequired", 3);
			TestThreshold(L"passwordddddD1", L"passwordddddD");
		}

		TEST_METHOD(ThresholdRequires4Charset)
		{
			SetValue(L"ComplexityThreshold1", 7);
			SetValue(L"Threshold1CharsetsRequired", 4);
			TestThreshold(L"passwordddddD1!", L"passwordddddD1");

			SetValue(L"ComplexityThreshold2", 8);
			SetValue(L"Threshold2CharsetsRequired", 4);
			TestThreshold(L"passwordddddD1!", L"passwordddddD1");

			SetValue(L"ComplexityThreshold3", 9);
			SetValue(L"Threshold3CharsetsRequired", 4);
			TestThreshold(L"passwordddddD1!", L"passwordddddD1");
		}
	};
}