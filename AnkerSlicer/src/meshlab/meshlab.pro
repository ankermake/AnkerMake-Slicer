include (../general.pri)

QT += gui \
    widgets quickwidgets
QT += opengl
QT += xml
QT += network
QT += remoteobjects
QT += gui-private

#CONFIG += debug_and_release
DESTDIR = $$MESHLAB_DISTRIB_DIRECTORY

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

INCLUDEPATH *= \
        . \
        .. \
        ../.. \
        $$VCGDIR \
        $$EIGENDIR

!CONFIG(system_glew): INCLUDEPATH *= $$GLEWDIR/include

DEPENDPATH += \
        $$VCGDIR \
        $$VCGDIR/vcg \
        $$VCGDIR/wrap

HEADERS += \
        LocalServer.h \
        NewRender/CH3dView.h \
        NewRender/CHChildDocWindow.h \
        NewRender/CHRenderData.h \
        ankermainwindow.h \
        controls/basetabwidget.h \
        controls/controlmanager.h \
        controls/copyrightwidget.h \
        controls/customtitle.h \
        controls/generalwidget.h \
        controls/glaretopview.h \
        controls/mainwidget.h \
        controls/meshinfowidget.h \
        controls/modeltransformwidget.h \
        controls/preferencesdialog.h \
        controls/splitterwidget.h \
        controls/testwidget.h \
        controls/toolBar.h \
        controls/useragreementwidget.h \
        controls/viewwidget.h \
        mainwindow.h \
        glarea.h \
        dialogs/about_dialog.h \
        dialogs/congrats_dialog.h \
        dialogs/filter_script_dialog.h \
        dialogs/options_dialog.h \
        dialogs/plugin_info_dialog.h \
        dialogs/save_mesh_attributes_dialog.h \
        dialogs/save_snapshot_dialog.h \
        dialogs/setting_dialog.h \
        message/messageprocessing.h \
        multiViewer_Container.h \
        glarea_setting.h \
        layerDialog.h \
        ml_std_par_dialog.h \
        additionalgui.h \
        qframelesshelper.h \
        qshadowhelper.h \
        settingmanager.h \
        snapshotsetting.h \
        ml_render_gui.h \
        ml_rendering_actions.h \
        ml_default_decorators.h \
        $$VCGDIR/wrap/gui/trackball.h \
        $$VCGDIR/wrap/gui/trackmode.h \
        $$VCGDIR/wrap/gl/trimesh.h \
        rich_parameter_gui/richparameterlistdialog.h \
        rich_parameter_gui/richparameterlistframe.h \
        rich_parameter_gui/richparameterwidgets.h \
        translator.h

SOURCES += \
        LocalServer.cpp \
        NewRender/CH3dView.cpp \
        NewRender/CHChildDocWindow.cpp \
        NewRender/CHRenderData.cpp \
        ankermainwindow.cpp \
        controls/basetabwidget.cpp \
        controls/controlmanager.cpp \
        controls/copyrightwidget.cpp \
        controls/customtitle.cpp \
        controls/generalwidget.cpp \
        controls/glaretopview.cpp \
        controls/mainwidget.cpp \
        controls/meshinfowidget.cpp \
        controls/modeltransformwidget.cpp \
        controls/preferencesdialog.cpp \
        controls/splitterwidget.cpp \
        controls/testwidget.cpp \
        controls/toolBar.cpp \
        controls/useragreementwidget.cpp \
        controls/viewwidget.cpp \
        main.cpp \
        mainwindow_Init.cpp \
        mainwindow_RunTime.cpp \
        glarea.cpp \
        dialogs/about_dialog.cpp \
        dialogs/congrats_dialog.cpp \
        dialogs/filter_script_dialog.cpp \
        dialogs/options_dialog.cpp \
        dialogs/plugin_info_dialog.cpp \
        dialogs/save_mesh_attributes_dialog.cpp \
        dialogs/save_snapshot_dialog.cpp \
        dialogs/setting_dialog.cpp \
        message/messageprocessing.cpp \
        multiViewer_Container.cpp \
        layerDialog.cpp \
        ml_std_par_dialog.cpp \
        additionalgui.cpp \
        ml_render_gui.cpp \
        ml_rendering_actions.cpp \
        ml_default_decorators.cpp \
        $$VCGDIR/wrap/gui/trackball.cpp \
        $$VCGDIR/wrap/gui/trackmode.cpp \
        $$VCGDIR/wrap/gui/coordinateframe.cpp \
        glarea_setting.cpp \
        qframelesshelper.cpp \
        qshadowhelper.cpp \
        rich_parameter_gui/richparameterlistdialog.cpp \
        rich_parameter_gui/richparameterlistframe.cpp \
        rich_parameter_gui/richparameterwidgets.cpp \
        settingmanager.cpp \
        translator.cpp
