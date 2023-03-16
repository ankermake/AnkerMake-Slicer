#include "common/GeoAndShow/CHScene.h"
#include "NewRender/CHChildDocWindow.h"
#include "ankermainwindow.h"
#include <common/mlapplication.h>
#include <common/filterscript.h>
#include <common/mlexception.h>
#include <common/globals.h>
//#include <common/utilities/load_save.h>
#include <wrap/io_trimesh/alnParser.h>
#include <QLocalSocket>
#include "rich_parameter_gui/richparameterlistdialog.h"
//#include "dialogs/about_dialog.h"
//#include "dialogs/filter_script_dialog.h"
//#include "dialogs/options_dialog.h"
//#include "dialogs/plugin_info_dialog.h"
//#include "dialogs/save_mesh_attributes_dialog.h"
//#include "dialogs/save_snapshot_dialog.h"
#include <common/file_property/project_load_save.h>
#include <common/file_property/mesh_model_import_export.h>
#include "common/GeoAndShow/CHCircleShowObj.h"
#include "common/file_property/import_model_thread.h"
#include "common/file_property/export_model_thread.h"
#include "common/file_property/read_mesh.h"
#include "qframelesshelper.h"
#ifdef _WIN32
#include <windows.h>
#include <Dbt.h>
#elif __APPLE__
#endif

ImportModelThread* importModelThread = nullptr;
ExportModelThread* exportModelThread = nullptr;
bool modelImporting = false;
bool modelExporting = false;
//ImportModelWoker *importModelWorker = nullptr;
int lastPos = 0;

using namespace std;
using namespace vcg;
#include "common/utilities/tlogger.h"
using namespace AkUtil;

AnkerMainWindow::AnkerMainWindow() :
    m_controlManager(new ControlManager(this)),
    m_settings(new SettingManager),
    m_importProgress(nullptr),
    m_exportProgress(nullptr),
    m_acodePath(QString()),
    m_pCmd(nullptr)
{
#ifdef Q_OS_WIN
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    installEventFilter(this);
#endif
    init();

    
    mainWindowInitFinished();
}

void AnkerMainWindow::paintEvent(QPaintEvent *e)
{
    if (m_framelessHelper == 0)
        return;
    m_framelessHelper->paintEvent(e);
}

void AnkerMainWindow::mainWindowInitFinished()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_SEND2ALL;
    data.msg = AkConst::Msg::MAIN_WINDOW_INIT_FINISHED;
    
    messageProcessing->sendMsg2Manager(data);
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef Q_OS_WIN
bool AnkerMainWindow::winEvent(MSG *message, long *result)
{
    return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif


AnkerMainWindow::~AnkerMainWindow()
{
    qDebug() << "~AnkerMainWindow() start.";
    if(m_pCmd) {
        delete m_pCmd;
        m_pCmd = nullptr;
    }
    if(!m_acodePath.isEmpty()) {
        removeAcodeFile(m_acodePath);
    }
    qDebug() << "~AnkerMainWindow() end.";
}

void AnkerMainWindow::init()
{
    TFunction("lauch plugin ... ");
    initMainWindow();
    initRichParameterList();
    initLoadPlugin();
    initTitleBar();
    initFdmWidget();
    
    for (MeshLabPlugin* fp : PM.pluginIterator()) {
        fp->initialize(m_controlManager, &this->defaultGlobalParams);
    }

    initEditTool();

    QTimer::singleShot(10, [this]() {
        init_9_Delayed();

    });
}


void AnkerMainWindow::initMainWindow() {
    TFunction("");
    QSettings settings;
    setContextMenuPolicy(Qt::NoContextMenu);
    setWindowIcon(QIcon(":/logo_AnkerMake.png")); 
    setWindowTitle("AnkerMake");

    setMinimumSize(1200, 720);
    m_meshSuffixList << "obj" << "stl";
}


void AnkerMainWindow::initRichParameterList()
{
    TFunction("");


    //    QSettings settings;
    
    
    //       **/
    //        QString paramName = AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE;
    //        Point3m paramDefval = Point3m(225, 225, 250);
    //        RichPoint3f param = RichPoint3f(paramName, paramDefval);
    //        defaultGlobalParams.addParam(param);
    //    }

    static auto createMachineSize = [this]() {
        {
            const RichPoint3f& param = static_cast<const RichPoint3f&>(defaultGlobalParams.getParameterByName(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE));
            mainMachineBoxChanged(param.value());
            QObject::connect(param.qobj.data(), &RichParameterQObject::valueChange, this, &AnkerMainWindow::mainMachineBoxChanged);
        }

        {
            PrintMachineBoxParam printMachineBoxParam;
            const RichParameter& param = defaultGlobalParams.getParameterByName(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE);
            printMachineBoxParam.m_length = param.value().getPoint3f().X();
            printMachineBoxParam.m_width = param.value().getPoint3f().Y();
            printMachineBoxParam.m_height = param.value().getPoint3f().Z();
            printMachineBoxParam.m_lineWidth = 1.0f;
            // printMachineBoxParam.m_color = QColor(229, 229, 229);
            // printMachineBoxParam.m_color = QColor(102, 102, 102);
            printMachineBoxParam.m_color = QColor(80, 80, 80);

            QString paramName = AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM;
            SceneParam sceneDefval;
            sceneDefval.logoMesh = curScene->m_doc->m_machineBox->getLogoCMeshO();
            sceneDefval.m_backgroundColor = QColor(245, 246, 247, 0.0);
            sceneDefval.m_printMachineBox = printMachineBoxParam;

            defaultGlobalParams.addParam(RichSceneParam(paramName, sceneDefval));
            const RichParameter& sceneParam = defaultGlobalParams.getParameterByName(paramName);
            CHChildDocWindow* docWindow = static_cast<CHChildDocWindow*>(curScene);

            docWindow->createMachineBox(QString("/models/logo.stl"),
                                        sceneParam.value().getSceneParam().m_printMachineBox.m_length,
                                        sceneParam.value().getSceneParam().m_printMachineBox.m_width,
                                        sceneParam.value().getSceneParam().m_printMachineBox.m_height, QColor(80, 80, 80));
            getDoc()->modelCheckSceneInChanged();

            //            auto sceneParamChanged = [](const Value& value) {
            
            //                qDebug() << "sceneParamChanged.";

            //            };
            //            sceneParamChanged(sceneParam.value());
            //            emit getDoc()->modelCheckSceneInChanged();
            //            QObject::connect(sceneParam.qobj.data(), &RichParameterQObject::valueChange, sceneParamChanged);
        }
    };

    static std::function<void(void)> delay_createMachineSize;
    delay_createMachineSize = [&](){
        if(defaultGlobalParams.hasParameter(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE)){
            createMachineSize();
            qDebug() << "createMachineSize";
        }
        else{
           QTimer::singleShot(1,delay_createMachineSize);
        }
    };
    delay_createMachineSize();
}



void AnkerMainWindow::initLoadPlugin()
{
    TFunction("");
    try {
        PM.loadPlugins();
    }
    catch (const MLException& e) {
        MessageDialog messageBox(tr("Plugin Loading Error"), QString( e.what()), MessageDialog::OK);
        messageBox.exec();
    }

    QObject::connect(messageProcessing, &MessageProcessing::sendMsg2Manager, &PM, &PluginManager::recMsgfromPlugin);
    QObject::connect(&PM, &PluginManager::sendMsg2MessageProcessing, messageProcessing, &MessageProcessing::recMsgfromManager);

    //    for (MeshLabPlugin* fp : PM.pluginIterator()) {
    //        fp->initialize(m_controlManager, &this->defaultGlobalParams);
    //    }


}

void AnkerMainWindow::init_9_Delayed()
{

}

void AnkerMainWindow::initFdmWidget()
{
    TFunction("");
    qDebug() << "MainWindow Widget: " << this << "WindId: " << window()->winId();
    FdmMainWidget* mainwidget = new FdmMainWidget(messageProcessing, m_mainTitleBar, m_controlManager, this);
    mainwidget->setMainWindowId(window()->winId());
    connect(mainwidget, &FdmMainWidget::unloadPlugins, this, &AnkerMainWindow::unloadPluginsSlot);
    connect(mainwidget, &FdmMainWidget::closeMainWindow, this, &AnkerMainWindow::onButtonCloseClicked);
    
    connect(this, &AnkerMainWindow::pluginsUnloaded, mainwidget, &FdmMainWidget::pluginsUnloaded);

    connect(mainwidget, SIGNAL(fdmOpenFile()), this, SLOT(open()));
    connect(mainwidget, SIGNAL(fdmSaveAllMesh()), this, SLOT(saveAllMesh()));
    connect(mainwidget, SIGNAL(fdmSaveMesh()), this, SLOT(saveMesh()));
    connect(mainwidget, SIGNAL(fdmSaveProject()), this, SLOT(slotSaveProject()));
    connect(mainwidget, SIGNAL(fdmSaveAsProject()), this, SLOT(slotSaveAsProject()));
    connect(mainwidget, SIGNAL(fdmOpenRecent(QStringList)), this, SLOT(openFileList(QStringList)));
    connect(this, SIGNAL(openFileSucessful(QStringList)), mainwidget, SLOT(insertRecent(QStringList)));
    this->setCentralWidget(mainwidget);
}

void AnkerMainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        for (AnkerEditPlugin *iEditFactory : PM.getAnkerEditPlugins()) {
            iEditFactory->changeEvent(e);
        }
    }
}

