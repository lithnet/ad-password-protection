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
		TEST_METHOD(ComplexityPointsPerCharacterTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 12);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerCharacterTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 13);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");

			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerLowerTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 9);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");

			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerLowerTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 10);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");

			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerNumberTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp3");

			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerNumberTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerSymbolTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp!");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerSymbolTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 1);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerUpperTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 1);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password(L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsPerUpperTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 2);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 1);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);
			
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password1@Pp");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfLowerTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);
			
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfLowerTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);
			
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}


		TEST_METHOD(ComplexityPointsUseOfNumberTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);
			
			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfNumberTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}


		TEST_METHOD(ComplexityPointsUseOfSymbolTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfSymbolTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 100);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"PASSWORD");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfUpperTestPass)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 100);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password1@PpW");
			Assert::IsTrue(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(ComplexityPointsUseOfUpperTestFail)
		{
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 100);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);

			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
			SetUnitTestPolicyValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 100);

			const user_policy pol = policy::GetPolicySet(L"UnitTests");
			const TestString password (L"password");
			Assert::IsFalse(ProcessPasswordComplexityPoints(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}
	};
}