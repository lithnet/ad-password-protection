---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Get-PasswordFilterPolicy

## SYNOPSIS
Gets the current active policy 

## SYNTAX

```
Get-PasswordFilterPolicy [[-User] <String>] [<CommonParameters>]
```

## DESCRIPTION
This cmdlet shows the effective policy in place for the password filter on the current machine, as applied by group policy

## EXAMPLES

### Example 1
```powershell
PS C:\>  Get-PasswordFilterPolicy | fl
```

Gets the current policy and formats it by list

## PARAMETERS

### -User
The username of the user to obtain the policy for

This parameter is reserved for future use and is currently ignored

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
