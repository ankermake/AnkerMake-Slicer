#include "akslicer.h"
//#include "tlogger.h"
#include <QDir>
#include "aksliceinfo.h"
#include <QRegExp>
#include <QCoreApplication>
#include <QMap>
#include "common/utilities/ioapi.h"
#include <QRegularExpression>
#include <QDateTime>
#include <QMessageBox>
#include "common/utilities/tlogger.h"
#include <QStandardPaths>

using namespace AkUtil;

AkSlicer::AkSlicer()
{
    //this->sliceEnginePath = enginePath;
    resetParam();
    //enginePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("SliceEngine");

    auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    
    
    appDataLocDir = QDir(writableLocation);

    
    enginePath = appDataLocDir.absoluteFilePath("SliceEngine");

    
    
    auto oriEnginePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("SliceEngine");
    IoApi::copyDir(oriEnginePath, enginePath, true,true);

    
    //enginePath = QFile::encodeName(enginePath);
    this->sliceInfo = new AkSliceInfo();
    this->pInvoker = new QProcess();
    pInvoker->setProcessChannelMode(QProcess::MergedChannels);
    connect(this->pInvoker, &QProcess::readyReadStandardError,this,&AkSlicer::newStdErrGenerated);
    
    connect(this->pInvoker, &QProcess::readyRead,this,&AkSlicer::newStdErrGenerated);
    connect(this->pInvoker, &QProcess::readAllStandardOutput,this,&AkSlicer::newStdErrGenerated);

#ifdef DEBUG_BY_CL
    connect(&this->engineLib, &EngineLibThread::newStdErrGeneratedChars, this,&AkSlicer::newStdErrGeneratedChars);
#endif
}

AkSlicer::~AkSlicer()
{
    if (NULL != pInvoker)
    {
        delete pInvoker;
        pInvoker = NULL;
    }
    if (NULL != sliceInfo)
    {
        delete sliceInfo;
        sliceInfo = NULL;
    }
}

void AkSlicer::slice()
{
    QDir engineDir(enginePath);
    if(!engineDir.exists())
    {
        emit progress(SliceStep::Slice, SliceStatus::Error,0, "ENGINE_NOT_EXIST");
        return;
    }
    
    //QString resultPath = engineDir.absoluteFilePath("result");
    QFileInfo fInfo(stlFile);
    QString gcodeName = fInfo.fileName().replace(".stl",".gcode",Qt::CaseInsensitive);
    
    //QString destFile = appDataLocDir.absoluteFilePath(QString("result/%1").arg(gcodeName));
    //QString destFile = fInfo.dir().absoluteFilePath(gcodeName).toLocal8Bit();
    QString destFile = fInfo.dir().absoluteFilePath(gcodeName);

    slice(stlFile,destFile,userSetting);
}


//bool AkSlicer::setEnginePath(QString enginePath)
//{

//}
QString AkSlicer::toRelativePath(QString absfilePath)
{
    QDir engineDir(enginePath);
    return engineDir.relativeFilePath(absfilePath);
    //return "../" + engineDir.relativeFilePath(absfilePath);
}


