#include "tlogger.h"
#include <QDebug>
#include <QDir>
#include "ioapi.h"
#include <QDateTime>
#include <QThread>
#include <QMutexLocker>
#include <QTextCodec>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QVariant>

AK_UTIL_NAME_SPACE_BEGIN

//TLogger::Level TLogger::level = TLogger::Level::Trace;
//int TLogger::maxSize = 20*1024*1024;
//QMutex TLogger::fileLock;
//QString TLogger::logPath = "C:/workspace/Log";
//QString TLogger::logName = "analyze.log";
//bool TLogger::setUtf8 = false;
//bool TLogger::enable = true;

QString _ThreadId_(){
    if (nullptr == qApp)
    {
        return "-9999";
    }
    const qintptr currId = qintptr(QThread::currentThread()->currentThreadId());
    const qintptr mainId = qintptr(qApp->thread()->currentThreadId());
    QString str = QThread::currentThread() == qApp->thread() ? QString("[main:%1]").arg(mainId) : QString("[sub:%1]").arg(currId);
    return str;
}

const QString &TLogger::getLogName()
{
    return logName;
}

void TLogger::setLogName(const QString &newLogName)
{
    logName = newLogName;
}

int TLogger::getMaxSize()
{
    return maxSize;
}

void TLogger::setMaxSize(int newMaxSize)
{
    maxSize = newMaxSize;
}

const QString &TLogger::getLogPath()
{
    return logPath;
}

void TLogger::setLogPath(const QString &newLogPath)
{
    logPath = newLogPath;
}
QString TLogger::getLevelStr(Level lev)
{
    switch (lev)
    {
        case  Level::Trace: return "Trace";
        case  Level::Debug: return "Debug";
        case  Level::Info: return "Info";
        case  Level::Warning: return "Warning";
        case  Level::Error: return "Error";
        case  Level::Fatal: return "Fatal";
    }
}
TLogger::Level TLogger::getLevel()
{
    return level;
}

void TLogger::setLevel(Level newLevel)
{
    level = newLevel;
}



TLogger::TLogger()
{
    //logPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("log");
    auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    logPath = QDir(writableLocation).absoluteFilePath("log");
    qDebug() << "logpath is " << logPath;

    QDir logDir = QDir(logPath);// QFileInfo(logPath).absoluteDir();
    if (!logDir.exists())
    {
        logDir.mkpath(logDir.absolutePath());
    }
    logName = "akSlicer.log";

    level = TLogger::Level::Trace;
    maxSize = 20 * 1024 * 1024;
    enable = true;

    qDebug() << "\n\n"
             << "TLogger" << qintptr(this)
             << "\n\n";
}

TLogger *TLogger::instance()
{
    static const char TLoggerKey[] = "AnkerMake-Logger";
    static TLogger logger;
    TLogger *ptrLogger = nullptr;
    if (nullptr == qApp)
    {
        return &logger;
    }

    auto var = qApp->property(TLoggerKey);
    if(TLogger *log = var.value<TLogger *>()){
        ptrLogger = log;
    }
    else{
        qApp->setProperty(TLoggerKey, QVariant::fromValue(&logger));
        ptrLogger = &logger;
    }


    return ptrLogger;
}

void TLogger::trace(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Trace)
    {
        return;
    }
    writeLog(TLogger::Level::Trace,msg,funcInfo);
}

void TLogger::debug(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Debug)
    {
        return;
    }
    writeLog(TLogger::Level::Debug,msg,funcInfo);

}

void TLogger::info(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Info)
    {
        return;
    }
    writeLog(TLogger::Level::Info,msg,funcInfo);
}


void TLogger::warning(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Warning)
    {
        return;
    }
    writeLog(TLogger::Level::Warning,msg,funcInfo);
}

void TLogger::error(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Error)
    {
        return;
    }
    writeLog(TLogger::Level::Error,msg,funcInfo);
}

void TLogger::fatal(QString msg,QString funcInfo)
{
    if (level > TLogger::Level::Fatal)
    {
        return;
    }
    writeLog(TLogger::Level::Fatal,msg,funcInfo);

}

void TLogger::message(QString msg, QString) //  add @2022-05-15 by CL
{
    writeMessage(msg);
}

void TLogger::setEnable(bool enableLog)
{
    enable = enableLog;
}

QString TLogger::logFileReady()
{
    
    //if (!setUtf8)
    //{
    //    IoApi::setEncodeUtf8();
    //    setUtf8 = true;
    //}

    //QTextCodec* codec = QTextCodec::codecForLocale();
    //msg = codec->fromUnicode(msg);
    //
    
    //msg = code->fromUnicode(msg).data();

    //msg = QString(msg.toLocal8Bit().constData());

    if (logPath.isEmpty() || logPath.isNull())
    {
        return QString();
    }
    QDir logDir(logPath);
    if (!logDir.exists())
    {
        logDir.mkpath(logPath);
    }

    QMutexLocker locker(&fileLock);
    
    QString logFile = logDir.absoluteFilePath(logName);
    QFileInfo fInfo(logFile);
    if (!fInfo.exists() )
    {
        IoApi::touch(logFile);
    }
    
    if(fInfo.size()>maxSize)
    {
        
        QString bakFile = fInfo.dir().absoluteFilePath(fInfo.fileName());
        bakFile.append(QDateTime::currentDateTime().toString("yyyyMMddHHmmss")).append(".log");
        QFile::rename(fInfo.filePath(),bakFile);
        
        IoApi::touch(logFile);
    }
    return logFile;
}

void TLogger::writeMessage(QString msg)
{
    if (!enable)
    {
        return;
    }

    QString logFile = logFileReady();
    if(logFile.isEmpty()){
        return;
    }

    
    IoApi::append(logFile, msg);
}


void TLogger::writeLog(Level lev, QString msg,QString funcInfo)
{
    if (!enable)
    {
        return;
    }

    QString logFile = logFileReady();
    if(logFile.isEmpty()){
        return;
    }
    
    //QString logMsg = QString("%1 %2 %3 %4 %5 \r\n").arg(__TimeStamp__).arg(getLevelStr(lev)).arg(__ThreadId2__).arg(funcInfo).arg(msg);

    QStringList logStr;
    logStr << __TimeStamp__;
    logStr << "[" + getLevelStr(lev) + "]";
    logStr << __ThreadId2__;
    logStr << funcInfo;
    logStr << msg;
    QString logMsg = logStr.join(" ") + "\r\n";

    
    IoApi::append(logFile, logMsg);
}

TLogHelper::TLogHelper(QString msg, QString funcInfo)
{
    this->message = msg;
    this->funcInfo = funcInfo;
    TLogger::instance()->info(QString(" >>>> Enter %1 >>>> ").arg(msg),funcInfo);
}

TLogHelper::~TLogHelper()
{
    TLogger::instance()->info(QString(" <<<< Exit  %1 <<<<").arg(message), funcInfo);
}



AK_UTIL_NAME_SPACE_END

