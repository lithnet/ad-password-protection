using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public enum PasswordTestResult : int
    {
        Approved = 0,
        LengthRequirementsNotMet = 1,
        ComplexityThresholdNotMet = 2,
        ComplexityPointsNotMet = 3,
        DidNotMatchApprovalRegex = 4,
        MatchedRejectRegex = 5,
        ContainsAccountName = 6,
        ContainsFullName = 7,
        Compromised = 8,
        CompromisedNormalizedPassword = 9,
        BannedNormalizedWord = 10,
        PasswordWasBlank = 11,
        FilterError = 100,
    }
}