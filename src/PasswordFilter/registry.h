#pragma once
#include "stdafx.h"

DWORD GetPolicyOrSettingsValue(HKEY hKeyPolicy, HKEY hKeySettings, const std::wstring & strValueName, DWORD defaultValue);

const std::wstring GetPolicyOrSettingsValue(HKEY hKeyPolicy, HKEY hKeySettings, const std::wstring & strValueName, const std::wstring & defaultValue);

const std::wstring GetValueString(DWORD & dwBufferSize, const HKEY & hKeyPolicy, const std::wstring & strValueName, const std::wstring & defaultValue);

std::wstring GetRegValue(const std::wstring & valueName, const std::wstring & defaultValue);

DWORD GetRegValue(const std::wstring & valueName, DWORD defaultValue);

HKEY OpenSettingsKey();

HKEY OpenPolicyKey();
