---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Add-CompromisedPassword

## SYNOPSIS
Adds a compromised password to the store

## SYNTAX

### String (Default)
```
Add-CompromisedPassword [-Value] <String> [<CommonParameters>]
```

### SecureString
```
Add-CompromisedPassword [-SecureString] <SecureString> [<CommonParameters>]
```

## DESCRIPTION
This cmdlet adds a string into the compromised password store. When the password filter is configured to check against compromised passwords, any password that is an exact match to this string will rejected.

## EXAMPLES

### Example 1
```powershell
Add-CompromisedPassword -Value "password"
```

This example adds a single compromised password to the store

### Example 2
```powershell
Add-CompromisedPassword -SecureString (Read-Host -Prompt "Enter the compromised password to add to the store" -AsSecureString)
```

This example prompts for a password to add to the compromised password store

## PARAMETERS

### -SecureString
The password to add to the compromised password store as a secure string

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

### -Value
The password to add to the compromised password store

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

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see [about_CommonParameters](http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### System.String

### System.Security.SecureString

## OUTPUTS

## NOTES
To import a file of compromised passwords, use the Import-CompromisedPassword cmdlet or the Import-CompromisedPasswordHashes cmdlets.

## RELATED LINKS
