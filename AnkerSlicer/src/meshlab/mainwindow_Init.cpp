/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/


#include <QToolBar>
#include <QProgressBar>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileOpenEvent>
#include <QFile>
#include <QtXml>
#include <QSysInfo>
#include <QDesktopServices>
#include <QStatusBar>
#include <QMenuBar>
#include <QWidgetAction>
#include <QMessageBox>
#include "mainwindow.h"
#include <common/searcher.h>
#include <common/mlapplication.h>
#include <common/mlexception.h>
#include <common/globals.h>
#include "dialogs/options_dialog.h"
#include "dialogs/save_snapshot_dialog.h"
#include "dialogs/congrats_dialog.h"
#include "message/messageprocessing.h"  //  @2021-12-13 by CL
#include "NewRender/CHChildDocWindow.h"

QProgressBar *MainWindow::qb;


MainWindow::MainWindow():
    //httpReq(this),
    gpumeminfo(NULL),
    defaultGlobalParams(meshlab::defaultGlobalParameterList()),
    lastUsedDirectory(QDir::home()),
    PM(meshlab::pluginManagerInstance()),
    _currviewcontainer(NULL)
{
    QSettings settings;
    //toDelete plugins, flagged in the last session
    //this is needed on windows, since it refuses to delete dll plugins while meshlab is
    //running. Therefore, in these cases plugins that are going to be deleted are
    //saved in this list, and the files are removed in the next meshlab session,
    //BEFORE they will be loaded. Therefore, these lines need to be executed
    //before the PM.loadPlugins call.
    QStringList toDeletePlugins = settings.value("ToDeletePlugins").value<QStringList>();
    if (!toDeletePlugins.isEmpty()){
        for (const QString& file : toDeletePlugins){
            QFile::remove(file);
        }
    }
    settings.remove("ToDeletePlugins");

    setContextMenuPolicy(Qt::NoContextMenu);

    
    //workspace = new QWorkspace(this);
    mdiarea = new QMdiArea();
	CHChildDocWindow* twww = new CHChildDocWindow(mdiarea);
    curScene = twww;
	mdiarea->addSubWindow(twww, Qt::FramelessWindowHint);



    //mdiarea->setViewMode(QMdiArea::TabbedView);
    layerDialog = new LayerDialog();
    connect(layerDialog, SIGNAL(toBeShow()), this, SLOT(updateLayerDialog()));
    layerDialog->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    layerDialog->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    //delete by Alice
    //addDockWidget(Qt::RightDockWidgetArea, layerDialog);

    
    try {
        PM.loadPlugins();
        PM.loadPlugins(QDir(MeshLabApplication::extraPluginsLocation()));
    }
    catch (const MLException& e) {
        QMessageBox::warning(this, "Error while loading plugins.", e.what());
    }
    //qDebug()<<"id_PM_Init:"<<&PM;
    //disable previously disabled plugins
    QStringList disabledPlugins = settings.value("DisabledPlugins").value<QStringList>();
    for (MeshLabPlugin* fp : PM.pluginIterator(true)){
        if (disabledPlugins.contains(fp->pluginName()))
            PM.disablePlugin(fp);
    }

    
    {   // @2021-12-13 by CL
         messageProcessing = new MessageProcessing(this);
         QObject::connect(messageProcessing, &MessageProcessing::sendMsg2Manager, &PM, &PluginManager::recMsgfromPlugin);
         QObject::connect(&PM, &PluginManager::sendMsg2MessageProcessing, messageProcessing,  &MessageProcessing::recMsgfromManager);

         
         QObject::connect(curScene->m_doc.get(), &CHDoc::modelObjsStatusChanged, messageProcessing,
             &MessageProcessing::receiveModelObjsStatusChangedFromDoc);
		 QObject::connect(curScene->m_doc.get(), &CHDoc::modelTransformChanged, messageProcessing,
             &MessageProcessing::receiveModelTransformChangedFromDoc);
		 QObject::connect(curScene->m_doc.get(), &CHDoc::modelOutOfRangeChanged, messageProcessing,
             &MessageProcessing::receiveModelOutOfRangeChangedFromDoc);
         QObject::connect(curScene->m_doc.get(), &CHDoc::visibleModelCountChanged, messageProcessing,
             &MessageProcessing::receiveVisibleModelCountChangedFromDoc);
    }

    //setCentralWidget(workspace);
    //setCentralWidget(mdiarea);
    windowMapper = new QSignalMapper(this);
    // Permette di passare da una finestra all'altra e tenere aggiornato il workspace
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(wrapSetActiveSubWindow(QWidget *)));
    // Quando si passa da una finestra all'altra aggiorna lo stato delle toolbar e dei menu
    connect(mdiarea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(switchCurrentContainer(QMdiSubWindow *)));
    //httpReq = new QNetworkAccessManager(this);
    //connect(&httpReq, SIGNAL(finished(QNetworkReply*)), this, SLOT(connectionDone(QNetworkReply*)));

    
    QIcon icon;
    icon.addPixmap(QPixmap(":/logo_ankerslicer.png"));
    setWindowIcon(icon); 

    QVariant vers = settings.value(MeshLabApplication::versionRegisterKeyName());
    //should update those values only after I run MeshLab for the very first time or after I installed a new version
    if (!vers.isValid() || vers.toString() < MeshLabApplication::appVer())
    {
        settings.setValue(MeshLabApplication::pluginsPathRegisterKeyName(), meshlab::defaultPluginPath());
        settings.setValue(MeshLabApplication::versionRegisterKeyName(), MeshLabApplication::appVer());
        settings.setValue(MeshLabApplication::wordSizeKeyName(), QSysInfo::WordSize);
    }

    //load default params from plugins
    loadDefaultSettingsFromPlugins();
    // Now load from the registry the settings and  merge the hardwired values
    // got from the PM.loadPlugins with the ones found in the registry.
    loadMeshLabSettings();
    mwsettings.updateGlobalParameterList(currentGlobalParams);
    createActions();
    createToolBars();
    createMenus();
    gpumeminfo = new vcg::QtThreadSafeMemoryInfo(mwsettings.maxgpumem);
    stddialog = 0;
    setAcceptDrops(true);
    //mdiarea->setAcceptDrops(true);

    
    setWindowTitle(MeshLabApplication::shortName());

    //setStatusBar(new QStatusBar(this));
    //globalStatusBar() = statusBar();
    qb = new QProgressBar();
    qb->setMaximum(100);
    qb->setMinimum(0);
    qb->reset();
    //statusBar()->addPermanentWidget(qb, 0);

    nvgpumeminfo = new QProgressBar();
    nvgpumeminfo->setStyleSheet(" QProgressBar { background-color: #d0d0d0; border: 2px solid grey; border-radius: 0px; text-align: center; }"
                                " QProgressBar::chunk {background-color: #80c080; width: 1px;}");
    //statusBar()->addPermanentWidget(nvgpumeminfo, 0);
    newProject();
    updateCustomSettings();

    connect(this, SIGNAL(updateLayerTable()), this, SLOT(updateLayerDialog()));
    connect(layerDialog, SIGNAL(removeDecoratorRequested(QAction*)), this, SLOT(switchOffDecorator(QAction*)));

    
//    //add by chuck
//
//#if 1

//    for(auto * editPlugin : PM.getAnkerEditPlugins()){
//        connect(getGlobalPick().get(), &CHPickOperationCommand::selectedObjsChanged, editPlugin, &AnkerEditPlugin::setEditToolsEnable);
//    }

//#endif
}

MainWindow::~MainWindow()
{
    delete gpumeminfo;
    clearToolBarAndMenuBar();
}

