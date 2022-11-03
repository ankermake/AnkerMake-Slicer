////#include "preferencesparamters.h"

//////class FdmProfileServerAndrew;

////#include "profilemanage/fdmmachineprofilemanager.h"
////#include "profilemanage/fdmmaterialprofilemanager.h"
////#include "profilemanage/fdmparameterprofilemanager.h"

////#include "filemanage/fdmqmlsourcetree.h"

////FdmProfileServer::FdmProfileServer(QObject *parent)
////    : QObject(parent)
////{

////}


////const QStringList FdmProfileServer::getDefaultMaterialList() const
////{
////     return FdmMaterialProfileManager::Instance().getDefaultProfileNameList();
////}

////const QStringList FdmProfileServer::getCustomMaterialList() const
////{
////    return FdmMaterialProfileManager::Instance().getCustomProfileNameList();
////}

////const QStringList FdmProfileServer::getAllMaterialList() const
////{
////    return FdmMaterialProfileManager::Instance().getAllProfileNameList();
////}

////const QStringList FdmProfileServer::getDefaultMachineList() const
////{
////    return FdmMachineProfileManager::Instance().getDefaultProfileNameList();
////}

////const QStringList FdmProfileServer::getCustomMachineList() const
////{
////    return FdmMachineProfileManager::Instance().getCustomProfileNameList();
////}

////const QStringList FdmProfileServer::getAllMachineList() const
////{
////    return FdmMachineProfileManager::Instance().getAllProfileNameList();
////}

////const QStringList FdmProfileServer::getDefaultParameterList() const
////{
////    return FdmParameterProfileManager::Instance().getDefaultProfileNameList();
////}

////const QStringList FdmProfileServer::getCustomParameterList() const
////{
////   return FdmParameterProfileManager::Instance().getCustomProfileNameList();
////}

////const QStringList FdmProfileServer::getAllParameterList() const
////{
////    return FdmParameterProfileManager::Instance().getAllProfileNameList();
////}


////const QString FdmProfileServer::getCurrentMaterialName() const
////{
////    return FdmParameterProfileManager::Instance().getCurrentMaterialName();
////}


////const QString FdmProfileServer::getCurrentMachineName() const
////{
////     return FdmParameterProfileManager::Instance().getCurrentMachineName();
////}


////const QString FdmProfileServer::getCurrentParameterName() const
////{
////    return FdmParameterProfileManager::Instance().getCurrentSelectedName();
////}


////void FdmProfileServer::deleteCustomMaterial(QString materialName)
////{
////    FdmMaterialProfileManager::Instance().removeProfile(materialName);
////    emit customMaterialListChanged();
////    emit allMaterialListChanged();
////}

////void FdmProfileServer::renameCustomMaterial(QString oldMaterialName, QString newMaterialName)
////{
////    FdmMaterialProfileManager::Instance().renameProfile(oldMaterialName, newMaterialName);
////    emit customMaterialListChanged();
////    emit allMaterialListChanged();
////}

////void FdmProfileServer::createMaterial(QString materialName)
////{
////    FdmMaterialProfileManager::Instance().createProfile(materialName);
////    emit customMaterialListChanged();
////    emit allMaterialListChanged();
////}


////void FdmProfileServer::currentMaterialChanged(QString materialName)
////{

////    auto profile = FdmMaterialProfileManager::Instance().getProfileByName(materialName);
////    if (NULL == profile )
////    {
////        return;
////    }
//<<<<<<< HEAD

////    QList<FdmProfileCategory> categories;
////    profile->getContent(categories);
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(categories);
////}

////void FdmProfileServer::deleteCustomMachine(QString machineName)
////{
////    FdmMachineProfileManager::Instance().removeProfile(machineName);
////    emit customMachineListChanged();
////    emit allMachineListChanged();

////}

////void FdmProfileServer::renameCustomMachine(QString oldMachineName, QString newMachineName)
////{
////    FdmMachineProfileManager::Instance().renameProfile(oldMachineName, newMachineName);
////    emit customMachineListChanged();
////    emit allMachineListChanged();

////}

////void FdmProfileServer::createMachine(QString machineName)
////{
////    FdmMachineProfileManager::Instance().createProfile(machineName);
////    emit customMachineListChanged();
////    emit allMachineListChanged();
////}

////void FdmProfileServer::currentMachineChanged(QString machineName)
////{

////    auto profile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
//=======

//    //auto content = profile->getContent();
//    QList<FdmProfileCategory> content;
//    profile.getContent(content);

//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);
//    FdmParameterProfileManager::Instance().setCurrentParameterProfileName(parameterName);
//}


//void FdmProfileServer::onImportBtnClicked(QString profileFullName)
//{

//    QList<FdmProfileCategory> rawContent;