void AnkerMainWindow::initEditTool()
{
    TFunction("");
    for (auto* editPlugin : PM.getAnkerEditPlugins()) {
        connect(getGlobalPick().get(), &CHPickOperationCommand::selectedObjsChanged, editPlugin, &AnkerEditPlugin::setEditToolsEnable);
    }
    for (AnkerEditPlugin* iEditFactory : PM.getAnkerEditPlugins())
    {
        for (QAction* editAction : iEditFactory->actions())
        {
            

            QObject::connect(editAction, &QAction::toggled, [this, iEditFactory, editAction]() {
                if (editAction->isChecked() == false) { return; }

                
                for (AnkerEditPlugin* another : PM.getAnkerEditPlugins()) {
                    if (iEditFactory != another)
                        another->setEditToolsEnable(0);
                }

                
                for (QAction* another : iEditFactory->actions()) {
                    if (editAction != another)
                        another->setChecked(false);
                }
            });

            
            QObject::connect(editAction, &QAction::toggled, [iEditFactory, editAction]() {
                ActionEditTool* actionEditTool = static_cast<ActionEditTool*>(editAction);
                if (actionEditTool->isChecked() == false) {
                    currActionEdit = nullptr;
                    iEditFactory->endAnkerEdit(actionEditTool);
                }

                if (actionEditTool->isChecked() == true) {
                    currActionEdit = actionEditTool;
                    iEditFactory->startAnkerEdit(actionEditTool, curScene);
                }
            });
            currActionEdit = currActionEdit ? currActionEdit : static_cast<ActionEditTool*>(editAction);

            
            connect(getGlobalPick().get(),
                    &CHPickOperationCommand::resetSeletedObjsSig,
                    [iEditFactory, editAction](){
                ActionEditTool* actionEditTool = static_cast<ActionEditTool*>(editAction);
                if(actionEditTool->isChecked())
                {
                    actionEditTool->setChecked(false);
                    currActionEdit = nullptr;
                    iEditFactory->endAnkerEdit(actionEditTool);
                }
            });
        }
    }
}

QString AnkerMainWindow::openFilter(QString projectSuffix, QStringList meshSuffixList, QString gcodeSuffix, QString acodeSuffix)
{
    TFunction(QString("projectSuffix %1, gcodeSuffix %2").arg(projectSuffix).arg(gcodeSuffix));
    //    QString filter = PM.inputProjectFormatListDialog().join(";;").append(PM.inputMeshFormatListDialog().join(";;"));
    //    qDebug() << " filter = " << filter;
    QString meshStr;
    QString meshFilter;
    for (auto i : meshSuffixList) {
        meshStr += (" *." + i);
        if (!meshFilter.isEmpty()) {
            meshFilter += ";;";
        }
        meshFilter += i.toUpper() + " File (" + " *." + i + ")";
    }
    QString allFilter = QString("All Support Types (") + " *." + projectSuffix + meshStr + " *." + gcodeSuffix +  " *." + acodeSuffix + ")";
    QString projectFilter = QString("Akpro (") + " *." + projectSuffix + ")";
    QString gcodeFilter = QString("GCode (") + " *." + gcodeSuffix + ")";
    QString acodeFilter = QString("acode (") + " *." + acodeSuffix + ")";
    QString pathFilter = allFilter + ";;" + projectFilter + ";;" + gcodeFilter + ";;" +acodeFilter + ";;"+ meshFilter;
     qDebug() << " path =====" << pathFilter;
    return pathFilter;
}


bool AnkerMainWindow::pathIsValid(QStringList pathList, QStringList suffixList)
{
    if (pathList.isEmpty() || suffixList.isEmpty()) {
        return false;
    }
    QFileInfo file(pathList.first());
    QString firstSuffix = file.suffix();
    if(pathList.size() >1 && m_projectSuffix.compare(firstSuffix, Qt::CaseInsensitive) == 0) {
        return false;
    }
    int proCount = 0; 
    for (const auto &path : pathList) {
        QFileInfo info(path);
        if (!suffixList.contains(info.suffix().toLower())) {
            return false;
        }
        if (firstSuffix.compare(info.suffix(), Qt::CaseInsensitive) != 0) {
            return false;
        }
        if(info.suffix().toLower() == "akpro")
        {
            proCount++;
        }
    }
    if(proCount > 1)
    {
        return false;
    }
    return  true;

}

int AnkerMainWindow::checkFileSuffix(const QStringList &pathList, const QStringList &suffixList)
{
    if (pathList.isEmpty() || suffixList.isEmpty()) {
        return 0;
    }
    
    QFileInfo file((*pathList.begin()));

    QString firstSuffix = file.suffix();
    for (const auto &path : pathList) {
        QFileInfo info(path);
        if(firstSuffix.compare(info.suffix(),Qt::CaseInsensitive) != 0) {
            return -3;
        };
    }

    
    if(!suffixList.contains(firstSuffix,Qt::CaseInsensitive)) {
        return -2;
    };


    
    
    if((firstSuffix.compare(m_projectSuffix,Qt::CaseInsensitive) == 0) && pathList.size() > 1) {
        qDebug() << "firstSuffix =   " << firstSuffix  ;
        return -1;
    }
    
    
    
    return 1;
}

bool AnkerMainWindow::isProject(QStringList pathList, QString suffix)
{
    //    if(pathList.isEmpty() || suffix
    return true;
}

void AnkerMainWindow::openFileFromAppRaram(const QString &fileName)
{
    QStringList fileNameList;
    fileNameList.append(fileName);
    openFileList(fileNameList);
}

void AnkerMainWindow::openFileList(QStringList fileNameList)
{
    if(fileNameList.isEmpty()) {
        return;
    }
    QStringList suffixList;
    QStringList tmpFileNameList = fileNameList;
    fileNameList.clear();
    for (int i = 0; i < tmpFileNameList.size(); i++)
    {
        int index = tmpFileNameList[i].lastIndexOf(".") + 1;
        QString tmppath = tmpFileNameList[i].mid(0, index);
        QString tmpSuffix = tmpFileNameList[i].mid(index, tmpFileNameList[i].length() - index).toLower();
        fileNameList.push_back(tmppath + tmpSuffix);
    }
    suffixList << m_projectSuffix << m_meshSuffixList << m_gcodeSuffix  << m_acodeSuffix;
    int resultInt = checkFileSuffix(fileNameList, suffixList);
    if(resultInt  < 0 ) {
        QString errorString;
        if(resultInt == -3) {
            errorString = QString(tr("Tip: Open files one at a time."));
        } else if(resultInt == -2) {
            errorString = QString(tr("The file is damaged. Try again."));
        } else if(resultInt == -1) {
            errorString = QString(tr("Only a single project file can be opened at a time."));
        }

        control::MessageDialog box(tr("Notice"), errorString, MessageDialog::OK, this);
        box.exec();
        return;
    }


    
    if (fileNameList.size() == 1) {
        QFileInfo info(*fileNameList.begin());
        QString suffix = info.suffix();
        if (suffix == m_projectSuffix) {

            if ((!m_projectPath.isEmpty() || !m_meshPath.isEmpty())) {
                control::MessageDialog box(tr("Notice"), tr("This will overwrite the file. Do you want to continue?"), MessageDialog::CANCEL | MessageDialog::SAVE, nullptr);
                if (box.exec() == MessageDialog::SAVE) {
                    if (!m_projectPath.isEmpty()) {
                        ProjectLoadSave::getInstance().saveProject(PM, m_projectPath);
                    }
                    else {
                        int index1 = m_meshPath[0].lastIndexOf("/") + 1;
                        int index2 =m_meshPath[0].lastIndexOf(".");
                        QString tmpMeshName = m_meshPath[0].mid(index1, index2 - index1);
                        m_projectPath = QFileDialog::getSaveFileName(NULL, tr("Save File"), QApplication::applicationDirPath() + QString("/Document/") + tmpMeshName, QString("Project File(*.akpro)"));

                        ProjectLoadSave::getInstance().saveProject(PM, m_projectPath);
                    }
                }

                if (!m_projectPath.isEmpty()) {
                    m_projectPath = QString();
                }
                else {
                    m_meshPath = QStringList();
                }
                getDoc()->clearDocMeshModels();
            }
            ProjectLoadSave::getInstance().setMainWidget(this);
            bool ok = ProjectLoadSave::getInstance().loadProject(*fileNameList.begin(), PM);
            if (ok) {
                m_projectPath = *fileNameList.begin();
                emit getDoc()->modelObjsStatusChanged(AddMesh);
                emit openFileSucessful(QStringList() << m_projectPath);
                // insertRecent(QStringList() << m_projectPath);
                if (curScene && curScene->m_pickCommand)
                {
                    emit curScene->m_pickCommand->selectedObjsChanged(QVariant::fromValue(curScene->m_pickCommand->m_selectedObjs.size()));
                }
            }
            QWidget *widget =  this->centralWidget();
            FdmMainWidget * fdm = static_cast<FdmMainWidget*>(widget);
            if(fdm) {
              fdm->currentPageChanged(FDMWIDGET);
            }
            return;
        }
    }
    QStringList openSuccessFileList;
    for (auto file : fileNameList) {
        QFileInfo info(file);
        QString suffix = info.suffix();
        if (suffix == m_gcodeSuffix) {
            openGcodePreview(file);
            openSuccessFileList.append(file);
//            QWidget *widget =  this->centralWidget();
//            FdmMainWidget * fdm = static_cast<FdmMainWidget*>(widget);
//            if(fdm) {
//              fdm->currentPageChanged(GCODEWIDGET);
//            }
        }
        else if(suffix == m_acodeSuffix) {
            openAcodePreview(file);
            openSuccessFileList.append(file);
//            QWidget *widget =  this->centralWidget();
//            FdmMainWidget * fdm = static_cast<FdmMainWidget*>(widget);
//            if(fdm) {
//              fdm->currentPageChanged(GCODEWIDGET);
//            }
        }
        else if (m_meshSuffixList.contains(suffix.toLower())) {
            if (importMeshWithLayerManagement_ak(file)) {
                m_meshPath.append(file);
                openSuccessFileList.append(file);
            }
            QWidget *widget =  this->centralWidget();
            FdmMainWidget * fdm = static_cast<FdmMainWidget*>(widget);
            if(fdm) {
              fdm->currentPageChanged(FDMWIDGET);
            }
        }
    }

    emit openFileSucessful(openSuccessFileList);

    // insertRecent(openSuccessFileList);
}

