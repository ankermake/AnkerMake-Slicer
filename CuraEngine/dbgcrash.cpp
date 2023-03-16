#include "dbgcrash.h"
#include <sstream>
#include "utils/logoutput.h"

#if defined(_WIN32)

//
void SafeStrCpy(char* szDest, size_t nMaxDestSize, const char* szSrc)
{
    if (nMaxDestSize <= 0) return;
    if (strlen(szSrc) < nMaxDestSize)
    {
        strcpy_s(szDest, nMaxDestSize, szSrc);
    }
    else
    {
        strncpy_s(szDest, nMaxDestSize, szSrc, nMaxDestSize);
        szDest[nMaxDestSize-1] = '\0';
    }
}


//
CrashInfo GetCrashInfo(const EXCEPTION_RECORD *pRecord)
{
    CrashInfo crashinfo;
    SafeStrCpy(crashinfo.Address, MAX_ADDRESS_LENGTH, "N/A");
    SafeStrCpy(crashinfo.ErrorCode, MAX_ADDRESS_LENGTH, "N/A");
    SafeStrCpy(crashinfo.Flags, MAX_ADDRESS_LENGTH, "N/A");

    sprintf_s(crashinfo.Address, "%08X", pRecord->ExceptionAddress);
    sprintf_s(crashinfo.ErrorCode, "%08X", pRecord->ExceptionCode);
    sprintf_s(crashinfo.Flags, "%08X", pRecord->ExceptionFlags);

    return crashinfo;
}


