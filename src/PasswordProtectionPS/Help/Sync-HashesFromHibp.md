---
external help file: LithnetPasswordProtection.dll-Help.xml
Module Name: LithnetPasswordProtection
online version:
schema: 2.0.0
---

# Sync-HashesFromHibp

## SYNOPSIS
Synchronizes the hashes from the Have I Been Pwned API into the compromised password store

## SYNTAX

```
Sync-HashesFromHibp [[-Threads] <Int32>] [-Reset] [<CommonParameters>]
```

## DESCRIPTION
This cmdlet performs a synchronization of the NTLM hashes in the Have I Been Pwned API into the compromised password store.

## EXAMPLES

### Example 1
```powershell
PS C:\> Sync-HashesFromHibp
```

Initializes the synchronization from the Have I Bene Pwned API

## PARAMETERS

### -Reset
Resets the synchronization state and forces a full enumeration of all hashes in the Have I Been Pwned API

```yaml
Type: SwitchParameter
Parameter Sets: (All)
Aliases:

Required: False
Position: Named
Default value: False
Accept pipeline input: False
Accept wildcard characters: False
```

### -Threads
Specifies the number of threads to use

```yaml
Type: Int32
Parameter Sets: (All)
Aliases:

Required: False
Position: 0
Default value: 8 x the number of CPU cores
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