void AnkerMainWindow::importModelProgress(int pos, const QString& str)
{
    if (m_importProgress == nullptr)
    {
        return;
    }

    m_importProgress->setValue(pos);
    m_importProgress->setText(str);
}

void AnkerMainWindow::importModelProgressFinished()
{
    if (m_importProgress != nullptr)
    {
        m_importProgress->close();
        m_importProgress->deleteLater();
        m_importProgress = nullptr;
        modelImporting = false;
        if(importModelThread->isRunning())
        {
            importModelThread->quit();
            importModelThread->wait();
            importModelThread->deleteLater();
            importModelThread = nullptr;
        }
    }
}

void AnkerMainWindow::exportModelProgress(int pos, const QString& str)
{
    if (m_exportProgress == nullptr)
    {
        return;
    }
    m_exportProgress->setValue(pos);
    m_exportProgress->setText(str);
}

void AnkerMainWindow::exportModelProgressFinished()
{
    if (m_exportProgress == nullptr)
    {
        return;
    }
    m_exportProgress->close();
    delete m_exportProgress;
    m_exportProgress = nullptr;
    modelExporting = false;

    if(exportModelThread->isRunning())
    {
        exportModelThread->quit();
        exportModelThread->wait();
        delete exportModelThread;
        exportModelThread = nullptr;
    }
}

void AnkerMainWindow::loginInitFeedbackSlot(qint64 id, int success)
{
    qDebug() << "loginInitFeedbackSlot: " << "id:" << id << ", success: " << success;
}

void AnkerMainWindow::openFileError(int type, QString errorString)
{
    // m_openFileSuccess = false;
    AkUtil::TError(errorString);
    control::MessageDialog box(tr("Error"), tr("The file is damaged. Try again.").append(errorString), MessageDialog::OK, nullptr);
    box.exec();
}

void AnkerMainWindow::exportModelError(int type, const QString &err)
{
    AkUtil::TFunction("");
    AkUtil::TError(err);
    control::MessageDialog box(tr("Error"), tr("Export File Error").append(err), MessageDialog::OK, nullptr);
    box.exec();
}

void AnkerMainWindow::mainMachineBoxChanged(const Value &value)
{
    Point3m size = value.getPoint3f();
    CHChildDocWindow* docWindow = static_cast<CHChildDocWindow*>(curScene);
    //  docWindow->createMachineBox(QString("/models/logo.stl"), size.X(), size.Y(), size.Z(), QColor(229, 229, 229));
    // docWindow->createMachineBox(QString("/models/logo.stl"), size.X(), size.Y(), size.Z(), QColor(102, 102, 102));
    if(docWindow->m_doc->m_machineBox == nullptr)
    {
        docWindow->createMachineBox(QString("/models/logo.stl"), size.X(), size.Y(), size.Z(), QColor(80, 80, 80));
    }
    else
    {
        docWindow->updateMechineBox(size.X(), size.Y(), size.Z());
        qDebug() << "update Mechine: " << size.X() << size.Y() << size.Z();
    }

    emit docWindow->machinePlatformSizeSignal(size.X(), size.Y(), size.Z());
    qDebug() << "mainMachineBoxChanged111: " << size.X() << size.Y() << size.Z() << AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE << "&RichParameterQObject::valueChange";
    getDoc()->modelCheckSceneInChanged();
    if(defaultGlobalParams.hasParameter(AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM))
    {
        RichParameter& param = defaultGlobalParams.getParameterByName(AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM);
        SceneParam sceneDefval = param.value().getSceneParam();
        sceneDefval.m_printMachineBox.m_length = size.X();
        sceneDefval.m_printMachineBox.m_width = size.Y();
        sceneDefval.m_printMachineBox.m_height = size.Z();
        SceneValue sceneValue(sceneDefval);
        param.setValue(sceneValue);
    }
}

bool AnkerMainWindow::removeAcodeFile(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); 
    QFileInfoList fileList = dir.entryInfoList(); 
    foreach (QFileInfo file, fileList){ 
        if (file.isFile()){ 
            if(file.fileName() == ".")   
            {

                continue;
            }
            file.dir().remove(file.fileName());
            //                    file.dir().removeRecursively()
        }else{ 
            removeAcodeFile(file.absoluteFilePath());
        }
    }
    
    QString strDirName = dir.absolutePath();
    if (strDirName != m_acodePath)
    {
        return dir.rmdir(dir.absolutePath());
    }

    return true;

}

void AnkerMainWindow::openGcodePreview(const QString& fileName)
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_GCODE_PARSER;
    data.msg = AkConst::Msg::PREVIEW_GCODE;
    data.map.insert(AkConst::Param::GCODE_FILE, fileName);
    QFileInfo fileInfo(fileName);
    QString ostNameT = fileInfo.fileName().replace(".gcode","");
    data.map.insert(AkConst::Param::ORIGINAL_STL_NAME,ostNameT);

    
    messageProcessing->sendMsg2Manager(data);

    
    PluginMessageData data1;
    data1.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data1.dest = AkConst::Plugin::FDM_SETTING;
    data1.msg = AkConst::Msg::PREVIEW_GCODE_BY_DRAW;
    data1.map.insert(AkConst::Param::GCODE_FILE, fileName);

    
    messageProcessing->sendMsg2Manager(data1);
}

