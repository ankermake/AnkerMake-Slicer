#include "fdmrightparameterservice.h"
#include "profilemanage/fdmparameterprofilemanager.h"
#include "filemanage/fdmqmlsourcetree.h"
#include "profilemanage/fdmmachineprofilemanager.h"
#include "profilemanage/fdmmaterialprofilemanager.h"
#include "common/ak_const.h"
#include "fdmmachineprofileservice.h"
#include "fdmmaterialprofileservice.h"
#include "fdmparameterprofileservice.h"
#include "fdmpreferencedialogservice.h"
#include "common/utilities/tlogger.h"
#include "profilemanage/ifdmprofile.h"
#include "common/utilities/tlogger.h"
#include "fdmprofilebackupservice.h"

using namespace AkConst;


void FdmRightParameterService::init()
{
    qDebug() << "FdmRightParameterService init";
    m_realTimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
    
    //connect(&rightTree, &FdmQmlTreeApi::anyNodeValueChangeByUI, this, &FdmRightParameterService::onSourceTreeApiNodeValueChanged);

    
    connect(&rightTree, &FdmQmlTreeApi::anyNodeValueChange, this, &FdmRightParameterService::onSourceTreeApiNodeValueChanged);
    
    connect(FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::sendOperLog, this, &FdmRightParameterService::receiveOperLog);

    connect(FdmPreferenceDialogService::instance(), &FdmPreferenceDialogService::operateComplete, this, &FdmRightParameterService::doOperateComplete);

    
    connect(&timer,&QTimer::timeout, [&]()->void {
        //qDebug() << "save .....";
        if (m_updateTime > 0)
        {
            qDebug() << "real save ....." << m_updateTime;
             //auto realProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
             m_realTimeProfile->save();
             m_updateTime=0;
        }
    });
    timer.start(500);
}

void FdmRightParameterService::doMainWindowInitFinished()
{
    
    auto paramProfile = FdmParameterProfileManager::Instance().getProfileByName(m_realTimeProfile->getProfileName());
    if (nullptr == paramProfile)
    {
        m_realTimeProfile->setProfileName("");
    }
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(m_realTimeProfile->getMachineName());
    if (nullptr == machineProfile)
    {
        m_realTimeProfile->setMachineName("");
    }
    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(m_realTimeProfile->getMaterialName());
    if (nullptr == materialProfile)
    {
        m_realTimeProfile->setMaterialName("");
    }

    
    if (m_realTimeProfile->getMachineName().isEmpty()
        || m_realTimeProfile->getMaterialName().isEmpty()
        || m_realTimeProfile->getProfileName().isEmpty()
        )
    {
        selectProfile(m_realTimeProfile->getProfileName());
    }
    if (m_realTimeProfile->getProfileName() == ProfileName::SIMPLE_MODE)
    {
        
        selectProfile(ProfileName::SIMPLE_MODE);
        applySimpleModeData();
    }

    refreshTree();
    
    syncRightTreeToRealtimeprofile();

    //QTimer::singleShot(10, [&]() {
    //    refreshTree();
    
    //    syncRightTreeToRealtimeprofile();
    //    });
}

//when this function be triggled . it means (m5  pla+ 0.4 simplemode)
void FdmRightParameterService::onPrintModeChanged(const QString name)
{
    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return ;
    }
    qDebug() << "onPrintModeChanged name is " << name;

    //save value to realtimeFile
    m_realTimeProfile->setPrintMode(name);

    checkPrintMode();

    qDebug() << "onPrintModeChanged" << m_realTimeProfile->getPrintMode();
    selectProfile(m_realTimeProfile->getProfileName());

    emit printModeChanged();
    m_updateTime++;
}

void FdmRightParameterService::onPrintModeIdxChanged(int index)
{
    auto list = getPrintModeOriginList();
    if(index >=0 && index < list.size())
    {
        onPrintModeChanged(list[index]);
    }
    else
    {
        onPrintModeChanged(AkConst::PrintMode::NORMAL);
    }
}

void FdmRightParameterService::checkPrintMode()
{
    qDebug() << "getNozzleSize " <<  m_realTimeProfile->getNozzleSize()
             << "getProfileName " << m_realTimeProfile->getProfileName()
             << "getMaterialName " <<  m_realTimeProfile->getMaterialName()
                << "getMachineName " <<  m_realTimeProfile->getMachineName()
                   << "getPrintMode " <<  m_realTimeProfile->getPrintMode();

    QString printMode = m_realTimeProfile->getPrintMode();
    //set value of printmode
    if (m_realTimeProfile->getNozzleSize()*100 == 40
            && (m_realTimeProfile->getProfileName() == AkConst::ProfileName::SIMPLE_MODE
                || m_realTimeProfile->getProfileName() == AkConst::ProfileName::EXPERT_MODE)
            && (m_realTimeProfile->getMachineName() == AkConst::MachineName::M5)
            )
    {
        if (AkConst::MaterialName::PLAPLUS_SERIES.contains(m_realTimeProfile->getMaterialName())
             && m_realTimeProfile->getPrintMode() == AkConst::PrintMode::FAST)
        {
            printMode = AkConst::PrintMode::FAST;
        }
        else if  (m_realTimeProfile->getMaterialName() == AkConst::MaterialName::PLAPLUS
                   && m_realTimeProfile->getPrintMode() == AkConst::PrintMode::SMOOTH)
        {
            printMode = AkConst::PrintMode::SMOOTH;
        }
        else
        {
            printMode = AkConst::PrintMode::NORMAL;
        }
    }

    //更新树中的数据 change @2023-04-14 by ChunLian
    m_realTimeProfile->setPrintMode(printMode);
    setTreeNodeValue(AkConst::SettingKey::QML_PRINT_MODE        , printMode             );

    qDebug() << "m_realTimeProfile->updatePrintMode " <<  printMode;
    //更新树中的数据
    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
    QList<FdmProfileCategory> categorys;
    FdmProfileCategory category;
    category.name = AkConst::Category::BASE_PARAM;
    category.set(AkConst::SettingKey::QML_PRINT_MODE, printMode);
    categorys.append(category);
    rightTree.setAny(categorys);

    emit printModeChanged();
    m_updateTime++;
}

void FdmRightParameterService::reload(FdmRealTimeProfile* newProfile)
{
    m_realTimeProfile->reload(newProfile);
    m_updateTime++;
    refreshTree();

    
    emit currentParameterSwitched(m_realTimeProfile->getProfileName());
}

QString FdmRightParameterService::getMachineName() 
{
    auto name = m_realTimeProfile->getMachineName();
    
    auto profile = FdmMachineProfileManager::Instance().getProfileByName(name);
    if (NULL == profile)
    {
        m_realTimeProfile->setMachineName("");
        return "";
    }
    return name;
}

void FdmRightParameterService::selectDefaultParamFromMachine()
{
    auto machineName = m_realTimeProfile->getMachineName();
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(machineName);

    if(machineProfile == nullptr){
        return;
    }

    {
        auto dafultMaterialName= machineProfile->getDefaultMaterialName();
        qDebug() << "dafultMaterialName" << dafultMaterialName;
        if(dafultMaterialName.isEmpty() || !AkConst::MaterialName::ALL_SERIES.contains(dafultMaterialName))
        {
            dafultMaterialName = AkConst::MaterialName::PLAPLUS;
        }

        selectMaterial(dafultMaterialName);
    }

    {
        auto dafultPrintMode= machineProfile->getDefaultPrintMode();
        qDebug() << "machineProfile->getDefaultPrintMode" << dafultPrintMode;
        if(!dafultPrintMode.isEmpty() && AkConst::PrintMode::ALL_SERIES.contains(dafultPrintMode))
        {
            m_realTimeProfile->setPrintMode(dafultPrintMode);
        }
        else
        {
            m_realTimeProfile->setPrintMode(AkConst::PrintMode::NORMAL);
        }
    }

    {
        auto dafultParameterMode= machineProfile->getDefaultParameterMode();
        if(!dafultParameterMode.isEmpty() && AkConst::ProfileName::ALL_SERIES.contains(dafultParameterMode))
        {
            m_realTimeProfile->setProfileName(dafultParameterMode);
        }
        else
        {
            m_realTimeProfile->setProfileName(AkConst::ProfileName::SIMPLE_MODE);
        }
    }

    //必须在前面模式切换之后
    {
        auto dafultNozzleSizeName= machineProfile->getDefaultNozzleSizeName();
        qDebug() << "dafultNozzleSizeName" << dafultNozzleSizeName;
        if(dafultNozzleSizeName.isEmpty() || !AkConst::NozzleSizeName::ALL_SERIES.contains(dafultNozzleSizeName))
        {
            dafultNozzleSizeName = AkConst::NozzleSizeName::SIZE4;
        }

        selectNozzle(dafultNozzleSizeName);
    }
}

