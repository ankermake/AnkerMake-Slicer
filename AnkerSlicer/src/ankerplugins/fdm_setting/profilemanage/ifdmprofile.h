#ifndef IFDMPROFILE_H
#define IFDMPROFILE_H
#include <QString>
#include "fdmprofilecategory.h"
enum ProfileGroup
{
    
    Default = 0,
    Custom,
    
    Realtime
};

enum EProfileType
{
    Machine,
    Material,
    Parameter
};


enum EProfileStatus
{
    
    Normal  = 0,
    
    NewFile = 1,
    
    Removed = 2,
    
    NodeValueChanged = 4,
    
    Renamed = 8,
    
    NodeValueChangedManually = 16, //0000 0000 0001 0000
    //ClearNodeValueChangedManually = 0x7FEF,//0111 1111 1110 1111
};

class IFdmProfile {
public:
    virtual void setSetting(const QString& categoryName, const FdmSettingItem& item) = 0;
    virtual const QString getName() const = 0;
    virtual ProfileGroup getGroup() const = 0;
    virtual void setModifyTimeNow() = 0;
    virtual int getStatus() const = 0;
    virtual void addStatus(EProfileStatus newStatus) = 0;
    virtual void setStatus(EProfileStatus newStatus) = 0;
};

//

//class IFdmProfile
//{
//public:

//    virtual void load() = 0;

//    virtual void save(bool saveDefault=false) = 0;
//
////    //rename
////    virtual void rename(const QString &fileFullName)= 0;

////    virtual void clear()= 0;
//
//    virtual bool getVisible() const = 0;
//    virtual void setVisible(bool visible) = 0;
//

//    virtual QVariant getSetting(const QString &categoryName, const QString &key)  = 0;

//    virtual void setSetting(const QString &categoryName, const FdmSettingItem & item)  = 0;
//

//    virtual FdmProfileCategory *getCategory(const QString &categoryName)  = 0;

//    virtual void setCategory(const FdmProfileCategory&) = 0;
//

//    virtual QList<FdmProfileCategory>* getAllCategories()  =0;

//    virtual void setCategories(const QList<FdmProfileCategory>&) = 0;
//

//    virtual void getCategoriesForRefreshTree(QList<FdmProfileCategory> &categories) = 0;
//

//    virtual ProfileGroup getGroup() const = 0;
//    virtual void setGroup(ProfileGroup newGroup) = 0;
//
//    virtual const QString getId() const = 0;
//    //virtual const QString &getId() const = 0;
//    //virtual void setId(const QString &newId) = 0;
//
//    virtual const QString getName() const = 0;
//    virtual void setName(const QString &newName) = 0;
//
//    virtual const QString &getDataSource() const = 0;
//    virtual void setDataSource(const QString &newDataSource) = 0;
//};

#endif // IFDMPROFILE_H
