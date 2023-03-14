#include "project_load_save.h"
#include "mesh_model_import_export.h"
#include "../common/GeoAndShow/CHCuboid.h"
#include "../common/GeoAndShow/support/SupportAssemblyMeshes.h"
#include "../common/utilities/tlogger.h"
#include "../common/controlInterface/messageDialog.h"
#include "mlexception.h"
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

ExportProjectWorker* exportProjectWorker = nullptr;
ImportProjectWorker* importProjectWorker = nullptr;


ProjectLoadSave ProjectLoadSave::_instance;

ProjectLoadSave& ProjectLoadSave::getInstance()
{
    return _instance;
}

bool ProjectLoadSave::_QCallBack(const int pos, const char* str)
{
    static int lastPos = -1;
    if (pos == lastPos) return true;
    lastPos = pos;
    return true;
}

ProjectLoadSave::ProjectLoadSave() :
    m_exportProjectProgress(nullptr),
    m_importProjectProgress(nullptr),
    mainWidget(nullptr),
    m_exportProjectThread(nullptr),
    m_importProjectThread(nullptr)
{

}

ProjectLoadSave::~ProjectLoadSave()
{

}

void ProjectLoadSave::setMainWidget(QWidget* parentWidget)
{
    mainWidget = parentWidget;
}



bool ProjectLoadSave::loadProject(const QString& proPath, PluginManager& plugins)
{
    if (proPath.isEmpty())
        return false;
    int index = proPath.lastIndexOf(QString("/"));
    QString filePath = proPath.mid(0, index + 1);
    QString proName = proPath.mid(index + 1, proPath.length() - (index + 1));
    QString outName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/";

    index = proName.lastIndexOf(QString("."));
    QString tmpName = proName.mid(0, index);

    QDir dir(outName + tmpName);
    if (!dir.exists())
        dir.mkpath(outName + tmpName);
    m_file_property.clear();
    for (auto it : plugins.pluginIterator())
    {
        if (it->needLoadProject())
        {

            QDir tmpDir(outName + tmpName + QString("/plugins/") + it->pluginName());
            if (!tmpDir.exists())
                tmpDir.mkpath(outName + tmpName + QString("/plugins/") + it->pluginName());
        }

    }
    m_file_property.m_docPtr = getDoc();
    bool ret = loadZip(filePath, proName, outName, plugins);

    if (ret)
    {

        if (!m_file_property.m_load_project_property.m_loadProjected && m_file_property.m_load_project_property.m_lastProjectFile != proPath)
        {
            m_file_property.m_load_project_property.m_lastProjectFile = proPath;
            m_file_property.m_load_project_property.m_loadProjected = true;
        }
    }

    return ret;
}

bool ProjectLoadSave::saveProject(PluginManager& plugins, const QString& filePath)
{
    if (filePath.isEmpty())
        return false;
    QFile file(filePath);
    if (file.exists())
    {
        file.remove();
    }

    return buildProject(filePath, plugins);
}

