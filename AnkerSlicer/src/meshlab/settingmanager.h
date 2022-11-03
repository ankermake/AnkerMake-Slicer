#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QSettings>
#include <QDebug>
namespace settings {

#define MAXRECENTCOUNT 5
class SettingManager
{
public:
    SettingManager();

    QStringList readRecent();
    void insertRecent(const QString &fileName);
    void insertRecent(const QStringList &fileList);
    void clearRecent();

    void setAiMode(bool flag);
    bool getAiMode();

    void setAutomaticallyUpdate(bool flag);
    bool getAutomaticallyUpdate();

    void setCurrentLanguage(int index);
    int getCurrentLanguage();

    void setRecentDownloadNetworkPath(const QString &fileName);
    void setRecentDownloadNetworkVersion(const QString &version);
    QString getRecentDownloadNetworkPath() const;
    QString getRecentDownloadNetworkVersion() const;

    void setLanguageList(const QStringList &languageList);
    QStringList getLanguageList();

    void setAcceptUserAgreement(bool flag);
    bool getAcceptUserAgreement();

private:
    QSettings m_settings;
};
}
#endif // SETTINGMANAGER_H
