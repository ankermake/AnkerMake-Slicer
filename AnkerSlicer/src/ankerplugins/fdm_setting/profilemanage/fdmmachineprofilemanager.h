#pragma once
#include <QStringList>
#include <QList>
#include "fdmmachineprofile.h"
#include "fdmprofilemanager.h"
#include "fdmparameterprofilemanager.h"
#include <QMap>

class FdmMachineProfileManager :public FdmProfileManager<FdmMachineProfile>
{
public:
    static FdmMachineProfileManager& Instance()
    {
        //static QString profileStorePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/machine");
        static QString profileStorePath = ":/fdm/machine";
        static QString customStorePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm/machine");
        static FdmMachineProfileManager manager(profileStorePath, customStorePath);
        return manager;
    }
    FdmMachineProfileManager(const QString & storePath, const QString& customStorePath ): FdmProfileManager<FdmMachineProfile>(storePath, customStorePath){}

    
    //QString getDefaultSelectedName(){
    //    auto machineId = FdmParameterProfileManager::Instance().getCurrentSelectedProfile()->getMachineName();
    //    auto profile = getProfileById(machineId);
    //    if (NULL == profile)
    //    {
    //        return NULL;
    //    }
    //    return profile->getName();
    //}
};


