#include "stdafx.h"
#include "CppUnitTest.h"
#include "passwordevaluator.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static void SetRegistryExcludedAccounts(const std::vector<std::wstring>& accounts)
{
    HKEY hKey;
    LONG result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_BASE_SETTINGS_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS)
    {
        // Calculate the size of the MULTI_SZ string
        size_t totalSize = 0;
        for (const auto& account : accounts)
        {
            totalSize += (account.size() + 1) * sizeof(wchar_t);
        }
        totalSize += sizeof(wchar_t); // For the final null terminator

        // Create the MULTI_SZ string
        std::vector<wchar_t> multiSz(totalSize / sizeof(wchar_t));
        wchar_t* ptr = multiSz.data();
        for (const auto& account : accounts)
        {
            wcscpy_s(ptr, account.size() + 1, account.c_str());
            ptr += account.size() + 1;
        }
        *ptr = L'\0'; // Final null terminator

        // Set the registry value
        RegSetValueEx(hKey, REG_VALUE_EXCLUDEDACCOUNTS, 0, REG_MULTI_SZ, (const BYTE*)multiSz.data(), totalSize);
        RegCloseKey(hKey);
    }
}

static void SetRegistryIncludedAccounts(const std::vector<std::wstring>& accounts)
{
    HKEY hKey;
    LONG result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, REG_BASE_SETTINGS_KEY, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS)
    {
        // Calculate the size of the MULTI_SZ string
        size_t totalSize = 0;
        for (const auto& account : accounts)
        {
            totalSize += (account.size() + 1) * sizeof(wchar_t);
        }
        totalSize += sizeof(wchar_t); // For the final null terminator

        // Create the MULTI_SZ string
        std::vector<wchar_t> multiSz(totalSize / sizeof(wchar_t));
        wchar_t* ptr = multiSz.data();
        for (const auto& account : accounts)
        {
            wcscpy_s(ptr, account.size() + 1, account.c_str());
            ptr += account.size() + 1;
        }
        *ptr = L'\0'; // Final null terminator

        // Set the registry value
        RegSetValueEx(hKey, REG_VALUE_INCLUDEDACCOUNTS, 0, REG_MULTI_SZ, (const BYTE*)multiSz.data(), totalSize);
        RegCloseKey(hKey);
    }
}

static void DeleteRegistryValues()
{
    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_BASE_SETTINGS_KEY, 0, KEY_WRITE, &hKey);
    if (result == ERROR_SUCCESS)
    {
        RegDeleteValue(hKey, REG_VALUE_EXCLUDEDACCOUNTS);
        RegDeleteValue(hKey, REG_VALUE_INCLUDEDACCOUNTS);
        RegCloseKey(hKey);
    }
}

namespace NativeUnitTests
{
    TEST_CLASS(PasswordEvaluatorInScopeTests)
    {
    public:

        TEST_METHOD_INITIALIZE(CleanupRegistry)
        {
            DeleteRegistryValues();
        }

        TEST_METHOD(TestIsUserInScope_ExcludedAccount)
        {
            std::wstring accountName = L"krbtgt";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsFalse(result);
        }

        TEST_METHOD(TestIsUserInScope_NotExcludedAccount)
        {
            std::wstring accountName = L"testuser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsTrue(result);
        }

        TEST_METHOD(TestIsUserInScope_ExcludedAccountFromRegistry)
        {
            std::vector<std::wstring> excludedAccounts = { L"excludeduser" };
            SetRegistryExcludedAccounts(excludedAccounts);

            std::wstring accountName = L"excludeduser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsFalse(result);
        }

        TEST_METHOD(TestIsUserInScope_MultipleExcludedAccountsFromRegistry)
        {
            std::vector<std::wstring> excludedAccounts = { L"excludeduser1", L"excludeduser2", L"excludeduser3" };
            SetRegistryExcludedAccounts(excludedAccounts);

            std::wstring accountName1 = L"excludeduser1";
            BOOLEAN result1 = IsUserInScope(accountName1);
            Assert::IsFalse(result1);

            std::wstring accountName2 = L"excludeduser2";
            BOOLEAN result2 = IsUserInScope(accountName2);
            Assert::IsFalse(result2);

            std::wstring accountName3 = L"excludeduser3";
            BOOLEAN result3 = IsUserInScope(accountName3);
            Assert::IsFalse(result3);

            std::wstring accountName4 = L"includeduser";
            BOOLEAN result4 = IsUserInScope(accountName4);
            Assert::IsTrue(result4);
        }

        TEST_METHOD(TestIsUserInScope_NotExcludedAccountFromRegistry)
        {
            std::vector<std::wstring> excludedAccounts = { L"excludeduser1", L"excludeduser2", L"excludeduser3" };
            SetRegistryExcludedAccounts(excludedAccounts);

            std::wstring accountName = L"includeduser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsTrue(result);
        }

        // New test cases for 'included user' logic

        TEST_METHOD(TestIsUserInScope_IncludedAccount)
        {
            std::vector<std::wstring> includedAccounts = { L"includeduser" };
            SetRegistryIncludedAccounts(includedAccounts);

            std::wstring accountName = L"includeduser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsTrue(result);
        }

        TEST_METHOD(TestIsUserInScope_IncludedAccountListEmpty)
        {
            std::vector<std::wstring> includedAccounts = {};
            SetRegistryIncludedAccounts(includedAccounts);

            std::wstring accountName = L"anyuser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsTrue(result);
        }

        TEST_METHOD(TestIsUserInScope_NotIncludedAccount)
        {
            std::vector<std::wstring> includedAccounts = { L"includeduser1", L"includeduser2" };
            SetRegistryIncludedAccounts(includedAccounts);

            std::wstring accountName = L"notincludeduser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsFalse(result);
        }

        TEST_METHOD(TestIsUserInScope_IncludedAndExcludedAccount)
        {
            std::vector<std::wstring> includedAccounts = { L"includeduser" };
            std::vector<std::wstring> excludedAccounts = { L"includeduser" };
            SetRegistryIncludedAccounts(includedAccounts);
            SetRegistryExcludedAccounts(excludedAccounts);

            std::wstring accountName = L"includeduser";
            BOOLEAN result = IsUserInScope(accountName);
            Assert::IsFalse(result);
        }
    };
}
