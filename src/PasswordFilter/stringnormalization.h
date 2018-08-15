#pragma once

static LPCWSTR CHARS_TO_TRIM = L"0123456789!@#$%^&*()[]{};:'\"<>,.?\\/+=_-~`";
static LPCWSTR CHARS_TO_REPLACE = L"$s5s0o4a3e@a^a(c6g1i7t8b2z!i";
static LPCWSTR CHARS_TO_DELETE = L"_.+";

LPWSTR ToLowerInvariant(const LPWSTR &s);

__declspec(dllexport) LPWSTR NormalizePassword(const LPWSTR &password);

void RemoveWhiteSpace(LPWSTR &s);

void RemoveChars(LPWSTR &s, const wchar_t *charsToRemove);

void ReplaceChars(LPWSTR &s, const wchar_t *charPairsToReplace);
