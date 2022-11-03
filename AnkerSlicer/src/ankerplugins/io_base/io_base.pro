include (../../shared.pri)

HEADERS += \
    baseio.h \
    form.h \
    load_project.h \
    save_project.h \
    $$VCGDIR/wrap/io_trimesh/import_ply.h \
    $$VCGDIR/wrap/io_trimesh/import_obj.h \
    $$VCGDIR/wrap/io_trimesh/import_off.h \
    $$VCGDIR/wrap/io_trimesh/import_ptx.h \
    $$VCGDIR/wrap/io_trimesh/import_stl.h \
    $$VCGDIR/wrap/io_trimesh/import_fbx.h \
    $$VCGDIR/wrap/io_trimesh/export_ply.h \
    $$VCGDIR/wrap/io_trimesh/export_obj.h \
    $$VCGDIR/wrap/io_trimesh/export_off.h \
    $$VCGDIR/wrap/ply/plylib.h \
    $$VCGDIR/wrap/io_trimesh/io_material.h


SOURCES += \
    baseio.cpp \
    form.cpp \
    load_project.cpp \
    save_project.cpp \
    $$VCGDIR/wrap/ply/plylib.cpp \
    $$VCGDIR/wrap/openfbx/src/ofbx.cpp \
    $$VCGDIR/wrap/openfbx/src/miniz.c \
    $$MESHLAB_EXTERNAL_DIRECTORY/easyexif/exif.cpp

INCLUDEPATH += $$MESHLAB_EXTERNAL_DIRECTORY/easyexif

TARGET = io_base

#INCLUDEPATH += $$PWD/include

#win32-msvc: LIBS += -L$$PWD/lib/ -lquazipd
#win32-msvc: LIBS += -L$$PWD/lib/ -lquazip


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lquazip
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lquazipd

#INCLUDEPATH += $$PWD/include
#DEPENDPATH += $$PWD/include

FORMS += \
    form.ui

QT += widgets
