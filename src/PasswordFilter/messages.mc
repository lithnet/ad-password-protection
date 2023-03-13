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

MessageId=0x3
Severity=Success
Facility=Io
SymbolicName=MSG_AGENT_INITIALIZED
Language=English
The password filter has been successfully loaded.
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
There was a problem opening the store file. Check that the store folder exists and is accessible.
Error code: %1
Path: %2
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
