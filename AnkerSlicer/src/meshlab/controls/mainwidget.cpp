#include "mainwidget.h"
#include "common/GeoAndShow/CH3DPrintModel.h"
#include "common/mlapplication.h"
#include <QWindow>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <qthreadpool.h>
namespace  control{
FdmMainWidget::FdmMainWidget(MessageProcessing *messageProcessing, customTitle *title, ControlManager *controlManager, QWidget *parent)
    : QWidget(parent),
      m_messageProcessing(messageProcessing),
      m_controlManager(controlManager),
      m_tabWidget(new BaseTabWidget(this)),
      m_titleBar(title)
{
    initWindow(controlManager);
}

FdmMainWidget::~FdmMainWidget()
{
    qDebug() << "~FdmMainWidget.";
}

void FdmMainWidget::initWindow(ControlManager *controlManager)
{
    AkUtil::TFunction("");
    QGridLayout *gridLayout = new QGridLayout(this);
    setLayout(gridLayout);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0,0,0,0);

//    ToolBar *topToolBar = controlManager->toolBar();
//    topToolBar->setParent(this);
//    topToolBar->setMinimumHeight(60);
//    topToolBar->setMaximumHeight(60);
//    gridLayout->addWidget(topToolBar,0,0);

    m_fdmWidget = new QWidget();
    QMdiArea* mdiarea = new QMdiArea();
    QHBoxLayout *splitter = new QHBoxLayout(m_fdmWidget);
    // splitter->setContentsMargins(5,0,5,5);
    splitter->setContentsMargins(0,0,0,0);
    splitter->setSpacing(0);
    splitter->addWidget(mdiarea);
    m_fdmWidget->setLayout(splitter);
    controlManager->setRightWidget(splitter);
    m_tabWidget->addTab(m_fdmWidget,QIcon(":/images/icon/fdmSlicerTab.png"),tr("Slice"));

    controlManager->setGlareaWidget(this);
    controlManager->setMainWindow(this->parentWidget());
    GeneralWidget *widget = new GeneralWidget(m_messageProcessing);
    controlManager->addPageToPreferences(widget,0);

    m_messageProcessing->sendMsg2Preview();
    connect(m_messageProcessing,&MessageProcessing::sendMsg2GetpreviewWid,[=](QObject *object){
        m_gcodeWidget = qobject_cast<QWidget *>(object);
        if(m_gcodeWidget != nullptr)
        {
            m_tabWidget->addTab(m_gcodeWidget,QIcon(":/images/icon/previewTab.png"), tr("Preview"));
            connect(m_tabWidget,SIGNAL(currentChanged(int)),m_gcodeWidget,SLOT(pauseGcodePlay(int)));
        }
    });




    connect(m_messageProcessing,&MessageProcessing::checkoutPreview,[=](bool isc){
          if(isc)
          {
           currentPageChanged(FDMWIDGET);
          // m_switchWidget->raise();
          }else{
           currentPageChanged(GCODEWIDGET);
          }
    });

    connect(m_messageProcessing,&MessageProcessing::networkSendWid2AnkerMake,[=](QObject *object){
        AkUtil::TFunction("");
        m_netWorkWidget = qobject_cast<QWidget *>(object);
        QString str;
        QDebug(&str) << "m_netWorkWidget: " << m_netWorkWidget << ", object: " << object;
        AkUtil::TDebug("network insert tab widget display: " + str);
        if(m_netWorkWidget != nullptr)
        {
            m_tabWidget->addTab(m_netWorkWidget,QIcon(":/images/icon/deviceTab.png"), tr("Device"));
        }

        //WId _wid = wid.toInt();
        //AkUtil::TDebug("FdmMainWidget WINID: " + QString::number(_wid));
//        QWindow *window= QWindow::fromWinId(_wid);
//        if(window) {
//            QWidget *widget = QWidget::createWindowContainer(window, this,Qt::Widget|Qt::FramelessWindowHint);
//            if(widget != nullptr) {
//                m_netWorkWidget = widget;
//                m_tabWidget->addTab(m_netWorkWidget,QIcon(":/images/icon/deviceTab.png"), tr("Device"));
//            }
//        }
    });

    connect(m_messageProcessing,&MessageProcessing::swtichNetworkWidgetSignal, [=]{
        AkUtil::TFunction("");
        AkUtil::TDebug("Network widget display.");
        currentPageChanged(NETWORKWIDGET);
    });