void AkSlicer::slice(QString stlFile,QString destFile, QString userSetting)
{
    TFunction(QString("%1,%2").arg(stlFile).arg(destFile));

    QDir engineDir(enginePath);
    if(!engineDir.exists())
    {
        emit progress(SliceStep::Slice, SliceStatus::Error,0, "ENGINE_NOT_EXIST");
        return;
    }
    //create the dest dir if it not exist
    QDir destFileDir = QFileInfo(destFile).absoluteDir();
    if (!destFileDir.exists())
    {
        //destFileDir.mkdir(destFileDir.absolutePath());
        destFileDir.mkpath(destFileDir.absolutePath());
    }

    
    this->sliceInfo->stlFile = stlFile;
    this->sliceInfo->gcodeFile = destFile;
    this->sliceInfo->originalStlName = this->originalStlName;
    
    auto getSptMeshCmd = [](QString meshFile)->QString {
        //-e0 -l "1" -s extruder_nr="0" -s support_mesh="True" -s support_mesh_drop_down="False"
        return QString(" -e0 -l \"%1\" -s extruder_nr=0 -s support_mesh=true -s support_mesh_drop_down=false ").arg(meshFile);
    };
    QStringList sptMeshCmd;
    for(int i=0;i<supportMeshes.size();i++)
    {
        
        QString relativeSptFilePath = toRelativePath(supportMeshes[i]);
        sptMeshCmd.append(getSptMeshCmd(relativeSptFilePath));
    }
    //QString cmd = QString("C:/workspace/TortoiseGit/Repository/SliceEngine/CuraEngine slice -v -p -m4 -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2.def.json -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2_extruder_0.def.json -o C:/workspace/TortoiseGit/Repository/SliceEngine/result/cube.gcode -l C:/workspace/TortoiseGit/Repository/SliceEngine/stl/SimpleCube.stl")

    
    //QString engineExe = engineDir.absoluteFilePath("CuraEngine").toLocal8Bit();
    
    //QString extruderConfig = engineDir.absoluteFilePath("config/anker_fdmextruder.def.json").toLocal8Bit();

    
    QString engineExe = engineDir.absoluteFilePath("CuraEngine");
    QString baseConfig = engineDir.absoluteFilePath("config/anker_fdmprinter.def.json");
    QString extruderConfig = engineDir.absoluteFilePath("config/anker_fdmextruder.def.json");
    QString otherConfig = getConfig(engineDir.absoluteFilePath("config"));

    
    auto relativeEngineExe = toRelativePath(engineExe);
    auto relativeBaseConfig = toRelativePath(baseConfig);
    auto relativeExtruderConfig = toRelativePath(extruderConfig);
    auto relativeOtherConfig = toRelativePath(otherConfig);
    auto relativeDestFile = toRelativePath(destFile);
    auto relativeStlFile = toRelativePath(stlFile);

    
    //QString cmd = QString("%1 slice -v -p -j %2 %3 %4 -o %5 -g -l \"%6\" %7 ").arg(engineExe).arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));
    QString extendCmd = QString(" slice -v -p -j \"%1\" %2 %3 -o \"%4\" -g -l \"%5\" %6 ").arg(relativeBaseConfig).arg(relativeOtherConfig).arg(userSetting).arg(relativeDestFile).arg(relativeStlFile).arg(sptMeshCmd.join(" "));
    QFileInfo fInfo(stlFile);
    
    //QString cmdFile = fInfo.dir().absoluteFilePath("sliceCmd.cmd").toLocal8Bit();
    QString cmdFile = fInfo.dir().absoluteFilePath("sliceCmd.cmd");

    
    TDebug(extendCmd);

    
    //auto extBase64Cmd = extendCmd.toLocal8Bit().toBase64();
    auto extBase64Cmd = extendCmd.toUtf8().toBase64();

    
    IoApi::write(cmdFile, extBase64Cmd);
    QString relativeCmdFile = toRelativePath(cmdFile);
    //QString cmd = QString("chcp 65001 & \"%1\" extParam -f \"%2\"").arg(engineExe).arg(cmdFile);
    //QString cmd = QString("\"%1\" extParam -f \"%2\"").arg(engineExe).arg(relativeCmdFile).toUtf8();
    //QString cmd = QString("cd  \"%1\" && \"%2\" extParam -f \"%3\"").arg(enginePath).arg(engineExe).arg(relativeCmdFile).toUtf8();

    
    QDir::setCurrent(enginePath);

    
    //QString runEngine = QString("chcp 65001 \r\n cd  \"%1\" \r\n \"%2\" extParam -f \"%3\"").arg(enginePath).arg(relativeEngineExe).arg(relativeCmdFile);

    
    //QString runEngine = QString("chcp 65001 \r\n set path=\"\%path\%;%1\" \r\n cd \"%2\" \r\n echo \"\%cd\%\" \r\n \"%3\" extParam -f \"%4\"").arg(enginePath).arg(enginePath).arg(relativeEngineExe).arg(relativeCmdFile);

//#ifdef _WIN32
//    //QString runEngine = QString("chcp 65001 \r\n set path=\"\%path\%;%1\" \r\n cd \"%2\" \r\n echo \"\%cd\%\" \r\n \"%3\" extParam -f \"%4\"").arg(enginePath).arg(enginePath).arg(relativeEngineExe).arg(relativeCmdFile);
//    QString runEngine = QString("chcp 65001 \r\n \"%1\" extParam -f \"%2\"").arg(relativeEngineExe).arg(relativeCmdFile);
//#else
//    //QString runEngine = QString("chcp 65001 \r\n set path=\"\$path %1\" \r\n cd \"%2\" \r\n echo \"\%cd\%\" \r\n \"%3\" extParam -f \"%4\"").arg(enginePath).arg(enginePath).arg(relativeEngineExe).arg(relativeCmdFile);
//    QString runEngine = QString("\"%1\" extParam -f \"%2\"").arg(relativeEngineExe).arg(relativeCmdFile);
//#endif
//    QString batFile = fInfo.dir().absoluteFilePath("runEngine.bat");
//    IoApi::write(batFile, runEngine);
//    runCmd(batFile);

    
    QString runEngine = QString("\"./%1\" extParam -f \"%2\"").arg(relativeEngineExe).arg(relativeCmdFile).toUtf8();

//    QString batFile = fInfo.dir().absoluteFilePath("runEngine.bat");
//    IoApi::write(batFile, runEngine);
    runCmd(runEngine);
}


