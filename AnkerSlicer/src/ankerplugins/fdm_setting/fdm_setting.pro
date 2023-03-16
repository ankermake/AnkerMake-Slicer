include (../../shared.pri)

QT += widgets core quick qml quickwidgets
CONFIG += c++17

TARGET = fdm_setting

HEADERS += \
    fdm_setting.h \
    fdmparamsettingswidget.h \
    fdmsettingstreemodel.h \
    filemanage/fdmSettingsCommon.h \
    filemanage/fdmparamdata.h \
    filemanage/fdmqmlengine.h \
    filemanage/fdmqmlsourcetree.h \
    filemanage/fdmsettingvisibilityconfig.h \
    filemanage/qmljsonsetting.hpp \
    filemanage/shadowtreetemplate.hpp \
    machinewidget.h \
    materialwidget.h \
    parameterswidget.h \
    paramlistmodel.h \
    preferencesparamters.h \
    profilemanage/fdmbaseprofile.h \
    profilemanage/fdmmachineprofilemanager.h \
    profilemanage/fdmmachineprofile.h \
    profilemanage/fdmmaterialprofile.h \
    profilemanage/fdmmaterialprofilemanager.h \
    profilemanage/fdmparameterprofile.h \
    profilemanage/fdmparameterprofilemanager.h \
    profilemanage/fdmprofilecategory.h \
    profilemanage/fdmprofilemanager.h \
    profilemanage/fdmprofilemanagerimpl.h \
    profilemanage/fdmrealtimeprofile.h \
    profilemanage/ifdmprofile.h \
    profilemanage/ifdmprofilemanager.h \
    profilemanage/operatelog.h \
    profilemanage/operlogcollector.h \
    savepanel.h \
    service/fdmmachineprofileservice.h \
    service/fdmmaterialprofileservice.h \
    service/fdmparameterprofileservice.h \
    service/fdmpreferencedialogservice.h \
    service/fdmprofilebackupservice.h \
    service/fdmrightparameterservice.h \
    tooltip.h

SOURCES += \
    fdm_setting.cpp \
    fdmparamsettingswidget.cpp \
    fdmsettingstreemodel.cpp \
    filemanage/fdmparamdata.cpp \
    filemanage/fdmqmlengine.cpp \
    filemanage/fdmqmlsourcetree.cpp \
    filemanage/fdmsettingvisibilityconfig.cpp \
    filemanage/qmljsonsetting.cpp \
    filemanage/shadowtreetemplate.cpp \
    machinewidget.cpp \
    materialwidget.cpp \
    parameterswidget.cpp \
    paramlistmodel.cpp \
    preferencesparamters.cpp \
    profilemanage/fdmbaseprofile.cpp \
    profilemanage/fdmmachineprofilemanager.cpp \
    profilemanage/fdmmachineprofile.cpp \
    profilemanage/fdmmaterialprofile.cpp \
    profilemanage/fdmmaterialprofilemanager.cpp \
    profilemanage/fdmparameterprofile.cpp \
    profilemanage/fdmparameterprofilemanager.cpp \
    profilemanage/fdmprofilecategory.cpp \
    profilemanage/fdmprofilemanager.cpp \
    profilemanage/fdmprofilemanagerimpl.cpp \
    profilemanage/fdmrealtimeprofile.cpp \
    profilemanage/operatelog.cpp \
    profilemanage/operlogcollector.cpp \
    savepanel.cpp \
    service/fdmmachineprofileservice.cpp \
    service/fdmmaterialprofileservice.cpp \
    service/fdmparameterprofileservice.cpp \
    service/fdmpreferencedialogservice.cpp \
    service/fdmprofilebackupservice.cpp \
    service/fdmrightparameterservice.cpp \
    tooltip.cpp

RESOURCES += \
    fdm_setting.qrc \
    qml.qrc

FORMS +=

DISTFILES +=










