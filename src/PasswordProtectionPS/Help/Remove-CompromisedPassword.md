---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Remove-CompromisedPassword

## SYNOPSIS
Removes a compromised password from the store

## SYNTAX

### String (Default)
```
Remove-CompromisedPassword [-Value] <String> [<CommonParameters>]
```

### SecureString
```
Remove-CompromisedPassword [-SecureString] <SecureString> [<CommonParameters>]
```

## DESCRIPTION
The Remove-CompromisedPassword cmdlet removes a string of text from the compromised password store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Remove-CompromisedPassword -Value "password"
```

Removes the string "password" from the compromised password store

## PARAMETERS

### -SecureString
The password to remove from the compromised password store as a secure string

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
The password to remove from the compromised password store as a string

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
