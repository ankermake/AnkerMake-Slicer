#include "QMainSocket.h"
#include <QTextCodec>
#include <QDateTime>
#include <QCoreApplication>
#include <QTime>
QMainSocket::QMainSocket(QObject *parent)/* : m_heartBeatThread(nullptr)*/
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &QMainSocket::readDataSlot);
    connect(m_socket, &QAbstractSocket::errorOccurred, this, &QMainSocket::socketErrorSlot);
//m_socket->stateChanged()
    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChangedSlot(QAbstractSocket::SocketState)));
    connectServer();
}

QMainSocket::~QMainSocket()
{
//    if(m_heartBeatThread != nullptr && m_heartBeatThread->isRunning())
//    {
//        m_heartBeatThread->quit();
//        m_heartBeatThread->wait();
//    }
}

//void QMainSocket::createHeartBeatThread()
//{
//    m_heartBeatThread = new HeartBeatThread(this);
//    //m_heartBeatThread->init(m_socket);
//    m_heartBeatThread->start();
//}

void QMainSocket::connectServer(QString ip, int port)
{
    if(m_socket == nullptr)
    {
        return;
    }
    m_socket->abort(); 
    m_socket->connectToHost(ip, port);
}

void QMainSocket::sendData(const QByteArray &bytes)
{
    if(m_socket == nullptr)
    {
        return;
    }
   // HeartBeatThread::socketWriteMutex.lock();
    int ret = m_socket->write(bytes, bytes.size());
   // HeartBeatThread::socketWriteMutex.unlock();
    qDebug() << "Send Size: " << ret;
}

void QMainSocket::sendData(const QString &str)
{
    if(m_socket == nullptr)
    {
        return;
    }
//#ifdef _WIN32
//    QTextCodec *code = QTextCodec::codecForName("GB2312");
//#elif __APPLE__
//    QTextCodec *code = QTextCodec::codecForLocale("UTF-8");
//#endif
    qDebug() << "Send str: " << str;
    QByteArray bytes = str.toLocal8Bit();
    //HeartBeatThread::socketWriteMutex.lock();
    m_socket->write(bytes, bytes.size());
    //HeartBeatThread::socketWriteMutex.unlock();
}

qint64 QMainSocket::loginWidgetDisplay(qint64 id)
{
    qint64 ret = checkConnectState();
    if(ret < 0)
    {
        return ret;
    }
    //qint64 id = QDateTime::currentSecsSinceEpoch();
    QString sendStr = QString(SOCKET_CMD_LOGIN_WIDGET_DISPLAY) + QString(",") + QString::number(id);
    sendData(sendStr);
    return id;
}

qint64 QMainSocket::selectPrintDisplay(qint64 id)
{
    qint64 ret = checkConnectState();
    if(ret < 0)
    {
        return ret;
    }
    //qint64 id = QDateTime::currentSecsSinceEpoch();
    QString sendStr = QString(SOCKET_CMD_SELECT_PRINT_DISPLAY) + QString(",") + QString::number(id);
    sendData(sendStr);
    return id;
}

qint64 QMainSocket::checkConnectState()
{
    if(m_socket == nullptr)
    {
        return -1;
    }
    int count = 0;
    while(m_socket->state() != QAbstractSocket::ConnectedState && count++ < 3)
    {
        connectServer();
        msleep(100);
    }
    if(m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "Connect Server Failed!";
        return -1;
    }
    return 0;
}

qint64 QMainSocket::sendFile(const QString &filePath, qint64 id)
{
    qint64 ret = checkConnectState();
    if(ret < 0)
    {
        return ret;
    }
    //qint64 id = QDateTime::currentSecsSinceEpoch();
    QString sendStr = QString(SOCKET_CMD_SEND_FILE) + QString(",") + QString::number(id) + QString(",") + filePath;
    sendData(sendStr);
    return id;
}

void QMainSocket::msleep(unsigned int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void QMainSocket::closeSocket()
{
    if(m_socket != nullptr)
    {
        m_socket->close();
    }
}

void QMainSocket::readDataSlot()
{
    if(m_socket != nullptr)
    {
        QString localStr = QString::fromLocal8Bit(m_socket->readAll());
        //emit recvDataSignal(localStr);

        QStringList list = localStr.split(",");
        if(list.length() < 3)
        {
            return ;
        }
        if(list[0] == SOCKET_CMD_HEARTBEAT)
        {
            qDebug() << "Recv HeartBear: " << localStr;
        }
        else if(list[0] == SOCKET_CMD_SEND_FILE)
        {
            emit sendFileFeedbackSignal(list[1].toInt(), list[3].toInt());
        }
        else if(list[0] == SOCKET_CMD_SELECT_PRINT_DISPLAY)
        {
            emit selectPrintDisplayFeedbackSignal(list[1].toInt(), list[2].toInt());
        }
        else if(list[0] == SOCKET_CMD_LOGIN_WIDGET_DISPLAY)
        {
            emit loginWidgetDisplayFeedbackSignal(list[1].toInt(), list[2].toInt());
        }
    }
}

void QMainSocket::socketErrorSlot(QAbstractSocket::SocketError err)
{
    emit socketErrorSignal(err);
    if(m_socket != nullptr)
    {
        qDebug() << "Error: " << m_socket->errorString();
    }
}

void QMainSocket::stateChangedSlot(QAbstractSocket::SocketState st)
{
    emit stateChangedSignal(st);
    if(m_socket != nullptr)
    {
        qDebug() << "State Changed: " << st;
    }
}
