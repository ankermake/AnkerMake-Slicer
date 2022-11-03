#ifndef HEARTBEAT_THREAD_H
#define HEARTBEAT_THREAD_H

#include "SocketBase.h"
#include "QMainSocket2.h"
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QDateTime>
#include <QProcess>
#include <QTimer>
#include <QSharedPointer>
class HeartBeatThread : public QThread
{
    Q_OBJECT
public:
    explicit HeartBeatThread(QObject *parent = nullptr);
    ~HeartBeatThread();
    void run();
    bool processIsRun(const QString &exeName);

private slots:
    //void errorOccurred(QProcess::ProcessError error);
    void processErrorOccurred(QProcess::ProcessError error);
    void heartBeatFeedbackSlot();

private:
    QSharedPointer<QMainSocket2> m_socket;
    QProcess* m_process;
    int heartBeatCount = 0;
    bool started = false;
};

class HeartBeatWork : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeatWork(QObject *parent = nullptr);
    ~HeartBeatWork();

public slots:
    void init();
    void doWork();
    void stopTimer();

private:
    bool processIsRun(const QString &exeName);
private slots:
    void heartBeatFeedbackSlot();
    void processErrorOccurred(QProcess::ProcessError error);
private:
    QSharedPointer<QMainSocket2> m_socket;
    //QSharedPointer<QTimer> m_heartTimer;
    QProcess* m_process;
    int heartBeatCount = 0;
    bool started = false;
};



#endif // HEARTBEAT_THREAD_H