FORMS += \
  dialogs/about_dialog.ui \
  dialogs/congrats_dialog.ui \
        dialogs/filter_script_dialog.ui \
        dialogs/plugin_info_dialog.ui \
        dialogs/save_mesh_attributes_dialog.ui \
        dialogs/save_snapshot_dialog.ui \
        ui/layerDialog.ui

RESOURCES += \
        ankerimages/ankerslicer.qrc \
        meshlab.qrc \
        translator.qrc

win32-msvc2005: RCC_DIR = $(ConfigurationName)
win32-msvc2008: RCC_DIR = $(ConfigurationName)


# add @2022-01-19 by CL
TARGET = AnkerSlicer
TEMPLATE = app

# to add windows icon
#win32:RC_FILE = meshlab.rc
win32:RC_FILE = ankerimages/ankerslicer.rc # exe icon @2022-01-19 by CL
macx:RC_FILE = ankerimages/ankerslicer.rc

# ## the xml info list
# ## the next time the app open a new extension
#QMAKE_INFO_PLIST = Info.plist

# to add MacOS icon
#ICON = images/meshlab.icns
ICON = ankerimages/logo_ankerslicer.png

# note that to add the file icons on the mac the following line does not work.
# You have to copy the file by hand into the meshlab.app/Contents/Resources directory.
# ICON += images/meshlab_obj.icns

CONFIG += stl

LIBS += -L$$MESHLAB_DISTRIB_EXT_DIRECTORY

win32-msvc:LIBS += \
        -L$$MESHLAB_DISTRIB_DIRECTORY/lib -lmeshlab-common \
        -lopengl32  \
        -lGLU32

win32-g++:LIBS += \
        -L$$MESHLAB_DISTRIB_DIRECTORY/lib -lmeshlab-common	\
        -lopengl32  \
        -lGLU32

macx:LIBS += \
        #-L$$MESHLAB_DISTRIB_DIRECTORY/lib/macx64 \
        $$MESHLAB_DISTRIB_DIRECTORY/lib/libmeshlab-common.dylib
#macx:LIBS += $$MESHLAB_DISTRIB_DIRECTORY/lib/libmeshlab-common.a
macx:{
    contains(QMAKE_HOST.arch, arm64){
    message("arm core"$$QMAKE_HOST.arch)
    SLICER_ENGINE_DIR=$$MESHLAB_DISTRIB_DIRECTORY/../../3rd/SliceEngine/SliceEngine_macx_arm64/
    }else{
    message("inter core"$$QMAKE_HOST.arch)
    SLICER_ENGINE_DIR=$$MESHLAB_DISTRIB_DIRECTORY/../../3rd/SliceEngine/SliceEngine_macx/
    #MAC_COMPILER_CHAIN=
    }
    message("MESHLAB_DISTRIB_DIRECTORY:")
    message($$MESHLAB_DISTRIB_DIRECTORY)
}

macx:{
QMAKE_POST_LINK += "\
    rm -rf $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/Plugins; \
    mkdir -p $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/PlugIns/; \
    cp -R $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/CompilerChain/macx/ $$MESHLAB_DISTRIB_DIRECTORY/../../distrib ;\
    cp -R $$SLICER_ENGINE_DIR $$MESHLAB_DISTRIB_DIRECTORY/../../distrib/SliceEngine ;\
    chmod +x $$MESHLAB_DISTRIB_DIRECTORY/../../distrib/SliceEngine/CuraEngine ;\
    cp -R $$MESHLAB_DISTRIB_DIRECTORY/../../distrib/ $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/MacOS/ ;\
    cp -R $$MESHLAB_DISTRIB_DIRECTORY/plugins/* $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/PlugIns/; \
    cp -P $$MESHLAB_DISTRIB_DIRECTORY/lib/libmeshlab-common.1.dylib $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/Frameworks/; \
    install_name_tool -change libmeshlab-common.1.dylib @rpath/libmeshlab-common.1.dylib $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/MacOS/AnkerSlicer ;\
    install_name_tool -change /usr/local/opt/libomp/lib/libomp.dylib @rpath/libomp.dylib $$MESHLAB_DISTRIB_DIRECTORY/AnkerSlicer.app/Contents/MacOS/AnkerSlicer ;"
}


