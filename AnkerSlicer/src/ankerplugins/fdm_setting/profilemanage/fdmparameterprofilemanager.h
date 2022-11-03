#ifndef FDMPARAMETERPROFILEMANAGER_H
#define FDMPARAMETERPROFILEMANAGER_H

#include "fdmprofilemanager.h"
#include "fdmparameterprofile.h"
#include "fdmrealtimeprofile.h"


class FdmParameterProfileManager : public FdmProfileManager<FdmParameterProfile>
{
public:
    
    static FdmParameterProfileManager& Instance()
    {
        static QString profileStorePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("setting/fdm/parameter");
        static QString customStorePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm/parameter");
        static FdmParameterProfileManager manager(profileStorePath, customStorePath);
        return manager;
    }

    QString getRealtimeProfilePath() { return QDir(customStorePath).absoluteFilePath("realtime"); }
    QString getBaseProfilePath() { return QDir(customStorePath).absoluteFilePath("base"); }

    FdmParameterProfileManager();

    FdmParameterProfileManager(const QString& storePath, const QString& customStorePath);

    ~FdmParameterProfileManager();

    void init() override;

    
    //QString getCurrentMachineName();
    //void setCurrentMachineName(const QString &newCurrentMachineName);
    
    //QString getCurrentMaterialName();
    //void setCurrentMaterialName(const QString &newCurrentMaterialName);

    
    //void setCurrentParameterProfile(const FdmParameterProfile* profile);

    //QString getDefaultSelectedName();

    
    //void exportCurrentProfile(QString destFileFullName);
    
    //void refreshCurrentProfile(const QList<FdmProfileCategory> & categories);


    
    void readProfile(QString profile, QList<FdmProfileCategory>& categories);

    FdmParameterProfile* getExpertProfile(QString machineName, QString materialName);

    
    //FdmParameterProfile *getCurrentSelectedProfile() override;

    
    FdmParameterProfile *newDefaultProfile(QString machineName, QString materialName);

    QStringList getDefaultProfileNameList() override;

    

    //QString getCurrentSelectedName() override;
    //void setCurrentSelectedName(const QString & name) override;

    //
    //void setExpertMachine();
    QStringList getAllProfileNameList() override;

    FdmRealTimeProfile*getRightRealTimeProfile();
    //void setRightRealTimeProfile(FdmParameterProfile *newRightRealTimeProfile);

    
    //void persistRealtimeProfileModification();

    
    //void backup() override;
    //void rollback() override;
    //void commit() override;
protected:
    //void copy(FdmProfileManager * from, FdmProfileManager * to) override;

private:
    void createBaseProfile();
    void createRealTimeProfile();

    
    FdmRealTimeProfile* rightRealTimeProfile;

    
    FdmParameterProfile *baseProfile;

    
//    QString currentMachineName;
//    QString currentMaterialName;

    
    
    

    //QString currentMachineId;
    //QString currentMaterialId;

    
    //QString currentProfileId;
};


#endif // FDMPARAMETERPROFILEMANAGER_H
