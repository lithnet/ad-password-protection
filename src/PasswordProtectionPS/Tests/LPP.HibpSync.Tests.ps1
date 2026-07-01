BeforeAll {
    Import-Module LithnetPasswordProtection -ErrorAction Stop
    Open-Store -Path 'C:\Password-Protection\store' -ErrorAction Stop
}

Describe 'Sync-HashesFromHibp' {
    Context 'small range sync' {
        BeforeAll {
            $script:result = Sync-HashesFromHibp -RangeStart 0x0000 -RangeEnd 0x000F -ErrorAction Stop
        }

        It 'retrieves at least one page' {
            $script:result.PagesRetrieved | Should -BeGreaterThan 0
        }

        It 'imports at least one new hash' {
            $script:result.NewHashesImported | Should -BeGreaterThan 0
        }
    }
}