bool ProjectLoadSave::buildProject(const QString& str, PluginManager& plugins)
{
    int index1 = str.lastIndexOf(QString("/")) + 1;
    int index2 = str.lastIndexOf(QString("."));
    QString fileName = str.mid(index1, str.length() - index1);
    qDebug() << "fileName: " << fileName << ", index1: " << index1 << ", index2: " << index2 << ", str: " << str;
    QString proPath = str.mid(0, index1);
    QString proName = str.mid(index1, index2 - index1);
    QString tmpProDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/";
    QString layer = tmpProDir;
    QDir dir(tmpProDir + proName);
    if (!dir.exists())
    {
        dir.mkpath(tmpProDir + proName);
    }
    for (auto it : plugins.pluginIterator())
    {
        if (it->needSaveProject())
        {
            QDir tmpDir(tmpProDir + proName + QString("/plugins/") + it->pluginName());
            if (!tmpDir.exists())
                tmpDir.mkpath(tmpProDir + proName + QString("/plugins/") + it->pluginName());
            it->saveProject(tmpProDir + proName + QString("/plugins/") + it->pluginName());
        }
    }

    m_file_property.m_docPtr = getDoc();
    QString proDir = tmpProDir + proName + QString("/");
    index1 = proDir.lastIndexOf(QString("/"));
    proDir.chop(1);
    index2 = proDir.lastIndexOf(QString("/")) + 1;

    m_file_property.m_docPtr->m_path = proDir.mid(index2, index1 - index2) + QString("/");
    CHDocPtr doc_ptr = m_file_property.m_docPtr;
    if (doc_ptr == nullptr)
    {
        return false;
    }
    m_exportProjectProgress = new ProgressDialog((QDialog*)mainWidget);
    m_exportProjectProgress->setAutoClosed(false);
    m_exportProjectProgress->setModal(true);
    m_exportProjectProgress->setWindowModality(Qt::ApplicationModal);
    m_exportProjectProgress->setCancelVisible(false);
    exportProjectWorker = new ExportProjectWorker();
    if(proDir.lastIndexOf("/") != proDir.length() - 1)
    {
        proDir += "/";
    }
    exportProjectWorker->initExportProjectParams(doc_ptr->m_printObjs, proDir, 1, &_QCallBack);
    m_exportProjectThread = new QThread();
    exportProjectWorker->moveToThread(m_exportProjectThread);
    connect(exportProjectWorker, &ExportProjectWorker::exitThreadSignal, this, &ProjectLoadSave::exportProjectProgressFinished);
    connect(m_exportProjectThread, &QThread::finished, exportProjectWorker, &QObject::deleteLater);  
    connect(exportProjectWorker, &ExportProjectWorker::exportModelSignal, this, &ProjectLoadSave::exportProjectProgress);
    connect(m_exportProjectThread, &QThread::finished, m_exportProjectThread, &QThread::deleteLater);
    connect(m_exportProjectThread, &QThread::started, exportProjectWorker, &ExportProjectWorker::doWork);
    connect(exportProjectWorker, &ExportProjectWorker::errorEncountered, this, &ProjectLoadSave::exportProjectError);
    m_exportProjectThread->start();

    connect(exportProjectWorker, &ExportProjectWorker::compressProject, [this, proPath, proName, fileName, layer, tmpProDir]() {
        QString tmpLayer = layer;
        saveZip(proPath, proName, fileName, tmpLayer, tmpProDir);
    });

    m_exportProjectProgress->exec();
    return true;
}

void ProjectLoadSave::setMeshDocumentFromDocumentProperty()
{
    /*auto docPtr = getDoc();
    docPtr->clearDocMeshModels();

    auto bIt = m_file_property.m_document_property.models.begin();
    auto eIt = m_file_property.m_document_property.models.end();

    while (bIt != eIt)
    {
        MeshModel mm((*bIt).meshId, (*bIt).meshPath, (*bIt).meshName);
        mm.cm = (*bIt).cm;
        CH3DPrintModelPtr tmpModelPtr = mm.toMyModel();
        tmpModelPtr->setColor((*bIt).meshColor);
        tmpModelPtr->setTransform((*bIt).matrix);
        tmpModelPtr->setVisuable((*bIt).visible);
        tmpModelPtr->setObjectName((*bIt).meshName);
        tmpModelPtr->setId((*bIt).meshId);
        tmpModelPtr->m_showMode = (*bIt).showMode;
        tmpModelPtr->m_rotCenter = (*bIt).rotCenter;
        tmpModelPtr->m_params = (*bIt).params;
        tmpModelPtr->calBaseAABB();
        tmpModelPtr->calRealAABB();
        tmpModelPtr->updateToScene();
        docPtr->m_printObjs.push_back(tmpModelPtr);
        bIt++;
    }  */
}