//切换machine
void FdmRightParameterService::onMachineNameChanged(const QString name)
{
    if (name == AkConst::ProfileName::More)
    {
        emit machineNameChanged();
        return;
    }

    emit currentMachineSwitched(name);

    m_realTimeProfile->setMachineName(name);
    if(AkConst::MachineName::ALL_SERIES.contains(name))
    {
        selectDefaultParamFromMachine();
    }
    else
    {
        checkPrintMode();
    }

    if (!AkConst::ProfileName::ALL_SERIES.contains(m_realTimeProfile->getProfileName()))
    {
        selectMachine(name);
    }
    
    else
    {
        selectProfile(m_realTimeProfile->getProfileName());
    }
    m_updateTime++;

    
    //FdmParameterProfileManager::Instance().setCurrentMachineName(name);
    
    //auto currentParamProfile = FdmParameterProfileManager::Instance().getCurrentSelectedProfile();

    
    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //if (currentParamProfile->isInDefaultGroup())
    //{
    
    //    updateRealtimeProfile(currentParamProfile);
    //}
    //else
    //{
    
    //    auto selectedMachine = FdmMachineProfileManager::Instance().getProfileByName(name);
    //    realtimeProfile->updateMachine(selectedMachine);
    //}

    
    //realtimeProfile->setMetaData(AkConst::SettingKey::PARAMETER_MACHINE_NAME, name);

    
    //refreshTree();
    
}

void FdmRightParameterService::onMaterialNameChanged(const QString name)
{
    if (name == AkConst::ProfileName::More)
    {
        emit materialNameChanged();
        return;
    }

    emit currentMaterialSwitched(name);

    m_realTimeProfile->setMaterialName(name);
    checkPrintMode();

    if (m_realTimeProfile->getProfileName() != AkConst::ProfileName::EXPERT_MODE
        && m_realTimeProfile->getProfileName() != AkConst::ProfileName::SIMPLE_MODE)
    {
        selectMaterial(name);
    }
    
    else
    {

        selectProfile(m_realTimeProfile->getProfileName());
    }
    emit nozzleSizeListChanged();
    m_updateTime++;
    //
    
    //FdmParameterProfileManager::Instance().setCurrentMaterialName(name);
    
    //auto currentParamProfile = FdmParameterProfileManager::Instance().getCurrentSelectedProfile();

    
    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //if (currentParamProfile->isInDefaultGroup())
    //{
    
    //    //realtimeProfile->updateProfile(currentParamProfile);
    //    updateRealtimeProfile(currentParamProfile);
    //}
    //else
    //{
    
    //    auto selectedMaterial = FdmMaterialProfileManager::Instance().getProfileByName(name);
    //    realtimeProfile->updateMaterial(selectedMaterial);
    //}

    
    ////realtimeProfile->setMetaData(AkConst::SettingKey::PARAMETER_MATERIAL_NAME, name);

    
    //refreshTree();

    
    //realtimeProfile->save();
}

//
//

//void FdmRightParameterService::updateRealtimeProfile(FdmParameterProfile* destProfile)
//{
//    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
//    realtimeProfile->updateProfile(destProfile);
//    realtimeProfile->setMetaData(AkConst::SettingKey::META_CURRENT_MACHINE_ID, destProfile->getMachineName());
//    realtimeProfile->setMetaData(AkConst::SettingKey::META_CURRENT_MATERIAL_ID, destProfile->getMaterialId());
//    realtimeProfile->setMetaData(AkConst::SettingKey::META_CURRENT_SELECT_NAME, FdmParameterProfileManager::Instance().getCurrentSelectedName());
//    realtimeProfile->setMetaData(AkConst::SettingKey::META_CURRENT_PROFILE_ID, destProfile->getId());
//    realtimeProfile->setMetaData(AkConst::SettingKey::META_CURRENT_PROFILE_GROUP, destProfile->getGroup());
//}


void FdmRightParameterService::selectProfile(QString name)
{
    if (name == AkConst::ProfileName::More)
    {
        
        emit currentParameterNameChanged();
        return;
    }
    
    if (name.isEmpty())
    {
        name = AkConst::ProfileName::SIMPLE_MODE;
        m_realTimeProfile->setNozzleSize(0.4);
    }
    bool getExpertSuccess = false;

    m_realTimeProfile->setProfileName(name);
    FdmParameterProfile* destProfile = nullptr;
    if (name == AkConst::ProfileName::EXPERT_MODE || name == AkConst::ProfileName::SIMPLE_MODE)
    {

//        if (m_realTimeProfile->getMachineName().isEmpty() || m_realTimeProfile->getMaterialName().isEmpty())
//        {
//            return;
//        }
        
        if (m_realTimeProfile->getMachineName().isEmpty())
        {
            m_realTimeProfile->setMachineName(AkConst::MachineName::M5);
            m_updateTime++;
        }

        if (m_realTimeProfile->getMaterialName().isEmpty()
                || !getMaterialList().contains(m_realTimeProfile->getMaterialName()))
        {
            m_realTimeProfile->setMaterialName(AkConst::MaterialName::PLAPLUS);
            m_updateTime++;
        }

        if (!getNozzleSizeList().contains(QString("%1 mm").arg(m_realTimeProfile->getNozzleSize(), 3)))
        {
            m_realTimeProfile->setNozzleSize(0.4);
            m_realTimeProfile->setNozzleName(AkConst::NozzleSizeName::SIZE4);
            m_updateTime++;
        }

        if(!getPrintModeList().contains(getPrintMode())){
            QString materialName = m_realTimeProfile->getMaterialName();
            QString machineName = m_realTimeProfile->getMachineName();
            double nozzleSize = m_realTimeProfile->getNozzleSize();

            if((int)(nozzleSize*100) == 40)
            {
                if(materialName == AkConst::MaterialName::PLAPLUS_METALIC
                        || materialName == AkConst::MaterialName::PLAPLUS_SILK
                        || materialName == AkConst::MaterialName::PLAPLUS_MATTE)
                {
                    m_realTimeProfile->setPrintMode(AkConst::PrintMode::FAST);
                }
                else if(AkConst::MachineName::M5C_SERIES.contains(machineName)
                        && materialName == AkConst::MaterialName::PLA_CF)
                {
                    m_realTimeProfile->setPrintMode(AkConst::PrintMode::FAST);
                }
                else
                {
                    m_realTimeProfile->setPrintMode(AkConst::PrintMode::NORMAL);
                }
            }
            else
            {
                m_realTimeProfile->setPrintMode(AkConst::PrintMode::NORMAL);
            }

            m_updateTime++;
        }

        destProfile = FdmParameterProfileManager::Instance().
                getExpertProfile(m_realTimeProfile->getMachineName(),
                                 m_realTimeProfile->getMaterialName(),
                                 m_realTimeProfile->getPrintMode(),
                                 m_realTimeProfile->getNozzleSize(),
                                 getExpertSuccess);

        qDebug() << "destProfile->getPrintMode()" << destProfile->getPrintMode() << getExpertSuccess;
    }
    else
    {
        destProfile = FdmParameterProfileManager::Instance().getProfileByName(m_realTimeProfile->getProfileName());
    }
    if (nullptr == destProfile)
    {
        return;
    }
    
    
//    m_realTimeProfile->setMachineName(destProfile->getMachineName());
//    m_realTimeProfile->setMaterialName(destProfile->getMaterialName());
      m_realTimeProfile->updateProfile(destProfile);

    auto material_print_temperature = m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, "material_print_temperature");
    qDebug() << "material_print_temperature:" << material_print_temperature;

    emit currentParameterSwitched(name);

    
    
    QList<FdmProfileCategory> content;
    
    
    destProfile->getCategoriesForRefreshTree(content);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(content);

    
    
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(content);

    
    auto parameterModifyTime = destProfile->getModifyTime();
    qDebug() << "parameterModifyTime" << parameterModifyTime;

    //如果最近有更新过引用的machine文件，需要更新
    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(m_realTimeProfile->getMachineName());
    if (nullptr != machineProfile && machineProfile->getModifyTime() > parameterModifyTime)
    {
        qDebug() << "machineProfile->getModifyTime()" << machineProfile->getModifyTime();
        selectMachine(m_realTimeProfile->getMachineName());
    }

    
    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(m_realTimeProfile->getMaterialName());
    if (nullptr != materialProfile && materialProfile->getModifyTime() > parameterModifyTime)
    {
        qDebug() << "materialProfile->getModifyTime()" << materialProfile->getModifyTime();
        selectMaterial(m_realTimeProfile->getMaterialName());
    }

    
    {
        if (nullptr == machineProfile)
        {
            m_realTimeProfile->setMachineName(AkConst::MachineName::M5);
            selectMachine(m_realTimeProfile->getMachineName());
            m_updateTime++;
        }
        if (nullptr == materialProfile)
        {
            m_realTimeProfile->setMaterialName(AkConst::MaterialName::PLAPLUS);
            selectMaterial(m_realTimeProfile->getMaterialName());
            m_updateTime++;
        }
    }

    

    
    if ((name == AkConst::ProfileName::EXPERT_MODE
             || name == AkConst::ProfileName::SIMPLE_MODE)
         && !getExpertSuccess)
    {
        selectNozzle(getNozzleSize());

        
        FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
        QList<FdmProfileCategory> categories;
        rightTree.getAll(categories);
        destProfile->setCategories(categories);
    }

    
    if (name == AkConst::ProfileName::SIMPLE_MODE)
    {
        setDefaultValueFromExpertMode();
    }

    refreshUI();

    m_updateTime++;
}
void FdmRightParameterService::applySimpleModeData()
{
    onAiQualityCurrentIdxChanged(getAiQualityCurrentIdx());
    onAiInfillDensityIdxChanged(getAiInfillDensityCurrentIdx());
    onGlobalSupportStateChanged(getGlobalSupportState());
    onGenerateAdhesionStateChanged(getGenerateAdhesionState());
    m_updateTime++;
}


