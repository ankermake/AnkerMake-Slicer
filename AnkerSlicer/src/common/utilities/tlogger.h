#ifndef TLOGGER_H
#define TLOGGER_H
#include <qstring.h>
#include "akUtilSpace.h"
#include <QMutex>
#include <QObject>

AK_UTIL_NAME_SPACE_BEGIN


QString _ThreadId_();

#define __TimeStamp__ (QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss.zzz]"))
#define __ThreadId__  (QString("[%1]").arg(qintptr(QThread::currentThread()->currentThreadId())))
#define __ThreadId2__ AkUtil::_ThreadId_()  // mainId == currId ? "[main:%1]" : "[sub:%1]"
#define __CodeLocation__ (QString ("[%1:%2:%3]").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))

//QString logMsg = QString("[%1] [%2] [%3:%4:%5] %6 \r\n").arg(timeStamp).arg(threadId).arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(msg);
#define TTrace(msg) TLogger::instance()->trace(msg,__CodeLocation__)
#define TDebug(msg) TLogger::instance()->debug(msg,__CodeLocation__)
#define TInfo(msg) TLogger::instance()->info(msg,__CodeLocation__)
#define TWarning(msg) TLogger::instance()->warning(msg,__CodeLocation__)
#define TError(msg) TLogger::instance()->error(msg,__CodeLocation__)
#define TFatal(msg) TLogger::instance()->fatal(msg,__CodeLocation__)
#define TMessage(msg) TLogger::instance()->message(msg,"")

#define TFunction(msg) TLogHelper toollibLogHelper(QString("Function %1, %2").arg(__FUNCTION__).arg(msg),__CodeLocation__)
#define TConditon(msg) TLogHelper toollibLogHelper(QString("Condition %1").arg(msg),__CodeLocation__)

//class TOOLLIB_EXPORT TLogger
class TLogger
{
public:
    TLogger();
    static TLogger* __instance;
    static TLogger* instance();
    void trace(QString msg, QString funcInfo);
    void debug(QString msg, QString funcInfo);
    void info(QString msg, QString funcInfo);
    void warning(QString msg, QString funcInfo);
    void error(QString msg, QString funcInfo);
    void fatal(QString msg, QString funcInfo);
    void message(QString msg, QString funcInfo="");  //  add @2022-05-15 by CL


public:
    enum Level {
        Trace = 0,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    void setEnable(bool enableLog);

    QString getLevelStr(Level lev);
    Level getLevel();
    void setLevel(Level newLevel);

    const QString &getLogPath();
    void setLogPath(const QString &newLogPath);

    int getMaxSize();
    void setMaxSize(int newMaxSize);

    const QString &getLogName();
    void setLogName(const QString &newLogName);

private:
   int maxSize;
   QMutex fileLock;
   // add @2022-05-15 by CL logFileReady,writeMessage
   QString logFileReady();
   void writeMessage(QString msg);
   void writeLog(Level lev, QString msg, QString funcInfo);
   Level level;
   QString logPath;
   QString logName;
   bool setUtf8;
   bool enable;

};

//class TOOLLIB_EXPORT TLogHelper
class TLogHelper
{
public:
    TLogHelper(QString msg, QString funcInfo);
    ~TLogHelper();
private:
    QString message;
    QString funcInfo;

};

AK_UTIL_NAME_SPACE_END

Q_DECLARE_METATYPE(AkUtil::TLogger *);

#endif // TLOGGER_H
