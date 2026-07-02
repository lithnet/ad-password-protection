param (
    [Parameter(Mandatory=$true)]
    [string] $pat,
    [Parameter(Mandatory=$true)]
    [string] $environmentName,
    [Parameter(Mandatory=$true)]
    [string] $projectName,
    [Parameter(Mandatory=$true)]
    [string] $organizationUrl,
    [string] $agentVersion = "4.258.1",
    [string] $resourceTags = "role:dc,product:lpp"
)

$ErrorActionPreference = "Stop"

$computerName = $env:ComputerName
Write-Host "Computer name: $computerName"

$agentDir = "C:\azagent\A1"

if (Test-Path "$agentDir\.agent") {
    Write-Host "Removing existing agent registration..."
    & "$agentDir\config.cmd" remove --unattended --auth pat --token $pat 2>&1 | Write-Host
}

if (-not (Test-Path "$agentDir\config.cmd")) {
    Write-Host "Downloading agent v$agentVersion..."
    New-Item -ItemType Directory -Force -Path $agentDir | Out-Null
    $zip = "$env:TEMP\agent.zip"
    $url = "https://download.agent.dev.azure.com/agent/$agentVersion/vsts-agent-win-x64-$agentVersion.zip"
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    (New-Object Net.WebClient).DownloadFile($url, $zip)
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [IO.Compression.ZipFile]::ExtractToDirectory($zip, $agentDir)
    Remove-Item $zip
    Write-Host "Agent downloaded and extracted."
}

Write-Host "Configuring agent for environment '$environmentName' in project '$projectName'..."
& "$agentDir\config.cmd" `
    --environment `
    --environmentname $environmentName `
    --agent $computerName `
    --replace `
    --unattended `
    --work '_work' `
    --url $organizationUrl `
    --projectname $projectName `
    --auth PAT `
    --token $pat `
    --addvirtualmachineresourcetags `
    --virtualmachineresourcetags $resourceTags 2>&1 | Write-Host

if ($LASTEXITCODE -ne 0) {
    throw "config.cmd failed with exit code $LASTEXITCODE"
}

Write-Host "Starting agent interactively..."
Start-Process -FilePath "$agentDir\run.cmd" -WorkingDirectory $agentDir -WindowStyle Hidden
Write-Host "Agent registered and started."