bool ProjectLoadSave::loadZip(QString filePath, const QString& proName, const QString& outPath, PluginManager& plugins)
{
    bool ret = false;
    QString tmpOutPath = outPath;
    QZipReader reader(filePath + proName);

    int _index = proName.lastIndexOf(QString("."));
    QString tmpName = proName.mid(0, _index);
    QDir _dir(tmpOutPath + tmpName);
    if (!_dir.exists())
        _dir.mkdir(tmpOutPath + tmpName);
    tmpOutPath += tmpName;
    tmpOutPath += QString("/");

    QString tmpProName = "";
    QVector<QZipReader::FileInfo> fileListInfo = reader.fileInfoList();

    for (QZipReader::FileInfo f : fileListInfo)
    {
        int index = f.filePath.lastIndexOf(QString("/"));
        int _count = f.filePath.count(QChar('/'));
        tmpProName = f.filePath.mid(0, index);
        if (index != -1 && _count == 2)
        {
            QString projectDir;
            if (f.filePath.mid(tmpName.length(), 1) == QString("/"))
            {
                projectDir = f.filePath.mid(tmpName.length() + 1, index - (tmpName.length() + 1));
            }
            else
            {
                projectDir = f.filePath.mid(0, index);
            }

            QDir dir(tmpOutPath + projectDir);
            if (!dir.exists())
                dir.mkdir(tmpOutPath + projectDir);
        }

        if (f.isDir)
        {
            int index = f.filePath.lastIndexOf(QString("/"));
            QString dirName = f.filePath;
            if (!f.filePath.mid(0, tmpName.length()).compare(tmpName, Qt::CaseInsensitive) && f.filePath.mid(tmpName.length(), 1) == QString("/"))
            {
                dirName = f.filePath.mid(tmpName.length() + 1, index - (tmpName.length() + 1));
            }
            QDir dir(tmpOutPath + dirName);
            if (!dir.exists())
                dir.mkdir(tmpOutPath + dirName);
        }
        else if (f.isFile)
        {
            QString fileName = f.filePath;
            int index = f.filePath.indexOf(QString("/")) + 1;
            if (!f.filePath.mid(0, tmpName.length()).compare(tmpName, Qt::CaseInsensitive) && f.filePath.mid(tmpName.length(), 1) == QString("/"))
            {
                fileName = f.filePath.mid(index, f.filePath.length() - index);
            }

            QFile file;
            QString tmpfileName = QString::fromLocal8Bit((tmpOutPath + fileName).toLocal8Bit());
            int tmpIndex1 = tmpfileName.lastIndexOf("/");
            QDir tmpDir = tmpfileName.mid(0, tmpIndex1);
            if(!tmpDir.exists())
            {
                tmpDir.mkpath(tmpfileName.mid(0, tmpIndex1));
            }
            AkUtil::TDebug("Write File Path: " + tmpfileName);
            file.setFileName(tmpfileName);
            file.open(QIODevice::WriteOnly);
            file.write(reader.fileData(QString::fromLocal8Bit(f.filePath.toUtf8())));
            file.flush();
            file.close();
        }
    }

    reader.close();
    for (auto it : plugins.pluginIterator())
    {
        if (it->needLoadProject())
        {
            QString loadPath = tmpOutPath;
            loadPath = loadPath.mid(0, loadPath.length()-1);
            int tmpIndex = loadPath.lastIndexOf("/")+1;
            loadPath = loadPath.mid(tmpIndex, loadPath.length() - tmpIndex);
            if(loadPath != tmpProName)
            {
                tmpOutPath += (tmpProName + QString("/"));
            }
            AkUtil::TDebug("loadPath: " + loadPath + ", tmpProName: " + tmpProName);
            AkUtil::TDebug("Plugin Path: " + tmpOutPath + QString("plugins/") + it->pluginName());
            it->loadProject(tmpOutPath + QString("plugins/") + it->pluginName());
            m_file_property.m_plugins_property.pluginNames.push_back(it->pluginName().toStdString());
        }

    }
    AkUtil::TDebug("Read Xml Path: " + tmpOutPath);
    ret = readAllXml(tmpOutPath);
    AkUtil::TDebug("Delete Dir: " + outPath + tmpName);
    ret &= deleteDir(outPath + tmpName);
    if (curScene)
    {
        curScene->updateDoc();
        curScene->refresh();
    }

    return ret;
}

