#include "stdafx.h"
#include "hibp.h"
#include <winhttp.h>
#include "SecureArrayT.h"
#include "hasher.h"
#include "utils.h"
#include "registry.h"
#include <algorithm>

bool IsInSha1HibpApi(const SecureArrayT<WCHAR> &password)
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

	registry reg;

	const std::wstring host = reg.GetRegValue(REG_VALUE_HIBPHOSTNAME, DefaultHibpAddress);
	const unsigned short port = reg.GetRegValue(REG_VALUE_HIBPPORT, INTERNET_DEFAULT_HTTPS_PORT);
	std::wstring path = reg.GetRegValue(REG_VALUE_HIBPSHA1PATH, L"range/{range}");
	path = ReplaceToken(path, L"{range}", range);

	const std::wstring hashes = GetHttpResponse(host, port, path);

	return IsInVariableWidthRangeData(hashes, matchtext);
}

bool IsInVariableWidthRangeData(const std::wstring &rangeData, const std::wstring &value)
{
	size_t startPos = 0, currentPos = 0;
	auto lastPos = rangeData.length();
	const auto match_length = value.length();
	int count = 0;

	while (startPos <= lastPos)
	{
		currentPos = (startPos + lastPos) / 2;
		count++;

		while (currentPos > startPos && rangeData.at(currentPos - 1) != '\n')
		{
			currentPos--;
		}

		const int result = rangeData.compare(currentPos, match_length, value, 0, match_length);

		if (result < 0)
		{
			startPos = currentPos + match_length;
		}
		else if (result > 0)
		{
			lastPos = currentPos - 1;
		}
		else
		{
			OutputDebugString(L"Hash found at position " + std::to_wstring(currentPos) + L" on loop " + std::to_wstring(count));
			return true;
		}
	}

	OutputDebugString(std::wstring(L"Hash not found after " + std::to_wstring(count) + L" loops"));
	return false;
}

std::wstring GetHttpResponse(const std::wstring &host, const INTERNET_PORT port, const std::wstring &path)
{
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

		hConnect = WinHttpConnect(hSession, host.c_str(), port, 0);

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
			const std::string message("The web request failed with HTTP status code" + std::to_string(dwStatusCode));
			throw std::runtime_error(message);
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