//void AkSlicer::slice(QString stlFile,QString destFile, QString userSetting)
//{
//    TFunction(QString("%1,%2").arg(stlFile).arg(destFile));

//    QDir engineDir(enginePath);
//    if(!engineDir.exists())
//    {
//        emit progress(SliceStep::Slice, SliceStatus::Error,0, "ENGINE_NOT_EXIST");
//        return;
//    }
//    //create the dest dir if it not exist
//    QDir destFileDir = QFileInfo(destFile).absoluteDir();
//    if (!destFileDir.exists())
//    {
//        //destFileDir.mkdir(destFileDir.absolutePath());
//        destFileDir.mkpath(destFileDir.absolutePath());
//    }


//    this->sliceInfo->stlFile = stlFile;
//    this->sliceInfo->gcodeFile = destFile;
//    this->sliceInfo->originalStlName = this->originalStlName;

//    auto getSptMeshCmd = [](QString meshFile)->QString {
//        //-e0 -l "1" -s extruder_nr="0" -s support_mesh="True" -s support_mesh_drop_down="False"
//        return QString(" -e0 -l \"%1\" -s extruder_nr=0 -s support_mesh=true -s support_mesh_drop_down=false ").arg(meshFile);
//    };
//    QStringList sptMeshCmd;
//    for(int i=0;i<supportMeshes.size();i++)
//    {
//        sptMeshCmd.append(getSptMeshCmd(supportMeshes[i]));
//    }
//    //QString cmd = QString("C:/workspace/TortoiseGit/Repository/SliceEngine/CuraEngine slice -v -p -m4 -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2.def.json -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2_extruder_0.def.json -o C:/workspace/TortoiseGit/Repository/SliceEngine/result/cube.gcode -l C:/workspace/TortoiseGit/Repository/SliceEngine/stl/SimpleCube.stl")


//    //QString engineExe = engineDir.absoluteFilePath("CuraEngine").toLocal8Bit();

//    //QString extruderConfig = engineDir.absoluteFilePath("config/anker_fdmextruder.def.json").toLocal8Bit();


//    QString engineExe = engineDir.absoluteFilePath("CuraEngine");
//    QString baseConfig = engineDir.absoluteFilePath("config/anker_fdmprinter.def.json");
//    QString extruderConfig = engineDir.absoluteFilePath("config/anker_fdmextruder.def.json");

//    QString otherConfig = getConfig(engineDir.absoluteFilePath("config"));
//    //QString cmd = QString("%1 slice -v -p -m4 -j %2 -j %3 %4 %5 -o %6 -l %7").arg(engineExe).arg(baseConfig).arg(extruderConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile);

