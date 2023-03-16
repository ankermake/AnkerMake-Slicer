#ifndef FDMPROFILEBACKUPSERVICE_H
#define FDMPROFILEBACKUPSERVICE_H
#include <QStringList>
#include <QTimer>

class FdmProfileBackupService
{
public:
    static FdmProfileBackupService* instance()
    {
        static FdmProfileBackupService manager;
        return &manager;
    }

    FdmProfileBackupService();
    
    void backupAll();
    void backupFolder(QString subPath);
    
    void backup(QString file);
    
    bool checkAndRestore();
    
    //bool needRestore();

    
    bool checkFileLost();

    bool backupExist();

private:
    
    bool restore(QString fileFullName);

    
    QString customStorePath = "";
    
    QString customStoreBackupPath =  "";

    QStringList backupPath ;

    QTimer timer;

    bool checkAndRestore(QString profileType);
    //machine material parameter
    QHash<QString,QString> getFileMap(QString profileDir);
    QHash<QString,QString> getNeedRestoreMap(QHash<QString,QString> profileMap,QHash<QString,QString> profileBackupMap);
    QHash<QString,QString> checkProfile(QString profileRootName);
};

#endif // FDMPROFILEBACKUPSERVICE_H
