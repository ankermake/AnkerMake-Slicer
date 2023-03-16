#include "fdmparameterprofileservice.h"
#include "service/fdmparameterprofileservice.h"
#include "profilemanage/fdmparameterprofilemanager.h"
#include "profilemanage/fdmmachineprofilemanager.h"
#include "profilemanage/fdmmaterialprofilemanager.h"
#include "service/fdmmaterialprofileservice.h"
#include "service/fdmmachineprofileservice.h"
#include "fdmpreferencedialogservice.h"

#include "../filemanage/fdmqmlsourcetree.h"
#include "../profilemanage/ifdmprofile.h"
#include "common/ak_const.h"
#ifdef __APPLE__
#include <unistd.h>
#endif

void FdmParameterProfileService::init()
{
    //FdmQmlTreeApi& preferenceTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference();
    //connect(&preferenceTree, &FdmQmlTreeApi::anyNodeValueChange, this, &FdmParameterProfileService::onSourceTreeApiNodeValueChanged);

    connect(FdmMachineProfileService::instance(), &FdmMachineProfileService::customMachineRenamed, this, &FdmParameterProfileService::doCustomMachineRenamed);
    connect(FdmMachineProfileService::instance(), &FdmMachineProfileService::customMachineDeleted, this, &FdmParameterProfileService::doCustomMachineDeleted);
    connect(FdmMaterialProfileService::instance(), &FdmMaterialProfileService::customMaterialRenamed, this, &FdmParameterProfileService::doCustomMaterialRenamed);
    connect(FdmMaterialProfileService::instance(), &FdmMaterialProfileService::customMaterialDeleted, this, &FdmParameterProfileService::doCustomMaterialDeleted);

    connect(FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::operateComplete, this, &FdmParameterProfileService::doOperateComplete);
}


QStringList FdmParameterProfileService::getDefaultParameterList() 
{
    return FdmParameterProfileManager::Instance().getDefaultProfileNameList();
}

QStringList FdmParameterProfileService::getCustomParameterList() 
{
    return FdmParameterProfileManager::Instance().getCustomProfileNameList();
}

QString FdmParameterProfileService::getCurrentSelectedName() 
{
    return m_currentProfileName;
}

QStringList FdmParameterProfileService::getAllParameterList() 
{
    QStringList resultList;
    resultList << AkConst::ProfileName::SIMPLE_MODE;
    resultList << AkConst::ProfileName::EXPERT_MODE;
    QStringList list = FdmParameterProfileManager::Instance().getAllProfileNameList();
    resultList << list;
    return resultList;
}


int FdmParameterProfileService::getExtruderCount()
{
    //return FdmParameterProfileManager::Instance().();
    QString machineName = getMachineName();
    auto profile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
    if (NULL == profile)
    {
        return 1;
    }

    return profile->getExtruderCount();
}

QStringList FdmParameterProfileService::getMachineList() 
{
    return FdmMachineProfileManager::Instance().getAllProfileNameList();
}

QStringList FdmParameterProfileService::getMaterialList() 
{
    QStringList resultList;
    QStringList list =  FdmMaterialProfileManager::Instance().getAllProfileNameList();
    resultList << list;
    if (getNozzleSize() == AkConst::NozzleSizeName::SIZE8)
    {
        resultList.removeOne(AkConst::MaterialName::TPU);
    }
    return resultList;
}


QString FdmParameterProfileService::getMachineName() 
{
    if (m_currentMachineName.isEmpty())
    {
        return m_currentMachineName;
    }

    
    auto profile = FdmMachineProfileManager::Instance().getProfileByName(m_currentMachineName);
    if (NULL == profile)
    {
        m_currentMachineName = "";
    }
    return m_currentMachineName;
}

QString FdmParameterProfileService::getMaterialName() 
{
    if (m_currentMaterialName.isEmpty())
    {
        return m_currentMaterialName;
    }

    
    auto profile = FdmMaterialProfileManager::Instance().getProfileByName(m_currentMaterialName);
    if (NULL == profile)
    {
        m_currentMaterialName = "";
    }
    return m_currentMaterialName;
}

