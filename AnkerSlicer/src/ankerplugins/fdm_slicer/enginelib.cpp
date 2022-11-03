#include "enginelib.h"
#include <QDebug>
#include <QProcess.h>
#ifdef DEBUG_BY_CL
#include "lib/Application.h"
#endif
#include "common/utilities/tlogger.h""


EngineLibThread *pEngineLib = nullptr;
void engineCallBack(const char *logData, int n){
    if(pEngineLib){
        QByteArray qLogData(logData, n);
        qCritical() << "[From EngineLib]" << QString(qLogData);
        emit pEngineLib->newStdErrGeneratedChars(qLogData);
    }
}


EngineLibThread::EngineLibThread(QObject *parent):QThread(parent)
{
    pEngineLib = this;
}

EngineLibThread::~EngineLibThread()
{
    pEngineLib = nullptr;
}

void EngineLibThread::run()
{
#ifdef DEBUG_BY_CL
    QStringList strList;


    strList = QProcess::splitCommand(m_cmd);

    QList<QByteArray> args_list;
    for (int i = 0; i < strList.size(); ++i) {
        args_list.append(strList[i].toLocal8Bit());
    }
    QVector<char *> args_vector;
    for(int i=0; i<args_list.size(); ++i){
        args_vector.append(args_list[i].data());
    }

    const int argc = args_list.size();
    char** argv = args_vector.data();

    cura::Application curaApp(engineCallBack); //
    cura::Application::getInstance().run(argc, argv);
#endif
}

bool EngineLibThread::runCmd(QString cmd)
{
    m_cmd = cmd;
    start();
    return true;
}