void MainWindow::createActions()
{
    searchShortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    searchShortCut->setAutoRepeat(false);
    searchShortCut->setContext(Qt::ApplicationShortcut); 
    //////////////Action Menu File ////////////////////////////////////////////////////////////////////////////
    newProjectAct = new QAction(QIcon(":/images/new_project.png"), QString("New Empty Project..."), this);
    newProjectAct->setShortcutContext(Qt::ApplicationShortcut);
    newProjectAct->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(newProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));

    openProjectAct = new QAction(QIcon(":/images/open_project.png"), QString("&Open project..."), this);
    openProjectAct->setShortcutContext(Qt::ApplicationShortcut);
    openProjectAct->setShortcut(Qt::CTRL + Qt::Key_O);
  //  connect(openProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));

    appendProjectAct = new QAction(QString("Append project to current..."), this);
    connect(appendProjectAct, SIGNAL(triggered()), this, SLOT(appendProject()));

    saveProjectAct = new QAction(QIcon(":/images/save.png"), QString("&Save Project As..."), this);
    saveProjectAct->setShortcutContext(Qt::ApplicationShortcut);
    saveProjectAct->setShortcut(Qt::CTRL + Qt::Key_S);
    //connect(saveProjectAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    closeProjectAct = new QAction(QString("Close Project"), this);
    //closeProjectAct->setShortcutContext(Qt::ApplicationShortcut);
    //closeAct->setShortcut(Qt::CTRL+Qt::Key_C);
    connect(closeProjectAct, SIGNAL(triggered()), mdiarea, SLOT(closeActiveSubWindow()));
    importMeshAct = new QAction(QIcon(":/images/import_mesh.png"), "&Import Mesh...", this);
    importMeshAct->setShortcutContext(Qt::ApplicationShortcut);
    importMeshAct->setShortcut(Qt::CTRL + Qt::Key_I);
    //connect(importMeshAct, SIGNAL(triggered()), this, SLOT(importMeshWithLayerManagement()));

    exportMeshAct = new QAction(QIcon(":/images/save.png"), "&Export Mesh...", this);
    exportMeshAct->setShortcutContext(Qt::ApplicationShortcut);
    exportMeshAct->setShortcut(Qt::CTRL + Qt::Key_E);
    //connect(exportMeshAct, SIGNAL(triggered()), this, SLOT(save()));

    exportMeshAsAct = new QAction(QIcon(":/images/save.png"), "&Export Mesh As...", this);
    exportMeshAsAct->setShortcutContext(Qt::ApplicationShortcut);
    exportMeshAsAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_E);
    //connect(exportMeshAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    reloadMeshAct = new QAction(QIcon(":/images/reload.png"), "&Reload", this);
    reloadMeshAct->setShortcutContext(Qt::ApplicationShortcut);
    reloadMeshAct->setShortcut(Qt::ALT + Qt::Key_R);
    reloadMeshAct->setToolTip("Reload Current Layer");
    connect(reloadMeshAct, SIGNAL(triggered()), this, SLOT(reload()));

    reloadAllMeshAct = new QAction(QIcon(":/images/reload_all.png"), "&Reload All", this);
    reloadAllMeshAct->setShortcutContext(Qt::ApplicationShortcut);
    reloadAllMeshAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    reloadAllMeshAct->setToolTip("Reload All Layers");
    connect(reloadAllMeshAct, SIGNAL(triggered()), this, SLOT(reloadAllMesh()));

    importRasterAct = new QAction(QIcon(":/images/open.png"), QString("Import Raster..."), this);
    connect(importRasterAct, SIGNAL(triggered()), this, SLOT(importRaster()));

    saveSnapshotAct = new QAction(QIcon(":/images/snapshot.png"), QString("Save snapsho&t"), this);
    connect(saveSnapshotAct, SIGNAL(triggered()), this, SLOT(saveSnapshot()));

    for (int i = 0; i < MAXRECENTFILES; ++i)
    {
        recentProjActs[i] = new QAction(this);
        recentProjActs[i]->setVisible(true);
        recentProjActs[i]->setEnabled(true);
        recentProjActs[i]->setShortcut(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::Key_1 + i));

        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(true);
        recentFileActs[i]->setEnabled(true);
        recentFileActs[i]->setShortcutContext(Qt::ApplicationShortcut);
        recentFileActs[i]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1 + i));
        connect(recentProjActs[i], SIGNAL(triggered()), this, SLOT(openRecentProj()));
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentMesh()));
    }

    exitAct = new QAction(QString("E&xit"), this);
    exitAct->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    //////////////Render Actions for Toolbar and Menu /////////////////////////////////////////////////////////


    //renderModeTextureWedgeAct  = new RenderModeTexturePerWedgeAction(this);
    //renderModeTextureWedgeAct->setCheckable(true);
    //rendlist.push_back(renderModeTextureWedgeAct);



/* QList<RenderModeAction*> rendlist;

setLightAct	  = new RenderModeLightOnOffAction(this);
setLightAct->setCheckable(true);
rendlist.push_back(setLightAct);

setDoubleLightingAct = new RenderModeDoubleLightingAction(this);
setDoubleLightingAct->setCheckable(true);
setDoubleLightingAct->setShortcutContext(Qt::ApplicationShortcut);
setDoubleLightingAct->setShortcut(Qt::CTRL+Qt::Key_D);
rendlist.push_back(setDoubleLightingAct);

setFancyLightingAct   = new RenderModeFancyLightingAction(this);
setFancyLightingAct->setCheckable(true);
setFancyLightingAct->setShortcutContext(Qt::ApplicationShortcut);
setFancyLightingAct->setShortcut(Qt::CTRL+Qt::Key_Y);
rendlist.push_back(setFancyLightingAct);

backFaceCullAct 	  = new RenderModeFaceCullAction(this);
backFaceCullAct->setCheckable(true);
backFaceCullAct->setShortcutContext(Qt::ApplicationShortcut);
backFaceCullAct->setShortcut(Qt::CTRL+Qt::Key_K);
rendlist.push_back(backFaceCullAct);

connectRenderModeActionList(rendlist);*/

//////////////Action Menu View ////////////////////////////////////////////////////////////////////////////
    fullScreenAct = new QAction("&FullScreen", this);
    fullScreenAct->setCheckable(true);
    fullScreenAct->setShortcutContext(Qt::ApplicationShortcut);
    fullScreenAct->setShortcut(Qt::ALT + Qt::Key_Return);
    connect(fullScreenAct, SIGNAL(triggered()), this, SLOT(fullScreen()));

    showToolbarStandardAct = new QAction(QString("&Standard"), this);
    showToolbarStandardAct->setCheckable(true);
    showToolbarStandardAct->setChecked(true);
    connect(showToolbarStandardAct, SIGNAL(triggered()), this, SLOT(showToolbarFile()));

    showInfoPaneAct = new QAction(QString("Show Info &Panel"), this);
    showInfoPaneAct->setCheckable(true);
    connect(showInfoPaneAct, SIGNAL(triggered()), this, SLOT(showInfoPane()));


    showTrackBallAct = new QAction(QString("Show &Trackball"), this);
    showTrackBallAct->setCheckable(true);
    showTrackBallAct->setShortcut(Qt::SHIFT + Qt::Key_H);
    connect(showTrackBallAct, SIGNAL(triggered()), this, SLOT(showTrackBall()));

    resetTrackBallAct = new QAction(QString("Reset &Trackball"), this);
    resetTrackBallAct->setShortcutContext(Qt::ApplicationShortcut);
#if defined(Q_OS_MAC)
    resetTrackBallAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_H);
#else
    resetTrackBallAct->setShortcut(Qt::CTRL + Qt::Key_H);