QList<ParamterItem> FdmParameterProfileService::getCustomItemList()
{
    QList<ParamterItem> resultList;
    ParamterItem item;
    for (int i = 0; i < 10; i++)
    {
        item.name = "test0"+i;
        item.status = i%3;
        resultList.push_back(item);
    }
    return resultList;
}


void FdmParameterProfileService::onMachineNameChanged(const QString name)
{
    
    if (nullptr == m_currentProfile)
    {
        return;
    }
    //m_currentProfile->setStatus(EProfileStatus::NodeValueChanged);
    m_currentProfile->setModifyTimeNow();
    m_currentProfile->addStatus(EProfileStatus::NodeValueChangedManually);
    
    if (m_currentProfileName != AkConst::ProfileName::EXPERT_MODE)
    {
        selectMachine(name);
    }
    
    else
    {
        m_currentMachineName = name;
        selectProfile(m_currentProfileName);
    }
}

void FdmParameterProfileService::onMaterialNameChanged(const QString name)
{
    
    if (nullptr == m_currentProfile)
    {
        return;
    }
    //m_currentProfile->setStatus(EProfileStatus::NodeValueChanged);
    m_currentProfile->setModifyTimeNow();
    m_currentProfile->addStatus(EProfileStatus::NodeValueChangedManually);

    
    if (m_currentProfileName != AkConst::ProfileName::EXPERT_MODE)
    {
        selectMaterial(name);
    }
    
    else
    {
        m_currentMaterialName = name;
        selectProfile(m_currentProfileName);
    }
    emit nozzleSizeListChanged();
}

void FdmParameterProfileService::deleteCustomParameter(QString parameterName)
{
    m_currentProfileName = FdmParameterProfileManager::Instance().removeProfile(parameterName);
    selectProfile(m_currentProfileName);
}

void FdmParameterProfileService::renameCustomParameter(QString oldParameterName, QString newParameterName)
{
    FdmParameterProfileManager::Instance().renameProfile(oldParameterName,newParameterName);
    selectProfile(newParameterName);
}

void FdmParameterProfileService::createParameter(QString parameterName)
{
    qDebug() << " FdmParameterProfileService::createParameter clicked";

    auto newProfile = FdmParameterProfileManager::Instance().createProfile(parameterName, m_currentProfile);
    if (nullptr == newProfile)
    {
        return;
    }

    selectProfile(newProfile->getName());
}