void AnkerMainWindow::openAcodePreview(const QString &fileName)
{
    QFileInfo fileInfo = QFileInfo(fileName);
    if(m_acodePath.isEmpty()) {
        auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        // QString dirPath = writableLocation + "/AICamera";
        m_acodePath = writableLocation + "/ACode";
    }
    QDir dir(m_acodePath);
    if(!dir.exists()){
        bool ismkdir = dir.mkdir(m_acodePath);
        if(!ismkdir) {
            AkUtil::TDebug("Create path fail : " + m_acodePath);
            qDebug() << "Create path fail" +  m_acodePath << endl;
            return;
        }
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy_MM_dd_hh_mm_ss_zzz");
    QString  acodePath = m_acodePath + "/" + fileInfo.baseName() + current_date;
    QDir saveFileName(acodePath);
    bool ismkdir = saveFileName.mkdir(acodePath);

    if(!m_pCmd) {
        m_pCmd = new QProcess();
        connect(m_pCmd,&QProcess::readyReadStandardError,this,[=]()
        {
            
            QString data=m_pCmd->readAllStandardError();
            AkUtil::TDebug("return  cmd : "+data);
        });
    }
    QString cmd;
    m_pCmd->setWorkingDirectory(fileInfo.absoluteDir().path());
    cmd = "tar";
    QStringList args;
    args.append("-zxvf");
    //args.append(fileInfoSave);
    args.append(fileInfo.fileName());
    args.append("-C");
    args.append(acodePath);
    qDebug() << "tar fileName = " << args;
    m_pCmd->start(cmd,args);
    m_pCmd->waitForStarted();
    AkUtil::TDebug("tar error : "+m_pCmd->errorString());

    m_pCmd->waitForFinished();

    QDir saveSucessFile(acodePath);
    QFileInfoList fileList =  saveSucessFile.entryInfoList();
    qDebug() << " file list " << fileList;
    QString gcodeFile;
    QString akpicFile;
    for(auto file : fileList) {
        if(file.suffix().compare(m_gcodeSuffix,Qt::CaseInsensitive) == 0) {
            gcodeFile = file.absoluteFilePath();
        }
        else if(file.suffix().compare("akpic",Qt::CaseInsensitive) == 0){
            akpicFile = file.absoluteFilePath();
        }
    }
    if(gcodeFile.isEmpty()) {
        AkUtil::TDebug("Gcode File or Akpic File is inexistence ");
        return;
    }
    AkUtil::TDebug( QString(" gcode file = ").append( gcodeFile).append( " akpic file = ").append(akpicFile));
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_GCODE_PARSER;
    data.msg = AkConst::Msg::PREVIEW_GCODE;
    data.map.insert(AkConst::Param::GCODE_FILE, gcodeFile);
    data.map.insert(AkConst::Param::AKPIC_FILE, akpicFile);
    data.map.insert(AkConst::Param::ORIGINAL_STL_NAME,fileInfo.fileName());

    
    messageProcessing->sendMsg2Manager(data);

    
    PluginMessageData data1;
    data1.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data1.dest = AkConst::Plugin::FDM_SETTING;
    data1.msg = AkConst::Msg::PREVIEW_GCODE_BY_DRAW;
    data1.map.insert(AkConst::Param::GCODE_FILE, gcodeFile);

    
    messageProcessing->sendMsg2Manager(data1);
}


void AnkerMainWindow::slotSaveProject()
{
    const FileProperty::ProjectLoadProperty proProperty = ProjectLoadSave::getInstance().getFileProperty().m_load_project_property;
    QString path = "";
    if (proProperty.m_loadProjected && !proProperty.m_lastProjectFile.isEmpty())
    {
        path = proProperty.m_lastProjectFile;
    }
    else
    {
        QString fileName = "";
        if (getDoc()->m_printObjs.size() > 0)
        {
            fileName = getDoc()->m_printObjs[0]->m_filePath;
            int index1 = fileName.lastIndexOf("/");
            int index2 = fileName.lastIndexOf(".");
            fileName = fileName.mid(index1, index2 - index1);
        }
        QFileDialog fileDialog;
        path = QFileDialog::getSaveFileName(NULL, tr("Save File"), QApplication::applicationDirPath() + QString("/Document/") + fileName, QString("Project File(*.akpro)"));
        if(!path.isEmpty() && !path.contains(".akpro"))
        {
            path += ".akpro";
        }
    }

    ProjectLoadSave::getInstance().setMainWidget(this);
    bool ret = ProjectLoadSave::getInstance().saveProject(PM, path);
    if (ret)
    {
        ProjectLoadSave::getInstance().getFileProperty().m_load_project_property.m_loadProjected = true;
        ProjectLoadSave::getInstance().getFileProperty().m_load_project_property.m_lastProjectFile = path;
        //QMessageBox::warning(NULL, tr("Save"), tr("Save Project Successful!"));
    }
    else
    {
        //QMessageBox::warning(NULL, tr("Save"), tr("Save Project Failed!"));

    }
}

void AnkerMainWindow::saveMesh()
{
    auto modelsPtr = getDoc()->m_printObjs;
    auto bIt = modelsPtr.begin();
    auto eIt = modelsPtr.end();

    std::vector<CH3DPrintModelPtr> modelPtrList;
    while (bIt != eIt)
    {
        if ((*bIt)->getStatus() == ObjStatus::selected && (*bIt)->getVisuable())
        {
            modelPtrList.push_back(*bIt);
        }
        bIt++;
    }
    if(modelPtrList.size() == 0)
    {
        MessageDialog messageBox(tr("Warning"), tr("Model Not Detected"), MessageDialog::OK);
        messageBox.exec();
        return ;
    }
    CMeshO cm;
    ProjectLoadSave::getInstance().mergeModel(modelPtrList, cm);
    exportMesh_ak((*modelsPtr.begin())->getObjectName(), cm);
    //saveAs_ak();
}

void AnkerMainWindow::saveAllMesh()
{
    auto modelsPtr = getDoc()->m_printObjs;
    auto bIt = modelsPtr.begin();
    auto eIt = modelsPtr.end();

    std::vector<CH3DPrintModelPtr> modelPtrList;
    while (bIt != eIt)
    {
        if ((*bIt)->getVisuable())
        {
            modelPtrList.push_back(*bIt);
        }
        bIt++;
    }
    if(modelPtrList.size() == 0)
    {
        MessageDialog messageBox(tr("Warning"), tr("Model Not Detected"), MessageDialog::OK);
        messageBox.exec();
        return ;
    }
    CMeshO cm;
    ProjectLoadSave::getInstance().mergeModel(modelPtrList, cm);
    exportMesh_ak((*modelsPtr.begin())->getObjectName(), cm);
}

void AnkerMainWindow::slotSaveAsProject()
{
    QString fileName = "";
    if (getDoc()->m_printObjs.size() > 0)
    {
        fileName = getDoc()->m_printObjs[0]->m_filePath;
        int index1 = fileName.lastIndexOf("/");
        int index2 = fileName.lastIndexOf(".");
        fileName = fileName.mid(index1, index2 - index1);
    }
    QString path = QFileDialog::getSaveFileName(NULL, tr("Save File"), QApplication::applicationDirPath() + QString("/Document/") + fileName, QString("Project File(*.akpro)"));
    if(path.isEmpty())
    {
        return;
    }
    int lastIndexDot = path.lastIndexOf(".");
    QString format = path.mid(lastIndexDot, path.length() - lastIndexDot);
    if(!format.contains("akpro"))
    {
        path += ".akpro";
    }

    bool ret = ProjectLoadSave::getInstance().saveProject(PM, path);

    if (ret)
    {
        ProjectLoadSave::getInstance().getFileProperty().m_load_project_property.m_loadProjected = true;
        ProjectLoadSave::getInstance().getFileProperty().m_load_project_property.m_lastProjectFile = path;
        //QMessageBox::warning(NULL, tr("Save As"), tr("Save As Project Successful!"));
    }
    else
    {
        //QMessageBox::warning(NULL, tr("Save As"), tr("Save As Failed!"));
        control::MessageDialog messageBox(tr("Warning"), tr("An unknown error has occurred, please try again"), MessageDialog::OK);
        messageBox.exec();
    }
}


void AnkerMainWindow::slotQuit()
{

}

void AnkerMainWindow::open()
{
    QString  filter = openFilter(m_projectSuffix, m_meshSuffixList, m_gcodeSuffix, m_acodeSuffix);
    // qDebug() << " str == " << filter;
    QStringList recentList = m_settings->readRecent();
    QString lastPath = QApplication::applicationDirPath() + "//document";
    if (!recentList.isEmpty()) {
        lastPath = recentList.first();
    }
    QDir dir(lastPath);
    dir.cdUp();
    //QFileInfo info(lastPath);
   // info.absoluteFilePath();
    QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("Open"), /*info.absolutePath()*/dir.path(), filter/*tr("files (*akpro; *obj; *stl)")*/);
    //qDebug() << "open fileNameList: " << fileNameList;
    if (!fileNameList.isEmpty()) {
        openFileList(fileNameList);
    }
}

//bool AnkerMainWindow::save_ak(const bool saveAllPossibleAttributes)
//{
//    return exportMesh_ak(meshDoc()->mm()->fullName(), meshDoc()->mm(), saveAllPossibleAttributes);
//}

//bool AnkerMainWindow::saveAs_ak(QString fileName, const bool saveAllPossibleAttributes)
//{
//    return exportMesh_ak(fileName, meshDoc()->mm(), saveAllPossibleAttributes);
//}


//void AnkerMainWindow::resizeEvent(QResizeEvent *event)
//{
//    emit resizeChildWidget();
//    QMainWindow::resizeEvent(event);
//}

//void AnkerMainWindow::changeEvent(QEvent *event)
//{
//    if(event->type() == QEvent::WindowStateChange)
//    {
//        QWindowStateChangeEvent *stateEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
//        if(stateEvent) {
//            Qt::WindowStates state = stateEvent->oldState();
//            if(state == Qt::WindowMaximized) {
//                QDesktopWidget *deskTop = QApplication::desktop();
//                move(deskTop->width() - this->width()/2,deskTop->height() - this->height()/2);
//                qDebug() << " changeEvent state == WindowMaximized" << " width =" << deskTop->width() - this->width()/2  <<"height ==" << deskTop->height() - this->height()/2   <<" desktop size =" << deskTop->size();
//            }
//        }
//    }
//    QMainWindow::changeEvent(event);
//}

//void AnkerMainWindow::insertRecentProject(const QString &file)
//{
//        m_settings->insertRecentProject(file);
//        m_recentProjectMenu->clear();
//        createRecentProject(m_settings->recentProject(),m_recentProjectMenu,true);
//        m_recentProjectMenu->setEnabled(true);
//}

