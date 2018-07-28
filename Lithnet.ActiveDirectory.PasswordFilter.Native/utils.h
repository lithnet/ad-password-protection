#pragma once
#include "stdafx.h"

void odprintf(const wchar_t * format, ...);

void CheckPasswordSet(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);

void CheckPasswordChange(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);
