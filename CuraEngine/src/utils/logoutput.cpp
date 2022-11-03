/** Copyright (C) 2013 Ultimaker - Released under terms of the AGPLv3 License */
#include <stdio.h>
#include <stdarg.h>
#include "gettime.h"
#ifdef _OPENMP
    #include <omp.h>
#endif // _OPENMP
#include "logoutput.h"

namespace cura {

/*================ add @2022-02-28 by CL ================*/
static FILE* tempLogFile = NULL;
TimeKeeper logTimeKeeper;
void logToTempFileEnd()
{
    if (tempLogFile) {
        fclose(tempLogFile);
    }
    tempLogFile = NULL;
}
void logToTempFileInit()
{
    logTimeKeeper.restart();
    logToTempFileEnd();
    tempLogFile = freopen("__Engine_.log", "w", stderr);
    if (tempLogFile == NULL) {
        fprintf(stdout, "errer on freopen");
    }
    else {
        fprintf(stderr, "Log redirection @2021-10-03 by CL\n\n");
    }
    fflush(stderr);
}

FuncEngineLogCallbackPtr LogWithChars::funcEngineLogCallback = nullptr;
char LogWithChars::buffer[BufferCount];
void LogWithChars::setFuncEngineLogCallback(FuncEngineLogCallbackPtr callback)
{
    funcEngineLogCallback = callback;
}
FuncEngineLogCallbackPtr LogWithChars::getFuncEngineLogCallback()
{
    return funcEngineLogCallback;
}


bool LogWithChars::toSlicer(const char * const _Format, ...){
    if(funcEngineLogCallback){
        LogWithChars log;
        va_list _ArgList;
        va_start(_ArgList, _Format);
        log.my_printf(_Format, _ArgList);
        va_end(_ArgList);
        log.emitSignal();
    }
    return funcEngineLogCallback;
}

LogWithChars::LogWithChars(){clean();}

void LogWithChars::clean(){
    //memset(buffer, 0x00, BufferCount);
    buffer[0] = 0x00;
    offset = 0;
}

void LogWithChars::my_printf(const char * const _Format, va_list _ArgList){
    int wn = vsnprintf(&buffer[offset], BufferCount-offset, _Format, _ArgList);
    offset += wn;
}

void LogWithChars::my_printf(const char * const _Format, ...){
    va_list _ArgList;
    va_start(_ArgList, _Format);
    int wn = snprintf(&buffer[offset], BufferCount-offset, _Format, _ArgList);
    va_end(_ArgList);
    offset += wn;
}

void LogWithChars::emitSignal(){
    if(funcEngineLogCallback){
        funcEngineLogCallback(buffer, offset);
    }
    clean();
}

double getTimeKeeperMs(char *buf)
{
    return logTimeKeeper.getMs(buf);
}

/*================ log func ================*/
static int verbose_level;
static bool progressLogging;

void increaseVerboseLevel()
{
    verbose_level++;
}

void enableProgressLogging()
{
    progressLogging = true;
}

void logError(const char* fmt, ...)
{
    va_list args;
    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        fprintf(stderr, "[ERROR] ");
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}

void logWarning(const char* fmt, ...)
{
    va_list args;
    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        fprintf(stderr, "[WARNING] ");
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}

void logAlways(const char* fmt, ...)
{
    va_list args;
    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}

void log(const char* fmt, ...)
{
    va_list args;
    if (verbose_level < 1)
        return;

    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}
void logCL(const char *fmt, ...)
{
    va_list args;

    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}

void logDebug(const char* fmt, ...)
{
    va_list args;
    if (verbose_level < 2)
    {
        return;
    }
    #pragma omp critical
    {
        if(!LogWithChars::toSlicer(fmt)){
        va_start(args, fmt);
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        fprintf(stderr, "[DEBUG] ");
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        va_end(args);
        }
    }
}

void logProgress(const char* type, int value, int maxValue, float percent)
{
    //if (!progressLogging)
    //    return;

    #pragma omp critical
    {
        if(!LogWithChars::toSlicer("Progress:%s:%i:%i \t%f%%\n", type, value, maxValue, percent)){
        static char  buf[128]{ 0 };
        logTimeKeeper.getMs(buf);
        fprintf(stderr, buf);
        fprintf(stderr, "Progress:%s:%i:%i \t%f%%\n", type, value, maxValue, percent);
        fflush(stderr);
        }
    }
}

}//namespace cura
