#include "QMainSocket2.h"
#include "../common/utilities/tlogger.h"
#include <QTime>
#include <QCoreApplication>
#include <QLocalSocket>

QMainSocket2::QMainSocket2(QObject *parent) :
    QObject(parent),
    m_pRemoteNode(new QRemoteObjectNode(this))
  , m_pInterface(nullptr)
  , firstHeartBeat(true)
{
    qRegisterMetaType<QLocalSocket::LocalSocketState>("QLocalSocket");
    m_pRemoteNode->connectToNode((QUrl("local:interfaces")));

    m_pInterface = m_pRemoteNode->acquireDynamic("Interfaces1"); 
    
    connect(m_pInterface, &QRemoteObjectDynamicReplica::initialized, this, &QMainSocket2::onInitConnect);
   // connect(m_pRemoteNode, &QRemoteObjectNode::heartbeatIntervalChanged, this, &QMainSocket2::heartbeatIntervalChanged);
}

QMainSocket2::~QMainSocket2()
{

}

void QMainSocket2::loginInit(qint64 id)
{
    QString sendStr = QString(SOCKET_CMD_LOGIN_INIT) + QString(",") + QString::number(id);
    sendData(sendStr);
}

void QMainSocket2::sendData(const QString &msg)
{
    qDebug() << "Send: " << msg;
    emit sigSendMsg(msg);
}

qint64 QMainSocket2::loginWidgetDisplay(qint64 id, WId wId)
{
    QString sendStr = QString(SOCKET_CMD_LOGIN_WIDGET_DISPLAY) + QString(",") + QString::number(id) + QString(",") + QString::number(wId);
    sendData(sendStr);
    return id;
}

qint64 QMainSocket2::selectPrintDisplay(qint64 id)
{
    QString sendStr = QString(SOCKET_CMD_SELECT_PRINT_DISPLAY) + QString(",") + QString::number(id);
    sendData(sendStr);
    return id;
}

void QMainSocket2::selectFromUpdate(qint64 id, const QString &version)
{
    QString sendStr = QString(SOCKET_CMD_SELECT_FROM_UPDATE) + QString(",") + QString::number(id) + QString(",") + version;
    sendData(sendStr);
}

void QMainSocket2::autoUpdateCmd(qint64 id, const QString &version, int start)
{
    QString sendStr = QString(SOCKET_CMD_AUTO_SELECT_FROM_UPDATE) + QString(",")
            + QString::number(id) + QString(",") + version + QString(",") + QString::number(start);
    sendData(sendStr);
}

qint64 QMainSocket2::sendFile(const QString &filePath, qint64 id)
{
    QString sendStr = QString(SOCKET_CMD_SEND_FILE) + QString(",") + QString::number(id) + QString(",") + filePath;
    sendData(sendStr);
    return id;
}

void QMainSocket2::displayDeviceListWidget(qint64 id, const QString &fileName)
{
    QString sendStr = QString(SOCKET_CMD_DEVICELIST_WIDGET_DISPLAY) + QString(",") + QString::number(id) + QString(",") + fileName;
    sendData(sendStr);
}

qint64 QMainSocket2::closeNetworkProcess(qint64 id)
{
    QString sendStr = QString(SOCKET_CMD_CLOSE_NETWORK_PROCESS) + QString(",") + QString::number(id);
    sendData(sendStr);
    return id;
}

