#include "fdmparameterprofile.h"
#include "common/ak_const.h"
#include <QException>


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

void FdmParameterProfile::updateNozzleSize(double nozzleSize)
{
    FdmSettingItem IdxItem;
    IdxItem.name = AkConst::SettingKey::MACHINE_NOZZLE_SIZE;
    IdxItem.value = nozzleSize;
    setSetting(AkConst::Category::MACHINE_SETTINGS, IdxItem);
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


void FdmParameterProfile::setMachineName(QString name)
{
    setMetaData(AkConst::SettingKey::META_CURRENT_MACHINE_NAME, name);
}

void FdmParameterProfile::setMaterialName(QString name)
{
    setMetaData(AkConst::SettingKey::META_CURRENT_MATERIAL_NAME, name);
}
