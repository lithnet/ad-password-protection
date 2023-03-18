---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Open-Store

## SYNOPSIS
Opens a password store

## SYNTAX

```
Open-Store [[-Path] <String>] [<CommonParameters>]
```

## DESCRIPTION
The Open-Store cmdlet connects the PowerShell module to a new or existing password store. The use of the Open-Store cmdlet is required when connecting to a store that is not the default store for the current machine. If the Open-Store cmdlet is not run, other cmdlets will try to connect to the machines default store, and throw an error if the default store does not exist.

## EXAMPLES

### Example 1
```powershell
PS C:\> Open-Store -Path "D:\password-protection\test-store"
```

Opens the non-default store located at D:\password-protection\test-store

## PARAMETERS

### -Path
The full path to the store you want to open.

```yaml
Type: String
Parameter Sets: (All)
Aliases:

Required: False
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
## NOTES

## RELATED LINKS
