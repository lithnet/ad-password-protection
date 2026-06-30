param(
    [Parameter(Mandatory)]
    [string]$BuildId,

    [Parameter(Mandatory)]
    [string]$Organization,

    [Parameter(Mandatory)]
    [string]$Project,

    [string]$WatchFolder = "$env:USERPROFILE\Downloads",

    [string]$ArtifactName = "lpp_filter_ms_signed",

    [int]$PollIntervalSeconds = 5
)

$ErrorActionPreference = 'Stop'

$cabDir = Join-Path $WatchFolder "lpp-signing-$BuildId"
New-Item -ItemType Directory -Force -Path $cabDir | Out-Null

Write-Host "Downloading CAB artifact from build $BuildId..." -ForegroundColor Cyan
az pipelines runs artifact download `
    --artifact-name lpp_filter_for_signing `
    --path $cabDir `
    --run-id $BuildId `
    --organization $Organization `
    --project $Project

if ($LASTEXITCODE -ne 0)
{
    throw "Failed to download CAB artifact"
}

$cab = Get-ChildItem $cabDir -Filter "*.cab" -Recurse | Select-Object -First 1
if (-not $cab)
{
    throw "No CAB file found in $cabDir"
}

Write-Host ""
Write-Host "CAB downloaded to: $($cab.FullName)" -ForegroundColor Green
Write-Host ""

Start-Process "https://partner.microsoft.com/en-us/dashboard/hardware/FileSign/NewLSA"

Write-Host "Opened Microsoft LSA signing portal in your browser." -ForegroundColor Cyan
Write-Host "Submit the CAB file above, then download the signed ZIP to $WatchFolder" -ForegroundColor Cyan
Write-Host ""

$existing = @{}
Get-ChildItem $WatchFolder -Filter "signed_*.zip" | ForEach-Object { $existing[$_.FullName] = $true }

Write-Host "Watching $WatchFolder for a new signed_*.zip file..." -ForegroundColor Yellow

while ($true)
{
    $candidates = Get-ChildItem $WatchFolder -Filter "signed_*.zip" | Where-Object { -not $existing.ContainsKey($_.FullName) }

    if ($candidates)
    {
        $signed = $candidates | Sort-Object LastWriteTime -Descending | Select-Object -First 1

        $retries = 0
        $previousSize = -1
        while ($retries -lt 3)
        {
            Start-Sleep -Seconds 1
            $currentSize = (Get-Item $signed.FullName).Length
            if ($currentSize -eq $previousSize -and $currentSize -gt 0)
            {
                break
            }
            $previousSize = $currentSize
            $retries++
        }

        Write-Host ""
        Write-Host "Found: $($signed.Name) ($([math]::Round($signed.Length / 1KB)) KB)" -ForegroundColor Green
        Write-Host "Uploading to pipeline artifact '$ArtifactName'..." -ForegroundColor Cyan

        az pipelines runs artifact upload `
            --artifact-name $ArtifactName `
            --path $signed.FullName `
            --run-id $BuildId `
            --organization $Organization `
            --project $Project

        if ($LASTEXITCODE -ne 0)
        {
            throw "Failed to upload signed artifact"
        }

        Write-Host ""
        Write-Host "Done. Signed artifact uploaded. Resume the pipeline validation to continue the build." -ForegroundColor Green
        break
    }

    Start-Sleep -Seconds $PollIntervalSeconds
}
