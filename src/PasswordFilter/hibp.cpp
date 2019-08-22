#include "stdafx.h"
#include "hibp.h"
#include <winhttp.h>
#include "SecureArrayT.h"
#include "hasher.h"
#include "utils.h"
#include "registry.h"
#include <locale>

int IsInHibpNtlmHexApi(const SecureArrayT<WCHAR> &password, const registry &reg)
{
	if (password.getSize() <= 0)
	{
		return 0;
	}

	SecureArrayT<BYTE> hashBytes = GetNtlmHashBytes(password);

	if (hashBytes.getSize() != 16)
	{
		throw std::system_error(GetLastError(), std::system_category(), "GetNtlmHashBytes returned an invalid value");
	}

	const std::wstring hashstring = ToHexWString(hashBytes.get(), hashBytes.get() + hashBytes.getSize());

	if (hashstring.length() != 32)
	{
		throw std::system_error(GetLastError(), std::system_category(), "ToHexWString returned an invalid value");
	}

	const std::wstring range = hashstring.substr(0, 5);
	const std::wstring matchtext = hashstring.substr(5, 27);

	std::wstring apiurl = reg.GetRegValue(REG_VALUE_HIBPNTLMHEXAPIURL, DefaultHibpNtlmHexApiUrl);
	apiurl = ReplaceToken(apiurl, L"{range}", range);

	auto responseData = GetHttpResponseString(apiurl);
	const auto hashes = responseData.str();

	return IsInVariableWidthRangeData(hashes, matchtext);
}

int IsInHibpNtlmBinaryApi(const SecureArrayT<WCHAR> &password, const registry &reg)
{
	if (password.getSize() <= 0)
	{
		return 0;
	}

	SecureArrayT<BYTE> hashBytes = GetNtlmHashBytes(password);

	if (hashBytes.getSize() != 16)
	{
		throw std::system_error(GetLastError(), std::system_category(), "GetNtlmHashBytes returned an invalid value");
	}

	const std::wstring hashstring = ToHexWString(hashBytes.get(), hashBytes.get() + hashBytes.getSize());

	if (hashstring.length() != 32)
	{
		throw std::system_error(GetLastError(), std::system_category(), "ToHexWString returned an invalid value");
	}

	const std::wstring range = hashstring.substr(0, 5);
	SecureArrayT<BYTE> matchBytes(14);
	memcpy_s(matchBytes.get(), matchBytes.getSize(), hashBytes.get() + 2, 14);

	std::wstring apiurl = reg.GetRegValue(REG_VALUE_HIBPNTLMBINARYAPIURL, DefaultHibpNtlmBinaryApiUrl);
	apiurl = ReplaceToken(apiurl, L"{range}", range);

	auto responseData = GetHttpResponseBinary(apiurl);

	return IsInFixedWidthRangeData(responseData, matchBytes, 14);
}

int IsInHibpNtlmBinaryPwnCountApi(const SecureArrayT<WCHAR> &password, const registry &reg)
{
	if (password.getSize() <= 0)
	{
		return 0;
	}

	SecureArrayT<BYTE> hashBytes = GetNtlmHashBytes(password);

	if (hashBytes.getSize() != 16)
	{
		throw std::system_error(GetLastError(), std::system_category(), "GetNtlmHashBytes returned an invalid value");
	}

	const std::wstring hashstring = ToHexWString(hashBytes.get(), hashBytes.get() + hashBytes.getSize());

	if (hashstring.length() != 32)
	{
		throw std::system_error(GetLastError(), std::system_category(), "ToHexWString returned an invalid value");
	}

	const std::wstring range = hashstring.substr(0, 5);
	SecureArrayT<BYTE> matchBytes(14);
	memcpy_s(matchBytes.get(), matchBytes.getSize(), hashBytes.get() + 2, 14);

	std::wstring apiurl = reg.GetRegValue(REG_VALUE_HIBPNTLMBINARYPWNCOUNTAPIURL, DefaultHibpNtlmBinaryPwnCountApiUrl);
	apiurl = ReplaceToken(apiurl, L"{range}", range);

	auto responseData = GetHttpResponseBinary(apiurl);

	return IsInFixedWidthRangeData(responseData, matchBytes, 16);
}

