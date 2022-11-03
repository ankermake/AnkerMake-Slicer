#include "HeartBeatThead.h"
#include <QCoreApplication>
#include <QLocalSocket>
#include "../common/utilities/tlogger.h"
#ifdef _WIN32
#define NETWORKEXE "AnkerSlicerNetWork.exe"

#elif __APPLE__
#define NETWORKEXE "AnkerSlicerNetWork.app"
#endif

HeartBeatThread::HeartBeatThread(QObject *parent) : QThread(parent) , m_process(nullptr)
{

}

HeartBeatThread::~HeartBeatThread()
{
    qint64 id = QDateTime::currentSecsSinceEpoch();
    m_socket->closeNetworkProcess(id);

    quit();
    wait();
}

void HeartBeatThread::run()
{
    while(!isInterruptionRequested())
    {

        if(!processIsRun(NETWORKEXE))
        {
            sleep(1);
            m_process = new QProcess(this);//m_process
            connect(m_process, &QProcess::errorOccurred, this , &HeartBeatThread::processErrorOccurred);
            QString dir = QCoreApplication::applicationDirPath() + QString("/Network/");
#ifdef _WIN32
            QString cmdStr = dir + NETWORKEXE;
#elif __APPLE__
            QString cmdStr = QString("open ") + dir + NETWORKEXE;
#endif
            //qDebug() << "cmdStr: " << cmdStr;

            m_process->start(cmdStr);
            started = false;
            heartBeatCount = 0;
            AkUtil::TDebug("start: " + cmdStr);
        }
        if((m_socket != nullptr) && (m_socket->getState() == QRemoteObjectReplica::Valid))
        {
            qint64 id = QDateTime::currentSecsSinceEpoch();
            QString sendStr = QString(SOCKET_CMD_HEARTBEAT) + QString(",") + QString::number(id);
            //HeartBeatThread::socketWriteMutex.lock();

            emit m_socket->sigSendMsg(sendStr);
            //HeartBeatThread::socketWriteMutex.unlock();
            heartBeatCount++;
            AkUtil::TDebug(sendStr + QString("  Count:") + QString(heartBeatCount));
        }
        sleep(3);
    }
}

bool HeartBeatThread::processIsRun(const QString &exeName)
{
    bool ret = false;
#ifdef _WIN32
    QProcess process;
    process.start("tasklist", QStringList() << "/FI" << "imagename eq " + exeName);
    process.waitForFinished();

    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
    ret = outputStr.contains("PID") && outputStr.contains(exeName);
#elif __APPLE__
    //QString strCommand = "ps -ef|grep " + exeName + " |grep -v grep |awk '{print $2}'";
    std::string strCmd = "ps -ef|grep " + exeName.toStdString() + " |grep -v grep |awk '{print $2}'";
    const char* strFindName = strCmd.c_str();
    FILE *pPipe = popen(strFindName, "r");
    if(pPipe != NULL)
    {
        char name[512] = { 0 };
        while(fgets(name, sizeof(name), pPipe) != NULL)
        {
            int nLen = strlen(name);
            if(nLen > 0 && name[nLen - 1] == '\n')
            {
                name[nLen - 1] = '\0';
                ret = true;
                break;
            }
        }
        pclose(pPipe);
    }
#endif
    if(ret && started && heartBeatCount >= 5) 
    {
        bool kill = false;
        
#ifdef _WIN32
        kill = QProcess::startDetached("taskkill -t  -f /IM " + exeName);
#elif __APPLE__
        kill = QProcess::startDetached("ps -ef | grep " + exeName + " | awk '{print $2}' | xargs kill -9");
#endif
        QString debugStr;
        QDebug(&debugStr) << QString("Network is zombie process. Kill: ") << kill;
        AkUtil::TDebug(debugStr);
    }
    ret &= (heartBeatCount < 5);
    return ret;
}

void HeartBeatThread::processErrorOccurred(QProcess::ProcessError error)
{
    qDebug() << "Process error: " << error;
}

void HeartBeatThread::heartBeatFeedbackSlot()
{
    heartBeatCount = 0;
    started = true;
    AkUtil::TDebug(QString("HeartBeat Feedback  Count:") + QString(heartBeatCount));
}

HeartBeatWork::HeartBeatWork(QObject *parent) :  m_socket(nullptr), m_process(nullptr)
{

}

HeartBeatWork::~HeartBeatWork()
{
    //qint64 id = QDateTime::currentSecsSinceEpoch();
    //m_socket->closeNetworkProcess(id);
}