#endif
    connect(resetTrackBallAct, SIGNAL(triggered()), this, SLOT(resetTrackBall()));

    showLayerDlgAct = new QAction(QIcon(":/images/layers.png"), QString("Show Layer Dialog"), this);
    showLayerDlgAct->setCheckable(true);
    showLayerDlgAct->setChecked(true);
    showLayerDlgAct->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(showLayerDlgAct, SIGNAL(triggered(bool)), this, SLOT(showLayerDlg(bool)));


    showRasterAct = new QAction(QIcon(":/images/view_raster.png"), QString("Show Current Raster Mode"), this);
    showRasterAct->setCheckable(true);
    showRasterAct->setChecked(true);
    showRasterAct->setShortcut(Qt::SHIFT + Qt::Key_R);
    connect(showRasterAct, SIGNAL(triggered()), this, SLOT(showRaster()));

    //////////////Action Menu EDIT /////////////////////////////////////////////////////////////////////////
    suspendEditModeAct = new QAction(QIcon(":/images/no_edit.png"), QString("Not editing"), this);
    suspendEditModeAct->setShortcut(Qt::Key_Escape);
    suspendEditModeAct->setCheckable(true);
    suspendEditModeAct->setChecked(true);
    connect(suspendEditModeAct, SIGNAL(triggered()), this, SLOT(suspendEditMode()));

    //////////////Action Menu WINDOWS /////////////////////////////////////////////////////////////////////////
    windowsTileAct = new QAction(QString("&Tile"), this);
    connect(windowsTileAct, SIGNAL(triggered()), mdiarea, SLOT(tileSubWindows()));

    windowsCascadeAct = new QAction("&Cascade", this);
    connect(windowsCascadeAct, SIGNAL(triggered()), mdiarea, SLOT(cascadeSubWindows()));

    windowsNextAct = new QAction("&Next", this);
    connect(windowsNextAct, SIGNAL(triggered()), mdiarea, SLOT(activateNextSubWindow()));

    closeAllAct = new QAction(QString("Close &All Windows"), this);
    connect(closeAllAct, SIGNAL(triggered()), mdiarea, SLOT(closeAllSubWindows()));

    setSplitGroupAct = new QActionGroup(this);	setSplitGroupAct->setExclusive(true);

    setSplitHAct = new QAction(QIcon(":/images/splitH.png"), "&Horizontally", setSplitGroupAct);
    setSplitVAct = new QAction(QIcon(":/images/splitV.png"), QString("&Vertically"), setSplitGroupAct);

    connect(setSplitGroupAct, SIGNAL(triggered(QAction *)), this, SLOT(setSplit(QAction *)));

    setUnsplitAct = new QAction("&Close current view", this);
    connect(setUnsplitAct, SIGNAL(triggered()), this, SLOT(setUnsplit()));

    linkViewersAct = new QAction(QString("Link Viewers"), this);
    linkViewersAct->setCheckable(true);
    connect(linkViewersAct, SIGNAL(triggered()), this, SLOT(linkViewers()));

    viewFromGroupAct = new QActionGroup(this); viewFromGroupAct->setExclusive(true);
    trackballStepGroupAct = new QActionGroup(this); trackballStepGroupAct->setExclusive(true);

    viewFrontAct = new QAction(QString("Front"), viewFromGroupAct);
    viewBackAct = new QAction(QString("Back"), viewFromGroupAct);
    viewRightAct = new QAction(QString("Right"), viewFromGroupAct);
    viewLeftAct = new QAction(QString("Left"), viewFromGroupAct);
    viewTopAct = new QAction(QString("Top"), viewFromGroupAct);
    viewBottomAct = new QAction(QString("Bottom"), viewFromGroupAct);
    // scene uses "engineering" reference system, with Z as vertical axis
    viewFrontYAct = new QAction(QString("Front (Z is up)"), viewFromGroupAct);
    viewBackYAct = new QAction(QString("Back (Z is up)"), viewFromGroupAct);
    viewRightYAct = new QAction(QString("Right (Z is up)"), viewFromGroupAct);
    viewLeftYAct = new QAction(QString("Left (Z is up)"), viewFromGroupAct);
    viewTopYAct = new QAction(QString("Top (Z is up)"), viewFromGroupAct);
    viewBottomYAct = new QAction(QString("Bottom (Z is up)"), viewFromGroupAct);
    //ortho
    toggleOrthoAct = new QAction(QString("Toggle Orthographic Camera"), this);
    toggleOrthoAct->setShortcutContext(Qt::ApplicationShortcut);
    //trackball
    trackballStepHP = new QAction(QString("Horizontal +"), trackballStepGroupAct);
    trackballStepHM = new QAction(QString("Horizontal -"), trackballStepGroupAct);
    trackballStepVP = new QAction(QString("Vertical +"), trackballStepGroupAct);
    trackballStepVM = new QAction(QString("Vertical -"), trackballStepGroupAct);
    trackballStepSP = new QAction(QString("Axial +"), trackballStepGroupAct);
    trackballStepSM = new QAction(QString("Axial -"), trackballStepGroupAct);

#ifdef WIN32 //these shortcuts work only on windows, and they result in conflicts on macos
    // keyboard shortcuts for canonical viewdirections, blender style
    viewFrontAct->setShortcut(Qt::KeypadModifier + Qt::Key_1);
    viewBackAct->setShortcut(Qt::CTRL + Qt::KeypadModifier + Qt::Key_1);
    viewRightAct->setShortcut(Qt::KeypadModifier + Qt::Key_3);
    viewLeftAct->setShortcut(Qt::CTRL + Qt::KeypadModifier + Qt::Key_3);
    viewTopAct->setShortcut(Qt::KeypadModifier + Qt::Key_7);
    viewBottomAct->setShortcut(Qt::CTRL + Qt::KeypadModifier + Qt::Key_7);
    // scene uses "engineering" reference system, with Z as vertical axis
    viewFrontYAct->setShortcut(Qt::ALT + Qt::KeypadModifier + Qt::Key_1);
    viewBackYAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::KeypadModifier + Qt::Key_1);
    viewRightYAct->setShortcut(Qt::ALT + Qt::KeypadModifier + Qt::Key_3);
    viewLeftYAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::KeypadModifier + Qt::Key_3);
    viewTopYAct->setShortcut(Qt::ALT + Qt::KeypadModifier + Qt::Key_7);
    viewBottomYAct->setShortcut(Qt::CTRL + Qt::ALT + Qt::KeypadModifier + Qt::Key_7);
    // other view-changing acts
    toggleOrthoAct->setShortcut(Qt::KeypadModifier + Qt::Key_5);
    //trackball
    trackballStepHP->setShortcut(Qt::KeypadModifier + Qt::Key_4);
    trackballStepHM->setShortcut(Qt::KeypadModifier + Qt::Key_6);
    trackballStepVP->setShortcut(Qt::KeypadModifier + Qt::Key_8);
    trackballStepVM->setShortcut(Qt::KeypadModifier + Qt::Key_2);
    trackballStepSP->setShortcut(Qt::KeypadModifier + Qt::Key_9);
    trackballStepSM->setShortcut(Qt::CTRL +  Qt::KeypadModifier + Qt::Key_9);
