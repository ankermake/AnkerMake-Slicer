#ifndef FDMPROFILEMANAGER_H
#define FDMPROFILEMANAGER_H

#include "fdmmaterialprofile.h"
#include <QCoreApplication>
#include <QDir>
#include "common/utilities/ioapi.h"
#include "ifdmprofilemanager.h"
#include <QDebug>
#include <QRegularExpression>
using namespace AkUtil;
#include <QUuid>
#include "filemanage/fdmqmlsourcetree.h"
#include "common/ak_const.h"
#include "operatelog.h"
#include <QSet>
#include <QMap>
#include <QStandardPaths>


struct profileSortItem
{
    QString name;
    QDateTime timestamp;
};


template <typename TProfile>
class FdmProfileManager : public IFdmProfileManager
{
public:
    
    inline TProfile* getProfileByName(const QString &profileName)
    {
        if (profileNameDict.contains(profileName))
        {
            return profileNameDict[profileName];
        }
        return NULL;
    }

    //inline TProfile* getProfileById(const QString& fileId)
    //{
    //    if (profileIdDict.contains(fileId))
    //    {
    //        return profileIdDict[fileId];
    //    }
    //    return NULL;
    //}
    // 
    
    //inline virtual TProfile* getCurrentSelectedProfile()
    //{
    //    QString currentName = getCurrentSelectedName();
    //    qDebug() << "getCurrentSelectedProfile currentName is" << currentName;
    //    return getProfileByName(currentName);
    //}

    // IFdmProfileManager interface
public:
    FdmProfileManager(const QString &profileStoragePath, const QString& customStoragePath)
    {
        profileStorePath = profileStoragePath;
        customStorePath = customStoragePath;
        //currentSelectedName = "";
        //currentSelectedId = "";
        backupData = NULL;
    }
    FdmProfileManager()
    {
        profileStorePath = "";
        customStorePath = "";
        //currentSelectedName = "";
        //currentSelectedId = "";
        backupData = NULL;
    }

    virtual ~FdmProfileManager()
    {
        if (NULL != backupData)
        {
            delete backupData;
            backupData = NULL;
        }
    }

    
    virtual void init()
    {
        //this->profileStorePath = profileStoragePath;
        
        QString defaultProfilePath = getDefaultProfilePath();
        QString customProfilePath = getCustomProfilePath();
        //QString realtimeProfilePath = getRealtimeProfilePath();
        loadProfiles(defaultProfilePath, defaultProfiles, ProfileGroup::Default);
        loadProfiles(customProfilePath, customProfiles, ProfileGroup::Custom);
        //loadProfiles(realtimeProfilePath, customProfiles, ProfileGroup::Custom);

        
        backupData = nullptr;
    }

    
    void loadProfiles(QString profilePath,  QList<TProfile>& profiles, ProfileGroup group)
    {
        profiles.clear();
        
        QDir dir(profilePath);
        if (!dir.exists())
        {
            dir.mkpath(dir.absolutePath());
        }

        QStringList filters;
        filters<<"*.ini";
        QFileInfoList fInfos = dir.entryInfoList(filters, QDir::Files|QDir::Readable,QDir::Name);
        for(int i = 0; i<fInfos.size();i++)
        {
            QFileInfo& fInfo = fInfos[i];
            TProfile profile(fInfo.absoluteFilePath(), group);
            
            profile.setStatus(EProfileStatus::Normal);
            
            profiles.append(profile);
            
            profileNameDict[profile.getName()] = &profiles.last();
            //profileIdDict[profile.getId()] = &profiles.last();
            //qDebug() << "loadProfiles profileIdDict[]" << profile.getId() << profile.getName();
        }
    }

    QString getDefaultProfilePath(){return QDir(profileStorePath).absoluteFilePath("default"); }
    
