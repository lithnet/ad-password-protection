#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	namespace
	{
		const size_t GuardSize = 16;
		const BYTE GuardPattern = 0xCC;
		const BYTE FillPattern = 0xA5;

		void* allocatedBlock = nullptr;
		size_t allocatedSize = 0;
		std::vector<BYTE> releasedContents;

		// An array element the same size as WCHAR. Its class-level array allocation
		// functions add guard bytes after each block, and copy the block (including
		// the guard bytes) just before it is freed. This lets a test see what
		// SecureArrayT left in memory after its destructor ran.
		struct TrackedWideChar
		{
			WCHAR value;

			static void* operator new[](size_t size)
			{
				BYTE* block = static_cast<BYTE*>(::operator new(size + GuardSize));
				memset(block + size, GuardPattern, GuardSize);

				allocatedBlock = block;
				allocatedSize = size;

				return block;
			}

			static void operator delete[](void* block)
			{
				const BYTE* bytes = static_cast<const BYTE*>(block);
				releasedContents.assign(bytes, bytes + allocatedSize + GuardSize);

				::operator delete(block);
			}
		};

		static_assert(sizeof(TrackedWideChar) == sizeof(WCHAR), "TrackedWideChar must be the same size as WCHAR");
	}

	TEST_CLASS(SecureArrayTests)
	{
	public:

		TEST_METHOD_INITIALIZE(ResetTracking)
		{
			allocatedBlock = nullptr;
			allocatedSize = 0;
			releasedContents.clear();
		}

		TEST_METHOD(DestructorZeroesEveryByteOfWideCharBuffer)
		{
			const int elementCount = 16;
			const size_t bufferSize = elementCount * sizeof(TrackedWideChar);

			ReleaseFilledArray(elementCount);

			for (size_t i = 0; i < bufferSize; i++)
			{
				std::wstring message = L"Byte " + std::to_wstring(i) + L" of " + std::to_wstring(bufferSize) + L" was not zeroed";
				Assert::AreEqual(0, static_cast<int>(releasedContents[i]), message.c_str());
			}
		}

		TEST_METHOD(DestructorDoesNotWritePastEndOfWideCharBuffer)
		{
			const int elementCount = 16;
			const size_t bufferSize = elementCount * sizeof(TrackedWideChar);

			ReleaseFilledArray(elementCount);

			for (size_t i = bufferSize; i < bufferSize + GuardSize; i++)
			{
				std::wstring message = L"Guard byte " + std::to_wstring(i - bufferSize) + L" was overwritten";
				Assert::AreEqual(static_cast<int>(GuardPattern), static_cast<int>(releasedContents[i]), message.c_str());
			}
		}

	private:

		// Creates a SecureArrayT, fills every byte of its buffer with a non-zero
		// pattern, then lets it go out of scope so that its destructor runs.
		static void ReleaseFilledArray(const int elementCount)
		{
			const size_t bufferSize = elementCount * sizeof(TrackedWideChar);

			{
				SecureArrayT<TrackedWideChar> array(elementCount);

				Assert::IsTrue(array.get() == allocatedBlock, L"The array elements do not start at the allocated block");
				Assert::AreEqual(bufferSize, allocatedSize);

				memset(array.get(), FillPattern, bufferSize);
			}

			Assert::AreEqual(bufferSize + GuardSize, releasedContents.size(), L"The buffer was not released");
		}
	};
}