//    FdmParameterProfileManager::Instance().readProfile(profileFullName, rawContent);

//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(rawContent);

//    QList<FdmProfileCategory> content;
//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getAll(content);



//    QString uniqueName = FdmParameterProfileManager::Instance().getValidName(profileFullName);
//    FdmParameterProfileManager::Instance().createProfile(uniqueName, content);
//    FdmParameterProfileManager::Instance().setCurrentParameterProfileName(uniqueName);


//    emit customParameterListChanged();
//    emit allParameterListChanged();
//}

//void FdmProfileServer::onExportBtnClicked(QString profileFullName)
//{

//    QList<FdmProfileCategory> content;
//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getAll(content);


//    FdmParameterProfileManager::Instance().refreshCurrentProfile(content);

//    FdmParameterProfileManager::Instance().exportCurrentProfile(profileFullName);
//}


//void FdmProfileServer::onMachineComboxItemSelected(QString machineName)
//{

//}

//void FdmProfileServer::onMaterialComboxItemSelected(QString materialName)
//{

//}

//void FdmProfileServer::onParameterExtruderSwitched(int extruderIdx)
//{
//    auto currentProfile = FdmParameterProfileManager::Instance().getSelectedProfile();
//    int currentActiveIdx = currentProfile->getActiveExtruderIdx();

//    if (currentActiveIdx == extruderIdx)
//    {
//        return;
//    }


//    //QString extruderCategoryName = QString("extruder_settings_%1").arg(currentActiveIdx);
//    FdmProfileCategory category;
//    category.name = QString("extruder_settings");
//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getCategory(category);
//    if (category.isEmpty())
//    {
//        return;
//    }


//    currentProfile->setExtruderSetting(currentActiveIdx, category);


//    currentProfile->setActiveExtruderIdx(extruderIdx);
//}



//void FdmProfileServer::onPreferenceWindowOpen()
//{
//    FdmParameterProfileManager::Instance().back();
//    FdmMachineProfileManager::Instance().back();
//    FdmMaterialProfileManager::Instance().back();
//}


//void FdmProfileServer::onMachineExtruderSwitched(int extruderIdx)
//{
//    auto currentProfile = FdmMachineProfileManager::Instance().getSelectedProfile();
//    int currentActiveIdx = currentProfile->getActiveExtruderIdx();

//    if (currentActiveIdx == extruderIdx)
//    {
//        return;
//    }


//    FdmProfileCategory category ;
//    category.name = QString("extruder_settings_%1").arg(currentActiveIdx);
//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getCategory(category);

//    currentProfile->setExtruderSetting(currentActiveIdx, category);


//    currentProfile->setActiveExtruderIdx(extruderIdx);

//    auto extruderSetting = currentProfile->getExtruderSetting(extruderIdx);

//    //extruderSetting->name =
//    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setCategory(*extruderSetting);

//}


//void FdmProfileServer::onSaveBtnClicked()
//{

//    FdmParameterProfileManager::Instance().commit();
//    FdmMachineProfileManager::Instance().commit();
//    FdmMaterialProfileManager::Instance().commit();


//    QString currentProfileName = FdmParameterProfileManager::Instance().getCurrentParameterProfileName();
//    FdmParameterProfile profile;
//    FdmParameterProfileManager::Instance().getProfile(currentProfileName, profile);
//>>>>>>> 202cf988bde92026a6d74f193017293de08fffc8
////    if (NULL == profile )
////    {
////        return;
////    }

////    QList<FdmProfileCategory> content;
////    profile->getContent(content);

////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);
////}



////void FdmProfileServer::deleteCustomParameter(QString parameterName)
////{
////    FdmParameterProfileManager::Instance().removeProfile(parameterName);
////    emit customParameterListChanged();
////    emit allParameterListChanged();
////}

////void FdmProfileServer::renameCustomParameter(QString oldParameterName, QString newParameterName)
////{
////    FdmParameterProfileManager::Instance().renameProfile(oldParameterName, newParameterName);
////    emit customParameterListChanged();
////    emit allParameterListChanged();
////}

////void FdmProfileServer::createParameter(QString parameterName)
////{
////    FdmParameterProfileManager::Instance().createProfile(parameterName);
////    emit customParameterListChanged();
////    emit allParameterListChanged();
////}

////void FdmProfileServer::currentParameterChanged(QString parameterName)
////{

////    FdmParameterProfile profile;
////    FdmParameterProfileManager::Instance().getProfile(parameterName, profile);

////    //FdmParameterProfileManager::Instance().getProfile(parameterName);
//////    if (NULL == profile )
//////    {
//////        return;
//////    }

////    //auto content = profile->getContent();
////    QList<FdmProfileCategory> content;
////    profile.getContent(content);