int IsInHibpSha1Api(const SecureArrayT<WCHAR> &password, const registry &reg)
{
	if (password.getSize() <= 0)
	{
		return 0;
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

	std::wstring apiurl = reg.GetRegValue(REG_VALUE_HIBPSHA1APIURL, DefaultHibpSha1ApiUrl);
	apiurl = ReplaceToken(apiurl, L"{range}", range);

	auto responseData = GetHttpResponseString(apiurl);
	const auto hashes = responseData.str();

	return IsInVariableWidthRangeData(hashes, matchtext);
}

void CrackUrl(const std::wstring &url, URL_COMPONENTS &urlComponents)
{
	ZeroMemory(&urlComponents, sizeof(urlComponents));

	urlComponents.dwStructSize = sizeof(urlComponents);
	urlComponents.dwSchemeLength = -1;
	urlComponents.dwHostNameLength = -1;
	urlComponents.dwUrlPathLength = -1;
	urlComponents.dwExtraInfoLength = -1;

	if (!WinHttpCrackUrl(url.c_str(), 0, 0, &urlComponents))
	{
		throw std::system_error(GetLastError(), std::system_category(), "WinHttpCrackUrl failed");
	}
}

int IsInVariableWidthRangeData(const std::wstring &rangeData, const std::wstring &value)
{
	long long startPos = 0, currentPos = 0;
	long long lastPos = rangeData.length();
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

		if (result == 0)
		{
			const auto eol = rangeData.find(L'\r', currentPos);
			const auto startPwnCount = currentPos + match_length + 1;
			const auto pwnCountLength = eol - startPwnCount;
			const auto pwnCount = rangeData.substr(startPwnCount, pwnCountLength);
			OutputDebugString(L"Hash found at position " + std::to_wstring(currentPos) + L" on loop " + std::to_wstring(count) + L" with a pwn count of " + pwnCount + L"\n");

			return std::stoi(pwnCount);
		}

		if (result < 0)
		{
			startPos = currentPos + match_length;
		}
		else if (result > 0)
		{
			if (currentPos > 0)
			{
				lastPos = currentPos - 1;
			}
			else
			{
				break;
			}
		}
	}

	OutputDebugString(std::wstring(L"Hash not found after " + std::to_wstring(count) + L" loops"));
	return 0;
}

int IsInFixedWidthRangeData(std::vector<BYTE>& rangeData, const SecureArrayT<BYTE> &hashBytes, const size_t& record_size)
{
	size_t firstRow = 0, currentRow = 0;
	const size_t length = rangeData.size();
	size_t lastRow = length / record_size;
	const size_t match_length = hashBytes.getSize();

	int count = 0;

	while (firstRow <= lastRow)
	{
		currentRow = (firstRow + lastRow) / 2;
		count++;

		const auto currentPos = rangeData.data() + (currentRow * record_size);

		const int result = memcmp(currentPos, hashBytes.get(), match_length);

		if (result == 0)
		{
			const size_t pwnCountLength = record_size - hashBytes.getSize();
			int pwnCount = 0;

			if (pwnCountLength > 0)
			{
				const size_t pwnCountPos = (currentRow * record_size) + hashBytes.getSize();

				for (size_t i = 0; i < pwnCountLength; i++)
				{
					pwnCount |= rangeData.at(pwnCountPos + i) << (i * 8);
				}

				if (pwnCount == 0)
				{
					pwnCount = 1;
				}
			}
			else
			{
				pwnCount = 1;
			}

			OutputDebugString(L"Hash found at row " + std::to_wstring(currentRow) + L" on loop " + std::to_wstring(count) + L" with pwn count " + std::to_wstring(pwnCount) + L"\n");

			return pwnCount;
		}

		if (result < 0)
		{
			firstRow = currentRow + 1;
		}
		else if (result > 0)
		{
			if (currentRow > 0)
			{
				lastRow = currentRow - 1;
			}
			else
			{
				break;
			}
		}
	}

	OutputDebugString(std::wstring(L"Hash not found after " + std::to_wstring(count) + L" loops" + L"\n"));
	return 0;
}

