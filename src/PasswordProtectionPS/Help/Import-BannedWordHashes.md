---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Import-BannedWordHashes

## SYNOPSIS
Imports a set of NTLM hashes of banned words into the store

## SYNTAX

```
Import-BannedWordHashes [-Filename] <String> [[-Sorted] <Boolean>] [[-BatchSize] <Int32>] [<CommonParameters>]
```

## DESCRIPTION
The Import-BannedWordHashes cmdlet will import a file containing newline-separated NTLM hashes into the banned word store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Import-BannedWordHashes -Filename "D:\password-protection\banned-word-hashes.txt"
```

This example imports the hashes contained in the file "D:\password-protection\banned-word-hashes.txt" into the store.

## PARAMETERS

### -BatchSize
Specifies how many hashes to read from an unsorted file before committing to disk. This parameter has no effect on a sorted file.

```yaml
Type: Int32
Parameter Sets: (All)
Aliases:

Required: False
Position: 3
Default value: 5,000,000
Accept pipeline input: False
Accept wildcard characters: False
```

### -Filename
The of the file containing the NTLM hashes. Each hash should be in hex format, be 32 characters in length, and each record must appear on a new line. The hash can optionally end with a colon and anything after the colon character is ignored.

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

### -Sorted
A boolean value indicating if the hashes in the file are sorted in ascending order or not. The cmdlet will optimize the type of import it performs based on this parameter. A sorted file will allow a much faster import of data into the store. However, there is a significant penalty if you specify that the file is sorted when it is not. If this parameter is omitted, the cmdlet will guess which algorithm to use by reading the first 1000 lines of the file. If those first 1000 lines are all in order, a sorted import will be performed.

```yaml
Type: Boolean
Parameter Sets: (All)
Aliases:

Required: False
Position: 2
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see [about_CommonParameters](http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### System.String

## OUTPUTS

### System.Object
## NOTES

The following list shows the allowed formats. The hashes are not case sensitive.

8846f7eaee8fb117ad06bdd830b7586c
5835048ce94ad0564e29a924a03510ef:123
e22e04519aa757d12f1219c4f31252f4:3
e22e04519aa757d12f1219c4f31252f4:

## RELATED LINKS
