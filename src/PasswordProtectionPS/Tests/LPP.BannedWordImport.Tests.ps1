BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
}

Describe 'Import banned words' {
    Context 'from text file' {
        BeforeAll {
            Import-BannedWords -Filename (Join-Path $PSScriptRoot 'TestData\test-banned-words.txt') -ErrorAction Stop
        }

        It 'detects password as a banned word' {
            Test-IsBannedWord -Value 'password' -ErrorAction Stop | Should -BeTrue
        }

        It 'detects lithnet as a banned word' {
            Test-IsBannedWord -Value 'lithnet' -ErrorAction Stop | Should -BeTrue
        }

        It 'detects company as a banned word' {
            Test-IsBannedWord -Value 'company' -ErrorAction Stop | Should -BeTrue
        }

        It 'does not flag a word that was not imported' {
            Test-IsBannedWord -Value 'xylophone' -ErrorAction Stop | Should -BeFalse
        }
    }
}