    //networkDeadSig
    connect(m_messageProcessing,&MessageProcessing::networkDeadSig, [=]{
         AkUtil::TDebug("networkDeadSig.");
    });

    //MainWindow modal
    connect(m_messageProcessing,&MessageProcessing::mainwindowModalSig, [=](int wid){
        AkUtil::TDebug("Qt::ApplicationModal.");
        QDialog *loginDialog = new QDialog(parentWidget());
        loginDialog->setAttribute(Qt::WA_DeleteOnClose);
        loginDialog->resize(800,700);
        QWindow *loginWindow = QWindow::fromWinId(wid);
        QWidget *loginWidget = QWidget::createWindowContainer(loginWindow, loginDialog, Qt::Widget);
        QHBoxLayout *layout = new QHBoxLayout(loginDialog);
        layout->addWidget(loginWidget);
        loginDialog->setLayout(layout);
        loginWidget->showMaximized();
        loginDialog->exec();
    });




//    connect(m_tabWidget, &BaseTabWidget::currentChanged, [&]() {
//        if (nullptr == m_menuFile)
//        {
//            return;
//        }
//        m_menuFile->show();
//        m_menuFile->hide();


//        //QObject* currentParent = m_tabWidget->parent();
//        //QObject* mainWindow = dynamic_cast<AnkerMainWindow*>(currentParent);
//        //while (currentParent != nullptr && mainWindow == nullptr)
//        //{
//        //    currentParent = currentParent->parent();
//        //    mainWindow = dynamic_cast<AnkerMainWindow*>(currentParent);
//        //}

//        //if (mainWindow != nullptr)
//        //{
//        //    AnkerMainWindow* win = dynamic_cast<AnkerMainWindow*>(currentParent);

//        //}
//    });

    gridLayout->addWidget(m_tabWidget,1,0);
    initScene(mdiarea);
    initTool();

    setActionEnble(false);
    initViewTransformAction();
    connect(m_tabWidget,&BaseTabWidget::currentChanged,this,&FdmMainWidget::tabCurrentPageChanged);

    PluginMessageData mwData;
    mwData.from = AkConst::Plugin::AK_MAIN_WINDOW;
    mwData.dest = AkConst::Plugin::FDM_NETWORK;
    mwData.msg = AkConst::Msg::ANKERMAKE_MAINWINDOW_PTR;
    mwData.object = parentWidget();
    m_messageProcessing->sendMsg2Manager(mwData);

    QString mwStr;
    QDebug(&mwStr) << mwData.object;
    AkUtil::TDebug("mwData.object: " + mwStr);
}

