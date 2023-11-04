include (../../shared.pri)
QT += xml
QT += opengl
QT += network

QT += gui-private
LIBS += -L$$MESHLAB_DISTRIB_EXT_DIRECTORY

TARGET = fdm_gcode_parser

#SUBDIRS += gcodeProcessor
#gcodeProcessor.subdir = gcodeProcessor
#win32-msvc:LIBS = $$MESHLAB_DISTRIB_DIRECTORY/lib/fdm_gcode_preview.lib  $$LIBS
#win32-g++:LIBS = $$MESHLAB_DISTRIB_DIRECTORY/lib/libfdm_gcode_preview.a $$LIBS
#linux:LIBS = -lfdm_gcode_preview $$LIBS
#macx:LIBS = $$MESHLAB_DISTRIB_DIRECTORY/lib/libfdm_gcode_preview.dylib $$LIBS
##macx:LIBS = $$MESHLAB_DISTRIB_DIRECTORY/lib/libfdm_gcode_preview.a $$LIBS
#macx:QMAKE_POST_LINK = " \ #every plugin needs to point to meshlab-common placed in the appbundle
#    cp -P $$MESHLAB_DISTRIB_DIRECTORY/lib/libmeshlab-common.1.dylib $$MESHLAB_DISTRIB_DIRECTORY/$${TARGET}.app/Contents/MacOS/; \
#    install_name_tool -change libfdm_gcode_preview.1.dylib @executable_path/libfdm_gcode_preview.1.dylib $$MESHLAB_DISTRIB_DIRECTORY/$${TARGET}.app/Contents/MacOS/$${TARGET}; \
#"

#message("xxxxxxxxxxxxxxxxxxxxxxxx")
#message($$LIBS)
HEADERS += \
    fdm_gcode_parser.h \
    fdm_rpc/fdm_rpc_wrapper.h\
    GcodeViewer.h \
    fdmgcodepreviewentry.h \
    glview/gcode_label_edit.h \
    glview/slider2/sliderrange_vertical.h	\
    glview/slider2/sliderrange_horizontal.h \
    glview/viewwidget.h \
    glview/scene3d.h \
    glview/renderscene.h   \
    post_process_gcode.h \
    process_ai_picture.h  \
    gcodeProcessor/GCodeReader.hpp \
    gcodeProcessor/GCodeProcessor.hpp


SOURCES += \
    fdm_gcode_parser.cpp \
    fdm_rpc/fdm_rpc_wrapper.cpp\
    fdmgcodepreviewentry.cpp \
    GcodeViewer.cpp \
    glview/gcode_label_edit.cpp \
    glview/slider2/sliderrange_vertical.cpp	\
    glview/slider2/sliderrange_horizontal.cpp \
    glview/viewwidget.cpp \
    glview/scene3d.cpp \
    glview/renderscene.cpp    \
    post_process_gcode.cpp \
    process_ai_picture.cpp \
    gcodeProcessor/GCodeReader.cpp \
    gcodeProcessor/GCodeProcessor.cpp

RESOURCES +=  \
    pic/pic.qrc \
    shader/Shader.qrc

FORMS += \
    glview/viewwidget.ui

REPC_SOURCE += \
    fdm_rpc/fdm_rpc.rep

CONFIG += c++17

INCLUDEPATH += \
    ./glview \
    ../fdm_gcode_preview \
    ../fdm_gcode_preview/glview \
    ../fdm_gcode_preview/glview/slider2 \
    ../fdm_gcode_preview/gcodeProcessor \


message($$PWD)

QT += widgets
QT += remoteobjects

#DEFINES += USE_EXTRA_UI
