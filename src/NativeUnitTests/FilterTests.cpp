#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/filter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(FilterTests)
	{
	private:
		void TestBannedPassword()
		{
			PUNICODE_STRING username = new UNICODE_STRING();
			RtlInitUnicodeString(username, L"test");

			PUNICODE_STRING fullname = new UNICODE_STRING();
			RtlInitUnicodeString(fullname, L"fullname");

			const wchar_t * pwd = L"Password345!";

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

	public:

		TEST_METHOD(TestBadPasswordv3Store)
		{
		}
	};
}