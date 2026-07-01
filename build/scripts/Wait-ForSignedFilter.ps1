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
        Write-Host "Signed artifact uploaded. Resuming pipeline..." -ForegroundColor Cyan

        $orgUrl = $Organization.TrimEnd('/')
        $timeline = az rest --method get --url "$orgUrl/$Project/_apis/build/builds/$BuildId/timeline?api-version=7.1" -o json 2>$null | ConvertFrom-Json

        if ($LASTEXITCODE -ne 0 -or -not $timeline)
        {
            Write-Host "Could not query build timeline. Please resume the validation manually." -ForegroundColor Yellow
            break
        }

        $checkpointId = $timeline.records | Where-Object { $_.type -eq "Checkpoint.Approval" -and $_.state -eq "inProgress" } | Select-Object -First 1 -ExpandProperty id

        if (-not $checkpointId)
        {
            Write-Host "No pending approval found. Please resume the validation manually." -ForegroundColor Yellow
            break
        }

        $body = @{ status = "approved"; comment = "Signed artifact uploaded by Wait-ForSignedFilter.ps1" } | ConvertTo-Json -Compress
        az rest --method patch --url "$orgUrl/$Project/_apis/pipelines/approvals/$($checkpointId)?api-version=7.1-preview.1" --body $body --headers "Content-Type=application/json" -o none 2>$null

        if ($LASTEXITCODE -eq 0)
        {
            Write-Host "Done. Pipeline resumed." -ForegroundColor Green
        }
        else
        {
            Write-Host "Could not auto-resume. Please resume the validation manually." -ForegroundColor Yellow
        }

        break
    }

    Start-Sleep -Seconds $PollIntervalSeconds
}
