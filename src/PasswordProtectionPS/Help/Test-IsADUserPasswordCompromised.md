---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Test-IsADUserPasswordCompromised

## SYNOPSIS
Checks an existing user account to see if their password is present in the compromised password store

## SYNTAX

### AccountName (Default)
```
Test-IsADUserPasswordCompromised [-AccountName] <String> [[-DomainName] <String>] [[-Server] <String>]
 [[-Credential] <PSCredential>] [-OutputCompromisedHashOnMatch] [<CommonParameters>]
```

### Upn
```
Test-IsADUserPasswordCompromised [-Upn] <String> [[-Server] <String>] [[-Credential] <PSCredential>]
 [-OutputCompromisedHashOnMatch] [<CommonParameters>]
```

### Sid
```
Test-IsADUserPasswordCompromised [-Sid] <String> [[-Server] <String>] [[-Credential] <PSCredential>]
 [-OutputCompromisedHashOnMatch] [<CommonParameters>]
```

## DESCRIPTION
The Test-IsADUserPasswordCompromised cmdlet will extract the current password hash from Active Directory for the specified user, and check to see if it exists in the compromised password store.

This cmdlet must be executed by a user in the Domain Admins group, or by a user who holds the Replicate Directory Changes All permission on the domain containing the user to test.

Performing this action is a security-sensitive operation, and should only be performed from trusted devices within the domain.

This cmdlet does not require the password filter to be registered or enabled, but it does require direct access to the store. 

The cmdlet will not check the password against other password policy settings. It only checks to see if the password hash is located in the compromised password store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Test-IsADUserPasswordCompromised -AccountName ryan -DomainName lithnet
True
```

This example checks to see if the password for the user ryan in the lithnet domain is present in the compromised password store.

### Example 2
```powershell
PS C:\> Test-IsADUserPasswordCompromised -Upn ryan@lithnet.io
True
```

This example checks to see if the password for the user ryan@lithnet.io is present in the compromised password store.

### Example 3
```powershell
PS C:\> Test-IsADUserPasswordCompromised -Sid S-1-5-23423432-2343243211-44423
True
```

This example checks to see if the password for the user with the specified SID is present in the compromised password store.

### Example 4
```powershell
PS C:\> Test-IsADUserPasswordCompromised -Upn ryan@lithnet.io -Server dc1.lithnet.local -Credentials (Get-Credential)
False
```

This example checks to see if the password for the user ryan@lithnet.io is present in the compromised password store, checking against dc1.lithnet.local using the specified credentials.

### Example 5
```powershell
# cmdlet returns the hash when a compromised password was found
PS C:\> Test-IsADUserPasswordCompromised -AccountName ryan -Domain lithnet -OutputCompromisedHashOnMatch
8846f7eaee8fb117ad06bdd830b7586c

# cmdlet returns nothing if the password isn't compromised
PS C:\> Test-IsADUserPasswordCompromised -AccountName bob -Domain lithnet -OutputCompromisedHashOnMatch
```

This example checks to see if the password for the user ryan in the lithnet domain is present in the compromised password store. If the password is compromised, the cmdlet will output the raw NTLM hash of the password. If the password is not compromised, the cmdlet will output nothing.


## PARAMETERS

### -AccountName
The samAccountName of the user whose password should be tested.

```yaml
Type: String
Parameter Sets: AccountName
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Credential
The credentials to use to retrieve the password has from the directory. If omitted, the credentials of the currently logged on user are used.

```yaml
Type: PSCredential
Parameter Sets: (All)
Aliases:

Required: False
Position: 4
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -DomainName
The domain of the user whose password should be tested. If omitted, the domain of the currently logged on user is used.

```yaml
Type: String
Parameter Sets: AccountName
Aliases:

Required: False
Position: 2
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -OutputCompromisedHashOnMatch
The cmdlet ordinarily returns a $true $false value to indicate if the password is in the store. If this switch is specified, the cmdlet will output the raw NTLM hash when a match is found in the store, and nothing if there was no match.

```yaml
Type: SwitchParameter
Parameter Sets: (All)
Aliases:

Required: False
Position: 5
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Server
The server to retrieve the password hash from. If omitted, the cmdlet will use local machine if it is a domain controller, or find a suitable domain controller within the domain.

```yaml
Type: String
Parameter Sets: (All)
Aliases:

Required: False
Position: 3
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Sid
The string-representation of the SID of the user whose password should be tested.

```yaml
Type: String
Parameter Sets: Sid
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Upn
The userPrincipalName attribute of the user whose password should be tested.

```yaml
Type: String
Parameter Sets: Upn
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see [about_CommonParameters](http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### None

## OUTPUTS

### System.Object
The cmdlet returns a boolean value indicating whether the user's password was found in the compromised password store. However, if the OutputCompromisedHashOnMatch switch is specified, and the user's password was found in the compromised password store, the cmdlet will return the compromised password hash in hex format.

## NOTES

## RELATED LINKS
