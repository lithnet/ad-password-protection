#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/hibp.h"
#include "passwordevaluator.h"
#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(HibpTests)
	{
	public:
		user_policy pol;

		TEST_METHOD(TestIsInHibp)
		{
			TestString password(L"Password");
			Assert::IsTrue(IsInSha1HibpApi(password));
		}

		TEST_METHOD(TestBadPasswordSetIsRejectedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 11);
			TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBadPasswordSetIsApprovedWhenDisabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 0);
			TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBadPasswordChangeIsRejectedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestBadPasswordChangeIsApprovedWhenDisabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 0);
			TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordChangeIsApprovedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordSetIsApprovedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordSetIsApprovedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordSetIsRejectedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordChangeIsApprovedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestPasswordChangeIsRejectedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME, L"nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(LoadTestOnMockApi)
		{
			return;

			SetGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME, L"localhost");
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			for (size_t i = 0; i < 1000000; i++)
			{
				std::wstringstream ss;
				ss << L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#";
				ss << i;

				TestString password(ss.str());
				if (!ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol))
				{
					Assert::Fail(L"Password check failed");
				}
			}
		}

		static std::wstring GetHttpResponseFromMockApi(const std::wstring range)
		{
			registry reg;
			const std::wstring host = reg.GetRegValue(L"UnitTest-HibpHostName", L"localhost");
			const unsigned short port = INTERNET_DEFAULT_HTTPS_PORT;
			std::wstring path = L"range/{range}";
			path = ReplaceToken(path, L"{range}", range);
			return GetHttpResponse(host, port, path);
		}

		TEST_METHOD(ValidateAllHashesAreFoundInRange)
		{
			const auto rangeData = GetHttpResponseFromMockApi(L"00000");

			std::wstring line;
			std::vector<std::wstring> hashes;
			std::wstringstream wss(rangeData);

			while (std::getline(wss, line, L'\n'))
			{
				hashes.push_back(line.substr(0, 35));
			}

			for each (std::wstring hash in hashes)
			{
				auto result = IsInVariableWidthRangeData(rangeData, hash);

				if (!result)
				{
					Assert::Fail();
				}
			}
		}

		TEST_METHOD(PerfTestStringContains)
		{
			const auto rangeData = GetHttpResponseFromMockApi(L"00000");
			for (size_t i = 0; i < 100000; i++)
			{
				//return hashes.find(matchtext + L":") != std::string::npos;
				auto result = rangeData.find(L"0198748F3315F40B1A102BF18EEA0194CD9:");
			}
		}

		TEST_METHOD(PerfTestBinarySearch)
		{
			const auto rangeData = GetHttpResponseFromMockApi(L"00000");
			for (size_t i = 0; i < 100000; i++)
			{
				auto result = IsInVariableWidthRangeData(rangeData, L"0198748F3315F40B1A102BF18EEA0194CD9");
			}
		}

		TEST_METHOD(TestRangeMatchAtStart)
		{
			std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			std::wstring matchData(L"003D68EB55068C33ACE09247EE4C639306B");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchAtEnd)
		{
			std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			std::wstring matchData(L"0198748F3315F40B1A102BF18EEA0194CD9");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchAtEof)
		{
			std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9");
			std::wstring matchData(L"0198748F3315F40B1A102BF18EEA0194CD9");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchInMiddle)
		{
			std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			std::wstring matchData(L"01330C689E5D64F660D6947A93AD634EF8F");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}


		TEST_METHOD_CLEANUP(HibpCleanup)
		{
			DeleteGlobalPolicyValue(REG_VALUE_HIBPHOSTNAME);
		}
	};
}