bool ProjectLoadSave::saveZip(QString filePath, const QString& folderName, QString proName, QString& layer, const QString& outPath)
{
    bool result = writeAllXml(outPath + folderName + QString("/"));
    if (!result)
    {
        throw MLException("Error while writing xml.");
    }
    QDir tmpDir(filePath);
    if(!tmpDir.exists())
    {
        control::MessageDialog messageBox(tr("Warning"), tr("Save Path Not Found"), MessageDialog::OK);
        messageBox.exec();
        QFileDialog fileDialog;
        filePath = QFileDialog::getSaveFileName(NULL, tr("Save File"), QApplication::applicationDirPath() + QString("/Document/"), QString("Project File(*.akpro)"));
        int index1 = filePath.lastIndexOf("/") + 1;
        int index2 = filePath.lastIndexOf(".");
        proName = filePath.mid(index1, index2 - index1) + QString(".akpro");
        filePath = filePath.mid(0, index1);
    }
    QZipWriter* writer = new QZipWriter(filePath + proName);
    result = QZipWriterEx(writer, outPath + folderName + QString("/"), layer);
    if (!result)
    {
        throw MLException("Error while compressed files.");
    }
    writer->close();
    delete writer;
    if (result)
    {
        result = deleteDir(outPath + folderName + QString("/"));
    }
    return result;
}


bool ProjectLoadSave::QZipWriterEx(QZipWriter* writer, QString dirPath, QString layer)
{
    QDir dir(dirPath);
    if (dir.exists())
    {
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::DirsFirst);
        QFileInfoList list = dir.entryInfoList();
        int i = 0;
        do
        {
            if (list.size() == 0)
                continue;
            QFileInfo fileInfo = list.at(i);
            bool bisDir = fileInfo.isDir();
            if (bisDir) {
                QZipWriterEx(writer, fileInfo.filePath(), layer);
            }
            else
            {
                QFile file(fileInfo.filePath());
                if (file.exists())
                {
                    file.open(QIODevice::ReadOnly);
                    QString fileName = file.fileName().replace(QApplication::applicationDirPath() + QString("/"), QString(""));

                    if (layer != QString(""))
                    {
                        fileName = fileName.replace(layer, QString(""));
                    }
                    writer->addFile(fileName, file.readAll());
                }
                file.close();
            }
            i++;
        } while (i < list.size());
    }
    else
    {

        return false;
    }
    return true;
}

bool ProjectLoadSave::readAllXml(const QString& proPath)
{
    bool ret = false;
    ret = readXml(proPath + QString("App.xml"), QString("App"), XmlStream::XmlStreamType::XmlStreamType_App);
    if (!ret)
    {
        throw MLException("Error while reading App.xml.");
        return false;
    }
    ret = readXml(proPath + QString("Document.xml"), QString("Document"), XmlStream::XmlStreamType::XmlStreamType_Document);
    if (!ret)
    {
        throw MLException("Error while reading Document.xml.");
        return false;
    }

    if (m_file_property.m_docPtr == nullptr)
        return false;
    importProjectWorker = new ImportProjectWorker();
    m_importProjectThread = new QThread();
    importProjectWorker->initImportProjectParams(proPath, &m_file_property.m_docPtr->m_printObjs, _QCallBack);
    importProjectWorker->moveToThread(m_importProjectThread);
    m_importProjectProgress = new ProgressDialog((QDialog*)mainWidget);
    m_importProjectProgress->setModal(true);
    m_importProjectProgress->setAutoClosed(false);
    m_importProjectProgress->setCancelVisible(false);
    connect(m_importProjectThread, &QThread::finished, importProjectWorker, &QObject::deleteLater);
    connect(m_importProjectThread, &QThread::finished, m_importProjectThread, &QThread::deleteLater);
    connect(importProjectWorker, &ImportProjectWorker::exitThreadSignal, this, &ProjectLoadSave::importProjectProgressFinished);
    connect(importProjectWorker, &ImportProjectWorker::importModelSignal, this, &ProjectLoadSave::importProjectProgress);
    connect(importProjectWorker, &ImportProjectWorker::errorEncountered, this, &ProjectLoadSave::importProjectError);

    connect(m_importProjectThread, &QThread::started, importProjectWorker, &ImportProjectWorker::doWork);
    m_importProjectThread->start();
    m_importProjectProgress->exec();
    ret = readXml(proPath + QString("View.xml"), QString("View"), XmlStream::XmlStreamType::XmlStreamType_View);
    if (!ret)
    {
        throw MLException("Error while reading View.xml.");
        return false;
    }
    return ret;
}
bool ProjectLoadSave::readXml(const QString& dirPath, const QString& xmlName, const XmlStream::XmlStreamType& xmlType)
{
    XmlStream rXml(dirPath, &m_file_property, xmlType);
    return rXml.readFile();
}

