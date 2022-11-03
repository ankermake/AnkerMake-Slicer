#ifndef ENGINELIB_H
#define ENGINELIB_H
#include <QObject>
#include <QThread>

typedef void (*FuncEngineLogCallbackPtr)(const char *, int);
void engineCallBack(const char * logData, int n);

class EngineLibThread : public QThread
{
    Q_OBJECT

public:
    explicit EngineLibThread(QObject *parent = nullptr);
    ~EngineLibThread();

    bool runCmd(QString cmd); 
signals:
    void newStdErrGeneratedChars(QByteArray);

    // QThread interface
protected:
    QString m_cmd;
    void run();
};

#endif // ENGINELIB_H