void FdmParameterProfileService::onImportBtnClicked(QString profileFullName)
{
    
    QList<FdmProfileCategory> rawContent;

    FdmParameterProfileManager::Instance().readProfile(profileFullName, rawContent);
    
    if (rawContent.size() < 2)
    {
        return;
    }
    
    //QString nameInProfile = "newProfile";
    //for(int i=0;i<rawContent.size();i++)
    //{
    //    if (rawContent[i].name != AkConst::Category::AK_META)
    //    {
    //        continue;
    //    }
    //    if (rawContent[i].contains(AkConst::SettingKey::META_PROFILE_NAME))
    //    {
    //        nameInProfile = rawContent[i].get(AkConst::SettingKey::META_PROFILE_NAME).toString();
    //    }
    //    break;
    //}
    //QString fileName = fInfo.fileName();
    //if (fileName.toLower() == "expert.ini")
    //{
    //    nameInProfile = "Expert";
    //}

    QFileInfo fInfo(profileFullName);
    
    //QString nameInProfile = fInfo.baseName().replace(QRegExp("\\s+"),"_");
    QString nameInProfile = fInfo.completeBaseName().replace(QRegExp("\\s+"), "_");

    
    auto vaildName = FdmParameterProfileManager::Instance().getValidName(nameInProfile);

    auto newProfile = FdmParameterProfileManager::Instance().createProfile(vaildName, rawContent);

    
    auto materialName = newProfile->getMaterialName();
    auto meterialProfile = FdmMaterialProfileManager::Instance().getProfileByName(materialName);
    if (!materialName.isEmpty() && meterialProfile == nullptr)
    {
        QList<FdmProfileCategory> categories;
        auto pCategory = newProfile->getCategory(AkConst::Category::MATERIAL);
        
        
        //if (pCategory != nullptr) { categories.push_back(*pCategory); }
        auto materialProfileTemplate = FdmMaterialProfileManager::Instance().getProfileByName(AkConst::MaterialName::PLAPLUS);
        if (pCategory != nullptr) {
            FdmProfileCategory materialCategory;
            materialCategory.name = pCategory->name;
            const QList<FdmSettingItem>& items = pCategory->getItems();
            for (size_t i = 0; i < items.size(); i++)
            {
                
                if (materialProfileTemplate->getSetting(pCategory->name, items[i].name).isValid())
                {
                    materialCategory.set(items[i].name,items[i].value);
                }
            }
            categories.push_back(materialCategory);
        }

        //materialProfileTemplate->getSetting(AkConst::Category::MATERIAL,)
        //if (pCategory != nullptr) { categories.push_back(*pCategory); }
        auto newMateiralProfile = FdmMaterialProfileManager::Instance().createProfile(materialName, categories);
        newMateiralProfile->setVisible(true);
    }

    
    auto machineName = newProfile->getMachineName();
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
    if (!machineName.isEmpty() && machineProfile == nullptr)
    {
        QList<FdmProfileCategory> categories;
        auto pCategory = newProfile->getCategory(AkConst::Category::MACHINE_SETTINGS);

        
        
        //if (pCategory != nullptr) { categories.push_back(*pCategory); }
        //auto pExtruderCategory = newProfile->getCategory(AkConst::Category::EXTRUDER_SETTINGS);
        //if (pExtruderCategory != nullptr) { categories.push_back(*pExtruderCategory); }
        auto machineProfileTemplate = FdmMachineProfileManager::Instance().getProfileByName(AkConst::MachineName::M5);
        if (pCategory != nullptr) {
            FdmProfileCategory machineCategory;
            machineCategory.name = pCategory->name;
            const QList<FdmSettingItem>& items = pCategory->getItems();
            for (size_t i = 0; i < items.size(); i++)
            {
                
                if (machineProfileTemplate->getSetting(pCategory->name, items[i].name).isValid())
                {
                    machineCategory.set(items[i].name,items[i].value);
                }
            }
            categories.push_back(machineCategory);
        }

        auto newMahchineProfile = FdmMachineProfileManager::Instance().createProfile(machineName, categories);
        newMahchineProfile->setVisible(true);
    }

    selectProfile(newProfile->getName());


}


void FdmParameterProfileService::onExportBtnClicked(QString profileFullName)
{
    if (nullptr == m_currentProfile)
    {
        return;
    }

    m_currentProfile->save(profileFullName);
}


void FdmParameterProfileService::exportIniFromTree(QString profileFullName)
{
    
    QFileInfo fInfo(profileFullName);

    QList<FdmProfileCategory> categories;
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().getAll(categories);

    
     QList<FdmProfileCategory> machineCategories;
     for(int i = 0;i< categories.size();i++)
     {
         if (categories[i].name == AkConst::Category::MACHINE_SETTINGS
                 || categories[i].name == AkConst::Category::EXTRUDER_SETTINGS)
         {

             machineCategories.append(categories[i]);
         }
     }
     auto createMachine = [&](QString machineName)->FdmMachineProfile {
         QString trimMachineName = machineName;
         QString profileName = fInfo.dir().absoluteFilePath(trimMachineName.trimmed().replace(" ","") + ".ini");
         qDebug() << "materialCategories.size is " << machineCategories.size() << profileName;
         FdmMachineProfile profile(profileName, machineCategories, ProfileGroup::Default);
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_NAME, machineName);
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_VISIBLE, true);
         profile.setCreateTimeNow();
         profile.setModifyTimeNow();
         profile.save(true);
         return profile;
     };
     auto x5Profile = createMachine(AkConst::MachineName::M5);
     //createMachine(AkConst::MachineName::X10);

    
     QList<FdmProfileCategory> materialCategories;
     for(int i = 0;i< categories.size();i++)
     {
         if (categories[i].name == AkConst::Category::MATERIAL)
         {
             materialCategories.append(categories[i]);
         }
     }
     auto createMaterial = [&](QString materialName)->FdmMaterialProfile {
         QString trimMaterialName = materialName;
         QString profileName = fInfo.dir().absoluteFilePath(trimMaterialName.trimmed().replace(" ","") + ".ini");
         qDebug() << "materialCategories.size is " << materialCategories.size() << profileName;
         FdmMaterialProfile profile(profileName, materialCategories, ProfileGroup::Default);
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_NAME, materialName);
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_VISIBLE, true);
         profile.setCreateTimeNow();
         profile.setModifyTimeNow();
         profile.save(true);
         return profile;
     };
     
     auto absProfile = createMaterial(AkConst::MaterialName::ABS);