#endif //WIN32
    connect(viewFromGroupAct, SIGNAL(triggered(QAction*)), this, SLOT(viewFrom(QAction*)));

    connect(toggleOrthoAct, SIGNAL(triggered()), this, SLOT(toggleOrtho()));

    connect(trackballStepGroupAct, SIGNAL(triggered(QAction*)), this, SLOT(trackballStep(QAction*)));

    viewFromMeshAct = new QAction(QString("View from Mesh Camera"), this);
    viewFromRasterAct = new QAction(QString("View from Raster Camera"), this);
    viewFromRasterAct->setShortcut(Qt::CTRL + Qt::Key_J);
    readViewFromFileAct = new QAction(QString("Read camera settings from file."), this);
    readViewFromFileAct->setToolTip(QString("Restore camera settings from an XML description stored in a file."));
    saveViewToFileAct = new QAction(QString("Save camera settings to file"), this);
    saveViewToFileAct->setToolTip(QString("Save camera settings to file as XML."));
    connect(viewFromMeshAct, SIGNAL(triggered()), this, SLOT(viewFromCurrentMeshShot()));
    connect(viewFromRasterAct, SIGNAL(triggered()), this, SLOT(viewFromCurrentRasterShot()));
    connect(readViewFromFileAct, SIGNAL(triggered()), this, SLOT(readViewFromFile()));
    connect(saveViewToFileAct, SIGNAL(triggered()), this, SLOT(saveViewToFile()));

    copyShotToClipboardAct = new QAction(QString("Copy camera settings to clipboard"), this);
    copyShotToClipboardAct->setToolTip(QString("Save current camera settings to clipboard as a XML document that you can share or restore anytime."));
    connect(copyShotToClipboardAct, SIGNAL(triggered()), this, SLOT(copyViewToClipBoard()));

    pasteShotFromClipboardAct = new QAction(QString("Paste clipboard to camera settings"), this);
    pasteShotFromClipboardAct->setToolTip(QString("Restore camera settings from a XML description stored in the clipboard."));
    connect(pasteShotFromClipboardAct, SIGNAL(triggered()), this, SLOT(pasteViewFromClipboard()));

    //////////////Action Menu Filters /////////////////////////////////////////////////////////////////////
    lastFilterAct = new QAction(QString("Apply filter"), this);
    lastFilterAct->setShortcutContext(Qt::ApplicationShortcut);
    lastFilterAct->setShortcut(Qt::CTRL + Qt::Key_P);
    lastFilterAct->setEnabled(false);
    connect(lastFilterAct, SIGNAL(triggered()), this, SLOT(applyLastFilter()));

    showFilterScriptAct = new QAction(QString("Show current filter script"), this);
    showFilterScriptAct->setEnabled(false);
    connect(showFilterScriptAct, SIGNAL(triggered()), this, SLOT(showFilterScript()));

    //////////////Action Menu Preferences /////////////////////////////////////////////////////////////////////
    setCustomizeAct = new QAction("&Options...", this);
    connect(setCustomizeAct, SIGNAL(triggered()), this, SLOT(setCustomize()));

    //////////////Action Menu About ///////////////////////////////////////////////////////////////////////////
    aboutAct = new QAction("&About", this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutPluginsAct = new QAction(QString("Plugin Info"), this);
    connect(aboutPluginsAct, SIGNAL(triggered()), this, SLOT(aboutPlugins()));

    //onlineHelpAct = new QAction(QString("Online &Documentation"), this);
    //connect(onlineHelpAct, SIGNAL(triggered()), this, SLOT(helpOnline()));

    //submitBugAct = new QAction(QString("Submit Bug"), this);
    //connect(submitBugAct, SIGNAL(triggered()), this, SLOT(submitBug()));

    //onscreenHelpAct = new QAction(QString("On screen quick help"), this);
    //onscreenHelpAct->setShortcut(Qt::Key_F1);
    //onscreenHelpAct->setShortcutContext(Qt::ApplicationShortcut);
    //connect(onscreenHelpAct, SIGNAL(triggered()), this, SLOT(helpOnscreen()));

    //checkUpdatesAct = new QAction(QString("Check for updates"), this);
    //connect(checkUpdatesAct, SIGNAL(triggered()), this, SLOT(checkForUpdates()));

    ///////////////Action Menu Split/Unsplit from handle////////////////////////////////////////////////////////
    splitGroupAct = new QActionGroup(this);
    unsplitGroupAct = new QActionGroup(this);

    splitUpAct = new QAction(QString("&Up"), splitGroupAct);
    splitDownAct = new QAction("&Down", splitGroupAct);
    unsplitUpAct = new QAction(QString("&Up"), unsplitGroupAct);
    unsplitDownAct = new QAction("&Down", unsplitGroupAct);
    splitRightAct = new QAction(QString("&Right"), splitGroupAct);
    splitLeftAct = new QAction("&Left", splitGroupAct);

    unsplitRightAct = new QAction(QString("&Right"), unsplitGroupAct);
    unsplitLeftAct = new QAction("&Left", unsplitGroupAct);

    connect(splitGroupAct, SIGNAL(triggered(QAction *)), this, SLOT(splitFromHandle(QAction *)));

    connect(unsplitGroupAct, SIGNAL(triggered(QAction *)), this, SLOT(unsplitFromHandle(QAction *)));

}

void MainWindow::createToolBars()
{
#if defined(Q_OS_MAC)
    this->setStyleSheet("QToolBar {spacing: 0px; } QToolButton {border-radius: 0px;} QToolButton:checked {background: darkgray}");
#endif

    //mainToolBar = addToolBar(QString("Standard"));
    mainToolBar = new QToolBar();
    //	mainToolBar->setIconSize(QSize(32,32));
    mainToolBar->addAction(this->newProjectAct);
    mainToolBar->addAction(this->openProjectAct);
    mainToolBar->addAction(importMeshAct);
    mainToolBar->addAction(reloadMeshAct);
    mainToolBar->addAction(reloadAllMeshAct);
    mainToolBar->addAction(exportMeshAct);
    mainToolBar->addAction(saveSnapshotAct);
    mainToolBar->addAction(showLayerDlgAct);
    mainToolBar->addAction(showRasterAct);


    mainToolBar->addSeparator();
    globrendtoolbar = new MLRenderingGlobalToolbar(this);
    connect(globrendtoolbar, SIGNAL(updateRenderingDataAccordingToActions(QList<MLRenderingGlobalAction*>)), this, SLOT(updateRenderingDataAccordingToActions(QList<MLRenderingGlobalAction*>)));

    mainToolBar->addWidget(globrendtoolbar);



    //decoratorToolBar = addToolBar("Decorator");
    decoratorToolBar = new QToolBar();
    for(DecoratePlugin *iDecorate: PM.decoratePluginIterator()) {
        for(QAction *decorateAction: iDecorate->actions()) {
            if (!decorateAction->icon().isNull())
                decoratorToolBar->addAction(decorateAction);
        }
    }

    //editToolBar = addToolBar(QString("Edit"));
    editToolBar = new QToolBar();
    editToolBar->addAction(suspendEditModeAct);
    for(EditPlugin *iEditFactory: PM.editPluginFactoryIterator()) {
        for(QAction* editAction: iEditFactory->actions()){
            if (!editAction->icon().isNull()) {
                editToolBar->addAction(editAction);
            }
            else qDebug() << "action was null";
        }
    }
    editToolBar->addSeparator();


    //filterToolBar = addToolBar(QString("Filter"));
    filterToolBar = new QToolBar();
    filterToolBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    updateFilterToolBar();

    QWidget *spacerWidget = new QWidget();
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    //searchToolBar = addToolBar(QString("Search"));
    searchToolBar = new QToolBar();
    searchToolBar->addWidget(spacerWidget);
    searchToolBar->setMovable(false);
    searchToolBar->setFloatable(false);
    searchToolBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    searchButton = new MyToolButton(0, this);
    //searchButton->setPopupMode(QToolButton::InstantPopup);
    searchButton->setIcon(QIcon(":/images/search.png"));

    searchToolBar->addWidget(searchButton);
}