//
vector<CallStackInfo> GetCallStack(const CONTEXT *pContext)
{
    HANDLE hProcess = GetCurrentProcess();

    SymInitialize(hProcess, NULL, TRUE);

    vector<CallStackInfo> arrCallStackInfo;

    CONTEXT c = *pContext;

    STACKFRAME64 sf;
    memset(&sf, 0, sizeof(STACKFRAME64));
    DWORD dwImageType = IMAGE_FILE_MACHINE_I386;

    
    //
#ifdef _M_IX86
    sf.AddrPC.Offset = c.Eip;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.Esp;
    sf.AddrStack.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.Ebp;
    sf.AddrFrame.Mode = AddrModeFlat;
#elif _M_X64
    dwImageType = IMAGE_FILE_MACHINE_AMD64;
    sf.AddrPC.Offset = c.Rip;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.Rsp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.Rsp;
    sf.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    dwImageType = IMAGE_FILE_MACHINE_IA64;
    sf.AddrPC.Offset = c.StIIP;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.IntSp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrBStore.Offset = c.RsBSP;
    sf.AddrBStore.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.IntSp;
    sf.AddrStack.Mode = AddrModeFlat;
#else
    #error "Platform not supported!"
#endif

    HANDLE hThread = GetCurrentThread();

    while (true)
    {
        
        
        //
        if (!::StackWalk64(dwImageType, hProcess, hThread, &sf, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            break;
        }

        if (sf.AddrFrame.Offset == 0)
        {
            break;
        }

        CallStackInfo callstackinfo;
        SafeStrCpy(callstackinfo.MethodName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.FileName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.ModuleName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.LineNumber, MAX_NAME_LENGTH, "N/A");

        BYTE symbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + MAX_NAME_LENGTH];
        IMAGEHLP_SYMBOL64 *pSymbol = (IMAGEHLP_SYMBOL64*)symbolBuffer;
        memset(pSymbol, 0, sizeof(IMAGEHLP_SYMBOL64) + MAX_NAME_LENGTH);

        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLength = MAX_NAME_LENGTH;

        DWORD symDisplacement = 0;

        
        //
        if (SymGetSymFromAddr64(hProcess, sf.AddrPC.Offset, NULL, pSymbol))
        {
            SafeStrCpy(callstackinfo.MethodName, MAX_NAME_LENGTH, pSymbol->Name);
        }

        IMAGEHLP_LINE64 lineInfo;
        memset(&lineInfo, 0, sizeof(IMAGEHLP_LINE64));

        lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        DWORD dwLineDisplacement;

        
        //
        if (SymGetLineFromAddr64(hProcess, sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo))
        {
            SafeStrCpy(callstackinfo.FileName, MAX_NAME_LENGTH, lineInfo.FileName);
            sprintf_s(callstackinfo.LineNumber, "%d", lineInfo.LineNumber);
        }

        IMAGEHLP_MODULE64 moduleInfo;
        memset(&moduleInfo, 0, sizeof(IMAGEHLP_MODULE64));

        moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

        
        //
        if (SymGetModuleInfo64(hProcess, sf.AddrPC.Offset, &moduleInfo))
        {
            SafeStrCpy(callstackinfo.ModuleName, MAX_NAME_LENGTH, moduleInfo.ModuleName);
        }

        arrCallStackInfo.push_back(callstackinfo);
    }

    SymCleanup(hProcess);

    return arrCallStackInfo;
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{ 
    
//#ifdef _M_IX86
//    if (pException->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
//    {
//        static char TempStack[1024 * 128];
//        __asm mov eax,offset TempStack[1024 * 128];
//        __asm mov esp,eax;
//    }
//#endif

    CrashInfo crashinfo = GetCrashInfo(pException->ExceptionRecord);
    //TFatal("Application is crashed.  errCode:" + QString(crashinfo.ErrorCode)  + " errAddr:" + crashinfo.Address + " errFlag:" + crashinfo.Flags);
    vector<CallStackInfo> arrCallStackInfo = GetCallStack(pException->ContextRecord);

    
    //
    //cout << "CallStack: " << endl;
    //QStringList sl;
    //for (vector<CallStackInfo>::iterator i = arrCallStackInfo.begin(); i != arrCallStackInfo.end(); ++i)
    //{
    //    CallStackInfo callstackinfo = (*i);
    //    //cout << callstackinfo.MethodName << "() : [" << callstackinfo.ModuleName << "] (File: " << callstackinfo.FileName << " @Line " << callstackinfo.LineNumber << ")" << endl;
    //    sl << QString(callstackinfo.MethodName) + "() : [" + callstackinfo.ModuleName + "] (File: " + callstackinfo.FileName + " @Line " + callstackinfo.LineNumber + ")";
    //    //TFatal(sl.join());
    //}

    stringstream ss;
    for (vector<CallStackInfo>::iterator i = arrCallStackInfo.begin(); i != arrCallStackInfo.end(); ++i)
    {
        CallStackInfo callstackinfo = (*i);
        ss << string(callstackinfo.MethodName) + "() : [" + callstackinfo.ModuleName + "] (File: " + callstackinfo.FileName + " @Line " + callstackinfo.LineNumber + ")";
        ss << std::endl;
    }

    
    cura::logWarning("Application is crashed:%s\n", ss.str().c_str());

    return EXCEPTION_EXECUTE_HANDLER;
}

void printStack( void )
{
    unsigned int   i;
    void         * stack[ 100 ];
    unsigned short frames;
    SYMBOL_INFO  * symbol;
    HANDLE         process;

    process = GetCurrentProcess();

    SymInitialize( process, NULL, TRUE );

    frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
    symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
    symbol->MaxNameLen   = 255;
    symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

    for( i = 0; i < frames; i++ )
    {
        SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );

        printf( "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address );
    }

    free( symbol );
}
//
//LONG crashStackCallback(struct _EXCEPTION_POINTERS* exceptionInfo) {
//
////    TFunction("");
////    EXCEPTION_RECORD *record = exceptionInfo->ExceptionRecord;
////    QString errCode(QString::number(record->ExceptionCode, 16));
////    QString errAddr(QString::number((qint64)record->ExceptionAddress, 16));
////    QString errFlag(QString::number(record->ExceptionFlags, 16));
////    QString errPara(QString::number(record->NumberParameters, 16));
////    TFatal("Application is crashed.  errCode:" + errCode + " errAddr:" + errAddr + " errFlag:" + errFlag + " errPara:"+errPara);
//
////    return EXCEPTION_EXECUTE_HANDLER;
//
//    auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
//    QString savePath = QDir(writableLocation).absoluteFilePath("log");
//    QDir dir(savePath);
//    if(!dir.exists() && !dir.mkpath(savePath)) {
//        return EXCEPTION_EXECUTE_HANDLER;
//    }
//
//    QString dumpFile = "dump_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + ".dmp";
//    auto dumpFullName =  dir.absoluteFilePath(dumpFile);
//    HANDLE dump = CreateFileW(dumpFullName.toStdWString().c_str(), GENERIC_WRITE,
//                              0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//    if(INVALID_HANDLE_VALUE == dump) {
//        return EXCEPTION_EXECUTE_HANDLER;
//    }
//
//    EXCEPTION_RECORD *record = exceptionInfo->ExceptionRecord;
//    QString errCode(QString::number(record->ExceptionCode, 16));
//    QString errAddr(QString::number((qint64)record->ExceptionAddress, 16));
//    QString errFlag(QString::number(record->ExceptionFlags, 16));
//    QString errPara(QString::number(record->NumberParameters, 16));
//    TFatal("Application is crashed. please see dump file "  + dumpFullName  + " errCode:" + errCode + " errAddr:" + errAddr + " errFlag:" + errFlag + " errPara:"+errPara);
//
//
//    MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo;
//    miniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
//    miniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
//    miniDumpExceptionInfo.ClientPointers = TRUE;
//    DWORD idProcess = GetCurrentProcessId();
//    MiniDumpWriteDump(GetCurrentProcess(), idProcess, dump,
//                      MiniDumpWithFullMemory, &miniDumpExceptionInfo, NULL, NULL);
//

//    CloseHandle(dump);
//
//    return EXCEPTION_EXECUTE_HANDLER;
//}

#endif // end #if defined(_WIN32)