//    //QString cmd = QString("%1 slice -v -p -m4 -j %2 -j %3 %4 %5 -o %6 -g -l \"%7\" %8 ").arg(engineExe).arg(baseConfig).arg(extruderConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));



//    //QString cmd = QString("%1 slice -v -p -j %2 %3 %4 -o %5 -g -l \"%6\" %7 ").arg(engineExe).arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));
//    QString extendCmd = QString(" slice -v -p -j \"%1\" %2 %3 -o \"%4\" -g -l \"%5\" %6 ").arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));
//    QFileInfo fInfo(stlFile);

//    //QString cmdFile = fInfo.dir().absoluteFilePath("sliceCmd.cmd").toLocal8Bit();
//    QString cmdFile = fInfo.dir().absoluteFilePath("sliceCmd.cmd");


//    TDebug(extendCmd);


//    //auto extBase64Cmd = extendCmd.toLocal8Bit().toBase64();
//    auto extBase64Cmd = extendCmd.toUtf8().toBase64();


//    IoApi::write(cmdFile, extBase64Cmd);
//    //QString cmd = QString("chcp 65001 & \"%1\" extParam -f \"%2\"").arg(engineExe).arg(cmdFile);
//    QString cmd = QString("\"%1\" extParam -f \"%2\"").arg(engineExe).arg(cmdFile).toUtf8();


//    //QString extendCmd = QString(" slice -v -p -j %2 %3 %4 -o %5 -g -l \"%6\" ").arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile);
//    //QFileInfo fInfo(stlFile);
//    //QString cmdFile = fInfo.dir().absoluteFilePath("sliceCmd.cmd");

//    //IoApi::write(cmdFile, extendCmd);
//    //QString cmd = QString("%1 extParam -f %2 %3").arg(engineExe).arg(cmdFile).arg(sptMeshCmd.join(" "));

//    //    //QString cmd = QString("%1 slice -v -p -j %2 %3 %4 -o %5 -g -l \"%6\" %7 ").arg(engineExe).arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));
//    //    QString cmd = QString("\"%1\" slice -v -p -j \"%2\" %3 %4 -o \"%5\" -g -l \"%6\" %7 ").arg(engineExe).arg(baseConfig).arg(otherConfig).arg(userSetting).arg(destFile).arg(stlFile).arg(sptMeshCmd.join(" "));

//    runCmd(cmd);

//}

QString AkSlicer::getConfig(QString configPath)
{
   QStringList settingList;
   QMap<QString, QString> paramMap;
   
   QStringList filter;
   filter << "*.cfg";
   auto fileInfoList = IoApi::getFiles(configPath,filter,IoApi::SearchOption::AllDirectories);
   for (int i = 0; i < fileInfoList.size(); i++)
   {
       QString configContent = IoApi::readAll(fileInfoList[i].absoluteFilePath());
       QString key = "[values]";
       int valueStartIdx = configContent.indexOf(key);
       if (valueStartIdx<0)
       {
           continue;
       }
       QString settingContent = configContent.mid(valueStartIdx + key.length());


       QRegularExpression exp("^(.*?)=(.*?)$", QRegularExpression::MultilineOption|QRegularExpression::InvertedGreedinessOption);
       auto kvMatches = exp.globalMatch(settingContent);


       //var kvMatchs = Regex.Matches(settingContent, regStr, RegexOptions.Multiline);
       while (kvMatches.hasNext())
       {
           auto kvMatch = kvMatches.next();
           if (kvMatch.hasMatch())
           {
               QString name = kvMatch.captured(1).trimmed();
               QString value = kvMatch.captured(2).trimmed();
               paramMap[name] = value;
           }
       }
   }

   //infill_pattern = ='lines' if infill_sparse_density > 25 else 'grid'
   if (paramMap.contains("infill_sparse_density"))
   {
       float density = paramMap["infill_sparse_density"].toFloat();
       if (density>25)
       {
           paramMap["infill_pattern"] = "lines";
       }
       else
       {
           paramMap["infill_pattern"] = "grid";
       }
   }
   for(auto it = paramMap.constBegin();it!=paramMap.constEnd();it++)
   {
       settingList.append(QString(" -s %1=%2 ").arg(it.key()).arg(it.value()));
   }

   return settingList.join(" ");
}

