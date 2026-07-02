param (
    [Parameter(Mandatory)]
    [string]$StorePath
)

$ErrorActionPreference = 'Stop'

Import-Module ActiveDirectory

$domainDn = (Get-ADDomain).DistinguishedName
$domainDns = (Get-ADDomain).DNSRoot
$ouPath = "OU=LPP Testing,$domainDn"

# Create test OU (idempotent)
if (-not (Get-ADOrganizationalUnit -Filter "Name -eq 'LPP Testing'" -SearchBase $domainDn -ErrorAction SilentlyContinue))
{
    New-ADOrganizationalUnit -Name 'LPP Testing' -Path $domainDn
    Write-Host "Created OU: $ouPath"
}
else
{
    Write-Host "OU already exists: $ouPath"
}

# Create test users (idempotent)
$users = @(
    @{ Name = 'TestUser1'; Password = 'KnownPassword123!' },
    @{ Name = 'TestUser2'; Password = 'Cl3anP@ssw0rd!Xyz' }
)

foreach ($u in $users)
{
    if (-not (Get-ADUser -Filter "SamAccountName -eq '$($u.Name)'" -ErrorAction SilentlyContinue))
    {
        New-ADUser -Name $u.Name `
            -SamAccountName $u.Name `
            -UserPrincipalName "$($u.Name)@$domainDns" `
            -AccountPassword (ConvertTo-SecureString $u.Password -AsPlainText -Force) `
            -Enabled $true `
            -Path $ouPath `
            -ChangePasswordAtLogon $false
        Write-Host "Created user: $($u.Name)"
    }
    else
    {
        Set-ADAccountPassword -Identity $u.Name -Reset -NewPassword (ConvertTo-SecureString $u.Password -AsPlainText -Force)
        Write-Host "User exists, password reset: $($u.Name)"
    }
}

# Create store directory and set registry before MSI install.
# The installer's AppSearch reads 'Store' from HKLM\SOFTWARE\Lithnet\PasswordFilter
# (AIP RegLocator: AppSearchSign). If found, STOREDIR inherits the value.
# If not found, SetStoredir defaults to [APPDIR]\Store (AIP condition: NOT STOREDIR).
New-Item -ItemType Directory -Path $StorePath -Force | Out-Null
Write-Host "Store directory: $StorePath"

$regPath = 'HKLM:\Software\Lithnet\PasswordFilter'
New-Item -Path $regPath -Force | Out-Null
Set-ItemProperty -Path $regPath -Name 'Store' -Value $StorePath -Type String
Write-Host "Registry Store = $((Get-ItemProperty $regPath).Store)"
