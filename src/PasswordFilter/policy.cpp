#include "stdafx.h"
#include "policy.h"
#include "registry.h"
#include "utils.h"

user_policy policy::getpolicy(LPCWSTR account)
{
	registry reg = registry::GetRegistryForUser(account);

	user_policy policy{};

	policy.ComplexityPoints.PerCharacter = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERCHAR, 0);
	policy.ComplexityPoints.PerLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERLOWER, 0);
	policy.ComplexityPoints.PerNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERNUMBER, 0);
	policy.ComplexityPoints.PerSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERSYMBOL, 0);
	policy.ComplexityPoints.PerUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSPERUPPER, 0);
	policy.ComplexityPoints.Required = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSREQUIRED, 0);
	policy.ComplexityPoints.UseOfLower = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFLOWER, 0);
	policy.ComplexityPoints.UseOfNumber = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFNUMBER, 0);
	policy.ComplexityPoints.UseOfSymbol = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFSYMBOL, 0);
	policy.ComplexityPoints.UseOfUpper = reg.GetRegValue(REG_VALUE_COMPLEXITYPOINTSUSEOFUPPER, 0);

	policy.ComplexityThreshold1.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT1, 0);
	policy.ComplexityThreshold1.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT1CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold1.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT1REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT1REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold1.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT1REQUIRESUPPER, 0) != 0;

	policy.ComplexityThreshold2.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT2, 0);
	policy.ComplexityThreshold2.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT2CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold2.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT2REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT2REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold2.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT2REQUIRESUPPER, 0) != 0;

	policy.ComplexityThreshold3.ComplexityThreshold = reg.GetRegValue(REG_VALUE_CT3, 0);
	policy.ComplexityThreshold3.ComplexityThresholdCharsetsRequired = reg.GetRegValue(REG_VALUE_CT3CHARSETSREQUIRED, 0);
	policy.ComplexityThreshold3.ComplexityThresholdRequiresLower = reg.GetRegValue(REG_VALUE_CT3REQUIRESLOWER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESNUMBER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresSymbol = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOL, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresSymbolOrNumber = reg.GetRegValue(REG_VALUE_CT3REQUIRESSYMBOLORNUMBER, 0) != 0;
	policy.ComplexityThreshold3.ComplexityThresholdRequiresUpper = reg.GetRegValue(REG_VALUE_CT3REQUIRESUPPER, 0) != 0;

	policy.GeneralPolicy.MinimumLength = reg.GetRegValue(REG_VALUE_MINIMUMLENGTH, 0);
	policy.GeneralPolicy.ValidatePasswordDoesntContainAccountName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINACCOUNTNAME, 0) != 0;
	policy.GeneralPolicy.ValidatePasswordDoesntContainFullName = reg.GetRegValue(REG_VALUE_PASSWORDDOESNTCONTAINFULLNAME, 0) != 0;
		
	//policy.GeneralPolicy.RegexApprove = reg.GetRegValue(REG_VALUE_REGEXAPPROVE, L"").c_str();
	//policy.GeneralPolicy.RegexReject = reg.GetRegValue(REG_VALUE_REGEXREJECT, L"").c_str();


	return policy;
}

policy::policy()
{
}

policy::~policy()
{
}