#ifdef _WIN32
     _sleep(2000);
#elif __APPLE__
     sleep(2);
#endif

     auto plaPlusProfile = createMaterial(AkConst::MaterialName::PLAPLUS);
#ifdef _WIN32
     _sleep(2000);
#elif __APPLE__
     sleep(2);
#endif
     auto tpuProfile = createMaterial(AkConst::MaterialName::TPU);
#ifdef _WIN32
     _sleep(2000);
#elif __APPLE__
     sleep(2);
#endif
     auto petgProfile = createMaterial(AkConst::MaterialName::PETG);

     
     auto createParamter = [&](QString paremeterName, FdmMachineProfile& machineProfile, FdmMaterialProfile& materailProfile)->FdmParameterProfile {
         QString trimParemeterName = paremeterName;
         QString profileName = fInfo.dir().absoluteFilePath(trimParemeterName.trimmed().replace(" ","") + ".ini");
         qDebug() << "materialCategories.size is " << materialCategories.size() << profileName;
         FdmParameterProfile profile(profileName, categories, ProfileGroup::Default);
         
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_NAME, "system_expert_"+  paremeterName +"_" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
         profile.setMetaData(AkConst::SettingKey::META_PROFILE_VISIBLE, true);
         profile.setMetaData(AkConst::SettingKey::META_CURRENT_MACHINE_NAME, machineProfile.getName());
         profile.setMetaData(AkConst::SettingKey::META_CURRENT_MATERIAL_NAME, materailProfile.getName());
         profile.setCreateTimeNow();
         profile.setModifyTimeNow();

         profile.updateMachine(&machineProfile);
         profile.updateMaterial(&materailProfile);
         profile.save(true);
         return profile;
     };
     
     createParamter(AkConst::ProfileName::X5_ABS, x5Profile, absProfile);
     createParamter(AkConst::ProfileName::X5_PLA_PLUS, x5Profile, plaPlusProfile);
     createParamter(AkConst::ProfileName::X5_PETG, x5Profile, petgProfile);
     createParamter(AkConst::ProfileName::X5_TPU, x5Profile, tpuProfile);
}

FdmParameterProfile* FdmParameterProfileService::getCurrentProifle()
{
    return m_currentProfile;
}

void FdmParameterProfileService::setCurrentProfile(QString name)
{
    m_currentProfileName = name;
}

void FdmParameterProfileService::setCurrentMachine(QString name)
{
    m_currentMachineName = name;
}

void FdmParameterProfileService::setCurrentMaterial(QString name)
{
    m_currentMaterialName = name;
}

void FdmParameterProfileService::setNozzleSize(double nozzleSize)
{
    m_nozzleSize = nozzleSize;
}


void FdmParameterProfileService::currentParameterChanged(QString parameterName)
{
    selectProfile(parameterName);
}


