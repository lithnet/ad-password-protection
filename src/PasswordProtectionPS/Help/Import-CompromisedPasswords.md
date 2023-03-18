---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Import-CompromisedPasswords

## SYNOPSIS
Imports plain-text passwords into the compromised password store

## SYNTAX

```
Import-CompromisedPasswords [-Filename] <String> [[-BatchSize] <Int32>] [<CommonParameters>]
```

## DESCRIPTION
The Import-CompromisedPasswords cmdlet will import a file containing new line-separated plain-text passwords into the compromised password store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Import-CompromisedPasswords -Filename "D:\password-protection\plain-text-passwords.txt"
```

Imports passwords from the file "D:\password-protection\plain-text-passwords.txt" into the compromised password store.

## PARAMETERS

### -BatchSize
Specifies how many passwords to read from the file before committing them to disk.

In order to minimize the impact of memory usage when importing passwords, the passwords are read from the file and committed to the store in batches of this size. The larger the batch size, the faster the import, but the more memory is used. If left unspecified, the default value of 5,000,000 is used which provides an adequate balance of memory usage and speed.

```yaml
Type: Int32
Parameter Sets: (All)
Aliases:

Required: False
Position: 2
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Filename
The name of the file containing the plain text passwords.

```yaml
Type: String
Parameter Sets: (All)
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

## OUTPUTS

### System.Object
## NOTES

## RELATED LINKS