void FdmRightParameterService::setDefaultValueFromExpertMode()
{
    
    auto layerHeightVar = m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, AkConst::SettingKey::LAYER_HEIGHT);
    if (!layerHeightVar.isNull())
    {
        float value = layerHeightVar.toFloat();
        int idx = getAiQualityIdx(value);
        onAiQualityCurrentIdxChanged(idx);
    }

    auto infillDensityVar = m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, AkConst::SettingKey::INFILL_SPARSE_DENSITY);
    if (!infillDensityVar.isNull())
    {
        float value = infillDensityVar.toFloat();
        int idx = getAiInfillDensityIdx(value);
        onAiInfillDensityIdxChanged(idx);
    }

    auto sptStateVar = m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, AkConst::SettingKey::SUPPORT_ENABLE);
    if (!sptStateVar.isNull())
    {
        bool value = sptStateVar.toBool();
        int idx = getGlobalSupportState(value);
        onGlobalSupportStateChanged(idx);
    }

    
    auto adhesionTypeVar = m_realTimeProfile->getSetting(AkConst::Category::BASE_PARAM, AkConst::SettingKey::ADHESION_TYPE);
    if (!adhesionTypeVar.isNull())
    {
        QString value = adhesionTypeVar.toString();

        FdmSettingItem IdxItem;
        IdxItem.name = AkConst::SettingKey::AI_ADHESION_TYPE;
        IdxItem.value = value;
        m_realTimeProfile->setSetting(AkConst::Category::AK_AI, IdxItem);

        int state = getAdhesionTypeState(value);
        onGenerateAdhesionStateChanged(state);
    }
}

//
QString FdmRightParameterService::getPrintMode()
{
    auto printMode =  m_realTimeProfile->getPrintMode();
    qDebug() << "getPrintMode" << printMode;
    if (printMode == AkConst::PrintMode::FAST)
    {
        return tr("Fast");
    }
    else if(printMode == AkConst::PrintMode::SMOOTH)
    {
        return tr("Smooth");
    }
    else
    {
        return tr("Normal");
    }
}

QStringList FdmRightParameterService::getPrintModeList()
{
    QList<QString> resultList;

    auto machineName = getMachineName();
    auto nozzleSize = getNozzleSize();
    auto meterialName = getMaterialName();

    if(!AkConst::MachineName::ALL_SERIES.contains(machineName)
            || !AkConst::MaterialName::ALL_SERIES.contains(meterialName))
    {
        return QList<QString> { tr("Normal") };
    }

    if(AkConst::NozzleSizeName::SIZE4==nozzleSize
            && (AkConst::MaterialName::PLAPLUS_SERIES.contains(meterialName)))
    {
        resultList << tr("Fast");
    }

    if(meterialName != AkConst::MaterialName::PLAPLUS_SILK
            && meterialName != AkConst::MaterialName::PLAPLUS_METALIC
            && meterialName != AkConst::MaterialName::PLA_CF
            && meterialName != AkConst::MaterialName::PLAPLUS_MATTE)
    {
            resultList << tr("Normal");
    }

    if(AkConst::MachineName::ALL_SERIES.contains(machineName)
            && meterialName==AkConst::MaterialName::PLAPLUS
            && (nozzleSize==AkConst::NozzleSizeName::SIZE4 || nozzleSize==AkConst::NozzleSizeName::SIZE2))
    {
        resultList << tr("Smooth");
    }

    return resultList;
}

QStringList FdmRightParameterService::getPrintModeOriginList()
{
    auto list = getPrintModeList();
    QStringList resultList;
    for(QString& item : list){
        if(tr("Fast") == item){
            resultList.append(AkConst::PrintMode::FAST);
        }
        else if(tr("Normal") == item){
            resultList.append(AkConst::PrintMode::NORMAL);
        }
        else if(tr("Smooth") == item){
            resultList.append(AkConst::PrintMode::SMOOTH);
        }
        else{
            resultList.append(AkConst::PrintMode::NORMAL);
        }
    }

    return resultList;
}

void FdmRightParameterService::selectMachine(QString name)
{
    if (name == AkConst::ProfileName::More)
    {
        emit machineNameChanged();
        return;
    }

    m_realTimeProfile->setMachineName(name);


//    auto currentProfileName = m_realTimeProfile->getProfileName();
//    if (currentProfileName == ProfileName::EXPERT_MODE || currentProfileName == ProfileName::SIMPLE_MODE)
//    {
//        return selectProfile(currentProfileName);
//    }

    auto machineProfile = FdmMachineProfileManager::Instance().getProfileByName(name);
    QList<FdmProfileCategory> machineContent;

    
    machineProfile->getCategoriesForRefreshTree(1, machineContent);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(machineContent);

    emit machineNameChanged();
    emit extruderCountChanged();
}

void FdmRightParameterService::selectMaterial(QString name)
{
    if (name == AkConst::ProfileName::More)
    {
        emit materialNameChanged();
        return;
    }
    m_realTimeProfile->setMaterialName(name);


//    auto currentProfileName = m_realTimeProfile->getProfileName();
//    if (currentProfileName == ProfileName::EXPERT_MODE || currentProfileName == ProfileName::SIMPLE_MODE)
//    {
//        return selectProfile(currentProfileName);
//    }

    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileByName(name);
    QList<FdmProfileCategory> materialContent;
    materialProfile->getCategoriesForRefreshTree(materialContent);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setAny(materialContent);
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setNodeValue(AkConst::SettingKey::MATERIAL_NAME, name); //  add @2023-04-12 by ChunLian

    emit materialNameChanged();
}

void FdmRightParameterService::setSupportMeshCount(int newSupportMeshCount)
{
    m_supportMeshCount = newSupportMeshCount;
}
void FdmRightParameterService::setGlobalSupportForbidden(bool forbidden)
{
    m_globalSupportForbidden = forbidden;
}

void FdmRightParameterService::syncRightTreeToRealtimeprofile()
{
    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
    QList<FdmProfileCategory> categories;
    rightTree.getAll(categories);
    
    
    m_realTimeProfile->setCategories(categories);
    m_updateTime++;
}

