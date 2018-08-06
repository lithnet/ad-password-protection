#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "../PasswordFilter/passwordevaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(StorePerformanceTests)
	{
	private:
		void LoopGoodRandomPasswords()
		{
			ClearCache();

			std::wstring accountName = std::wstring(L"accountName");
			std::wstring fullname = std::wstring(L"full name");

			for (size_t i = 0; i < 1000; i++)
			{
				GUID gidReference;
				HRESULT hCreateGuid = CoCreateGuid(&gidReference);
				WCHAR* wszUuid = NULL;
				UuidToStringW(&gidReference, (RPC_WSTR*)&wszUuid);
				std::wstringstream ss;
				ss << i << L": " << wszUuid << L": ";

				bool result = ProcessPasswordRaw(wszUuid, accountName, fullname, TRUE);

				ss << (result ? "passed" : "rejected");

				OutputDebugString(ss.str().c_str());
			}
		}

		void LoopKnownBadPasswords()
		{
			ClearCache();

			std::wstring accountName = std::wstring(L"accountName");
			std::wstring fullname = std::wstring(L"full name");

			std::wstring filename = L"D:\\pwnedpwds\\raw\\words.txt";

			std::wifstream file(filename.c_str());
			std::wstring line;

			int limit = 1000;
			int count = 0;

			while (std::getline(file, line))
			{
				count++;

				std::wstringstream ss;

				ss << count << L": " << line << L": ";

				bool result = ProcessPasswordRaw((LPWSTR)line.c_str(), accountName, fullname, TRUE);

				ss << (result ? "passed" : "rejected");

				OutputDebugString(ss.str().c_str());

				if (count >= limit)
				{
					break;
				}
			}
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
			SetValue(L"HashCheckMode", 2);
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
			SetValue(L"HashCheckMode", 2);

			LoopGoodRandomPasswords();

			SetValue(L"HashCheckMode", 0);
		}

		TEST_METHOD(LoopGoodPasswordEseStore)
		{
			SetValue(L"HashCheckMode", 0);

			LoopGoodRandomPasswords();
		}

		TEST_CLASS_INITIALIZE(Initialize)
		{
			SetValue(L"ValidateRawPasswordOnSet", 1);
		}
	};
}