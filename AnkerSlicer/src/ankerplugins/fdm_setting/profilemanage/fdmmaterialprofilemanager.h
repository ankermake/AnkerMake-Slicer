#ifndef FDMMATERIALPROFILEMANAGER_H
#define FDMMATERIALPROFILEMANAGER_H
#include "ifdmprofilemanager.h"
#include "fdmprofilemanager.h"
#include "fdmprofilemanager.h"
#include "fdmmaterialprofile.h"
#include <QCoreApplication>
#include "fdmparameterprofilemanager.h"
#include <QDir>



class FdmMaterialProfileManager : public FdmProfileManager<FdmMaterialProfile>
{
public:
    
    static FdmMaterialProfileManager& Instance()
    {
        static QString profileStorePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/material");
        static QString customStorePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm/material");
        static FdmMaterialProfileManager manager(profileStorePath, customStorePath);
        return manager;
    }
    FdmMaterialProfileManager(const QString & storePath, const QString& customStorePath):FdmProfileManager<FdmMaterialProfile>(storePath, customStorePath){}

    
    //QString getDefaultSelectedName(){
    //    auto materialId = FdmParameterProfileManager::Instance().getCurrentSelectedProfile()->getMaterialId();
    //    auto profile = getProfileById(materialId);
    //    if (NULL == profile)
    //    {
    //        return NULL;
    //    }
    //    return profile->getName();
    //}
};

#endif // FDMMATERIALPROFILEMANAGER_H