void MainWindow::createMenus()
{
    //////////////////// Menu File ////////////////////////////////////////////////////////////////////////////
    //fileMenu = menuBar()->addMenu(QString("&File"));
    fileMenu = new QMenu();
    fileMenu->addAction(newProjectAct);
    fileMenu->addAction(openProjectAct);
    fileMenu->addAction(appendProjectAct);
    fileMenu->addAction(saveProjectAct);
    fileMenu->addAction(closeProjectAct);
    fileMenu->addSeparator();

    fileMenu->addAction(importMeshAct);
    fileMenu->addAction(exportMeshAct);
    fileMenu->addAction(exportMeshAsAct);
    fileMenu->addAction(reloadMeshAct);
    fileMenu->addAction(reloadAllMeshAct);
    fileMenu->addSeparator();
    fileMenu->addAction(importRasterAct);
    fileMenu->addSeparator();

    fileMenu->addAction(saveSnapshotAct);
    fileMenu->addSeparator();
    recentProjMenu = fileMenu->addMenu(QString("Recent Projects"));
    recentFileMenu = fileMenu->addMenu(QString("Recent Files"));


    for (int i = 0; i < MAXRECENTFILES; ++i)
    {
        recentProjMenu->addAction(recentProjActs[i]);
        recentFileMenu->addAction(recentFileActs[i]);
    }
    //updateRecentFileActions();
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    //////////////////// Menu Edit //////////////////////////////////////////////////////////////////////////
    //editMenu = menuBar()->addMenu(QString("&Edit"));
    editMenu = new QMenu();
    fillEditMenu();

    //////////////////// Menu Filter //////////////////////////////////////////////////////////////////////////
    //filterMenu = menuBar()->addMenu(QString("Fi&lters"));
    filterMenu = new QMenu();
    fillFilterMenu();

    //////////////////// Menu Render //////////////////////////////////////////////////////////////////////////
    //renderMenu = menuBar()->addMenu(QString("&Render"));
    renderMenu = new QMenu();
    fillRenderMenu();

    // Shaders SUBmenu
    //shadersMenu = renderMenu->addMenu(QString("&Shaders"));
    //renderMenu->addSeparator();

    //////////////////// Menu View ////////////////////////////////////////////////////////////////////////////
    //viewMenu = menuBar()->addMenu(QString("&View"));
    viewMenu = new QMenu();
    viewMenu->addAction(fullScreenAct);
    viewMenu->addSeparator();
    viewMenu->addAction(showLayerDlgAct);
    viewMenu->addAction(showRasterAct);
    viewMenu->addSeparator();
    viewMenu->addAction(showTrackBallAct);
    viewMenu->addAction(resetTrackBallAct);
    viewMenu->addSeparator();
    viewMenu->addAction(toggleOrthoAct);
    viewMenu->addSeparator();
    viewMenu->addAction(showInfoPaneAct);
    viewMenu->addSeparator();
    toolBarMenu = viewMenu->addMenu(QString("&ToolBars"));
    toolBarMenu->addAction(showToolbarStandardAct);
    //connect(toolBarMenu, SIGNAL(aboutToShow()), this, SLOT(updateMenus()));

    //////////////////// Menu Windows /////////////////////////////////////////////////////////////////////////
    //windowsMenu = menuBar()->addMenu(QString("&Windows"));
    windowsMenu = new QMenu();
    windowsMenu->setToolTipsVisible(true);
    updateWindowMenu();
    menuBar()->addSeparator();

    //////////////////// Menu Preferences /////////////////////////////////////////////////////////////////////
    //preferencesMenu = menuBar()->addMenu(QString("&Tools"));
    preferencesMenu = new QMenu();
    /*preferencesMenu->addAction(showFilterEditAct);
    preferencesMenu->addSeparator();*/
    preferencesMenu->addAction(setCustomizeAct);

    //////////////////// Menu Help ////////////////////////////////////////////////////////////////
    //helpMenu = menuBar()->addMenu(QString("&Help"));
    helpMenu = new QMenu();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutPluginsAct);
    //helpMenu->addAction(onlineHelpAct);
    //helpMenu->addAction(onscreenHelpAct);
    //helpMenu->addAction(submitBugAct);
    //helpMenu->addAction(checkUpdatesAct);

    //////////////////// Menu Split/Unsplit from handle
    handleMenu = new QMenu();
    splitMenu = handleMenu->addMenu(QString("&Split"));
    unSplitMenu = handleMenu->addMenu("&Close");

    //SearchMenu
    if (searchButton != NULL)
    {
        initSearchEngine();
        int longest = longestActionWidthInAllMenus();
        searchMenu = new SearchMenu(wama, 15, searchButton, longest);
        searchButton->setMenu(searchMenu);
        connect(searchShortCut, SIGNAL(activated()), searchButton, SLOT(openMenu()));
    }
}

void MainWindow::initSearchEngine()
{
    for (const auto& p : PM.filterPluginIterator()){
        for (QAction* act : p->actions())
            initItemForSearching(act);
    }
    /*for (const auto& p : PM.editPluginFactoryIterator()){
        for (QAction* act : p->actions())
            initItemForSearching(act);
    }
    for (const auto& p : PM.renderPluginIterator()){
        for (QAction* act : p->actions())
            initItemForSearching(act);
    }*/

    initMenuForSearching(editMenu);
    initMenuForSearching(renderMenu);
}

void MainWindow::initMenuForSearching(QMenu* menu)
{
    if (menu == NULL)
        return;
    const QList<QAction*>& acts = menu->actions();
    for(QAction* act: acts) {
        QMenu* submenu = act->menu();
        if (!act->isSeparator() && (submenu == NULL))
            initItemForSearching(act);
        else if (!act->isSeparator())
            initMenuForSearching(submenu);
    }
}

void MainWindow::initItemForSearching(QAction* act)
{
    QString tx = act->text() + " " + act->toolTip();
    wama.addWordsPerAction(*act, tx);
}

QString MainWindow::getDecoratedFileName(const QString& name)
{
    return  QString("<br><b><i>(") + name + ")</i></b>";
}

void MainWindow::fillFilterMenu()
{
    clearMenu(filterMenu);
    //filterMenu->clear();
    filterMenu->addAction(lastFilterAct);
    filterMenu->addAction(showFilterScriptAct);
    filterMenu->addSeparator();
    //filterMenu->addMenu(new SearcherMenu(this,filterMenu));
    //filterMenu->addSeparator();
    // Connects the events of the actions within colorize to the method which shows their tooltip

    filterMenuSelect = new MenuWithToolTip(QString("Selection"), this);
    filterMenu->addMenu(filterMenuSelect);
    filterMenuClean = new MenuWithToolTip(QString("Cleaning and Repairing"), this);
    filterMenu->addMenu(filterMenuClean);
    filterMenuCreate = new MenuWithToolTip(QString("Create New Mesh Layer"), this);
    filterMenu->addMenu(filterMenuCreate);
    filterMenuRemeshing = new MenuWithToolTip(QString("Remeshing, Simplification and Reconstruction"), this);
    filterMenu->addMenu(filterMenuRemeshing);
    filterMenuPolygonal = new MenuWithToolTip(QString("Polygonal and Quad Mesh"), this);
    filterMenu->addMenu(filterMenuPolygonal);
    filterMenuColorize = new MenuWithToolTip(QString("Color Creation and Processing"), this);
    filterMenu->addMenu(filterMenuColorize);
    filterMenuSmoothing = new MenuWithToolTip(QString("Smoothing, Fairing and Deformation"), this);
    filterMenu->addMenu(filterMenuSmoothing);
    filterMenuQuality = new MenuWithToolTip(QString("Quality Measure and Computations"), this);
    filterMenu->addMenu(filterMenuQuality);
    filterMenuNormal = new MenuWithToolTip(QString("Normals, Curvatures and Orientation"), this);
    filterMenu->addMenu(filterMenuNormal);
    filterMenuMeshLayer = new MenuWithToolTip(QString("Mesh Layer"), this);
    filterMenu->addMenu(filterMenuMeshLayer);
    filterMenuRasterLayer = new MenuWithToolTip(QString("Raster Layer"), this);
    filterMenu->addMenu(filterMenuRasterLayer);
    filterMenuRangeMap = new MenuWithToolTip(QString("Range Map"), this);
    filterMenu->addMenu(filterMenuRangeMap);
    filterMenuPointSet = new MenuWithToolTip(QString("Point Set"), this);
    filterMenu->addMenu(filterMenuPointSet);
    filterMenuSampling = new MenuWithToolTip(QString("Sampling"), this);
    filterMenu->addMenu(filterMenuSampling);
    filterMenuTexture = new MenuWithToolTip(QString("Texture"), this);
    filterMenu->addMenu(filterMenuTexture);
    filterMenuCamera = new MenuWithToolTip(QString("Camera"), this);
    filterMenu->addMenu(filterMenuCamera);


    //this is used just to fill the menus with alhabetical order
    std::map<QString, FilterPlugin*> mapFilterPlugins;

    //populate the map
    for (FilterPlugin* fpi : PM.filterPluginIterator()){
        for (QAction* act : fpi->actions()){
            mapFilterPlugins[act->text()] = fpi;
        }
    }

    for (const auto& p : mapFilterPlugins) {
        FilterPlugin * iFilter = p.second;
        QAction *filterAction = iFilter->getFilterAction(p.first);
        QString tooltip = iFilter->filterInfo(filterAction) + "<br>" + getDecoratedFileName(filterAction->data().toString());
        filterAction->setToolTip(tooltip);
        //connect(filterAction, SIGNAL(hovered()), this, SLOT(showActionMenuTooltip()) );
        connect(filterAction, SIGNAL(triggered()), this, SLOT(startFilter()));

        int filterClass = iFilter->getClass(filterAction);
        if (filterClass & FilterPlugin::FaceColoring)
        {
            filterMenuColorize->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::VertexColoring)
        {
            filterMenuColorize->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::MeshColoring)
        {
            filterMenuColorize->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Selection)
        {
            filterMenuSelect->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Cleaning)
        {
            filterMenuClean->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Remeshing)
        {
            filterMenuRemeshing->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Smoothing)
        {
            filterMenuSmoothing->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Normal)
        {
            filterMenuNormal->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Quality)
        {
            filterMenuQuality->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Measure)
        {
            filterMenuQuality->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Layer)
        {
            filterMenuMeshLayer->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::RasterLayer)
        {
            filterMenuRasterLayer->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::MeshCreation)
        {
            filterMenuCreate->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::RangeMap)
        {
            filterMenuRangeMap->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::PointSet)
        {
            filterMenuPointSet->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Sampling)
        {
            filterMenuSampling->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Texture)
        {
            filterMenuTexture->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Polygonal)
        {
            filterMenuPolygonal->addAction(filterAction);
        }
        if (filterClass & FilterPlugin::Camera)
        {
            filterMenuCamera->addAction(filterAction);
        }
        //  MeshFilterInterface::Generic :
        if (filterClass == 0)
        {
            filterMenu->addAction(filterAction);
        }
        //if(!filterAction->icon().isNull())
        //    filterToolBar->addAction(filterAction);


    }
}

