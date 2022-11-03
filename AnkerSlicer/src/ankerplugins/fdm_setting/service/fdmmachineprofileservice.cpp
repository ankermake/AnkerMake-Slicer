#include "fdmmachineprofileservice.h"
#include "profilemanage/fdmmachineprofile.h"
#include "profilemanage/fdmmachineprofilemanager.h"
#include "../filemanage/fdmqmlsourcetree.h"
#include <QDebug>

QStringList FdmMachineProfileService::getDefaultMachineList() 
{
    return FdmMachineProfileManager::Instance().getDefaultProfileNameList();
}

QStringList FdmMachineProfileService::getCustomMachineList() 
{
    return FdmMachineProfileManager::Instance().getCustomProfileNameList();
}

QStringList FdmMachineProfileService::getAllMachineList() 
{
    return FdmMachineProfileManager::Instance().getAllProfileNameList();
}

QString FdmMachineProfileService::getDefaultSelectedName() 
{
    return currentMachineName;
}

void FdmMachineProfileService::refreshUI()
{
    emit defaultMachineListChanged();
    emit customMachineListChanged();
    emit allMachineListChanged();
    emit defaultMachineNameChanged();
}

void FdmMachineProfileService::refreshTree()
{
    auto currentParamProfile = FdmMachineProfileManager::Instance().getProfileByName(currentMachineName);
    if (currentParamProfile == NULL)
    {
        return;
    }
    QList<FdmProfileCategory> categories;
    int activeIdx = 0;
    if (machineActiveTabIdxMap.contains(currentMachineName))
    {
        activeIdx = machineActiveTabIdxMap[currentMachineName];
    }
    int extruderNum = activeIdx== 0? 1: activeIdx;
    currentParamProfile->getCategoriesForRefreshTree(extruderNum, categories);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(categories);
}

void FdmMachineProfileService::selectMachine(QString name)
{
    currentMachineName = name;

    
    qDebug() << "machineSelected()" << name;
    emit machineSelected(name);

    refreshTree();
    refreshUI();
}

void FdmMachineProfileService::refreshCurrentPage()
{
    selectMachine(currentMachineName);
}

FdmMachineProfile* FdmMachineProfileService::getCurrentProifle()
{
    return FdmMachineProfileManager::Instance().getProfileByName(currentMachineName);
}

//
void FdmMachineProfileService::clearCookie()
{
    machineActiveTabIdxMap.clear();
}


void FdmMachineProfileService::deleteCustomMachine(QString machineName)
{
    currentMachineName = FdmMachineProfileManager::Instance().removeProfile(machineName);
    selectMachine(currentMachineName);

    emit customMachineDeleted(machineName);
}

void FdmMachineProfileService::renameCustomMachine(QString oldMachineName, QString newMachineName)
{
    FdmMachineProfileManager::Instance().renameProfile(oldMachineName,newMachineName);
    selectMachine(newMachineName);

    emit customMachineRenamed(oldMachineName, newMachineName);
}


void FdmMachineProfileService::createMachine(QString machineName)
{
    auto currentProfile = FdmMachineProfileManager::Instance().getProfileByName(currentMachineName);
    auto newProfile = FdmMachineProfileManager::Instance().createProfile(machineName, currentProfile);
    if (nullptr == newProfile)
    {
        return;
    }

    selectMachine(newProfile->getName());
}


void FdmMachineProfileService::currentMachineChanged(QString machineName)
{
    selectMachine(machineName);
}



void FdmMachineProfileService::onTabSwitched(int idx)
{
    
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(currentMachineName);
    if (NULL == machineProfile)
    {
        return;
    }
    machineActiveTabIdxMap[currentMachineName] = idx;

    
    //machineProfile->setActiveComponentIdx(idx);

    QList<FdmProfileCategory> categories;
    machineProfile->getComponentCategories(idx, categories);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(categories);
}
