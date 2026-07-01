BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
}

Describe 'Banned word operations' {
    Context 'add and test' {
        BeforeAll {
            $testWord = 'pestertestword'
            Add-BannedWord -Value $testWord -ErrorAction Stop
        }

        AfterAll {
            Remove-BannedWord -Value $testWord -ErrorAction SilentlyContinue
        }

        It 'returns true for a word that was added to the store' {
            Test-IsBannedWord -Value $testWord -ErrorAction Stop | Should -BeTrue
        }

        It 'returns false for a word not in the store' {
            Test-IsBannedWord -Value 'neveraddedword' -ErrorAction Stop | Should -BeFalse
        }
    }

    Context 'normalization' {
        BeforeAll {
            $normalizedWord = 'normalizeword'
            Add-BannedWord -Value $normalizedWord -ErrorAction Stop
        }

        AfterAll {
            Remove-BannedWord -Value $normalizedWord -ErrorAction SilentlyContinue
        }

        It 'matches an uppercase variant against a lowercase entry' {
            Test-IsBannedWord -Value 'NORMALIZEWORD' -ErrorAction Stop | Should -BeTrue
        }

        It 'matches a mixed-case variant against a lowercase entry' {
            Test-IsBannedWord -Value 'NormalizeWord' -ErrorAction Stop | Should -BeTrue
        }
    }

    Context 'remove' {
        BeforeAll {
            $removeWord = 'pesterremoveword'
            Add-BannedWord -Value $removeWord -ErrorAction Stop
            Remove-BannedWord -Value $removeWord -ErrorAction Stop
        }

        It 'returns false after the word is removed from the store' {
            Test-IsBannedWord -Value $removeWord -ErrorAction Stop | Should -BeFalse
        }
    }
}
