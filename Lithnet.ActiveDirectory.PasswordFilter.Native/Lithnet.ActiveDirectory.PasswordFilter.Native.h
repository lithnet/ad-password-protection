// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//#ifdef LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_EXPORTS
//#define LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_API __declspec(dllexport) 
//#else
//#define LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_API __declspec(dllimport)
//#endif
//
//// This class is exported from the Lithnet.ActiveDirectory.PasswordFilter.Native.dll
//class LITHNETACTIVEDIRECTORYPASSWORDFILTERNATIVE_API CLithnetActiveDirectoryPasswordFilterNative {
//public:
//	CLithnetActiveDirectoryPasswordFilterNative(void);
//};
//
//
//__declspec(dllexport)  extern "C" BOOLEAN  __stdcall InitializeChangeNotify(void);
//__declspec(dllexport)  extern "C" NTSTATUS __stdcall PasswordChangeNotify(
//	PUNICODE_STRING UserName,
//	ULONG RelativeId,
//	PUNICODE_STRING NewPassword
//);
//
//__declspec(dllexport)  extern "C" BOOLEAN __stdcall PasswordFilter(
//	PUNICODE_STRING AccountName,
//	PUNICODE_STRING FullName,
//	PUNICODE_STRING Password,
//	BOOLEAN SetOperation
//);
//
//
//
