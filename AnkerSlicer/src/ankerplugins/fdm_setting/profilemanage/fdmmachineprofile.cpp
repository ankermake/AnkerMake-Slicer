#include "fdmmachineprofile.h"
#include "common/ak_const.h"


int FdmMachineProfile::getExtruderCount()
{
    return getSetting(AkConst::Category::MACHINE_SETTINGS, AkConst::SettingKey::MACHINE_EXTRUDER_COUNT).toInt() ;
}


FdmProfileCategory FdmMachineProfile::getExtruderCategory(int extruderNum)
{
    if (extruderNum == 0)
    {
        return FdmProfileCategory();
    }
    //QString extruder_setting_n = QString("%1_%2").arg(AkConst::Category::EXTRUDER_SETTINGS).arg(activeComponentIdx);
    
    QString extruder_setting_n = AkConst::Category::EXTRUDER_SETTINGS;
    auto extruderCategory = getCategory(extruder_setting_n);
    if (extruderCategory == NULL)
    {
#if defined(__GNUG__) || defined(__APPLE__)
        throw("FdmMachineProfile.getExtruderCategory: can't found the destcategory ");
#else
        throw std::exception("FdmMachineProfile.getExtruderCategory: can't found the destcategory ");
#endif

    }
    
    FdmProfileCategory result(*extruderCategory);
    result.name = AkConst::Category::EXTRUDER_SETTINGS;
    return result;
}


const FdmProfileCategory * FdmMachineProfile::getMachineCategory()
{
    return getCategory(AkConst::Category::MACHINE_SETTINGS);
}

void FdmMachineProfile::setExtruderCategory(const FdmProfileCategory &)
{
    
}


void FdmMachineProfile::getCategoriesForRefreshTree(int extruderNum, QList<FdmProfileCategory> &categories)
{
    auto machineCate = getMachineCategory();
    if (nullptr == machineCate)
    {
        return;
    }
    categories.append(*machineCate);

    categories.append(getExtruderCategory(extruderNum));
}

void FdmMachineProfile::getComponentCategories(int componentIdx, QList<FdmProfileCategory> &categories)
{
    if (componentIdx == 0)
    {
        categories.append(*getMachineCategory());
    }
    else
    {
        categories.append(getExtruderCategory(componentIdx));
    }
}
void FdmMachineProfile::setSetting(const QString& categoryName, const FdmSettingItem& item)
{
    
    FdmBaseProfile::setSetting(categoryName, item);
}
//
//int FdmMachineProfile::getActiveComponentIdx() const
//{
//    return activeComponentIdx;
//}
//
//void FdmMachineProfile::setActiveComponentIdx(int newActiveComponentIdx)
//{
//    activeComponentIdx = newActiveComponentIdx;
//}
