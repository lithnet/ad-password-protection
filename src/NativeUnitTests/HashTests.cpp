#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PasswordFilter/stringnormalization.h"
#include "../PasswordFilter/hasher.h"
#include "../PasswordFilter/utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(HashTests)
	{
	public:

		TEST_METHOD(TestNTLMHash)
		{
			auto h = GetNtlmHashBytes(TestString(L"password"));
			auto s = ToHexWString(h.get(), h.get() + 16);

			Assert::AreEqual(std::wstring(L"8846F7EAEE8FB117AD06BDD830B7586C"), s);
		}

		TEST_METHOD(TestSHA1Hash)
		{
			auto h = GetSha1HashBytes(TestString(L"password"));
			auto s = ToHexWString(h.get(), h.get() + 20);

			Assert::AreEqual(std::wstring(L"5BAA61E4C9B93F3F0682250B6CF8331B7EE68FD8"), s);
		}
	};
}