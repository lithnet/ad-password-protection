#pragma once
#include "SecureArrayT.h"

static LPCWSTR CHARS_TO_TRIM = L"0123456789!@#$%^&*()[]{};:'\"<>,.?\\/+=_-~`";
static LPCWSTR CHARS_TO_REPLACE = L"$s5s0o4a3e@a^a(c6g1i7t8b2z!i";
static LPCWSTR CHARS_TO_DELETE = L"_.+";

SecureArrayT<WCHAR> ToLowerInvariant(const SecureArrayT<WCHAR> &s);

SecureArrayT<WCHAR> NormalizePassword(const SecureArrayT<WCHAR> &password);

void RemoveWhiteSpace(SecureArrayT<WCHAR> &s);

void RemoveChars(SecureArrayT<WCHAR> &s, const WCHAR *charsToRemove);

void ReplaceChars(SecureArrayT<WCHAR> &s, const WCHAR *charPairsToReplace);
