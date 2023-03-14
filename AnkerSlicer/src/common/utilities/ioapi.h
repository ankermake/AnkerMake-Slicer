#ifndef IOAPI_H
#define IOAPI_H
#include "akUtilSpace.h"
#include <qstring.h>
#include <QFile>
#include <QFileInfo>

AK_UTIL_NAME_SPACE_BEGIN
class IoApi
{
public:
    
    enum SearchOption
    {
        
        TopDirectoryOnly = 0,
        
        AllDirectories
    };

    IoApi();
    static QFileInfoList getFiles(QString path,QStringList filters,SearchOption option);
    static bool copyDir(QString path,QString destPath, bool coverFileIfExist,bool failedContinue = true);
    static bool write(QString fileName,QString info, QFile::OpenModeFlag flag);
    static bool append(QString fileName, QString info);
    static bool write(QString fileName, QString info);
    static bool touch(QString fileName);
    static QString readAll(QString fileName, QString encode= "UTF-8");
    static QList<QString> getLines(QString fileName, QString encode = "UTF-8");
    
    static void clearFoldersUnderPath(int second,QString absPath, QSet<QString> excludeSet);
    
    static void clearFilesUnderPath(int second,QString absPath, QStringList filter,QSet<QString> excludeSet);
    
    static QDir createTimestampFolderUnderPath(QString path);

    
    static void remove(QString absPath);

    
    //static void setEncodeUtf8();
    //static void setEncodeGb2312();
    //static void setEncode(const char* charSet);
    
    static QString subContent(QString fileName, QString startWords, QString endWords, bool trimStartWords, QString encode = "UTF-8");
    static QList<QByteArray> splite(QByteArray array, int len);
    static QString mergeStr(QString content, QString trim);
    static void getPath(QString path, QString folderName, QString& destPath);
};
AK_UTIL_NAME_SPACE_END

#endif // IOAPI_H
