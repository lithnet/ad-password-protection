---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Test-IsBannedWord

## SYNOPSIS
Checks to see if a string matches a value in the banned word store

## SYNTAX

### String (Default)
```
Test-IsBannedWord [-Value] <String> [<CommonParameters>]
```

### Hash
```
Test-IsBannedWord [-Hash] <Byte[]> [<CommonParameters>]
```

### SecureString
```
Test-IsBannedWord [-SecureString] <SecureString> [<CommonParameters>]
```

## DESCRIPTION
The Test-IsBannedWord cmdlet checks to see if a specified string matches a value in the banned word store. Normalization rules are applied to the string and the resulting value is checked against the store.

## EXAMPLES

### Example 1
```powershell
# Add a word to the store
PS C:\> Add-BannedWord -Value password

# Test the exact word and ensure it is found
PS C:\> Test-IsBannedWord -Value password
True

# Test a modification of the banned word, and ensure it is found
PS C:\> Test-IsBannedWord -Value P@ssw0rd!
True
```

The example shown above adds a word to the banned word store, and then tests the exact word and a modified version of the word. The modified version of the word is found because the normalization rules applied to the word before it is checked against the store.

## PARAMETERS

### -Hash
A binary hash to test. A hash cannot be normalized, so it is tested as-is.

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

### -SecureString
The string to test as a secure string

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
The string to test as a plain-text string

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
## NOTES

## RELATED LINKS
[Normalization rules](https://docs.lithnet.io/password-protection/help-and-support/normalization-rules)