//void AnkerMainWindow::insertRecentFile(const QStringList &fileList)
//{
//    if(fileList.isEmpty()) {
//        return;
//    }
//    for(const auto &file : fileList) {
//        m_settings->insertRecentFile(file);
//    }
//    m_recentFileMenu->clear();
//    createRecentProject(m_settings->recentFile(),m_recentFileMenu,false);
//    m_recentFileMenu->setEnabled(true);
//}

bool AnkerMainWindow::importMeshWithLayerManagement_ak(QString fileName)
{
    importMesh(fileName);
    //importMesh_ak(fileName);
    //setCurrentMeshBestTab();
    return true;
}

bool AnkerMainWindow::QCallBack_ak(const int pos, const char* str)
{
    if (importModelThread != nullptr && modelImporting)
    {
        if (lastPos == pos) 
        {
            return true;
        }
        emit importModelThread->openData(pos, tr(""));

        lastPos = pos;
    }

    if (exportModelThread != nullptr && modelExporting)
    {
        if (lastPos == pos)
        {
            return true;
        }
        emit exportModelThread->exportData(pos, QString(""));
        lastPos = pos;
    }

    return true;
}

void AnkerMainWindow::importMesh(QString fileName)
{
    QStringList fileNameList;
    if (fileName.isEmpty())
    {
        fileNameList = QFileDialog::getOpenFileNames(
                    this, tr("Import Mesh"),
                    lastUsedDirectory.path(),
                    PM.inputMeshFormatListDialog().join(";;"));
    }
    else
    {
        fileNameList.push_back(fileName);
    }
    if (fileNameList.isEmpty())
    {
        return;
    }
    else
    {
        QString path = fileNameList.first();
        path.truncate(path.lastIndexOf("/"));
        lastUsedDirectory.setPath(path);
    }

    QElapsedTimer allFileTime;
    allFileTime.start();
    for (const QString& fileName : fileNameList)
    {
        QFileInfo fi(fileName);
        if (!fi.exists())
        {
            QString errorMsgFormat = "Unable to open file:\n\"%1\"\n\nError details: file %1 does not exist.";
            MessageDialog messageBox(tr("Error While Opening"), errorMsgFormat.arg(fileName), MessageDialog::OK);
            messageBox.exec();
            return;
        }
        if (!fi.isReadable())
        {
            QString errorMsgFormat = "Unable to open file:\n\"%1\"\n\nError details: file %1 is not readable.";
            MessageDialog messageBox(tr("Error While Opening"), errorMsgFormat.arg(fileName), MessageDialog::OK);
            messageBox.exec();
            return;
        }
        bool ret = ReadMesh::checkMeshHeader(fileName);
        if(!ret)
        {
            MessageDialog messageBox(tr("Warning"), tr("Model File Error"), MessageDialog::OK);
            messageBox.exec();
            break;
        }

        QString extension = fi.suffix();

        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));



        // m_openFileSuccess = true;
        int mask;
        CH3DPrintModelPtr tmpPrintModelPtr;
        try
        {
            tmpPrintModelPtr = CH3DPrintModelPtr(new CH3DPrintModel);
            tmpPrintModelPtr->m_oldMesh = new CMeshO;

            importModelThread = new ImportModelThread();
            m_importProgress = new ProgressDialog();
            m_importProgress->setModal(true);
            m_importProgress->setAutoClosed(false);
            m_importProgress->setCancelVisible(false);
            modelImporting = true;
            //m_importProgress->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
            importModelThread->initOpenParam(extension, fileName, tmpPrintModelPtr->m_oldMesh, QCallBack_ak);
            connect(importModelThread, &ImportModelThread::finished, this, &AnkerMainWindow::importModelProgressFinished);
            //connect(importModelThread, &ImportModelThread::finished, importModelThread, &QObject::deleteLater);
            connect(importModelThread, &ImportModelThread::openData, this, &AnkerMainWindow::importModelProgress);
            connect(importModelThread, &ImportModelThread::exitThreadSignal, this, &AnkerMainWindow::importModelProgressFinished);
            connect(importModelThread, &ImportModelThread::errorEncountered, this, &AnkerMainWindow::openFileError);
            //connect(this, &AnkerMainWindow::usbStateChanged, importModelThread, &ImportModelThread::usbChanged);
            importModelThread->start();
            m_importProgress->exec();

            //            importModelWorker = new ImportModelWoker();
            //            m_importProgress = new ProgressDialog();
            //            m_importProgress->setModal(true);
            //            m_importProgress->setCancelVisible(false);
            //            importModelWorker->initOpenParam(extension, fileName, tmpPrintModelPtr->m_oldMesh, QCallBack_ak);
            //            importModelWorker->moveToThread(&loadThread);
            //            connect(importModelWorker, &ImportModelWoker::exitThreadSignal, this, &AnkerMainWindow::importModelProgressFinished);
            //            connect(importModelWorker, &ImportModelWoker::exitThreadSignal, importModelWorker, &QObject::deleteLater);
            //            connect(importModelWorker, &ImportModelWoker::openData, this, &AnkerMainWindow::importModelProgress);
            //            loadThread.start();
            //            QTimer::singleShot(0, importModelWorker, &ImportModelWoker::doWork);
            //            m_importProgress->exec();

            if(importModelThread->openFileSuccessful()){  //

                tmpPrintModelPtr->cmeshoToMeshShowObj(*tmpPrintModelPtr->m_oldMesh);
                int index1 = fileName.lastIndexOf("/") + 1;
                int index2 = fileName.lastIndexOf(".");
                QString meshName = fileName.mid(index1, index2 - index1);
                qDebug() << "Import Mesh Name: " << meshName;
                tmpPrintModelPtr->setObjectName(meshName);
                tmpPrintModelPtr->m_meshPath = fileName;
                tmpPrintModelPtr->m_filePath = fileName;
                tmpPrintModelPtr->setColor(QColor(198, 198, 198));
                QMatrix4x4 trans;
                QVector3D offset;
                QVector3D platformCenter = getDoc()->m_machineBox->getBaseAABB().getCenterPoint();
                platformCenter.setZ(0.0);
                QVector3D modelCenter = tmpPrintModelPtr->calRealAABB().getCenterPoint();
                modelCenter.setZ(modelCenter.z() - tmpPrintModelPtr->m_realAABB.getLenZ() / 2);
                offset = platformCenter - modelCenter;
                tmpPrintModelPtr->m_rotCenter = tmpPrintModelPtr->m_realAABB.getCenterPoint();
                tmpPrintModelPtr->m_realAABB.m_Xmin = offset.x() + tmpPrintModelPtr->m_realAABB.m_Xmin;
                tmpPrintModelPtr->m_realAABB.m_Ymin = offset.y() + tmpPrintModelPtr->m_realAABB.m_Ymin;
                tmpPrintModelPtr->m_realAABB.m_Zmin = offset.z() + tmpPrintModelPtr->m_realAABB.m_Zmin;

                tmpPrintModelPtr->m_realAABB.m_Xmax = offset.x() + tmpPrintModelPtr->m_realAABB.m_Xmax;
                tmpPrintModelPtr->m_realAABB.m_Ymax = offset.y() + tmpPrintModelPtr->m_realAABB.m_Ymax;
                tmpPrintModelPtr->m_realAABB.m_Zmax = offset.z() + tmpPrintModelPtr->m_realAABB.m_Zmax;
                tmpPrintModelPtr->m_params.clear();
                tmpPrintModelPtr->m_params.push_back(1.0);
                tmpPrintModelPtr->m_params.push_back(1.0);
                tmpPrintModelPtr->m_params.push_back(1.0);
                tmpPrintModelPtr->m_params.push_back(0.0);
                tmpPrintModelPtr->m_params.push_back(0.0);
                tmpPrintModelPtr->m_params.push_back(0.0);
                tmpPrintModelPtr->m_params.push_back(offset[0]);
                tmpPrintModelPtr->m_params.push_back(offset[1]);
                tmpPrintModelPtr->m_params.push_back(offset[2]);
                trans.translate(offset[0], offset[1], offset[2]);

                tmpPrintModelPtr->setTransform(trans * tmpPrintModelPtr->getTransform());
                tmpPrintModelPtr->m_initParams = tmpPrintModelPtr->m_params;
                tmpPrintModelPtr->m_initTransform = tmpPrintModelPtr->getTransform();
                //tmpPrintModelPtr->isSceneIn(tmpPrintModelPtr->m_realAABB, getDoc()->m_machineBox->getBaseAABB());
                tmpPrintModelPtr->updateToScene();
                getDoc()->addObj(tmpPrintModelPtr);
                getDoc()->modelCheckSceneIn();
            }
        }
        catch (const MLException& e)
        {
            MessageDialog messageBox(tr("Error While Opening"), QString(e.what()), MessageDialog::OK);
            messageBox.exec();
        }
        qApp->restoreOverrideCursor();
    }

    curScene->refresh();
    return;
}

//void AnkerMainWindow::importMesh_ak(QString fileName)
//{
//    QStringList fileNameList;
//    if (fileName.isEmpty())
//    {
//        fileNameList = QFileDialog::getOpenFileNames(
//                    this, tr("Import Mesh"),
//                    lastUsedDirectory.path(),
//                    PM.inputMeshFormatListDialog().join(";;"));
//    }
//    else
//    {
//        fileNameList.push_back(fileName);
//    }

