---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Set-PasswordFilterConfig

## SYNOPSIS
Changes the password filter configuration

## SYNTAX

```
Set-PasswordFilterConfig [[-StorePath] <String>] [-Enabled] [<CommonParameters>]
```

## DESCRIPTION
This cmdlet allows you to change the store location, and the enabled state of the password filter

## EXAMPLES

### Example 1
```powershell
PS C:\> Set-PasswordFilterConfig -Enabled:$false
```

Disables the password filter

### Example 2
```powershell
PS C:\> Set-PasswordFilterConfig -Enabled
```

Enables the password filter

### Example 3
```powershell
PS C:\> Set-PasswordFilterConfig -StorePath C:\newstore
```

Changes the store location to C:\newstore

## PARAMETERS

### -Enabled
Enables or disables the password filter

```yaml
Type: SwitchParameter
Parameter Sets: (All)
Aliases:

Required: False
Position: Named
Default value: true
Accept pipeline input: False
Accept wildcard characters: False
```

### -StorePath
The path to the password store

```yaml
Type: String
Parameter Sets: (All)
Aliases:

Required: False
Position: 0
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
