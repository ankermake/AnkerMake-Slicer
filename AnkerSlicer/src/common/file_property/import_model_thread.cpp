#include "import_model_thread.h"
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

ImportModelThread::ImportModelThread(QObject* parent) : m_exit(false), m_cm(NULL)
{

}

void ImportModelThread::run()
{
    if (m_cm == nullptr)
    {
        return;
    }
    successful = true;
    try {
        QString tmpFileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/";
        QFile tmpFile(m_fileName);
        int index = m_fileName.lastIndexOf("/") + 1;
        tmpFileName += m_fileName.mid(index, m_fileName.length() - index);
        bool ret = tmpFile.copy(tmpFileName);
        //qDebug() << "copy " << ret << ", source: " << m_fileName << ", target: " << tmpFileName;
        if(ret)
        {
            ret = MeshModelImport_Export::open(m_formatName, tmpFileName, *m_cm, m_cb);
        }
        if (!ret)
        {
            successful = false;
            emit  errorEncountered(0, QString::fromLocal8Bit("Other Error"));
        }
        QFile::remove(tmpFileName);
    }
    catch (std::exception& e) {
        qDebug() << __FUNCTION__ << __LINE__ << e.what();
        successful = false;
        emit  errorEncountered(0, QString::fromLocal8Bit(e.what()));
    }
    catch (...) {
        successful = false;
        qDebug() << __FUNCTION__ << __LINE__;
        emit  errorEncountered(0, QString::fromLocal8Bit("Other Error"));
    }
    //MeshModelImport_Export::open(m_formatName, m_fileName, *m_cm, m_cb);
//    while(1)
//    {
//        if(m_exit)
//        {
//            break;
//        }
//        sleep(1);
//    }
    m_exit = false;
    qDebug() << "Import Thread m_cm.VertexNumber(): " << m_cm->VertexNumber();
}

void ImportModelThread::initOpenParam(const QString& formatName, const QString& fileName, CMeshO* cm, vcg::CallBackPos* cb)
{
    m_formatName = formatName;
    m_fileName = fileName;
    m_cm = cm;
    m_cb = cb;
}

void ImportModelThread::exitThreadSLot()
{
    m_exit = true;
}

void ImportModelThread::usbChanged(int state)
{
    if(state == 1) 
    {
        successful = false;
    }
}

ImportModelWoker::ImportModelWoker(QObject* parent) : m_cm(NULL)
{

}

void ImportModelWoker::initOpenParam(const QString& formatName, const QString& fileName, CMeshO* cm, vcg::CallBackPos* cb)
{
    m_formatName = formatName;
    m_fileName = fileName;
    m_cm = cm;
    m_cb = cb;
}

void ImportModelWoker::doWork()
{
    if (m_cm == nullptr)
    {
        return;
    }
    MeshModelImport_Export::open(m_formatName, m_fileName, *m_cm, m_cb);
    emit exitThreadSignal();
    //QString result;
    // ...
    //emit resultReady(result);
}

ImportModelController::ImportModelController()
{
    ImportModelWoker* worker = new ImportModelWoker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ImportModelController::operate, worker, &ImportModelWoker::doWork);
    connect(worker, &ImportModelWoker::resultReady, this, &ImportModelController::handleResults);
}

ImportModelController::~ImportModelController()
{
    workerThread.quit();
    workerThread.wait();
}

void ImportModelController::handleResults(const QString&)
{

}
