#include "ioapi.h"
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>
#include <QThread>
#include "tlogger.h"
AK_UTIL_NAME_SPACE_BEGIN
IoApi::IoApi()
{

}


bool IoApi::copyDir(QString srcPath,QString destPath, bool coverFileIfExist, bool failedContinue)
{
    QDir srcDir(srcPath);
    QDir dstDir(destPath);
    if (!dstDir.exists())
    {
        dstDir.mkpath(destPath);
    }
    
    QFileInfoList fInfos = srcDir.entryInfoList();
    foreach(QFileInfo fInfo, fInfos)
    {
        if (fInfo.fileName() == "." || fInfo.fileName() == "..")
        {
            continue;
        }
        if (fInfo.isDir())
        {
            auto destPath =  dstDir.filePath(fInfo.fileName());
            //QDir::filePath  Returns the path name of a file in the directory. Does not check if the file actually exists in the directory;
            if (!copyDir(fInfo.filePath(),destPath, coverFileIfExist))
            {
                if (failedContinue)
                {
                    continue;
                }
                return false;
            }
        }
        
        else
        {
            
            if(coverFileIfExist && dstDir.exists(fInfo.fileName()))
            {
                dstDir.remove(fInfo.fileName());
            }
            auto destPath =  dstDir.filePath(fInfo.fileName());
            if (!QFile::copy(fInfo.filePath(), destPath))
            {
                TError("copy " + fInfo.filePath() + " to " + destPath + "failed.");
                if (failedContinue)
                {
                    return true;
                }
                return false;
            }
            else
            {
                TInfo("copy " + fInfo.filePath() + " to " + destPath + "success.");
            }
        }
    }
    return true;
}



void IoApi::getPath(QString path, QString folderName, QString& destPath)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);

    auto dirList = dir.entryInfoList();
    foreach(QFileInfo fInfo, dirList)
    {
        auto dirName = fInfo.fileName();
        if (dirName == folderName)
        {
            destPath = fInfo.filePath();
            return;
        }
        if (fInfo.isDir())
        {
            auto subDir = dir.filePath(fInfo.fileName());
            getPath(subDir, folderName, destPath);
            if (!destPath.isEmpty())
            {
                return;
            }
        }
    }
    if (!destPath.isEmpty())
    {
        return;
    }
    destPath = "";
}


QFileInfoList IoApi::getFiles(QString path,QStringList filters, SearchOption option)
{
    QDir dir(path);
    QFileInfoList fInfos;
    if (option  == SearchOption::TopDirectoryOnly)
    {
        fInfos = dir.entryInfoList(filters, QDir::Files|QDir::Readable,QDir::Name);
    }
    else if  (option  == SearchOption::AllDirectories)
    {
        
        auto fileInfos = dir.entryInfoList(filters, QDir::Files|QDir::Readable,QDir::Name);
        fInfos.append(fileInfos);

        
        auto fInfoList = dir.entryInfoList();
        foreach(QFileInfo fInfo, fInfoList)
        {
            if (fInfo.fileName() == "." || fInfo.fileName() == "..")
            {
                continue;
            }
            if (fInfo.isDir())
            {
                auto subDir =  dir.filePath(fInfo.fileName());
                auto subFileInfoList =  getFiles(subDir,filters, option);
                fInfos.append(subFileInfoList);
            }

//            else
//            {
//                fInfos.append(fInfo);
//            }
        }
    }
    return fInfos;
}


QString IoApi::readAll(QString fileName, QString encode)
{
    QFile file(fileName);
    if (!file.open(QFile::OpenModeFlag::ReadOnly))
    {
        return "";
    }
    QTextStream out(&file);
    out.setCodec(encode.toStdString().c_str());
    QString result = out.readAll();
    file.close();
    return result;
}
QList<QString> IoApi::getLines(QString fileName, QString encode)
{
    QList<QString> lines ;
    QFile file(fileName);
    if (!file.open(QFile::OpenModeFlag::ReadOnly))
    {
        return lines;
    }
    QTextStream out(&file);
    out.setCodec(encode.toStdString().c_str());
    QString line ;
    while(out.readLineInto(&line))
    {
        lines.append(line);
    }
    file.close();
    return lines;
}


QList<QByteArray> IoApi::splite(QByteArray array, int len)
{
    QList<QByteArray> resultList;
    if (len <=0)
    {
        resultList.append(array);
        return resultList;
    }

    QByteArray subArray;
    for (int i = 0; i< array.size();i++)
    {
        subArray.append(array[i]);
        
        if ((i+1)%len == 0 || i == array.size() -1 )
        {
            resultList.append(subArray);
            subArray.clear();
        }
    }

    return resultList;
}

QString IoApi::mergeStr(QString content, QString trim)
{
    auto list =  content.split("\r\n",Qt::SplitBehaviorFlags::SkipEmptyParts);
    for(int i=0;i<list.size();i++)
    {
        auto line = list[i];
        int startIdx = line.indexOf(trim);
        if (startIdx < 0)
        {
            continue;
        }

        line = line.mid(startIdx+trim.length());
        list.append(line);
    }
    return list.join("");
}


