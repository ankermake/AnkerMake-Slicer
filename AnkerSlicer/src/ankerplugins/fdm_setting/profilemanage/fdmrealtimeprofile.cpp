#include "fdmrealtimeprofile.h"
#include <QDir>
#include "common/ak_const.h"
#include "fdmparameterprofilemanager.h"

FdmRealTimeProfile::FdmRealTimeProfile(const QString& dateSource):FdmParameterProfile(dateSource, ProfileGroup::Realtime)
{
}

FdmRealTimeProfile::FdmRealTimeProfile(const QString& dateSource, const QList<FdmProfileCategory>& categories) : FdmParameterProfile(dateSource, categories, ProfileGroup::Realtime)
{
}


void FdmRealTimeProfile::reload(FdmRealTimeProfile* rtProfile)
{
    if (NULL == rtProfile)
    {
        return;
    }

    
    //clearData();

    for (FdmProfileCategory& category : *(rtProfile->getAllCategories()))
    {
        setCategory(category);
    }
    setModifyTimeNow();
}

QString FdmRealTimeProfile::getProfileName()
{
    auto profileName = getRealTimeMetaDate(AkConst::SettingKey::META_CURRENT_PROFILE_NAME);
    if (profileName.isNull())
    {
        return "";
    }
    return profileName.toString();
}

void FdmRealTimeProfile::setProfileName(QString name)
{
    setRealTimeMetaDate(AkConst::SettingKey::META_CURRENT_PROFILE_NAME, name);
}

void FdmRealTimeProfile::setRealTimeMetaDate(const QString& key, const QVariant& value)
{
    if (!categoryDict.contains(AkConst::Category::AK_META_REALTIME)) {
        FdmProfileCategory category(AkConst::Category::AK_META_REALTIME);
        setCategory(category);
    }
    categoryDict[AkConst::Category::AK_META_REALTIME]->set(key, value);
}

QVariant FdmRealTimeProfile::getRealTimeMetaDate(const QString& key)
{
    if (!categoryDict.contains(AkConst::Category::AK_META_REALTIME)) {
        return QVariant();
    }
    if (!categoryDict[AkConst::Category::AK_META_REALTIME]->contains(key))
    {
        return QVariant();
    }

    return categoryDict[AkConst::Category::AK_META_REALTIME]->get(key);
}

void FdmRealTimeProfile::getInnerProfileCategories(QList<FdmProfileCategory>& categories)
{
    for(FdmProfileCategory& category: *(getAllCategories()))
    {
        if(category.name == AkConst::Category::AK_AI
            || category.name == AkConst::Category::AK_META_REALTIME)
        {
            continue;
        }
        categories.append(category);
    }
}


void FdmRealTimeProfile::updateProfile(FdmParameterProfile *profile)
{
    if (NULL == profile)
    {
        return;
    }
    
    
    
    //clearExcludeAkData();
    for(FdmProfileCategory& category: *(profile->getAllCategories()))
    {
        
        if(category.name == AkConst::Category::AK_META)
        {
            continue;
        }
        setCategory(category);
    }
    setMachineName(profile->getMachineName());
    setMaterialName(profile->getMaterialName());
    setModifyTimeNow();
}

