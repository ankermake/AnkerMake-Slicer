#ifndef QMAINSOCKET_H
#define QMAINSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "HeartBeatThead.h"


class QMainSocket : public QObject
{
    Q_OBJECT

public:
    QMainSocket(QObject *parent = nullptr);
    ~QMainSocket();
    //void createHeartBeatThread();
    void connectServer(QString ip = SOCKET_IP, int port = SOCKET_PORT);
    void sendData(const QByteArray &);
    void sendData(const QString &);

    qint64 loginWidgetDisplay(qint64 id);

    qint64 selectPrintDisplay(qint64 id);

    qint64 checkConnectState();

    qint64 sendFile(const QString &filePath, qint64 id);
    void msleep(unsigned int ms);

    void closeSocket();

signals:
    void recvDataSignal(const QString &);
    void loginWidgetDisplayFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);
    void selectPrintDisplayFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);
    void sendFileFeedbackSignal(qint64 /*Cmd id*/, int /*Cmd Work Success/Failed*/);

    void socketErrorSignal(QAbstractSocket::SocketError);
    void stateChangedSignal(QAbstractSocket::SocketState);

public slots:
    void readDataSlot();
    void socketErrorSlot(QAbstractSocket::SocketError);
    void stateChangedSlot(QAbstractSocket::SocketState);

private:
    QTcpSocket *m_socket;
//    HeartBeatThread *m_heartBeatThread;
};

#endif
