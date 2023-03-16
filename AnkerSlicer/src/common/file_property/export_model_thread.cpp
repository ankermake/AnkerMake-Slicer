#include "export_model_thread.h"


ExportModelThread::ExportModelThread(QObject *parent) : m_cm(nullptr), m_cb(nullptr)
{

}

ExportModelThread::~ExportModelThread()
{

}

void ExportModelThread::run()
{
    if(m_cm == nullptr)
    {
        return;
    }
    successful = true;
    try{
         MeshModelImport_Export::save(m_formatName, m_fileName, *m_cm, m_mask, m_cb);
    }
    catch(std::exception & e){
        qDebug() << __FUNCTION__ << __LINE__ << e.what();
        successful =false;
        emit  errorEncountered(0, QString::fromLocal8Bit(e.what()));
    }
    catch(...){
       successful =false;
        qDebug() << __FUNCTION__ << __LINE__;
        emit  errorEncountered(0, tr("An unknown error has occurred, please try again"));
    }
}

void ExportModelThread::initExportParam(const QString &formatName, const QString &fileName, CMeshO *cm, int mask, vcg::CallBackPos *cb)
{
    m_formatName = formatName;
    m_fileName = fileName;
    m_cm = cm;
    m_mask = mask;
    m_cb = cb;
}