    QString getCustomProfilePath(){return QDir(customStorePath).absoluteFilePath("custom");  }

    
    virtual QStringList getAllProfileNameList()
    {
        QList<profileSortItem> sortItems;
        convertToSortItems(defaultProfiles,sortItems);
        convertToSortItems(customProfiles,sortItems);
        sortByTimestamp(sortItems);
        return getNameList(sortItems);
    }

    void convertToSortItems(QList<TProfile> & profiles,QList<profileSortItem>& sortItems)
    {
        profileSortItem item;
        foreach(const TProfile &profile, profiles)
        {
            
            if (!profile.getVisible())
            {
                continue;
            }
            item.name = profile.getName();
            
            //item.timestamp = profile.getCreateTime();
            item.timestamp = profile.getModifyTime();
            sortItems.append(item);
        }
    }
    virtual QStringList getDefaultProfileNameList()
    {
        QList<profileSortItem> sortItems;
        convertToSortItems(defaultProfiles,sortItems);
        sortByTimestamp(sortItems);
        return getNameList(sortItems);
    }
    QStringList getCustomProfileNameList()
    {
        QList<profileSortItem> sortItems;
        convertToSortItems(customProfiles,sortItems);
        sortByTimestamp(sortItems);
        return getNameList(sortItems);
    }

    QString createNewDataSourceName()
    {
        return QUuid::createUuid().toString();
    }
    
    QString removeProfile(const QString& profileName) {
        
        auto sortedNameList = getCustomProfileNameList();
        
        QString candidateName = "";
        int profileIdx = sortedNameList.indexOf(profileName);
        if (profileIdx >= 0 && profileIdx < sortedNameList.size() - 1)
        {
            candidateName = sortedNameList[profileIdx+1];
        }
        else
        {
            QStringList nameList = getDefaultProfileNameList();
            if (nameList.size() > 0)
            {
                candidateName = nameList.last();
            }
        }

        
        QString newName = QString("removed_%1_%2").arg(profileName).arg(createNewDataSourceName());
        renameProfile(profileName, newName);
        TProfile* profile = getProfileByName(newName);
        if (NULL == profile)
        {
            qDebug() << "not exist : " << newName;
            return candidateName;
        }
        profile->setStatus(EProfileStatus::Removed);
        profile->setVisible(false);

        
        //setCurrentSelectedName(candidateName);
        return candidateName;
    }

    
    //QString removeProfile(const QString &profileName){
    
    //    auto sortedNameList = getCustomProfileNameList();
    
    //    bool currentNameSetted = false;
    //    QString candidateName = "";
    //    for (int i = 0; i < sortedNameList.size(); i++)
    //    {
    //        if (i < sortedNameList.size() - 1)
    //        {
    //            if (profileName == sortedNameList[i + 1])
    //            {
    //                candidateName = sortedNameList[i];
    //                currentNameSetted = true;
    //                break;
    //            }
    //        }
    //    }

    
    //    if (!currentNameSetted)
    //    {
    //        QStringList nameList = getDefaultProfileNameList();
    //        if (nameList.size() > 0)
    //        {
    //            candidateName = nameList.last();
    //        }
    //    }

    
    //    QString newName = QString("removed_%1_%2").arg(profileName).arg(createNewDataSourceName());
    //    renameProfile(profileName, newName);
    //    TProfile * profile = getProfileByName(newName);
    //    if (NULL == profile )
    //    {
    //        qDebug() << "not exist : " << newName;
    //        return candidateName;
    //    }
    //    profile->setStatus(EProfileStatus::Removed);
    //    profile->setVisible(false);

    
    //    //setCurrentSelectedName(candidateName);
    //    return candidateName;
    //}

    QString getValidCustomProfilePath(QString profileName)
    {
        QString validName = getValidName(profileName);
        QDir dir(getCustomProfilePath());
        return dir.absoluteFilePath(validName);
    }