//    if (fileNameList.isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        //save path away so we can use it again
//        QString path = fileNameList.first();
//        path.truncate(path.lastIndexOf("/"));
//        lastUsedDirectory.setPath(path);
//    }

//    QElapsedTimer allFileTime;
//    allFileTime.start();
//    for (const QString& fileName : fileNameList)
//    {
//        QFileInfo fi(fileName);
//        if (!fi.exists())
//        {
//            QString errorMsgFormat = "Unable to open file:\n\"%1\"\n\nError details: file %1 does not exist.";
//            //QMessageBox::critical(this, tr("Meshlab Opening Error"), errorMsgFormat.arg(fileName));
//            MessageDialog messageBox(tr("Error While Opening"), errorMsgFormat, MessageDialog::OK);
//            messageBox.exec();
//            return;
//        }
//        if (!fi.isReadable())
//        {
//            QString errorMsgFormat = "Unable to open file:\n\"%1\"\n\nError details: file %1 is not readable.";
//            //QMessageBox::critical(this, tr("Meshlab Opening Error"), errorMsgFormat.arg(fileName));
//            MessageDialog messageBox(tr("Error While Opening"), errorMsgFormat, MessageDialog::OK);
//            messageBox.exec();
//            return;
//        }

//        QString extension = fi.suffix();
//        IOPlugin* pCurrentIOPlugin = PM.inputMeshPlugin(extension);

//        if (pCurrentIOPlugin == nullptr)
//        {
//            QString errorMsgFormat("Unable to open file:\n\"%1\"\n\nError details: file format " + extension + " not supported.");
//            //QMessageBox::critical(this, tr("Meshlab Opening Error"), errorMsgFormat.arg(fileName));
//            MessageDialog messageBox(tr("Error While Opening"), errorMsgFormat, MessageDialog::OK);
//            messageBox.exec();
//            return;
//        }

//        pCurrentIOPlugin->setLog(&meshDoc()->Log);
//        RichParameterList prePar = pCurrentIOPlugin->initPreOpenParameter(extension);
//        if (!prePar.isEmpty())
//        {
//            // take the default values of the plugin and overwrite to the settings
//            // default values
//            for (RichParameter& p : prePar)
//            {
//                QString prefixName = "MeshLab::IO::" + extension.toUpper() + "::";
//                if (currentGlobalParams.hasParameter(prefixName + p.name()))
//                {
//                    const RichParameter& cp = currentGlobalParams.getParameterByName(prefixName + p.name());
//                    p.setValue(cp.value());
//                }
//            }
//        }

//        meshDoc()->setBusy(true);
//        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

//        //check how many meshes are going to be loaded from the file
//        unsigned int nMeshes = pCurrentIOPlugin->numberMeshesContainedInFile(extension, fileName, prePar);

//        QFileInfo info(fileName);
//        std::list<MeshModel*> meshList;
//        for (unsigned int i = 0; i < nMeshes; i++)
//        {
//            MeshModel* mm = meshDoc()->addNewMesh(fileName, info.fileName());

//            if (nMeshes != 1)
//            {
//                // if the file contains more than one mesh, this id will be
//                // != -1
//                mm->setIdInFile(i);
//            }
//            meshList.push_back(mm);
//        }
//        //qb->show();
//        std::list<int> masks;
//        try
//        {
//            QElapsedTimer t;
//            t.start();
//            std::list<std::string> unloadedTextures =
//                    meshlab::loadMesh(fileName, pCurrentIOPlugin, prePar, meshList, masks, QCallBack_ak);
//            //saveRecentFileList_ak(fileName);
//            //updateLayerDialog();
//            /*for (MeshModel* mm : meshList)
//            {
//                computeRenderingDataOnLoading(mm, false, nullptr);
//                if (!(mm->cm.textures.empty()))
//                    updateTexture(mm->id());
//            }*/
//            QString warningString = pCurrentIOPlugin->warningMessageString();
//            if (unloadedTextures.size() > 0)
//            {
//                warningString += "\n\nThe following textures have not been loaded: \n";
//                for (const std::string& txt : unloadedTextures)
//                    warningString += QString::fromStdString(txt) + "\n";
//            }
//            if (!warningString.isEmpty())
//            {
//                //QMessageBox::warning(this, "Meshlab Opening Warning", warningString);
//                MessageDialog messageBox(tr("Error While Opening"), warningString, MessageDialog::OK);
//                messageBox.exec();
//            }
//            //GLA()->Logf(0, "Opened mesh %s in %i msec", qUtf8Printable(fileName), t.elapsed());
//        }
//        catch (const MLException& e)
//        {
//            for (const MeshModel* mm : meshList)
//                meshDoc()->delMesh(mm->id());
//            //GLA()->Logf(0, "Error: File %s has not been loaded", qUtf8Printable(fileName));
//            //QMessageBox::critical(this, "Meshlab Opening Error", e.what());
//            MessageDialog messageBox(tr("Error While Opening"), QString(e.what()), MessageDialog::OK);
//            messageBox.exec();
//        }
//        meshDoc()->setBusy(false);
//        qApp->restoreOverrideCursor();
//    }// end foreach file of the input list
//    //GLA()->Logf(0, "All files opened in %i msec", allFileTime.elapsed());

//    static std::set<MeshModel*> doneModels;
//    std::vector<CH3DPrintModelPtr> ttmeshes;
//    int countNum = 0;
//    for (MeshDocument::MeshIterator it = meshDoc()->meshBegin(); it != meshDoc()->meshEnd(); it++)
//    {
//        std::set<MeshModel*>::iterator tttt = doneModels.find(&*it);
//        if (tttt == doneModels.end())
//        {
//            doneModels.insert(&*it);
//            CH3DPrintModelPtr tmodel = it->toMyModel();
//            tmodel->setColor(QColor(198, 198, 198));
//            QMatrix4x4 trans;
//            QVector3D offset;

//            QVector3D platformCenter = getDoc()->m_machineBox->getBaseAABB().getCenterPoint();
//            QVector3D modelCenter = tmodel->calRealAABB().getCenterPoint();
//            offset = platformCenter - modelCenter;
//            tmodel->m_rotCenter = modelCenter;
//            tmodel->m_realAABB.m_Xmin = offset.x() + tmodel->m_realAABB.m_Xmin;
//            tmodel->m_realAABB.m_Ymin = offset.y() + tmodel->m_realAABB.m_Ymin;
//            //tmodel->m_realAABB.m_Zmin = offset.z() + tmodel->m_realAABB.m_Zmin;

//            tmodel->m_realAABB.m_Xmax = offset.x() + tmodel->m_realAABB.m_Xmax;
//            tmodel->m_realAABB.m_Ymax = offset.y() + tmodel->m_realAABB.m_Ymax;
//            //tmodel->m_realAABB.m_Zmax = offset.z() + tmodel->m_realAABB.m_Zmax;

//            tmodel->m_params.clear();
//            tmodel->m_params.push_back(1.0);
//            tmodel->m_params.push_back(1.0);
//            tmodel->m_params.push_back(1.0);
//            tmodel->m_params.push_back(0.0);
//            tmodel->m_params.push_back(0.0);
//            tmodel->m_params.push_back(0.0);
//            tmodel->m_params.push_back(offset[0]);
//            tmodel->m_params.push_back(offset[1]);
//            tmodel->m_params.push_back(0.0);
//            trans.translate(offset[0], offset[1], 0.0);

//            tmodel->setTransform(trans * tmodel->getTransform());
//            tmodel->m_initParams = tmodel->m_params;
//            tmodel->m_initTransform = tmodel->getTransform();

//            //QString mmname = it->fullName();
//            //QFileInfo info(mmname);

//            QString tname = it->fullName().right(it->fullName().length() - first - 1);
//            //QString tname = info.fileName();

//            tmodel->updateToScene();

//            ttmeshes.push_back(tmodel);
//            countNum++;
//        }
//    }

//    if (ttmeshes.size() > 0)
//    {
//        getDoc()->addObjs(ttmeshes);
//        curScene->refresh();
//    }


//    //    qb->reset();
//    //insertRecentFile(fileNameList);
//    //QMdiSubWindow* subwindow = mdiarea->currentSubWindow();
//    //if (subwindow) {
//    //    MultiViewer_Container* split = qobject_cast<CHChildDocWindow*>(mdiarea->currentSubWindow()->widget());
//    //    if (split) {
//    //        GLArea* glarea = split->currentgla;
//    //        glarea->update();
//    //    }
//    //}
//    return;
//}

