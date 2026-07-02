BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
    Import-CompromisedPasswordHashes -Filename (Join-Path $PSScriptRoot 'TestData\test-hashes.txt') -ErrorAction Stop
}

Describe 'Test-IsADUserPasswordCompromised' {
    Context 'user with a compromised password' {
        It 'returns true for TestUser1' {
            Test-IsADUserPasswordCompromised -AccountName 'TestUser1' -ErrorAction Stop | Should -BeTrue
        }
    }

    Context 'user with a clean password' {
        It 'returns false for TestUser2' {
            Test-IsADUserPasswordCompromised -AccountName 'TestUser2' -ErrorAction Stop | Should -BeFalse
        }
    }
}