const QStringList &AkSlicer::getSupportMeshes() const
{
    return supportMeshes;
}

void AkSlicer::setSupportMeshes(const QStringList &newSupportMeshes)
{
    supportMeshes = newSupportMeshes;
}


const QString AkSlicer::getStlPath() const
{
    //QDir engineDir(enginePath);
    //QString resultPath = engineDir.absoluteFilePath("stl");

   /* QString appDataLoc = QStandardPaths::displayName(QStandardPaths::AppLocalDataLocation);
    QString appDataLoc2 = QStandardPaths::displayName(QStandardPaths::DesktopLocation);
    QString appDataLoc3 = QStandardPaths::displayName(QStandardPaths::ApplicationsLocation);
    QString appDataLoc4 = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);


    QString bppDataLoc = QStandardPaths::displayName(QStandardPaths::AppLocalDataLocation);
    QString bppDataLoc2 = QStandardPaths::displayName(QStandardPaths::DesktopLocation);
    QString bppDataLoc3 = QStandardPaths::displayName(QStandardPaths::ApplicationsLocation);
    QString bppDataLoc4 = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);*/


    QString resultPath = appDataLocDir.absoluteFilePath("stl");

    QDir resultDir(resultPath);
    if (!resultDir.exists())
    {
        resultDir.mkpath(resultPath);
    }

    return resultPath;
}

const QString &AkSlicer::getStlFile() const
{
    return stlFile;
}

void AkSlicer::setStlFile(const QString &newStlFile)
{
    stlFile = newStlFile;
}

void AkSlicer::setOriginalStlName(const QString &newOriginalStlName)
{
    originalStlName = newOriginalStlName;
}

const QString &AkSlicer::getUserSetting() const
{
    return userSetting;
}

void AkSlicer::setUserSetting(const QString &newUserSetting)
{
    userSetting = newUserSetting;
}

//
void AkSlicer::runCmd(QString cmd)
{
    TDebug(cmd);
    this->sliceInfo->sliceCmd = cmd;

    //QDir engineDir(enginePath);
    //engineDir.cd(enginePath);

#if defined(DEBUG_BY_CL)
    
    engineLib.runCmd(cmd);
    AkUtil::TWarning("run static lib: ");
#else
    //pInvoker->start("C:/workspace/TortoiseGit/Repository/SliceEngine/CuraEngine slice -v -p -m4 -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2.def.json -j C:/workspace/TortoiseGit/Repository/SliceEngine/config/ultimaker2_extruder_0.def.json -o C:/workspace/TortoiseGit/Repository/SliceEngine/result/cube.gcode -l C:/workspace/TortoiseGit/Repository/SliceEngine/stl/SimpleCube.stl");
    
    //pInvoker->setProcessChannelMode(QProcess::MergedChannels);
    pInvoker->start(cmd);
    AkUtil::TWarning("run exe : ");

#endif
    qDebug() << "\t" << cmd;
}


void AkSlicer::cancel()
{
    //pInvoker->close();
    //pInvoker->finished(0,QProcess::ExitStatus::NormalExit);
    pInvoker->kill();
    emit sliceFailed(*sliceInfo);
}

void AkSlicer::resetParam()
{
    stlFile = "";
    userSetting = "";
}

void AkSlicer::clearGcode(QSet<QString> excludeSet,int second)
{
    QStringList filter;
    filter << "*.gcode";
    clearFile(second,"result",filter,excludeSet);
}
void AkSlicer::clearStl(QSet<QString> excludeSet,int second)
{
//    QStringList filter;
//    filter << "*.stl";
//    clearFolder(second,"stl",filter,excludeSet);
      clearFolder(second,"stl",excludeSet);
}