void FdmMainWidget::initTool()
{
//    QAction *actionDevice = new QAction(QIcon(":/images/icon/fdm_tool_home_icon_n.png"),tr("Device"),nullptr);
//      //void toggled(bool)
//    connect(actionDevice, &QAction::triggered, this, &FdmMainWidget::deviceActionToggled);
   // m_controlManager->addActionToToolBar(actionDevice);
   // m_controlManager->addSeparatorToToolBar();
    m_menuFile = m_titleBar->addMenu(tr("File"));
    //QMenu* menuFile = new QMenu(tr("File"));
   // menuFile->setAttribute(Qt::WA_TranslucentBackground);
   // menuFile->setIcon(QIcon(":/images/icon/fdm_tool_open_icon_n.png"));
    //QMenu *menuFile = new QMenu(tr("File"));
   // m_menuFile = new QMenu(tr("File"));
   // menuFile->setAttribute(Qt::WA_TranslucentBackground);
   // m_menuFile->setIcon(QIcon(":/images/icon/fdm_tool_open_icon_n.png"));

   // menu->addAction(QIcon(":/images/icon/menu_file.png"), tr(" File"));
    m_actionOpen = m_menuFile->addAction(QIcon(), tr("Open"), this, &FdmMainWidget::open);
    m_recentMenu = m_menuFile->addMenu(QIcon(), tr("Open Recent"));

    createRecentProject(m_settings.readRecent(), m_recentMenu);
    m_exportMenu = m_menuFile->addMenu(QIcon(), tr("Export..."));
    m_exportAllObjectAction = m_exportMenu->addAction(QIcon(), tr("All Objects"), this, &FdmMainWidget::saveAllMesh);
    m_exportSelectObjectAction = m_exportMenu->addAction(QIcon(), tr("Select Object"), this, &FdmMainWidget::saveMesh);
    //m_exportMenu->addAction(QIcon(), tr("G-code"));
    m_actionFileSave = m_menuFile->addAction(QIcon(), tr("Save"), this, &FdmMainWidget::slotSaveProject);
    m_actionFileSaveAs = m_menuFile->addAction(QIcon(""), tr("Save As"), this, &FdmMainWidget::slotSaveAsProject);
    m_controlManager->addMenuToToolBar(m_menuFile);


    m_menuEdit = m_titleBar->addMenu(tr("Edit"));
    m_undo = m_menuEdit->addAction(QIcon(), tr("Undo"));
    m_redo = m_menuEdit->addAction(QIcon(), tr("Redo"));
    m_undo->setVisible(false);
    m_redo->setVisible(false);
    m_selPro = m_menuEdit->addAction(QIcon(), tr("Select Project"), this, &FdmMainWidget::slotSelectAllObjects);
    m_copyAction = m_menuEdit->addAction(QIcon(""), tr("Copy"), this, &FdmMainWidget::slotCopyObject);
    m_deleteMenu = m_menuEdit->addMenu(QIcon(), tr("Delete"));
    m_deleteSelectedObjectAction = m_deleteMenu->addAction(QIcon(), tr("Select Object"), this, &FdmMainWidget::deleteSelectedObjs);
    m_deleteSelectedObjectAction->setShortcut(QKeySequence::Delete);
    m_deleteAllObjectAction = m_deleteMenu->addAction(QIcon(), tr("All Objects"), this, &FdmMainWidget::deleteAllObject);
    m_resetMenu = m_menuEdit->addMenu(QIcon(), tr("Revert to Original"));
    m_resetSelectedAction = m_resetMenu->addAction(QIcon(), tr("Select Object"), this, &FdmMainWidget::resetSelectedObjs);
    m_resetAllAction = m_resetMenu->addAction(QIcon(), tr("All Objects"), this, &FdmMainWidget::resetAllMeshModels);
    m_controlManager->addMenuToToolBar(m_menuEdit);

    m_menuSetting = m_titleBar->addMenu(tr("Settings"));
    //QMenu* menuSetting= new QMenu(tr("Setting"));
   // menuSetting->setIcon(QIcon(":/images/icon/fdm_tool_setting_icon_n.png"));
    //     QAction* languageAction = menuSetting->addAction(QIcon(), tr("Language"), this, &FdmMainWidget::openPreferencesDialog);
    //     languageAction->setObjectName("General");
    // QMenu* prefereMenu = menuSetting->addMenu(QIcon(), tr("Preferences"));
    m_generalAction = m_menuSetting->addAction(QIcon(), tr("General"), this, &FdmMainWidget::openPreferencesDialog);
    m_generalAction->setObjectName("General");
    m_machineAction = m_menuSetting->addAction(QIcon(), tr("Machine"), this, &FdmMainWidget::openPreferencesDialog);
    m_machineAction->setObjectName("Machine");
    m_materialAction = m_menuSetting->addAction(QIcon(), tr("Material"), this, &FdmMainWidget::openPreferencesDialog);
    m_materialAction->setObjectName("Material");
    m_parametersAction = m_menuSetting->addAction(QIcon(), tr("Parameters"), this, &FdmMainWidget::openPreferencesDialog);
    m_parametersAction->setObjectName("Parameters");
    m_controlManager->addMenuToToolBar(m_menuSetting);
    m_controlManager->addSeparatorToToolBar();

    m_actionSave = new QAction(QIcon(":/images/icon/fdm_tool_save_icon_n.png"), tr("Save"));
    connect(m_actionSave,&QAction::triggered,this, &FdmMainWidget::slotSaveProject);

    // QAction *actionUndo = new QAction(QIcon(":/images/icon/fdm_tool_undo_icon_n.png"), tr("Undo"));
    // QAction *actionRedo = new QAction(QIcon(":/images/icon/fdm_tool_redo_icon_n.png"), tr("Redo"));

    m_controlManager->addActionToToolBar(m_actionSave);
   //  m_controlManager->addActionToToolBar(actionUndo);
   //  m_controlManager->addActionToToolBar(actionRedo);

     m_controlManager->addStructToToolBar();
     m_menuHelp = m_titleBar->addMenu(tr("Help"));
     //QMenu* menuHelp= new QMenu(tr("Help"));
     //menuHelp->setIcon(QIcon(":/images/icon/fdm_tool_help_icon_n.png"));
     m_tutorial = m_menuHelp->addAction(QIcon(), tr("Tutorial"),[=]{
         const QUrl regUrl(QLatin1String("https://support.ankermake.com"));
         QDesktopServices::openUrl(regUrl);
     });
     m_feedback = m_menuHelp->addAction(QIcon(), tr("Feedback"));

     //menuHelp->addAction(QIcon(), tr("Software Update"));
     m_actionUpdate = m_menuHelp->addAction(QIcon(), tr("Software Update"),this,&FdmMainWidget::openPreferencesDialog);
     m_actionUpdate->setObjectName("actionUpdate");
     m_controlManager->addMenuToToolBar(m_menuHelp,Qt::AlignRight);
     m_priPolicy = m_menuHelp->addAction(QIcon(), tr("Privacy Policy"),[=]() {
         const QUrl regUrl(QLatin1String("https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-privacy.en.html"));
         QDesktopServices::openUrl(regUrl);
     });
     m_termsOfUse = m_menuHelp->addAction(QIcon(), tr("Terms of Use"),[=] {
         const QUrl regUrl(QLatin1String("https://public-make-moat-us.s3.us-east-2.amazonaws.com/overall/AnkerMake-terms-of-service.en.html"));
         QDesktopServices::openUrl(regUrl);
     });
     m_copyright = m_menuHelp->addAction(QIcon(), tr("Copyright Information"),this, &FdmMainWidget::openCopyrightWidget);

//     QMenu* menuAccount= new QMenu(tr("Account"));
//     menuAccount->setIcon(QIcon(":/images/icon/fdm_tool_account_icon_n.png"));
//     // menu->addAction(QIcon(":/images/icon/menu_account.png"), tr(" Account"));
//     menuAccount->addAction(QIcon(), tr("My Account"), this, &FdmMainWidget::loginWidgetDisplaySlot);
     //m_controlManager->addMenuToToolBar(menuAccount,Qt::AlignRight);
    // QAction *accountAction = new QAction(tr("Account"));
    // m_titleBar->addAction(accountAction);
//     m_accoutMenu = m_titleBar->addMenu(tr("Account"));
//     m_accountAction = m_accoutMenu->addAction(QIcon(), tr("Account"));
     connect(m_accountAction,&QAction::triggered,this, &FdmMainWidget::loginWidgetDisplaySlot);
     m_controlManager->addAction(m_accountAction,Qt::AlignRight);

     m_actionLeftOpen = new QAction(QIcon(":/images/icon/fdm_leftTool_open_icon_n.png"), tr("Open"));
     connect(m_actionLeftOpen,&QAction::triggered,this, &FdmMainWidget::open);
     m_controlManager->addActionToModelTranform(m_actionLeftOpen);

     m_controlManager->addSeparatorToModelTranform();

     m_exportAllObjectAction->setEnabled(false);
     m_deleteAllObjectAction->setEnabled(false);
     m_resetAllAction->setEnabled(false);
}