QStringList FdmRightParameterService::getMachineList() 
{
    QStringList resultList;
    QStringList list = FdmMachineProfileManager::Instance().getAllProfileNameList();
    resultList << list;
    resultList << AkConst::ProfileName::More;
    return resultList;
}

QString FdmRightParameterService::getMaterialName() 
{
    
    auto name = m_realTimeProfile->getMaterialName();
    
    auto profile = FdmMaterialProfileManager::Instance().getProfileByName(name);
    if (NULL == profile)
    {
        return "";
    }
    return name;
}

QStringList FdmRightParameterService::getMaterialList() 
{
    QStringList resultList;
    QStringList list = FdmMaterialProfileManager::Instance().getAllProfileNameList();
    resultList << list;
    resultList << AkConst::ProfileName::More;

    if (getNozzleSize() == AkConst::NozzleSizeName::SIZE8)
    {
        resultList.removeOne(AkConst::MaterialName::TPU);
    }

    if (getNozzleSize() != AkConst::NozzleSizeName::SIZE4)
    {
        resultList.removeOne(AkConst::MaterialName::PLAPLUS_MATTE);
        resultList.removeOne(AkConst::MaterialName::PLA_CF);
        resultList.removeOne(AkConst::MaterialName::PET_CF);
        resultList.removeOne(AkConst::MaterialName::PA_CF);
        resultList.removeOne(AkConst::MaterialName::PLAPLUS_SILK);
        resultList.removeOne(AkConst::MaterialName::PLAPLUS_METALIC);
    }

    if(!AkConst::MachineName::M5C_SERIES.contains(m_realTimeProfile->getMachineName()))
    {
        resultList.removeOne(AkConst::MaterialName::PLA_CF);
        resultList.removeOne(AkConst::MaterialName::PET_CF);
        resultList.removeOne(AkConst::MaterialName::PA_CF);
    }

    if(getNozzleSize() == AkConst::NozzleSizeName::SIZE2
            && AkConst::MachineName::M5C_SERIES.contains(m_realTimeProfile->getMachineName()))
    {
        resultList.removeOne(AkConst::MaterialName::TPU);
        resultList.removeOne(AkConst::MaterialName::ABS);
        resultList.removeOne(AkConst::MaterialName::PETG);
    }

    return resultList;
}

const QString FdmRightParameterService::getCurrentSelectedName() 
{
    if (m_realTimeProfile->getProfileName() != AkConst::ProfileName::EXPERT_MODE
        && m_realTimeProfile->getProfileName() != AkConst::ProfileName::SIMPLE_MODE)
    {
        auto profile = FdmParameterProfileManager::Instance().getProfileByName(m_realTimeProfile->getProfileName());
        if (nullptr == profile)
        {
            return "";
        }
    }
    return m_realTimeProfile->getProfileName();

    //qDebug() << "FdmRightParameterService::getCurrentSelectedName()" << FdmParameterProfileManager::Instance().getCurrentSelectedName();
    //return FdmParameterProfileManager::Instance().getCurrentSelectedName();
}


QStringList FdmRightParameterService::getAllParameterList() 
{
    
    QStringList resultList;
    resultList << AkConst::ProfileName::SIMPLE_MODE;
    resultList << AkConst::ProfileName::EXPERT_MODE;
    QStringList list = FdmParameterProfileManager::Instance().getAllProfileNameList();
    resultList << list;
    resultList << AkConst::ProfileName::More;
    qDebug() << "FdmRightParameterService::getAllParameterList is " << resultList;
    return resultList;
}

int FdmRightParameterService::getExtruderCount()
{
    QString machineName = getMachineName();
    auto profile = FdmMachineProfileManager::Instance().getProfileByName(machineName);
    if (NULL == profile)
    {
        return 1;
    }

    return profile->getExtruderCount();
}

void FdmRightParameterService::refreshUI()
{
    emit machineListChanged();
    emit materialListChanged();
    emit allParameterListChanged();

    emit currentParameterNameChanged();
    emit machineNameChanged();
    emit materialNameChanged();
    emit extruderCountChanged();

//    emit aiNormalQualityClicked();
//    emit aiHighQualityClicked();
    emit aiInfillDensityListChanged();
    emit aiQualityListChanged();
    emit aiQualityCurrentIdxChanged();

    emit nozzleSizeChanged();
    emit nozzleSizeListChanged();
    emit printModeListChanged();

    emit infillThicknessChanged();
}


void FdmRightParameterService::onSaveBtnClicked()
{
    qDebug() << "onSaveBtnClicked";
    //FdmParameterProfileManager::Instance().persistRealtimeProfileModification();
    //m_realTimeProfile->saveInnerProfile();

    QList<FdmProfileCategory> categories;
    m_realTimeProfile->getInnerProfileCategories(categories);

    auto currentProfile = FdmParameterProfileManager::Instance().getProfileByName(m_realTimeProfile->getProfileName());
    if (currentProfile == nullptr)
    {
        TDebug("the currentProfile is not exist. maybe the current profile name is null. please check");
        return;
    }

    currentProfile->clearData();
    currentProfile->setCategories(categories);
    
    currentProfile->setName(m_realTimeProfile->getProfileName());
    currentProfile->setVisible(true);
    currentProfile->save();

    
    FdmProfileBackupService::instance()->backup(currentProfile->getDataSource());

    
    emit allParameterListChanged();
}


void FdmRightParameterService::onSaveAsBtnClicked(QString profileName)
{
#ifdef TOOL_SAVE_AS_PARAM_INI
    {
        m_realTimeProfile->setMachineName (m_realTimeProfile->getMachineName ());
        m_realTimeProfile->setMaterialName(m_realTimeProfile->getMaterialName());
        m_realTimeProfile->setProfileName (profileName.split(".").first());
        m_realTimeProfile->setNozzleSize  (m_realTimeProfile->getNozzleSize  ());
        m_realTimeProfile->setNozzleName  (getNozzleSize());

        qDebug() <<"onSaveAsBtnClicked";
        //QString path = FdmParameterProfileManager::Instance().getValidCustomProfilePath(profileName);

        QList<FdmProfileCategory> categories;
        m_realTimeProfile->getInnerProfileCategories(categories);
        auto newProfile = FdmParameterProfileManager::Instance().createProfile(profileName, categories);
        newProfile->setVisible(true);
        newProfile->save(__ToolSaveAs_2(profileName));

        //刷新排序
        emit allParameterListChanged();

        //使用当前保存的数据作为当前数据
        selectProfile(profileName);
        return;
    }
#endif
    qDebug() <<"onSaveAsBtnClicked";
    //QString path = FdmParameterProfileManager::Instance().getValidCustomProfilePath(profileName);

    QList<FdmProfileCategory> categories;
    m_realTimeProfile->getInnerProfileCategories(categories);
    auto newProfile = FdmParameterProfileManager::Instance().createProfile(profileName, categories);
    newProfile->setVisible(true);
    newProfile->save();

    
    FdmProfileBackupService::instance()->backup(newProfile->getDataSource());

    
    emit allParameterListChanged();

    
    selectProfile(profileName);

    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //auto templateProfile = FdmParameterProfileManager::Instance().getProfileByName();
    //auto newProfile = FdmParameterProfileManager::Instance().createProfile(profileName,);
    //newProfile->updateProfile(realtimeProfile);

    //newProfile->save();

    
    //FdmParameterProfileManager::Instance().setCurrentSelectedName(profileName);

    //onCurrentSelectedNameChanged(profileName);

    
    //refreshUI();
    //FdmParameterProfileService::instance()->refreshUI();

}



void FdmRightParameterService::onExtruderSwitched(int extruderIdx)
{

}


void FdmRightParameterService::onSourceTreeApiNodeValueChanged(const QString &categoryName, const FdmSettingItem &item)
{
    qDebug() << "FdmRightParameterService onSourceTreeApiNodeValueChanged categoryName" << categoryName << "key" << item.name << "value " << item.value;

    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return;
    }

    
    m_realTimeProfile->setSetting(categoryName,item);
    
    m_updateTime++;

    
    if (item.name == "support_enable")
    {
        bool sptState = item.value.toBool();
        emit setSupportEnabled(sptState);
    }
}

void FdmRightParameterService::onExtruderSwitched(int extruderIdx, FdmQmlTreeApi &treeApi)
{

}