////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);
////    FdmParameterProfileManager::Instance().setCurrentParameterProfileName(parameterName);
////}


////void FdmProfileServer::onImportBtnClicked(QString profileFullName)
////{

////    QList<FdmProfileCategory> rawContent;

////    FdmParameterProfileManager::Instance().readProfile(profileFullName, rawContent);

////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(rawContent);

////    QList<FdmProfileCategory> content;
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getAll(content);



////    QString uniqueName = FdmParameterProfileManager::Instance().getValidName(profileFullName);
////    FdmParameterProfileManager::Instance().createProfile(uniqueName, content);
////    FdmParameterProfileManager::Instance().setCurrentParameterProfileName(uniqueName);


////    emit customParameterListChanged();
////    emit allParameterListChanged();
////}

////void FdmProfileServer::onExportBtnClicked(QString profileFullName)
////{

////    QList<FdmProfileCategory> content;
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getAll(content);


////    FdmParameterProfileManager::Instance().refreshCurrentProfile(content);

////    FdmParameterProfileManager::Instance().exportCurrentProfile(profileFullName);
////}


////void FdmProfileServer::onMachineComboxItemSelected(QString machineName)
////{

////}

////void FdmProfileServer::onMaterialComboxItemSelected(QString materialName)
////{

////}

////void FdmProfileServer::onParameterExtruderSwitched(int extruderIdx)
////{
////    auto currentProfile = FdmParameterProfileManager::Instance().getSelectedProfile();
////    int currentActiveIdx = currentProfile->getActiveExtruderIdx();

////    if (currentActiveIdx == extruderIdx)
////    {
////        return;
////    }


////    //QString extruderCategoryName = QString("extruder_settings_%1").arg(currentActiveIdx);
////    QString extruderCategoryName = QString("extruder_settings");
////    FdmProfileCategory category;
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getCategory(extruderCategoryName,category);
////    if (category.isEmpty())
////    {
////        return;
////    }


////    currentProfile->setExtruderCategory(currentActiveIdx, category);


////    currentProfile->setActiveExtruderIdx(extruderIdx);
////}



////void FdmProfileServer::onPreferenceWindowOpen()
////{
////    FdmParameterProfileManager::Instance().backup();
////    FdmMachineProfileManager::Instance().backup();
////    FdmMaterialProfileManager::Instance().backup();
////}


////void FdmProfileServer::onMachineExtruderSwitched(int extruderIdx)
////{
////    auto currentProfile = FdmMachineProfileManager::Instance().getSelectedProfile();
////    int currentActiveIdx = currentProfile->getActiveExtruderIdx();

////    if (currentActiveIdx == extruderIdx)
////    {
////        return;
////    }


////    QString extruderCategoryName = QString("extruder_settings_%1").arg(currentActiveIdx);
////    FdmProfileCategory category ;
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().getCategory(extruderCategoryName, category);

////    currentProfile->setExtruderSetting(currentActiveIdx, category);


////    currentProfile->setActiveExtruderIdx(extruderIdx);

////    auto extruderSetting = currentProfile->getExtruderSetting(extruderIdx);

////    //extruderSetting->name =
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setCategory(*extruderSetting);

////}


////void FdmProfileServer::onSaveBtnClicked()
////{

////    FdmParameterProfileManager::Instance().commit();
////    FdmMachineProfileManager::Instance().commit();
////    FdmMaterialProfileManager::Instance().commit();


////    QString currentProfileName = FdmParameterProfileManager::Instance().getCurrentSelectedName();
////    FdmParameterProfile profile;
////    FdmParameterProfileManager::Instance().ge(currentProfileName, profile);
//////    if (NULL == profile )
//////    {
//////        return;
//////    }


////    QList<FdmProfileCategory> content;

////    profile.getAllCategories(content);


////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(content);
////}


////void FdmProfileServer::onCloseBtnClicked()
////{
////    FdmParameterProfileManager::Instance().rollback();
////    FdmMachineProfileManager::Instance().rollback();
////    FdmMaterialProfileManager::Instance().rollback();


////    QString currentProfileName = FdmParameterProfileManager::Instance().getCurrentParameterProfileName();
////    //auto profile = FdmParameterProfileManager::Instance().getProfile(currentProfileName);
////    FdmParameterProfile profile;
////    FdmParameterProfileManager::Instance().getProfile(currentProfileName, profile);

//////    if (NULL == profile )
//////    {
//////        return;
//////    }

////    QList<FdmProfileCategory> content ;
////    profile.getContent(content);
////    //auto content = profile->getContent();
////    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(content);

////}

////void FdmProfileServer::onSaveParameterBtnClicked()
////{

////}

////void FdmProfileServer::onSaveParameterAsBtnClicked()
////{

////}
