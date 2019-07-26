#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/policy.h"
#include "../PasswordFilter/utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(PolicySetTests)
	{
	private:
		registry reg = registry(L"UnitTests");

	public:
		/*
		 * These tests require the following objects to be created in AD
		 * - Ten accounts, policy-test-user-0 through policy-test-user-9
		 * - Ten groups, policy-test-group-0 through policy-test-group-9
		 * - Each user is a member of its corresponding policy group (eg policy-test-user-0 is a member of policy-test-group-0, etc)
		 * - At least one group should have an additional member which has a lower number than the group's number. (eg policy-test-user-7 in policy-test-group-8)
		 * - An additional user called policy-test-user-10 who is not a member of any of the policy-test-group objects
		 */

		TEST_CLASS_INITIALIZE(SetupRegistryTestData)
		{
			SetUnitTestPolicyValue(L"0", L"GroupName", L"policy-test-group-0");
			SetUnitTestPolicyValue(L"0", L"Enabled", 1);

			SetUnitTestPolicyValue(L"1", L"GroupName", L"policy-test-group-1");
			SetUnitTestPolicyValue(L"1", L"Enabled", 1);

			SetUnitTestPolicyValue(L"2", L"GroupName", L"policy-test-group-2");
			SetUnitTestPolicyValue(L"2", L"Enabled", 1);

			SetUnitTestPolicyValue(L"3", L"GroupName", L"policy-test-group-3");
			SetUnitTestPolicyValue(L"3", L"Enabled", 1);

			SetUnitTestPolicyValue(L"4", L"GroupName", L"policy-test-group-4");
			SetUnitTestPolicyValue(L"4", L"Enabled", 1);

			SetUnitTestPolicyValue(L"5", L"GroupName", L"policy-test-group-5");
			SetUnitTestPolicyValue(L"5", L"Enabled", 1);

			SetUnitTestPolicyValue(L"6", L"GroupName", L"policy-test-group-6");
			SetUnitTestPolicyValue(L"6", L"Enabled", 1);

			SetUnitTestPolicyValue(L"7", L"GroupName", L"policy-test-group-7");
			SetUnitTestPolicyValue(L"7", L"Enabled", 1);

			SetUnitTestPolicyValue(L"8", L"GroupName", L"policy-test-group-8");
			SetUnitTestPolicyValue(L"8", L"Enabled", 1);

			SetUnitTestPolicyValue(L"9", L"GroupName", L"policy-test-group-9");
			SetUnitTestPolicyValue(L"9", L"Enabled", 1);
		}

		TEST_METHOD(TestGetPolicySetNameForUser0)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-0", reg);
			Assert::AreEqual(L"0", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser1)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-1", reg);
			Assert::AreEqual(L"1", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser2)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-2", reg);
			Assert::AreEqual(L"2", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser3)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-3", reg);
			Assert::AreEqual(L"3", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser4)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-4", reg);
			Assert::AreEqual(L"4", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser5)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-5", reg);
			Assert::AreEqual(L"5", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser6)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-6", reg);
			Assert::AreEqual(L"6", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser7)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-7", reg);
			Assert::AreEqual(L"7", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser8)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-8", reg);
			Assert::AreEqual(L"8", setName.c_str());
		}

		TEST_METHOD(TestGetPolicySetNameForUser9)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-9", reg);
			Assert::AreEqual(L"9", setName.c_str());
		}
		
		TEST_METHOD(TestGetPolicySetNameForUser10)
		{
			auto setName = policy::GetPolicySetNameForUser(L"policy-test-user-10", reg);
			Assert::AreEqual(L"Default", setName.c_str());
		}
	};
}