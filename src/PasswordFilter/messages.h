 // The following are message definitions.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_RUNTIME                 0x2
#define FACILITY_STUBS                   0x3
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: MSG_UNEXPECTEDERROR
//
// MessageText:
//
// An unexpected error occurred.
// %1
//
#define MSG_UNEXPECTEDERROR              ((DWORD)0xC0020001L)

//
// MessageId: MSG_AGENT_DISABLED
//
// MessageText:
//
// The password filter is currently disabled and has not evaluated the password change request.
//
#define MSG_AGENT_DISABLED               ((DWORD)0x80040002L)

//
// MessageId: MSG_AGENT_INITIALIZED
//
// MessageText:
//
// The password filter has been successfully loaded.
//
#define MSG_AGENT_INITIALIZED            ((DWORD)0x00040003L)

//
// MessageId: MSG_PROCESSING_REQUEST
//
// MessageText:
//
// Processing a password %1 request for %2 (%3).
//
#define MSG_PROCESSING_REQUEST           ((DWORD)0x00000004L)

//
// MessageId: MSG_PASSWORD_APPROVED
//
// MessageText:
//
// The password %1 request for %2 (%3) was approved.
//
#define MSG_PASSWORD_APPROVED            ((DWORD)0x00000005L)

//
// MessageId: MSG_WIN32ERROR
//
// MessageText:
//
// An unexpected error occurred.
// Error code: %1
// Message: %2
//
#define MSG_WIN32ERROR                   ((DWORD)0xC0020008L)

//
// MessageId: MSG_STOREERROR
//
// MessageText:
//
// There was a problem opening the store file. Check that the store folder exists and is accessible.\n
// Error code: %1
// Path: %2
//
#define MSG_STOREERROR                   ((DWORD)0xC0020009L)

//
// MessageId: MSG_PASSWORD_REJECTED_ON_ERROR
//
// MessageText:
//
// The password %1 request was rejected. The module is configured to deny password requests when an error occurs.
//
#define MSG_PASSWORD_REJECTED_ON_ERROR   ((DWORD)0x80002001L)

//
// MessageId: MSG_PASSWORD_REJECTED_MINLENGTH
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because its length (%4) did not meet the minimum configured length (%5).
//
#define MSG_PASSWORD_REJECTED_MINLENGTH  ((DWORD)0x80002002L)

//
// MessageId: MSG_PASSWORD_REJECTED_BANNED
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it matched a password in the compromised password store.
//
#define MSG_PASSWORD_REJECTED_BANNED     ((DWORD)0x80002003L)

//
// MessageId: MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected after being normalized because it matched a password in the compromised password store.
//
#define MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_PASSWORD ((DWORD)0x80002004L)

//
// MessageId: MSG_PASSWORD_REJECTED_APPROVAL_REGEX
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it did not match the approval regular expression.
//
#define MSG_PASSWORD_REJECTED_APPROVAL_REGEX ((DWORD)0x80002005L)

//
// MessageId: MSG_PASSWORD_REJECTED_REJECTION_REGEX
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it matched the rejection regular expression.
//
#define MSG_PASSWORD_REJECTED_REJECTION_REGEX ((DWORD)0x80002006L)

//
// MessageId: MSG_PASSWORD_REJECTED_NOT_ENOUGH_POINTS
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it achieved only %4 of the required %5 complexity points.
//
#define MSG_PASSWORD_REJECTED_NOT_ENOUGH_POINTS ((DWORD)0x80002007L)

//
// MessageId: MSG_PASSWORD_REJECTED_BELOW_THRESHOLD
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password below the specified threshold.
//
#define MSG_PASSWORD_REJECTED_BELOW_THRESHOLD ((DWORD)0x80002008L)

//
// MessageId: MSG_PASSWORD_REJECTED_ABOVE_THRESHOLD
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password above the specified threshold.
//
#define MSG_PASSWORD_REJECTED_ABOVE_THRESHOLD ((DWORD)0x80002009L)

//
// MessageId: MSG_PASSWORD_REJECTED_CONTAINS_ACCOUNTNAME
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it contained the account name
//
#define MSG_PASSWORD_REJECTED_CONTAINS_ACCOUNTNAME ((DWORD)0x8000200AL)

//
// MessageId: MSG_PASSWORD_REJECTED_CONTAINS_DISPLAYNAME
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it contained at least part of the display name
//
#define MSG_PASSWORD_REJECTED_CONTAINS_DISPLAYNAME ((DWORD)0x8000200BL)

//
// MessageId: MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected because it did not meet the complexity requirements of a password of %4 characters.
//
#define MSG_PASSWORD_REJECTED_THRESHOLD_COMPLEXITY ((DWORD)0x8000200CL)

//
// MessageId: MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_WORD
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected after being normalized because it matched a password in the banned word store.
//
#define MSG_PASSWORD_REJECTED_BANNED_NORMALIZED_WORD ((DWORD)0x8000200DL)

