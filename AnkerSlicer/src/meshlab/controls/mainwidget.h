#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QAction>
#include <QGridLayout>
#include <QMdiArea>
#include <QDesktopServices>
#include <QStackedLayout>
#include <QButtonGroup>
//#include "glarea.h"
#include "controls/controlmanager.h"
#include "controls/preferencesdialog.h"
#include "controls/generalwidget.h"
#include "controls/testwidget.h"
#include "settingmanager.h"
#include "common/GeoAndShow/CHCircleShowObj.h"
#include "message/messageprocessing.h"
#include "NewRender/CHChildDocWindow.h"
#include "common/controlInterface/messageDialog.h"
#include "copyrightwidget.h"
#include "common/Socket/QMainSocket2.h"
#include "controls/customtitle.h"
#include "controls/basetabwidget.h"

enum CURRENTPAGE {
    FDMWIDGET = 0 ,
    GCODEWIDGET ,
    NETWORKWIDGET,

};


#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#endif

namespace  control{
class FdmMainWidget : public BubbleWidget
{
    Q_OBJECT
public:
    explicit FdmMainWidget(MessageProcessing *messageProcessing, customTitle *title, ControlManager *controlManager, QWidget *parent = nullptr);
   ~FdmMainWidget();
    void initWindow(ControlManager *controlManager);

    void initTool();
    void initScene(QMdiArea* mdiarea );

    void setMainWindowId(WId id);
    void retranslateUi();
protected:
    void changeEvent(QEvent *e);
private:
    void createRecentProject(const QStringList& fileList, QMenu* recentMenu);

    void setActionEnble(bool enble);
    void initViewTransformAction();

signals:
    void fdmOpenFile();
    void fdmOpenRecent(QStringList fileNameList);
    void fdmSaveAllMesh();
    void fdmSaveMesh();
    void fdmSaveProject();
    void fdmSaveAsProject();
    void resizeWidget();
    void unloadPlugins();

public slots:
      void currentPageChanged(int index);

private slots:
    void open();
    void saveAllMesh();
    void saveMesh();
    void slotSaveProject();
    void slotSaveAsProject();

    void fdmOpeFileList(QStringList fileNameList);
    void openRecent();
    void clearRecentProjectList();
    void insertRecent(const QStringList& fileList);

    void openPreferencesDialog();

    void slotSelectAllObjects();
    void slotCopyObject();

    void deleteAllObject();
    void deleteSelectedObjs();

    void resetAllMeshModels();
    void resetSelectedObjs();

    void loginWidgetDisplaySlot();
    void loginWidgetDisplayFeedBackSlot(qint64, int);

    void setButtonEnableByPickStatus();
    void setButtonEnableByDocument();
    void setButtonEnableByVisible(int visbleModelNumber);

    
    void viewChanged(bool checked);
    void frontViewChanged(bool checked);
    void rearViewChanged(bool checked);
    void leftViewChanged(bool checked);
    void rightViewChanged(bool checked);
    void topViewChanged(bool checked);
    void bottomViewChanged(bool checked);

    void deviceActionToggled(bool checked);
    void displayMainWindow(bool showed);

    void openCopyrightWidget();

    void tabCurrentPageChanged(int index);

    void visibleModelCountChanged(int count);

private:
    QMenu* m_recentMenu;
    QMenu* m_exportMenu;
    QAction* m_exportSelectObjectAction;
    QAction* m_exportAllObjectAction;
    QAction* m_copyAction;
    QMenu* m_deleteMenu;
    QAction* m_deleteAllObjectAction;
    QAction* m_deleteSelectedObjectAction;
    QAction* m_undo;
    QAction* m_redo;
    QAction *m_selPro = nullptr;
    QMenu* m_resetMenu;
    QAction* m_resetSelectedAction;
    QAction* m_resetAllAction;

    QAction *m_actionOpen = nullptr;
    QAction *m_actionFileSave = nullptr;
    QAction *m_actionFileSaveAs = nullptr;
    MessageProcessing *m_messageProcessing;
    ControlManager* m_controlManager;

    settings::SettingManager m_settings;
    QList <QAction*> m_viewTransform;

    WId m_mainWinId;
    QMenu *m_menuFile = nullptr;
    QMenu *m_menuEdit = nullptr;
    QMenu *m_menuSetting = nullptr;
    QAction *m_actionSave = nullptr;
    QMenu *m_menuHelp = nullptr;
    QAction *m_tutorial = nullptr;
    QAction *m_feedback = nullptr;
    QAction *m_actionUpdate = nullptr;
    QAction *m_priPolicy = nullptr;
    QAction *m_termsOfUse = nullptr;
    QAction *m_copyright = nullptr;
    QAction *m_accountAction = nullptr;
    QAction *m_actionLeftOpen = nullptr;
    QAction *m_generalAction = nullptr;
    QAction *m_machineAction = nullptr;
    QAction *m_materialAction  = nullptr;
    QAction *m_parametersAction = nullptr;
    BaseTabWidget *m_tabWidget;
    QWidget *m_fdmWidget = nullptr;
    QWidget *m_gcodeWidget = nullptr;
    QWidget *m_netWorkWidget = nullptr;
    customTitle *m_titleBar = nullptr;
    QMenu *m_accoutMenu = nullptr;
};
}
#endif // MAINWIDGET_H