    TProfile* createProfile(const QString &profileName, TProfile* templateProfile)
    {
        qDebug() << "createMaterial : " << profileName;
        
        QString validName = getValidName(profileName);
        qDebug() << "validName : " << validName;

        
        //auto templateProfile = getCurrentSelectedProfile();
        if (NULL == templateProfile)
        {
            return NULL;
        }

        
        TProfile newProfile(*templateProfile);
        //newProfile.regenMetaData();
        newProfile.setGroup(ProfileGroup::Custom);
        newProfile.setName(validName);
        newProfile.setOriginName(validName);
        newProfile.setVisible(true);
        newProfile.addStatus(EProfileStatus::NewFile);
        newProfile.setCreateTime(QDateTime::currentDateTime());
        newProfile.setModifyTime(QDateTime::currentDateTime());

        
        QString fileName = QUuid::createUuid().toString();
        auto validFullName = QDir(getCustomProfilePath()).absoluteFilePath(fileName) + ".ini";

        qDebug() << "validFullName : " << validFullName;
        newProfile.setDataSource(validFullName);


//        auto absPath = getAbsPath(validName);
//        newProfile.setDataSource(absPath);

        
        auto result = this->addCustom(newProfile);
        //setCurrentSelectedName(validName);
        return result;

        
    }

    
    TProfile*  createProfile(const QString &profileName, const QList<FdmProfileCategory> &categories){
        
        //QString validName = getValidName(profileName);
        
        //auto fileFullName = getAbsPath(validName);
        //newProfile.setDataSource(absPath);

        
        TProfile newProfile("", categories, ProfileGroup::Custom);
        QString fileName = QUuid::createUuid().toString();
        auto validFullName = QDir(getCustomProfilePath()).absoluteFilePath(fileName) + ".ini";
        //newProfile.regenMetaData();
        newProfile.setDataSource(validFullName);
        newProfile.setName(profileName);
        newProfile.setOriginName(profileName);
        newProfile.addStatus(EProfileStatus::NewFile);
        newProfile.setCreateTime(QDateTime::currentDateTime());
        newProfile.setModifyTime(QDateTime::currentDateTime());
        
        newProfile.setVisible(true);

        
        auto result = this->addCustom(newProfile);
        //setCurrentSelectedName(profileName);
        return result;
    }

    void renameProfile(const QString &profileName, const QString &newName){
        auto profile = getProfileByName(profileName);
        if (NULL == profile)
        {
            return;
        }

        qDebug() << "getProfileByName()"<<profileName<<" profile is " << profile;
        profile->setName(newName);
        profile->addStatus(EProfileStatus::Renamed);

        
        profileNameDict.remove(profileName);
        profileNameDict[newName] = profile;
    }
    //virtual QString getDefaultSelectedName(){return NULL;}
    
//    QString getDefaultSelectedName(){
//        auto materialId = FdmParameterProfileManager::Instance().getCurrentSelectedProfile()->getMaterialId();
//        auto profile = getProfileById(materialId);
//        if (NULL == profile)
//        {
//            return NULL;
//        }

//        return profile->getName();
//    }

   /* virtual inline QString getCurrentSelectedName()
    {
        return currentSelectedName;
    }

    virtual void setCurrentSelectedName(const QString & name){
        currentSelectedName = name;
    }*/

    //const QString &getCurrentSelectedId() const
    //{
    //    return currentSelectedId;
    //}

    //void setCurrentSelectedId(const QString &newCurrentSelectedId)
    //{
    //    currentSelectedId = newCurrentSelectedId;
    //    auto profile = getProfileById(currentSelectedId);
    //    if (profile != NULL)
    //    {
    //        currentSelectedName = profile->getName();
    //    }
    //    qDebug() << "setCurrentSelectedId " << currentSelectedId << currentSelectedName;
    //}

    inline bool exist(QString name) const
    {
        return profileNameDict.contains(name);
    }
    virtual void backup(){
        if (NULL == this->backupData)
        {
            this->backupData = new FdmProfileManager();
        }
        copy(this, this->backupData);
        releaseBackupData(this->backupData);
    }
    virtual void rollback()
    {
        if (NULL == this->backupData)
        {
            //qDebug() <<
            return;
        }
        copy(this->backupData, this);
        releaseBackupData(this);
    }
    