void AnkerMainWindow::closeEvent(QCloseEvent* event)
{
    
    MessageDialog messageDialog(tr("Notice"), tr("Your changes will be lost without saving. Do you want to save now?"),
                                MessageDialog::CANCEL | MessageDialog::SAVE | MessageDialog::DoNotSave);
    int button = messageDialog.exec();
    if(button == MessageDialog::SAVE | button == MessageDialog::DoNotSave)
    {
        if(button == MessageDialog::SAVE) {
            qDebug() << " button ==" << button << "save  ==" << MessageDialog::SAVE  << " not save ==" <<  MessageDialog::DoNotSave;
             slotSaveProject();
        }
//        QList<MeshLabPlugin*> plugins;
//        for (MeshLabPlugin* fp : PM.pluginIterator()) {
//            plugins.append(fp);
//        }
//        currActionEdit = nullptr;

//        for (auto fp : plugins) {
//            PM.unloadPlugin(fp);
//        }
        
        //unloadPluginsSlot();
        QMainWindow::closeEvent(event);
        return;

    }
    event->ignore();
   // return;
//    else if()

//    QList<MeshLabPlugin*> plugins;
//    for (MeshLabPlugin* fp : PM.pluginIterator()) {
//        plugins.append(fp);
//    }
//    currActionEdit = nullptr;

//    for (auto fp : plugins) {
//        PM.unloadPlugin(fp);
//    }

//    QMainWindow::closeEvent(event);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool AnkerMainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool AnkerMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
#ifdef _WIN32
    MSG *msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    qDebug() << __FUNCTION__ << "u disk come";
                    TDebug("u disk come");
                    emit usbStateChanged(USB_STATE_COME_IN);
                }
            }
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    qDebug() << __FUNCTION__ << "u disk remove";
                    TDebug("u disk remove");
                    emit usbStateChanged(USB_STATE_COME_OUT);
                }
            }
            break;

        case DBT_DEVNODES_CHANGED:
            qDebug() << "USB Change";
            TDebug("USB Change");
            break;
        }
    }
    if (m_framelessHelper != 0) {
        return m_framelessHelper->nativeEvent(eventType, message, result);
    }

#elif __APPLE__

#endif
    return QWidget::nativeEvent(eventType, message, result);
}

void AnkerMainWindow::getMask(int& mask, const CMeshO& cm)
{
    if (HasPerFaceColor(cm))
    {
        mask = mask | vcg::tri::io::Mask::IOM_FACECOLOR;
    }
    if (HasPerWedgeTexCoord(cm))
    {
        mask = mask | vcg::tri::io::Mask::IOM_WEDGTEXCOORD;
    }
    if (HasPerWedgeNormal(cm))
    {
        mask = mask | vcg::tri::io::Mask::IOM_WEDGNORMAL;
    }
    if (HasPerVertexColor(cm))
    {
        mask = mask | vcg::tri::io::Mask::IOM_VERTCOLOR;
    }
}

bool AnkerMainWindow::exportMesh_ak(const QString& _fileName, CMeshO& cm)
{
    QString lastPath = QApplication::applicationDirPath() + "/document";
    QString recentExport = m_settings->readExportRecent();
    if(!recentExport.isEmpty())
    {
        lastPath = recentExport;
    }
    lastPath += "/" + _fileName + ".stl";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model File"), lastPath, QString("Model Files(*.stl) ;; Model Files(*.obj)"));
    if (fileName.isEmpty())
        return false;
    int index1 = fileName.lastIndexOf("/");
    //qDebug() << "write recent export: " << fileName.mid(0, index1);
    m_settings->writeExportRecent(fileName.mid(0, index1));

    QFileInfo fi(fileName);
    QString format = fi.suffix();

    bool saved = true;
    int mask = 0;
    getMask(mask, cm);
    qDebug() << "exportMesh_ak mask: " << mask;
    
    bool isBinary = true;
    try
    {
        // MeshModelImport_Export::save(format, fileName, cm, mask, ProjectLoadSave::getInstance()._QCallBack);
        exportModelThread = new ExportModelThread(this);
        m_exportProgress = new ProgressDialog;
        modelExporting = true;
        m_exportProgress->setModal(true);
        m_exportProgress->setAutoClosed(false);
        m_exportProgress->setCancelVisible(false);
        exportModelThread->initExportParam(format, fileName, &cm, mask, QCallBack_ak);
        connect(exportModelThread, &ExportModelThread::finished, this, &AnkerMainWindow::exportModelProgressFinished);
        connect(exportModelThread, &ExportModelThread::finished, exportModelThread, &QObject::deleteLater);
        connect(exportModelThread, &ExportModelThread::exportData, this, &AnkerMainWindow::exportModelProgress);
        connect(exportModelThread, &ExportModelThread::errorEncountered, this, &AnkerMainWindow::exportModelError);
        exportModelThread->start();
        m_exportProgress->exec();
    }
    catch (const std::exception& e)
    {
        MessageDialog messageBox(tr("Error While Saving"), QString(e.what()), MessageDialog::OK);
        messageBox.exec();
        saved = false;
    }
    //QMessageBox::information(NULL, tr("Success"), tr("Export Successful!"));
    qApp->restoreOverrideCursor();
    if (saved)
        QDir::setCurrent(fi.absoluteDir().absolutePath());
    return saved;
}


//bool AnkerMainWindow::exportMesh_ak(QString fileName, MeshModel* mod, const bool saveAllPossibleAttributes)
//{
//    const QStringList& suffixList = PM.outputMeshFormatListDialog();
//    if (fileName.isEmpty()) {
//        //QHash<QString, MeshIOInterface*> allKnownFormats;
//        //PM.LoadFormats( suffixList, allKnownFormats,PluginManager::EXPORT);
//        //QString defaultExt = "*." + mod->suffixName().toLower();
//        QString defaultExt = "*.ply";
//        if (mod == NULL)
//            return false;
//        mod->setMeshModified(false);
//        QString laylabel = "Save \"" + mod->label() + "\" Layer";
//        QFileDialog* saveDialog = new QFileDialog(this, laylabel, lastUsedDirectory.path());
//        //saveDialog->setOption(QFileDialog::DontUseNativeDialog);
//        saveDialog->setNameFilters(suffixList);
//        saveDialog->setAcceptMode(QFileDialog::AcceptSave);
//        saveDialog->setFileMode(QFileDialog::AnyFile);
//        saveDialog->selectFile(fileName);
//        QStringList matchingExtensions = suffixList.filter(defaultExt);
//        if (!matchingExtensions.isEmpty())
//            saveDialog->selectNameFilter(matchingExtensions.last());

//        saveDialog->selectFile(meshDoc()->mm()->fullName());
//        int dialogRet = saveDialog->exec();
//        if (dialogRet == QDialog::Rejected)
//            return false;
//        fileName = saveDialog->selectedFiles().at(0);
//        QFileInfo fni(fileName);
//        if (fni.suffix().isEmpty()) {
//            QString ext = saveDialog->selectedNameFilter();
//            ext.chop(1); ext = ext.right(4);
//            fileName = fileName + ext;
//            qDebug("File without extension adding it by hand '%s'", qUtf8Printable(fileName));
//        }
//    }
//    QFileInfo fi(fileName);

//    QStringList fs = fileName.split(".");

//    if (!fileName.isEmpty() && fs.size() < 2) {
//        MessageDialog messageBox(tr("Error While Saving"), tr("You must specify file extension!!"), MessageDialog::OK);
//        messageBox.exec();
//        return false;
//    }

//    bool saved = true;
//    if (!fileName.isEmpty()) {
//        //save path away so we can use it again
//        QString path = fileName;
//        path.truncate(path.lastIndexOf("/"));
//        lastUsedDirectory.setPath(path);

//        QString extension = fileName;
//        extension.remove(0, fileName.lastIndexOf('.') + 1);

//        QStringListIterator itFilter(suffixList);

//        IOPlugin* pCurrentIOPlugin = PM.outputMeshPlugin(extension);
//        if (pCurrentIOPlugin == 0) {
//            MessageDialog messageBox(tr("Unknown File Type"), tr("File extension not supported."), MessageDialog::OK);
//            messageBox.exec();
//            return false;
//        }
//        pCurrentIOPlugin->setLog(&meshDoc()->Log);

//        int capability = 0, defaultBits = 0;
//        pCurrentIOPlugin->exportMaskCapability(extension, capability, defaultBits);

//        // optional saving parameters (like ascii/binary encoding)
//        RichParameterList savePar = pCurrentIOPlugin->initSaveParameter(extension, *(mod));

//        SaveMeshAttributesDialog maskDialog(this, mod, capability, defaultBits, savePar);
//        int quality = -1;
//        bool saveTextures = true;
//        if (!saveAllPossibleAttributes)
//        {
//            maskDialog.exec();
//        }
//        else
//        {
//            //this is horrible: creating a dialog object but then not showing the
//            //dialog.. And using it just to select all the possible options..
//            //to be removed soon
//            maskDialog.selectAllPossibleBits();
//        }
//        int mask = maskDialog.getNewMask();
//        savePar = maskDialog.getNewAdditionalSaveParameters();
//        quality = maskDialog.getTextureQuality();
//        saveTextures = maskDialog.saveTextures();

//        if (!saveTextures)
//        {
//            std::vector<std::string> textureNames = maskDialog.getTextureNames();

