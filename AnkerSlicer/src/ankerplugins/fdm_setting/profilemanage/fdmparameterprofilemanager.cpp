#include "fdmparameterprofilemanager.h"
#include "fdmmachineprofilemanager.h"
#include "fdmmaterialprofilemanager.h"
#include "common/ak_const.h"
#include "ifdmprofile.h"
#include <memory>
#include "common/utilities/tlogger.h"
#include <QDebug>
using namespace AkUtil;

FdmParameterProfileManager::FdmParameterProfileManager():rightRealTimeProfile(NULL),
baseProfile(NULL)
{
}
FdmParameterProfileManager::FdmParameterProfileManager(const QString& storePath, const QString& customStorePath)
    : FdmProfileManager<FdmParameterProfile>(storePath, customStorePath),
    rightRealTimeProfile(NULL),
    baseProfile(NULL)
{
}

FdmParameterProfileManager::~FdmParameterProfileManager()
{
    if (baseProfile != NULL)
    {
        delete baseProfile;
        baseProfile = NULL;
    }

    if (rightRealTimeProfile != NULL)
    {
        delete rightRealTimeProfile;
        rightRealTimeProfile = NULL;
    }
}

void FdmParameterProfileManager::createBaseProfile()
{
    QString baseProfilePath = QDir(getBaseProfilePath()).absoluteFilePath("base.ini");
    QFileInfo fInfo(baseProfilePath);
    if (!fInfo.absoluteDir().exists())
    {
        fInfo.absoluteDir().mkpath(fInfo.absolutePath());
    }

    
    QList<FdmProfileCategory> categories;
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().getAll(categories);
    baseProfile = new FdmParameterProfile(baseProfilePath, categories, ProfileGroup::Default);
    baseProfile->setName("system_base_profile");
    baseProfile->save(true);
}

void FdmParameterProfileManager::createRealTimeProfile()
{
    QString realTimeProfilePath = QDir(getRealtimeProfilePath()).absoluteFilePath("realtimeprofile.ini");
    QFileInfo fInfo(realTimeProfilePath);
    if (!fInfo.absoluteDir().exists())
    {
        fInfo.absoluteDir().mkpath(fInfo.absolutePath());
    }

    
    if (!QFile::exists(realTimeProfilePath))
    {
        QList<FdmProfileCategory> categories;
        FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().getAll(categories);
        rightRealTimeProfile = new FdmRealTimeProfile(realTimeProfilePath, categories);
        rightRealTimeProfile->setName("system_realtime_profile");

        
        //QString x5 = QDir(profileStorePath).absoluteFilePath("realtime/realtimeprofile.ini");
        //QString abs = QDir(profileStorePath).absoluteFilePath("realtime/realtimeprofile.ini");
        auto x5Profile = FdmMachineProfileManager::Instance().getProfileByName(AkConst::MachineName::M5);
        auto plaPlusProfile = FdmMaterialProfileManager::Instance().getProfileByName(AkConst::MaterialName::PLAPLUS);

        rightRealTimeProfile->setProfileName(AkConst::ProfileName::SIMPLE_MODE);
        rightRealTimeProfile->setVisible(true);
        rightRealTimeProfile->updateMachine(x5Profile);
        rightRealTimeProfile->updateMaterial(plaPlusProfile);

        rightRealTimeProfile->save();
    }
    else
    {
        rightRealTimeProfile = new FdmRealTimeProfile(realTimeProfilePath);
    }
}


