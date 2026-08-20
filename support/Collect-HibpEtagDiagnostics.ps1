#requires -Version 5.1

<#
.SYNOPSIS
Collects response details for an intermittent HIBP ETag failure.

.DESCRIPTION
This script reads the saved HIBP ETags from a Lithnet Password Protection store and sends the same conditional range requests as Sync-HashesFromHibp. It does not change the password store or its saved HIBP state.

Run the script on the affected server as soon as possible after Sync-HashesFromHibp reports a missing ETag. Use the same proxy setting that you use with Sync-HashesFromHibp.

The script writes a JSON report that contains the exact HIBP prefix, the saved request ETag, the response ETag, selected HTTP and Cloudflare response headers, and the approximate location that you supply. The report does not include the computer name, user name, store path, public IP address, or password hashes.

The full test sends one conditional request for each of the 1,048,576 HIBP NTLM prefixes. It can take 30 minutes or longer, depending on the server and network connection.

.PARAMETER Location
Specifies the approximate city, state or region, and country where the affected server is located. Do not enter an address, computer name, organization name, or IP address.

.PARAMETER StorePath
Specifies the Lithnet Password Protection store. If you omit this parameter, the script gets the configured path from Get-PasswordFilterConfig.

.PARAMETER ProxyAddress
Specifies the proxy that Sync-HashesFromHibp uses. Omit this parameter if Sync-HashesFromHibp uses the system proxy or a direct connection. The report records only whether the script used an explicit proxy, the system proxy, or a direct connection.

.PARAMETER Threads
Specifies the maximum number of requests that the script can run at the same time. The default matches the Sync-HashesFromHibp calculation, up to 256 requests.

.PARAMETER OutputPath
Specifies the JSON report path. The default path is in the current directory and contains the current UTC date and time.

.PARAMETER RangeStart
Specifies the first five-character HIBP prefix to test as an integer. This parameter is intended for a test run or a request from Lithnet support.

.PARAMETER RangeEnd
Specifies the last five-character HIBP prefix to test as an integer. This parameter is intended for a test run or a request from Lithnet support.

.PARAMETER MaximumAnomalies
Specifies the maximum number of detailed anomaly records to include in the report. The script continues to count all anomalies after it reaches this limit.

.EXAMPLE
PS C:\> .\Collect-HibpEtagDiagnostics.ps1 -Location "Brisbane, Queensland, Australia"

Tests all HIBP prefixes by using the configured Lithnet Password Protection store and writes the report to the current directory.

.EXAMPLE
PS C:\> .\Collect-HibpEtagDiagnostics.ps1 -Location "London, United Kingdom" -ProxyAddress "http://proxy.example.com:8080"

Tests all HIBP prefixes through the specified proxy.
#>

