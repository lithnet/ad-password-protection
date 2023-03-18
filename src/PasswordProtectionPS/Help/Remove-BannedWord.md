---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Remove-BannedWord

## SYNOPSIS
Removes a banned word from the store

## SYNTAX

### String (Default)
```
Remove-BannedWord [-Value] <String> [<CommonParameters>]
```

### SecureString
```
Remove-BannedWord [-SecureString] <SecureString> [<CommonParameters>]
```

## DESCRIPTION
The Remove-BannedWord cmdlet takes a string of text, applies the normalization rules to it, and removes it from the banned word store. If the normalized word is not present in the store, this cmdlet does nothing.

## EXAMPLES

### Example 1
```powershell
PS C:\> Remove-BannedWord -Value "password"
```

Removes the word "password" from the banned word store

## PARAMETERS

### -SecureString
The string of text to remove from the store as a secure string object. Normalization rules will be applied to the string.

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
The string of text to remove from the store. Normalization rules will be applied to the string.

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

### System.Object
## NOTES

## RELATED LINKS
[Normalization rules](https://docs.lithnet.io/password-protection/help-and-support/normalization-rules)