bool ProjectLoadSave::writeAllXml(const QString& proPath)
{
    bool ret = writeXml(proPath + QString("App.xml"), QString("App"), XmlStream::XmlStreamType::XmlStreamType_App);
    if (!ret)
    {
        throw MLException("Error while writing App.xml.");
    }
    ret = writeXml(proPath + QString("Document.xml"), QString("Document"), XmlStream::XmlStreamType::XmlStreamType_Document);
    if (!ret)
    {
        throw MLException("Error while writing Document.xml.");
    }
    ret = writeXml(proPath + QString("View.xml"), QString("View"), XmlStream::XmlStreamType::XmlStreamType_View);
    if (!ret)
    {
        throw MLException("Error while writing View.xml.");
    }
    return ret;
}

bool ProjectLoadSave::writeXml(const QString& dirPath, const QString& xmlName, const XmlStream::XmlStreamType& xmlType)
{
    QFile file(dirPath);
    if (file.exists())
        file.remove();
    int index = dirPath.lastIndexOf(QString("/"));
    QString proPath = dirPath.mid(0, index);
    QDir dir(proPath);
    if (!dir.exists())
    {
        dir.mkpath(proPath);
    }

    XmlStream wXml(dirPath, &m_file_property, xmlType);
    return wXml.writeFile();
}


void ProjectLoadSave::setDocumentPropertyFromMeshDocument(const QString& proDir)
{
    /* m_file_property.m_document_property.clear();
     auto docPtr = getDoc();
     QString tmpProDir = proDir;
     tmpProDir.chop(1);
     int index1 = proDir.lastIndexOf(QString("/"));
     int index2 = tmpProDir.lastIndexOf(QString("/")) + 1;

     m_file_property.m_document_property.pathName = proDir.mid(index2, index1 - index2) + QString("/");

     bool ret = false;
     auto bIt = docPtr->m_printObjs.begin();
     auto eIt = docPtr->m_printObjs.end();
     QString tmpMeshName = QString("Tmp");
     while (bIt != eIt)
     {
         DocumentProperty::MeshProperty tmpMeshPro;
         tmpMeshPro.meshId = (*bIt)->getId();
         int lastDotIndex = (*bIt)->getObjectName().lastIndexOf(QString("."));
         tmpMeshName = (*bIt)->getObjectName().mid(0, lastDotIndex);
         tmpMeshPro.meshName = tmpMeshName;
         tmpMeshPro.meshColor = (*bIt)->getColor();
         tmpMeshPro.visible = (*bIt)->getVisuable();
         tmpMeshPro.matrix = (*bIt)->getTransform();
         tmpMeshPro.meshPath = tmpMeshName + QString("/");
         tmpMeshPro.cm = *(*bIt)->m_oldMesh;

         tmpMeshPro.showMode = (*bIt)->m_showMode;
         tmpMeshPro.rotCenter = (*bIt)->m_rotCenter;
         tmpMeshPro.params = (*bIt)->m_params;

         m_file_property.m_document_property.models.push_back(tmpMeshPro);
         exportModel(proDir, tmpMeshName, tmpMeshPro.cm);
         bIt++;
     }  */
}

