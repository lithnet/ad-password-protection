param(
    [Parameter(Mandatory)]
    [string]$ClientId,

    [Parameter(Mandatory)]
    [string]$PrivateKeyPem,

    [Parameter(Mandatory)]
    [string]$Repository
)

$ErrorActionPreference = 'Stop'

$pemContent = $PrivateKeyPem -replace '-----BEGIN RSA PRIVATE KEY-----', '' -replace '-----END RSA PRIVATE KEY-----', '' -replace '\s', ''
$keyBytes = [Convert]::FromBase64String($pemContent)

$rsa = [System.Security.Cryptography.RSA]::Create()
$rsa.ImportRSAPrivateKey($keyBytes, [ref]$null)

$now = [DateTimeOffset]::UtcNow.ToUnixTimeSeconds()
$header = @{ alg = 'RS256'; typ = 'JWT' } | ConvertTo-Json -Compress
$payload = @{ iss = $ClientId; iat = ($now - 60); exp = ($now + 540) } | ConvertTo-Json -Compress

$toBase64Url = { param($bytes) [Convert]::ToBase64String($bytes).TrimEnd('=').Replace('+', '-').Replace('/', '_') }

$headerB64 = & $toBase64Url ([System.Text.Encoding]::UTF8.GetBytes($header))
$payloadB64 = & $toBase64Url ([System.Text.Encoding]::UTF8.GetBytes($payload))
$unsigned = "$headerB64.$payloadB64"

$sigBytes = $rsa.SignData(
    [System.Text.Encoding]::UTF8.GetBytes($unsigned),
    [System.Security.Cryptography.HashAlgorithmName]::SHA256,
    [System.Security.Cryptography.RSASignaturePadding]::Pkcs1
)
$sigB64 = & $toBase64Url $sigBytes
$jwt = "$unsigned.$sigB64"

$headers = @{
    Authorization    = "Bearer $jwt"
    Accept           = 'application/vnd.github+json'
    'X-GitHub-Api-Version' = '2022-11-28'
}

$installation = Invoke-RestMethod -Uri "https://api.github.com/repos/$Repository/installation" -Headers $headers
$installationId = $installation.id
Write-Host "Found installation ID: $installationId"

$tokenResponse = Invoke-RestMethod -Uri "https://api.github.com/app/installations/$installationId/access_tokens" -Method Post -Headers $headers
$token = $tokenResponse.token

Write-Host "##vso[task.setvariable variable=GitHubAppToken;issecret=true]$token"
Write-Host "Installation token acquired (expires in ~1 hour)"
