include (../../shared.pri)

TARGET = fdm_slicer

HEADERS += \
    akprogressbar.h \
    aksliceinfo.h \
    akslicepanel.h \
    akslicer.h \
    enginelib.h \
    fdm_slicer.h 

SOURCES += \
    akprogressbar.cpp \
    aksliceinfo.cpp \
    akslicepanel.cpp \
    akslicer.cpp \
    enginelib.cpp \
    fdm_slicer.cpp 

RESOURCES += 

#FORMS +=  \
#    akprogressbar.ui

DISTFILES += \

QT += widgets

#DEFINES += DEBUG_BY_CL
contains(DEFINES, DEBUG_BY_CL){
    message("DEBUG_BY_CL=true")
    message(ENGINE_LIB $$PWD/lib)
    HEADERS += $$PWD/lib/Application.h

    win32:{
        contains(QT_ARCH, i386) {
            message("32-bit")
        } else {
            message("64-bit")
            win32-msvc: {
                LIBS += $$PWD/lib/clipper.lib
                LIBS += $$PWD/lib/_CuraEngine.lib
            }
            win32-g++: {
                LIBS += $$PWD/lib/libclipper.a
                LIBS += $$PWD/lib/lib_CuraEngine.a
            }
        }
    }
}