void FdmParameterProfileService::refreshTree()
{
    //auto currentProfile = FdmMaterialProfileManager::Instance().getProfileByName(m_currentProfileName);
    //if (currentProfile == NULL)
    //{
    //    return;
    //}
    //auto currentParamProfile = FdmParameterProfileManager::Instance().getCurrentSelectedProfile();
    QList<FdmProfileCategory> categories;
    m_currentProfile->getCategoriesForRefreshTree(categories);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(categories);
}



//void FdmParameterProfileService::getCurrentProfile()
//{
//    
//}

void FdmParameterProfileService::selectProfile(QString name)
{
    m_currentProfileName = name;
    if (m_currentProfileName.isEmpty())
    {
        return;
    }
    bool getExpertSuccess = false;
    
    if (m_currentProfileName == AkConst::ProfileName::EXPERT_MODE
        || m_currentProfileName == AkConst::ProfileName::SIMPLE_MODE)
    {
        
        if (m_currentMachineName.isEmpty())
        {
            m_currentMachineName = AkConst::MachineName::M5;
        }
        if (m_currentMaterialName.isEmpty())
        {
            m_currentMaterialName = AkConst::MaterialName::PLAPLUS;
        }
        m_currentProfile = FdmParameterProfileManager::Instance().getExpertProfile(m_currentMachineName, m_currentMaterialName, m_nozzleSize,getExpertSuccess);
    }
    else
    {
        m_currentProfile = FdmParameterProfileManager::Instance().getProfileByName(m_currentProfileName);
    }

    if(m_currentProfile == nullptr)
    {
        return;
    }

    
    m_currentMachineName = m_currentProfile->getMachineName();
    m_currentMaterialName = m_currentProfile->getMaterialName();
    m_nozzleSize = m_currentProfile->getNozzleSize();

    
    emit ParameterSelected(name);

    
    
    QList<FdmProfileCategory> content;
    
    
    m_currentProfile->getCategoriesForRefreshTree(content);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);
    
    
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);

    
    auto parameterModifyTime = m_currentProfile->getModifyTime();
    
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(m_currentMachineName);
    if (nullptr != machineProfile && machineProfile->getModifyTime() > parameterModifyTime)
    {
        selectMachine(m_currentMachineName);
    }

    
    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(m_currentMaterialName);
    if (nullptr != materialProfile && materialProfile->getModifyTime() > parameterModifyTime)
    {
        selectMaterial(m_currentMaterialName);
    }

    
    if (nullptr == machineProfile)
    {
        selectMachine(AkConst::MachineName::M5);
    }
    if (nullptr == materialProfile)
    {
        selectMaterial(AkConst::MaterialName::PLAPLUS);
    }
    

    
    if ((m_currentProfileName == AkConst::ProfileName::EXPERT_MODE
             || m_currentProfileName == AkConst::ProfileName::SIMPLE_MODE)
         && !getExpertSuccess)
    {
        selectNozzle(m_nozzleSize);
    }

    
    refreshUI();
}

void FdmParameterProfileService::selectMachine(QString name)
{
    m_currentMachineName = name;
    if (m_currentMachineName.isEmpty())
    {
        return;
    }
    m_currentProfile->setMachineName(m_currentMachineName);
    
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(m_currentMachineName);
    QList<FdmProfileCategory> machineContent;
    int activeIdx = 0;
    if (machineActiveTabIdxMap.contains(m_currentMachineName))
    {
        activeIdx = machineActiveTabIdxMap[m_currentMachineName];
    }
    int extruderNum = activeIdx == 0 ? 1 : activeIdx;

    machineProfile->getCategoriesForRefreshTree(extruderNum, machineContent);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(machineContent);

    emit machineNameChanged();
    emit extruderCountChanged();
}


void FdmParameterProfileService::selectNozzle(double nozzleSize)
{
    m_nozzleSize = nozzleSize;

    
    FdmQmlTreeApi& preferenceTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference();
    QList<FdmProfileCategory> categorys;
    FdmProfileCategory category;
    category.name = AkConst::Category::EXTRUDER_SETTINGS;
    category.set(AkConst::SettingKey::MACHINE_NOZZLE_SIZE, m_nozzleSize);
    categorys.append(category);
    preferenceTree.setAny(categorys);

    emit nozzleSizeChanged();
}


