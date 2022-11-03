#include "fdmmaterialprofileservice.h"
#include "../profilemanage/fdmmaterialprofilemanager.h"
#include "../filemanage/fdmqmlsourcetree.h"
#include <QDebug>

QStringList FdmMaterialProfileService::getDefaultMaterialList() 
{
    //qDebug() << "FdmMaterialProfileService::getDefaultMaterialList() ";
    return FdmMaterialProfileManager::Instance().getDefaultProfileNameList();
}

QStringList FdmMaterialProfileService::getCustomMaterialList() 
{
    return FdmMaterialProfileManager::Instance().getCustomProfileNameList();
}

QStringList FdmMaterialProfileService::getAllMaterialList() 
{
    return FdmMaterialProfileManager::Instance().getAllProfileNameList();
}

FdmMaterialProfile* FdmMaterialProfileService::getCurrentProifle()
{
    return FdmMaterialProfileManager::Instance().getProfileByName(currentMaterialName);
}

 QString FdmMaterialProfileService::getCurrentSelectedName()
 {
     //refreshTree();
     //refreshUI();
     return currentMaterialName;
 }

 void FdmMaterialProfileService::refreshUI()
{
    emit defaultMaterialListChanged();
    emit customMaterialListChanged();
    emit allMaterialListChanged();
    emit defaultMaterialNameChanged();
}

void FdmMaterialProfileService::refreshTree()
{
    auto currentProfile = FdmMaterialProfileManager::Instance().getProfileByName(currentMaterialName);
    if (currentProfile == NULL)
    {
        return;
    }
    QList<FdmProfileCategory> categories;
    currentProfile->getCategoriesForRefreshTree(categories);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(categories);
}

void FdmMaterialProfileService::selectMaterial(QString name)
{
    currentMaterialName = name;
    qDebug() << "materialSelected()" << currentMaterialName;

    
    emit materialSelected(name);
    refreshTree();
    refreshUI();

}

void FdmMaterialProfileService::refreshCurrentPage()
{
    selectMaterial(currentMaterialName);
}

void FdmMaterialProfileService::deleteCustomMaterial(QString materialName)
{
    currentMaterialName = FdmMaterialProfileManager::Instance().removeProfile(materialName);
    selectMaterial(currentMaterialName);

    emit customMaterialDeleted(materialName);
}

void FdmMaterialProfileService::renameCustomMaterial(QString oldMaterialName, QString newMaterialName)
{
    
    qDebug() << "rename from " << oldMaterialName << "to" <<newMaterialName;
    FdmMaterialProfileManager::Instance().renameProfile(oldMaterialName,newMaterialName);

    selectMaterial(newMaterialName);

    emit customMaterialRenamed(oldMaterialName, newMaterialName);
}

void FdmMaterialProfileService::createMaterial(QString materialName)
{
    auto currentProfile = FdmMaterialProfileManager::Instance().getProfileByName(currentMaterialName);
    auto newProfile = FdmMaterialProfileManager::Instance().createProfile(materialName, currentProfile);
    if (nullptr == newProfile)
    {
        return;
    }

    selectMaterial(newProfile->getName());
}

void FdmMaterialProfileService::currentMaterialChanged(QString materialName)
{
    selectMaterial(materialName);
}
