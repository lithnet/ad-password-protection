#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/policy.h"
#include "../PasswordFilter/utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PolicyTests)
	{
	public:

		TEST_METHOD(TestPolicy)
		{
			policy p;
			p.GetPolicyForUser(L"mgr-rnewing");
		}
	};
}