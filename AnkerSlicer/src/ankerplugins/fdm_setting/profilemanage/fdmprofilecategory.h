#ifndef FDMPROFILECATEGORY_H
#define FDMPROFILECATEGORY_H
#include <QString>
#include <QVariant>
#include <QSettings>

struct FdmSettingItem
{
    QString name;
    QVariant value;
    
    bool fdmSettablePerExtruder;

    
    inline bool operator==(const FdmSettingItem &v){return name == v.name;}
    inline bool operator!=(const FdmSettingItem &v) { return !(*this == v); }
};

class FdmProfileCategory
{
public:
    FdmProfileCategory();
    
    FdmProfileCategory(const QString categoryName);
    
    FdmProfileCategory(const QString categoryName,  const QList<QString>& items,const QSettings &settingMap);
    //FdmProfileCategory(const QString categoryName,  const QList<Q>& items);
    ~FdmProfileCategory();

    
    FdmProfileCategory(const FdmProfileCategory& category);

    
    QVariant get(const QString& key);
    void set(const QString& key,const QVariant& value);
    
    void set(const QList<FdmSettingItem> & items);
    void remove(const QString& key);

    bool contains(const QString& key);

    //category name
    QString name;
    //const QList<FdmSettingItem*> &getSettings();
    QList<FdmSettingItem> getSettings() const;

    const QList<FdmSettingItem> &getItems() const;

    inline bool isEmpty() const
    {
        return settingItems.size() == 0;
    }

    inline bool operator==(const FdmProfileCategory &v){
        return name == v.name;
    }
    inline bool operator!=(const FdmProfileCategory &v) { return !(*this == v); }

    
    void clear();
    //
    void set(const QList<QString> &items, const QSettings &settingMap);
private:
    
    QMap<QString, FdmSettingItem*> settingDict;
    
    QList<FdmSettingItem> settingItems;

    void __append(const QString& key,const QVariant& value);
    //void __remove(const QString& key);
};


#endif // FDMPROFILECATEGORY_H