void FdmMainWidget::initScene(QMdiArea* mdiarea)
{
    
    // QMdiArea* mdiarea = new QMdiArea();
    CHChildDocWindow *docWindow = new CHChildDocWindow(mdiarea);
    curScene = docWindow;  
    mdiarea->addSubWindow(docWindow, Qt::FramelessWindowHint);

    QHBoxLayout* layout = new QHBoxLayout(mdiarea);
    layout->addWidget(docWindow);
    
    layout->setContentsMargins(0, 0, 0, 0);
    m_controlManager->setTopViewGlarea(docWindow);
    mdiarea->show();
    mdiarea->activatePreviousSubWindow();

    
    QObject::connect(docWindow->m_doc.get(), &CHDoc::modelObjsStatusChanged, m_messageProcessing,
                     &MessageProcessing::receiveModelObjsStatusChangedFromDoc);
    QObject::connect(docWindow->m_doc.get(), &CHDoc::modelTransformChanged, m_messageProcessing,
                     &MessageProcessing::receiveModelTransformChangedFromDoc);
    QObject::connect(docWindow->m_doc.get(), &CHDoc::modelOutOfRangeChanged, m_messageProcessing,
                     &MessageProcessing::receiveModelOutOfRangeChangedFromDoc);
    QObject::connect(docWindow->m_doc.get(), &CHDoc::visibleModelCountChanged, m_messageProcessing,
                     &MessageProcessing::receiveVisibleModelCountChangedFromDoc);
    QObject::connect(docWindow->m_doc.get(), &CHDoc::modelSupportNumberChanged, m_messageProcessing,
                     &MessageProcessing::receiveModelSupportNumberChangedFromDoc);

    
    //  m_meshSuffixList << "obj" << "stl";
    connect(docWindow->m_pickCommand.get(), &CHPickOperationCommand::selectedObjsChanged,
            this, &FdmMainWidget::setButtonEnableByPickStatus);
    connect(docWindow->m_doc.get(), &CHDoc::modelObjsStatusChanged,
            this, &FdmMainWidget::setButtonEnableByDocument);

    connect(docWindow, &CHChildDocWindow::openFiles, this, &FdmMainWidget::fdmOpeFileList);

    connect(getGlobalPick().get(), SIGNAL(resetSeletedObjsSig()), getGlobalPick().get(), SLOT(resetSelectedObjs()));
}