void MainWindow::fillRenderMenu()
{
    clearMenu(renderMenu);

    shadersMenu = renderMenu->addMenu(QString("&Shaders"));
    renderMenu->addSeparator();
    fillShadersMenu();

    for(DecoratePlugin *iDecorate: PM.decoratePluginIterator())
    {
        for(QAction *decorateAction: iDecorate->actions())
        {
            connect(decorateAction, SIGNAL(triggered()), this, SLOT(applyDecorateMode()));
            decorateAction->setToolTip(iDecorate->decorationInfo(decorateAction));
            renderMenu->addAction(decorateAction);
        }
    }
    connect(renderMenu, SIGNAL(hovered(QAction*)), this, SLOT(showTooltip(QAction*)));
}

void MainWindow::fillShadersMenu()
{
    QAction * qaNone = new QAction("None", this);
    qaNone->setCheckable(false);
    shadersMenu->addAction(qaNone);
    connect(qaNone, SIGNAL(triggered()), this, SLOT(applyRenderMode()));
    for(RenderPlugin *iRender:  PM.renderPluginIterator()) {
        addToMenu(iRender->actions(), shadersMenu, SLOT(applyRenderMode()));
    }
    shadersMenu->addSeparator();
    QAction * addShaderAction = new QAction("Add Shaders", this);
    addShaderAction->setCheckable(false);
    shadersMenu->addAction(addShaderAction);
    connect(addShaderAction, SIGNAL(triggered()), this, SLOT(addShaders()));
}

void MainWindow::fillEditMenu()
{
    return;  

    clearMenu(editMenu);
    editMenu->addAction(suspendEditModeAct);
    for(EditPlugin *iEditFactory: PM.editPluginFactoryIterator())
    {
        for(QAction* editAction: iEditFactory->actions())
        {
            editMenu->addAction(editAction);
            connect(editAction, SIGNAL(triggered()), this, SLOT(applyEditMode()));
        }
    }
}

void MainWindow::clearMenu(QMenu* menu)
{
    for (QAction *action : menu->actions()) {
        if (action->menu()) {
            clearMenu(action->menu());
        } else if (!action->isSeparator() && !(action==suspendEditModeAct)){
            disconnect(action, SIGNAL(triggered()), 0, 0);
        }
    }
    menu->clear();
}

void MainWindow::updateAllPluginsActions()
{
    //update menus
    fillFilterMenu();
    fillRenderMenu();
    fillEditMenu();

    //update toolbars
    decoratorToolBar->clear();
    for(DecoratePlugin *iDecorate: PM.decoratePluginIterator()) {
        for(QAction *decorateAction: iDecorate->actions()) {
            if (!decorateAction->icon().isNull())
                decoratorToolBar->addAction(decorateAction);
        }
    }

    editToolBar->clear();
    editToolBar->addAction(suspendEditModeAct);
    for(EditPlugin *iEditFactory: PM.editPluginFactoryIterator()) {
        for(QAction* editAction: iEditFactory->actions()){
            if (!editAction->icon().isNull()) {
                editToolBar->addAction(editAction);
            }
            else qDebug() << "action was null";
        }
    }
    editToolBar->addSeparator();

    filterToolBar->clear();
    updateFilterToolBar();

    //TODO update the searcher: this seems to be an impossible task due to unreadable code.
    //for now, just close and reopen meshlab....
    /*
    disconnect(searchShortCut, SIGNAL(activated()), searchButton, SLOT(openMenu()));
    wama.clear();
    delete searchMenu;

    initSearchEngine();
    int longest = longestActionWidthInAllMenus();
    searchMenu = new SearchMenu(wama, 15, searchButton, longest);
    searchButton->setMenu(searchMenu);
    connect(searchShortCut, SIGNAL(activated()), searchButton, SLOT(openMenu()));
    */
}

void MainWindow::loadDefaultSettingsFromPlugins()
{
    //decorate settings
    for (DecoratePlugin* dp : PM.decoratePluginIterator()){
        for(QAction *decoratorAction : dp->actions()) {
            dp->initGlobalParameterList(decoratorAction, defaultGlobalParams);
        }
    }

    //io settings
    for (IOPlugin* iop : PM.ioPluginIterator()){
        for (const FileFormat& ff : iop->importFormats()) {
            for (const QString& format : ff.extensions) {
                RichParameterList tmplist = iop->initPreOpenParameter(format);
                if (!tmplist.isEmpty()){
                    QString prefixName = "MeshLab::IO::" + format.toUpper() + "::";
                    for (RichParameter& rp : tmplist){
                        rp.setName(prefixName + rp.name());
                    }
                    RichBool rp(prefixName + "showPreOpenParameterDialog", true, "", "");
                    tmplist.addParam(rp);
                    defaultGlobalParams.join(tmplist);
                }
            }
        }
    }
}


