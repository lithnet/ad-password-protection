#include "stdafx.h"
#include "CppUnitTest.h"
#include <new>
#include "../PasswordFilter/utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	// Spies on the COM task allocator for the thread that created it. Each allocation
	// made on that thread is extended by a guard region that is filled with a known
	// pattern. A write past the requested size lands in the guard region rather than
	// in the heap, so an overflow is detected deterministically in every build
	// configuration, without the debug heap. The spy can also force allocations to fail.
	class GuardMallocSpy : public IMallocSpy
	{
	public:
		static const SIZE_T GuardSize = 64;
		static const BYTE GuardPattern = 0xFD;

		GuardMallocSpy()
			: refCount(1), ownerThreadId(GetCurrentThreadId()), failAllocations(false), pendingRequest(0), lastRequest(0), lastAllocation(NULL)
		{
		}

		void SetFailAllocations(bool value)
		{
			this->failAllocations = value;
		}

		SIZE_T GetLastRequest() const
		{
			return this->lastRequest;
		}

		void* GetLastAllocation() const
		{
			return this->lastAllocation;
		}

		bool IsGuardIntact() const
		{
			const BYTE* guard = static_cast<const BYTE*>(this->lastAllocation) + this->lastRequest;

			for (SIZE_T i = 0; i < GuardSize; i++)
			{
				if (guard[i] != GuardPattern)
				{
					return false;
				}
			}

			return true;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
		{
			if (ppvObject == NULL)
			{
				return E_POINTER;
			}

			if (riid == IID_IUnknown || riid == IID_IMallocSpy)
			{
				*ppvObject = static_cast<IMallocSpy*>(this);
				this->AddRef();
				return S_OK;
			}

			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef() override
		{
			return InterlockedIncrement(&this->refCount);
		}

		ULONG STDMETHODCALLTYPE Release() override
		{
			const ULONG count = InterlockedDecrement(&this->refCount);

			if (count == 0)
			{
				delete this;
			}

			return count;
		}

		SIZE_T STDMETHODCALLTYPE PreAlloc(SIZE_T cbRequest) override
		{
			if (GetCurrentThreadId() != this->ownerThreadId)
			{
				return cbRequest;
			}

			if (this->failAllocations)
			{
				return 0;
			}

			this->pendingRequest = cbRequest;
			return cbRequest + GuardSize;
		}

		void* STDMETHODCALLTYPE PostAlloc(void* pActual) override
		{
			if (GetCurrentThreadId() != this->ownerThreadId || pActual == NULL)
			{
				return pActual;
			}

			memset(static_cast<BYTE*>(pActual) + this->pendingRequest, GuardPattern, GuardSize);
			this->lastRequest = this->pendingRequest;
			this->lastAllocation = pActual;

			return pActual;
		}

		void* STDMETHODCALLTYPE PreFree(void* pRequest, BOOL fSpyed) override
		{
			return pRequest;
		}

		void STDMETHODCALLTYPE PostFree(BOOL fSpyed) override
		{
		}

		SIZE_T STDMETHODCALLTYPE PreRealloc(void* pRequest, SIZE_T cbRequest, void** ppNewRequest, BOOL fSpyed) override
		{
			*ppNewRequest = pRequest;
			return cbRequest;
		}

		void* STDMETHODCALLTYPE PostRealloc(void* pActual, BOOL fSpyed) override
		{
			return pActual;
		}

		void* STDMETHODCALLTYPE PreGetSize(void* pRequest, BOOL fSpyed) override
		{
			return pRequest;
		}

		SIZE_T STDMETHODCALLTYPE PostGetSize(SIZE_T cbActual, BOOL fSpyed) override
		{
			return cbActual;
		}

		void* STDMETHODCALLTYPE PreDidAlloc(void* pRequest, BOOL fSpyed) override
		{
			return pRequest;
		}

		int STDMETHODCALLTYPE PostDidAlloc(void* pRequest, BOOL fSpyed, int fActual) override
		{
			return fActual;
		}

		void STDMETHODCALLTYPE PreHeapMinimize() override
		{
		}

		void STDMETHODCALLTYPE PostHeapMinimize() override
		{
		}

	private:
		~GuardMallocSpy()
		{
		}

		volatile ULONG refCount;
		const DWORD ownerThreadId;
		bool failAllocations;
		SIZE_T pendingRequest;
		SIZE_T lastRequest;
		void* lastAllocation;
	};

	TEST_CLASS(InteropStringTests)
	{
	public:
		GuardMallocSpy* spy = NULL;

		TEST_METHOD_INITIALIZE(Init)
		{
			this->spy = new GuardMallocSpy();
			Assert::AreEqual(S_OK, CoRegisterMallocSpy(this->spy), L"CoRegisterMallocSpy failed");
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			// E_ACCESSDENIED means the revoke is deferred until allocations made while the
			// spy was active are freed. COM releases its reference to the spy at that point.
			const HRESULT hr = CoRevokeMallocSpy();
			this->spy->Release();
			this->spy = NULL;

			Assert::IsTrue(hr == S_OK || hr == E_ACCESSDENIED, L"CoRevokeMallocSpy failed");
		}

		void AssertInteropStringCopiedWithinAllocation(LPCWSTR value)
		{
			LPCWSTR result = GetInteropString(value);

			Assert::IsTrue(result == this->spy->GetLastAllocation(), L"The returned string was not allocated with CoTaskMemAlloc on the test thread");

			const bool guardIntact = this->spy->IsGuardIntact();
			const std::wstring copied(result);
			const size_t requiredBytes = (wcslen(value) + 1) * sizeof(wchar_t);
			const std::wstring message = L"Allocated " + std::to_wstring(this->spy->GetLastRequest()) + L" bytes but the string requires " + std::to_wstring(requiredBytes) + L" bytes";

			CoTaskMemFree((LPVOID)result);

			Assert::IsTrue(guardIntact, message.c_str());
			Assert::AreEqual(value, copied.c_str());
		}

		TEST_METHOD(GetInteropStringEmptyValue)
		{
			AssertInteropStringCopiedWithinAllocation(L"");
		}

		TEST_METHOD(GetInteropStringSingleCharacter)
		{
			AssertInteropStringCopiedWithinAllocation(L"a");
		}

		TEST_METHOD(GetInteropStringRegexValue)
		{
			AssertInteropStringCopiedWithinAllocation(L"^(?=.*[A-Z])(?=.*[0-9]).{12,}$");
		}

		TEST_METHOD(GetInteropStringThrowsWhenAllocationFails)
		{
			this->spy->SetFailAllocations(true);

			Assert::ExpectException<std::bad_alloc>([] { GetInteropString(L"abc"); }, L"GetInteropString did not throw std::bad_alloc when CoTaskMemAlloc returned NULL");
		}
	};
}
