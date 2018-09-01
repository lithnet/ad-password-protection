#include "stdafx.h"
#include "hasher.h"
#include <fstream>
#include "registry.h"
#include "hashevaluator.h"
#include "shlwapi.h"
#include <sstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "esestore.h"
#include "v1store.h"
#include "v2store.h"
#include "SecureArrayT.h"
#include "utils.h"

registry reg;


bool IsPasswordInStore(const SecureArrayT<WCHAR> &password)
{
	int hashCheckMode = reg.GetRegValue(L"HashCheckMode", 2);
	std::wstring storePath = reg.GetRegValue(L"Store", L"");

	if (storePath.empty())
	{
		throw std::exception("Store path was null");
	}

	if (hashCheckMode == 0)
	{
		SecureArrayT<BYTE> hash = GetSha1HashBytes(password);
		return esestore::getInstance().IsHashInDb(hash);
	}
	else if (hashCheckMode == 1)
	{
		v1store v1s(storePath);
		return v1s.IsPasswordInStore(password);
	}
	else if (hashCheckMode == 2)
	{
		v2store v2s(storePath);
		return v2s.IsPasswordInStore(password);
	}
}