void FdmMainWidget::setMainWindowId(WId id)
{
    m_mainWinId = id;
}

void FdmMainWidget::createRecentProject(const QStringList &fileList, QMenu *recentMenu)
{
    if (fileList.isEmpty()) {
        recentMenu->setEnabled(false);
        return;
    }
    else {
        recentMenu->setEnabled(true);
    }
    int num = 0;
    for (const auto& file : fileList) {
        num++;
        QAction* action = new QAction();
        connect(action, &QAction::triggered, this, &FdmMainWidget::openRecent);
        QString fileName = QFileInfo(file).fileName();
        QFontMetrics fontMetrics(action->font());
        if(fontMetrics.width(fileName) > 188) {
            fileName = fontMetrics.elidedText(fileName,Qt::ElideMiddle,188);
        }
        action->setText(fileName);
        // qDebug() << " set data = " << QFileInfo(file).fileName();
        action->setData(file);
       // action->
        action->setIcon(QIcon(":/images/icon/fdm_recentFile_icon.png"));
        recentMenu->addAction(action);
        recentMenu->addSeparator();
    }
    QAction* clearAction = new QAction(tr("Clear"));
    connect(clearAction, &QAction::triggered, this, &FdmMainWidget::clearRecentProjectList);
    recentMenu->addAction(clearAction);
}

void FdmMainWidget::insertRecent(const QStringList &fileList)
{
    if (fileList.isEmpty()) {
        return;
    }
    m_settings.insertRecent(fileList);
    m_recentMenu->clear();
    createRecentProject(m_settings.readRecent(), m_recentMenu);
}

void FdmMainWidget::setActionEnble(bool enble)
{
    m_exportSelectObjectAction->setEnabled(enble);
    m_copyAction->setEnabled(enble);
    m_undo->setEnabled(enble);
    m_redo->setEnabled(enble);
    m_deleteSelectedObjectAction->setEnabled(enble);
    m_resetSelectedAction->setEnabled(enble);
}


void FdmMainWidget::initViewTransformAction()
{
    QAction* viewAction = new QAction(QIcon(":/images/icon/fdm_full_icon_n.png"), tr("3D View"));
    connect(viewAction, &QAction::toggled, this, &FdmMainWidget::viewChanged);
    m_viewTransform.append(viewAction);

    QAction* frontAction = new QAction(QIcon(":/images/icon/fdm_front_icon_n.png"), tr("Front View"));
    connect(frontAction, &QAction::toggled, this, &FdmMainWidget::frontViewChanged);
    m_viewTransform.append(frontAction);

    QAction* rearAction = new QAction(QIcon(":/images/icon/fdm_rear_icon_n.png"), tr("Rear View"));
    connect(rearAction, &QAction::toggled, this, &FdmMainWidget::rearViewChanged);
    m_viewTransform.append(rearAction);

    QAction* leftAction = new QAction(QIcon(":/images/icon/fdm_left_icon_n.png"), tr("Left View"));
    connect(leftAction, &QAction::toggled, this, &FdmMainWidget::leftViewChanged);
    m_viewTransform.append(leftAction);

    QAction* rightAction = new QAction(QIcon(":/images/icon/fdm_right_icon_n.png"), tr("Right View"));
    connect(rightAction, &QAction::toggled, this, &FdmMainWidget::rightViewChanged);
    m_viewTransform.append(rightAction);

    QAction* topAction = new QAction(QIcon(":/images/icon/fdm_on_icon_n.png"), tr("Top View"));
    connect(topAction, &QAction::toggled, this, &FdmMainWidget::topViewChanged);
    m_viewTransform.append(topAction);

    QAction* bottomAction = new QAction(QIcon(":/images/icon/fdm_bottom_icon_n.png"), tr("Bottom View"));
    connect(bottomAction, &QAction::toggled, this, &FdmMainWidget::bottomViewChanged);
    m_viewTransform.append(bottomAction);

    for (auto action : m_viewTransform) {
        action->setCheckable(true);
        action->setChecked(false);
        m_controlManager->addActionToViewTransform(action);
    }
}