[CmdletBinding()]
param
(
    [Parameter(Mandatory = $true)]
    [ValidateLength(2, 200)]
    [string]$Location,

    [Parameter()]
    [string]$StorePath,

    [Parameter()]
    [Uri]$ProxyAddress,

    [Parameter()]
    [ValidateRange(1, 256)]
    [int]$Threads = [Math]::Min([Environment]::ProcessorCount * 8, 256),

    [Parameter()]
    [string]$OutputPath,

    [Parameter()]
    [ValidateRange(0, 0xFFFFF)]
    [int]$RangeStart = 0,

    [Parameter()]
    [ValidateRange(0, 0xFFFFF)]
    [int]$RangeEnd = 0xFFFFF,

    [Parameter()]
    [ValidateRange(1, 1000)]
    [int]$MaximumAnomalies = 100
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

$diagnosticVersion = '1.0'
$apiBaseAddress = [Uri]'https://api.pwnedpasswords.com/range/'
$startedAtUtc = [DateTime]::UtcNow

if ($RangeStart -gt $RangeEnd)
{
    throw "RangeStart must not be greater than RangeEnd."
}

if ($ProxyAddress -ne $null -and -not $ProxyAddress.IsAbsoluteUri)
{
    throw "ProxyAddress must be an absolute URI."
}

$syncCommand = Get-Command -Name 'Sync-HashesFromHibp' -ErrorAction SilentlyContinue
if ($syncCommand -eq $null)
{
    throw "The Sync-HashesFromHibp command is not available. Install or import the Lithnet Password Protection PowerShell module before you run this diagnostic."
}

$productVersion = $syncCommand.ImplementingType.Assembly.GetName().Version.ToString()

if ([string]::IsNullOrWhiteSpace($StorePath))
{
    $configCommand = Get-Command -Name 'Get-PasswordFilterConfig' -ErrorAction SilentlyContinue
    if ($configCommand -eq $null)
    {
        throw "The Get-PasswordFilterConfig command is not available. Specify StorePath or import the Lithnet Password Protection PowerShell module."
    }

    $config = & $configCommand
    $StorePath = $config.StorePath
}

if ([string]::IsNullOrWhiteSpace($StorePath))
{
    throw "The Lithnet Password Protection store path is not configured."
}

$metadataPath = Join-Path -Path $StorePath -ChildPath 'metadata\hibp-state.dat'
if (-not [IO.File]::Exists($metadataPath))
{
    throw "The saved HIBP state was not found. Run this diagnostic against a store that has completed a HIBP synchronization."
}

if ([string]::IsNullOrWhiteSpace($OutputPath))
{
    $fileName = 'LppHibpEtagDiagnostic-{0}.json' -f $startedAtUtc.ToString('yyyyMMdd-HHmmss')
    $OutputPath = Join-Path -Path (Get-Location).Path -ChildPath $fileName
}

$fullOutputPath = [IO.Path]::GetFullPath($OutputPath)
$outputDirectory = [IO.Path]::GetDirectoryName($fullOutputPath)
if ([string]::IsNullOrWhiteSpace($outputDirectory) -or -not [IO.Directory]::Exists($outputDirectory))
{
    throw "The output directory does not exist."
}

if ([string]::Equals([IO.Path]::GetFullPath($metadataPath), $fullOutputPath, [StringComparison]::OrdinalIgnoreCase))
{
    throw "OutputPath must not refer to the saved HIBP state file."
}

Write-Host "Reading saved HIBP ETags."

$hibpState = New-Object 'System.Collections.Generic.Dictionary[string,string]' ([StringComparer]::OrdinalIgnoreCase)
$ignoredMetadataLines = 0

foreach ($line in [IO.File]::ReadLines($metadataPath))
{
    if ([string]::IsNullOrWhiteSpace($line))
    {
        continue
    }

    if ($line -match '^(?<Prefix>[0-9A-Fa-f]{5}):(?<ETag>.+)$')
    {
        $prefix = $Matches['Prefix'].ToUpperInvariant()
        if (-not $hibpState.ContainsKey($prefix))
        {
            $hibpState.Add($prefix, $Matches['ETag'])
        }
    }
    else
    {
        $ignoredMetadataLines++
    }
}

$missingValidatorCount = 0
$missingValidatorExamples = New-Object 'System.Collections.Generic.List[string]'

for ($range = $RangeStart; $range -le $RangeEnd; $range++)
{
    $prefix = $range.ToString('X5')
    if (-not $hibpState.ContainsKey($prefix))
    {
        $missingValidatorCount++
        if ($missingValidatorExamples.Count -lt 10)
        {
            $missingValidatorExamples.Add($prefix)
        }
    }
}

if ($missingValidatorCount -gt 0)
{
    $examples = [string]::Join(', ', $missingValidatorExamples.ToArray())
    throw "The saved HIBP state has no ETag for $missingValidatorCount selected prefixes. The first missing prefixes are $examples. The diagnostic stopped because requests without saved validators can download a large amount of data."
}

$proxyMode = 'Direct'
if ($ProxyAddress -ne $null)
{
    $proxyMode = 'Explicit'
}
else
{
    try
    {
        $systemProxy = [Net.WebRequest]::DefaultWebProxy
        if ($systemProxy -ne $null -and -not $systemProxy.IsBypassed($apiBaseAddress))
        {
            $proxyMode = 'System'
        }
    }
    catch
    {
        $proxyMode = 'Unknown'
    }
}

$workerScript =
{
    param
    (
        [int]$WorkerId,
        [int]$WorkerRangeStart,
        [int]$WorkerRangeEnd,
        $HibpState,
        [string]$ApiBaseAddress,
        [string]$ProxyAddress,
        [string]$ProductVersion,
        [int[]]$ProgressCounts,
        $AnomalyQueue,
        $AnomalySlots,
        [Threading.CancellationToken]$CancellationToken
    )

    Set-StrictMode -Version 2.0
    $ErrorActionPreference = 'Stop'

    function Get-ResponseHeader
    {
        param
        (
            [Net.Http.HttpResponseMessage]$Response,
            [string]$Name
        )

        $values = $null
        if ($Response.Headers.TryGetValues($Name, [ref]$values))
        {
            return [string]::Join(', ', [string[]]$values)
        }

        if ($Response.Content -ne $null -and $Response.Content.Headers.TryGetValues($Name, [ref]$values))
        {
            return [string]::Join(', ', [string[]]$values)
        }

        return $null
    }

    function Add-Anomaly
    {
        param
        (
            $Anomaly
        )

        if ($AnomalySlots.Wait(0))
        {
            $AnomalyQueue.Enqueue($Anomaly)
        }
    }

    function New-ResponseAnomaly
    {
        param
        (
            [string]$Classification,
            [string]$Prefix,
            [string]$RequestEtag,
            [int]$AttemptCount,
            [Net.Http.HttpResponseMessage]$Response
        )

        $typedEtag = $null
        if ($Response.Headers.ETag -ne $null)
        {
            $typedEtag = $Response.Headers.ETag.ToString()
        }

        return [pscustomobject][ordered]@{
            TimestampUtc = [DateTime]::UtcNow.ToString('o')
            Prefix = $Prefix
            Classification = $Classification
            AttemptCount = $AttemptCount
            StatusCode = [int]$Response.StatusCode
            ReasonPhrase = $Response.ReasonPhrase
            RequestEtag = $RequestEtag
            ResponseEtag = $typedEtag
            RawResponseEtag = Get-ResponseHeader -Response $Response -Name 'ETag'
            CfRay = Get-ResponseHeader -Response $Response -Name 'CF-Ray'
            CfCacheStatus = Get-ResponseHeader -Response $Response -Name 'CF-Cache-Status'
            Age = Get-ResponseHeader -Response $Response -Name 'Age'
            Date = Get-ResponseHeader -Response $Response -Name 'Date'
            Server = Get-ResponseHeader -Response $Response -Name 'Server'
            ContentMd5 = Get-ResponseHeader -Response $Response -Name 'Content-MD5'
            ContentLength = Get-ResponseHeader -Response $Response -Name 'Content-Length'
            ErrorType = $null
            ErrorCode = $null
        }
    }

    function New-RequestAnomaly
    {
        param
        (
            [string]$Prefix,
            [string]$RequestEtag,
            [int]$AttemptCount,
            [Exception]$Exception
        )

        return [pscustomobject][ordered]@{
            TimestampUtc = [DateTime]::UtcNow.ToString('o')
            Prefix = $Prefix
            Classification = 'RequestError'
            AttemptCount = $AttemptCount
            StatusCode = $null
            ReasonPhrase = $null
            RequestEtag = $RequestEtag
            ResponseEtag = $null
            RawResponseEtag = $null
            CfRay = $null
            CfCacheStatus = $null
            Age = $null
            Date = $null
            Server = $null
            ContentMd5 = $null
            ContentLength = $null
            ErrorType = $Exception.GetType().FullName
            ErrorCode = $Exception.HResult
        }
    }

    Add-Type -AssemblyName 'System.Net.Http'

    $notModified = 0
    $modifiedWithEtag = 0
    $missingEtag = 0
    $invalidEtag = 0
    $unexpectedHttpStatus = 0
    $requestErrors = 0
    $retryAttempts = 0
    $processed = 0
    $maxApiRetries = 5

    $handler = New-Object 'System.Net.Http.HttpClientHandler'
    if (-not [string]::IsNullOrWhiteSpace($ProxyAddress))
    {
        $handler.Proxy = New-Object 'System.Net.WebProxy' ([Uri]$ProxyAddress)
    }

    if ($handler.SupportsAutomaticDecompression)
    {
        $handler.AutomaticDecompression = [Net.DecompressionMethods]::GZip
    }

    $handler.SslProtocols = [Security.Authentication.SslProtocols]::Tls12
    $handler.MaxConnectionsPerServer = 256

    $client = New-Object 'System.Net.Http.HttpClient' ($handler)
    $client.BaseAddress = [Uri]$ApiBaseAddress
    $client.Timeout = [TimeSpan]::FromMinutes(15)
    $client.DefaultRequestHeaders.UserAgent.ParseAdd("LithnetPasswordProtection/$ProductVersion")

    try
    {
        for ($range = $WorkerRangeStart; $range -le $WorkerRangeEnd; $range++)
        {
            if ($CancellationToken.IsCancellationRequested)
            {
                break
            }

            $prefix = $range.ToString('X5')
            $etag = $null
            if (-not $HibpState.TryGetValue($prefix, [ref]$etag))
            {
                throw "The saved ETag for prefix $prefix was not available to the worker."
            }

            $attempt = 0
            $response = $null
            $requestFailed = $false

            while ($true)
            {
                $attempt++
                $request = New-Object 'System.Net.Http.HttpRequestMessage' ([Net.Http.HttpMethod]::Get, "${prefix}?mode=ntlm")

                try
                {
                    $request.Headers.Add('If-None-Match', $etag)
                    $response = $client.SendAsync($request, $CancellationToken).GetAwaiter().GetResult()
                    $statusCode = [int]$response.StatusCode

                    if (($statusCode -eq 408 -or $statusCode -ge 500) -and $attempt -le $maxApiRetries)
                    {
                        $retryAttempts++
                        $response.Dispose()
                        $response = $null
                        $delay = [TimeSpan]::FromSeconds([Math]::Pow(2, $attempt))
                        if ($CancellationToken.WaitHandle.WaitOne($delay))
                        {
                            break
                        }

                        continue
                    }

                    break
                }
                catch [Net.Http.HttpRequestException]
                {
                    if ($attempt -le $maxApiRetries)
                    {
                        $retryAttempts++
                        $delay = [TimeSpan]::FromSeconds([Math]::Pow(2, $attempt))
                        if ($CancellationToken.WaitHandle.WaitOne($delay))
                        {
                            break
                        }

                        continue
                    }

                    $requestErrors++
                    Add-Anomaly -Anomaly (New-RequestAnomaly -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Exception $_.Exception)
                    $requestFailed = $true
                    break
                }
                catch [OperationCanceledException]
                {
                    if (-not $CancellationToken.IsCancellationRequested)
                    {
                        $requestErrors++
                        Add-Anomaly -Anomaly (New-RequestAnomaly -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Exception $_.Exception)
                        $requestFailed = $true
                    }

                    break
                }
                catch
                {
                    $requestErrors++
                    Add-Anomaly -Anomaly (New-RequestAnomaly -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Exception $_.Exception)
                    $requestFailed = $true
                    break
                }
                finally
                {
                    $request.Dispose()
                }
            }

            if (-not $requestFailed -and $response -ne $null)
            {
                try
                {
                    $statusCode = [int]$response.StatusCode
                    if ($statusCode -eq 304)
                    {
                        $notModified++
                    }
                    elseif ($statusCode -eq 200)
                    {
                        $rawEtag = Get-ResponseHeader -Response $response -Name 'ETag'
                        if ($response.Headers.ETag -eq $null)
                        {
                            if ([string]::IsNullOrWhiteSpace($rawEtag))
                            {
                                $missingEtag++
                                Add-Anomaly -Anomaly (New-ResponseAnomaly -Classification 'MissingEtag' -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Response $response)
                            }
                            else
                            {
                                $invalidEtag++
                                Add-Anomaly -Anomaly (New-ResponseAnomaly -Classification 'InvalidEtag' -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Response $response)
                            }
                        }
                        else
                        {
                            $modifiedWithEtag++
                        }
                    }
                    else
                    {
                        $unexpectedHttpStatus++
                        Add-Anomaly -Anomaly (New-ResponseAnomaly -Classification 'UnexpectedHttpStatus' -Prefix $prefix -RequestEtag $etag -AttemptCount $attempt -Response $response)
                    }
                }
                finally
                {
                    $response.Dispose()
                }
            }

            $processed++
            if (($processed % 25) -eq 0 -or $range -eq $WorkerRangeEnd)
            {
                $ProgressCounts[$WorkerId] = $processed
            }
        }
    }
    finally
    {
        $ProgressCounts[$WorkerId] = $processed
        $client.Dispose()
        $handler.Dispose()
    }

    [pscustomobject][ordered]@{
        WorkerId = $WorkerId
        Processed = $processed
        NotModified = $notModified
        ModifiedWithEtag = $modifiedWithEtag
        MissingEtag = $missingEtag
        InvalidEtag = $invalidEtag
        UnexpectedHttpStatus = $unexpectedHttpStatus
        RequestErrors = $requestErrors
        RetryAttempts = $retryAttempts
    }
}

$rangeCount = $RangeEnd - $RangeStart + 1
$workerCount = [Math]::Min($Threads, $rangeCount)
$baseChunkSize = [int][Math]::Floor($rangeCount / [double]$workerCount)
$workersWithExtraRange = $rangeCount % $workerCount
$progressCounts = New-Object 'int[]' $workerCount
$anomalyQueue = New-Object 'System.Collections.Concurrent.ConcurrentQueue[object]'
$anomalySlots = New-Object 'System.Threading.SemaphoreSlim' ($MaximumAnomalies, $MaximumAnomalies)
$cancellationSource = New-Object 'System.Threading.CancellationTokenSource'
$runspacePool = [RunspaceFactory]::CreateRunspacePool(1, $workerCount)
$jobs = New-Object 'System.Collections.Generic.List[object]'
$workerResults = New-Object 'System.Collections.Generic.List[object]'

Write-Host "Testing $rangeCount HIBP prefixes with $workerCount workers."

try
{
    $runspacePool.Open()
    $nextWorkerRangeStart = $RangeStart

    for ($workerId = 0; $workerId -lt $workerCount; $workerId++)
    {
        $workerRangeSize = $baseChunkSize
        if ($workerId -lt $workersWithExtraRange)
        {
            $workerRangeSize++
        }

        $workerRangeStart = $nextWorkerRangeStart
        $workerRangeEnd = $workerRangeStart + $workerRangeSize - 1
        $nextWorkerRangeStart = $workerRangeEnd + 1
        $powerShell = [PowerShell]::Create()
        $powerShell.RunspacePool = $runspacePool

        $null = $powerShell.AddScript($workerScript.ToString())
        $null = $powerShell.AddArgument($workerId)
        $null = $powerShell.AddArgument($workerRangeStart)
        $null = $powerShell.AddArgument($workerRangeEnd)
        $null = $powerShell.AddArgument($hibpState)
        $null = $powerShell.AddArgument($apiBaseAddress.AbsoluteUri)
        $null = $powerShell.AddArgument($(if ($ProxyAddress -eq $null) { $null } else { $ProxyAddress.AbsoluteUri }))
        $null = $powerShell.AddArgument($productVersion)
        $null = $powerShell.AddArgument($progressCounts)
        $null = $powerShell.AddArgument($anomalyQueue)
        $null = $powerShell.AddArgument($anomalySlots)
        $null = $powerShell.AddArgument($cancellationSource.Token)

        $handle = $powerShell.BeginInvoke()
        $jobs.Add([pscustomobject]@{
            PowerShell = $powerShell
            Handle = $handle
        })
    }

    while ($true)
    {
        $allCompleted = $true
        foreach ($job in $jobs)
        {
            if (-not $job.Handle.IsCompleted)
            {
                $allCompleted = $false
                break
            }
        }

        $processedCount = 0
        foreach ($count in $progressCounts)
        {
            $processedCount += $count
        }

        $percentComplete = [Math]::Min(100, [int](($processedCount / [double]$rangeCount) * 100))
        Write-Progress -Activity 'Testing HIBP ETag responses' -Status "$processedCount of $rangeCount prefixes" -PercentComplete $percentComplete

        if ($allCompleted)
        {
            break
        }

        Start-Sleep -Milliseconds 500
    }

    Write-Progress -Activity 'Testing HIBP ETag responses' -Completed

    foreach ($job in $jobs)
    {
        $results = $job.PowerShell.EndInvoke($job.Handle)
        foreach ($result in $results)
        {
            if ($result.PSObject.Properties['WorkerId'] -eq $null)
            {
                $resultText = ($result | Out-String).Trim()
                throw "A diagnostic worker returned unexpected output of type $($result.GetType().FullName): $resultText"
            }

            $workerResults.Add($result)
        }

        if ($job.PowerShell.Streams.Error.Count -gt 0)
        {
            throw $job.PowerShell.Streams.Error[0]
        }
    }
}
catch
{
    $cancellationSource.Cancel()
    throw
}
finally
{
    foreach ($job in $jobs)
    {
        $job.PowerShell.Dispose()
    }

    $runspacePool.Dispose()
    $cancellationSource.Dispose()
    $anomalySlots.Dispose()
}

$anomalies = New-Object 'System.Collections.Generic.List[object]'
$anomaly = $null
while ($anomalyQueue.TryDequeue([ref]$anomaly))
{
    $anomalies.Add($anomaly)
    $anomaly = $null
}

$notModified = ($workerResults | Measure-Object -Property NotModified -Sum).Sum
$modifiedWithEtag = ($workerResults | Measure-Object -Property ModifiedWithEtag -Sum).Sum
$missingEtag = ($workerResults | Measure-Object -Property MissingEtag -Sum).Sum
$invalidEtag = ($workerResults | Measure-Object -Property InvalidEtag -Sum).Sum
$unexpectedHttpStatus = ($workerResults | Measure-Object -Property UnexpectedHttpStatus -Sum).Sum
$requestErrors = ($workerResults | Measure-Object -Property RequestErrors -Sum).Sum
$retryAttempts = ($workerResults | Measure-Object -Property RetryAttempts -Sum).Sum
$totalAnomalyCount = $missingEtag + $invalidEtag + $unexpectedHttpStatus + $requestErrors
$completedAtUtc = [DateTime]::UtcNow

$report = [pscustomobject][ordered]@{
    ReportVersion = 1
    DiagnosticVersion = $diagnosticVersion
    StartedAtUtc = $startedAtUtc.ToString('o')
    CompletedAtUtc = $completedAtUtc.ToString('o')
    Duration = ($completedAtUtc - $startedAtUtc).ToString()
    Location = $Location
    ProductVersion = $productVersion
    PowerShellVersion = $PSVersionTable.PSVersion.ToString()
    OperatingSystem = [Environment]::OSVersion.VersionString
    Is64BitOperatingSystem = [Environment]::Is64BitOperatingSystem
    ProxyMode = $proxyMode
    RangeStart = $RangeStart.ToString('X5')
    RangeEnd = $RangeEnd.ToString('X5')
    RangesRequested = $rangeCount
    SavedEtagCount = $hibpState.Count
    IgnoredMetadataLines = $ignoredMetadataLines
    Summary = [pscustomobject][ordered]@{
        NotModified = $notModified
        ModifiedWithEtag = $modifiedWithEtag
        MissingEtag = $missingEtag
        InvalidEtag = $invalidEtag
        UnexpectedHttpStatus = $unexpectedHttpStatus
        RequestErrors = $requestErrors
        RetryAttempts = $retryAttempts
        AnomalyCount = $totalAnomalyCount
        DetailedAnomaliesCaptured = $anomalies.Count
        DetailedAnomaliesTruncated = $totalAnomalyCount -gt $anomalies.Count
    }
    Anomalies = @($anomalies | Sort-Object -Property Prefix, TimestampUtc)
}

$report | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $fullOutputPath -Encoding UTF8

if ($totalAnomalyCount -gt 0)
{
    Write-Warning "The diagnostic found $totalAnomalyCount anomalous responses. Send the JSON report to Lithnet support."
}
else
{
    Write-Host "The diagnostic did not find an anomalous response."
}

Write-Host "Report written to $fullOutputPath"
$report
