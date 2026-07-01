BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
}

Describe 'Password filter configuration' {
    Context 'Get-PasswordFilterConfig' {
        BeforeAll {
            $script:config = Get-PasswordFilterConfig -ErrorAction Stop
        }

        It 'reports the filter as registered' {
            $script:config.IsFilterRegistered | Should -BeTrue
        }

        It 'reports the filter as enabled' {
            $script:config.IsFilterEnabled | Should -BeTrue
        }
    }

    Context 'Set-PasswordFilterConfig' {
        It 'persists the store path without error' {
            Set-PasswordFilterConfig -StorePath 'C:\Password-Protection\store' -ErrorAction Stop
            $config = Get-PasswordFilterConfig -ErrorAction Stop
            $config.StorePath | Should -Be 'C:\Password-Protection\store'
        }
    }

    Context 'Get-PasswordFilterPolicy' {
        It 'returns without error' {
            { Get-PasswordFilterPolicy -ErrorAction Stop } | Should -Not -Throw
        }
    }
}