//            for (unsigned int i = 0; i < mod->cm.textures.size(); ++i)
//            {
//                if (textureNames[i].find('.') == std::string::npos)
//                {
//                    textureNames[i] += ".png";
//                }
//                mod->changeTextureName(mod->cm.textures[i], textureNames[i]);
//            }
//        }
//        if (!saveAllPossibleAttributes)
//        {
//            maskDialog.close();
//            if (maskDialog.result() == QDialog::Rejected)
//                return false;
//        }
//        if (mask == -1)
//            return false;

//        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
//        //qb->show();
//        QElapsedTimer tt; tt.start();
//        meshDoc()->setBusy(true);
//        try
//        {
//            if (mask & vcg::tri::io::Mask::IOM_BITPOLYGONAL)
//                mod->updateDataMask(MeshModel::MM_FACEFACETOPO);
//            pCurrentIOPlugin->save(extension, fileName, *mod, mask, savePar, QCallBack_ak);
//            QFileInfo finfo(fileName);
//            if (saveTextures)
//                mod->saveTextures(finfo.absolutePath(), quality, &meshDoc()->Log, QCallBack_ak);
//            //GLA()->Logf(GLLogStream::SYSTEM, "Saved Mesh %s in %i msec", qUtf8Printable(fileName), tt.elapsed());
//            mod->setFileName(fileName);
//            QSettings settings;
//            int savedMeshCounter = settings.value("savedMeshCounter", 0).toInt();
//            settings.setValue("savedMeshCounter", savedMeshCounter + 1);
//        }
//        catch (const MLException& e) {
//            //GLA()->Logf(GLLogStream::SYSTEM, "Error Saving Mesh %s", qUtf8Printable(fileName));
//            MessageDialog messageBox(tr("Error While Saving"), QString(e.what()), MessageDialog::OK);
//            messageBox.exec();
//            saved = false;
//        }
//        qApp->restoreOverrideCursor();
//        //updateLayerDialog();
//        meshDoc()->setBusy(false);
//        //qb->reset();

//        if (saved)
//            QDir::setCurrent(fi.absoluteDir().absolutePath()); //set current dir
//    }
//    return saved;
//}

void AnkerMainWindow::initTitleBar()
{
    auto mainMenuBar = new QMenuBar();
    mainMenuBar->setFixedHeight(28);
    mainMenuBar->setObjectName("mainMenuBar");
    m_mainTitleBar = new customTitle(this, mainMenuBar);
    m_mainTitleBar->setObjectName("m_mainTitleBar");
    m_mainTitleBar->setFixedHeight(40);
#ifdef Q_OS_WIN
    this->setMenuWidget(m_mainTitleBar);
    m_framelessHelper = new QFramelessHelper(this, true, false, true, this);
    m_framelessHelper->setTitleBar(m_mainTitleBar);
#endif
    connect(m_mainTitleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_mainTitleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()), Qt::DirectConnection);
    connect(m_mainTitleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()), Qt::DirectConnection);
    connect(m_mainTitleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void AnkerMainWindow::onButtonMinClicked()
{
    showMinimized();
}

void AnkerMainWindow::onButtonRestoreClicked()
{
    showNormal();
}

void AnkerMainWindow::onButtonMaxClicked()
{
    showMaximized();
    QDesktopWidget *desk = QApplication::desktop();
    setGeometry(desk->availableGeometry());
}

void AnkerMainWindow::onButtonCloseClicked()
{
    close();
}



void AnkerMainWindow::unloadPluginsSlot()
{
    QList<MeshLabPlugin*> plugins;
    for (MeshLabPlugin* fp : PM.pluginIterator()) {
        plugins.append(fp);
    }
    currActionEdit = nullptr;

    for (auto fp : plugins) {
        PM.unloadPlugin(fp);
    }
    emit pluginsUnloaded();
}

bool AnkerMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (this != 0 && watched == this) {
        if (event->type() == QEvent::Resize) {
            
            int width = this->width();
            int height = this->height();

            
            rectLeft = QRect(0, padding, padding, height - padding * 2);
            
            rectTop = QRect(padding, 0, width - padding * 2, padding);
            
            rectRight = QRect(width - padding, padding, padding, height - padding * 2);
            
            rectBottom = QRect(padding, height - padding, width - padding * 2, padding);

            
            rectLeftTop = QRect(0, 0, padding, padding);
            
            rectRightTop = QRect(width - padding, 0, padding, padding);
            
            rectLeftBottom = QRect(0, height - padding, padding, padding);
            
            rectRightBottom = QRect(width - padding, height - padding, padding, padding);
        } else if (event->type() == QEvent::HoverMove) {
            
            QHoverEvent *hoverEvent = (QHoverEvent *)event;
            QPoint point = hoverEvent->pos();
            bool resizeEnable = true;
            if (resizeEnable) {
                if (!mousePressedStatus) {
                    if (rectLeft.contains(point)) {
                        this->setCursor(Qt::SizeHorCursor);
                    } else if (rectRight.contains(point)) {
                        this->setCursor(Qt::SizeHorCursor);
                    } else if (rectTop.contains(point)) {
                        this->setCursor(Qt::SizeVerCursor);
                    } else if (rectBottom.contains(point)) {
                        this->setCursor(Qt::SizeVerCursor);
                    } else if (rectLeftTop.contains(point)) {
                        this->setCursor(Qt::SizeFDiagCursor);
                    } else if (rectRightTop.contains(point)) {
                        this->setCursor(Qt::SizeBDiagCursor);
                    } else if (rectLeftBottom.contains(point)) {
                        this->setCursor(Qt::SizeBDiagCursor);
                    } else if (rectRightBottom.contains(point)) {
                        this->setCursor(Qt::SizeFDiagCursor);
                    } else {
                        this->setCursor(Qt::ArrowCursor);
                    }
                }
            }

            
            int offsetX = point.x() - lastPosP.x();
            int offsetY = point.y() - lastPosP.y();

            
            bool moveEnable = false;
            if (moveEnable) {
                if (pressed) {
                    this->move(this->x() + offsetX, this->y() + offsetY);
                }
            }

            if (resizeEnable) {
                if (pressedLeft) {
                    int resizeW = this->width() - offsetX;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, rectY, resizeW, rectH);
                    }
                } else if (pressedRight) {
                    this->setGeometry(rectX, rectY, rectW + offsetX, rectH);
                } else if (pressedTop) {
                    int resizeH = this->height() - offsetY;
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(rectX, this->y() + offsetY, rectW, resizeH);
                    }
                } else if (pressedBottom) {
                    this->setGeometry(rectX, rectY, rectW, rectH + offsetY);
                } else if (pressedLeftTop) {
                    int resizeW = this->width() - offsetX;
                    int resizeH = this->height() - offsetY;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, this->y(), resizeW, resizeH);
                    }
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y() + offsetY, resizeW, resizeH);
                    }
                } else if (pressedRightTop) {
                    int resizeW = rectW + offsetX;
                    int resizeH = this->height() - offsetY;
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y() + offsetY, resizeW, resizeH);
                    }
                } else if (pressedLeftBottom) {
                    int resizeW = this->width() - offsetX;
                    int resizeH = rectH + offsetY;
                    if (this->minimumWidth() <= resizeW) {
                        this->setGeometry(this->x() + offsetX, this->y(), resizeW, resizeH);
                    }
                    if (this->minimumHeight() <= resizeH) {
                        this->setGeometry(this->x(), this->y(), resizeW, resizeH);
                    }
                } else if (pressedRightBottom) {
                    int resizeW = rectW + offsetX;
                    int resizeH = rectH + offsetY;
                    this->setGeometry(this->x(), this->y(), resizeW, resizeH);
                }
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            
            QMouseEvent *mouseEvent = (QMouseEvent *)event;
            rectX = this->x();
            rectY = this->y();
            rectW = this->width();
            rectH = this->height();
            lastPosP = mouseEvent->pos();

            
            if (rectLeft.contains(lastPosP)) {
                pressedLeft = true;
            } else if (rectRight.contains(lastPosP)) {
                pressedRight = true;
            } else if (rectTop.contains(lastPosP)) {
                pressedTop = true;
            } else if (rectBottom.contains(lastPosP)) {
                pressedBottom = true;
            } else if (rectLeftTop.contains(lastPosP)) {
                pressedLeftTop = true;
            } else if (rectRightTop.contains(lastPosP)) {
                pressedRightTop = true;
            } else if (rectLeftBottom.contains(lastPosP)) {
                pressedLeftBottom = true;
            } else if (rectRightBottom.contains(lastPosP)) {
                pressedRightBottom = true;
            } else {
                pressed = true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            
        } else if (event->type() == QEvent::MouseButtonRelease) {
            
            pressed = false;
            pressedLeft = false;
            pressedRight = false;
            pressedTop = false;
            pressedBottom = false;
            pressedLeftTop = false;
            pressedRightTop = false;
            pressedLeftBottom = false;
            pressedRightBottom = false;
            this->setCursor(Qt::ArrowCursor);
        }
    }

    return QObject::eventFilter(watched, event);
}


