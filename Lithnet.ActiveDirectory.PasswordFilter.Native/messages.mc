; // ***** Sample.mc *****
; // This is the header section.

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
The Lithnet AD Password Filter is currently disabled and has not evaluated the password change request
.

MessageId=0x4
Severity=Success
Facility=System
SymbolicName=MSG_PROCESSING_REQUEST
Language=English
Processing a password %1 request for %2 (%3)
.

MessageId=0x5
Severity=Success
Facility=System
SymbolicName=MSG_PASSWORD_APPROVED
Language=English
The password %1 request for %2 (%3) was approved
.

MessageId=0x6
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED
Language=English
The password %1 request for %2 (%3) was rejected. %4
.

MessageId=0x7
Severity=Warning
Facility=System
SymbolicName=MSG_PASSWORD_REJECTED_ON_ERROR
Language=English
The password %1 request was rejected. The module is configured to deny password requests when an error occurs.
.
