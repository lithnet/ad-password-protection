---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Get-PasswordFilterResult

## SYNOPSIS
Processes a password through the password filter and gets the result of the policy 

## SYNTAX

### String (Default)
```
Get-PasswordFilterResult [-Password] <String> [-Username] <String> [-Fullname] <String>
 [[-IsSetOperation] <Boolean>] [<CommonParameters>]
```

### SecureString
```
Get-PasswordFilterResult [-SecurePassword] <SecureString> [-Username] <String> [-Fullname] <String>
 [[-IsSetOperation] <Boolean>] [<CommonParameters>]
```

## DESCRIPTION
The Get-PasswordFilterResult cmdlet allows you to programmatically test passwords against your password policy.

This cmdlet does not attempt to change the user's password, nor does it contact the domain controller. Use of this cmdlet requires that the group policy for the password filter is configured and applied to the computer you are running the cmdlet on. The password filter does not need to be configured to filter passwords on the local computer, but the policy needs to be in place.

## EXAMPLES

### Example 1
```powershell
PS C:\> Get-PasswordFilterResult -Password "password" -Username "test-user" -Fullname "John Test"
Compromised

PS C:\> Get-PasswordFilterResult -Password "John" -Username "test-user" -Fullname "John Test"
ContainsFullName
```

These examples show two password filter operations that are rejected for different reasons.


### Example 2
```powershell
PS C:\> Get-PasswordFilterResult -SecurePassword (Read-Host -Prompt "Enter the password" -AsSecureString) -Username (Read-Host -Prompt "Enter the user's username") -Fullname (Read-Host -Prompt "Enter the user's full name")
```

Prompts for a password and username and then tests the password against the password filter.

## PARAMETERS

### -Fullname
The full display name of the user

```yaml
Type: String
Parameter Sets: (All)
Aliases:

Required: True
Position: 3
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### -IsSetOperation
Specifies if the password should be tested as a set operation. By default it is tested as a change operation.

```yaml
Type: Boolean
Parameter Sets: (All)
Aliases:

Required: False
Position: 4
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Password
The password to test

```yaml
Type: String
Parameter Sets: String
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### -SecurePassword
A secure string representation of the password to test

```yaml
Type: SecureString
Parameter Sets: SecureString
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### -Username
The users account name

```yaml
Type: String
Parameter Sets: (All)
Aliases:

Required: True
Position: 2
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see [about_CommonParameters](http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### System.String

### System.Security.SecureString

## OUTPUTS

### Lithnet.ActiveDirectory.PasswordProtection.PasswordTestResult
The cmdlet will return one of the following values

| String value                  | Description                                                                                          |
| ----------------------------- | ---------------------------------------------------------------------------------------------------- |
| Approved                      | The password was approved by the filter                                                              |
| LengthRequirementsNotMet      | The password did not meet the minimum length requirements                                            |
| ComplexityThresholdNotMet     | The password did not meet the complexity requirements for a password of the given length             |
| ComplexityPointsNotMet        | The password did not meet the minimum number of complexity points                                    |
| DidNotMatchApprovalRegex      | The password did not match the regular expression required for approval                              |
| MatchedRejectRegex            | The password did matched the rejection regular expression                                            |
| ContainsAccountName           | The password contained the user's account name                                                       |
| ContainsFullName              | The password contained all or part of the user's full name                                           |
| Compromised                   | The password was found in the compromised password store                                             |
| CompromisedNormalizedPassword | The password was found in the compromised password store after the normalization rules were applied  |
| BannedNormalizedWord          | The password was found in the banned word store after the normalization rules were applied           |
| PasswordWasBlank              | The password was an empty string                                                                     |
| FilterError                   | An exception occurred in the filter engine and the password could not be processed                   |


## NOTES

## RELATED LINKS
[Normalization rules](https://docs.lithnet.io/password-protection/help-and-support/normalization-rules)