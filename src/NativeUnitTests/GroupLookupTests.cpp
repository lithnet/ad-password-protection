#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/groups.h"
#include "sddl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(GroupTests)
	{
	public:

		TEST_METHOD(TestUserLookup)
		{
			PSID sid;

			GetSid(L"fim-dev1\\rnewing", &sid);

			LPWSTR sidstring = nullptr;

			ConvertSidToStringSid(sid, &sidstring);

			LocalFree(sidstring);

			AUTHZ_CLIENT_CONTEXT_HANDLE hAuthzClientContext = nullptr;
			DWORD result = GetUserContext(sid, hAuthzClientContext);

			result = GetGroupsFromContext(hAuthzClientContext);



		}

		TEST_METHOD(TestSHA1Hash)
		{

			//Assert::AreEqual(std::wstring(L"5BAA61E4C9B93F3F0682250B6CF8331B7EE68FD8"), s);
		}

	};

}