void FdmParameterProfileService::selectMaterial(QString name)
{
    m_currentMaterialName = name;
    if (m_currentMaterialName.isEmpty())
    {
        return;
    }
    m_currentProfile->setMaterialName(m_currentMaterialName);
    
    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(m_currentMaterialName);
    QList<FdmProfileCategory> materialContent;
    materialProfile->getCategoriesForRefreshTree(materialContent);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(materialContent);

    emit materialNameChanged();
}

void FdmParameterProfileService::refreshCurrentPage()
{
    selectProfile(m_currentProfileName);
}

void FdmParameterProfileService::clearCookie()
{
    machineActiveTabIdxMap.clear();
}

void FdmParameterProfileService::onExtruderSwitched(int extruderIdx)
{
    if (nullptr == m_currentProfile)
    {
        return;
    }

    //int currentActiveIdx = m_currentProfile->getActiveExtruderIdx();
    
    //if (currentActiveIdx == extruderIdx)
    //{
    //    return;
    //}
    
    machineActiveTabIdxMap[m_currentMachineName] = extruderIdx;
    
    //m_currentProfile->setActiveExtruderIdx(extruderIdx);

    
    auto extruderSetting = m_currentProfile->getExtruderCategory();

    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setCategory(extruderSetting);
}

//void FdmParameterProfileService::onSourceTreeApiNodeValueChanged(const QString& categoryName, const FdmSettingItem& item)
//{
//    qDebug() << "FdmParameterProfileService onSourceTreeApiNodeValueChanged categoryName" << categoryName << "key" << item.name << "value " << item.value;

//    if (m_currentProfileName != AkConst::ProfileName::EXPERT)
//    {
//        return;
//    }
//
//    if (nullptr == m_currentProfile)
//    {
//        qDebug() << "m_currentProfile is null";
//        return;
//    }
//

//    m_currentProfile->setSetting(categoryName, item);
//}


void FdmParameterProfileService::allProfileDoMachineRenamed(QString oldName, QString newName)
{
    auto profiles = FdmParameterProfileManager::Instance().getVisiableCustomProfiles();
    foreach(const auto profile, profiles)
    {
        if (profile->getMachineName() == oldName)
        {
            profile->setMachineName(newName);
        }
    }
}

void FdmParameterProfileService::allProfileDoMaterialRenamed(QString oldName, QString newName)
{
    auto profiles = FdmParameterProfileManager::Instance().getVisiableCustomProfiles();
    foreach(const auto profile, profiles)
    {
        if (profile->getMaterialName() == oldName)
        {
            profile->setMaterialName(newName);
        }
    }
}


void FdmParameterProfileService::doCustomMachineRenamed(QString oldName, QString newName)
{
    if (m_currentMachineName != oldName)
    {
        return;
    }
    m_currentMachineName = newName;
    emit machineNameChanged();
    allProfileDoMachineRenamed(oldName,newName);
}

void FdmParameterProfileService::doCustomMachineDeleted(QString name)
{
    if (m_currentMachineName != name)
    {
        return;
    }
    m_currentMachineName = "";
    emit machineNameChanged();
}

void FdmParameterProfileService::doCustomMaterialRenamed(QString oldName, QString newName)
{
    if (m_currentMaterialName != oldName)
    {
        return;
    }
    m_currentMaterialName = newName;
    emit materialNameChanged();
    allProfileDoMaterialRenamed(oldName,newName);
}

void FdmParameterProfileService::doCustomMaterialDeleted(QString name)
{
    if (m_currentMaterialName != name)
    {
        return;
    }
    m_currentMaterialName = "";
    emit materialNameChanged();
}

void FdmParameterProfileService::doOperateComplete()
{
    
    emit machineListChanged();
    emit materialListChanged();
    emit allParameterListChanged();
}



