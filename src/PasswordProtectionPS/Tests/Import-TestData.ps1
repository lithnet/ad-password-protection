param (
    [Parameter(Mandatory)]
    [string]$StorePath
)

$ErrorActionPreference = 'Stop'

Import-Module LithnetPasswordProtection -ErrorAction Stop
Open-Store -Path $StorePath -ErrorAction Stop

Import-CompromisedPasswords -Filename 'C:\LPP-TestData\test-passwords.txt' -ErrorAction Stop
Import-CompromisedPasswordHashes -Filename 'C:\LPP-TestData\test-hashes.txt' -ErrorAction Stop
Import-BannedWords -Filename 'C:\LPP-TestData\test-banned-words.txt' -ErrorAction Stop

# Verify imports
$pwdCheck = Test-IsCompromisedPassword -Value 'KnownPassword123!' -ErrorAction Stop
if (-not $pwdCheck)
{
    throw "Verification failed: KnownPassword123! not detected as compromised"
}

$wordCheck = Test-IsBannedWord -Value 'password' -ErrorAction Stop
if (-not $wordCheck)
{
    throw "Verification failed: 'password' not detected as banned word"
}

Write-Host "Test data imported and verified."
