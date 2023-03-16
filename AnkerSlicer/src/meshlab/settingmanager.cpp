#include "settingmanager.h"

namespace settings {
SettingManager::SettingManager()
{
  //qDebug() << " SETTINGS = " <<  m_settings.fileName();
}

QStringList SettingManager::readRecent()
{
    QVariant  value =  m_settings.value("recentList");
    if(!value.isValid()) {
        return QStringList();
    }
    return value.toStringList();
}

void SettingManager::insertRecent(const QString &fileName)
{
    if(fileName.isEmpty()) {
        return;
    }
    QStringList list = readRecent();
    if(list.contains(fileName)) {
        list.removeOne(fileName);
        list.insert(0,fileName);
        return;
    }
    if(!list.isEmpty() && list.size() >= MAXRECENTCOUNT) {
         list.pop_back();
    }
    list.insert(0,fileName);

    m_settings.setValue("recentList",fileName);
}

void SettingManager::insertRecent(const QStringList &fileList)
{
    if(fileList.isEmpty()) {
        return;
    }
    QStringList list = readRecent();
    for(const auto &file : fileList) {
        if(list.contains(file)) {
            list.removeOne(file);
        }
        list.insert(0,file);
    }
    QStringList maxList;
    int max = MAXRECENTCOUNT > list.size() ? list.size() : MAXRECENTCOUNT;
    for(auto i = 0; i < max; i++) {
        maxList.append(list.at(i));
    }
//    qDebug() << " set value =" << maxList;
    m_settings.setValue("recentList",maxList);
}
void SettingManager::clearRecent()
{
    m_settings.remove("recentList");
}

QString SettingManager::readExportRecent()
{
    if(m_settings.contains("exportRecent"))
    {
        return m_settings.value("exportRecent").toString();
    }
    return QString();
}

void SettingManager::writeExportRecent(const QString &fileName)
{
    m_settings.setValue("exportRecent", fileName);
}


void SettingManager::setAiMode(bool flag)
{
     m_settings.setValue("AiMode",flag);
}

bool SettingManager::getAiMode()
{
    QVariant var = m_settings.value("AiMode");
    if(!var.isValid()) {
        setAiMode(true);
    }
    return  m_settings.value("AiMode").toBool();
}

void SettingManager::setAutomaticallyUpdate(bool flag)
{
    m_settings.setValue("AutomaticallyUpdate",flag);
}

bool SettingManager::getAutomaticallyUpdate()
{
    QVariant var = m_settings.value("AutomaticallyUpdate");
    if(!var.isValid()) {
        setAutomaticallyUpdate(true);
    }
    return  m_settings.value("AutomaticallyUpdate").toBool();
}

void SettingManager::setCurrentLanguage(int index)
{
    m_settings.setValue("CurrentLanguage",index);
}

int SettingManager::getCurrentLanguage()
{
    QVariant var = m_settings.value("CurrentLanguage");
    if(!var.isValid()) {
        setCurrentLanguage(0);
    }
    return  m_settings.value("CurrentLanguage").toInt();
}

void SettingManager::setRecentDownloadNetworkPath(const QString &fileName)
{
    m_settings.setValue("RecentDownloadNetworkPath", fileName);
}

void SettingManager::setRecentDownloadNetworkVersion(const QString &version)
{
    m_settings.setValue("RecentDownloadNetworkVersion", version);
}

QString SettingManager::getRecentDownloadNetworkPath() const
{
    if(m_settings.contains("RecentDownloadNetworkPath"))
        return m_settings.value("RecentDownloadNetworkPath").toString();
    else
        return QString();
}

QString SettingManager::getRecentDownloadNetworkVersion() const
{
    if(m_settings.contains("RecentDownloadNetworkVersion"))
        return m_settings.value("RecentDownloadNetworkVersion").toString();
    else
        return QString();
}

void SettingManager::setLanguageList(const QStringList &languageList)
{
    m_settings.setValue("LanguageList",languageList);
}

QStringList SettingManager::getLanguageList()
{
    setLanguageList(QStringList() << QObject::tr("English")  << QObject::tr("Chinese") << QObject::tr("Japanese"));
    return  m_settings.value("LanguageList").toStringList();
}

void SettingManager::setAcceptUserAgreement(bool flag)
{
    m_settings.setValue("AcceptUserAgreement",flag);
}

bool SettingManager::getAcceptUserAgreement()
{
    QVariant var = m_settings.value("AcceptUserAgreement");
    if(!var.isValid()) {
        setAcceptUserAgreement(false);
    }
    return  m_settings.value("AcceptUserAgreement").toBool();
}

}
