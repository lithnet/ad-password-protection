param (
    [Parameter(Mandatory)]
    [string]$StorePath
)

$ErrorActionPreference = 'Stop'

$testDataPath = Join-Path $PSScriptRoot 'TestData'

if (-not (Test-Path $testDataPath))
{
    throw "Test data directory not found: $testDataPath"
}

Import-Module LithnetPasswordProtection -ErrorAction Stop
Open-Store -Path $StorePath -ErrorAction Stop

Import-CompromisedPasswords -Filename (Join-Path $testDataPath 'test-passwords.txt') -ErrorAction Stop
Import-CompromisedPasswordHashes -Filename (Join-Path $testDataPath 'test-hashes.txt') -ErrorAction Stop
Import-BannedWords -Filename (Join-Path $testDataPath 'test-banned-words.txt') -ErrorAction Stop

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

Write-Host "Test data imported and verified from $testDataPath"
