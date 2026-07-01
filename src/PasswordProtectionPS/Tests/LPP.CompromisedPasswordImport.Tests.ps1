BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
}

Describe 'Import compromised passwords' {
    Context 'from plaintext file' {
        BeforeAll {
            Import-CompromisedPasswords -Filename 'C:\LPP-TestData\test-passwords.txt' -ErrorAction Stop
        }

        It 'detects KnownPassword123! as compromised' {
            Test-IsCompromisedPassword -Value 'KnownPassword123!' -ErrorAction Stop | Should -BeTrue
        }

        It 'detects Password1! as compromised' {
            Test-IsCompromisedPassword -Value 'Password1!' -ErrorAction Stop | Should -BeTrue
        }

        It 'detects Summer2024! as compromised' {
            Test-IsCompromisedPassword -Value 'Summer2024!' -ErrorAction Stop | Should -BeTrue
        }
    }

    Context 'from NTLM hash file' {
        BeforeAll {
            Import-CompromisedPasswordHashes -Filename 'C:\LPP-TestData\test-hashes.txt' -ErrorAction Stop
        }

        It 'detects the hash for KnownPassword123! as compromised' {
            $hashBytes = [byte[]]@(0xb3,0x34,0xaa,0x6d,0xd2,0x6d,0x7f,0x29,0x69,0xd1,0xdc,0xf5,0x07,0x75,0x9b,0xac)
            Test-IsCompromisedPassword -Hash $hashBytes -ErrorAction Stop | Should -BeTrue
        }

        It 'detects the hash for Password1! as compromised' {
            $hashBytes = [byte[]]@(0x7f,0xac,0xdc,0x49,0x8e,0xd1,0x68,0x0c,0x4f,0xd1,0x44,0x83,0x19,0xa8,0xc0,0x4f)
            Test-IsCompromisedPassword -Hash $hashBytes -ErrorAction Stop | Should -BeTrue
        }

        It 'detects the hash for Summer2024! as compromised' {
            $hashBytes = [byte[]]@(0x72,0xf0,0xee,0xfc,0xc2,0x13,0xea,0x8f,0x35,0x07,0x73,0xb8,0x31,0xcf,0x2c,0x9c)
            Test-IsCompromisedPassword -Hash $hashBytes -ErrorAction Stop | Should -BeTrue
        }
    }
}