win32:{
    contains(QT_ARCH, i386) {
        message("32-bit")
        win32-g++: target_engine = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/SliceEngine/SliceEngine_gcc_x32/*.*
        win32-g++: target_PlatformApi = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/PlatformApi/win_x32/*.*
        win32-g++: target_CompilerChain = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/CompilerChain/mingw_x32/*.*
    } else {
        message("64-bit")
        win32-msvc: target_engine = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/SliceEngine/SliceEngine_msvc_x64/*.*
        win32-msvc: target_PlatformApi = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/PlatformApi/win_x64/*.*
        win32-msvc: target_CompilerChain = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/CompilerChain/msvc_x64/*.*
        win32-g++: target_engine = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/SliceEngine/SliceEngine_gcc_x64/*.*
        win32-g++: target_PlatformApi = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/PlatformApi/win_x64/*.*
        win32-g++: target_CompilerChain = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/CompilerChain/mingw_x64/*.*
    }
	
	 
	
    target1 = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/Network/Network_gcc_x64/*.*
    contains(QT_VERSION, 5.15.1){
        target2 = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/Network/Network_gcc_x64/Mqtt/*.*
        target2 = $$replace(target2, /, \\)
    }
    target3 = $$MESHLAB_DISTRIB_DIRECTORY/../../3rd/Network/Network_gcc_x64/plugins/*.*
    target_out1 = $$OUT_PWD/../distrib/Network/
    target_out2 = $$OUT_PWD/../distrib/
    target_out3 = $$OUT_PWD/../distrib/plugins/
	 
    target1 = $$replace(target1, /, \\)
    target3 = $$replace(target3, /, \\)
    target_out1 = $$replace(target_out1, /, \\)
    target_out2 = $$replace(target_out2, /, \\)
    target_out3 = $$replace(target_out3, /, \\)

message("target3: " $$target3)
message("target_out3: " $$target_out3)


    output_dir = $$MESHLAB_DISTRIB_DIRECTORY/SliceEngine/
    output_bindir = $$MESHLAB_DISTRIB_DIRECTORY
    target_engine = $$replace(target_engine, /, \\)
    target_PlatformApi = $$replace(target_PlatformApi, /, \\)
    target_CompilerChain = $$replace(target_CompilerChain, /, \\)
    output_dir = $$replace(output_dir, /, \\)
    output_bindir = $$replace(output_bindir, /, \\)
    QMAKE_POST_LINK +=  mkdir $$target_out1 &\
     copy /Y $$target_engine $$output_dir  &\
     copy /Y $$target_PlatformApi $$output_bindir  &\
     copy /Y $$target_CompilerChain $$output_bindir &\
     copy /Y $$target3 $$target_out3 &\
     copy /Y $$target1 $$target_out1 &\
     copy /Y $$target1 $$target_out2 &\
     copy /Y $$target2 $$target_out1 &\
     copy /Y $$target2 $$target_out2
}


linux:LIBS += \
        -lmeshlab-common -lGLU #\
        #-L$$MESHLAB_DISTRIB_DIRECTORY/lib/linux

!CONFIG(system_glew) {
        INCLUDEPATH *= $$GLEWDIR/include
}
CONFIG(system_glew): LIBS += -lGLEW

# uncomment in your local copy only in emergency cases.
# We should never be too permissive
# win32-g++:QMAKE_CXXFLAGS += -fpermissive

# The following define is needed in gcc to remove the asserts
win32-g++:DEFINES += NDEBUG
CONFIG(debug, debug|release):win32-g++:release:DEFINES -= NDEBUG