void FdmRightParameterService::refreshTree()
{
    
    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    QList<FdmProfileCategory> categories;
    m_realTimeProfile->getCategoriesForRefreshTree(categories);
    rightTree.setAny(categories);

    
    
    rightTree.setAny(categories);
    
    refreshUI();
}

void FdmRightParameterService::setTreeNodeValue(const QString &nodeName, const QVariant value)
{
    if(AkConst::SettingKey::QML_PRINT_MODE == nodeName)
    {
        qDebug() << "FdmRightParameterService::setTreeNodeValue:" << value;
    }
    FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right().setNodeValue(nodeName, value);
}


void FdmRightParameterService::onAiQualityCurrentIdxChanged(int idx)
{
     qDebug() << "onAiQualityCurrentIdxChanged is " << idx;
     if(idx == 0){
         int a = 0;
     }

    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return ;
    }
    auto qualityList = getAiQualityList();
    if (idx > qualityList.size() - 1)
    {
        idx = 0;
    }

    
    float newAiQuality = qualityList[idx];
    
    FdmSettingItem qualityIdxItem;
    qualityIdxItem.name = AkConst::SettingKey::AI_QUALITY_INDEX;
    qualityIdxItem.value = idx;
    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, qualityIdxItem);

    m_realTimeProfile->setLayerHeight(newAiQuality);
    setTreeNodeValue(AkConst::SettingKey::LAYER_HEIGHT          , newAiQuality          );
    if(m_realTimeProfile->getPrintMode() == AkConst::PrintMode::SMOOTH)
    {
        float thickness = newAiQuality*2;
        m_realTimeProfile->setInfillThickness(thickness);
        setTreeNodeValue(AkConst::SettingKey::INFILL_SPARSE_THICKNESS, thickness);
    }

    m_updateTime++;

    emit aiQualityCurrentIdxChanged();
}

void FdmRightParameterService::selectNozzle(QString name)
{
    
    auto valueList = name.split(" ");
    auto nozzleSize = valueList[0].toDouble();

    m_realTimeProfile->setNozzleName(name      );
    m_realTimeProfile->setNozzleSize(nozzleSize);
    setTreeNodeValue(AkConst::SettingKey::QML_NOZZLE_NAME       , name       );
    setTreeNodeValue(AkConst::SettingKey::MACHINE_NOZZLE_SIZE   , nozzleSize );

    m_updateTime++;

    emit nozzleSizeChanged();
    emit aiQualityListChanged();

    
    onAiQualityCurrentIdxChanged(getAiQualityIdx(nozzleSize / 2));
}


void FdmRightParameterService::onNozzleSizeChanged(const QString name)
{
    auto valueList = name.split(" ");
    auto nozzleSize = valueList[0].toDouble();
    m_realTimeProfile->setNozzleName(name);
    m_realTimeProfile->setNozzleSize(nozzleSize);

    checkPrintMode();

    if (!AkConst::ProfileName::ALL_SERIES.contains(m_realTimeProfile->getProfileName()))
    {
        selectNozzle(name);
    }
    
    else
    {
        selectProfile(m_realTimeProfile->getProfileName());
    }
    
    emit materialListChanged();
    m_updateTime++;
}

//QList<int> FdmRightParameterService::getAiInfillDensityList()
//{
//    float defaultValue =  20.0f;
//    if (NULL == m_realTimeProfile)
//    {
//        qDebug() << "realtimeProfile is null";
//        return defaultValue;
//    }
//    auto density = m_realTimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_INFILL_DENSITY);
//    if (density.isNull())
//    {
//        return defaultValue;
//    }


//    //onAiInfillDensityChanged(density.toFloat());

//    return density.toFloat();
//}

//QList<int> FdmRightParameterService::getAiInfillDensityList()
//{
//    float defaultValue =  20.0f;
//    if (NULL == m_realTimeProfile)
//    {
//        qDebug() << "realtimeProfile is null";
//        return defaultValue;
//    }
//    auto density = m_realTimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_INFILL_DENSITY);
//    if (density.isNull())
//    {
//        return defaultValue;
//    }


//    //onAiInfillDensityChanged(density.toFloat());

//    return density.toFloat();
//}


QList<int> FdmRightParameterService::getAiInfillDensityList()
{
    QList<int> defaultList;
    defaultList << 0;
    defaultList << 10;
    defaultList << 20;
    defaultList << 30;
    defaultList << 40;
    defaultList << 50;
    defaultList << 60;
    defaultList << 70;
    defaultList << 80;
    defaultList << 90;
    defaultList << 100;
    return defaultList;
}

void FdmRightParameterService::onAiInfillDensityIdxChanged(int idx)
{
    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return ;
    }
    auto infillDensityList = getAiInfillDensityList();
    if (idx > infillDensityList.size() - 1)
    {
        idx = 0;
    }

    
    float newDensity = infillDensityList[idx];
    
    FdmSettingItem IdxItem;
    IdxItem.name = AkConst::SettingKey::AI_INFILL_DENSITY_INDEX;
    IdxItem.value = idx;
    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, IdxItem);


    m_realTimeProfile->setInfillDensity(newDensity);
    setTreeNodeValue(AkConst::SettingKey::INFILL_SPARSE_DENSITY , newDensity          );

    m_updateTime++;

    emit aiInfillDensityCurrentIdxChanged();
}


//void FdmRightParameterService::onAiInfillDensityChanged(double newAiInfillDensity)
//{
//    if (NULL == m_realTimeProfile)
//    {
//        qDebug() << "realtimeProfile is null";
//        return ;
//    }
//    FdmSettingItem akDensityItem;
//    akDensityItem.name = AkConst::SettingKey::AI_INFILL_DENSITY;
//    akDensityItem.value = newAiInfillDensity;
//    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, akDensityItem);


//    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
//    QList<FdmProfileCategory> categorys;
//    FdmProfileCategory category;
//    category.name = AkConst::Category::INFILL;
//    category.set(AkConst::SettingKey::INFILL_SPARSE_DENSITY, newAiInfillDensity);
//    categorys.append(category);
//    rightTree.setAny(categorys);

//    emit aiInfillDensityListChanged();

//    m_updateTime++;
//}


void FdmRightParameterService::onSetBtnClicked(AkConst::EWidgetType widgetType, QString name)
{
    FdmPreferenceDialogService::instance()->setSource(AkConst::EOpenPreferenceSource::RightSetting);

    
    
    auto machineName = m_realTimeProfile->getMachineName();
    if (widgetType == AkConst::EWidgetType::Machine)
    {
        machineName = name;
    }
    if (machineName.isEmpty())
    {
        machineName = AkConst::MachineName::M5;
    }
    FdmMachineProfileService::instance()->selectMachine(machineName);

    
    auto materialName = m_realTimeProfile->getMaterialName();
    if (widgetType == AkConst::EWidgetType::Material)
    {
        materialName = name;
    }
    if (materialName.isEmpty())
    {
        materialName = AkConst::MaterialName::PLAPLUS;
    }
    FdmMaterialProfileService::instance()->selectMaterial(materialName);

    
    auto profileName = m_realTimeProfile->getProfileName();
    if (widgetType == AkConst::EWidgetType::Parameter)
    {
        profileName = name;
    }
    if (profileName.isEmpty() || profileName == AkConst::ProfileName::SIMPLE_MODE)
    {
        profileName = AkConst::ProfileName::EXPERT_MODE;;
    }
    
    if (profileName == AkConst::ProfileName::EXPERT_MODE)
    {
        FdmParameterProfileService::instance()->setCurrentMachine(m_realTimeProfile->getMachineName());
        FdmParameterProfileService::instance()->setCurrentMaterial(m_realTimeProfile->getMaterialName());
        FdmParameterProfileService::instance()->setNozzleSize(m_realTimeProfile->getNozzleSize());
    }
    FdmParameterProfileService::instance()->selectProfile(profileName);

    
    if (widgetType == AkConst::EWidgetType::Machine)
    {
        FdmMachineProfileService::instance()->selectMachine(machineName);
    }
    else if (widgetType == AkConst::EWidgetType::Material)
    {
        FdmMaterialProfileService::instance()->selectMaterial(materialName);
    }
    else if (widgetType == AkConst::EWidgetType::Parameter)
    {
        FdmParameterProfileService::instance()->selectProfile(profileName);
    }
}


