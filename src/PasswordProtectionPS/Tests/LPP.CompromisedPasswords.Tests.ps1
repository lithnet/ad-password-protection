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
            Add-CompromisedPassword -Value 'normalizetest' -ErrorAction Stop
        }

        AfterAll {
            Remove-CompromisedPassword -Value 'normalizetest' -ErrorAction SilentlyContinue
        }

        It 'returns true when a variation matches the normalized base' {
            Test-IsCompromisedPassword -Value 'NormalizeTest!55' -Normalize -ErrorAction Stop | Should -BeTrue
        }
    }
}
