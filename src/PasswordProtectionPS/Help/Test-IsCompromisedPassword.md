---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Test-IsCompromisedPassword

## SYNOPSIS
Checks if a password is in the compromised password store

## SYNTAX

### String (Default)
```
Test-IsCompromisedPassword [-Value] <String> [-Normalize] [<CommonParameters>]
```

### Hash
```
Test-IsCompromisedPassword [-Hash] <Byte[]> [<CommonParameters>]
```

### SecureString
```
Test-IsCompromisedPassword [-SecureString] <SecureString> [-Normalize] [<CommonParameters>]
```

## DESCRIPTION
The Test-IsCompromisedPassword cmdlet checks to see if a specified string matches a value in the compromised password store.

## EXAMPLES

### Example 1
```powershell
# Add a password to the store
PS> Add-CompromisedPassword -Value password!!!

# Test the password and ensure it is found
PS> Test-IsCompromisedPassword -Value password!!!
True
```

This example adds a password to the compromised password store, and then tests it to ensure it is found

## PARAMETERS

### -Hash
A binary hash to test

```yaml
Type: Byte[]
Parameter Sets: Hash
Aliases:

Required: True
Position: 1
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### -Normalize
Normalizes the password before testing it against the store

```yaml
Type: SwitchParameter
Parameter Sets: String, SecureString
Aliases:

Required: False
Position: 2
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -SecureString
The string to test as a SecureString.

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
The string to test.

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

### System.Byte[]

### System.Security.SecureString

## OUTPUTS

### System.Object
The cmdlet returns a boolean value indicating whether the input string or hash matches a value in the compromised password store.

## NOTES

## RELATED LINKS