void FdmMainWidget::open()
{
    emit fdmOpenFile();
}

void FdmMainWidget::saveAllMesh()
{
    emit fdmSaveAllMesh();
}

void FdmMainWidget::saveMesh()
{
    emit fdmSaveMesh();
}

void FdmMainWidget::slotSaveProject()
{
    emit fdmSaveProject();
}

void FdmMainWidget::slotSaveAsProject()
{
    emit fdmSaveAsProject();
}

void FdmMainWidget::fdmOpeFileList(QStringList fileNameList)
{
    emit fdmOpenRecent(fileNameList);
}

void FdmMainWidget::openRecent()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QVariant data = action->data();
    if (!data.isValid()) {
        return;
    }
    QString filePath = data.toString();
    QFileInfo info(filePath);
    if(info.isDir() | info.isFile()) {
        emit fdmOpenRecent(QStringList() << data.toString());
    }
    else {
        control::MessageDialog box(tr("Notice"), tr("The file can't be found. Please check."), MessageDialog::OK, this);
        box.exec();
    }
   // openFileList(QStringList() << data.toString());
}

void FdmMainWidget::clearRecentProjectList()
{
    m_recentMenu->clear();
    m_recentMenu->setEnabled(false);
    m_settings.clearRecent();
}

void FdmMainWidget::openPreferencesDialog()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    int currentIndex = -1;
    QString text = action->objectName();
    if (text == "Machine") {
        currentIndex = AkConst::EWidgetType::Machine;
    }
    else if (text == "Material") {
        currentIndex = AkConst::EWidgetType::Material;
    }
    else if (text == "Parameters") {
        currentIndex = AkConst::EWidgetType::Parameter;
    }
    else {
        currentIndex = AkConst::EWidgetType::General;
    }
    // qDebug() << " index ==" << currentIndex;
    m_controlManager->openPreferences(currentIndex);
}

void FdmMainWidget::slotSelectAllObjects()
{
    if (curScene)
    {
        curScene->m_pickCommand->selectAllobjs();
    }
}

void FdmMainWidget::slotCopyObject()
{
    if (curScene)
    {
        
        curScene->m_pickCommand->copySelectedAndPasteObjs();
    }
}

void FdmMainWidget::deleteAllObject()
{
    getDoc()->clearDocMeshModels();
}

void FdmMainWidget::deleteSelectedObjs()
{
    auto pick = getGlobalPick();
    auto selectedObjects = pick->m_selectedObjs;
    std::vector<CH3DPrintModelPtr> meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = selectedObjects.begin(); it != selectedObjects.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
    }
    getDoc()->deleteObjs(meshes);
}

void FdmMainWidget::resetAllMeshModels()
{
    getDoc()->resetAllMeshModels();

    emit getGlobalPick()->resetSeletedObjsSig();
}

void FdmMainWidget::resetSelectedObjs()
{
    std::vector<CH3DPrintModelPtr> selectedObjs;
    getGlobalPick()->currentSelectedPrints(selectedObjs);
    getDoc()->resetObjs(selectedObjs);
    emit getGlobalPick()->resetSeletedObjsSig();
    return;
    auto pick = getGlobalPick();
    auto selectedObjects = pick->m_selectedObjs;
    std::vector<CH3DPrintModelPtr> meshes;
    for (std::set<CHMeshShowObjPtr>::iterator it = selectedObjects.begin(); it != selectedObjects.end(); it++)
    {
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*it));
    }
    getDoc()->resetObjs(meshes);
}

void FdmMainWidget::loginWidgetDisplaySlot()
{
    AkUtil::TFunction("");

    qDebug() << "loginWidgetDisplaySlot.";
    m_messageProcessing->sendMsg2NetworkLogin();



//    if(m_tabWidget->currentWidget()  && (m_tabWidget->currentWidget() != m_fdmWidget)) {
//        int current = m_tabWidget->currentIndex();
//        m_tabWidget->setCurrentIndex(0);
//        m_tabWidget->raise();
//        m_tabWidget->setCurrentIndex(current);
//        m_tabWidget->raise();
//    }
}

