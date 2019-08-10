#pragma once
#include "registry.h"
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
	bool CheckPasswordNotInHibpOnChange;
	bool CheckPasswordNotInHibpOnSet;
};

struct general_policy
{
	int MinimumLength = 0;

	LPWSTR RegexApprove = 0;
	LPWSTR RegexReject = 0;

	bool ValidatePasswordDoesntContainAccountName = false;
	bool ValidatePasswordDoesntContainFullName = false;

	~general_policy()
	{
		OutputDebugString(L"Delete general policy");
		delete[] RegexApprove;
		delete[] RegexReject;
	}

	general_policy()
	{
		OutputDebugString(L"Create general policy");
	}

	general_policy(const general_policy&) = delete; // Copy constructor
	general_policy& operator=(const general_policy&) = delete; // Copy assignment constructor
	general_policy(general_policy&&) = delete; // Move constructor
	general_policy& operator=(general_policy&&) = delete; // Move assignment operator
};

struct user_policy
{
	int Version = 1;
	general_policy GeneralPolicy{};
	store_policy StorePolicy{};
	complexity_points ComplexityPoints{};
	complexity_threshold ComplexityThreshold1{};
	complexity_threshold ComplexityThreshold2{};
	complexity_threshold ComplexityThreshold3{};

	user_policy()
	{
		OutputDebugString(L"Create user policy");
	}


	~user_policy()
	{
		OutputDebugString(L"Delete user policy");
	}

	user_policy(const user_policy&) = delete; // Copy constructor
	user_policy& operator=(const user_policy&) = delete; // Copy assignment constructor
	user_policy(user_policy&&) = delete; // Move constructor
	user_policy& operator=(user_policy&&) = delete; // Move assignment operator
};

class policy
{
public:
	/*static user_policy& GetPolicySetForUser(const std::wstring &accountName);
	static user_policy& GetPolicySetByName(const std::wstring &policySetName);*/
	static std::wstring GetPolicySetNameForUser(const std::wstring &accountName);
	static std::wstring GetPolicySetNameForUser(const std::wstring &accountName, const std::wstring baseKey);
	static std::vector<PolicySetMap> GetActivePolicySetMap();
	static std::vector<PolicySetMap> GetActivePolicySetMap(const std::wstring baseKey);
	static void PopulatePolicySetObject(const registry &reg, user_policy *policy);
	static void PopulatePolicySetObject(const std::wstring &policySetName, user_policy *policy);

	policy() = default;
};
