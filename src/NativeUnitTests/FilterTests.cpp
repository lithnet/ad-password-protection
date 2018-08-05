#include "stdafx.h"
#include "CppUnitTest.h"
#include <rpcdce.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "FilterTests.h"
#include "../PasswordFilter/filter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(FilterTests)
	{
	private:
		void LoopRandomPasswords()
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

		void TestBannedPassword()
		{
			PUNICODE_STRING username = new UNICODE_STRING();
			RtlInitUnicodeString(username, L"test");

			PUNICODE_STRING fullname = new UNICODE_STRING();
			RtlInitUnicodeString(fullname, L"fullname");

			const wchar_t * pwd = L"Password";

			size_t  len = wcslen(pwd);
			wchar_t * p = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
			wcscpy(p, pwd);

			PUNICODE_STRING password = new UNICODE_STRING();
			RtlInitUnicodeString(password, p);

			Assert::IsFalse(PasswordFilter(username, fullname, password, TRUE));

			free(p);

			delete[] username;
			delete[] fullname;
			delete[] password;
		}

		void TestBannedPassword2()
		{
			PUNICODE_STRING username = new UNICODE_STRING();
			RtlInitUnicodeString(username, L"test");

			PUNICODE_STRING fullname = new UNICODE_STRING();
			RtlInitUnicodeString(fullname, L"fullname");

			const wchar_t * pwd = L"!!$P@s sw_o+rd$#%^$";

			size_t  len = wcslen(pwd);
			wchar_t * p = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
			wcscpy(p, pwd);

			PUNICODE_STRING password = new UNICODE_STRING();
			RtlInitUnicodeString(password, p);

			Assert::IsFalse(PasswordFilter(username, fullname, password, TRUE));

			free(p);

			delete[] username;
			delete[] fullname;
			delete[] password;
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

		TEST_METHOD(TestBadPasswordEseStore)
		{
			SetValue(L"HashCheckMode", 0);

			TestBannedPassword();
		}

		TEST_METHOD(TestBadPasswordv1Store)
		{
			SetValue(L"HashCheckMode", 1);

			TestBannedPassword();

			SetValue(L"HashCheckMode", 0);
		}

		/*TEST_METHOD(TestBadPasswordTextStore)
		{
			SetValue(L"HashCheckMode", 2);

			TestBannedPassword();

			SetValue(L"HashCheckMode", 0);
		}*/

		TEST_METHOD(TestBadPasswordv2Store)
		{
			SetValue(L"HashCheckMode", 3);

			TestBannedPassword2();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopTestTextStore)
		{
			SetValue(L"HashCheckMode", 2);

			LoopRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordv1Store)
		{
			SetValue(L"HashCheckMode", 1);

			LoopRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordv2Store)
		{
			SetValue(L"HashCheckMode", 3);

			LoopRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordEseStore)
		{
			SetValue(L"HashCheckMode", 0);

			LoopRandomPasswords();
		}
	};
}