 // ***** Sample.mc *****
 // This is the header section.
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
// The Lithnet AD Password Filter is currently disabled and has not evaluated the password change request
//
#define MSG_AGENT_DISABLED               ((DWORD)0x80040002L)

//
// MessageId: MSG_PROCESSING_REQUEST
//
// MessageText:
//
// Processing a password %1 request for %2 (%3)
//
#define MSG_PROCESSING_REQUEST           ((DWORD)0x00000004L)

//
// MessageId: MSG_PASSWORD_APPROVED
//
// MessageText:
//
// The password %1 request for %2 (%3) was approved
//
#define MSG_PASSWORD_APPROVED            ((DWORD)0x00000005L)

//
// MessageId: MSG_PASSWORD_REJECTED
//
// MessageText:
//
// The password %1 request for %2 (%3) was rejected. %4
//
#define MSG_PASSWORD_REJECTED            ((DWORD)0x80000006L)

//
// MessageId: MSG_PASSWORD_REJECTED_ON_ERROR
//
// MessageText:
//
// The password %1 request was rejected. The module is configured to deny password requests when an error occurs.
//
#define MSG_PASSWORD_REJECTED_ON_ERROR   ((DWORD)0x80000007L)

