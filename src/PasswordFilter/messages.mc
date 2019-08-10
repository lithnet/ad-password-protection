MessageIdTypedef=DWORD

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
Warning=0x2:STATUS_SEVERITY_WARNING
Error=0x3:STATUS_SEVERITY_ERROR
)

FacilityNames=(System=0x0:FACILITY_SYSTEM
Runtime=0x2:FACILITY_RUNTIME
Stubs=0x3:FACILITY_STUBS
Io=0x4:FACILITY_IO_ERROR_CODE
)

LanguageNames=(English=0x409:MSG00409)

; // The following are message definitions.

MessageId=0x1
Severity=Error
Facility=Runtime
SymbolicName=MSG_UNEXPECTEDERROR
Language=English
An unexpected error occurred.
%1
.

MessageId=0x2
Severity=Warning
Facility=Io
SymbolicName=MSG_AGENT_DISABLED
Language=English
The password filter is currently disabled and has not evaluated the password change request.
.

MessageId=0x4
Severity=Success
Facility=System
SymbolicName=MSG_PROCESSING_REQUEST
Language=English
Processing a password %1 request for %2 (%3).
.

MessageId=0x5
Severity=Success
Facility=System
SymbolicName=MSG_PASSWORD_APPROVED
Language=English
The password %1 request for %2 (%3) was approved.
.

MessageId=0x8
Severity=Error
Facility=Runtime
SymbolicName=MSG_WIN32ERROR
Language=English
An unexpected error occurred.
Error code: %1
Message: %2
.

MessageId=0x9
Severity=Error
Facility=Runtime
SymbolicName=MSG_STOREERROR
Language=English
There was a problem opening the store file. Check that the store folder exists and is accessible
Error code: %1
.

MessageId=0xA
Severity=Error
Facility=Runtime
SymbolicName=MSG_WIN32_HIBP_ERROR
Language=English
An error occurred calling the HIBP API.
Error code: %1
Message: %2
.

MessageId=0xB
Severity=Error
Facility=Runtime
SymbolicName=MSG_OTHER_HIBP_ERROR
Language=English
An error occurred calling the HIBP API.
Message: %1
.

MessageId=0x2001
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_ON_ERROR
Language=English
The password %1 request was rejected. The module is configured to deny password requests when an error occurs.
.

MessageId=0x2002
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_MINLENGTH
Language=English
The password %1 request for %2 (%3) was rejected because its length (%4) did not meet the minimum configured length (%5).
.

MessageId=0x2003
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_BANNED
Language=English
The password %1 request for %2 (%3) was rejected because it matched a password in the compromised password store.
.

MessageId=0x2004
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD
Language=English
The password %1 request for %2 (%3) was rejected after being normalized because it matched a password in the compromised password store.
.

MessageId=0x2005
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_APPROVAL_REGEX
Language=English
The password %1 request for %2 (%3) was rejected because it did not match the approval regular expression.
.

MessageId=0x2006
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_REJECTION_REGEX
Language=English
The password %1 request for %2 (%3) was rejected because it matched the rejection regular expression.
.

MessageId=0x2007
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_NOT_ENOUGH_POINTS
Language=English
The password %1 request for %2 (%3) was rejected because it achieved only %4 of the required %5 complexity points.
.

MessageId=0x2008
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_BELOW_THRESHOLD
Language=English
The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password below the specified threshold.
.

MessageId=0x2009
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_ABOVE_THRESHOLD
Language=English
The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password above the specified threshold.
.

MessageId=0x200A
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_CONTAINS_ACCOUNTNAME
Language=English
The password %1 request for %2 (%3) was rejected because it contained the account name
.

MessageId=0x200B
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_CONTAINS_DISPLAYNAME
Language=English
The password %1 request for %2 (%3) was rejected because it contained at least part of the display name
.

MessageId=0x200C
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY
Language=English
The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password of %4 characters.
.

MessageId=0x200D
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_WORD
Language=English
The password %1 request for %2 (%3) was rejected after being normalized because it matched a password in the banned word store.
.

MessageId=0x200E
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_HIBP_API
Language=English
The password %1 request for %2 (%3) was rejected as it was found in the HIBP API.
.

MessageId=0x200F
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_ON_HIBP_ERROR
Language=English
The password %1 request was rejected. The module is configured to deny password requests when an error occurs when communicating with the HIBP API.
.

MessageId=0x2010
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_PASSED_ON_HIBP_ERROR
Language=English
The password %1 request passed through the HIBP API module without being tested. The HIBP module is currently configured to silently pass through password requests when an error occurs when communicating with the HIBP API. Other modules may yet reject this password change.
.

MessageId=0x3000
Severity=Error
Facility=Runtime
SymbolicName=MSG_GROUP_MAPPING_ERROR
Language=English
An error occurring during the process of mapping policy groups to Active Directory groups. Check that the group name listed in password policy set %3 exists.
Error code: %1
Message: %2
.

MessageId=0xD001
Severity=Informational
Facility=Runtime
SymbolicName=MSG_POLICY_MAPPING_NOT_CONFIGURED
Language=English
There were no policy sets configured. User %1 will recieve default policies
.

MessageId=0xD002
Severity=Informational
Facility=Runtime
SymbolicName=MSG_POLICY_MAPPING_COMPLETE
Language=English
User '%1' was mapped to policy set '%2' due to membership in group '%3'
.

MessageId=0xD003
Severity=Informational
Facility=Runtime
SymbolicName=MSG_POLICY_MAPPING_DEFAULTED
Language=English
User '%1' was mapped to the defauly policy set as they did not match any policy set groups
.