//std::wstring GetHttpResponse(const std::wstring &host, const INTERNET_PORT port, const std::wstring &path, const bool usetls)
//{
//	HINTERNET hSession = NULL;
//	HINTERNET hConnect = NULL;
//	HINTERNET hRequest = NULL;
//
//	try
//	{
//		hSession = WinHttpOpen(UserAgent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
//
//		if (!hSession)
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "WinHttpOpen failed");
//		}
//
//		hConnect = WinHttpConnect(hSession, host.c_str(), port, 0);
//
//		if (!hConnect)
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "WinHttpConnect failed");
//		}
//
//		hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, usetls ? WINHTTP_FLAG_SECURE : 0);
//
//		if (!hRequest)
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "WinHttpOpenRequest failed");
//		}
//
//		if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "WinHttpSendRequest failed");
//		}
//
//		if (!WinHttpReceiveResponse(hRequest, NULL))
//		{
//			throw std::system_error(GetLastError(), std::system_category(), "WinHttpReceiveResponse failed");
//		}
//
//		DWORD dwStatusCode = 0;
//		DWORD dwSize = sizeof(dwStatusCode);
//
//		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);
//
//		if (dwStatusCode != 200)
//		{
//			const std::string message("The web request failed with HTTP status code" + std::to_string(dwStatusCode));
//			throw std::runtime_error(message);
//		}
//
//		std::wstringstream data;
//
//		dwSize = 0;
//		DWORD dwDownloaded = 0;
//
//		do
//		{
//			dwSize = 0;
//			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
//			{
//				throw std::system_error(GetLastError(), std::system_category(), "WinHttpQueryDataAvailable failed");
//			}
//
//			const auto pszOutBuffer = new char[dwSize + 1];
//
//			if (!pszOutBuffer)
//			{
//				throw std::system_error(GetLastError(), std::system_category(), "Out of memory");
//			}
//
//			ZeroMemory(pszOutBuffer, dwSize + 1);
//
//			if (!WinHttpReadData(hRequest, static_cast<LPVOID>(pszOutBuffer), dwSize, &dwDownloaded))
//			{
//				delete[] pszOutBuffer;
//				throw std::system_error(GetLastError(), std::system_category(), "WinHttpReadData failed");
//			}
//
//			data << pszOutBuffer;
//			delete[] pszOutBuffer;
//
//		} while (dwSize > 0 && dwDownloaded);
//
//		if (hRequest)
//		{
//			WinHttpCloseHandle(hRequest);
//		}
//
//		if (hConnect)
//		{
//			WinHttpCloseHandle(hConnect);
//		}
//
//		if (hSession)
//		{
//			WinHttpCloseHandle(hSession);
//		}
//
//		return data.str();
//	}
//	catch (...)
//	{
//		if (hRequest)
//		{
//			WinHttpCloseHandle(hRequest);
//		}
//
//		if (hConnect)
//		{
//			WinHttpCloseHandle(hConnect);
//		}
//
//		if (hSession)
//		{
//			WinHttpCloseHandle(hSession);
//		}
//
//		throw;
//	}
//}

std::wstringstream GetHttpResponseString(const std::wstring &url)
{
	auto response = GetHttpResponseBinary(url);
	std::wstringstream ss;

	for each (char b in response)
	{
		ss << b;
	}

	return ss;
}

std::vector<BYTE> GetHttpResponseBinary(const std::wstring &url)
{
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	try
	{
		const wchar_t *acceptTypes[] = { L"*/*", NULL };

		URL_COMPONENTS urlComponents;
		CrackUrl(url, urlComponents);

		const std::wstring host(urlComponents.lpszHostName, urlComponents.dwHostNameLength);
		std::wstring path(urlComponents.lpszUrlPath, urlComponents.dwUrlPathLength);
		if (urlComponents.dwExtraInfoLength > 0)
		{
			path.append(urlComponents.lpszExtraInfo, urlComponents.dwExtraInfoLength);
		}

		hSession = WinHttpOpen(UserAgent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

		if (!hSession)
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpOpen failed");
		}

		hConnect = WinHttpConnect(hSession, host.c_str(), urlComponents.nPort, 0);

		if (!hConnect)
		{
			throw std::system_error(GetLastError(), std::system_category(), "WinHttpConnect failed");
		}

		hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER, acceptTypes, urlComponents.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);

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
			const std::string message("The web request failed with HTTP status code " + std::to_string(dwStatusCode));
			throw std::runtime_error(message);
		}

		std::vector<BYTE> data;

		dwSize = 0;
		DWORD dwDownloaded = 0;

		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				throw std::system_error(GetLastError(), std::system_category(), "WinHttpQueryDataAvailable failed");
			}

			std::unique_ptr<unsigned char[]> pszOutBuffer(new unsigned char[dwSize]);

			if (!pszOutBuffer)
			{
				throw std::system_error(GetLastError(), std::system_category(), "Out of memory");
			}

			ZeroMemory(pszOutBuffer.get(), dwSize);

			if (!WinHttpReadData(hRequest, static_cast<LPVOID>(pszOutBuffer.get()), dwSize, &dwDownloaded))
			{
				throw std::system_error(GetLastError(), std::system_category(), "WinHttpReadData failed");
			}

			data.insert(data.end(), pszOutBuffer.get(), pszOutBuffer.get() + dwSize);

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

		return data;
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