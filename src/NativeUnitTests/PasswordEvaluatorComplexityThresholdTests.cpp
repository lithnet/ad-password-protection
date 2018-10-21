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
		registry reg = registry(L"UnitTests");

		void ResetValues() const
		{
			SetValue(REG_VALUE_CT1REQUIRESLOWER, 0);
			SetValue(REG_VALUE_CT1REQUIRESNUMBER, 0);
			SetValue(REG_VALUE_CT1REQUIRESSYMBOL, 0);
			SetValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 0);
			SetValue(REG_VALUE_CT1REQUIRESUPPER, 0);
			SetValue(REG_VALUE_CT1CHARSETSREQUIRED, 0);
			SetValue(REG_VALUE_CT1, 0);

			SetValue(REG_VALUE_CT2REQUIRESLOWER, 0);
			SetValue(REG_VALUE_CT2REQUIRESNUMBER, 0);
			SetValue(REG_VALUE_CT2REQUIRESSYMBOL, 0);
			SetValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 0);
			SetValue(REG_VALUE_CT2REQUIRESUPPER, 0);
			SetValue(REG_VALUE_CT2CHARSETSREQUIRED, 0);
			SetValue(REG_VALUE_CT2, 0);


			SetValue(REG_VALUE_CT3REQUIRESLOWER, 0);
			SetValue(REG_VALUE_CT3REQUIRESNUMBER, 0);
			SetValue(REG_VALUE_CT3REQUIRESSYMBOL, 0);
			SetValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 0);
			SetValue(REG_VALUE_CT3REQUIRESUPPER, 0);
			SetValue(REG_VALUE_CT3CHARSETSREQUIRED, 0);
			SetValue(REG_VALUE_CT3, 0);
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
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESNUMBER, 1);
			TestThreshold(L"password0", L"password");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESNUMBER, 1);
			TestThreshold( L"password123", L"passwordabc");

			SetValue(REG_VALUE_CT3REQUIRESNUMBER, 1);
			TestThreshold(L"passwordabcde 0", L"passwordabcde");
		}


		TEST_METHOD(ThresholdRequiresLower)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESLOWER, 1);
			TestThreshold(L"PASSWORd", L"PASSWORD");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESLOWER, 1);
			TestThreshold(L"PASSWORDABc", L"PASSWORDABC");

			SetValue(REG_VALUE_CT3REQUIRESLOWER, 1);		
			TestThreshold(L"PASSWORDABCDe", L"PASSWORDABCDE");
		}

		TEST_METHOD(ThresholdRequiresSymbol)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESSYMBOL, 1);
			TestThreshold(L"password!", L"password");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESSYMBOL, 1);
			TestThreshold(L"passwordab!", L"passwordabc");

			SetValue(REG_VALUE_CT3REQUIRESSYMBOL, 1);
			TestThreshold(L"passwordabcd!", L"passwordabcde");
		}


		TEST_METHOD(ThresholdRequiresSymbolOrNumber_Number)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"password1", L"password");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"passwordab1", L"passwordabc");

			SetValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"passwordabcd1", L"passwordabcde");
		}

		TEST_METHOD(ThresholdRequiresSymbolOrNumber_Symbol)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"password!", L"password");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"passwordab!", L"passwordabc");

			SetValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 1);
			TestThreshold(L"passwordabcd!", L"passwordabcde");
		}

		TEST_METHOD(ThresholdRequiresUpper)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1REQUIRESUPPER, 1);
			TestThreshold(L"passwordA", L"password");
					   
			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2REQUIRESUPPER, 1);
			TestThreshold(L"passwordabC", L"passwordabc");
			
			SetValue(REG_VALUE_CT3REQUIRESUPPER, 1);
			TestThreshold(L"passwordabcdE", L"passwordabcde");
		}

		TEST_METHOD(ThresholdRequires1Charset)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1CHARSETSREQUIRED, 1);
			TestThreshold(L"password", L"");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2CHARSETSREQUIRED, 1);
			TestThreshold(L"passwordabc", L"");

			SetValue(REG_VALUE_CT3CHARSETSREQUIRED, 1);
			TestThreshold(L"passwordabcde", L"");
		}

		TEST_METHOD(ThresholdRequires2Charset)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1CHARSETSREQUIRED, 2);
			TestThreshold(L"Password", L"password");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2CHARSETSREQUIRED, 2);
			TestThreshold(L"Passwordabc", L"passwordabc");

			SetValue(REG_VALUE_CT3CHARSETSREQUIRED, 2);
			TestThreshold(L"Passwordabcde", L"passwordabcde");
		}

		TEST_METHOD(ThresholdRequires3Charset)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1CHARSETSREQUIRED, 3);
			TestThreshold(L"Password1", L"password1");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2CHARSETSREQUIRED, 3);
			TestThreshold(L"Passwordab1", L"passwordab1");

			SetValue(REG_VALUE_CT3CHARSETSREQUIRED, 3);
			TestThreshold(L"Passwordabcd1", L"passwordabcd1");
		}

		TEST_METHOD(ThresholdRequires4Charset)
		{
			SetValue(REG_VALUE_CT1, 10);
			SetValue(REG_VALUE_CT1CHARSETSREQUIRED, 4);
			TestThreshold(L"Passwor!1", L"Pasword1");

			SetValue(REG_VALUE_CT2, 12);
			SetValue(REG_VALUE_CT2CHARSETSREQUIRED, 4);
			TestThreshold(L"Passworda!1", L"Passwordab1");

			SetValue(REG_VALUE_CT3CHARSETSREQUIRED, 4);
			TestThreshold(L"Passwordabc!1", L"Passwordabcd1");
		}
	};
}