#include "LocalServer.h"


LocalServer::LocalServer() : m_server(NULL)
{
    m_server = new QLocalServer;
    connect(m_server, SIGNAL(newConnection()), SLOT(serverNewConnectionHandler()));
}

LocalServer::~LocalServer()
{
    m_server->close();
    delete m_server;
}

void LocalServer::RunServer(QString serverName)
{
    QLocalServer::removeServer(serverName);
    bool ok = m_server->listen(serverName);
    if (!ok)
    {
        qDebug() << "Run Server fail";
    }
    qDebug() << "Run Server ok";
}

void LocalServer::serverNewConnectionHandler()
{
    qDebug() << "new connection to" << m_server->serverName();
    QLocalSocket *socket = m_server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadHandler()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void LocalServer::socketReadyReadHandler()
{
    QLocalSocket *socket = static_cast<QLocalSocket *>(sender());
    if (socket)
    {
        QTextStream stream(socket);
        QString recvMsg = stream.readAll();

        
        QStringList fileNameList;
        fileNameList.append(recvMsg.toLocal8Bit().data());
        emit processArgvfileNameMsg(fileNameList);

        qDebug() << m_server->serverName()<< " receive msg:"<< recvMsg.toLocal8Bit().data() ;
    }
    else
    {
    }
}


LocalClient::LocalClient() : m_socket(NULL)
{
    m_socket = new QLocalSocket();

    connect(m_socket, SIGNAL(connected()), SLOT(socketConnectedHandler()));
    connect(m_socket, SIGNAL(disconnected()), SLOT(socketDisConnectedHandler()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), SLOT(socketErrorHandler(QLocalSocket::LocalSocketError)));
}

LocalClient::~LocalClient()
{
    m_socket->disconnectFromServer();
    delete m_socket;
}

bool LocalClient::ConnectToServer(const QString &strServerName)
{
    
    m_socket->connectToServer(strServerName);
    if (m_socket->waitForConnected())
    {
         return true;
    }
    return false;
}

void LocalClient::sendMessage(const QString &msg)
{
    m_socket->write(msg.toStdString().c_str());
    m_socket->flush();
}

void LocalClient::socketConnectedHandler()
{
    qDebug() << "connected.";
}

void LocalClient::socketDisConnectedHandler()
{
    qDebug() << "disconnected.";
}

void LocalClient::socketErrorHandler(QLocalSocket::LocalSocketError error)
{
    qWarning() << error;
}
