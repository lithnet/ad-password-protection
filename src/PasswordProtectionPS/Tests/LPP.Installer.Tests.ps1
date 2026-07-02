Describe 'LPP installation' {
    BeforeAll {
        $script:modulePath = 'C:\Program Files\WindowsPowerShell\Modules\LithnetPasswordProtection'
    }

    Context 'module directory' {
        It 'exists on disk' {
            Test-Path $modulePath | Should -BeTrue
        }
    }

    Context 'required DLLs' {
        It 'contains LithnetPasswordProtection.dll' {
            Test-Path (Join-Path $modulePath 'LithnetPasswordProtection.dll') | Should -BeTrue
        }

        It 'contains Lithnet.ActiveDirectory.PasswordProtection.dll' {
            Test-Path (Join-Path $modulePath 'Lithnet.ActiveDirectory.PasswordProtection.dll') | Should -BeTrue
        }

        It 'contains Polly.dll' {
            Test-Path (Join-Path $modulePath 'Polly.dll') | Should -BeTrue
        }

        It 'contains Polly.Extensions.Http.dll' {
            Test-Path (Join-Path $modulePath 'Polly.Extensions.Http.dll') | Should -BeTrue
        }

        It 'contains Microsoft.Extensions.Http.Polly.dll' {
            Test-Path (Join-Path $modulePath 'Microsoft.Extensions.Http.Polly.dll') | Should -BeTrue
        }
    }

    Context 'module import' {
        It 'imports without error' {
            { Import-Module LithnetPasswordProtection -ErrorAction Stop } | Should -Not -Throw
        }
    }

    Context 'exported commands' {
        BeforeAll {
            Import-Module LithnetPasswordProtection -ErrorAction Stop
            $script:commands = Get-Command -Module LithnetPasswordProtection
        }

        It 'exports exactly 17 commands' {
            $script:commands.Count | Should -Be 17
        }

        It 'exports Open-Store' {
            $script:commands.Name | Should -Contain 'Open-Store'
        }

        It 'exports Add-CompromisedPassword' {
            $script:commands.Name | Should -Contain 'Add-CompromisedPassword'
        }

        It 'exports Test-IsCompromisedPassword' {
            $script:commands.Name | Should -Contain 'Test-IsCompromisedPassword'
        }

        It 'exports Remove-CompromisedPassword' {
            $script:commands.Name | Should -Contain 'Remove-CompromisedPassword'
        }

        It 'exports Import-CompromisedPasswords' {
            $script:commands.Name | Should -Contain 'Import-CompromisedPasswords'
        }

        It 'exports Import-CompromisedPasswordHashes' {
            $script:commands.Name | Should -Contain 'Import-CompromisedPasswordHashes'
        }

        It 'exports Add-BannedWord' {
            $script:commands.Name | Should -Contain 'Add-BannedWord'
        }

        It 'exports Test-IsBannedWord' {
            $script:commands.Name | Should -Contain 'Test-IsBannedWord'
        }

        It 'exports Remove-BannedWord' {
            $script:commands.Name | Should -Contain 'Remove-BannedWord'
        }

        It 'exports Import-BannedWords' {
            $script:commands.Name | Should -Contain 'Import-BannedWords'
        }

        It 'exports Import-BannedWordHashes' {
            $script:commands.Name | Should -Contain 'Import-BannedWordHashes'
        }

        It 'exports Sync-HashesFromHibp' {
            $script:commands.Name | Should -Contain 'Sync-HashesFromHibp'
        }

        It 'exports Get-PasswordFilterConfig' {
            $script:commands.Name | Should -Contain 'Get-PasswordFilterConfig'
        }

        It 'exports Set-PasswordFilterConfig' {
            $script:commands.Name | Should -Contain 'Set-PasswordFilterConfig'
        }

        It 'exports Get-PasswordFilterPolicy' {
            $script:commands.Name | Should -Contain 'Get-PasswordFilterPolicy'
        }

        It 'exports Get-PasswordFilterResult' {
            $script:commands.Name | Should -Contain 'Get-PasswordFilterResult'
        }

        It 'exports Test-IsADUserPasswordCompromised' {
            $script:commands.Name | Should -Contain 'Test-IsADUserPasswordCompromised'
        }
    }

    Context 'installed version' {
        It 'matches the expected version from environment variable' {
            if (-not $env:LPP_EXPECTED_VERSION) {
                Set-ItResult -Skipped -Because 'LPP_EXPECTED_VERSION environment variable is not set'
                return
            }

            $mod = Get-Module LithnetPasswordProtection -ListAvailable | Select-Object -First 1
            $mod | Should -Not -BeNull
            $mod.Version.ToString() | Should -Be $env:LPP_EXPECTED_VERSION
        }
    }
}
