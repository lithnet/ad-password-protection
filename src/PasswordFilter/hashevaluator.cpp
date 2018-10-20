#include "stdafx.h"
#include "registry.h"
#include "hashevaluator.h"
#include <iomanip>
#include "SecureArrayT.h"
#include "utils.h"
#include "v3store.h"

registry reg;

bool IsPasswordInStore(const SecureArrayT<WCHAR> &password)
{
	std::wstring storePath = reg.GetRegValue(L"Store", L"");

	if (storePath.empty())
	{
		throw std::exception("Store path was null");
	}

	v3store v3s(storePath);
	return v3s.IsPasswordInStore(password);
}