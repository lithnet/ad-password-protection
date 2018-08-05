#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "../PasswordFilter/filter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(StorePerformanceTests)
	{
	private:
		void LoopGoodRandomPasswords()
		{
			ClearCache();

			PUNICODE_STRING username = new UNICODE_STRING();
			RtlInitUnicodeString(username, L"test");

			PUNICODE_STRING fullname = new UNICODE_STRING();
			RtlInitUnicodeString(fullname, L"fullname");

			for (size_t i = 0; i < 1000; i++)
			{
				PUNICODE_STRING password = new UNICODE_STRING();
				GUID gidReference;
				HRESULT hCreateGuid = CoCreateGuid(&gidReference);
				WCHAR* wszUuid = NULL;
				UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);

				RtlInitUnicodeString(password, wszUuid);

				PasswordFilter(username, fullname, password, TRUE);

				RpcStringFree((RPC_WSTR*)&wszUuid);

				delete[] password;
			}

			delete[] username;
			delete[] fullname;
		}

		void LoopKnownBadPasswords()
		{
			ClearCache();

			std::wstring filename = L"D:\\pwnedpwds\\raw\\words.txt";

			std::wifstream file(filename.c_str());
			std::wstring line;

			PUNICODE_STRING username = new UNICODE_STRING();
			RtlInitUnicodeString(username, L"test");

			PUNICODE_STRING fullname = new UNICODE_STRING();
			RtlInitUnicodeString(fullname, L"fullname");

			int limit = 1000;
			int count = 0;

			while (std::getline(file, line))
			{
				count++;
				PUNICODE_STRING password = new UNICODE_STRING();

				RtlInitUnicodeString(password, line.c_str());

				std::wstringstream ss;

				ss << line << L": ";

				bool result = PasswordFilter(username, fullname, password, TRUE);

				ss << (result ? "passed" : "rejected");

				OutputDebugString(ss.str().c_str());

				delete[] password;

				if (count >= limit)
				{
					break;
				}
			}

			delete[] username;
			delete[] fullname;
		}

	public:

		TEST_METHOD(LoopKnownBadPasswordsv1Store)
		{
			SetValue(L"HashCheckMode", 1);
			LoopKnownBadPasswords();
			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopKnownBadPasswordsEseStore)
		{
			SetValue(L"HashCheckMode", 0);
			LoopKnownBadPasswords();
			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopKnownBadPasswordsv2Store)
		{
			SetValue(L"HashCheckMode", 3);
			LoopKnownBadPasswords();
			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordv1Store)
		{
			SetValue(L"HashCheckMode", 1);

			LoopGoodRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordv2Store)
		{
			SetValue(L"HashCheckMode", 3);

			LoopGoodRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordEseStore)
		{
			SetValue(L"HashCheckMode", 0);

			LoopGoodRandomPasswords();
		}
	};
}