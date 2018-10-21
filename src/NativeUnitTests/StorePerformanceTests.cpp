#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "../PasswordFilter/passwordevaluator.h"
#include "../PasswordFilter/registry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(StorePerformanceTests)
	{
		registry reg;

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
				TestString password(wszUuid);

				bool result = ProcessPasswordRaw(password, accountName, fullname, TRUE, reg);

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
				TestString password(&line.front());

				bool result = ProcessPasswordRaw(password, accountName, fullname, TRUE, reg);

				ss << (result ? "passed" : "rejected");

				OutputDebugString(ss.str().c_str());

				if (count >= limit)
				{
					break;
				}
			}
		}

	public:
		TEST_METHOD(LoopKnownBadPasswordsv3Store)
		{
			LoopKnownBadPasswords();
		}

		TEST_METHOD(LoopGoodPasswordv3Store)
		{
			LoopGoodRandomPasswords();

		}

		TEST_CLASS_INITIALIZE(Initialize)
		{
			SetValue(REG_VALUE_CHECKBANNEDPASSWORDONSET, 1);
		}
	};
}