    QMap<QString, QString> getRenamedMap()
    {
        QMap<QString, QString> resultMap;
        for (int i = 0; i < customProfiles.size(); i++)
        {
            TProfile& profile = customProfiles[i];
            
            if (!(profile.getStatus() & EProfileStatus::Renamed)
                || profile.getOriginName().isEmpty())
            {
                continue;
            }
            if (profile.getOriginName() != profile.getName())
            {
                resultMap[profile.getOriginName()] = profile.getName();
            }
        }
        return resultMap;
    }

    
    QSet<QString> getRemovedSet()
    {
        QSet<QString> resultSet;
        for (int i = 0; i < customProfiles.size(); i++)
        {
            TProfile& profile = customProfiles[i];
            
            if (!(profile.getStatus() & EProfileStatus::Removed)
                || profile.getOriginName().isEmpty())
            {
                continue;
            }
            resultSet.insert(profile.getOriginName());
        }
        return resultSet;
    }
    
    QSet<QString> getValueChangeSet()
    {
        QSet<QString> resultSet;
        for (int i = 0; i < customProfiles.size(); i++)
        {
            TProfile& profile = customProfiles[i];
            
            if (!(profile.getStatus() & EProfileStatus::NodeValueChanged)
                || profile.getOriginName().isEmpty())
            {
                continue;
            }
            resultSet.insert(profile.getOriginName());
        }
        return resultSet;
    }

    AkOperateValueChangeDetailPtr getValueChangeDetail(QString profieName)
    {
        AkOperateValueChangeDetailPtr changeDetail = AkOperateValueChangeDetailPtr(new AkOperateValueChangeDetail);
        
        if (nullptr == backupData || !backupData->profileNameDict.contains(profieName))
        {
            return changeDetail;
        }
        auto currentProfile = getProfileByName(profieName);
        if (nullptr == currentProfile)
        {
            return changeDetail;
        }
        
        auto originProfile = backupData->profileNameDict[profieName];
        

    }

    virtual void commit(){
        
        
        QList<TProfile*> removeList;
        for(int i=0;i<customProfiles.size();i++)
        {
            TProfile& profile = customProfiles[i];
            
            if (profile.getStatus() == EProfileStatus::Removed)
            {
                
                profileNameDict.remove(profile.getName());
                //profileIdDict.remove(profile.getId());

                
                QString dataSource = profile.getDataSource();
                QFileInfo fInfo(dataSource);
                //fInfo.absoluteDir().remove(fInfo.fileName());

                //fInfo.absoluteDir().remove(fInfo.filePath());
                IoApi::remove(fInfo.filePath());

                
                removeList.append(&profile);
            }
            else
            {
                //QFileInfo profileInfo(profile.getDataSource());
                profile.setOriginName(profile.getName());
                profile.addStatus(EProfileStatus::Normal);
                
                IoApi::write(profile.getDataSource(), "", QFile::OpenModeFlag::WriteOnly);
                
                profile.save();
            }
        }
        
        for (size_t i = 0; i < removeList.size(); i++)
        {
            customProfiles.removeOne(*removeList[i]);
        }

        
        if(NULL != backupData)
        {
            delete backupData;
        }
        backupData = NULL;
    }
    //QString getRecommandName()
    //{
    //    return getValidName(currentSelectedName);
    //}

    
    inline QString getValidName(const QString profileName)
    {
        qDebug() << "getValidName " << profileName << "profileNameDict.contains" << profileNameDict.contains(profileName) << " profileNameDict.size is " << profileNameDict.size();
        QString validName = "newName";
        if (!profileName.isEmpty())
        {
            validName = profileName.trimmed();
            
            QRegExp rx("\\s+");
            validName = validName.replace(rx,"");
        }
        
        if (!profileNameDict.contains(profileName))
        {
            return profileName;
        }

        QRegularExpression rg("^#\\d+_new_");
        validName.replace(rg,"");
        
        for(int i =0 ; i< 2000; i++)
        {
            QString tryName = QString("#%1_new_%2").arg(i).arg(validName);
            if (!profileNameDict.contains(tryName))
            {
                return tryName;
            }
        }

        return QString("##%1").arg(validName);
    }

