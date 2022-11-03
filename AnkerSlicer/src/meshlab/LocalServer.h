#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QObject>
#include <QLocalSocket>
#include <QLocalServer>

class LocalServer : public QObject
{
    Q_OBJECT

public:
    LocalServer();
    ~LocalServer();
    void RunServer(QString serverName);

signals:
    void processArgvfileNameMsg(QStringList strList);

private slots:
    void serverNewConnectionHandler();
    void socketReadyReadHandler();

private:
    QLocalServer *m_server;
};


class LocalClient : public QObject
{
    Q_OBJECT

public:
    LocalClient();
    ~LocalClient();

public:
    bool ConnectToServer(const QString &strServerName);
    void sendMessage(const QString &msg);

private slots:
    void socketConnectedHandler();
    void socketDisConnectedHandler();
    void socketErrorHandler(QLocalSocket::LocalSocketError error);

private:
    QLocalSocket *m_socket;
};

#endif // LOCALSERVER_H
