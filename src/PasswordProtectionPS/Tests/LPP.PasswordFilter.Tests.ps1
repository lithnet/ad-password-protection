BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Import-Module ActiveDirectory -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
    Import-CompromisedPasswords -Filename (Join-Path $PSScriptRoot 'TestData\test-passwords.txt') -ErrorAction Stop
    Import-BannedWords -Filename (Join-Path $PSScriptRoot 'TestData\test-banned-words.txt') -ErrorAction Stop

    $script:testUser1 = 'TestUser1'
    $script:domainDns = (Get-ADDomain).DNSRoot
    $script:bootTime = (Get-CimInstance Win32_OperatingSystem).LastBootUpTime
}

Describe 'LSASS password filter' {
    Context 'filter registration' {
        It 'is listed in LSA Notification Packages' {
            $packages = (Get-ItemProperty 'HKLM:\SYSTEM\CurrentControlSet\Control\Lsa' -ErrorAction Stop).'Notification Packages'
            $packages | Should -Contain 'lithnetpwdf'
        }

        It 'is reported as registered by Get-PasswordFilterConfig' {
            $config = Get-PasswordFilterConfig -ErrorAction Stop
            $config.IsFilterRegistered | Should -BeTrue
        }
    }

    Context 'filter DLL loaded by LSASS' {
        It 'has lithnetpwdf.dll loaded in the lsass process' {
            $lsassModules = try {
                (Get-Process lsass -ErrorAction Stop).Modules | ForEach-Object { $_.ModuleName }
            }
            catch {
                $null
            }

            if ($null -eq $lsassModules) {
                $dllPath = Join-Path $env:SystemRoot 'System32\lithnetpwdf.dll'
                Test-Path $dllPath | Should -BeTrue
            }
            else {
                $lsassModules | Should -Contain 'lithnetpwdf.dll'
            }
        }
    }

    Context 'password change enforcement' {
        It 'rejects a compromised password' {
            $cleanPassword = ConvertTo-SecureString 'Tmp$etup!Pwd4Test' -AsPlainText -Force
            Set-ADAccountPassword -Identity $script:testUser1 -Reset -NewPassword $cleanPassword -Server $script:domainDns -ErrorAction Stop

            $compromisedPassword = ConvertTo-SecureString 'KnownPassword123!' -AsPlainText -Force
            {
                Set-ADAccountPassword -Identity $script:testUser1 -Reset -NewPassword $compromisedPassword -Server $script:domainDns -ErrorAction Stop
            } | Should -Throw
        }

        It 'rejects a banned-word-based password' {
            $bannedPassword = ConvertTo-SecureString 'lithnet123!' -AsPlainText -Force
            {
                Set-ADAccountPassword -Identity $script:testUser1 -Reset -NewPassword $bannedPassword -Server $script:domainDns -ErrorAction Stop
            } | Should -Throw
        }

        It 'accepts a clean complex password' {
            $cleanPassword = ConvertTo-SecureString 'V@lid8edCle@nPwd!2026' -AsPlainText -Force
            {
                Set-ADAccountPassword -Identity $script:testUser1 -Reset -NewPassword $cleanPassword -Server $script:domainDns -ErrorAction Stop
            } | Should -Not -Throw
        }
    }

    Context 'event log' {
        It 'has no LPP errors in the Application log since boot' {
            $errors = Get-WinEvent -FilterHashtable @{
                LogName   = 'Application'
                Level     = 2
                StartTime = $script:bootTime
            } -ErrorAction SilentlyContinue | Where-Object { $_.ProviderName -match 'lithnet|passwordprotection|lithnetpwdf' }

            $errors | Should -BeNullOrEmpty
        }

        It 'has no LPP errors in the System log since boot' {
            $errors = Get-WinEvent -FilterHashtable @{
                LogName   = 'System'
                Level     = 2
                StartTime = $script:bootTime
            } -ErrorAction SilentlyContinue | Where-Object { $_.ProviderName -match 'lithnet|passwordprotection|lithnetpwdf' }

            $errors | Should -BeNullOrEmpty
        }
    }

}
