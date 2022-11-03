#include "import_project_thread.h"
#include "xml_stream.h"
#include "../common/GeoAndShow/support/SupportAssemblyMeshes.h"
#include "import_export_support.h"


ImportProjectWorker::ImportProjectWorker(QObject *parent) : m_printModels(nullptr), m_cb(nullptr)
{

}

void ImportProjectWorker::initImportProjectParams(const QString &proPath, std::vector<CH3DPrintModelPtr> *printModels, vcg::CallBackPos *cb)
{
    m_proPath = proPath;
    m_printModels = printModels;
    m_cb = cb;
}

void ImportProjectWorker::doWork()
{
    if(m_printModels == nullptr || m_cb == nullptr)
    {
        return;
    }
    auto bIt = (*m_printModels).begin();
    auto eIt = (*m_printModels).end();
    int count = 0;
    int totalCount = (*m_printModels).size();

    QString tmpProName = m_proPath;
    int index1 = tmpProName.lastIndexOf("/");
    tmpProName = tmpProName.left(tmpProName.size() - 1); 
    int index2 = tmpProName.lastIndexOf("/") + 1;
    QString proName = tmpProName.mid(index2, index1 - index2);
    while (bIt != eIt)
    {
        QString meshName = (*bIt)->getObjectName();
        if ((*bIt)->m_oldMesh == nullptr)
        {
            (*bIt)->m_oldMesh = new CMeshO;
        }

        successful = true;
        emit importModelSignal(++count, totalCount, proName);

        try{
             MeshModelImport_Export::open(QString("obj"), m_proPath + meshName + QString("/") + meshName + QString(".obj"), *(*bIt)->m_oldMesh, m_cb);
         }
        catch(std::exception & e){
            qDebug() << __FUNCTION__ << __LINE__ << e.what();
            successful =false;
            emit  errorEncountered(0, QString::fromLocal8Bit(e.what()));
        }
        catch(...){
           successful =false;
            qDebug() << __FUNCTION__ << __LINE__;
            emit  errorEncountered(0, QString::fromLocal8Bit("Other Error"));
        }


        bool ret = XmlStream::readSupportParamsXml2(*bIt, m_proPath + meshName + QString("/") + meshName + QString(".xml"));
        for (int i = 0; i < (*bIt)->m_supportMeshes.size(); i++)
        {
            SupportAssemblyMeshesPtr assemblyMeshes = std::dynamic_pointer_cast<SupportAssemblyMeshes>((*bIt)->m_supportMeshes[i]);
            if (assemblyMeshes != nullptr)
            {
                for (int j = 0; j < assemblyMeshes->m_baseShowObjs.size(); j++)
                {
                    QString importPath = m_proPath + meshName + QString("/") + QString::number(assemblyMeshes->getId()) + QString("/");
                    bool ret = Import_Export_Support::importSupport(importPath, assemblyMeshes->m_baseShowObjs[j]);
                    if (!ret)
                    {
                        break;
                    }
                }
            }

        }

        bIt++;
    }
    emit importModelSignal(++count, totalCount, proName);
    QThread::sleep(1);
    emit exitThreadSignal();
}
