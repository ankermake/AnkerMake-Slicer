#include "fdmprofilebackupservice.h"
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include "../common/utilities/ioapi.h"
#include "../common/utilities/tlogger.h"
#include <QTimer>
#include "profilemanage/fdmparameterprofilemanager.h"

FdmProfileBackupService::FdmProfileBackupService()
{
    
    customStorePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm");
    
    customStoreBackupPath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm_backup");
    
    backupPath.clear();
    backupPath.append("machine");
    backupPath.append("material");
    backupPath.append("parameter");

}


void FdmProfileBackupService::backupAll()
{
    for (size_t i = 0; i < backupPath.size(); i++)
    {
        backupFolder(backupPath[i]);
    }
}
void FdmProfileBackupService::backupFolder(QString subPath)
{
    AkUtil::TFunction(" backup folder " + subPath);
    QDir backupDir(customStoreBackupPath);
    QDir storeDir(customStorePath);

    QString backupSubPath = backupDir.absoluteFilePath(subPath);
    QString storeSubPath = storeDir.absoluteFilePath(subPath);
    QDir backupSubDir(backupSubPath);

    AkUtil::IoApi::remove(backupSubPath);
    AkUtil::IoApi::copyDir(storeSubPath, backupSubPath, true, true);

    
    AkUtil::TDebug("backup profile: change the ini to txt");
    QFileInfoList infoList = AkUtil::IoApi::getFiles(backupSubPath, QStringList(), AkUtil::IoApi::SearchOption::AllDirectories);

    for (int i = 0; i < infoList.size(); i++)
    {
        QFileInfo& fi = infoList[i];
        AkUtil::TDebug("backup profile: rename " + fi.absoluteFilePath() + " append suffix .txt");
        bool suc = QFile::rename(fi.absoluteFilePath(), fi.absoluteFilePath() + ".txt");
        if (!suc)
        {
            AkUtil::TError("backup profile: rename failed. " + fi.absoluteFilePath() + " append suffix .txt failed.");
        }
    }

    //if (backupSubDir.exists())
    //{
    //   /* QString removedName = backupSubPath + "_to_be_removed";
    
    //    bool suc = backupSubDir.rename(backupSubPath, removedName);
    //    if (!suc)
    //    {
    //        AkUtil::TDebug("rename " + backupSubPath + "to " + removedName + "failed. remove it directly");
    //        AkUtil::IoApi::remove(backupSubPath);
    //    }
    //    else
    //    {
    //        AkUtil::IoApi::copyDir(storeSubPath, backupSubPath, true, true);
    //        AkUtil::IoApi::remove(removedName);
    //    }
    //    */
    //}
    //else
    //{
    //    AkUtil::IoApi::copyDir(storeSubPath, backupSubPath, true, true);
    //}
}


void FdmProfileBackupService::backup(QString file)
{
    AkUtil::TFunction("");
    QFileInfo fi(file);
    if (!fi.exists())
    {
        AkUtil::TWarning("backup profile: file not exist " + file);
        return;
    }
    
    QDir fmdDir = fi.dir();
    bool cdSuccess = true;
    QString relatedFileName = fi.fileName();
    while(cdSuccess && fmdDir.dirName()!="fdm")
    {
        relatedFileName = fmdDir.dirName() + "/" + relatedFileName;
        cdSuccess = fmdDir.cdUp();
    }
    
    if (!cdSuccess)
    {
        return;
    }

    
    QString destFile = QDir(customStoreBackupPath).absoluteFilePath(relatedFileName+".txt");
    
    QFile::remove(destFile);
    
    bool suc = QFile::copy(file, destFile);
    if (!suc)
    {
        AkUtil::TWarning("backup failed: copy file from " + file + " to " + destFile + " failed. make sure destFile is not exist before copy perfermance");
        return;
    }
    AkUtil::TDebug("backup success: copy file from " + file + " to " + destFile + " success.");
}
bool FdmProfileBackupService::restore(QString fileFullName)
{
    AkUtil::TFunction("");
    QString destFile = fileFullName;
    
    destFile = destFile.replace("fdm_backup","fdm").replace(".txt","");
    
    QFileInfo destFi(destFile);
    if (!destFi.absoluteDir().exists())
    {
        AkUtil::TWarning("restore make path: path " + destFi.absolutePath() + " not exist . make path ");
        destFi.dir().mkpath(destFi.absolutePath());
    }

    
    bool suc = QFile::copy(fileFullName, destFile);
    if (!suc)
    {
       
       AkUtil::TWarning("restore failed: copy file from " + fileFullName + " to " + destFile + " failed. make sure destFile is not exist before copy perfermance");
       return false;
    }
    AkUtil::TDebug("restore success: copy file from " + fileFullName + " to " + destFile + " success");
    return true;
}


