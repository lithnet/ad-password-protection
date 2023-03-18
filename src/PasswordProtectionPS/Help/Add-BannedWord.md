---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Add-BannedWord

## SYNOPSIS
Adds a new banned word to the store

## SYNTAX

### String (Default)
```
Add-BannedWord [-Value] <String> [<CommonParameters>]
```

### SecureString
```
Add-BannedWord [-SecureString] <SecureString> [<CommonParameters>]
```

## DESCRIPTION
The Add-BannedWord cmdlet takes a string of text, applies normalization rules to it, and adds it to the banned word store

## EXAMPLES

### Example 1
```powershell
Add-BannedWord -Value "password"
```

This example adds a single banned word to the store


### Example 1
```powershell
Add-BannedWord -SecureString (Read-Host -Prompt "Enter the banned word to add to the store" -AsSecureString)
```

This example prompts for a word to be added to the store


## PARAMETERS

### -SecureString
The string of text to add to the store as a secure string object. The normalization rules will be applied to the string before it is committed to the store.

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
The string of text to add to the store. The normalization rules will be applied to the string before it is committed to the store.

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

## RELATED LINKS
[Normalization rules](https://docs.lithnet.io/password-protection/help-and-support/normalization-rules)