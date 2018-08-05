#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/stringnormalization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(StringNormalizationTests)
	{
	public:

		TEST_METHOD(NormalizedStringTestLowerCase)
		{
			LPWSTR password = L"Password";
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestLTrim)
		{
			LPWSTR password = L"!@#$%^&*()091234567890password";
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestRTrim)
		{
			LPWSTR password = L"password!@#$%^&*()091234567890";
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestRemoveWhitespace)
		{
			LPWSTR password = L"pas     sw\tord";
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestReplaceSubstitutions)
		{
			LPWSTR password = L"p@$5w0rd";
			Assert::AreEqual(L"password", NormalizePassword(password));
		}
	};

}