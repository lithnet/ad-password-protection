#include "stdafx.h"
#include "utils.h"
#include <vector>
#include "SecureArrayT.h"
#include <atlalloc.h>

PSID ConvertNameToBinarySid(const std::wstring &accountName)
{
	LPTSTR szDomainName = NULL;
	PSID pSid = NULL;

	try
	{
		DWORD cbDomainName = 0;
		DWORD cbSid = 0;
		SID_NAME_USE sidType;

		const BOOL res = LookupAccountName(NULL, accountName.c_str(), pSid, &cbSid, szDomainName, &cbDomainName, &sidType);
		const DWORD result = GetLastError();

		if (result != ERROR_INSUFFICIENT_BUFFER)
		{
			throw std::system_error(result, std::system_category(), "LookupAccountName failed");
		}

		pSid = LocalAlloc(LPTR, cbSid);
		szDomainName = static_cast<LPTSTR>(LocalAlloc(LPTR, cbDomainName * sizeof(TCHAR)));

		if (szDomainName == NULL || pSid == NULL)
		{
			throw std::system_error(GetLastError(), std::system_category(), "Out of memory");
		}

		if (!LookupAccountName(NULL, accountName.c_str(), pSid, &cbSid, szDomainName, &cbDomainName, &sidType))
		{
			throw std::system_error(result, std::system_category(), "LookupAccountName failed");
		}

		if (szDomainName != NULL)
		{
			LocalFree(szDomainName);
		}

		return pSid;
	}
	catch (...)
	{
		if (szDomainName != NULL)
		{
			LocalFree(szDomainName);
		}

		if (pSid != NULL)
		{
			LocalFree(pSid);
		}

		throw;
	}
}

std::vector<std::wstring> SplitString(const std::wstring &text, const wchar_t sep)
{
	std::vector<std::wstring> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::wstring::npos) {
		if (end != start) {
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(text.substr(start));
	}

	return tokens;
}

SecureArrayT<WCHAR> StringToWcharArray(const LPCWSTR str)
{
	const int charCount = wcslen(str);
	const int len = charCount + 1;

	SecureArrayT<WCHAR> ar(len);

	wcsncpy_s(ar.get(), len, str, charCount);

	return ar;
}

SecureArrayT<WCHAR> UnicodeStringToWcharArray(const UNICODE_STRING& str)
{
	const int charCount = str.Length / sizeof(wchar_t);
	const int len = charCount + 1;

	SecureArrayT<WCHAR> ar(len);

	wcsncpy_s(ar.get(), len, str.Buffer, charCount);

	return ar;
}

bool DirectoryExists(const std::wstring& dirName)
{
	const DWORD attributes = GetFileAttributes(dirName.c_str());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}

	return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

std::wstring& ReplaceToken(std::wstring& sourceString, const std::wstring& token, const std::wstring& replacementValue)
{
	if (!token.empty())
	{
		for (size_t pos = 0; (pos = sourceString.find(token, pos)) != std::string::npos; pos += replacementValue.size())
		{
			sourceString.replace(pos, token.size(), replacementValue);
		}
	}

	return sourceString;
}