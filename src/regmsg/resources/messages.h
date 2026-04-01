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
#define FACILITY_APPLICATION             0x1
#define FACILITY_SYSTEM                  0x0


//
// Define the severity codes
//
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: CATEGORY_GENERIC
//
// MessageText:
//
// Application
//
#define CATEGORY_GENERIC                 ((WORD)0x00000001L)

//
// MessageId: CATEGORY_CONFIGURATION
//
// MessageText:
//
// Configuration
//
#define CATEGORY_CONFIGURATION           ((WORD)0x00000002L)

//
// MessageId: CATEGORY_SERVICE
//
// MessageText:
//
// Service
//
#define CATEGORY_SERVICE                 ((WORD)0x00000003L)

//
// MessageId: CATEGORY_NETWORK
//
// MessageText:
//
// Network
//
#define CATEGORY_NETWORK                 ((WORD)0x00000004L)

//
// MessageId: CATEGORY_WEB_SERVER
//
// MessageText:
//
// Web Server
//
#define CATEGORY_WEB_SERVER              ((WORD)0x00000005L)

//
// MessageId: CATEGORY_CASH_REGISTER
//
// MessageText:
//
// Cash register
//
#define CATEGORY_CASH_REGISTER           ((WORD)0x00000006L)

//
// MessageId: MSG_GENERIC_INFO
//
// MessageText:
//
// INFO: %1
//
#define MSG_GENERIC_INFO                 ((DWORD)0x40010101L)

//
// MessageId: MSG_GENERIC_INFO_WITH_SOURCE
//
// MessageText:
//
// INFO: %1%nSOURCE: %2
//
#define MSG_GENERIC_INFO_WITH_SOURCE     ((DWORD)0x40010102L)

//
// MessageId: MSG_GENERIC_WARNING
//
// MessageText:
//
// WARNING: %1
//
#define MSG_GENERIC_WARNING              ((DWORD)0x80010103L)

//
// MessageId: MSG_GENERIC_WARNING_WITH_SOURCE
//
// MessageText:
//
// WARNING: %1%nSOURCE: %2
//
#define MSG_GENERIC_WARNING_WITH_SOURCE  ((DWORD)0x80010104L)

//
// MessageId: MSG_GENERIC_ERROR
//
// MessageText:
//
// ERROR: %1
//
#define MSG_GENERIC_ERROR                ((DWORD)0xC0010105L)

//
// MessageId: MSG_GENERIC_ERROR_WITH_SOURCE
//
// MessageText:
//
// ERROR: %1\n%nSOURCE: %2
//
#define MSG_GENERIC_ERROR_WITH_SOURCE    ((DWORD)0xC0010106L)