void FdmMainWidget::loginWidgetDisplayFeedBackSlot(qint64 cmdId, int result)
{
    qDebug() << "FeedBack Cmd Id: " << cmdId << ", result: " << result;
}

void FdmMainWidget::setButtonEnableByPickStatus()
{
    auto pick = getGlobalPick();
    auto selected = pick.get()->m_selectedObjs;
    if (selected.size() <= 0) {
        setActionEnble(false);
    }
    else {
        setActionEnble(true);
    }
}

void FdmMainWidget::setButtonEnableByDocument()
{
      auto doc = getDoc();
     //qDebug() << "doc->m_printObjs.size() = " << doc->m_printObjs.size();

    if (doc->m_printObjs.size() > 0) {
        m_exportAllObjectAction->setEnabled(true);
        m_deleteAllObjectAction->setEnabled(true);
        m_resetAllAction->setEnabled(true);
    }
    else {
        m_exportAllObjectAction->setEnabled(false);
        m_deleteAllObjectAction->setEnabled(false);
        m_resetAllAction->setEnabled(false);
    }
}

void FdmMainWidget::viewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_full_icon_s.png" : ":/images/icon/fdm_full_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::JUST_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::frontViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_front_icon_s.png" : ":/images/icon/fdm_front_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::FRONT_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::rearViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_rear_icon_s.png" : ":/images/icon/fdm_rear_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::BACK_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::leftViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_left_icon_s.png" : ":/images/icon/fdm_left_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::LEFT_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::rightViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_right_icon_s.png" : ":/images/icon/fdm_right_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::RIGHT_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::topViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_on_icon_s.png" : ":/images/icon/fdm_on_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::TOP_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

void FdmMainWidget::bottomViewChanged(bool checked)
{
    QAction* viewAction = qobject_cast<QAction*> (sender());
    viewAction->setIcon(QIcon(checked ? ":/images/icon/fdm_bottom_icon_s.png" : ":/images/icon/fdm_bottom_icon_n.png"));
    if (!checked)
    {
        return;
    }
    CHChildDocWindow* childDocWindow = (CHChildDocWindow*)(curScene);
    if (childDocWindow)
    {
        childDocWindow->setView(CH3dView::ViewType::BOTTOM_VIEW_TYPE, getDoc()->m_machineBox->m_realAABB);
    }
}

/*


*/

void FdmMainWidget::deviceActionToggled(bool checked)
{
    qint64 id = QDateTime::currentSecsSinceEpoch();
    //m_mainSocket->printCtrlWidgetDisplay(id);
    //connect(m_mainSocket, &QMainSocket2::displayAnkerMakeMainWindow, this, &FdmMainWidget::displayMainWindow);
}

void FdmMainWidget::displayMainWindow(bool showed)
{
    if(showed)
    {
        parentWidget()->showMaximized();
    }
    else
    {
        parentWidget()->hide();
    }
}

void FdmMainWidget::openCopyrightWidget()
{
    CopyRightWidget *copyWidget = new CopyRightWidget(this);
    copyWidget->setAttribute(Qt::WA_DeleteOnClose);
    copyWidget->exec();
}

void FdmMainWidget::tabCurrentPageChanged(int index)
{
    AkUtil::TFunction("");
    AkUtil::TDebug(QString("m_tabWidget index =").arg(index));
    if(index < 0 || (index >= m_tabWidget->count())) {
        return;
    }
    switch (index) {
    case (FDMWIDGET):
        m_menuEdit->setEnabled(true);
        m_menuSetting->setEnabled(true);
        break;
    case (GCODEWIDGET):

    default:
        m_menuEdit->setEnabled(false);
        m_menuSetting->setEnabled(false);
        break;
    }
   // qDebug() << " current ===" << m_tabWidget->currentIndex();
}

