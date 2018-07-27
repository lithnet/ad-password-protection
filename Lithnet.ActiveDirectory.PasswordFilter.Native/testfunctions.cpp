#include "stdafx.h"
#include "winternl.h"

static PUNICODE_STRING ToUniCodeString(WCHAR *text)
{
	PUNICODE_STRING username;
	RtlInitUnicodeString(username, text);

	return username;
}