QString FdmParameterProfileService::getNozzleSize()
{
    //QString defaultValue = AkConst::NozzleSizeName::SIZE4;
    ////auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //auto machineNozzleSize = m_currentProfile->getSetting(AkConst::Category::EXTRUDER_SETTINGS, AkConst::SettingKey::MACHINE_NOZZLE_SIZE);
    //if (machineNozzleSize.isNull())
    //{
    //    return defaultValue;

    int nozzleSize = (int)(m_nozzleSize * 10);
    switch (nozzleSize) {
        case 2: return AkConst::NozzleSizeName::SIZE2;
        case 4: return AkConst::NozzleSizeName::SIZE4;
        case 6: return AkConst::NozzleSizeName::SIZE6;
        case 8: return AkConst::NozzleSizeName::SIZE8;
        default: return AkConst::NozzleSizeName::SIZE4;
    }
}


QStringList FdmParameterProfileService::getNozzleSizeList()
{
//    QStringList resultList;
//    resultList << AkConst::NozzleSizeName::SIZE4;
//    return resultList;
//    QStringList resultList;
//    resultList << AkConst::NozzleSizeName::SIZE2;
//    resultList << AkConst::NozzleSizeName::SIZE4;
//    resultList << AkConst::NozzleSizeName::SIZE6;
//    resultList << AkConst::NozzleSizeName::SIZE8;
//    return resultList;

    QStringList resultList;
    resultList << AkConst::NozzleSizeName::SIZE2;
    resultList << AkConst::NozzleSizeName::SIZE4;
    resultList << AkConst::NozzleSizeName::SIZE6;
    
    if (getMaterialName() != AkConst::MaterialName::TPU)
    {
         resultList << AkConst::NozzleSizeName::SIZE8;
    }
    return resultList;
}


void FdmParameterProfileService::onNozzleSizeChanged(const QString name)
{
    //qDebug() << " FdmParameterProfileService ==" <<name;
    QString nozzleSizeStr  =name;
    if (nozzleSizeStr.isEmpty())
    {
        nozzleSizeStr = AkConst::NozzleSizeName::SIZE4;
    }
    
    auto valueList = nozzleSizeStr.split(" ");
    if (valueList[0].isEmpty())
    {
        return;
    }
    m_nozzleSize = valueList[0].toDouble();

    
    if (m_currentProfileName != AkConst::ProfileName::EXPERT_MODE
        && m_currentProfileName != AkConst::ProfileName::SIMPLE_MODE)
    {
        selectNozzle(m_nozzleSize);
    }
    
    else
    {
        selectProfile(m_currentProfileName);
    }
    
    emit materialListChanged();
}
//
//void FdmParameterProfileService::onExtruderSwitched(int extruderIdx, FdmQmlTreeApi & treeApi)
//{
//    if (nullptr == m_currentProfile)
//    {
//        return;
//    }
//
//    int currentActiveIdx = m_currentProfile->getActiveExtruderIdx();

//    if (currentActiveIdx == extruderIdx)
//    {
//        return;
//    }
//    machineActiveTabIdxMap[currentMachineName] = idx;

//    m_currentProfile->setActiveExtruderIdx(extruderIdx);
//

//    auto extruderSetting = m_currentProfile->getExtruderCategory();
//    treeApi.setCategory(extruderSetting);
//}


//const QString &FdmParameterProfileService::getExportProperName() const
//{
//    return QString("%1_%2_%3").arg(getCurrentSelectedName()).arg(FdmParameterProfileManager::Instance().getCurrentMachineName()).arg(FdmParameterProfileManager::Instance().getCurrentMaterialName());
//}


void FdmParameterProfileService::refreshUI()
{
    emit defaultParameterListChanged();
    emit customParameterListChanged();
    emit currentParameterNameChanged();
    emit machineNameChanged();
    emit materialNameChanged();
    emit machineListChanged();
    emit materialListChanged();
    emit extruderCountChanged();
    emit allParameterListChanged();
    emit nozzleSizeChanged();
    emit nozzleSizeListChanged();
}
