#include "fdmbaseprofile.h"
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "common/utilities/ioapi.h"
#include <QSettings>
#include <QDir>
#include "QUuid"
#include "common/ak_const.h"
#include <QDebug>
#include <QTextCodec>
using namespace AkUtil;


FdmBaseProfile::FdmBaseProfile(const QString& fileFullName, ProfileGroup group)
{
    this->dataSource = fileFullName;
    QFileInfo fInfo(fileFullName);
    this->group = group;
    load();
    
    //initMetaData();
    
    setStatus(EProfileStatus::Normal);
    originName = getName();
}

//
FdmBaseProfile::FdmBaseProfile(const QString &fileFullName, const QList<FdmProfileCategory> &categories, ProfileGroup group)
{
    this->dataSource = fileFullName;
    this->group = group;
    clearData();
    setCategories(categories);

    //this->categories = categories;
    //for(FdmProfileCategory& item:  this->categories)
    //{
    //    categoryDict[item.name] = &item;
    //}
    
    //initMetaData();
    originName = getName();
}


FdmBaseProfile::FdmBaseProfile(const FdmBaseProfile& profile)
{
    clone(profile);
}

FdmBaseProfile::~FdmBaseProfile()
{
}

void FdmBaseProfile::operator=(const FdmBaseProfile &profile)
{
    clone(profile);
}


void FdmBaseProfile::clone(const FdmBaseProfile &profile)
{
    
    if (this == &profile)
    {
        return;
    }

    this->categories.clear();
    this->categoryDict.clear();
    
    this->group = profile.group;
    this->dataSource = profile.dataSource;
    this->originName = profile.originName;
    this->status = profile.status;

    setCategories(profile.categories);
    //this->categories = profile.categories;
    
    //for(FdmProfileCategory& item:  categories)
    //{
    //    categoryDict[item.name] = &item;
    //}

    //initMetaData();

}


void FdmBaseProfile::load()
{
    if (!QFile::exists(dataSource))
    {
        return;
    }
    
    this->categories.clear();
    this->categoryDict.clear();

    //if(dataSource == "C:/Users/Administrator/AppData/Local/AnkerSlicer/AnkerMake_64bit_fp/setting/fdm/parameter/custom/{0c3cbb5d-dd46-46d2-8c8c-42932ca69c0a}.ini")
    //{
    //    int x = 0;
    //}
    
    QSettings ini(dataSource, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName("utf-8")); 
    QList<QString> lines = IoApi::getLines(dataSource);
    
    QString name("");
    QList<QString> items;
    for(int i = 0; i<lines.size();i++)
    {
        QString line = lines[i].trimmed();
        
        if (line.isEmpty() || line.startsWith(";"))
        {
            continue;
        }
        
        if(line.startsWith('[') && line.endsWith(']') && line.size() > 2)
        {
            
            if (items.size()>0)
            {
                FdmProfileCategory category(name, items, ini);
                this->categories.append(category);
                this->categoryDict[name] = &(this->categories.last());
                //reset
                name = "";
                items.clear();
            }
            
            name = line.mid(1,line.size()-2).trimmed();
        }
        
        else
        {
            items.append(line);
        }

    }

    
    if (name != "")
    {
        FdmProfileCategory category(name, items, ini);
        this->categories.append(category);
        this->categoryDict[name] = &(this->categories.last());
    }

    mergeCategory();
}


void FdmBaseProfile::mergeCategory()
{
    QList<FdmProfileCategory> newCategories = categories;
    categories.clear();
    categoryDict.clear();
    setCategories(newCategories);
}


QString FdmBaseProfile::mergeCategoryName(QString cateName)
{
    if (cateName != AkConst::Category::AK_AI
        && cateName != AkConst::Category::AK_META
        && cateName != AkConst::Category::AK_META_REALTIME)
    {
        return AkConst::Category::BASE_PARAM;
    }
    return cateName;
}

void FdmBaseProfile::save(bool saveDefault)
{
    if (!saveDefault && group == ProfileGroup::Default)
    {
        return;
    }
    save(dataSource);
}

void FdmBaseProfile::save(QString path)
{
    
    QSettings ini(path, QSettings::IniFormat);
    
    ini.clear();
    ini.setIniCodec(QTextCodec::codecForName("utf-8")); 
    for(int i=0;i<categories.size();i++)
    {
        const FdmProfileCategory& category = categories[i];
        ini.beginGroup(category.name);
        for(const FdmSettingItem & item : category.getItems()){
//            if (item.name.isEmpty())
//            {
//                int x = 0;
//            }
            
            if (item.value.type() == QVariant::Type::Double || item.value.type() == QMetaType::Float)
            {
                ini.setValue(item.name, QString::number(item.value.toDouble(), 'f', 2));
            }
            else
            {
                ini.setValue(item.name, item.value);
            }
        }
        ini.endGroup();
    }
    
    setStatus(EProfileStatus::Normal);
}


