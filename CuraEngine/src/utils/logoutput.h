//Copyright (c) 2018 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <stdarg.h>

namespace cura {

/*================ add @2022-02-28 by CL ================*/
void logToTempFileEnd();
void logToTempFileInit();
typedef void (*FuncEngineLogCallbackPtr)(const char *, int);
class LogWithChars{
    constexpr static int BufferCount = 4096;
    static char buffer[BufferCount];
    static FuncEngineLogCallbackPtr funcEngineLogCallback;
public:
    static void setFuncEngineLogCallback(FuncEngineLogCallbackPtr callback);
    static FuncEngineLogCallbackPtr getFuncEngineLogCallback();
    static bool toSlicer(const char * const _Format, ...);
    static void toSlicer2(const char * const _Format, ...);
public:
    int offset;

    LogWithChars();
    void clean();

    void my_printf(char const* const _Format, va_list _ArgList);
    void my_printf(char const* const _Format, ...);

    void emitSignal();
};


double getTimeKeeperMs(char * buf);

/*
 * \brief Increase verbosity level by 1.
 */
void increaseVerboseLevel();

/*
 * \brief Enable logging the current slicing progress to the log.
 */
void enableProgressLogging();

/*
 * \brief Report an error message.
 *
 * This is always reported, regardless of verbosity level.
 */
void logError(const char* fmt, ...);

/*
 * \brief Report a warning message.
 * 
 * Always reported, regardless of verbosity level.
 */
void logWarning(const char* fmt, ...);

/*
 * \brief Report a message if the verbosity level is 1 or higher.
 */
void log(const char* fmt, ...);
void logCL(const char* fmt, ...);   // add @2022-05-15 by CL

/*
 * \brief Log a message, regardless of verbosity level.
 */
void logAlways(const char* fmt, ...);

/*
 * \brief Log a debugging message.
 *
 * The message is only logged if the verbosity level is 2 or higher.
 */
void logDebug(const char* fmt, ...);

/*
 * \brief Report the progress in the log.
 *
 * Only works if ``enableProgressLogging()`` has been called.
 */
void logProgress(const char* type, int value, int maxValue, float percent);

} //namespace cura

#endif //LOGOUTPUT_H