void FdmParameterProfileManager::init()
{
    FdmProfileManager<FdmParameterProfile>::init();
    createBaseProfile();
    createRealTimeProfile();

    
//    for (size_t i = 0; i < customProfiles.size(); i++)
//    {
//        auto cates = customProfiles[i].getAllCategories();
//        for (size_t j = 0; j < cates->size(); j++)
//        {
//            if ((*cates)[j].name == "ak_meta")
//            {
//                auto items = (*cates)[j].getItems();
//                for (size_t k = 0; k < items.size(); k++)
//                {
//                    if (items[k].name == "meta_create_time")
//                    {
//                        TDebug(QString("customProfiles %1, create_time %2").arg(customProfiles[i].getName()).arg(items[k].value.toString()));
//                    }
//                }
//            }
//        }
//    }
    
    assert(nullptr != rightRealTimeProfile);
    assert(nullptr != baseProfile);

    
    FdmParameterProfile newProfile("",ProfileGroup::Default);
    //newProfile.regenMetaData();
    newProfile.setName(AkConst::ProfileName::EXPERT_MODE);
    newProfile.setVisible(false);
    newProfile.addStatus(EProfileStatus::NewFile);
    addDefault(newProfile);

    //newProfile.regenMetaData();
    newProfile.setName(AkConst::ProfileName::SIMPLE_MODE);
    addDefault(newProfile);
}


void FdmParameterProfileManager::readProfile(QString profile, QList<FdmProfileCategory>& categories)
{
    FdmParameterProfile newProfile(profile, ProfileGroup::Custom);
    categories.append(*newProfile.getAllCategories());
}

FdmParameterProfile* FdmParameterProfileManager::getExpertProfile(QString machineName, QString materialName, double nozzleSize,bool& success)
{
    
    int nozzleSizeInt = 100*nozzleSize;
    
    for (int i = 0; i < defaultProfiles.size(); i++)
    {
        FdmParameterProfile& profile = defaultProfiles[i];
        
        if (machineName == profile.getMachineName() &&
            materialName == profile.getMaterialName() &&
            nozzleSizeInt ==  int(profile.getNozzleSize()*100))
        {
            //qDebug() << "get " << machineName << "&" << materialName << " parameter package";
            success = true;
            return &profile;
        }
    }
    
    auto newProfile = newDefaultProfile(machineName, materialName);
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(materialName);
    newProfile->updateMachine(machineProfile);
    newProfile->updateMaterial(materialProfile);
    
    newProfile->updateNozzleSize(nozzleSize);
    return newProfile;
}


FdmParameterProfile *FdmParameterProfileManager::newDefaultProfile(QString machineName, QString materialName)
{
    //qDebug() << "getCurrentMachineName() ,getCurrentMaterialName : " << getCurrentMachineName() << "," << getCurrentMaterialName() ;

    QString fileName = QUuid::createUuid().toString();

    FdmParameterProfile newProfile(*baseProfile);
    //newProfile.regenMetaData();
    newProfile.setGroup(ProfileGroup::Default);
    newProfile.setVisible(false);
    newProfile.addStatus(EProfileStatus::NewFile);
    auto profileName = QString("%1_%2_%3").arg(machineName, materialName, fileName);
    newProfile.setName(profileName);
    
    newProfile.setCreateTimeNow();
    newProfile.setModifyTimeNow();

    
    auto validFullName = QDir(getDefaultProfilePath()).absoluteFilePath(fileName) + ".ini";
    newProfile.setDataSource(validFullName);
    
    return this->addDefault(newProfile);

}

void createProfile(const QString &profileName)
{

    
}


QStringList FdmParameterProfileManager::getDefaultProfileNameList()
{
    QStringList list;
    list << AkConst::ProfileName::EXPERT_MODE;


//    QList<profileSortItem> sortItems;
//    convertToSortItems(defaultProfiles,sortItems);
//    sortByTimestamp(sortItems);
//    list << getNameList(sortItems);

    return list;
}


QStringList FdmParameterProfileManager::getAllProfileNameList()
{
    
    QList<profileSortItem> sortItems;
    
    //convertToSortItems(defaultProfiles,sortItems);
    convertToSortItems(customProfiles,sortItems);
    sortByTimestamp(sortItems);
    return getNameList(sortItems);
}

FdmRealTimeProfile *FdmParameterProfileManager::getRightRealTimeProfile()
{
    return rightRealTimeProfile;
}