bool FdmProfileBackupService::checkAndRestore()
{
    AkUtil::TFunction("");
    for (size_t i = 0; i < backupPath.size(); i++)
    {
        checkAndRestore(backupPath[i]);
    }
    return true;
}

QHash<QString,QString> FdmProfileBackupService::checkProfile(QString profileRootName)
{
    QString profileDir = QDir(customStorePath).absoluteFilePath(profileRootName);
    QString backProfileDir = QDir(customStoreBackupPath).absoluteFilePath(profileRootName);
    auto profileMap = getFileMap(profileDir);
    auto profileBackupMap = getFileMap(backProfileDir);
    return getNeedRestoreMap(profileMap, profileBackupMap);
}

//
bool FdmProfileBackupService::checkAndRestore(QString profileRootName)
{
    auto profileNeedStoreMap = checkProfile(profileRootName);
    if (profileNeedStoreMap.size()>0)
    {
        AkUtil::TWarning("need restore profile file ");
        auto iter = profileNeedStoreMap.begin();
        while (iter != profileNeedStoreMap.end())
        {
            QString backFilePath = iter.value();
            restore(backFilePath);
            iter++;
        }
    }
    return true;
}


QHash<QString,QString> FdmProfileBackupService::getFileMap(QString profileDir)
{
    QHash<QString,QString> hashMap;
    QFileInfoList infoList = AkUtil::IoApi::getFiles(profileDir,QStringList(), AkUtil::IoApi::SearchOption::AllDirectories);
    for(int i = 0; i< infoList.size(); i++)
    {
        QFileInfo& fi = infoList[i];
        QString suffix = fi.suffix();
        QString baseName = fi.completeBaseName();
        if (suffix == "txt")
        {
            baseName = QFileInfo(baseName).completeBaseName();
        }
        if (!hashMap.contains(baseName))
        {
            hashMap[baseName] =  fi.absoluteFilePath();
        }
    }
    return hashMap;
}


QHash<QString,QString> FdmProfileBackupService::getNeedRestoreMap(QHash<QString,QString> profileMap,QHash<QString,QString> profileBackupMap)
{
    QHash<QString,QString> resultMap;
    auto iter = profileBackupMap.begin();
    while (iter != profileBackupMap.end())
    {
        QString fileName = iter.key();
        if (!profileMap.contains(fileName))
        {
            resultMap[fileName] = iter.value();
        }
        iter++;
    }
    return resultMap;
}


bool FdmProfileBackupService::checkFileLost()
{
    
    auto realTimeProfile = FdmParameterProfileManager::Instance().getRightRealTimeProfile();
    int checkInterval =  5;
    checkInterval =  realTimeProfile->getSetting("",AkConst::SettingKey::CHECK_PARAM_LOST_INTERVAL).toInt();
    if (checkInterval<=0)
    {
        AkUtil::TDebug("don't have to check parameter lost" );
        return false;
    }

    AkUtil::TDebug("check parameter every " + QString::number(checkInterval) + "seconds" );

    
    auto checkProfileStatus = [&](QString profileRootName){
        auto profileNeedStoreMap = FdmProfileBackupService::instance()->checkProfile(profileRootName);
        if (profileNeedStoreMap.size()>0)
        {
            AkUtil::TWarning("Fatal: profile file lost: ");
            auto iter = profileNeedStoreMap.begin();
            while (iter != profileNeedStoreMap.end())
            {
                QString backFilePath = iter.value();
                
                AkUtil::TWarning("Fatal: profile " + backFilePath + " is in backup folder but not exist in parameter path. please check what heppened.");
                //restore(backFilePath);
                iter++;
            }
        }
        //else
        //{
        //    AkUtil::TWarning("profile file ok.");
        //}
    };

    
    QObject::connect(&timer,&QTimer::timeout, [&]()->void {
        for (size_t i = 0; i < backupPath.size(); i++)
        {
            checkProfileStatus(backupPath[i]);
        }
    });
    timer.start(checkInterval*1000);

    return true;
}

bool FdmProfileBackupService::backupExist()
{
    return QDir(customStoreBackupPath).exists();
}