void FdmRightParameterService::onResetBtnClicked()
{
    selectProfile(m_realTimeProfile->getProfileName());

    //auto profile = FdmParameterProfileManager::Instance().getCurrentSelectedProfile();
    //auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    //realtimeProfile->updateProfile(profile);
    //refreshTree();
}


void FdmRightParameterService::receiveOperLog(OperateLogPtr operLogPtr)
{
    AkUtil::TFunction("");
    
    QString oriProfileName = m_realTimeProfile->getProfileName();
    QString oriMachineName = m_realTimeProfile->getMachineName();
    QString oriMaterialName = m_realTimeProfile->getMaterialName();

    
    emit machineListChanged();
    emit materialListChanged();
    emit allParameterListChanged();

    
    if (operLogPtr->constainsRemoveOper(EProfileType::Parameter, oriProfileName))
    {
        m_realTimeProfile->setProfileName(AkConst::ProfileName::SIMPLE_MODE);
        m_realTimeProfile->setMachineName("");
        m_realTimeProfile->setMaterialName("");
        m_realTimeProfile->setNozzleSize(0.4);
        m_updateTime++;
        selectProfile(m_realTimeProfile->getProfileName());
        AkUtil::TDebug(oriProfileName + " was deleted. enter simple mode");
        return;
    }


    
    auto adjustName = [&]()->void {
        
        if (operLogPtr->constainsRenameOper(EProfileType::Parameter, oriProfileName))
        {
            auto newName = operLogPtr->getCurrentName(EProfileType::Parameter, oriProfileName);
            m_realTimeProfile->setProfileName(newName);
            AkUtil::TDebug("operlog: rename " + oriProfileName + " -> " + newName);
        }

        
        if (operLogPtr->constainsRemoveOper(EProfileType::Machine, oriMachineName))
        {
            m_realTimeProfile->setMachineName("");
            AkUtil::TDebug("operlog: delete " + oriMachineName);
        }
        else if (operLogPtr->constainsRenameOper(EProfileType::Machine, oriMachineName))
        {
            auto newName = operLogPtr->getCurrentName(EProfileType::Machine, oriMachineName);
            m_realTimeProfile->setMachineName(newName);
            AkUtil::TDebug("operlog: rename " + oriMachineName + " -> " + newName);
        }

        
        if (operLogPtr->constainsRemoveOper(EProfileType::Material, oriMaterialName))
        {
            m_realTimeProfile->setMaterialName("");
        }
        else if (operLogPtr->constainsRenameOper(EProfileType::Material, oriMaterialName))
        {
            auto newName = operLogPtr->getCurrentName(EProfileType::Material, oriMaterialName);
            m_realTimeProfile->setMaterialName(newName);
            AkUtil::TDebug("operlog: rename " + oriMaterialName + " -> " + newName);
        }
    };

    adjustName();

    
    if (m_realTimeProfile->getMachineName().isEmpty()
        || m_realTimeProfile->getMaterialName().isEmpty())
    {
        selectProfile(m_realTimeProfile->getProfileName());
        AkUtil::TDebug("operlog: machine or material is null. reset to origin parameter ");
        return;
    }

    
    auto reloadData = [&]()->void {
        
        if (operLogPtr->constainsManuallyValueChangeOper(EProfileType::Parameter, oriProfileName))
        {
            selectProfile(m_realTimeProfile->getProfileName());
            AkUtil::TDebug("operlog: manually value change in file " + oriProfileName);
        }

        
        
        if (operLogPtr->constainsManuallyValueChangeOper(EProfileType::Machine, oriMachineName))
        {
            selectMachine(m_realTimeProfile->getMachineName());
            AkUtil::TDebug("operlog: manually value change in file " + oriMachineName);
        }

        
        if (operLogPtr->constainsManuallyValueChangeOper(EProfileType::Material, oriMaterialName))
        {
            selectMaterial(m_realTimeProfile->getMaterialName());
            AkUtil::TDebug("operlog: manually value change in file " + oriMaterialName);
        }
    };

    
    reloadData();

    emit currentParameterNameChanged();
    emit machineNameChanged();
    emit materialNameChanged();
    AkUtil::TDebug("operlog: refresh ui ");
}


//void FdmRightParameterService::receiveOperLog(OperateLogPtr operLogPtr)
//{

//    emit machineListChanged();
//    emit materialListChanged();
//    emit allParameterListChanged();


//    if (operLogPtr->constainsRemoveOper(EProfileType::Parameter, m_realTimeProfile->getProfileName()))
//    {
//        m_realTimeProfile->setProfileName(AkConst::ProfileName::SIMPLE_MODE);
//        m_realTimeProfile->setMachineName("");
//        m_realTimeProfile->setMaterialName("");
//        m_updateTime++;
//        selectProfile(m_realTimeProfile->getProfileName());
//        return;
//    }


//    auto reloadData = [&]()->void {

//        if (m_realTimeProfile->getProfileName() == AkConst::ProfileName::SIMPLE_MODE
//            || m_realTimeProfile->getProfileName() == AkConst::ProfileName::EXPERT_MODE)
//        {
//            return;
//        }



//        //if (operLogPtr->constainsRemoveOper(EProfileType::Parameter, m_realTimeProfile->getProfileName()))
//        //{
//        //    m_realTimeProfile->setProfileName("");
//        //    return;
//        //}

//        if (operLogPtr->constainsRenameOper(EProfileType::Parameter, m_realTimeProfile->getProfileName()))
//        {
//            auto newName = operLogPtr->getCurrentName(EProfileType::Parameter, m_realTimeProfile->getProfileName());
//            m_realTimeProfile->setProfileName(newName);
//        }

//        if (operLogPtr->constainsValueChangeOper(EProfileType::Parameter, m_realTimeProfile->getProfileName()))
//        {
//            selectProfile(m_realTimeProfile->getProfileName());
//        }
//    };
//    reloadData();


//    auto adjustName = [&]()->void {

//        if (operLogPtr->constainsRemoveOper(EProfileType::Machine, m_realTimeProfile->getMachineName()))
//        {
//            m_realTimeProfile->setMachineName("");
//        }
//        else if (operLogPtr->constainsRenameOper(EProfileType::Machine, m_realTimeProfile->getMachineName()))
//        {
//            auto newName = operLogPtr->getCurrentName(EProfileType::Machine, m_realTimeProfile->getMachineName());
//            m_realTimeProfile->setMachineName(newName);
//        }


//        if (operLogPtr->constainsRemoveOper(EProfileType::Material, m_realTimeProfile->getMaterialName()))
//        {
//            m_realTimeProfile->setMaterialName("");
//        }
//        else if (operLogPtr->constainsRenameOper(EProfileType::Material, m_realTimeProfile->getMaterialName()))
//        {
//            auto newName = operLogPtr->getCurrentName(EProfileType::Material, m_realTimeProfile->getMaterialName());
//            m_realTimeProfile->setMaterialName(newName);
//        }
//    };

//    adjustName();



////    if (m_realTimeProfile->getProfileName() == AkConst::ProfileName::SIMPLE_MODE
////        || m_realTimeProfile->getProfileName() == AkConst::ProfileName::EXPERT_MODE)
////    {
////        if (m_realTimeProfile->getMachineName().isEmpty()
////            || m_realTimeProfile->getMaterialName().isEmpty())
////        {
////            selectProfile(m_realTimeProfile->getProfileName());
////        }

////        return;
////    }

//    if (m_realTimeProfile->getMachineName().isEmpty()
//        || m_realTimeProfile->getMaterialName().isEmpty())
//    {
//        selectProfile(m_realTimeProfile->getProfileName());
//        return;
//    }

//    emit currentParameterNameChanged();
//    emit machineNameChanged();
//    emit materialNameChanged();
//}

void FdmRightParameterService::onGlobalSupportStateChanged(int state)
{
    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return ;
    }
    
    
    FdmSettingItem IdxItem;
    IdxItem.name = AkConst::SettingKey::AI_GLOBAL_SUPPORT_STATE;
    IdxItem.value = state;
    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, IdxItem);

    
    bool supportEnable = (state == EGlobalSupportState::Selected) ? true : false;
    m_realTimeProfile->setSupportEnable(supportEnable);
    setTreeNodeValue(AkConst::SettingKey::SUPPORT_ENABLE        , supportEnable         );

    m_updateTime++;

    emit globalSupportStateChanged();
}