void MainWindow::loadMeshLabSettings()
{
    // I have already loaded the plugins so the default parameters for the settings
    // of the plugins are already in the <defaultGlobalParams> .
    // we just miss the globals default of meshlab itself
    MainWindowSetting::initGlobalParameterList(defaultGlobalParams);
    GLArea::initGlobalParameterList(defaultGlobalParams);

    QSettings settings;
    QStringList klist = settings.allKeys();

    // 1) load saved values into the <currentGlobalParams>
    for (int ii = 0; ii < klist.size(); ++ii)
    {
        QDomDocument doc;
        doc.setContent(settings.value(klist.at(ii)).toString());

        QString st = settings.value(klist.at(ii)).toString();
        QDomElement docElem = doc.firstChild().toElement();

        if (!docElem.isNull())
        {
            RichParameter* rp = nullptr;
            bool b = RichParameterAdapter::create(docElem, &rp);
            if (b && defaultGlobalParams.hasParameter(rp->name()))
                currentGlobalParams.pushFromQDomElement(docElem);
            if (b)
                delete rp;
        }
    }

    // 2) eventually fill missing values with the hardwired defaults
    for (const RichParameter& p : defaultGlobalParams) {
        // qDebug("Searching param[%i] %s of the default into the loaded settings. ", ii, qUtf8Printable(defaultGlobalParams.paramList.at(ii)->name));
        if (!currentGlobalParams.hasParameter(p.name())) {
            //qDebug("Warning! a default param was not found in the saved settings. This should happen only on the first run...");
            RichParameter& v = currentGlobalParams.addParam(p);

            QDomDocument doc("MeshLabSettings");
            doc.appendChild(v.fillToXMLDocument(doc));
            QString docstring = doc.toString();
            QSettings setting;
            setting.setValue(v.name(), QVariant(docstring));
        }
    }
    assert(currentGlobalParams.size() == defaultGlobalParams.size());

}

void MainWindow::addToMenu(QList<QAction *> actionList, QMenu *menu, const char *slot)
{
    foreach(QAction *a, actionList)
    {
        connect(a, SIGNAL(triggered()), this, slot);
        menu->addAction(a);
    }
}

// this function update the app settings with the current recent file list
// and update the loaded mesh counter
void MainWindow::saveRecentFileList(const QString &fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MAXRECENTFILES)
        files.removeLast();

    //avoid the slash/back-slash path ambiguity
    for (int ii = 0; ii < files.size(); ++ii)
        files[ii] = QDir::fromNativeSeparators(files[ii]);
    settings.setValue("recentFileList", files);

    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) mainWin->updateRecentFileActions();
    }

    settings.setValue("totalKV", settings.value("totalKV", 0).toInt() + (GLA()->mm()->cm.vn) / 1000);
    settings.setValue("loadedMeshCounter", settings.value("loadedMeshCounter", 0).toInt() + 1);

}

void MainWindow::sendUsAMail()
{
    return;

    QSettings settings;

    // Check if the user specified not to be reminded to send email
    const QString dontRemindMeToSendEmailVar("dontRemindMeToSendEmail");
    bool dontRemindMeToSendEmailVal = false;
    if (settings.contains(dontRemindMeToSendEmailVar))
        dontRemindMeToSendEmailVal = settings.value(dontRemindMeToSendEmailVar).toBool();
    if (dontRemindMeToSendEmailVal)
        return;

    int loadedMeshCounter = settings.value("loadedMeshCounter").toInt();

    int congratsMeshCounter = settings.value("congratsMeshCounter", 50).toInt();
    if (loadedMeshCounter > congratsMeshCounter) {
        CongratsDialog* congratsDialog = new CongratsDialog(this);
        congratsDialog->show();
    }
}

void MainWindow::saveRecentProjectList(const QString &projName)
{
    QSettings settings;
    QStringList files = settings.value("recentProjList").toStringList();
    files.removeAll(projName);
    files.prepend(projName);
    while (files.size() > MAXRECENTFILES)
        files.removeLast();

    for (int ii = 0; ii < files.size(); ++ii)
        files[ii] = QDir::fromNativeSeparators(files[ii]);

    settings.setValue("recentProjList", files);

    foreach(QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentProjActions();
    }
}

void MainWindow::checkForUpdates(bool verboseFlag)
{/*
    verboseCheckingFlag = verboseFlag;

    bool checkForMonthlyAndBetasVal = false;
    const QString checkForMonthlyAndBetasVar("checkForMonthlyAndBetas");

    QString urlCheck = "https://www.meshlab.net/ML_VERSION";
    QSettings settings;
    if (settings.contains(checkForMonthlyAndBetasVar))
        checkForMonthlyAndBetasVal = settings.value(checkForMonthlyAndBetasVar).toBool();
    if (checkForMonthlyAndBetasVal){
        urlCheck = "https://raw.githubusercontent.com/cnr-isti-vclab/meshlab/master/ML_VERSION";
    }
    int totalKV = settings.value("totalKV", 0).toInt();
    int loadedMeshCounter = settings.value("loadedMeshCounter", 0).toInt();
    int savedMeshCounter = settings.value("savedMeshCounter", 0).toInt();
    QString UID = settings.value("UID", QString("")).toString();
    if (UID.isEmpty()) {
        UID = QUuid::createUuid().toString();
        settings.setValue("UID", UID);
    }
    QString baseCommand("/~cignoni/meshlab_latest.php");

    #ifdef Q_OS_WIN
        QString OS = "Win";
    #elif defined( Q_OS_OSX)
        QString OS = "Mac";
    #else
        QString OS = "Lin";
    #endif
    QString message = baseCommand + QString("?code=%1&count=%2&scount=%3&totkv=%4&ver=%5&os=%6").arg(UID).arg(loadedMeshCounter).arg(savedMeshCounter).arg(totalKV).arg(MeshLabApplication::appVer()).arg(OS);

    QNetworkAccessManager stats;
    QNetworkRequest statreq(MeshLabApplication::organizationHost() + message);
    stats.get(statreq);

    QNetworkRequest request(urlCheck);
    httpReq.get(request);
    */
}