void FdmBaseProfile::clearExcludeAkData()
{
    
    QList<FdmProfileCategory> remainCategories;
    if (NULL != getCategory(AkConst::Category::AK_META))
    {
        remainCategories.append(*getCategory(AkConst::Category::AK_META));
    }
    if (NULL != getCategory(AkConst::Category::AK_AI))
    {
        remainCategories.append(*getCategory(AkConst::Category::AK_AI));
    }
    if (NULL != getCategory(AkConst::Category::AK_META_REALTIME))
    {
        remainCategories.append(*getCategory(AkConst::Category::AK_META_REALTIME));
    }

    
    categories.clear();
    categoryDict.clear();
    for (int i = 0; i < remainCategories.size(); i++)
    {
        auto remainCate = remainCategories[i];
        categories.append(remainCategories[i]);
        categoryDict[remainCate.name] = &categories.last();
    }
}

void FdmBaseProfile::clearData()
{
    categories.clear();
    categoryDict.clear();
}

bool FdmBaseProfile::isModified()
{
    return status == EProfileStatus::Normal;
}


void FdmBaseProfile::remove(const QString &categoryName)
{
    if (!categoryDict.contains(categoryName))
    {
        return;
    }
    auto destCategory = categoryDict[categoryName];
    
    categoryDict.remove(categoryName);
    categories.removeOne(*destCategory);
}

void FdmBaseProfile::clearCategory(const QString& categoryName)
{
    if (!categoryDict.contains(categoryName))
    {
        return;
    }
    auto destCategory = categoryDict[categoryName];
    destCategory->clear();
}


void FdmBaseProfile::remove(const QString &categoryName, const QString &key)
{
    if (!categoryDict.contains(categoryName))
    {
        return;
    }
    auto destCategory = categoryDict[categoryName];
    if (!destCategory->contains(key))
    {
        return;
    }

    
    categoryDict.remove(categoryName);
    categories.removeOne(*destCategory);

}

QVariant FdmBaseProfile::getSetting(const QString &categoryName, const QString &key)
{
    auto mergeName = mergeCategoryName(categoryName);

    if (!categoryDict.contains(mergeName))
    {
        return QVariant();
    }
    return this->categoryDict[mergeName]->get(key);
}

FdmProfileCategory *FdmBaseProfile::getCategory(const QString &categoryName)
{
    auto mergeName = mergeCategoryName(categoryName);
    if (!categoryDict.contains(mergeName))
    {
        return nullptr;
    }
    return this->categoryDict[mergeName];
}
QList<FdmProfileCategory> *FdmBaseProfile::getAllCategories()
{
     return &categories;
}


void FdmBaseProfile::setCategory(const FdmProfileCategory &category)
{
    //if (category.name == "material")
    //{
    //    int x = 0;
    //}
    auto mergeName = mergeCategoryName(category.name);
    
    if (!categoryDict.contains(mergeName))
    {
        
        categories.push_back(category);
        categories.last().name = mergeName;
        categoryDict[mergeName] = &(categories.last());
        return;
    }

    
    categoryDict[mergeName]->set(category.getSettings());
}

void FdmBaseProfile::setCategories(const QList<FdmProfileCategory> &categories)
{
    for(int i=0;i<categories.size();i++)
    {
        setCategory(categories[i]);
    }
}

QDateTime FdmBaseProfile::getModifyTime() const
{
    return getMetaData(AkConst::SettingKey::META_MODIFY_TIME).toDateTime();
}

QDateTime FdmBaseProfile::getCreateTime() const
{
    return getMetaData(AkConst::SettingKey::META_CREATE_TIME).toDateTime();
}


void FdmBaseProfile::setModifyTimeNow()
{
    setModifyTime(QDateTime::currentDateTime());
}

void FdmBaseProfile::setCreateTimeNow()
{
    setCreateTime(QDateTime::currentDateTime());
}

void FdmBaseProfile::setModifyTime(const QDateTime &time)
{
     setMetaData(AkConst::SettingKey::META_MODIFY_TIME_STR,  time.toString("yyyy-MM-dd hh:mm:ss"));
     setMetaData(AkConst::SettingKey::META_MODIFY_TIME,  time);
}

void FdmBaseProfile::setCreateTime(const QDateTime &time)
{
    setMetaData(AkConst::SettingKey::META_CREATE_TIME_STR,  time.toString("yyyy-MM-dd hh:mm:ss"));
    setMetaData(AkConst::SettingKey::META_CREATE_TIME, time);
}


