#include "export_project_thread.h"
#include <QDir>
#include "../common/file_property/xml_stream.h"
#include "../common/file_property/import_export_support.h"
#include "../common/GeoAndShow/support/SupportAssemblyMeshes.h"

bool _QCallBack2(const int pos, const char* str)
{
    static int lastPos = -1;
    if (pos == lastPos) return true;
    lastPos = pos;
    //qApp->processEvents();
    qDebug() << " pos: " << pos << ", str: " << QString(str);
    return true;
}

ExportProjectThread::ExportProjectThread(QObject* parent) : m_cb(nullptr)
{

}

void ExportProjectThread::run()
{
    if (m_cb == nullptr)
    {
        return;
    }
    int totalsize = m_printModels.size();
    for (int i = 0; i < totalsize; i++)
    {
        if (m_printModels[i] == nullptr)
        {
            continue;
        }
        int lastDotIndex = m_printModels[i]->getObjectName().lastIndexOf(QString("."));
        QString tmpMeshName = m_printModels[i]->getObjectName().mid(0, lastDotIndex);
        m_printModels[i]->m_meshPath = tmpMeshName + QString("/");
        bool ret = exportModel(m_proDir, tmpMeshName, *m_printModels[i]->m_oldMesh);
        if (!ret)
        {
            break;
        }
        emit exportModelSignal(i + 1, totalsize, tmpMeshName);
        if (m_printModels[i]->m_supportMeshes.size() <= 0)
        {
            continue;
        }

        ret = XmlStream::writeSupportParamsXml(m_printModels[i], m_proDir + tmpMeshName + QString("/"));
        if (!ret)
        {
            break;
        }
        for (int j = 0; j < m_printModels[i]->m_supportMeshes.size(); j++)
        {
            SupportAssemblyMeshesPtr supportAssemplys = std::dynamic_pointer_cast<SupportAssemblyMeshes>(m_printModels[i]->m_supportMeshes[j]);
            if (supportAssemplys != nullptr)
            {
                for (int k = 0; k < supportAssemplys->m_baseShowObjs.size(); k++)
                {
                    QString exportPath = m_proDir + tmpMeshName + QString("/") + QString::number(supportAssemplys->getId()) + QString("/");
                    ret = Import_Export_Support::exportSupport(exportPath, supportAssemplys->m_baseShowObjs[k]);
                    if (!ret)
                    {
                        break;
                    }
                }
            }
        }
    }
}

bool ExportProjectThread::exportModel(const QString& dirPath, const QString& modelName, const CMeshO& _cm)
{
    int index = modelName.lastIndexOf(QString("."));
    QString tmpDir = modelName.mid(0, index);
    QDir dir(dirPath + tmpDir);
    if (!dir.exists())
        dir.mkpath(dirPath + tmpDir);
    CMeshO cm = _cm;
    QString formatName = QString("obj");
    QString fileName = dirPath + tmpDir + QString("/") + tmpDir + QString(".obj");
    MeshModelImport_Export::save(formatName, fileName, cm, m_mask, m_cb);
    return true;
}

void ExportProjectThread::setPrintModels(const std::vector<CH3DPrintModelPtr>& printModels)
{
    m_printModels = printModels;
}

void ExportProjectThread::initExportProjectParams(const std::vector<CH3DPrintModelPtr>& printModels, const QString& proDir, int mask, vcg::CallBackPos* cb)
{
    m_proDir = proDir;
    m_mask = mask;
    m_cb = cb;
    m_printModels = printModels;
}

ExportProjectWorker::ExportProjectWorker(QObject* parent) : m_cb(nullptr)
{

}

void ExportProjectWorker::initExportProjectParams(const std::vector<CH3DPrintModelPtr>& printModels, const QString& proDir, int mask, vcg::CallBackPos* cb)
{
    m_proDir = proDir;
    m_mask = mask;
    m_cb = _QCallBack2;
    m_printModels = printModels;
}

bool ExportProjectWorker::exportModel(const QString& dirPath, const QString& modelName, CMeshO& _cm)
{
    int index = modelName.lastIndexOf(QString("."));
    QString tmpDir = modelName.mid(0, index);
    QDir dir(dirPath + tmpDir);
    if (!dir.exists())
        dir.mkpath(dirPath + tmpDir);
    //CMeshO cm = _cm;
    QString formatName = QString("obj");
    QString fileName = dirPath + tmpDir + QString("/") + tmpDir + QString(".obj");
    MeshModelImport_Export::save(formatName, fileName, _cm, m_mask, m_cb);
    return true;
}

void ExportProjectWorker::doWork()
{
    if (m_cb == nullptr)
    {
        emit exitThreadSignal();
        return;
    }
    QString tmpProName = m_proDir;
    int index1 = tmpProName.lastIndexOf("/");
    tmpProName = tmpProName.left(tmpProName.size() - 1); 
    int index2 = tmpProName.lastIndexOf("/") + 1;
    QString proName = tmpProName.mid(index2, index1 - index2);
    int totalsize = m_printModels.size();
    for (int i = 0; i < totalsize; i++)
    {
        if (m_printModels[i] == nullptr)
        {
            continue;
        }
        int lastDotIndex = m_printModels[i]->getObjectName().lastIndexOf(QString("."));
        QString tmpMeshName = m_printModels[i]->getObjectName().mid(0, lastDotIndex);
        m_printModels[i]->m_meshPath = tmpMeshName + QString("/");
        emit exportModelSignal(i + 1, totalsize, proName);

        bool ret = true;
        try{
            ret = exportModel(m_proDir, tmpMeshName, *m_printModels[i]->m_oldMesh);
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

        emit exportModelSignal(i + 2, totalsize, proName); 
        QThread::sleep(1);
        if (!ret)
        {
            break;
        }

        if (m_printModels[i]->m_supportMeshes.size() <= 0)
        {
            continue;
        }

        ret = XmlStream::writeSupportParamsXml(m_printModels[i], m_proDir + tmpMeshName + QString("/"));
        if (!ret)
        {
            break;
        }
        for (int j = 0; j < m_printModels[i]->m_supportMeshes.size(); j++)
        {
            SupportAssemblyMeshesPtr supportAssemplys = std::dynamic_pointer_cast<SupportAssemblyMeshes>(m_printModels[i]->m_supportMeshes[j]);
            if (supportAssemplys != nullptr)
            {
                for (int k = 0; k < supportAssemplys->m_baseShowObjs.size(); k++)
                {
                    QString exportPath = m_proDir + tmpMeshName + QString("/") + QString::number(supportAssemplys->getId()) + QString("/");
                    ret = Import_Export_Support::exportSupport(exportPath, supportAssemplys->m_baseShowObjs[k]);
                    if (!ret)
                    {
                        break;
                    }
                }
            }
        }
    }
    emit compressProject();
    emit exitThreadSignal();
}