void HeartBeatWork::init()
{
//    m_socket = QSharedPointer<QMainSocket2>(new QMainSocket2(this));
//    connect(m_socket.get(), &QMainSocket2::heartBeatFeedbackSignal, this, &HeartBeatWork::heartBeatFeedbackSlot, Qt::QueuedConnection);
//    m_heartTimer = QSharedPointer<QTimer> (new QTimer(this));
//    connect(m_heartTimer.get(), &QTimer::timeout, this, &HeartBeatWork::doWork);
//    connect(m_socket->thread(), &QThread::finished, this, &HeartBeatWork::stopTimer);

    //doWork();
}

void HeartBeatWork::processErrorOccurred(QProcess::ProcessError error)
{
    qDebug() << "Process error: " << error;
}


bool HeartBeatWork::processIsRun(const QString &exeName)
{
    bool ret = false;
#ifdef _WIN32
    QProcess process;
    process.start("tasklist", QStringList() << "/FI" << "imagename eq " + exeName);
    process.waitForFinished();

    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
    ret = outputStr.contains("PID") && outputStr.contains(exeName);
#elif __APPLE__
    //QString strCommand = "ps -ef|grep " + exeName + " |grep -v grep |awk '{print $2}'";
    std::string strCmd = "ps -ef|grep " + exeName.toStdString() + " |grep -v grep |awk '{print $2}'";
    const char* strFindName = strCmd.c_str();
    FILE *pPipe = popen(strFindName, "r");
    if(pPipe != NULL)
    {
        char name[512] = { 0 };
        while(fgets(name, sizeof(name), pPipe) != NULL)
        {
            int nLen = strlen(name);
            if(nLen > 0 && name[nLen - 1] == '\n')
            {
                name[nLen - 1] = '\0';
                ret = true;
                break;
            }
        }
        pclose(pPipe);
    }
#endif
    if(ret && started && heartBeatCount >= 5) 
    {
        bool kill = false;
        
#ifdef _WIN32
        kill = QProcess::startDetached("taskkill -t  -f /IM " + exeName);
#elif __APPLE__
        kill = QProcess::startDetached("ps -ef | grep " + exeName + " | awk '{print $2}' | xargs kill -9");
#endif
        QString debugStr;
        QDebug(&debugStr) << QString("Network is zombie process. Kill: ") << kill;
        AkUtil::TDebug(debugStr);
    }
    ret &= (heartBeatCount < 5);
    return ret;
}

void HeartBeatWork::doWork()
{
    m_socket = QSharedPointer<QMainSocket2>(new QMainSocket2(this));
    connect(m_socket.get(), &QMainSocket2::heartBeatFeedbackSignal, this, &HeartBeatWork::heartBeatFeedbackSlot, Qt::QueuedConnection);
    while(1)
    {
        if(!processIsRun(NETWORKEXE))
        {
            QThread::sleep(1);
            m_process = new QProcess(this);//m_process
            connect(m_process, &QProcess::errorOccurred, this , &HeartBeatWork::processErrorOccurred);
            QString dir = QCoreApplication::applicationDirPath() + QString("/Network/");
#ifdef _WIN32
            QString cmdStr = dir + NETWORKEXE;
#elif __APPLE__
            QString cmdStr = QString("open ") + dir + NETWORKEXE;
#endif
            //qDebug() << "cmdStr: " << cmdStr;

            m_process->start(cmdStr);
            started = false;
            heartBeatCount = 0;
            AkUtil::TDebug("start: " + cmdStr);
        }
        if((m_socket != nullptr) && (m_socket->getState() == QRemoteObjectReplica::Valid))
        {
            qint64 id = QDateTime::currentSecsSinceEpoch();
            QString sendStr = QString(SOCKET_CMD_HEARTBEAT) + QString(",") + QString::number(id);
            //HeartBeatThread::socketWriteMutex.lock();

            emit m_socket->sigSendMsg(sendStr);
            //HeartBeatThread::socketWriteMutex.unlock();
            heartBeatCount++;
            AkUtil::TDebug(sendStr + QString("  Count:") + QString(heartBeatCount));
        }
        QThread::sleep(3);
    }
}

void HeartBeatWork::stopTimer()
{
//    if(m_heartTimer->isActive())
//    {
//        m_heartTimer->stop();
//    }
}

void HeartBeatWork::heartBeatFeedbackSlot()
{
    heartBeatCount = 0;
    started = true;
    AkUtil::TDebug(QString("HeartBeat Feedback  Count:") + QString(heartBeatCount));
}
