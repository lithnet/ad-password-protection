if ($null -eq $($env:releaseType))
{
    write-error "release type not specified"
}

if ($null -eq $($env:ARTIFACTSLIST_ARTIFACTSPRESENT))
{
    Write-Warning "No artifacts present"
    return
}

$env:AZCOPY_SPA_CLIENT_SECRET = $env:servicePrincipalKey
azcopy login --service-principal --application-id $env:servicePrincipalId --tenant-id $env:tenantId              

function UploadFiles([string]$artifact, [string]$destinationPath, [string]$fileName, [string]$arch)
{
    if ($($env:ARTIFACTSLIST_ARTIFACTSPRESENT) -notmatch $artifact)
    {
        Write-Warning "Artifact $artifact not present in build"
        Write-Host "##vso[task.logissue type=warning]Artifact $artifact not present in build"
        return
    }
   
    New-Item "$($env:Pipeline_Workspace)/$artifact/$($env:ReleaseType).lnk" -ItemType File -Value $fileName

    $fullArtifactPath = "$($env:Pipeline_Workspace)/$artifact/$fileName"

    $obj = @{
        Component      = "$($artifact)"
        CurrentVersion = "$($env:build_version)"
        ReleaseDate    = "$($env:build_date)"
        UserUrl        = "https://lithnet.io/products/password-protection/downloads"
        Assets         = @{
            Arch   = "$arch"
            Uri    = "$destinationPath/$fileName"
            Size   = $((Get-Item $fullArtifactPath).Length)
            Hashes = @{
                SHA1   = "$((Get-FileHash -Path $fullArtifactPath -Algorithm SHA1).Hash)"
                SHA256 = "$((Get-FileHash -Path $fullArtifactPath -Algorithm SHA256).Hash)"
                SHA384 = "$((Get-FileHash -Path $fullArtifactPath -Algorithm SHA384).Hash)"
                SHA512 = "$((Get-FileHash -Path $fullArtifactPath -Algorithm SHA512).Hash)"
            }
        }
    }
    
    $manifestOutPath = "$($env:Pipeline_Workspace)/$($artifact)/$($fileName).manifest.json"
    $obj | ConvertTo-Json -Depth 99 | Out-File $manifestOutPath 

    Write-Host "Manifest created"
    Get-Content "$($manifestOutPath)"

    $fileSource = "$($env:Pipeline_Workspace)/$artifact/*"
    $fileDestination = "$($env:BLOBSTORAGE_URL)/$($env:BLOBSTORAGE_CONTAINER)/$destinationPath"
    Write-Host "Copying artifact $artifact from $fileSource to $fileDestination" -ForegroundColor White
    azcopy.exe copy "$fileSource" "$fileDestination" --log-level=INFO --recursive --overwrite true

    Write-Host "Uploaded artifact $artifact" -ForegroundColor White
}

UploadFiles -artifact "installer" -destinationPath "$($env:package_path)" -fileName "$($env:package_name)" -arch "x64"
