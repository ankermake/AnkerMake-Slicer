#include "fdmpreferencedialogservice.h"
#include "fdmparameterprofileservice.h"
#include "fdmmachineprofileservice.h"
#include "fdmmaterialprofileservice.h"
#include "fdmrightparameterservice.h"

#include "profilemanage/fdmmachineprofilemanager.h"
#include "profilemanage/fdmmaterialprofilemanager.h"
#include "profilemanage/fdmparameterprofilemanager.h"

#include "filemanage/fdmqmlsourcetree.h"
#include "../profilemanage/operlogcollector.h"
#include <QDebug>
#include "common/utilities/tlogger.h"
using namespace AkUtil;


void FdmPreferenceDialogService::init()
{
    openSource = AkConst::EOpenPreferenceSource::Init;
    activeProfile = NULL;
    connect(FdmMachineProfileService::instance(),&FdmMachineProfileService::machineSelected,this,&FdmPreferenceDialogService::onCurrentMachineNameChanged);
    connect(FdmMaterialProfileService::instance(),&FdmMaterialProfileService::materialSelected,this,&FdmPreferenceDialogService::onCurrentMaterialNameChanged);
    connect(FdmParameterProfileService::instance(), &FdmParameterProfileService::ParameterSelected,this, &FdmPreferenceDialogService::onCurrentParameterNameChanged);

    
    connect(&(FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference()), &FdmQmlTreeApi::anyNodeValueChange, this, &FdmPreferenceDialogService::onSourceTreeApiNodeValueChanged);
    
    connect(&(FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference()), &FdmQmlTreeApi::anyNodeValueChangeByUI, this, &FdmPreferenceDialogService::onSourceTreeApiNodeValueChangedByUI);
}



void FdmPreferenceDialogService::onPreferenceWindowOpen(int idx)
{
    //qDebug() << "------------onPreferenceWindowOpen backup-----------";
    TFunction(QString("idx=%1 .backup begin").arg(idx));
    FdmParameterProfileManager::Instance().backup();
    FdmMachineProfileManager::Instance().backup();
    FdmMaterialProfileManager::Instance().backup();

    
    if (openSource != AkConst::EOpenPreferenceSource::RightSetting)
    {
        
        auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
        auto machineName = realtimeProfile->getMachineName();
        if (machineName.isEmpty())
        {
            machineName = AkConst::MachineName::M5;
        }
        FdmMachineProfileService::instance()->clearCookie();
        FdmMachineProfileService::instance()->selectMachine(machineName);

        auto materialName = realtimeProfile->getMaterialName();
        if (materialName.isEmpty())
        {
            materialName = AkConst::MaterialName::ABS;
        }
        FdmMaterialProfileService::instance()->selectMaterial(materialName);

        
        auto profileName = realtimeProfile->getProfileName();
        if (profileName.isEmpty() || profileName == AkConst::ProfileName::SIMPLE_MODE)
        {
            profileName = AkConst::ProfileName::EXPERT_MODE;;
        }
        
        if (profileName == AkConst::ProfileName::EXPERT_MODE)
        {
            FdmParameterProfileService::instance()->setCurrentMachine(realtimeProfile->getMachineName());
            FdmParameterProfileService::instance()->setCurrentMaterial(realtimeProfile->getMaterialName());
        }
        FdmParameterProfileService::instance()->selectProfile(profileName);

        
        if (idx == AkConst::EWidgetType::Machine)
        {
            FdmMachineProfileService::instance()->selectMachine(machineName);
        }
        else if (idx == AkConst::EWidgetType::Material)
        {
            FdmMaterialProfileService::instance()->selectMaterial(materialName);
        }
        else if (idx == AkConst::EWidgetType::Parameter)
        {
            FdmParameterProfileService::instance()->selectProfile(profileName);
        }
    }

}

void FdmPreferenceDialogService::setSource(AkConst::EOpenPreferenceSource source)
{
    openSource = source;
}