    inline QString getAbsPath(const QString profileName)
    {
         return QDir(profileStorePath).absoluteFilePath(profileName);
    }
    const QString &getProfileStorePath() const {return profileStorePath;}
    void setProfileStorePath(const QString &newProfileStorePath){profileStorePath = newProfileStorePath;}

//    const QString &getCurrentSelectedId() const;
//    void setCurrentSelectedId(const QString &newCurrentSelectedId);

protected:
    QStringList getNameList(const  QList<profileSortItem>& sortItems)
    {
        QStringList listPrinting;
        QStringList list;
        for(int i =0;i<sortItems.size();i++)
        {
            list.append(sortItems[i].name);
            listPrinting.append(sortItems[i].name);
            listPrinting.append(sortItems[i].timestamp.toString());
        }
        //qDebug()<< "profiles.count is " <<  sortItems.size();
        //qDebug()<< "getNameList" << listPrinting;
        

        return list;
    }
    
    void sortByTimestamp(QList<profileSortItem>& sortItems)
    {
        std::sort(sortItems.begin(), sortItems.end(),
                  [&](auto &lv, auto &rv)->bool {return lv.timestamp > rv.timestamp;});
    }

    
    TProfile* addCustom(const TProfile& profile){
        
        customProfiles.append(profile);
        TProfile & lastOne =customProfiles.last();
        profileNameDict[lastOne.getName()] = &lastOne;
        //profileIdDict[lastOne.getId()] = &lastOne;
        return &lastOne;
    }
    TProfile* addDefault(const TProfile& profile){
        
        defaultProfiles.append(profile);
        TProfile & lastOne =defaultProfiles.last();
        profileNameDict[lastOne.getName()] = &lastOne;
        //profileIdDict[lastOne.getId()] = &lastOne;
        return &lastOne;
    }

    //void clone(FdmMaterialProfile& profile);
    
    virtual void copy(FdmProfileManager * from, FdmProfileManager * to){
        to->defaultProfiles.clear();
        to->customProfiles.clear();
        to->profileNameDict.clear();
        //to->profileIdDict.clear();

        
        //to->currentSelectedName = from->currentSelectedName;
        //to->currentSelectedId = from->currentSelectedId;
        to->profileStorePath = from->profileStorePath;
        to->customStorePath = from->customStorePath;

        
        to->defaultProfiles.append(from->defaultProfiles);
        to->customProfiles.append(from->customProfiles);

        auto initDict = [&](QList<TProfile>& profiles)->void {
            for(int i =0;i<profiles.size();i++)
            {
                TProfile& profile =profiles[i];
                to->profileNameDict[profile.getName()] = &profile;
                //to->profileIdDict[profile.getId()] = &profile;
            }
        };

        initDict(to->defaultProfiles);
        initDict(to->customProfiles);
    }
    void releaseBackupData(FdmProfileManager* to)
    {
        
        if (NULL != to->backupData)
        {
            delete to->backupData;
        }
        to->backupData = NULL;
    }
protected:
    
    //QString currentSelectedName;
    
    //QString currentSelectedId;

    QString profileStorePath;
    QString customStorePath;

    
    QList<TProfile> defaultProfiles;
    QList<TProfile> customProfiles;

    
    QMap<QString, TProfile*> profileNameDict;
    //QMap<QString, TProfile*> profileIdDict;


    
    FdmProfileManager<TProfile>* backupData;
};
#endif // FDMPROFILEMANAGER_H
