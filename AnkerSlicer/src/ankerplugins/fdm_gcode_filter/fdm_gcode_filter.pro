#  new @2023-03-02 by ChunLian
include (../../shared.pri)

QT += 

TARGET = fdm_gcode_filter
win32-msvc:QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8

HEADERS += \
    GCodeFilterControl.h \
    GCodeFilter_AIBase.h \
    GCodeFilter_AIProducer.h \
    GCodeFilter_Color.h \
    QFileMemoryMap.h \
    QMemoryReference.h \
    fdm_gcode_filter.h 

SOURCES += \
    GCodeFilterControl.cpp \
    GCodeFilter_AIBase.cpp \
    GCodeFilter_AIProducer.cpp \
    GCodeFilter_Color.cpp \
    QFileMemoryMap.cpp \
    QMemoryReference.cpp \
    fdm_gcode_filter.cpp 

RESOURCES += 

FORMS += 

DISTFILES += 

win32:DEFINES += GCODE_FILTER_OMP
