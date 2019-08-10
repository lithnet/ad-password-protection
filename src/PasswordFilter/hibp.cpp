#include "stdafx.h"
#include "hibp.h"
#include <winhttp.h>
#include "SecureArrayT.h"
#include "hasher.h"
#include "utils.h"
#include "registry.h"
#include <algorithm>

bool IsInHibp(const SecureArrayT<WCHAR> &password, const registry &reg)
{
	if (password.getSize() <= 0)
	{
		return false;
	}

	SecureArrayT<BYTE> hashBytes = GetSha1HashBytes(password);

	if (hashBytes.getSize() != 20)
	{
		throw std::system_error(GetLastError(), std::system_category(), "GetSha1HashBytes returned an invalid value");
	}

	const std::wstring hashstring = ToHexWString(hashBytes.get(), hashBytes.get() + hashBytes.getSize());

	if (hashstring.length() != 40)
	{
		throw std::system_error(GetLastError(), std::system_category(), "ToHexWString returned an invalid value");
	}

	const std::wstring range = hashstring.substr(0, 5);
	const std::wstring matchtext = hashstring.substr(5, 35);

	const std::wstring hashes = GetHibpRangeData(range, reg);
	
	return hashes.find(matchtext + L":") != std::string::npos;
}

std::wstring GetHibpRangeData(const std::wstring &range, const registry &reg)
{
	const std::wstring path = L"range/" + range;

	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;

	try
	{
		hSession = WinHttpOpen(UserAgent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

		if (!hSession)
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpOpen failed");
		}
		
		hConnect = WinHttpConnect(hSession, reg.GetRegValue(REG_VALUE_HIBPHOSTNAME, DefaultHibpAddress).c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);

		if (!hConnect)
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpConnect failed");
		}

		hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

		if (!hRequest)
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpOpenRequest failed");
		}

		if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpSendRequest failed");
		}

		if (!WinHttpReceiveResponse(hRequest, NULL))
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpReceiveResponse failed");
		}

		DWORD dwStatusCode = 0;
		DWORD dwSize = sizeof(dwStatusCode);

		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);

		if (dwStatusCode != 200)
		{
			std::stringstream ss;
			ss << "The web request failed with HTTP status code ";
			ss << dwStatusCode;
			throw std::runtime_error(ss.str());
		}

		std::wstringstream data;

		dwSize = 0;
		DWORD dwDownloaded = 0;

		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				throw std::system_error(GetLastError(), std::system_category(), "WinHttpQueryDataAvailable failed");
			}

			const auto pszOutBuffer = new char[dwSize + 1];

			if (!pszOutBuffer)
			{
				throw std::system_error(GetLastError(), std::system_category(), "Out of memory");
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, static_cast<LPVOID>(pszOutBuffer), dwSize, &dwDownloaded))
			{
				delete[] pszOutBuffer;
				throw std::system_error(GetLastError(), std::system_category(), "WinHttpReadData failed");
			}

			data << pszOutBuffer;
			delete[] pszOutBuffer;

		} while (dwSize > 0 && dwDownloaded);

		if (hRequest)
		{
			WinHttpCloseHandle(hRequest);
		}

		if (hConnect)
		{
			WinHttpCloseHandle(hConnect);
		}

		if (hSession)
		{
			WinHttpCloseHandle(hSession);
		}

		return data.str();
	}
	catch (...)
	{
		if (hRequest)
		{
			WinHttpCloseHandle(hRequest);
		}

		if (hConnect)
		{
			WinHttpCloseHandle(hConnect);
		}

		if (hSession)
		{
			WinHttpCloseHandle(hSession);
		}

		throw;
	}
}
