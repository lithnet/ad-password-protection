#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/hibp.h"
#include "passwordevaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(HibpTests)
	{
	public:
		registry reg = registry(L"UnitTests");

		TEST_METHOD(TestIsInHibp)
		{
			TestString password(L"Password");
			Assert::IsTrue(IsInHibp(password, reg));
		}

		TEST_METHOD(TestBadPasswordSetIsRejectedWhenEnabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBadPasswordSetIsApprovedWhenDisabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONSET, 0);
			TestString password(L"password");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestBadPasswordChangeIsRejectedWhenEnabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"password");
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestBadPasswordChangeIsApprovedWhenDisabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONCHANGE, 0);
			TestString password(L"password");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestGoodPasswordChangeIsApprovedWhenEnabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestGoodPasswordSetIsApprovedWhenEnabled)
		{
			SetValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestPasswordSetIsApprovedWhenHibpErrors)
		{
			SetValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestPasswordSetIsRejectedWhenHibpErrors)
		{
			SetValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg));
		}

		TEST_METHOD(TestPasswordChangeIsApprovedWhenHibpErrors)
		{
			SetValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(TestPasswordChangeIsRejectedWhenHibpErrors)
		{
			SetValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, reg));
		}

		TEST_METHOD(LoadTestOnMockApi)
		{
			return;

			SetValue(REG_VALUE_HIBPHOSTNAME, L"localhost");
			SetValue(REG_VALUE_CHECKHIBPONSET, 1);

			for (size_t i = 0; i < 1000000; i++)
			{
				std::wstringstream ss;
				ss << L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#";
				ss << i;

				TestString password(ss.str());
				if (!ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, reg))
				{
					Assert::Fail(L"Password check failed");
				}
			}
		}

		TEST_METHOD_CLEANUP(HibpCleanup)
		{
			DeleteValue(REG_VALUE_HIBPHOSTNAME);
		}
	};
}