bool ProjectLoadSave::exportDocumentAllMesh(const QString& proDir, CHDocPtr doc_ptr)
{
    if (doc_ptr == nullptr)
    {
        return false;
    }
    m_exportProjectProgress = new ProgressDialog((QDialog*)mainWidget);
    m_exportProjectProgress->setAutoClosed(false);
    m_exportProjectProgress->setModal(true);
    m_exportProjectProgress->setWindowModality(Qt::ApplicationModal);
    m_exportProjectProgress->setCancelVisible(false);
    exportProjectWorker = new ExportProjectWorker();
    exportProjectWorker->initExportProjectParams(doc_ptr->m_printObjs, proDir, 1, &_QCallBack);
    m_exportProjectThread = new QThread();
    exportProjectWorker->moveToThread(m_exportProjectThread);
    connect(exportProjectWorker, &ExportProjectWorker::exitThreadSignal, this, &ProjectLoadSave::exportProjectProgressFinished);
    connect(m_exportProjectThread, &QThread::finished, exportProjectWorker, &QObject::deleteLater);  
    connect(exportProjectWorker, &ExportProjectWorker::exportModelSignal, this, &ProjectLoadSave::exportProjectProgress);
    connect(m_exportProjectThread, &QThread::finished, m_exportProjectThread, &QThread::deleteLater);
    connect(m_exportProjectThread, &QThread::started, exportProjectWorker, &ExportProjectWorker::doWork);
    connect(exportProjectWorker, &ExportProjectWorker::errorEncountered, this, &ProjectLoadSave::exportProjectError);
    //QTimer::singleShot(0, exportProjectWorker, &ExportProjectWorker::doWork);
    m_exportProjectThread->start();
    m_exportProjectProgress->exec();

    return true;
}

void ProjectLoadSave::setAppPropertyFromMain(const AppParamsProperty& params)
{
    m_file_property.m_appParams_property = params;
}

void ProjectLoadSave::setViewPropertyFromViewCK(const ViewProperty& view)
{
    m_file_property.m_view_property = view;
}


bool ProjectLoadSave::exportModel(const QString& dirPath, const QString& modelName, const CMeshO& _cm)
{
    int index = modelName.lastIndexOf(QString("."));
    QString tmpDir = modelName.mid(0, index);
    QDir dir(dirPath + tmpDir);
    if (!dir.exists())
        dir.mkpath(dirPath + tmpDir);
    CMeshO cm = _cm;
    MeshModelImport_Export::save(QString("obj"), dirPath + tmpDir + QString("/") + tmpDir + QString(".obj"), cm, 1, _QCallBack/*myCallBack*/);
    return true;
}

bool ProjectLoadSave::deleteDir(const QString& proDir)
{
    QDir dir(proDir);
    return dir.removeRecursively();
}

bool ProjectLoadSave::saveModelFolder(const QString& dirPath, const QString& folderName)
{
    return true;
}

void ProjectLoadSave::setFileProperty(FileProperty& _property)
{
    m_file_property = _property;
}

FileProperty& ProjectLoadSave::getFileProperty()
{
    return m_file_property;
}

FileProperty ProjectLoadSave::getFileProperty() const
{
    return m_file_property;
}

void ProjectLoadSave::exportProjectProgress(int num, int totalNum, const QString& str)
{
    if (m_exportProjectProgress == nullptr)
    {
        return;
    }
    qDebug() << "str: " << str << ", num: " << num;
    m_exportProjectProgress->setText(str);
    if(num > totalNum)
    {
        m_exportProjectProgress->setValue(99);
    }
    else if(num >= 0 && num <= totalNum)
    {
        float value = (float)num / (totalNum + 1);
        m_exportProjectProgress->setValue(value * 100);
    }

    qApp->processEvents();
}

void ProjectLoadSave::exportProjectProgressFinished()
{
    if (m_exportProjectProgress == nullptr)
    {
        return;
    }
    m_exportProjectProgress->close();
    delete m_exportProjectProgress;
    m_exportProjectProgress = nullptr;
    m_exportProjectThread->quit();
    m_exportProjectThread->wait();
    delete m_exportProjectThread;
    m_exportProjectThread = nullptr;
    exportProjectWorker = nullptr;
}