void FdmBaseProfile::setMetaDataIfNotExist(const QString &key, const QVariant &value)
{
    if (!categoryDict.contains(AkConst::Category::AK_META)){
        FdmProfileCategory category(AkConst::Category::AK_META);
        setCategory(category);
    }

    if (!categoryDict[AkConst::Category::AK_META]->contains(key))
    {
        categoryDict[AkConst::Category::AK_META]->set(key, value);
    }
}

void FdmBaseProfile::setMetaData(const QString &key, const QVariant &value)
{
    //setMetaDataIfNotExist(key,value);
    if (!categoryDict.contains(AkConst::Category::AK_META)) {
        FdmProfileCategory category(AkConst::Category::AK_META);
        setCategory(category);
    }
    categoryDict[AkConst::Category::AK_META]->set(key, value);
    //status = ProfileStatus::NodeValueChanged;
}

QVariant FdmBaseProfile::getMetaData(const QString &key) const
{
    if (!categoryDict.contains(AkConst::Category::AK_META)){
        return QVariant();
    }
    if (!categoryDict[AkConst::Category::AK_META]->contains(key))
    {
        return QVariant();
    }

    return categoryDict[AkConst::Category::AK_META]->get(key);
}

int FdmBaseProfile::getStatus() const
{
    return status;
}


void FdmBaseProfile::addStatus(EProfileStatus newStatus)
{
    status |= newStatus;
    
    //setModifyTimeNow();
}

void FdmBaseProfile::setStatus(EProfileStatus newStatus)
{
    status = newStatus;
}




void FdmBaseProfile::setSetting(const QString& cateName, const FdmSettingItem& item)
{
    
    auto categoryName = mergeCategoryName(cateName);

    //qDebug() << "-- setSetting -- categoryName key value fdmSettablePerExtruder " << categoryName << " " << item.name << " " << item.value << " " << item.fdmSettablePerExtruder;
    if (!categoryDict.contains(categoryName)) {
        FdmProfileCategory category(categoryName);
        setCategory(category);
        
        addStatus(EProfileStatus::NodeValueChanged);
    }

    

    
    QVariant originValue = getSetting(categoryName, item.name);
    if (originValue != item.value)
    {
        qDebug() << QString("*** file %1 content changed. categoryName %2 key=%3").arg(getName()).arg(categoryName).arg(item.name)
            << originValue << item.value;

        categoryDict[categoryName]->set(item.name, item.value);
        addStatus(EProfileStatus::NodeValueChanged);
        
        //setModifyTimeNow();
    }
}

ProfileGroup FdmBaseProfile::getGroup() const
{
    return group;
}

void FdmBaseProfile::setGroup(ProfileGroup newGroup)
{
    group = newGroup;
}

bool FdmBaseProfile::getVisible() const
{
    return getMetaData(AkConst::SettingKey::META_PROFILE_VISIBLE).toBool();
}

void FdmBaseProfile::setVisible(bool visible)
{
    setMetaData(AkConst::SettingKey::META_PROFILE_VISIBLE,  visible);
}


//const QString FdmBaseProfile::getId() const
//{
//    return getMetaData(AkConst::SettingKey::META_PROFILE_ID).toString();
//}

const QString FdmBaseProfile::getName() const
{
    return getMetaData(AkConst::SettingKey::META_PROFILE_NAME).toString();
}


void FdmBaseProfile::setName(const QString &newName)
{
    setMetaData(AkConst::SettingKey::META_PROFILE_NAME, newName);
    setStatus(EProfileStatus::Renamed);
}

const QString &FdmBaseProfile::getDataSource() const
{
    return dataSource;
}

void FdmBaseProfile::setDataSource(const QString &newDataSource)
{
    dataSource = newDataSource;
}


//void FdmBaseProfile::initMetaData()
//{
//    //setMetaDataIfNotExist(AkConst::SettingKey::META_PROFILE_ID,  QUuid::createUuid().toString());
//    setMetaDataIfNotExist(AkConst::SettingKey::META_CREATE_TIME, QDateTime::currentDateTime());
//    setMetaDataIfNotExist(AkConst::SettingKey::META_MODIFY_TIME, QDateTime::currentDateTime());
//}


//void FdmBaseProfile::regenMetaData()
//{
//    //setMetaData(AkConst::SettingKey::META_PROFILE_ID,  QUuid::createUuid().toString());
//    setMetaData(AkConst::SettingKey::META_CREATE_TIME, QDateTime::currentDateTime());
//    setMetaData(AkConst::SettingKey::META_MODIFY_TIME, QDateTime::currentDateTime());
//}
