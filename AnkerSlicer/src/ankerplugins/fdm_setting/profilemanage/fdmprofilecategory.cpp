#include "fdmprofilecategory.h"
#include <QRegularExpression>
#include <QDebug>
#include "../filemanage/fdmqmlsourcetree.h"
#include "common/utilities/tlogger.h"
#include "common/ak_const.h"
//FdmProfileCategory::FdmProfileCategory()
//{

//}

FdmProfileCategory::FdmProfileCategory()
{
    settingDict.clear();
    settingItems.clear();
}

FdmProfileCategory::FdmProfileCategory(const QString categoryName)
{
    name = categoryName;
    settingDict.clear();
    settingItems.clear();
}

FdmProfileCategory::~FdmProfileCategory()
{
    settingDict.clear();
    settingItems.clear();
}

FdmProfileCategory::FdmProfileCategory(const QString categoryName, const QList<QString> &items, const QSettings &settingMap)
{
    //if (categoryName == "experimental")
    //{
    //    int x = 1;
    //}
    settingDict.clear();
    settingItems.clear();
    name = categoryName;
    set(items, settingMap);
}

void FdmProfileCategory::set(const QList<QString> &items, const QSettings &settingMap)
{
    QStringList rawkeys = settingMap.allKeys();
    FdmQmlTreeApi& rightTree = FdmQmlSourceTree::instance().getFdmQmlTreeApi_Right();
    for(int i =0;i < items.size();i++)
    {
        const QString & item = items[i];
        int equalIdx = item.indexOf('=');
        
        if (equalIdx <= 0)
        {
            AkUtil::TWarning("skip unexpected line " + item);
            continue;
        }
        QString settingName = item.mid(0,equalIdx).trimmed();
        
        if (settingName.length() > AkConst::SettingKey::MAX_NAME_SIZE)
        {
            AkUtil::TWarning("skip unexpected line " + item);
            continue;
        }
        //if (settingName == "support_zag_skip_count")
        //{
        //    int x = 0;
        //}
        QString key = QString("%1/%2").arg(name, settingName);

        
        //QString value = item.mid(equalIdx+1).trimmed();
        QVariant value = settingMap.value(key);

        QVariant strvalue = value;
        QVariant::Type type = rightTree.getTypeOfKey(settingName);
        if (type == QVariant::Type::Int)
        {
            value = int(value.toDouble());
        }
        else if(value.canConvert(type)){
            value.convert(type);
        }
        //qDebug() <<  __FUNCTION__ << __LINE__ << key << value << strvalue;
        FdmSettingItem settingItem;
        settingItem.name = settingName;
        settingItem.value = value;
        settingItems.append(settingItem);
        settingDict[settingName] = &settingItems.last();
    }
}


FdmProfileCategory::FdmProfileCategory(const FdmProfileCategory &category)
{
    settingDict.clear();
    settingItems.clear();
    this->name = category.name;
    this->settingItems = category.settingItems;
    
    for(FdmSettingItem& item:  settingItems)
    {
        settingDict[item.name] = &item;
    }
}

QVariant FdmProfileCategory::get(const QString &key)
{
    FdmSettingItem* temp = settingDict.value(key, nullptr);
    if(temp){
        return temp->value;
    }
    return QVariant();
}

void FdmProfileCategory::set(const QString &key, const QVariant &value)
{
    __append(key, value);
}

void FdmProfileCategory::set(const QList<FdmSettingItem> &items)
{
    for(const FdmSettingItem & item : items){
        __append(item.name, item.value);
    }
}


void FdmProfileCategory::remove(const QString &key)
{
    if (!settingDict.contains(key))
    {
        return;
    }
    FdmSettingItem * dest = settingDict[key];
    settingDict.remove(key);
    settingItems.removeOne(*dest);
}

bool FdmProfileCategory::contains(const QString &key)
{
    return settingDict.contains(key);
}

void FdmProfileCategory::clear()
{
    settingDict.clear();
    settingItems.clear();
}

void FdmProfileCategory::__append(const QString &key, const QVariant &value)
{
    FdmSettingItem* temp = settingDict.value(key, nullptr);
    if(temp){
        //qDebug() << "__append category key value" << name << " " << key << " " << value ;
        
        temp->value = value;
    }
    else{
        //qDebug() << "---new category key value" << name << " " << key << " " << value ;
        
        FdmSettingItem newitem{key, value};
        settingItems.append(newitem);
        settingDict[key] = &settingItems.last();
    }
}

QList<FdmSettingItem> FdmProfileCategory::getSettings() const
{
    return settingItems;
}

const QList<FdmSettingItem> &FdmProfileCategory::getItems() const
{
    return settingItems;
}