void ProjectLoadSave::importProjectProgress(int num, int totalNum, const QString& str)
{
    if (m_importProjectProgress == nullptr)
    {
        return;
    }
    m_importProjectProgress->setText(str);
    if(num > totalNum)
    {
        m_importProjectProgress->setValue(99);
    }
    else if(num >= 0 && num <= totalNum)
    {
        float value = (float)num / (totalNum + 1);
        m_importProjectProgress->setValue(value * 100);
    }
    qApp->processEvents();
}

void ProjectLoadSave::importProjectProgressFinished()
{
    if (m_importProjectProgress == nullptr)
    {
        return;
    }
    m_importProjectProgress->close();
    delete m_importProjectProgress;
    m_importProjectProgress = nullptr;

    m_importProjectThread->quit();
    m_importProjectThread->wait();
    delete  m_importProjectThread;
    m_importProjectThread = nullptr;
    importProjectWorker = nullptr;
}

void ProjectLoadSave::importProjectError(int type, const QString &errStr)
{
    AkUtil::TFunction("");
    AkUtil::TError(errStr);
    control::MessageDialog box(tr("Error"), tr("Import Project Error").append(errStr), MessageDialog::OK, nullptr);
    box.exec();
}

void ProjectLoadSave::exportProjectError(int type, const QString &errStr)
{
    AkUtil::TFunction("");
    AkUtil::TError(errStr);
    control::MessageDialog box(tr("Error"), tr("Export Project Error").append(errStr), MessageDialog::OK, nullptr);
    box.exec();
}

#include "vcg/math/matrix44.h"
void ProjectLoadSave::mergeCMeshO(const std::vector<CMeshO>& cmeshoList, CMeshO& cm)
{
    if (cmeshoList.empty())
        return;
    cm.Clear();
    for (auto it1 = cmeshoList.begin(); it1 != cmeshoList.end(); it1++)
    {
        Matrix44m norTran = vcg::Inverse((*it1).Tr).transpose();
        for (auto it2 = (*it1).edge.begin(); it2 != (*it1).edge.end(); it2++)
        {
            cm.edge.push_back(*it2);
        }

        for (auto it2 = (*it1).vert.begin(); it2 != (*it1).vert.end(); it2++)
        {
            auto tmpVert = *it2;

            tmpVert.P().Import((*it1).Tr * tmpVert.P());
            tmpVert.N().Import(norTran * (*it2).N());
            cm.vert.push_back(tmpVert);
        }

        for (auto it2 = (*it1).face.begin(); it2 != (*it1).face.end(); it2++)
        {
            cm.face.push_back(*it2);
        }

        for (auto it2 = (*it1).tetra.begin(); it2 != (*it1).tetra.end(); it2++)
        {
            cm.tetra.push_back(*it2);
        }

        for (auto it2 = (*it1).textures.begin(); it2 != (*it1).textures.end(); it2++)
        {
            cm.textures.push_back(*it2);
        }
        for (auto it2 = (*it1).normalmaps.begin(); it2 != (*it1).normalmaps.end(); it2++)
        {
            cm.normalmaps.push_back(*it2);
        }

        cm.vn += (*it1).vn;
        cm.en += (*it1).en;
        cm.fn += (*it1).fn;
        cm.hn += (*it1).hn;
        cm.tn += (*it1).tn;
        cm.attrn += (*it1).attrn;
    }

}

