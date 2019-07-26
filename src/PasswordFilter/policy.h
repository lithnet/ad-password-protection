#pragma once

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
	static user_policy GetPolicyForUser(const std::wstring &account);
	static user_policy GetPolicyForGroup(const std::wstring &groupName);

	policy();

	~policy();
private:
	static std::wstring GetPolicyNameForUser(const std::wstring &account);
	static PSID ConvertNameToBinarySid(const std::wstring &pAccountName);
};
