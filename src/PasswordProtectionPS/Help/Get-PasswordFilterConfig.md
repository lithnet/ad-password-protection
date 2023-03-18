---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Get-PasswordFilterConfig

## SYNOPSIS
Gets the current password filter configuration

## SYNTAX

```
Get-PasswordFilterConfig [<CommonParameters>]
```

## DESCRIPTION
This cmdlet gets the current password filter configuration, showing the configured store location, if the password filter is registered, and if the password filter is enabled.

## EXAMPLES

### Example 1
```powershell
PS C:\>  Get-PasswordFilterConfig

StorePath       IsFilterRegistered IsFilterEnabled
---------       ------------------ ---------------
d:\lppstore                   True            True

```

Shows the output of the cmdlet

## PARAMETERS

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see [about_CommonParameters](http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### None

## OUTPUTS

### System.Object
## NOTES

## RELATED LINKS
