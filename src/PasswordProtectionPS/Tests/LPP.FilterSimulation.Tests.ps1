BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
    Import-CompromisedPasswords -Filename (Join-Path $PSScriptRoot 'TestData\test-passwords.txt') -ErrorAction Stop
    Import-BannedWords -Filename (Join-Path $PSScriptRoot 'TestData\test-banned-words.txt') -ErrorAction Stop
}

Describe 'Get-PasswordFilterResult' {
    Context 'compromised password' {
        It 'rejects a known compromised password' {
            $result = Get-PasswordFilterResult -Password 'KnownPassword123!' -Username 'testuser' -Fullname 'Test User' -ErrorAction Stop
            $result | Should -Not -Be 'Approved'
        }
    }

    Context 'clean password' {
        It 'approves a password that is not compromised or banned' {
            $result = Get-PasswordFilterResult -Password 'ABCxyz!@#789qwerty' -Username 'testuser' -Fullname 'Test User' -ErrorAction Stop
            $result | Should -Be 'Approved'
        }
    }

    Context 'banned word password' {
        It 'rejects a password based on a banned word' {
            $result = Get-PasswordFilterResult -Password 'lithnet123!' -Username 'testuser' -Fullname 'Test User' -ErrorAction Stop
            $result | Should -Not -Be 'Approved'
        }
    }
}