void QMainSocket2::msleep(unsigned int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

QRemoteObjectReplica::State QMainSocket2::getState() const
{
    return m_pInterface->state();
}

void QMainSocket2::printCtrlWidgetDisplay(qint64 id)
{
    QString sendStr = QString(SOCKET_CMD_PRINT_CTRL_WIDGET_DISPLAY) +
            QString(",") + QString::number(id);
    sendData(sendStr);
}

void QMainSocket2::onReceiveMsg(QString msg)
{
    QString localStr = msg;
    //emit recvDataSignal(localStr);
    QStringList list = localStr.split(",");
    if(list.length() < 2)
    {
        return ;
    }
//    static int id = 0;
//    if(id == list[1].toInt())
//    {
//        return;
//    }
//    qDebug() << "recv: " << msg;
//    id = list[1].toInt();
    if(list[0] == SOCKET_CMD_SEND_FILE && list.size() >= 4)
    {
        emit sendFileFeedbackSignal(list[1].toInt(), list[3].toInt());
    }
    else if(list[0] == SOCKET_CMD_HEARTBEAT)
    {
        emit heartBeatFeedbackSignal();
    }
    else if(list[0] == SOCKET_CMD_SELECT_PRINT_DISPLAY && list.size() >= 3)
    {
        emit selectPrintDisplayFeedbackSignal(list[1].toInt(), list[2].toInt());
    }
    else if(list[0] == SOCKET_CMD_LOGIN_WIDGET_DISPLAY && list.size() >= 3)
    {
        emit loginWidgetDisplayFeedbackSignal(list[1].toInt(), list[2].toInt());
    }
    else if(list[0] == SOCKET_CMD_CLOSE_NETWORK_PROCESS && list.size() >= 3)
    {
        emit closeNetworkProcessFBSignal(list[1].toInt(), list[2].toInt());
    }
    else if(list[0] == SOCKET_CMD_SELECT_FROM_UPDATE && list.size() >= 5)
    {
        emit selectFromUpdateSignal(qint64(list[1].toInt()), list[2],  QString(list[3]), list[4].toInt());
        qDebug() << list[1].toInt() << ", " << list[3] << ", " << list[4].toInt();
    }
    else if(list[0] == SOCKET_CMD_AUTO_SELECT_FROM_UPDATE && list.size() >= 6)
    {
        emit autoUpdateCmdFeedbackSignal(qint64(list[1].toInt()), list[2],  list[3].toInt(), list[4], list[5].toInt());
    }
    else if(list[0] == SOCKET_CMD_DEVICELIST_WIDGET_DISPLAY && list.size() >= 4)
    {
        emit displayDeviceListWidgetFeedbackSignal(list[1].toInt(), list[3].toInt());
    }
    else if(list[0] == SOCKET_CMD_LOGIN_INIT && list.size() >= 3)
    {
        emit loginInitFeedbackSignal(list[1].toInt(), list[2].toInt());
    }
    else if(list[0] == SOCKET_CMD_PRINT_CTRL_WIDGET_DISPLAY && list.size() >= 3)
    {
        emit displayAnkerMakeMainWindow(false);
    }
    else if(list[0] == SOCKET_CMD_ANKERMAKE_WIDGET_DISPLAY && list.size() >= 2)
    {
        emit displayAnkerMakeMainWindow(true);
        sendData(localStr + ",0");
    }
}

void QMainSocket2::onInitConnect()
{
    qDebug() << "--->>>" << __FUNCTION__;
    connect(m_pInterface, SIGNAL(sigMessage(QString)), this, SLOT(onReceiveMsg(QString)));
    connect(this, SIGNAL(sigSendMsg(QString)), m_pInterface, SLOT(onMessage(QString)));
   // connect(m_pInterface, &QRemoteObjectDynamicReplica::error, this, SLOT(communicationErrorSlot(QRemoteObjectNode::ErrorCode)));

    connect(m_pInterface, &QRemoteObjectDynamicReplica::stateChanged, this, &QMainSocket2::stateChangedSlot);
}

void QMainSocket2::communicationErrorSlot(QRemoteObjectNode::ErrorCode errCode)
{
    emit communicationErrorSignal(errCode);
}

void QMainSocket2::stateChangedSlot(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState)
{
    emit stateChangedSignal(state, oldState);
}

//void QMainSocket2::heartbeatIntervalChanged(int heartbeatInterval)
//{
//     AkUtil::TDebug("HeartBeat: " + QString::number(heartbeatInterval));
//}

