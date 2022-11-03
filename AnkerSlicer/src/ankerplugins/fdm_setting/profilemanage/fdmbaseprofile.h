#ifndef FDMBASEPROFILE_H
#define FDMBASEPROFILE_H
#include "ifdmprofile.h"
#include <QDateTime>


class FdmBaseProfile: public IFdmProfile
{
    // IFdmProfile interface
public:
    
    //FdmBaseProfile(){};
    
    FdmBaseProfile(const QString& fileFullName, ProfileGroup group);
    //
    FdmBaseProfile(const QString &fileFullName, const QList<FdmProfileCategory> &categories, ProfileGroup group);
    virtual ~FdmBaseProfile();

    
    FdmBaseProfile(const FdmBaseProfile& profile);
    void operator=(const FdmBaseProfile& profile);
    void clone(const FdmBaseProfile& profile);

    
    void load();
    
    void save(bool saveDefault=false);
    void save(QString path);

    
    void clearExcludeAkData();
    
    void clearData();

    
    bool isModified();

    inline bool isInDefaultGroup(){return group == ProfileGroup::Default;}

    
    void remove(const QString &categoryName);
    void clearCategory(const QString& categoryName);
    void remove(const QString &categoryName, const QString &key);

    
    QVariant getSetting(const QString &categoryName, const QString &key) ;
    
    virtual void setSetting(const QString &categoryName, const FdmSettingItem & item);

    
    FdmProfileCategory *getCategory(const QString &categoryName) ;
    void setCategory(const FdmProfileCategory &category) ;

    
    void setCategories(const QList<FdmProfileCategory> &categories) ;
    QList<FdmProfileCategory> *getAllCategories();

    
    virtual void getCategoriesForRefreshTree(QList<FdmProfileCategory> &categories){}

    
    
    //const QString getId() const;
    
    const QString getName() const;
    void setName(const QString &newName);

    
    QString getOriginName() { return originName; }
    void setOriginName(QString name) { originName = name; }

    
    QDateTime getModifyTime() const;
    QDateTime getCreateTime() const;
    void setModifyTimeNow() ;
    void setCreateTimeNow() ;
    void setModifyTime(const QDateTime &time) ;
    void setCreateTime(const QDateTime &time) ;

    
    const QString &getDataSource() const;
    void setDataSource(const QString &newDataSource);
    
    ProfileGroup getGroup() const;
    void setGroup(ProfileGroup newGroup);

    
    bool getVisible() const;
    void setVisible(bool visible);

    
    inline bool operator==(const FdmBaseProfile&v){
        //return getId() == v.getId();
        return getName() == v.getName();
    }
    inline bool operator!=(const FdmBaseProfile&v) { return !(*this == v); }

    

//    virtual void regenMetaData();

//    virtual void initMetaData();

    int getStatus() const;
    void addStatus(EProfileStatus newStatus);
    void setStatus(EProfileStatus newStatus);

    void setMetaData(const QString &key, const QVariant &value);
    QVariant getMetaData(const QString &key) const;



    
    //virtual void commit();

protected:
    void setMetaDataIfNotExist(const QString &key, const QVariant &value);

protected:
    
    ProfileGroup group;
    QString dataSource;
    
    //EProfileStatus status;
    int status;
    
    //bool isContentChanged;
    
    QString originName;

    
    QList<FdmProfileCategory> categories;
    QMap<QString, FdmProfileCategory*> categoryDict;
private:
    void mergeCategory();
    QString mergeCategoryName(QString cateName);
};
#endif // FDMBASEPROFILE_H