QString IoApi::subContent(QString fileName, QString startWords,QString endWords,bool trimStartWords, QString encode)
{
    QStringList lines ;
    QFile file(fileName);
    if (!file.open(QFile::OpenModeFlag::ReadOnly))
    {
        return "";
    }
    QTextStream out(&file);
    out.setCodec(encode.toStdString().c_str());
    QString line ;
    bool startCut =false;
    bool endCut =false;
    int startIdx = -1;
    int endIdx = -1;
    if (startWords == "")
    {
        startIdx = 0;
    }

    while(out.readLineInto(&line))
    {
        if (startWords == "")
        {
            startIdx = 0;
        }
        else
        {
            startIdx = line.indexOf(startWords);
        }
        if (!startCut && startIdx >= 0)
        {
            startCut = true;
            line = line.mid(startIdx);
            if (trimStartWords)
            {
                line = line.mid(startWords.length());
            }
        }

        if (endWords != "")
        {
            endIdx = line.indexOf(endWords);
            if (endIdx >= 0)
            {
                line = line.mid(0, endIdx);
                endCut = true;
            }
        }

        if (startCut)
        {
            lines.append(line);
        }
        if (endCut)
        {
            break;
        }
    }
    file.close();
    if (lines.size()==0)
    {
        return "";
    }
    QString subContent = lines.join("\r\n");
    return subContent;
}

void IoApi::clearFoldersUnderPath(int second, QString absPath, QSet<QString> excludeSet)
{
    AkUtil::TInfo("clear folders under path " + absPath);
    QDir resultDir(absPath);
    if (!resultDir.exists())
    {
        resultDir.mkpath(absPath);
    }

    
    auto fInfoList = resultDir.entryInfoList();
    foreach(QFileInfo fInfo, fInfoList)
    {
        if (fInfo.fileName() == "." || fInfo.fileName() == "..")
        {
            continue;
        }
        if (excludeSet.contains(fInfo.fileName()))
        {
            continue;
        }
        if (fInfo.isDir())
        {
            if (fInfo.lastModified().addSecs(second) < QDateTime::currentDateTime())
            {
                //fInfo.dir().removeRecursively();
                remove(fInfo.filePath());
                AkUtil::TInfo("remove folders " + fInfo.filePath());
            }
        }
    }
}


void IoApi::clearFilesUnderPath(int second, QString absPath, QStringList filter, QSet<QString> excludeSet)
{
    QDir resultDir(absPath);
    if (!resultDir.exists())
    {
        resultDir.mkpath(absPath);
    }

    QFileInfoList fInfos = resultDir.entryInfoList(filter, QDir::Files|QDir::Readable,QDir::Name);
    foreach(auto fInfo, fInfos)
    {
        if (excludeSet.contains(fInfo.fileName()))
        {
            continue;
        }
        
        //if (fInfo.birthTime().addSecs(second) < QDateTime::currentDateTime())
        if (fInfo.lastModified().addSecs(second) < QDateTime::currentDateTime())
        {
            //resultDir.remove(fInfo.fileName());
            remove(fInfo.filePath());
        }
    }

}


QDir IoApi::createTimestampFolderUnderPath(QString path)
{
    QString timeStamp = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    
    QDir stlDir(path);
    QString destStlPath = stlDir.absoluteFilePath(timeStamp);
    QDir destDir(destStlPath);
    if (!destDir.exists())
    {
        destDir.mkpath(destStlPath);
    }
    return destDir;
}


void IoApi::remove(QString absPath)
{
     QFileInfo fi(absPath);
     int count = 20;
     while (count > 0 && fi.exists())
     {
         if (fi.isDir())
         {
             QDir dir(absPath);
             dir.removeRecursively();
             AkUtil::TWarning("delete folder" + absPath + "");
         }
         else
         {
             fi.dir().remove(absPath);
             AkUtil::TWarning("delete file " + absPath + "");
         }
         
         fi= QFileInfo(absPath);
         count--;
         QThread::msleep(50);
     }

     if (count == 0)
     {
         AkUtil::TError("delete " + absPath + "failed. please check the reason");
     }
     //else if (!fi.exists())
     //{
     
     //}
}

//
//void IoApi::setEncodeUtf8()
//{
//    setEncode("UTF-8");
//}
//
//void IoApi::setEncodeGb2312()
//{
//    setEncode("gb2312");
//}
//void IoApi::setEncode(const char* charSet)
//{
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName(charSet));
//}

//void IoApi::setEncode(QString charSet)
//{
//    QByteArray array = charSet.toLatin1();
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName(array.data()));
//}

bool IoApi::append(QString fileName,QString info)
{
    return write(fileName, info, QFile::OpenModeFlag::Append);
}
bool IoApi::write(QString fileName,QString info)
{
    return write(fileName, info, QFile::OpenModeFlag::WriteOnly);
}
bool IoApi::touch(QString fileName)
{
    QFileInfo fInfo(fileName);
    if (fInfo.exists())
    {
        return true;
    }
    return write(fileName, "", QFile::OpenModeFlag::WriteOnly);
}


bool IoApi::write(QString fileName, QString info, QFile::OpenModeFlag flag)
{
    QFile file(fileName);
    if (!file.open(flag))
    {
        return false;
    }
    //qDebug() << qApp << ">>log>>" << info;
    QTextStream out(&file);
    
    out.setCodec("UTF-8");
    out << info;
    file.flush();
    file.close();
    return true;
}

AK_UTIL_NAME_SPACE_END
