#include "fdmparameterprofile.h"
#include "common/ak_const.h"
#include <QException>
#include <QDebug>

void FdmParameterProfile::setExtruderCategory(const FdmProfileCategory &category)
{
    
    //QString destcategoryName = AkConst::Category::EXTRUDER_SETTINGS + "_" + activeExtruderIdx;
    QString destcategoryName = AkConst::Category::EXTRUDER_SETTINGS;
    FdmProfileCategory newCategory = category;
    newCategory.name = destcategoryName;
    setCategory(newCategory);
}

FdmProfileCategory FdmParameterProfile::getExtruderCategory()
{
    //QString destCategoryName = QString("%1_%2").arg(AkConst::Category::EXTRUDER_SETTINGS).arg(activeExtruderIdx);
    QString destCategoryName = AkConst::Category::EXTRUDER_SETTINGS;
    auto destcategory = getCategory(destCategoryName);
    if (destcategory == NULL)
    {
//#if defined(__GNUG__) || defined(__APPLE__)
//        throw("FdmParameterProfile.getExtruderCategory: can't found the destcategory ");
//#else
//        throw std::exception("FdmParameterProfile.getExtruderCategory: can't found the destcategory ");
//#endif
        
        FdmProfileCategory emptyResult;
        emptyResult.name = AkConst::Category::EXTRUDER_SETTINGS;
        return emptyResult;
    }
    FdmProfileCategory result = *destcategory;
    result.name = AkConst::Category::EXTRUDER_SETTINGS;
    return result;
}


void FdmParameterProfile::getCategoriesForRefreshTree(QList<FdmProfileCategory> &categories)
{
    auto allCategories = getAllCategories();
    
    auto extruderSetting = getExtruderCategory();
    bool extruderHasAdded =false;
    for(int i = 0; i < allCategories->size();i++)
    {
        auto category = allCategories->value(i);
        if (category.name.indexOf(AkConst::Category::EXTRUDER_SETTINGS)>=0)
        {
            if (extruderHasAdded)
            {
                continue;
            }
            categories.append(extruderSetting);
            extruderHasAdded = true;
        }
        else
        {
            categories.append(category);
        }
    }
}


void FdmParameterProfile::setSetting(const QString &categoryName, const FdmSettingItem &item)
{
    
    FdmBaseProfile::setSetting(categoryName, item);

    
    
}




//void FdmParameterProfile::setPersistSetting(const QString &categoryName, const FdmSettingItem &item)
//{

//}

int FdmParameterProfile::getExtruderCount()
{
    auto extruderCount = getSetting(AkConst::Category::MACHINE_SETTINGS, AkConst::SettingKey::MACHINE_EXTRUDER_COUNT);
    if (extruderCount.isNull())
    {
        return 1;
    }
    return extruderCount.toInt();
}



void FdmParameterProfile::updateMachine(FdmMachineProfile *profile)
{
    if (NULL == profile)
    {
        return;
    }

    
    //clearCategory(AkConst::Category::MACHINE_SETTINGS);

    for(FdmProfileCategory& category: *(profile->getAllCategories()))
    {
        if(category.name == AkConst::Category::AK_META)
        {
            continue;
        }
        setCategory(category);
    }
    setMachineName(profile->getName());
    setModifyTimeNow();
}

void FdmParameterProfile::updateMaterial(FdmMaterialProfile *profile)
{
    if (NULL == profile)
    {
        return;
    }

    
    //clearCategory(AkConst::Category::MATERIAL);

    for(FdmProfileCategory& category: *(profile->getAllCategories()))
    {
        if(category.name == AkConst::Category::AK_META)
        {
            continue;
        }
        setCategory(category);
    }

    
    setMaterialName(profile->getName());
    setModifyTimeNow();
}

QString FdmParameterProfile::getMachineName()
{
    //auto akMachineId = getSetting(AkConst::Category::MACHINE_SETTINGS, AkConst::SettingKey::AK_MACHINE_ID);
    auto akMachineName = getMetaData(AkConst::SettingKey::META_CURRENT_MACHINE_NAME);
    if (akMachineName.isNull())
    {
        return "";
    }
    return akMachineName.toString();
}

