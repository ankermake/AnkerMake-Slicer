#ifndef DBGCRASH_H
#define DBGCRASH_H

//class DbgCrash
//{
//public:
//    DbgCrash();
//};

#if defined(_WIN32)
#include <Windows.h>
#include <DbgHelp.h>
#include <iostream>
#include <vector>

//
using namespace std;
const int MAX_ADDRESS_LENGTH = 32;
const int MAX_NAME_LENGTH = 1024;

struct CrashInfo
{
    CHAR ErrorCode[MAX_ADDRESS_LENGTH];
    CHAR Address[MAX_ADDRESS_LENGTH];
    CHAR Flags[MAX_ADDRESS_LENGTH];
};


//
struct CallStackInfo
{
    CHAR ModuleName[MAX_NAME_LENGTH];
    CHAR MethodName[MAX_NAME_LENGTH];
    CHAR FileName[MAX_NAME_LENGTH];
    CHAR LineNumber[MAX_NAME_LENGTH];
};

void SafeStrCpy(char* szDest, size_t nMaxDestSize, const char* szSrc);
CrashInfo GetCrashInfo(const EXCEPTION_RECORD *pRecord);
vector<CallStackInfo> GetCallStack(const CONTEXT *pContext);
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
void printStack( void );
//LONG crashStackCallback(struct _EXCEPTION_POINTERS* exceptionInfo);

#endif // end #if defined(_WIN32)

#endif // DBGCRASH_H
