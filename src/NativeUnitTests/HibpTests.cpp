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
		size_t validation_upper_bound = 0x0FFFF;


	public:
		user_policy pol;

		TEST_METHOD(TestIsInHibpSha1Api)
		{
			registry reg;
			const TestString password(L"Password");
			Assert::IsTrue(IsInHibpSha1Api(password, reg));
		}

		TEST_METHOD(TestIsInHibpHtlmHexApi)
		{
			registry reg;
			const TestString password(L"Password");
			Assert::IsTrue(IsInHibpNtlmHexApi(password, reg));
		}

		TEST_METHOD(TestIsInHibpNtlmBinaryApi)
		{
			registry reg;
			const TestString password(L"Password");
			Assert::IsTrue(IsInHibpNtlmBinaryApi(password, reg));
		}

		TEST_METHOD(TestIsInHibpNtlmBinaryPwnCountApi)
		{
			registry reg;
			const TestString password(L"Password");
			Assert::IsTrue(IsInHibpNtlmBinaryPwnCountApi(password, reg));
		}

		TEST_METHOD(TestBadPasswordSetIsRejectedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 11);
			const TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBadPasswordSetIsApprovedWhenDisabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 0);
			const TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestBadPasswordChangeIsRejectedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			const TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestBadPasswordChangeIsApprovedWhenDisabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 0);
			const TestString password(L"password");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordChangeIsApprovedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestGoodPasswordSetIsApprovedWhenEnabled)
		{
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordSetIsApprovedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL, L"http://nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordSetIsRejectedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL, L"http://nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol));
		}

		TEST_METHOD(TestPasswordChangeIsApprovedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL, L"http://nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 0);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsTrue(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(TestPasswordChangeIsRejectedWhenHibpErrors)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL, L"http://nowhere.local");
			SetGlobalPolicyValue(REG_VALUE_REJECTPASSWORDONHIBPERROR, 1);
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONCHANGE, 1);
			const TestString password(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);
			Assert::IsFalse(ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), FALSE, pol));
		}

		TEST_METHOD(LoadTestOnSha1MockApi)
		{
			SetGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL, L"http://localhost/range/{range}");
			SetUnitTestPolicyValue(REG_VALUE_CHECKHIBPONSET, 1);
			policy::PopulatePolicySetObject(L"UnitTests\\Default", &pol);

			for (size_t i = 0; i < 10000; i++)
			{
				std::wstring pwd(L"asfdadsfasdjhk348925hjksdgfhjksdfhgjkdsfAASDFASD23423432sdgasd$#$#");
				pwd.append(std::to_wstring(i));

				const TestString password(pwd);
				if (!ProcessPasswordHibp(password, std::wstring(L"accountName"), std::wstring(L"full name"), TRUE, pol))
				{
					Assert::Fail(L"Password check failed");
				}
			}
		}

		static std::wstring GetHttpResponseFromMockApiSha1Hex(const std::wstring& range)
		{
			registry reg;
			std::wstring apiurl = L"http://localhost/range/{range}";
			apiurl = ReplaceToken(apiurl, L"{range}", range);
			return  GetHttpResponseString(apiurl).str();
		}


		static std::wstring GetHttpResponseFromMockApiNtlmHex(const std::wstring& range)
		{
			registry reg;
			std::wstring apiurl = L"http://localhost/ntlm/hex/range/{range}";
			apiurl = ReplaceToken(apiurl, L"{range}", range);
			return  GetHttpResponseString(apiurl).str();
		}

		static std::vector<BYTE> GetBinaryHttpResponseFromMockApNtlmPwnCount(const std::wstring& range)
		{
			registry reg;
			std::wstring apiurl = L"http://localhost/ntlm/binary-pwncount/range/{range}";
			apiurl = ReplaceToken(apiurl, L"{range}", range);
			return GetHttpResponseBinary(apiurl);
		}

		static std::vector<BYTE> GetBinaryHttpResponseFromMockApNtlm(const std::wstring& range)
		{
			registry reg;
			std::wstring apiurl = L"https://api.lithiumblue.com/v1/ntlm/{range}";
			apiurl = ReplaceToken(apiurl, L"{range}", range);
			return GetHttpResponseBinary(apiurl);
		}

		static std::wstring IntToHex(int i)
		{
			std::wstringstream stream;
			stream << std::uppercase << std::setfill(L'0') << std::setw(5) << std::hex << i;
			return stream.str();
		}

		TEST_METHOD(ValidateAllHashesAreFoundInSha1HexRange)
		{
			for (size_t x = 0; x <= validation_upper_bound; x++)
			{
				const auto range = IntToHex(x);
				const auto rangeData = GetHttpResponseFromMockApiSha1Hex(range);

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
		}

		TEST_METHOD(ValidateAllHashesAreFoundInNtlmHexRange)
		{
			for (size_t x = 0; x <= validation_upper_bound; x++)
			{
				const auto range = IntToHex(x);
				const auto rangeData = GetHttpResponseFromMockApiNtlmHex(range);

				std::wstring line;
				std::vector<std::wstring> hashes;
				std::wstringstream wss(rangeData);

				while (std::getline(wss, line, L'\n'))
				{
					hashes.push_back(line.substr(0, 27));
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
		}

		TEST_METHOD(ValidateAllHashesAreFoundInBinaryNtlmWithPwnCount)
		{
			for (size_t x = 0; x <= validation_upper_bound; x++)
			{
				const auto range = IntToHex(x);
				auto rangeData = GetBinaryHttpResponseFromMockApNtlmPwnCount(range);
				size_t record_size = 16;
				size_t hash_size = 14;
				size_t hashCount = rangeData.size() / record_size;

				for (size_t i = 0; i < hashCount; i++)
				{
					SecureArrayT<BYTE> data(hash_size);
					memcpy_s(data.get(), hash_size, rangeData.data() + (i * record_size), hash_size);

					auto result = IsInFixedWidthRangeData(rangeData, data, record_size);

					if (!result)
					{
						Assert::Fail();
					}
				}
			}
		}

		TEST_METHOD(ValidateAllHashesAreFoundInBinaryNtlm)
		{
			for (size_t x = 0; x <= validation_upper_bound; x++)
			{
				const auto range = IntToHex(x);
				auto rangeData = GetBinaryHttpResponseFromMockApNtlm(range);
				size_t record_size = 14;
				const size_t hash_size = 14;
				const size_t hashCount = rangeData.size() / record_size;

				OutputDebugString(L"Processing range " + range);

				for (size_t i = 0; i < hashCount; i++)
				{
					SecureArrayT<BYTE> data(hash_size);
					memcpy_s(data.get(), hash_size, rangeData.data() + (i * record_size), hash_size);

					const auto result = IsInFixedWidthRangeData(rangeData, data, record_size);

					if (!result)
					{
						Assert::Fail();
					}
				}
			}
		}

		TEST_METHOD(PerfTestStringContains)
		{
			const auto rangeData = GetHttpResponseFromMockApiSha1Hex(L"00000");
			for (size_t i = 0; i < 100000; i++)
			{
				//return hashes.find(matchtext + L":") != std::string::npos;
				auto result = rangeData.find(L"0198748F3315F40B1A102BF18EEA0194CD9:");
			}
		}

		TEST_METHOD(PerfTestBinarySearch)
		{
			const auto rangeData = GetHttpResponseFromMockApiSha1Hex(L"00000");
			for (size_t i = 0; i < 100000; i++)
			{
				auto result = IsInVariableWidthRangeData(rangeData, L"0198748F3315F40B1A102BF18EEA0194CD9");
			}
		}

		TEST_METHOD(TestRangeMatchAtStart)
		{
			const std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			const std::wstring matchData(L"003D68EB55068C33ACE09247EE4C639306B");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchAtEnd)
		{
			const std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			const std::wstring matchData(L"0198748F3315F40B1A102BF18EEA0194CD9");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchAtEof)
		{
			const std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9");
			const std::wstring matchData(L"0198748F3315F40B1A102BF18EEA0194CD9");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD(TestRangeMatchInMiddle)
		{
			const std::wstring rangeData(L"003D68EB55068C33ACE09247EE4C639306B:3\r\n012C192B2F16F82EA0EB9EF18D9D539B0DD:1\r\n01330C689E5D64F660D6947A93AD634EF8F:1\r\n0198748F3315F40B1A102BF18EEA0194CD9:1\r\n");
			const std::wstring matchData(L"01330C689E5D64F660D6947A93AD634EF8F");
			Assert::IsTrue(IsInVariableWidthRangeData(rangeData, matchData));
		}

		TEST_METHOD_CLEANUP(HibpCleanup)
		{
			DeleteGlobalPolicyValue(REG_VALUE_HIBPSHA1APIURL);
		}
	};
}