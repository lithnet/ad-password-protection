---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Import-BannedWords

## SYNOPSIS
Imports a file containing new line-separated words into the banned word store

## SYNTAX

```
Import-BannedWords [-Filename] <String> [[-BatchSize] <Int32>] [<CommonParameters>]
```

## DESCRIPTION
The Import-BannedWords cmdlet will import a file containing new line-separated words into the banned word store. 
Each line read from the file will have the normalization rules applied to it before being committed to the store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Import-BannedWords -Filename "D:\password-protection\english-dictionary.txt"
```

This example shows how to import a file of words

### Example 2
```powershell
PS C:\> Import-BannedWords -Filename "D:\password-protection\english-dictionary.txt" -BatchSize 50000000
```

To increase speed when importing a large file when additional memory is available, you can specify a larger batch size

## PARAMETERS

### -BatchSize
Specifies how many lines to read from the file before committing them to disk.

In order to minimize the impact of memory usage when importing words, the words are read from the file and committed to the store in batches of this size. The larger the batch size, the faster the import, but the more memory is used. If left unspecified, the default value of 5,000,000 is used which provides an adequate balance of memory usage and speed.

```yaml
Type: Int32
Parameter Sets: (All)
Aliases:

Required: False
Position: 2
Default value: 5,000,000
Accept pipeline input: False
Accept wildcard characters: False
```

### -Filename
The name of the file containing the words to add to the store.

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