void FdmPreferenceDialogService::onPreferencesPageSwitch(int idx)
{
    switch (idx)
    {
        case AkConst::EWidgetType::Machine:
        {
            FdmMachineProfileService::instance()->refreshCurrentPage();
            break;
        }
        case AkConst::EWidgetType::Material:
        {
            FdmMaterialProfileService::instance()->refreshCurrentPage();
            break;
        }
        case AkConst::EWidgetType::Parameter:
        {
            FdmParameterProfileService::instance()->refreshCurrentPage();
            break;
        }
    }
}


void FdmPreferenceDialogService::onSaveBtnClicked()
{
    //qDebug() << "onSaveBtnClicked";
    TFunction(" commit oper");

    
    auto operLogPtr = OperLogCollector::instance()->getLog();
    TDebug(QString("Send OperLog to right param service: ") + operLogPtr->toString());

    FdmParameterProfileManager::Instance().commit();
    FdmMachineProfileManager::Instance().commit();
    FdmMaterialProfileManager::Instance().commit();

    TDebug("oper commit over");
    
    emit sendOperLog(operLogPtr);
    
    openSource = AkConst::EOpenPreferenceSource::Init;
    
    emit operateComplete();

    return;

    //syncCurrentParameterFromMangerToTree();

    
    //FdmParameterProfileService::instance()->refreshUI();
    //FdmRightParameterService::instance()->refreshUI();
    //FdmMachineProfileService::instance()->refreshUI();
    //FdmMaterialProfileService::instance()->refreshUI();
}


void FdmPreferenceDialogService::onCloseBtnClicked()
{
    //qDebug() << "onCloseBtnClicked";
    TFunction(" rollback oper");
    FdmParameterProfileManager::Instance().rollback();
    FdmMachineProfileManager::Instance().rollback();
    FdmMaterialProfileManager::Instance().rollback();

    openSource = AkConst::EOpenPreferenceSource::Init;

    emit operateComplete();

    
    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //auto currentProfieName = realtimeProfile->getProfileName();
    //if (currentProfieName.isEmpty())
    //{
    //    return;
    //}
    //FdmParameterProfileService::instance()->setCurrentProfile(currentProfieName);

    
}

void FdmPreferenceDialogService::onCurrentMaterialNameChanged(const QString & name)
{
    TFunction(name);
    activeProfile = FdmMaterialProfileService::instance()->getCurrentProifle();
}

void FdmPreferenceDialogService::onCurrentMachineNameChanged(const QString & name)
{
    TFunction(name);
    activeProfile = FdmMachineProfileService::instance()->getCurrentProifle();
}

void FdmPreferenceDialogService::onCurrentParameterNameChanged(const QString & name)
{
    TFunction(name);
    activeProfile = FdmParameterProfileService::instance()->getCurrentProifle();
}


void FdmPreferenceDialogService::onSourceTreeApiNodeValueChanged(const QString &categoryName,const FdmSettingItem & item)
{
    //qDebug() << "activeProfile.name onSourceTreeApiNodeValueChanged categoryName" << activeProfile->getName() << categoryName << "key" << item.name << "value " << item.value;

    if (NULL == activeProfile)
    {
        TDebug("activeProfile is NULL");
        return;
    }
    
    if (ProfileGroup::Default == activeProfile->getGroup())
    {
        return;
    }

    
    activeProfile->setSetting(categoryName,item);
}

void FdmPreferenceDialogService::onSourceTreeApiNodeValueChangedByUI(const QString& categoryName, const FdmSettingItem& item)
{
    activeProfile->setModifyTimeNow();
}

void FdmPreferenceDialogService::syncCurrentParameterFromMangerToTree()
{
    ////



    
    //QList<FdmProfileCategory> content ;
    //profile->getCategoriesForRefreshTree(content);
    //FdmQmlSourceTree::instance().getFdmQmlTreeApi_Preference().setAny(content);
    //FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(content);

    
    //FdmParameterProfileService::instance()->refreshUI();
    //FdmRightParameterService::instance()->refreshUI();
    //FdmMachineProfileService::instance()->refreshUI();
    //FdmMaterialProfileService::instance()->refreshUI();
}
