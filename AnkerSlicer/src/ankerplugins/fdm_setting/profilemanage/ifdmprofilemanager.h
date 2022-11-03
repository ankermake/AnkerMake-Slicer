#ifndef IFDMPROFILEMANAGER_H
#define IFDMPROFILEMANAGER_H
#include <QStringList>
#include "ifdmprofile.h"
#include "fdmprofilecategory.h"
class IFdmProfileManager {

};
//

//class IFdmProfileManager
//{
//public:
//    //virtual void init(QString profileStoragePath) = 0;
//    virtual void init() = 0;

//    virtual QStringList getAllProfileNameList() = 0;
//    virtual QStringList getDefaultProfileNameList()= 0;
//    virtual QStringList getCustomProfileNameList()= 0;

//    virtual void removeProfile(const QString &profileName)= 0;
//

////    virtual void createProfile(const QString &profileName)= 0;

////    virtual void createProfile(const QString &profileName, const QList<FdmProfileCategory> &categories)= 0;
//

//    virtual void renameProfile(const QString &profileName, const QString &newName)= 0;
//

//    //virtual QString getDefaultSelectedName() = 0;
//

//    virtual QString getCurrentSelectedName() = 0;

//    virtual void setCurrentSelectedName(const QString & name) = 0;
//

//    //virtual bool isDefaultProfile(QString profileName) = 0;
//

//    virtual bool exist(QString name) const = 0;
//

//    virtual void backup()= 0;

//    virtual void rollback()= 0;

//    virtual void commit()= 0;

//    virtual QString getValidName(const QString profileName) = 0;

//    virtual QString getAbsPath(const QString profileName) = 0;
//

////    virtual IFdmProfile* getProfile(const QString &profileName)= 0;
////    virtual IFdmProfile* getCurrentSelectedProfile();
//};

#endif // IFDMPROFILEMANAGER_H
