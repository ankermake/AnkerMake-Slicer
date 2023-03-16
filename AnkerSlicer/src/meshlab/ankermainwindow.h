#ifndef ANKERMAINWINDOW_H
#define ANKERMAINWINDOW_H

#include <QMainWindow>
//#include "mainwindow.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSettings>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QMimeData>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QThread>
#include <QProcess>
#include <QDir>
#include <QEvent>
#include "controls/controlmanager.h"
#include "controls/toolBar.h"
#include "settingmanager.h"
#include "controls/meshinfowidget.h"
#include "controls/splitterwidget.h"
#include "controls/mainwidget.h"
#include "common/controlInterface/progressdialog.h"
#include "common/controlInterface/messageDialog.h"
#include "../common/Socket/QMainSocket2.h"
#include "../common/Socket/HeartBeatThead.h"
#include "controls/customtitle.h"
#include "qframelesshelper.h"
using namespace control;
using namespace settings;

class AkMainWindowPre : public QMainWindow
{
    Q_OBJECT
public:
    AkMainWindowPre():
        //gpumeminfo(NULL),
        defaultGlobalParams(meshlab::defaultGlobalParameterList()),
        lastUsedDirectory(QDir::home()),
        PM(meshlab::pluginManagerInstance()),
        messageProcessing(new MessageProcessing(this))
    {}

public:
    QMdiArea *mdiarea;
    GLArea* glarea;
    MessageProcessing * messageProcessing;

    RichParameterList currentGlobalParams;
    RichParameterList& defaultGlobalParams;
    QDir lastUsedDirectory;  //This will hold the last directory that was used to load/save a file/project in
    PluginManager& PM;

    MeshDocument meshDocument;
    MeshDocument *meshDoc() {
        return &meshDocument;
    }

    void updateTexture(int meshid){}
//    void computeRenderingDataOnLoading(MeshModel* mm,bool isareload, MLRenderingData* rendOpt){}
};

enum USB_STATE
{
   USB_STATE_KNOWN = -1,
   USB_STATE_COME_IN = 0,
   USB_STATE_COME_OUT = 1,
};

//class AnkerMainWindow : public MainWindow
class AnkerMainWindow : public AkMainWindowPre
{
    Q_OBJECT
public:
    explicit AnkerMainWindow();
    ~AnkerMainWindow();

private:
    void init();  

    void initMainWindow();           
    void initRichParameterList();    
    void initLoadPlugin();           
    void init_9_Delayed();              
    void initFdmWidget();
    void initEditTool(); 

    void changeEvent(QEvent *e);
private:
    QString openFilter(QString projectSuffix, QStringList meshSuffixList, QString gcodeSuffix, QString acodeSuffix);
    bool pathIsValid(QStringList pathList, QStringList suffixList);
    

    
    int checkFileSuffix(const QStringList &pathList, const QStringList &suffixList);
    bool isProject(QStringList pathList, QString gcodeSuffix);
    // void openFileList(QStringList fileNameList);
    void openGcodePreview(const QString& fileName);
    void openAcodePreview(const QString& fileName);

    static bool QCallBack_ak(const int pos, const char* str);
    //bool importMesh_ak(QString fileName=QString());
//    bool exportMesh_ak(QString fileName, MeshModel* mod, const bool saveAllPossibleAttributes);
    bool exportMesh_ak(const QString& fileName, CMeshO& cm);
    void getMask(int& mask, const CMeshO& cm);

    bool importMeshWithLayerManagement_ak(QString fileName = QString());
//    bool save_ak(const bool saveAllPossibleAttributes = false);
//    bool saveAs_ak(QString fileName = QString(), const bool saveAllPossibleAttributes = false);
    void paintEvent(QPaintEvent *e);
    
    void mainWindowInitFinished();
signals:
    void openFileSucessful(QStringList fileList);
    void usbStateChanged(int);

    
    void pluginsUnloaded();

//signals:
//    void resizeChildWidget();
//protected:
//    virtual void resizeEvent(QResizeEvent *event) override;

public slots:
    void openFileFromAppRaram(const QString &fileName);
    void openFileList(QStringList fileNameList);
    void otaNeedSaveProject(const QString &filePath);

private slots:
    void slotSaveProject();

    void slotSaveAsProject();

    void slotQuit();

    void saveMesh();
    void saveAllMesh();

    void open();

//    void importMesh_ak(QString fileName = QString());
    void importMesh(QString fileName = QString());

    void importModelProgress(int pos, const QString& str);
    void importModelProgressFinished();

    void exportModelProgress(int pos, const QString& str);
    void exportModelProgressFinished();

    void loginInitFeedbackSlot(qint64, int);

    void openFileError(int type, QString errorString);
    void exportModelError(int type, const QString &err);

    void mainMachineBoxChanged(const Value& value);

    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

    void unloadPluginsSlot();


private:
    bool processIsRun(const QString &exeName);
    bool removeAcodeFile(QString const &path);
    void initTitleBar();
    void showNormalWin();
    void showMaximizedWin();
private:
    QFramelessHelper *m_framelessHelper = nullptr;
    ControlManager* m_controlManager;
    SettingManager* m_settings;

    QString m_projectSuffix = QString("akpro");
    QStringList m_meshSuffixList;
    QString m_gcodeSuffix = QString("gcode");
    QString m_projectPath = QString();
    QString m_acodeSuffix = QString("acode");
    QStringList m_meshPath = QStringList();
    QString m_acodePath;
    QProcess  *m_pCmd;

    ProgressDialog *m_importProgress;
    QThread loadThread;
    ProgressDialog *m_exportProgress;


    QMainSocket2* m_socket = nullptr;
    QTimer* m_heartTimer = nullptr;
    QProcess* m_process;
    int heartBeatCount = 0;
    bool started = false;
    int m_nBorderWidth=8;
    customTitle *m_mainTitleBar = nullptr;
    QRect rectLeft;
    QRect rectTop;
    QRect rectRight;
    QRect rectBottom;
    QRect rectLeftTop;
    QRect rectRightTop;
    QRect rectLeftBottom;
    QRect rectRightBottom;
    QPoint lastPosP;
    int rectY;
    int rectH;
    int rectX;
    int rectW;
    int padding = 5;
    bool pressed = false;
    bool pressedLeft = false;
    bool pressedRight = false;
    bool pressedTop = false;
    bool pressedBottom = false;
    bool pressedLeftTop = false;
    bool pressedRightTop = false;
    bool pressedLeftBottom = false;
    bool pressedRightBottom = false;

    bool mousePressedStatus = false;
    bool m_ota = false;
protected:
    
    void closeEvent(QCloseEvent* event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result); 
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // ANKERMAINWINDOW_H