void ProjectLoadSave::mergeModel(const std::vector<CH3DPrintModelPtr>& modelPtrList, CMeshO& cm)
{
    if (modelPtrList.empty())
        return;

    cm.Clear();
    for (auto it1 = modelPtrList.begin(); it1 != modelPtrList.end(); it1++)
    {
        (*it1)->m_oldMesh->Tr = Matrix44m((*it1)->getTransform().transposed().data());
        Matrix44m norTran = vcg::Inverse((*it1)->m_oldMesh->Tr).transpose();
        vcg::Point3f p;
        /*for (auto it2 = (*it1)->m_oldMesh->edge.begin(); it2 != (*it1)->m_oldMesh->edge.end(); it2++)
        {
            cm.edge.push_back(*it2);
        } */
        //std::vector<vcg::Point3f> points;
        /*for (auto it2 = (*it1)->m_oldMesh->vert.begin(); it2 != (*it1)->m_oldMesh->vert.end(); it2++)
        {
            auto tmpVert = *it2;
            tmpVert.P().Import((*it1)->m_oldMesh->Tr * tmpVert.P());
            tmpVert.N().Import(norTran * vcg::Normalize((*it2).N()));
            p.SetZero();
            p.Import((*it1)->m_oldMesh->Tr * (*it2).P());
            //vcg::tri::Allocator<CMeshO>::AddVertex(cm, p);
            vcg::tri::Allocator<CMeshO>::AddVertices(cm, 1);
            cm.vert.back().ImportData(p);
            //(*it2).P() = p;
            //p.SetZero();
            //p.Import(norTran * (*it2).N());
            //(*it2).N() = p;
            //cm.vert.push_back(*it2);
        }  */
        //vcg::tri::Allocator<CMeshO>::AddVertices(cm, (*it1)->m_oldMesh->face.size());
        for (auto it2 = (*it1)->m_oldMesh->face.begin(); it2 != (*it1)->m_oldMesh->face.end(); it2++)
        {
            if (!(*it2).IsD())
            {
                vcg::Point3f nor = vcg::TriangleNormal(*it2);

                //vcg::Point3f nord = nor.Normalize();
                p.Import(norTran * nor.Normalize());

                //(*it2).N() = p;
                vcg::Point3f points[3];
                for (int i = 0; i < 3; i++)
                {
                    p.SetZero();
                    p.Import((*it1)->m_oldMesh->Tr * ((*it2).V(i)->P()));
                    points[i] = p;
                }

                vcg::tri::Allocator<CMeshO>::AddVertices(cm, 1);
                cm.vert.back().P() = vcg::Point3f((*it2).cP(0) * points[0], (*it2).cP(1) * points[1], (*it2).cP(2) * points[2]);
                //cm.vert.back().N() = vcg::Point3f((*it2).cV(0)->N() * points[0], (*it2).cV(1)->N() * points[1], (*it2).cV(2)->N() * points[2]);

                //cm.vert[count].P() = vcg::Point3f((*it2).cP(0) * points[0], (*it2).cP(1) * points[1], (*it2).cP(2) * points[2]);
                //cm.vert[count].N() = vcg::Point3f((*it2).cV(0)->N() * points[0], (*it2).cV(1)->N() * points[1], (*it2).cV(2)->N() * points[2]);

                //count++;
                vcg::tri::Allocator<CMeshO>::AddFace(cm, points[0], points[1], points[2]);
            }

            //cm.face.push_back(*it2);
        }

        /*for (auto it2 = (*it1)->m_oldMesh->tetra.begin(); it2 != (*it1)->m_oldMesh->tetra.end(); it2++)
        {
            cm.tetra.push_back(*it2);
        }

        for (auto it2 = (*it1)->m_oldMesh->textures.begin(); it2 != (*it1)->m_oldMesh->textures.end(); it2++)
        {
            cm.textures.push_back(*it2);
        }

        for (auto it2 = (*it1)->m_oldMesh->normalmaps.begin(); it2 != (*it1)->m_oldMesh->normalmaps.end(); it2++)
        {
            cm.normalmaps.push_back(*it2);
        } */

        //cm.vn += (*it1)->m_oldMesh->vn;
        //cm.en += (*it1)->m_oldMesh->en;
        //cm.fn += (*it1)->m_oldMesh->fn;
        //cm.hn += (*it1)->m_oldMesh->hn;
        //cm.tn += (*it1)->m_oldMesh->tn;
        //cm.attrn += (*it1)->m_oldMesh->attrn;
        vcg::tri::UpdateNormal<CMeshO>::PerFace(cm);
    }


//    vcg::tri::UpdateNormal<CMeshO>::PerVertexFromCurrentFaceNormal(cm);
}