void FdmRightParameterService::onGenerateAdhesionStateChanged(int state)
{
    if (NULL == m_realTimeProfile)
    {
        qDebug() << "realtimeProfile is null";
        return ;
    }
    
    
    FdmSettingItem IdxItem;
    IdxItem.name = AkConst::SettingKey::AI_GENERATE_ADHESION_STATE;
    IdxItem.value = state;
    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, IdxItem);

    
    //int adhesionType = EBuildPlateAdhesionType::AdType_None;
    //if (state == EGenerateAdhesionState::GenAdhesionSelected)
    //{
    //    adhesionType = EBuildPlateAdhesionType::AdType_Brim;
    //}
    QString adhesionType = EBuildPlateAdhesionType::None;
    if (state == EGenerateAdhesionState::GenAdhesionSelected)
    {
        //adhesionType = EBuildPlateAdhesionType::Brim;
        
        auto  adhesionTypeVar = m_realTimeProfile->getSetting(AkConst::Category::AK_AI,  AkConst::SettingKey::AI_ADHESION_TYPE);
        if (!adhesionTypeVar.isNull())
        {
            adhesionType = adhesionTypeVar.toString();
        }
        else
        {
            adhesionType = EBuildPlateAdhesionType::AutoBrim;
        }
    }

    m_realTimeProfile->setAdhesionType(adhesionType);
    setTreeNodeValue(AkConst::SettingKey::ADHESION_TYPE         , adhesionType          );

    m_updateTime++;

    emit generateAdhesionStateChanged();
}
//void FdmRightParameterService::onGenerateAdhesionStateChanged(int state)
//{
//    if (NULL == m_realTimeProfile)
//    {
//        qDebug() << "realtimeProfile is null";
//        return ;
//    }


//    FdmSettingItem IdxItem;
//    IdxItem.name = AkConst::SettingKey::AI_GENERATE_ADHESION_STATE;
//    IdxItem.value = state;
//    m_realTimeProfile->setSetting(AkConst::Category::AK_AI, IdxItem);


//    //int adhesionType = EBuildPlateAdhesionType::AdType_None;
//    //if (state == EGenerateAdhesionState::GenAdhesionSelected)
//    //{
//    //    adhesionType = EBuildPlateAdhesionType::AdType_Brim;
//    //}
//    QString adhesionType = EBuildPlateAdhesionType::None;
//    if (state == EGenerateAdhesionState::GenAdhesionSelected)
//    {
//        adhesionType = EBuildPlateAdhesionType::Brim;
//    }
//    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
//    QList<FdmProfileCategory> categorys;
//    FdmProfileCategory category;
//    category.name = AkConst::Category::PLATFORM_ADHESION;
//    category.set(AkConst::SettingKey::ADHESION_TYPE, adhesionType);
//    categorys.append(category);
//    rightTree.setAny(categorys);

//    m_updateTime++;

//    emit generateAdhesionStateChanged();
//}

void FdmRightParameterService::doOperateComplete()
{
    
    emit machineListChanged();
    emit materialListChanged();
    emit allParameterListChanged();
}

void FdmRightParameterService::onCurrentSelectedNameChanged(const QString &parameterName)
{
    if (parameterName == AkConst::ProfileName::More)
    {
        emit currentParameterNameChanged();
        return;
    }
    m_realTimeProfile->setProfileName(parameterName);
    checkPrintMode();


    selectProfile(parameterName);
//
//    //return FdmParameterProfileManager::Instance().setCurrentSelectedName(newCurrentParameterName);

//    qDebug() << "FdmRightParameterService::currentParameterChanged " << parameterName;
//    FdmParameterProfileManager::Instance().setCurrentSelectedName(parameterName);

//    auto profile = FdmParameterProfileManager::Instance().getCurrentSelectedProfile();
//    if (NULL == profile )
//    {
//        return;
//    }
//

//    emit currentParameterSwitched(parameterName);
//

//    auto realtimeProfile =FdmParameterProfileManager::Instance().getRightRealTimeProfile();
//    //realtimeProfile->updateProfile(profile);
//    updateRealtimeProfile(profile);
//

////    FdmSettingItem currentParameterItem;
////    currentParameterItem.name = AkConst::SettingKey::META_PROFILE_NAME;
////    currentParameterItem.value = parameterName;
////    realtimeProfile->setSetting(AkConst::Category::AK_META, currentParameterItem);
//


//    auto parameterModifyTime = profile->getModifyTime();

//    QString currentMachineId = profile->getMachineName();
//    auto machineProfile = FdmMachineProfileManager::Instance().getProfileById(currentMachineId);
//    if (machineProfile->getModifyTime() > parameterModifyTime)
//    {
//        realtimeProfile->updateMachine(machineProfile);
//    }
//

//    QString currentMaterialId = profile->getMaterialId();
//    auto materialProfile = FdmMaterialProfileManager::Instance().getProfileById(currentMaterialId);
//    if (materialProfile->getModifyTime() > parameterModifyTime)
//    {
//        realtimeProfile->updateMaterial(materialProfile);
//    }
//

//    if (parameterName == AkConst::ProfileName::AI)
//    {

//        onAiQualityCurrentIdxChanged(getAiQualityCurrentIdx());
//        onAiInfillDensityChanged(getAiInfillDensity());
//    }
//

//    refreshTree();
//

//    m_updateTime++;
}

//0.05 0.1 0.2 0.4
int FdmRightParameterService::getAiQualityCurrentIdx() 
{
    int defaultValue = 1;
    
    auto list = getAiQualityList();
    defaultValue = list.size()/2;

    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    auto qualityIdx = realtimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_QUALITY_INDEX);
    if (qualityIdx.isNull())
    {
        onAiQualityCurrentIdxChanged(defaultValue);
        return defaultValue;
    }
    return qualityIdx.toInt();
}

QStringList FdmRightParameterService::getAiQualityStrList()
{
    QList<QString> resultList;
    auto list = getAiQualityList();
    for(int i = 0;i<list.size();i++)
    {
        resultList << QString::number(list[i],'f',2).append("mm");
    }
    return resultList;

}
int FdmRightParameterService::getAiQualityIdx(float layerHeight)
{
//    if(m_realTimeProfile->getPrintMode() == AkConst::PrintMode::SMOOTH)
//    {
//        return getNozzleSize() == AkConst::NozzleSizeName::SIZE2 ? 1 : 0;
//    }
//    else
    {
        int layerHeightInt = layerHeight * 100;
        auto list = getAiQualityList();
        for (int i = 0; i < list.size(); i++)
        {
            int quality = list[i] * 100;

            if (quality == layerHeightInt)
            {
                return i;
            }
        }

        if (list.size() > 2)
        {
            return 1;
        }
    }
    return 0;
}


int FdmRightParameterService::getAiInfillDensityIdx(float density)
{
    QList<QString> resultList;
    int infillDensity = density;
    auto list = getAiInfillDensityList();
    for (int i = 0; i < list.size(); i++)
    {
        if (infillDensity == list[i])
        {
            return i;
        }
    }
    if (list.size() > 2)
    {
        return 1;
    }
    return 0;
}

int FdmRightParameterService::getGlobalSupportState(bool enable)
{
    return enable?Selected:UnSelected;
}

QList<float> FdmRightParameterService::getAiQualityList()
{
    QList<float> resultList { 0.10, 0.20, 0.30};

    auto machineName = getMachineName();
    auto nozzleSizeName = getNozzleSize();
    auto printMode = m_realTimeProfile->getPrintMode();

    if(printMode != AkConst::PrintMode::SMOOTH)
    {
        if(printMode == AkConst::PrintMode::NORMAL
                && nozzleSizeName == AkConst::NozzleSizeName::SIZE2
                && AkConst::MachineName::M5C_SERIES.contains(machineName)){
            return QList<float>{ 0.10 };
        }

        //新需求，根据喷嘴直径设置list  1/4 2/4 3/4
        QString defaultValue = AkConst::NozzleSizeName::SIZE4;
        auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
        auto machineNozzleSize = realtimeProfile->getSetting(AkConst::Category::EXTRUDER_SETTINGS, AkConst::SettingKey::MACHINE_NOZZLE_SIZE);
        if (machineNozzleSize.isNull())
        {
            return resultList;
        }

        float nozzleSize = machineNozzleSize.toFloat();
        resultList = {
            nozzleSize*0.25f,
            nozzleSize*0.5f,
            nozzleSize*0.75f
        };

        if (realtimeProfile->getPrintMode() == AkConst::PrintMode::FAST)
        {
            resultList = {
                0.10f,
                0.20f,
                0.25f,
                0.30f,
            };
        }
    }
    else
    {
        if(nozzleSizeName == AkConst::NozzleSizeName::SIZE4)
        {
            resultList = {
                0.12f,
                0.16f
            };
        }
        else if(nozzleSizeName == AkConst::NozzleSizeName::SIZE2)
        {
//            if(machineName == AkConst::MachineName::M5)
//            {
//                resultList = {
//                    0.1f
//                };
//            }
//            else
            {
                resultList = {
                    0.05f,
                    0.1f
                };
            }
        }
    }

    return resultList;
}
//QList<float> FdmRightParameterService::getAiQualityList()
//{

