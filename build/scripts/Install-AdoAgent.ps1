# Installs and registers an Azure DevOps pipeline agent on a VM via Run Command.
#
# We register the agent without --runAsService because config.cmd's service
# installer creates a local group and tries to add the service account to it.
# On a domain controller, built-in machine accounts (SYSTEM) can't be added
# to local groups, so that fails. The local group only controls folder ACLs
# which SYSTEM already has, so it's unnecessary.
#
# Instead we create a scheduled task to run the agent as SYSTEM on startup
# and trigger it immediately. The task survives reboots, and SYSTEM on a DC
# has the permissions the E2E tests need (including DRS replication).

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
$agentDir = "C:\azagent\A1"
$taskName = "AzurePipelinesAgent"

Write-Host "Computer name: $computerName"

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

Write-Host "Creating scheduled task to run agent as SYSTEM..."
$existingTask = Get-ScheduledTask -TaskName $taskName -ErrorAction SilentlyContinue
if ($existingTask) {
    Unregister-ScheduledTask -TaskName $taskName -Confirm:$false
}

$action = New-ScheduledTaskAction -Execute "$agentDir\run.cmd" -WorkingDirectory $agentDir
$trigger = New-ScheduledTaskTrigger -AtStartup
$principal = New-ScheduledTaskPrincipal -UserId "NT AUTHORITY\SYSTEM" -LogonType ServiceAccount -RunLevel Highest
$settings = New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries -ExecutionTimeLimit ([TimeSpan]::Zero)

Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger -Principal $principal -Settings $settings | Out-Null
Start-ScheduledTask -TaskName $taskName

Write-Host "Agent registered and started via scheduled task."
