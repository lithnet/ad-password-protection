BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
}

Describe 'Open-Store' {
    Context 'with explicit path' {
        It 'returns the correct StorePath' {
            $result = Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
            $result.StorePath | Should -Be 'C:\Password-Protection\store'
        }
    }

    Context 'with registry default' {
        It 'opens the store without a path argument' {
            $result = Open-Store -ErrorAction Stop
            $result.StorePath | Should -Not -BeNullOrEmpty
        }

        It 'returns a valid store path' {
            $result = Open-Store -ErrorAction Stop
            Test-Path $result.StorePath | Should -BeTrue
        }
    }
}