QString FdmParameterProfile::getMaterialName()
{
    //auto akMaterialId = getSetting(AkConst::Category::MACHINE_SETTINGS, AkConst::SettingKey::AK_MATERIAL_ID);
    auto akMaterialName = getMetaData(AkConst::SettingKey::META_CURRENT_MATERIAL_NAME);
    if (akMaterialName.isNull())
    {
        return "";
    }
    return akMaterialName.toString();
}

//
double FdmParameterProfile::getNozzleSize()
{
    auto nozzleSizeVar = getSetting(AkConst::Category::MACHINE_SETTINGS, AkConst::SettingKey::MACHINE_NOZZLE_SIZE);
    if (nozzleSizeVar.isNull())
    {
        return 0.4;
    }
    return nozzleSizeVar.toDouble();
}

QString FdmParameterProfile::getPrintMode()
{
    auto printModeVar = getSetting(AkConst::Category::BASE_PARAM, AkConst::SettingKey::QML_PRINT_MODE);
    if (printModeVar.isNull())
    {
        qDebug() << "printModeVar.isNull()" << getOriginName();
        return AkConst::PrintMode::NORMAL;
    }
    return printModeVar.toString();
}

/*
    const QString QML_PRINT_MODE      = "param_print_mode";
    const QString QML_MACHINE_NAME    = "machine_index_name";
    const QString QML_MATERIAL_NAME   = "material_index_name";
    const QString QML_NOZZLE_NAME     = "extruder_index_name";
    const QString MACHINE_NOZZLE_SIZE = "machine_nozzle_size";
    const QString QML_PARAMETER_NAME  = "parameter_index_name";
*/
void FdmParameterProfile::setPrintMode(QString value)
{  
    qDebug() << "FdmParameterProfile::setPrintMode" << value;
    setBaseParamData(AkConst::SettingKey::QML_PRINT_MODE, value);
    //qDebug() << "FdmParameterProfile::getPrintMode" << getPrintMode();
}

void FdmParameterProfile::setMachineName(QString value)
{
    setMetaData(AkConst::SettingKey::META_CURRENT_MACHINE_NAME, value);
    setBaseParamData(AkConst::SettingKey::QML_MACHINE_NAME, value);
}

void FdmParameterProfile::setMaterialName(QString value)
{
    setMetaData(AkConst::SettingKey::META_CURRENT_MATERIAL_NAME, value);
    setBaseParamData(AkConst::SettingKey::QML_MATERIAL_NAME, value);
}

void FdmParameterProfile::setNozzleName(QString value)
{
    setMetaData(AkConst::SettingKey::META_CURRENT_NOZZLE_NAME, value);
    setBaseParamData(AkConst::SettingKey::QML_NOZZLE_NAME, value);
}

void FdmParameterProfile::setNozzleSize(double value)
{
    setBaseParamData(AkConst::SettingKey::MACHINE_NOZZLE_SIZE, value);
}

void FdmParameterProfile::setProfileName(QString value)
{
    //setRealTimeMetaDate(AkConst::SettingKey::META_CURRENT_PROFILE_NAME, value);
    setBaseParamData(AkConst::SettingKey::QML_PARAMETER_NAME, value);
}

void FdmParameterProfile::setAdhesionType(QString value){
    setBaseParamData(AkConst::SettingKey::ADHESION_TYPE, value);
}

void FdmParameterProfile::setLayerHeight(double value){
    setBaseParamData(AkConst::SettingKey::LAYER_HEIGHT, value);
}

void FdmParameterProfile::setInfillDensity(double value){
    setBaseParamData(AkConst::SettingKey::INFILL_SPARSE_DENSITY, value);
}

void FdmParameterProfile::setInfillThickness(double value){
    setBaseParamData(AkConst::SettingKey::INFILL_SPARSE_THICKNESS, value);
}

void FdmParameterProfile::setSupportEnable(bool value){
    setBaseParamData(AkConst::SettingKey::SUPPORT_ENABLE, value);
}