void MainWindow::connectionDone(QNetworkReply *reply)
{/*
    QSettings settings;
    QSettings::setDefaultFormat(QSettings::NativeFormat);

    bool dontRemindMeAboutUpgradeVal = false;
    bool checkForMonthlyAndBetasVal = false;
    const QString dontRemindMeAboutUpgradeVar("dontRemindMeAboutUpgrade");
    const QString checkForMonthlyAndBetasVar("checkForMonthlyAndBetas");

    // Check if the user specified not to be reminded to upgrade
    if (settings.contains(dontRemindMeAboutUpgradeVar))
        dontRemindMeAboutUpgradeVal = settings.value(dontRemindMeAboutUpgradeVar).toBool();
    if (!verboseCheckingFlag) {
        if (dontRemindMeAboutUpgradeVal)
            return;
    }

    if (settings.contains(checkForMonthlyAndBetasVar)){
        checkForMonthlyAndBetasVal = settings.value(checkForMonthlyAndBetasVar).toBool();;
    }

    QByteArray ddata = reply->readAll();
    QString onlineVersion = QString::fromStdString(ddata.toStdString());
    QStringList splitOnlineVersion = onlineVersion.split(".");


    QString thisVersion = MeshLabApplication::appVer();
    if (thisVersion.contains("_")){ //remove nightly or rc
        int pos = thisVersion.indexOf("_");
        thisVersion = thisVersion.left(pos);
    }
    if (thisVersion.endsWith("d")){
        thisVersion.chop(1);
    }
    QStringList splitThisVersion = thisVersion.split(".");

    bool newVersionAvailable = false;
    if (splitOnlineVersion.first().toInt() > splitThisVersion.first().toInt()){
        newVersionAvailable = true;
    }
    else if (splitOnlineVersion.first().toInt() == splitThisVersion.first().toInt()){
        if (splitOnlineVersion[1].toInt() > splitThisVersion[1].toInt()) {
            newVersionAvailable = true;
        }
        else if (splitOnlineVersion[1].toInt() == splitThisVersion[1].toInt() && splitOnlineVersion.size() > 2) {
            //case of beta version or very important fixes
            if (splitThisVersion.size() == 2 || (splitThisVersion.size() > 2 && splitOnlineVersion[2] > splitThisVersion[2])){
                newVersionAvailable = true;
            }
        }
    }

    // Set up a message box for the user
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("MeshLab Version Checking");
    msgBox.addButton(QMessageBox::Ok);
    QCheckBox dontShowCheckBox("Don't show this message again.");
    dontShowCheckBox.blockSignals(true);
    msgBox.addButton(&dontShowCheckBox, QMessageBox::ResetRole);
    dontShowCheckBox.setChecked(dontRemindMeAboutUpgradeVal);
    QCheckBox checkMonthlysCheckBox("Check for Monthly and Beta versions.");
    checkMonthlysCheckBox.blockSignals(true);
    msgBox.addButton(&checkMonthlysCheckBox, QMessageBox::ResetRole);
    checkMonthlysCheckBox.setChecked(checkForMonthlyAndBetasVal);

    if (newVersionAvailable){
        QString message =
                "<center>You are using an old version of MeshLab.<br>"
                "A new MeshLab version is available: " + onlineVersion + "<br><br>"
                "Please, upgrade to the new version!<br><br>";
        if (checkForMonthlyAndBetasVal){
            message +=
                    "<big> <a href=\"https://github.com/cnr-isti-vclab/meshlab/releases\">Download</a></big></center>";
        }
        else {
            message +=
                    "<big> <a href=\"https://www.meshlab.net/#download\">Download</a></big></center>";
        }

        msgBox.setText(message);
    }
    else if (verboseCheckingFlag && !newVersionAvailable) {
        msgBox.setText("<center>Your MeshLab version is the most recent one: " + onlineVersion + ".</center>");
    }
    reply->deleteLater();

    // Showing the dialog only if there is a new version or if we are verbose
    if (newVersionAvailable || verboseCheckingFlag) {
        int userReply = msgBox.exec();
        if (userReply == QMessageBox::Ok && dontShowCheckBox.checkState() == Qt::Checked)
            settings.setValue(dontRemindMeAboutUpgradeVar, true);
        else if (userReply == QMessageBox::Ok && dontShowCheckBox.checkState() == Qt::Unchecked)
            settings.setValue(dontRemindMeAboutUpgradeVar, false);
        if (userReply == QMessageBox::Ok && checkMonthlysCheckBox.checkState() == Qt::Checked) {
            settings.setValue(checkForMonthlyAndBetasVar, true);
            if (!checkForMonthlyAndBetasVal) {
                //the user changed the states: he now wants to check for betas
                //need to check again with properly set
                checkForUpdates(false);
            }
        }
        else if (userReply == QMessageBox::Ok && checkMonthlysCheckBox.checkState() == Qt::Unchecked)
            settings.setValue(checkForMonthlyAndBetasVar, false);
    }*/
}


void MainWindow::submitBug()
{/*
    QMessageBox mb(QMessageBox::NoIcon, MeshLabApplication::appName(), MeshLabApplication::appName(), QMessageBox::NoButton, this);
    //mb.setWindowTitle(QString("MeshLab"));
    QPushButton *submitBug = mb.addButton("Submit Bug", QMessageBox::AcceptRole);
    mb.addButton(QMessageBox::Cancel);
    mb.setText(QString("If Meshlab closed in unexpected way (e.g. it crashed badly) and"
        "if you are able to repeat the bug, please consider to submit a report using the github issue tracking system.\n"
    ));
    mb.setInformativeText(tr(
        "Hints for a good, useful bug report:\n"
        "- Be verbose and descriptive\n"
        "- Report meshlab version and OS\n"
        "- Describe the sequence of actions that bring you to the crash.\n"
        "- Consider submitting the mesh file causing a particular crash.\n"
    ));

    mb.exec();

    if (mb.clickedButton() == submitBug)
        QDesktopServices::openUrl(QUrl("https://github.com/cnr-isti-vclab/meshlab/issues"));
*/
}

void MainWindow::wrapSetActiveSubWindow(QWidget* window) {
    QMdiSubWindow* subwindow;
    subwindow = dynamic_cast<QMdiSubWindow*>(window);
    if (subwindow != NULL)
    {
        mdiarea->setActiveSubWindow(subwindow);
    }
    else {
        qDebug("Type of window is not a QMdiSubWindow*");
    }
}

int MainWindow::longestActionWidthInMenu(QMenu* m, const int longestwidth)
{
    int longest = longestwidth;

    const QList<QAction*>& acts = m->actions();
    foreach(QAction* act, acts)
    {
        QMenu* submenu = act->menu();
        if (!act->isSeparator() && (submenu == NULL))
            longest = std::max(longest, m->actionGeometry(act).width());

        else if (!act->isSeparator())
            longest = std::max(longest, longestActionWidthInMenu(submenu, longest));
    }
    return longest;
}

int MainWindow::longestActionWidthInMenu(QMenu* m)
{
    return longestActionWidthInMenu(m, 0);
}

int MainWindow::longestActionWidthInAllMenus()
{
    int longest = 0;
    QList<QMenu*> list = menuBar()->findChildren<QMenu*>();
    for(QMenu* m: list)
        longest = std::max(longest, longestActionWidthInMenu(m));
    return longest;
}

void MainWindowSetting::initGlobalParameterList(RichParameterList& gbllist)
{
    gbllist.addParam(RichInt(maximumDedicatedGPUMem(), 350, "Maximum GPU Memory Dedicated to MeshLab (Mb)", "Maximum GPU Memory Dedicated to MeshLab (megabyte) for the storing of the geometry attributes. The dedicated memory must NOT be all the GPU memory presents on the videocard."));
    gbllist.addParam(RichInt(perBatchPrimitives(), 100000, "Per batch primitives loaded in GPU", "Per batch primitives (vertices and faces) loaded in the GPU memory. It's used in order to do not overwhelm the system memory with an entire temporary copy of a mesh."));
    gbllist.addParam(RichInt(minFaceNumberPerSmoothRendering(), 2000000, "Default Face number per smooth rendering", "Minimum number of faces in order to automatically render a newly created mesh layer with the per vertex normal attribute activated."));

//	glbset->addParam(RichBool(perMeshRenderingToolBar(), true, "Show Per-Mesh Rendering Side ToolBar", "If true the per-mesh rendering side toolbar will be redendered inside the layerdialog."));

    if (MeshLabScalarTest<Scalarm>::doublePrecision())
        gbllist.addParam(RichBool(highPrecisionRendering(), false, "High Precision Rendering", "If true all the models in the scene will be rendered at the center of the world"));
    gbllist.addParam(RichInt(maxTextureMemoryParam(), 256, "Max Texture Memory (in MB)", "The maximum quantity of texture memory allowed to load mesh textures"));
}

void MainWindowSetting::updateGlobalParameterList(const RichParameterList& rpl)
{
    maxgpumem = (std::ptrdiff_t)rpl.getInt(maximumDedicatedGPUMem()) * (float)(1024 * 1024);
    perbatchprimitives = (size_t)rpl.getInt(perBatchPrimitives());
    minpolygonpersmoothrendering = (size_t)rpl.getInt(minFaceNumberPerSmoothRendering());
    highprecision = false;
    if (MeshLabScalarTest<Scalarm>::doublePrecision())
        highprecision = rpl.getBool(highPrecisionRendering());
    maxTextureMemory = (std::ptrdiff_t) rpl.getInt(this->maxTextureMemoryParam()) * (float)(1024 * 1024);
}

void MainWindow::defaultPerViewRenderingData(MLRenderingData& dt) const
{
    MLRenderingData::RendAtts tmpatts;
    tmpatts[MLRenderingData::ATT_NAMES::ATT_VERTPOSITION] = true;
    tmpatts[MLRenderingData::ATT_NAMES::ATT_VERTNORMAL] = true;
    tmpatts[MLRenderingData::ATT_NAMES::ATT_VERTCOLOR] = true;
    tmpatts[MLRenderingData::ATT_NAMES::ATT_WEDGETEXTURE] = true;
    dt.set(MLRenderingData::PR_SOLID, tmpatts);
    MLPerViewGLOptions opts;
    dt.set(opts);
}
