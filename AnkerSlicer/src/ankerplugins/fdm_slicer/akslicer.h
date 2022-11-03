#ifndef AKSLICER_H
#define AKSLICER_H
#include <QObject>
#include <QProcess>
#include "aksliceinfo.h"
#include <QSet>
#include <QDir>

#ifdef DEBUG_BY_CL
    #include "enginelib.h"
#endif


class AkSlicer : public QObject
{
    Q_OBJECT

public:
    enum SliceStatus
    {
        Doing = 0,
        Error,
        Success
    };

    enum SliceStep
    {
        FileLoad = 0,
        Slice
    };

public:
    AkSlicer();
    ~AkSlicer();
    
    //void sliceBatch(QString stlStore);
    
    void slice();
    void slice(QString stlFile,QString destFile, QString userSetting="");
    
    void cancel();

    //get,set
    void resetParam();
    //clear gcode created before x sencod.default clear all
    void clearGcode(QSet<QString> excludeSet, int second = -1);
    void clearStl(QSet<QString> excludeSet, int second = -1);

    const QString &getStlFile() const;
    void setStlFile(const QString &newStlFile);
    void setOriginalStlName(const QString &newOriginalStlName);
    const QString &getUserSetting() const;
    void setUserSetting(const QString &newUserSetting);

    const QString getStlPath() const;

    const QStringList &getSupportMeshes() const;
    void setSupportMeshes(const QStringList &newSupportMeshes);

signals:
    
    void progress(SliceStep step, SliceStatus status, float percent, QString msg);
    
    void sliceSuccess(AkSliceInfo sliceInfo);
    
    void sliceFailed(AkSliceInfo sliceInfo);
private:
    void runCmd(QString cmd);
    void newStdErrGenerated();
    void newStdErrGeneratedChars(QByteArray);
    QString getConfig(QString configPath);

private:
#ifdef DEBUG_BY_CL
    EngineLibThread engineLib;
#endif
    QProcess *pInvoker;
    QString enginePath;
    AkSliceInfo * sliceInfo;

    
    QString originalStlName;
    QString stlFile;
    QStringList supportMeshes;
    //QString destFile;
    QString userSetting;

    QDir appDataLocDir;

    void clearFile(int second, QString folder, QStringList filter,QSet<QString> excludeSet);
    //void clearFolder(int second, QString folder, QStringList filter, QSet<QString> excludeSet);
    void clearFolder(int second, QString folder, QSet<QString> excludeSet);
};

#endif // AKSLICER_H
