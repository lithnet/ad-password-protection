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

	private:
		static void LoopGoodRandomPasswords()
		{
			//ClearCache();

			const std::wstring accountName = std::wstring(L"accountName");
			const std::wstring fullname = std::wstring(L"full name");
			const user_policy pol = policy::GetPolicySet(L"UnitTests");

			for (size_t i = 0; i < 1000; i++)
			{
				const std::wstring guid = GetGuid();
				std::wstringstream ss;
				ss << i << L": " << guid << L": ";
				TestString password(guid);

				const bool result = ProcessPasswordRaw(password, accountName, fullname, TRUE, pol);

				ss << (result ? "passed" : "rejected");

				OutputDebugString(ss.str().c_str());
			}
		}

		void LoopKnownBadPasswords() const
		{
			//ClearCache();
			const user_policy pol = policy::GetPolicySet(L"UnitTests");

			const std::wstring accountName = std::wstring(L"accountName");
			const std::wstring fullname = std::wstring(L"full name");

			std::wstring filename = L"D:\\pwnedpwds\\raw\\words.txt";

			std::wifstream file(filename.c_str());
			std::wstring line;

			const int limit = 1000;
			int count = 0;

			while (std::getline(file, line))
			{
				count++;

				std::wstringstream ss;

				ss << count << L": " << line << L": ";
				TestString password(&line.front());

				const bool result = ProcessPasswordRaw(password, accountName, fullname, TRUE, pol);

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
			SetUnitTestPolicyValue(REG_VALUE_CHECKCOMPROMISEDPASSWORDONSET, 1);
		}
	};
}