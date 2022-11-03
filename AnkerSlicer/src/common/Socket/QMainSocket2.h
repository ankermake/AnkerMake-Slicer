#ifndef QMAINSOCKET2_H
#define QMAINSOCKET2_H
#include "SocketBase.h"
#include <QObject>
#include <QRemoteObjectNode>
#include <QRemoteObjectDynamicReplica>
#include <QWidget>
class QMainSocket2 : public QObject
{
    Q_OBJECT
public:
    QMainSocket2(QObject *parent = nullptr);
    ~QMainSocket2();

    void loginInit(qint64);

    void sendData(const QString &);
    qint64 loginWidgetDisplay(qint64 id, WId wId);
    qint64 selectPrintDisplay(qint64 id);
    void selectFromUpdate(qint64 id, const QString &version);
    void autoUpdateCmd(qint64 id, const QString &version, int start);
    qint64 sendFile(const QString &filePath, qint64 id);

    void displayDeviceListWidget(qint64 id, const QString &fileName);

    qint64 closeNetworkProcess(qint64 id);
    void msleep(unsigned int ms);

    QRemoteObjectDynamicReplica::State getState() const;

    void printCtrlWidgetDisplay(qint64 id);

signals:
    void sigSendMsg(QString msg);
    void heartBeatFeedbackSignal();
    void loginInitFeedbackSignal(qint64, int);
    void loginWidgetDisplayFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);
    void selectPrintDisplayFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);

    void selectFromUpdateSignal(qint64, const QString &/*version*/, const QString &, int);
    void autoUpdateCmdFeedbackSignal(qint64 /*id*/, const QString &/*version*/, int/*start*/, const QString &/*FileName*/, int/*success*/);

    void sendFileFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);
    void displayDeviceListWidgetFeedbackSignal(qint64, int);
    void closeNetworkProcessFBSignal(qint64, int);

    void displayAnkerMakeMainWindow(bool showed);

    void communicationErrorSignal(QRemoteObjectNode::ErrorCode);
    void stateChangedSignal(QRemoteObjectDynamicReplica::State state, QRemoteObjectDynamicReplica::State oldState);


private slots:
    void onReceiveMsg(QString msg);
    void onInitConnect();
    void communicationErrorSlot(QRemoteObjectNode::ErrorCode);
    void stateChangedSlot(QRemoteObjectDynamicReplica::State state, QRemoteObjectDynamicReplica::State oldState);

    //void heartbeatIntervalChanged(int heartbeatInterval);

private:
    QRemoteObjectNode *m_pRemoteNode;
    QRemoteObjectDynamicReplica *m_pInterface;

    bool firstHeartBeat;
};



#endif