void FdmMainWidget::currentPageChanged(int index)
{
    AkUtil::TFunction("");
    AkUtil::TDebug(QString("m_tabWidget index =").arg(index));
    if(index < 0 || (index >= m_tabWidget->count())) {
        return;
    }
    switch (index) {
    case (FDMWIDGET):
        if(m_fdmWidget) {
            m_tabWidget->setCurrentWidget(m_fdmWidget);
        }
        break;
    case (GCODEWIDGET):
        if(m_gcodeWidget) {
            m_tabWidget->setCurrentWidget(m_gcodeWidget);
        }
        break;

    default:
        if(m_netWorkWidget) {
            m_tabWidget->setCurrentWidget(m_netWorkWidget);
        }
        break;
    }
    qDebug() << " current ===" << m_tabWidget->currentIndex();
}


void FdmMainWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QWidget::changeEvent(e);
}

void FdmMainWidget::retranslateUi()
{
    if (m_menuFile != nullptr) {
        m_menuFile->setTitle(tr("File"));
        if (m_actionOpen != nullptr) {
            m_actionOpen->setText(tr("Open"));
        }
        if (m_actionFileSave != nullptr) {
            m_actionFileSave->setText(tr("Save"));
        }
        if (m_actionFileSaveAs != nullptr) {
            m_actionFileSaveAs->setText(tr("Save as"));
        }
        if (m_recentMenu != nullptr) {
            m_recentMenu->setTitle(tr("Open Recent..."));
        }
        if (m_exportMenu != nullptr) {
            m_exportMenu->setTitle(tr("Export..."));
        }
        if (m_exportAllObjectAction != nullptr) {
            m_exportAllObjectAction->setText(tr("All Object"));
        }
        if (m_exportSelectObjectAction != nullptr) {
            m_exportSelectObjectAction->setText(tr("Select Object"));
        }
    }
    if (m_menuEdit != nullptr) {
        m_menuEdit->setTitle(tr("Edit"));
    }
    if (m_undo != nullptr) {
        m_undo->setText(tr("Undo"));
    }
    if (m_redo != nullptr) {
        m_redo->setText(tr("Redo"));
    }
    if (m_selPro != nullptr) {
        m_selPro->setText(tr("Select Project..."));
    }
    if (m_copyAction != nullptr) {
        m_copyAction->setText(tr("Copy"));
    }
    if (m_deleteMenu != nullptr) {
        m_deleteMenu->setTitle(tr("Delete.."));
    }
    if (m_deleteSelectedObjectAction != nullptr) {
        m_deleteSelectedObjectAction->setText(tr("Select Object"));
    }
    if (m_deleteAllObjectAction != nullptr) {
        m_deleteAllObjectAction->setText(tr("All Objects"));
    }
    if (m_resetMenu != nullptr) {
        m_resetMenu->setTitle(tr("Reset Transformations"));
    }
    if (m_resetSelectedAction != nullptr) {
        m_resetSelectedAction->setText(tr("Select Object"));
    }
    if (m_resetAllAction != nullptr) {
        m_resetAllAction->setText(tr("All Objects"));
    }
    if (m_menuSetting != nullptr) {
        m_menuSetting->setTitle(tr("Setting"));
    }
    if (m_actionSave != nullptr) {
        m_actionSave->setText(tr("Save"));
    }
    if (m_menuHelp != nullptr) {
        m_menuHelp->setTitle(tr("Help"));
    }
    if (m_tutorial != nullptr) {
        m_tutorial->setText(tr("Tutorial"));
    }
    if (m_feedback != nullptr) {
        m_feedback->setText(tr("Feedback"));
    }
    if (m_actionUpdate != nullptr) {
        m_actionUpdate->setText(tr("Software Update"));
    }
    if (m_priPolicy != nullptr) {
        m_priPolicy->setText(tr("Privacy Policy"));
    }
    if (m_termsOfUse != nullptr) {
        m_termsOfUse->setText(tr("Terms of Use"));
    }
    if (m_copyright != nullptr) {
        m_copyright->setText(tr("Copyright Information"));
    }
    if (m_accountAction != nullptr) {
        m_accountAction->setText(tr("Account"));
    }
    if (m_actionLeftOpen != nullptr) {
        m_actionLeftOpen->setText(tr("Open"));
    }
    if (m_generalAction != nullptr) {
        m_generalAction->setText(tr("General"));
    }
    if (m_machineAction != nullptr) {
        m_machineAction->setText(tr("Machine"));
    }
    if (m_materialAction != nullptr) {
        m_materialAction->setText(tr("Material"));
    }
    if (m_parametersAction != nullptr) {
        m_parametersAction->setText(tr("Parameters"));
    }
    if (m_accoutMenu != nullptr) {
        m_accoutMenu->setTitle(tr("Account"));
    }
}

}