void AkSlicer::clearFolder(int second,QString folder, QSet<QString> excludeSet)
{
    //QDir engineDir(enginePath);
    QString resultPath = appDataLocDir.absoluteFilePath(folder);
    IoApi::clearFoldersUnderPath(second,resultPath,excludeSet);
}
void AkSlicer::clearFile(int second,QString folder, QStringList filter,QSet<QString> excludeSet)
{
    //QDir engineDir(enginePath);
    QString resultPath = appDataLocDir.absoluteFilePath(folder);
    IoApi::clearFilesUnderPath(second,resultPath,filter, excludeSet);
}


void AkSlicer::newStdErrGenerated()
{
    //QByteArray res = pInvoker->readAllStandardError();
    QByteArray res = pInvoker->readAllStandardOutput();
    newStdErrGeneratedChars(res);
}

void AkSlicer::newStdErrGeneratedChars(QByteArray res)
{
    
    //static QRegExp progressRx("Progress:(.*?):(\\d+):(\\d+)\\s*(?:\\\\t)*([0-9.]+)%");
    //static QRegExp progressRx("Progress:(.*?):(\\d+):(\\d+)(?:.*?)([0-9.]+)");
    //QRegExp progressRx("Progress:(.*)");
    static QRegExp progressRx("Progress:(.*):(\\d+):(\\d+)\\s*([0-9.]+)%");
    progressRx.setMinimal(true);

    static int count = 0;

    //QString msg = QString::fromLocal8Bit(res);
    QString msg = QString(res);
    
    TDebug(msg);
    //qWarning().noquote().nospace() << "[[SlicerEngine]]" << msg;
//    TInfo(msg);
//    TInfo(QString("================== %1 ").arg(++count));
    
    //QChar hell[]{'\r','\n'};
    QStringList lines = msg.split('\r',Qt::SkipEmptyParts);
    foreach(QString line, lines)
    {
#if defined(DEBUG_BY_CL) // add  @2022-05-15 by CL
        if(true || line.startsWith("$CL$"))  
        {
            
            QString logPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            QString logFile = QDir(logPath).absoluteFilePath("engine.log");

            AkUtil::IoApi::touch(logFile);
            AkUtil::IoApi::append(logFile, line);
            qDebug() << line;
        }
#endif

        
        if (line.indexOf("[ERROR]") >= 0)
        {
            emit progress(SliceStep::Slice, SliceStatus::Error,0, line);
            
            QMessageBox::warning(nullptr, tr("Slice Engine Error"), line);
            cancel();
            return;
        }
        
        if (line.indexOf("Progress:")>=0)
        {
            int pos = progressRx.indexIn(line);
            if (pos < 0)
            {
                continue;
//                QString str1 = progressRx.cap(1);
//                QString str2 = progressRx.cap(2);
//                QString str3 = progressRx.cap(3);
//                QString str4 = progressRx.cap(4);
            }

            auto matchs = progressRx.capturedTexts();
            if (matchs.size() < 5)
            {
                
                emit progress(SliceStep::Slice, SliceStatus::Error,0, "PROGRESS_READ_FAILED");
                continue;
            }
            float percent =  matchs[4].toFloat()*100;
            QString msg = QString("%1:%2:%3").arg(matchs[1]).arg(matchs[2]).arg(matchs[3]);
            emit progress(SliceStep::Slice, SliceStatus::Doing, percent, msg);
        }
        
        
        int indexOfSliceTime = line.indexOf("Total time elapsed");
        if (indexOfSliceTime>=0)
        {
            //Total time elapsed  3.80s.
            QString sliceTime = line.mid(indexOfSliceTime+18);
            sliceTime = sliceTime.mid(0,sliceTime.length()-2);
            sliceInfo->sliceTime = sliceTime.toFloat();
        }

        
        int indexOfPrintTime = line.indexOf("Print time (s):");
        if (indexOfPrintTime>=0)
        {
            QString printTime = line.mid(indexOfPrintTime+15);
            sliceInfo->printTime = printTime.toFloat();
        }

        
        int indexOfFilament = line.indexOf("Filament (mm^3):");
        if (line.indexOf("Filament (mm^3):")>=0)
        {
            QString filament = line.mid(indexOfFilament+16);
            sliceInfo->filamentCost = filament.toFloat();
            
            emit sliceSuccess(*sliceInfo);
        }

    }
}


