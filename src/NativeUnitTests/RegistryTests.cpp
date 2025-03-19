#include "stdafx.h"
#include "CppUnitTest.h"
#include "registry.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeUnitTests
{
	TEST_CLASS(RegistryTests)
	{
	public:
		
		TEST_METHOD(TestGetRegValueString)
		{
			registry reg;
			std::wstring defaultValue = L"default";
			std::wstring value = reg.GetRegValue(L"NonExistentValue", defaultValue);
			Assert::AreEqual(defaultValue, value);
		}

		TEST_METHOD(TestGetRegValueDWORD)
		{
			registry reg;
			DWORD defaultValue = 1234;
			DWORD value = reg.GetRegValue(L"NonExistentValue", defaultValue);
			Assert::AreEqual(defaultValue, value);
		}

		TEST_METHOD(TestGetRegValueMultiString)
		{
			registry reg;
			std::vector<std::wstring> defaultValue = { L"default1", L"default2" };
			std::vector<std::wstring> value = reg.GetRegValue(L"NonExistentValue", REG_DEFAULT_MAX_ITEMS, defaultValue);
			Assert::IsTrue(defaultValue == value);
		}

		TEST_METHOD(TestGetRegistryForUser)
		{
			registry reg = registry::GetRegistryForUser(L"testuser");
			Assert::IsNotNull(&reg);
		}

		TEST_METHOD(TestSetAndGetRegValueString)
		{
			HKEY hKey;
			LPCWSTR subKey = REG_BASE_SETTINGS_KEY;
			LPCWSTR valueName = L"TestString";
			std::wstring setValue = L"TestValue";

			RegCreateKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
			RegSetValueEx(hKey, valueName, 0, REG_SZ, (const BYTE*)setValue.c_str(), (setValue.size() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			registry reg;
			std::wstring getValue = reg.GetRegValue(valueName, L"");
			Assert::AreEqual(setValue, getValue);
		}

		TEST_METHOD(TestSetAndGetRegValueDWORD)
		{
			HKEY hKey;
			LPCWSTR subKey = REG_BASE_SETTINGS_KEY;
			LPCWSTR valueName = L"TestDWORD";
			DWORD setValue = 5678;

			RegCreateKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
			RegSetValueEx(hKey, valueName, 0, REG_DWORD, (const BYTE*)&setValue, sizeof(setValue));
			RegCloseKey(hKey);

			registry reg;
			DWORD getValue = reg.GetRegValue(valueName, 0);
			Assert::AreEqual(setValue, getValue);
		}

		TEST_METHOD(TestSetAndGetRegValueMultiString)
		{
			HKEY hKey;
			LPCWSTR subKey = REG_BASE_SETTINGS_KEY;
			LPCWSTR valueName = L"TestMultiString";
			std::vector<std::wstring> setValue = { L"Value1", L"Value2" };
			std::wstring multiString;
			for (const auto& str : setValue)
			{
				multiString.append(str).append(1, L'\0');
			}
			multiString.append(1, L'\0');

			RegCreateKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
			RegSetValueEx(hKey, valueName, 0, REG_MULTI_SZ, (const BYTE*)multiString.c_str(), (multiString.size() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			registry reg;
			std::vector<std::wstring> getValue = reg.GetRegValue(valueName, REG_DEFAULT_MAX_ITEMS,  std::vector<std::wstring>());
            Assert::IsTrue(std::equal(setValue.begin(), setValue.end(), getValue.begin(), getValue.end()));
		}
	};
}
