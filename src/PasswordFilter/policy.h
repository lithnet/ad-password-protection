#pragma once
#include "registry.h";
#include "policysetmap.h"

struct complexity_threshold
{
	int ComplexityThreshold;
	int ComplexityThresholdCharsetsRequired;
	bool ComplexityThresholdRequiresLower;
	bool ComplexityThresholdRequiresNumber;
	bool ComplexityThresholdRequiresSymbol;
	bool ComplexityThresholdRequiresSymbolOrNumber;
	bool ComplexityThresholdRequiresUpper;
};

struct complexity_points
{
	int PerCharacter;
	int PerLower;
	int PerNumber;
	int PerSymbol;
	int PerUpper;
	int Required;
	int UseOfLower;
	int UseOfNumber;
	int UseOfSymbol;
	int UseOfUpper;
};

struct store_policy
{
	bool CheckNormalizedPasswordNotInCompromisedPasswordStoreOnChange;
	bool CheckNormalizedPasswordNotInCompromisedPasswordStoreOnSet;
	bool CheckNormalizedPasswordNotInBannedWordStoreOnChange;
	bool CheckNormalizedPasswordNotInBannedWordStoreOnSet;
	bool CheckPasswordNotInCompromisedPasswordStoreOnChange;
	bool CheckPasswordNotInCompromisedPasswordStoreOnSet;
};

struct general_policy
{
	int MinimumLength;

	LPCWSTR RegexApprove;
	LPCWSTR RegexReject;

	bool ValidatePasswordDoesntContainAccountName;
	bool ValidatePasswordDoesntContainFullName;
};

struct user_policy
{
	general_policy GeneralPolicy;
	store_policy StorePolicy;
	complexity_points ComplexityPoints;
	complexity_threshold ComplexityThreshold1;
	complexity_threshold ComplexityThreshold2;
	complexity_threshold ComplexityThreshold3;
};

class policy
{
public:
	static user_policy GetPolicySetForUser(const std::wstring &accountName);
	static user_policy GetPolicySetByName(const std::wstring &policySetName);
	static std::wstring GetPolicySetNameForUser(const std::wstring &accountName);
	static std::wstring GetPolicySetNameForUser(const std::wstring &accountName, const std::wstring baseKey);
	static std::vector<PolicySetMap> GetActivePolicySetMap();
	static std::vector<PolicySetMap> GetActivePolicySetMap(const std::wstring baseKey);

	policy() = default;
};
