/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/
#ifndef PROJECT_LOAD_SAVE_H
#define PROJECT_LOAD_SAVE_H


#include "../ml_document/mesh_document.h"
#include "../plugins/plugin_manager.h"
#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"
#include "file_property.h"
#include "xml_stream.h"
#include <QObject>
#include <QString>
#include "../GeoAndShow/CHScene.h"
#include "import_export_support.h"
#include "export_project_thread.h"
#include "import_project_thread.h"
#include "../common/controlInterface/progressdialog.h"
class ProjectLoadSave : public QObject
{
    Q_OBJECT
public:
    static ProjectLoadSave& getInstance();
    static bool _QCallBack(const int pos, const char* str);

    ProjectLoadSave();
    ~ProjectLoadSave();

    void setMainWidget(QWidget* parentWidget = nullptr);

    bool loadProject(const QString& proPath, PluginManager& plugins);
    bool saveProject(PluginManager& plugins, const QString& filePath);
    bool buildProject(const QString& proPath, PluginManager& plugins);

    bool loadZip(QString filePath, const QString& proName, const QString& outPath, PluginManager& plugins);
    bool saveZip(QString filePath, const QString& folderName, QString proName, QString& layer, const QString& outPath);
    bool QZipWriterEx(QZipWriter* writer, QString dirPath, QString layer);

    bool readAllXml(const QString& proPath);
    bool readXml(const QString& dirPath, const QString& xmlName, const XmlStream::XmlStreamType& xmlType);
    bool writeAllXml(const QString& proPath);
    bool writeXml(const QString& dirPath, const QString& xmlName, const XmlStream::XmlStreamType& xmlType);

    bool deleteDir(const QString& proDir);
    bool saveModelFolder(const QString& dirPath, const QString& folderName);
    bool exportModel(const QString& dirPath, const QString& modelName, const CMeshO& cm);
    void setDocumentPropertyFromMeshDocument(const QString& proDir);
    void setMeshDocumentFromDocumentProperty();
    void setAppPropertyFromMain(const AppParamsProperty& params);  
    void setViewPropertyFromViewCK(const ViewProperty& view);
    void setFileProperty(FileProperty& _property);

    bool exportDocumentAllMesh(const QString& proDir, CHDocPtr doc_ptr);

    void mergeCMeshO(const std::vector<CMeshO>& cmeshoList, CMeshO& cm);
    void mergeModel(const std::vector<CH3DPrintModelPtr>& modelPtrList, CMeshO& cm);

    FileProperty& getFileProperty();
    FileProperty getFileProperty() const;

private slots:
    void exportProjectProgress(int num, int totalNum, const QString& str);
    void exportProjectProgressFinished();
    void importProjectProgress(int num, int totalNum, const QString& str);
    void importProjectProgressFinished();

    void importProjectError(int type, const QString &errStr);
    void exportProjectError(int type, const QString &errStr);

private:
    FileProperty m_file_property;
    static ProjectLoadSave _instance;

    ProgressDialog* m_exportProjectProgress;
    QThread* m_exportProjectThread;
    ProgressDialog* m_importProjectProgress;
    QThread* m_importProjectThread;

    QWidget* mainWidget;
};


#endif // !PROJECT_LOAD_SAVE_H