//    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
//    auto qualityList = realtimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_QUALITY_LIST);
//    if (qualityList.isNull())
//    {
//        QList<QVariant> defaultList;
//        defaultList << 0.05;
//        defaultList << 0.1;
//        defaultList << 0.2;
//        defaultList << 0.4;

//        FdmSettingItem qualityListItem;
//        qualityListItem.name = AkConst::SettingKey::AI_QUALITY_LIST;
//        qualityListItem.value = defaultList;
//        realtimeProfile->setSetting(AkConst::Category::AK_AI, qualityListItem);

//        qualityList = defaultList;
//    }

//    auto list = qualityList.toList();
//    QList<float> resultList;
//    for (int i = 0; i < list.size(); i++)
//    {
//        resultList.append(list[i].toFloat());
//    }
//    return resultList;
//}


int FdmRightParameterService::getAiInfillDensityCurrentIdx() 
{
    int defaultValue = 1;
    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    auto qualityIdx = realtimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_INFILL_DENSITY_INDEX);
    if (qualityIdx.isNull())
    {
        onAiInfillDensityIdxChanged(defaultValue);
        return defaultValue;
    }

    return qualityIdx.toInt();
}


QStringList FdmRightParameterService::getAiInfillDensityStrList()
{
    QList<QString> resultList;
    auto list = getAiInfillDensityList();
    for(int i = 0;i<list.size();i++)
    {
        resultList << QString::number(list[i]).append("%");
    }
    return resultList;
}


int FdmRightParameterService::getGlobalSupportState()
{
    EGlobalSupportState sptState = EGlobalSupportState::Unactived;
    calcGlobalSupportTextCode();
    switch (sptTxtCode) {
        case EGlobalSupportTextCode::GlobalSupportUnactived:
        {
            sptState = EGlobalSupportState::Unactived;
            break;
        }
        case EGlobalSupportTextCode::GlobalSupportAndManualSupport:
        case EGlobalSupportTextCode::GlobalSupportOnly:
        {
            sptState = EGlobalSupportState::Selected;
            break;
        }
        case EGlobalSupportTextCode::GlobalSupportUnSelected:
        {
            sptState = EGlobalSupportState::UnSelected;
            break;
        }
    }

    
    emit globalSupportTextCodeChanged();
    return sptState;
}

int FdmRightParameterService::getAdhesionTypeState(QString type)
{
    return (type == EBuildPlateAdhesionType::None) ? 1: 0;
//    if(type == EBuildPlateAdhesionType::None) {return EGenerateAdhesionState::None;}
//    else if(type == EBuildPlateAdhesionType::Brim) {return EGenerateAdhesionState::Brim;}
//    else if(type == EBuildPlateAdhesionType::Raft) {return EGenerateAdhesionState::Raft;}
//    else if(type == EBuildPlateAdhesionType::Skirt) {return EGenerateAdhesionState::Skirt;}
}


int FdmRightParameterService::getGenerateAdhesionState()
{
    int defaultValue = EGenerateAdhesionState::GenAdhesionSelected;
    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    auto adhesionTypeVar = realtimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_GENERATE_ADHESION_STATE);
    if (adhesionTypeVar.isNull())
    {
        
        onGenerateAdhesionStateChanged(defaultValue);
        return defaultValue;
    }
    
    return adhesionTypeVar.toInt();
}

int FdmRightParameterService::getGlobalSupportTextCode()
{
    return sptTxtCode;
}


QString FdmRightParameterService::getNozzleSize()
{
    QString defaultValue = AkConst::NozzleSizeName::SIZE4;
    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    auto machineNozzleSize = realtimeProfile->getSetting(AkConst::Category::EXTRUDER_SETTINGS, AkConst::SettingKey::MACHINE_NOZZLE_SIZE);
    if (machineNozzleSize.isNull())
    {
        return defaultValue;
    }
    int nozzleSize = (int)(machineNozzleSize.toFloat()*10);
    switch (nozzleSize) {
        case 2: return AkConst::NozzleSizeName::SIZE2;
        case 4: return AkConst::NozzleSizeName::SIZE4;
        case 6: return AkConst::NozzleSizeName::SIZE6;
        case 8: return AkConst::NozzleSizeName::SIZE8;
        default: return AkConst::NozzleSizeName::SIZE4;
    }
}


QStringList FdmRightParameterService::getNozzleSizeList()
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
    resultList << AkConst::NozzleSizeName::SIZE8;

    //材料不影响喷嘴 不反向传播 2023、07、31
//    //先屏蔽掉0.8的nozzle。打印效果不好
//    if (getMaterialName() == AkConst::MaterialName::TPU)
//    {
//         resultList.removeOne(AkConst::NozzleSizeName::SIZE8);
//    }
//    else if(getMaterialName() == AkConst::MaterialName::PA_CF)
//    {
//         resultList.removeOne(AkConst::NozzleSizeName::SIZE2);
//         resultList.removeOne(AkConst::NozzleSizeName::SIZE6);
//         resultList.removeOne(AkConst::NozzleSizeName::SIZE8);
//    }

    if(AkConst::MachineName::M5C_SERIES.contains(getMachineName()))
    {
        resultList.removeOne(AkConst::NozzleSizeName::SIZE6);
        resultList.removeOne(AkConst::NozzleSizeName::SIZE8);
    }
    return resultList;


}


void FdmRightParameterService::calcGlobalSupportTextCode()
{
    sptTxtCode = EGlobalSupportTextCode::GlobalSupportUnactived;
    
    
    if (m_globalSupportForbidden)
    {
        
        onGlobalSupportStateChanged(EGlobalSupportState::UnSelected);
        return ;
    }

    
    int globalSptState = EGlobalSupportState::Unactived;
    auto realtimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    auto globalSptStateItem = realtimeProfile->getSetting(AkConst::Category::AK_AI, AkConst::SettingKey::AI_GLOBAL_SUPPORT_STATE);
    if (!globalSptStateItem.isNull())
    {
        globalSptState =  globalSptStateItem.toInt();
    }
    switch (globalSptState) {
        
        case EGlobalSupportState::UnSelected:
        case EGlobalSupportState::Unactived:
        {
            sptTxtCode = EGlobalSupportTextCode::GlobalSupportUnSelected;
            break;
        }
        case  EGlobalSupportState::Selected:
        {
            
            if (m_supportMeshCount>0)
            {
                sptTxtCode =  EGlobalSupportTextCode::GlobalSupportAndManualSupport;
            }
            else
            {
                
                sptTxtCode = EGlobalSupportTextCode::GlobalSupportOnly;
            }
            break;
        }
    }
}

#ifdef TOOL_SAVE_AS_PARAM_INI
QString FdmRightParameterService::__ToolSaveAs_1()
{
    QString str, str1, str2 ,str3;
    str1 = getMachineName().split(" ").last();
    str2 = QString::number(int(10 * m_realTimeProfile->getSetting(AkConst::Category::EXTRUDER_SETTINGS, AkConst::SettingKey::MACHINE_NOZZLE_SIZE).toDouble() ));
    str3 = getMaterialName();
    str = QString("%1_%2_%3.ini").arg(str1).arg(str2).arg(str3);
    AkUtil::TFunction(str);
    return str;
}

QString FdmRightParameterService::__ToolSaveAs_2(QString str)
{
    return QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).absoluteFilePath("ini/"+str);
}
#endif
