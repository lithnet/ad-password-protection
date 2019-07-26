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
			const TestString password(L"Password");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestLTrim)
		{
			const TestString password(L"!@#$%^&*()091234567890password");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestRTrim)
		{
			const TestString password(L"password!@#$%^&*()091234567890");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestRemoveWhitespace)
		{
			const TestString password(L"pas     sw\tord");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestReplaceSubstitutions)
		{
			const TestString password(L"p@$5w0rd");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringTestReplaceSubstitutions2)
		{
			const TestString password(L"Password345!");
			Assert::AreEqual(L"password", NormalizePassword(password));
		}

		TEST_METHOD(NormalizedStringNoLetters)
		{
			const TestString password(L"123456789");
			Assert::AreEqual(L"123456789", NormalizePassword(password));
		}
	};
}