BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
}

Describe 'Compromised password operations' {
    Context 'add and test' {
        BeforeAll {
            $testPassword = 'PesterTestPassword!99'
            Add-CompromisedPassword -Value $testPassword -ErrorAction Stop
        }

        AfterAll {
            Remove-CompromisedPassword -Value $testPassword -ErrorAction SilentlyContinue
        }

        It 'returns true for a password that was added to the store' {
            Test-IsCompromisedPassword -Value $testPassword -ErrorAction Stop | Should -BeTrue
        }

        It 'returns false for a password not in the store' {
            Test-IsCompromisedPassword -Value 'NeverAddedPassword!42' -ErrorAction Stop | Should -BeFalse
        }
    }

    Context 'remove' {
        BeforeAll {
            $removePassword = 'PesterRemoveTest!77'
            Add-CompromisedPassword -Value $removePassword -ErrorAction Stop
            Remove-CompromisedPassword -Value $removePassword -ErrorAction Stop
        }

        It 'returns false after the password is removed from the store' {
            Test-IsCompromisedPassword -Value $removePassword -ErrorAction Stop | Should -BeFalse
        }
    }

    Context 'normalize flag' {
        BeforeAll {
            $normalizePassword = 'NormalizeTest!55'
            Add-CompromisedPassword -Value $normalizePassword -ErrorAction Stop
        }

        AfterAll {
            Remove-CompromisedPassword -Value $normalizePassword -ErrorAction SilentlyContinue
        }

        It 'returns true when testing with -Normalize' {
            Test-IsCompromisedPassword -Value $normalizePassword -Normalize -ErrorAction Stop | Should -BeTrue
        }
    }
}
