#include "fdm_slicer.h"
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include "../common/ak_const.h"
#include <QThread>
#include <QtDebug>
#include "akslicepanel.h"
#include <QDateTime>
#include <QSet>
#include <QDir>
#include <QTextCodec>
#include "common/utilities/tlogger.h"
#include "common/utilities/ioapi.h"
#include <QStandardPaths>
#include <QThreadPool>
#include <QCoreApplication>
#include "common/mlapplication.h"
#include <QRegularExpression>
#include "../common/controlInterface/messageDialog.h"
#include <QDebug>
using namespace AkUtil;

FdmSlicer::FdmSlicer()
{
    slicer = new AkSlicer();
    progressBar = NULL;
    timer = new QTimer(this);
}

FdmSlicer::~FdmSlicer()
{
    TFunction("");
    if (NULL != slicer)
    {
        delete slicer;
        slicer = NULL;
    }
}

void FdmSlicer::initialize(ControlInterface *controlmanager, RichParameterList *globalParameterList)
{
    this->ctrlmanager = controlmanager;
    //AkSlicePanel* panel = new AkSlicePanel();
    panel = new AkSlicePanel();
    

    panel->sliceEnable(false);
    

    ctrlmanager->addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex::SlicePanel, panel);

    

    connect(panel,&AkSlicePanel::slice,this,&FdmSlicer::doSlice);
    connect(panel,&AkSlicePanel::save,this,&FdmSlicer::doSave);
    connect(panel,&AkSlicePanel::preview,this,&FdmSlicer::doPreview);
    connect(panel,&AkSlicePanel::previewBtnClicked,this,&FdmSlicer::doPreview);//change doPreviewBtnClicked to doPreview
    

    connect(this,&FdmSlicer::sliceSucess,panel,&AkSlicePanel::doSliceSuccess);
    connect(this,&FdmSlicer::sliceFailed,panel,&AkSlicePanel::doSliceFailed);
    //connect(this, &FdmSlicer::appendSliceBtnStat, panel, &AkSlicePanel::reset);
    connect(this,&FdmSlicer::appendSliceBtnStat,panel,&AkSlicePanel::doAppendSliceBtnStat);

    //connect(this,&FdmSlicer::sliceEnable,panel,&AkSlicePanel::sliceEnable);

    

    //connect(panel,&AkSlicePanel::quit,this,&FdmSlicer::quit);

    connect(this->slicer, &AkSlicer::progress, this, &FdmSlicer::setProgressBar);
    connect(this->slicer, &AkSlicer::sliceSuccess, this, &FdmSlicer::doSliceSuccess);
    connect(this->slicer, &AkSlicer::sliceFailed, this, &FdmSlicer::doSliceFailed);
}


void FdmSlicer::doSliceSuccess(AkSliceInfo sliceInfo)
{
    this->sliceResult = sliceInfo;
    if (progressBar != nullptr)
    {
        progressBar->setValue(100);
    }
    bool fileOperEnd = false;
    QThreadPool::globalInstance()->start([&,this]()->void{
        //sliceBtn->setText("save");
        

        //if (NULL != progressBar){
        //    delete progressBar;
        //    progressBar = NULL;
        //};

        

        

        QString realtimeProfilePath = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)).absoluteFilePath("setting/fdm/parameter/realtime/realtimeprofile.ini");
        QString content = IoApi::readAll(realtimeProfilePath);
    //    QStringList paramList;
    //    paramList << ";;paramBegin";
    //    paramList << content.toUtf8().toBase64();
    //    paramList << ";;paramEnd";
    //
        

        auto gcodeContent = IoApi::readAll(sliceResult.gcodeFile);
        int count = 100;
        while (count > 0 && gcodeContent.indexOf(";End of Gcode") < 0)
        {
            QThread::msleep(100);
            gcodeContent = IoApi::readAll(sliceResult.gcodeFile);
            count--;
        }
        QStringList strList;

//        auto getMaxSpeedAndPrintParams = [&]()->float{
//            float maxSpeed = -99;
//            QString travelSpeed = "250";
//            //遍历所有行. 获取打印速度
//            auto lines = gcodeContent.split("\n",Qt::KeepEmptyParts);
//            for(int i = 0; i< lines.size();i++)
//            {
//                QString line = lines[i].trimmed();
//                strList.append(line);
//                if (line.isEmpty())
//                {
//                    continue;
//                }
//                line = line.trimmed();
//                //qDebug() << line;
//                //add some line after this line
//                if (line.startsWith(";TIME:"))
//                {
//                    //get param from realtimeprofile
//                    QString filamentName = "";
//                    QString nozzleSize = "";
//                    QString printMode = "";
//                    auto paramLines = content.split("\n",Qt::SkipEmptyParts);
//                    int count = 0;
//                    for(int j = 0; j< paramLines.size();j++)
//                    {
//                        QString parmLine = paramLines[j].trimmed();
//                        if (parmLine.startsWith("meta_current_material_name="))
//                        {
//                            filamentName = parmLine.replace("meta_current_material_name=","");
//                            count++;
//                        }
//                        if (parmLine.startsWith("machine_nozzle_size="))
//                        {
//                            nozzleSize = parmLine.replace("machine_nozzle_size=","");
//                            count++;
//                        }
//                        if (parmLine.startsWith("param_print_mode="))
//                        {
//                            printMode = parmLine.replace("param_print_mode=","");
//                            count++;
//                        }
//                        if (parmLine.startsWith("speed_travel="))
//                        {
//                            travelSpeed = parmLine.replace("speed_travel=","");
//                            count++;
//                        }
//                        if (count == 4)
//                        {
//                            break;
//                        }
//                    }

//                    strList.append(QString(";Filament Name:%1").arg(filamentName));
//                    strList.append(QString(";Machine Nozzle Size:%1").arg(nozzleSize));
//                    strList.append(QString(";Print Mode:%1").arg(printMode));
//                    qDebug() << "filamentName" << filamentName << "nozzleSize" << nozzleSize << "printMode" << printMode << "travelSpeed" << travelSpeed;
//                }

//                //跳过注释和非挤出的行
//                if (line.trimmed().startsWith(";")
//                    || !line.trimmed().startsWith("G")
//                    || line.indexOf("E") < 0
//                    || line.indexOf("F") < 0)
//                {
//                     continue;
//                }
//                auto words = line.split(" ",Qt::SkipEmptyParts);
//                for(int j = 0; j< words.size();j++)
//                {
//                    if (!words[j].startsWith("F"))
//                    {
//                        continue;
//                    }
//                    try
//                    {
//                        float speed = words[j].mid(1).toFloat();
//                        maxSpeed = (maxSpeed < speed) ? speed : maxSpeed;
//                    }
//                    catch(...){}
//                }
//            }
//            //show travel speed as max speed
//            float travelSpeedF = travelSpeed.toFloat()*60;
//            maxSpeed = (maxSpeed < travelSpeedF) ? travelSpeedF : maxSpeed;
//            return maxSpeed;
//        };

        QMap<QString, QString> kvMap;
        const QString meta_current_material_name = AkConst::SettingKey::META_CURRENT_MATERIAL_NAME;
        const QString machine_nozzle_size = AkConst::SettingKey::MACHINE_NOZZLE_SIZE;
        const QString param_print_mode = AkConst::SettingKey::QML_PRINT_MODE;
        const QString speed_travel = "speed_travel";
        const QString meta_current_profile_name = AkConst::SettingKey::META_CURRENT_PROFILE_NAME;
        QList<QString> keyList = {
            meta_current_material_name,
            machine_nozzle_size,
            param_print_mode,
            meta_current_profile_name,
            speed_travel,
            "material_print_temperature",
            "material_print_temperature_layer_0",
            "material_bed_temperature",
            "material_bed_temperature_layer_0",
            "layer_height_0",
            "bridge_settings_enabled",
            "bridge_wall_speed",
            "bridge_skin_speed",
            "bridge_skin_speed_2",
            "bridge_skin_speed_3",
            "speed_print",
            "speed_infill",
            "speed_wall",
            "speed_wall_0",
            "speed_wall_x",
            "speed_topbottom",
            "speed_roofing",
            "support_enable",
            "speed_support",
            "speed_support_infill",
            "speed_support_interface",
            "speed_support_roof",
            "speed_support_bottom",
            "speed_layer_0",
            "speed_print_layer_0",
            "speed_travel_layer_0",
            "speed_z_hop",
            "retraction_amount",
            "retraction_retract_speed",
            "retraction_prime_speed",
            "adhesion_type",
            "skirt_brim_speed",
            "raft_speed",
            "raft_surface_speed",
            "raft_interface_speed",
            "raft_base_speed",
            "acceleration_enabled",
            "acceleration_travel",
            "acceleration_print",
            "jerk_enabled",
            "jerk_travel",
            "jerk_print",
            "jerk_infill",
            "jerk_wall",
            "jerk_wall_0",
            "jerk_wall_x",
            "jerk_topbottom",
            "jerk_roofing",
            "jerk_support",
            "jerk_support_infill",
            "jerk_support_interface",
            "jerk_support_roof",
            "jerk_support_bottom",
            "jerk_prime_tower",
            "jerk_layer_0",
            "jerk_travel_layer_0",
            "jerk_print_layer_0",
            "jerk_skirt_brim",
            "ak_VAJK_J_E_enabled",
            "ak_J_E_print",
            "ak_J_E_infill",
            "ak_J_E_wall",
            "ak_J_E_wall_0",
            "ak_J_E_wall_x",
            "ak_J_E_topbottom",
            "ak_J_E_support",
            "ak_J_E_skirt_brim",
            "ak_J_E_layer_0",
            "ak_VAJK_K_enabled",
            "ak_K_print",
            "ak_K_infill",
            "ak_K_wall",
            "ak_K_wall_0",
            "ak_K_wall_x",
            "ak_K_topbottom",
            "ak_K_support",
            "ak_K_skirt_brim",
            "ak_K_layer_0",
        };

        auto getValue = [&](const QString& key)->QString {
            if (kvMap.contains(key))
            {
                return kvMap[key];
            }
            return "";
        };

        //parameter lines
        auto initPrintParams = [&]()->void {
            auto lines = content.split("\n", Qt::SkipEmptyParts);
            for (int i = 0; i < lines.size(); i++)
            {
                QString line = lines[i].trimmed();
                if (line.isEmpty() || line.startsWith(";"))
                {
                    continue;
                }

                auto words = line.split("=", Qt::KeepEmptyParts);
                if (words.size() < 2)
                {
                    continue;
                }
                auto key = words[0].trimmed();
                auto value = words[1].trimmed();
                kvMap[key] = value;
            }
        };

        //insert print parameters into gcode
        auto insertPrintParams = [&](const QStringList& lines)->void {
            for (int i = 0; i < lines.size(); i++)
            {
                QString line = lines[i].trimmed();
                if(line.isEmpty() || line.startsWith(";IN jerk of travel")){
                    continue;
                }

                strList.append(line);                     
                line = line.trimmed();
                //add some line after this line
                if (line.startsWith(";TIME:"))
                {
                    //get param from realtimeprofile
                    strList.append(QString(";Machine Name:%1").arg(getValue(AkConst::SettingKey::META_CURRENT_MACHINE_NAME)));
                    strList.append(QString(";Filament Name:%1").arg(getValue(meta_current_material_name)));
                    strList.append(QString(";Machine Nozzle Size:%1").arg(getValue(machine_nozzle_size)));
                    strList.append(QString(";Print Mode:%1").arg(getValue(param_print_mode)));
                    strList.append(QString(";Profile Name:%1").arg(getValue(meta_current_profile_name)));
                    strList.append(QString(";multi_model:%1").arg(sliceInfo.meshCount>1?"true":"false"));
                    strList.append(QString(";damaged:false"));
                    strList.append(QString(";repaired:false"));
                    for (int k = 4; k < keyList.size(); k++)
                    {
                        strList.append(QString(";%1:%2").arg(keyList[k]).arg(getValue(keyList[k])));
                    }
                }
            }
        };
        auto getMaxSpeed = [&](const QStringList& lines)->float {
            //get max speed
            float maxSpeed = -99;
            QString travelSpeed = getValue(speed_travel);
            if (travelSpeed == "") { travelSpeed = "250"; }
            for (int i = 0; i < lines.size(); i++)
            {
                QString line = lines[i].trimmed();
                if (line.isEmpty())
                {
                    continue;
                }
                line = line.trimmed();
    
                if (line.trimmed().startsWith(";")
                    || !line.trimmed().startsWith("G")
                    || line.indexOf("E") < 0
                    || line.indexOf("F") < 0)
                {
                     continue;
                }
                auto words = line.split(" ",Qt::SkipEmptyParts);
                for(int j = 0; j< words.size();j++)
                {
                    if (!words[j].startsWith("F"))
                    {
                        continue;
                    }
                    try
                    {
                        float speed = words[j].mid(1).toFloat();
                        maxSpeed = (maxSpeed < speed) ? speed : maxSpeed;
                    }
                    catch(...){}
                }
            }
            //show travel speed as max speed
            float travelSpeedF = travelSpeed.toFloat() * 60;
            maxSpeed = (maxSpeed < travelSpeedF) ? travelSpeedF : maxSpeed;
            return maxSpeed;
        };
        

        if (gcodeContent.indexOf(";MAXSPEED:") > 0)
        {
            auto lines = gcodeContent.split("\n", Qt::KeepEmptyParts);
            initPrintParams();
            insertPrintParams(lines);
            float maxPrintSpeed = getMaxSpeed(lines);
            QString newContent = strList.join("\r\n");

            if (maxPrintSpeed > 0)
            {
                newContent = newContent.replace(QRegularExpression(";MAXSPEED:.*?\r"), ";MAXSPEED:" + QString::number(maxPrintSpeed) + "\r");
            }

            IoApi::write(sliceResult.gcodeFile, newContent.toUtf8());
        }

        

    //    QStringList paramList;
    //    paramList << ";paramBegin";
    //    paramList << content.toUtf8().toBase64();
    //    paramList << ";paramEnd";
    //    IoApi::append(sliceInfo.gcodeFile, paramList.join(""));

        QStringList paramList;
        paramList << ";paramBegin";
        //paramList << content.toUtf8().toBase64();
        auto paramContent = content.toUtf8().toBase64();
        auto subList = IoApi::splite(paramContent, 200);
        for (int i = 0; i < subList.size(); i++)
        {
            paramList << ";" + subList[i];
        }
        paramList << ";paramEnd";
        paramList << ";AnkerMake version: " + MeshLabApplication::appVer();
        

        paramList << ";End of Gcode";
        auto data = paramList.join("\r\n").toUtf8();
        IoApi::append(sliceResult.gcodeFile, data);
        fileOperEnd = true;
        //IoApi::append(sliceInfo.gcodeFile, QString(";paramBegin").toLocal8Bit());
        //auto realtimeBase64 = content.toLocal8Bit().toBase64();
        //IoApi::append(sliceInfo.gcodeFile, realtimeBase64);
        //IoApi::append(sliceInfo.gcodeFile, ";paramEnd");





//        PluginMessageData data;
//        data.from = AkConst::Plugin::FDM_SLICER;
//        data.dest = AkConst::Plugin::FDM_SETTING;
//        data.msg = AkConst::Msg::SLICE_SUCCESS;
//        emit sendMsg2Manager(data);


//        emit this->sliceSucess();

//        emit this->appendSliceBtnStat(ESliceBtnStat::SLICE_END);




    });
    //emit this->appendSliceBtnStat(ESliceBtnStat::SLICE_END);

    

    int count = 500;
    while(!fileOperEnd && count>0)
    {
        count--;
        QThread::msleep(20);
        QCoreApplication::processEvents();
    }
    

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_SETTING;
    data.msg = AkConst::Msg::SLICE_SUCCESS;
    emit sendMsg2Manager(data);
    

    emit this->sliceSucess();
    

    emit this->appendSliceBtnStat(ESliceBtnStat::SLICE_END);
}

void FdmSlicer::doSliceFailed(AkSliceInfo sliceInfo)
{
    this->sliceResult = sliceInfo;
    qDebug() << sliceInfo.sliceCmd;
    qDebug() << QString("enter doSliceFailed emit sliceFailed %1").arg((long long)(progressBar));
    //sliceBtn->setText("slice");
    

    //if (NULL != progressBar){
    //    delete progressBar;
    //    progressBar = NULL;
    //}



    

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_SETTING;
    data.msg = AkConst::Msg::SLICE_FAILED;
    emit sendMsg2Manager(data);

    qDebug() << "doSliceFailed emit sliceFailed" ;
    emit sliceFailed();
    emit appendSliceBtnStat(ESliceBtnStat::SLICE_END);
}



void FdmSlicer::getGlobalSupportStatus()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SETTING;
    data.dest = AkConst::Plugin::FDM_EDIT_MODEL;
    data.msg = AkConst::Msg::GET_GLOBAL_SUPPORT_DISABLED_STATUS;
    emit sendMsg2Manager(data);
}


void FdmSlicer::setProgressBar(AkSlicer::SliceStep step, AkSlicer::SliceStatus status, float percent, QString msg)
{
   if (progressBar == nullptr)
    {
        return;
    }
    if (status == AkSlicer::Doing)
    {
        if (slicePercent<30)
        {
            if (step == AkSlicer::SliceStep::FileLoad )
            {
                slicePercent = percent;
            }
        }
        else
        {
            if (step == AkSlicer::SliceStep::Slice)
            {
                slicePercent = 30 + percent*0.7;
            }
        }

        if (slicePercent >= 100)
        {
            slicePercent = 99.9;
        }
        progressBar->setText(tr("Slicing"));
        progressBar->setValue(slicePercent);
        return;
    }
    

    else
    {
        progressBar->setText(tr("Slicing"));
    }
}

void FdmSlicer::doSlice()
{
    int buttonState = panel->getButtonState();
    if ((buttonState & ESliceBtnStat::MODEL_IS_SUSPEND_STATUS)
       && !(buttonState & ESliceBtnStat::GENERATE_SUPPORT_RESULT_T))
    {
        control::MessageDialog messageDialog(tr("Notice"), tr("The model is hovering in the air. Please generate support or switch on [place on bed]."),
            control::MessageDialog::OK);
        messageDialog.exec();
        return;
    }


    emit appendSliceBtnStat(ESliceBtnStat::SLICE_BEING);
    //emit sliceSucess();
    //return;

    

    slicer->resetParam();
    progressBar = nullptr;
    slicePercent = 0;
    

    //sliceBtn->setText("cancel");

//    progressBar = new AkProgressBar();
//    connect(progressBar, &AkProgressBar::cancel, slicer, &AkSlicer::cancel);
//    ctrlmanager->addProgressBar(progressBar);

    


    

    QTimer::singleShot(5, [&]{
        

        progressBar = new ProgressDialog();
        progressBar->setText(tr("Slicing"));
        connect(progressBar, &ProgressDialog::cancel, slicer, &AkSlicer::cancel);
        //connect(progressBar, &ProgressDialog::progressClosed, slicer, &AkSlicer::cancel);
        

        connect(progressBar, &ProgressDialog::destroyed, slicer, [&]()->void { progressBar = nullptr; });
        progressBar->exec();
    });

    QTimer::singleShot(50, [&]{
        

        saveStlFile();
        getCustomSetting();
    });

    

    QTimer::singleShot(100, [&] {

        //int percent = 0;
        while (nullptr == progressBar)
        {
            QThread::msleep(10);
        }
        setProgressBar(AkSlicer::SliceStep::FileLoad, AkSlicer::SliceStatus::Doing, 1, "");
        int maxCycle =20;
        int cycleTime  = 0;
        

        QString stlFile = slicer->getStlFile();
        while (stlFile.isEmpty())
        {
            QThread::msleep(10);
            stlFile = slicer->getStlFile();
            cycleTime++;
            if (cycleTime >maxCycle )
            {
                return;
            }
        }
        maxCycle =200;
        cycleTime = 0;
        qint64 preFileSize = -1;
        qint64 currentFileSize = -2;
        while (1)
        {
            QFileInfo fi(stlFile);
            currentFileSize = fi.size();
            if (preFileSize == currentFileSize)
            {
                break;
            }
            QThread::msleep(10);
            stlFile = slicer->getStlFile();
            preFileSize = currentFileSize;
            if (cycleTime >maxCycle)
            {
                currentFileSize = 500*1024;//500m
                break;
            }
        }

        //
        float timePerK = 0.2117;//0.1117
        float timeEstimate = currentFileSize/1024 * timePerK;
        int timePerStep = timeEstimate/30;
        if (timePerStep == 0)
        {
            timePerStep = 10;
        }
//        while (percent < 30)
//        {
//            percent++;
//            setProgressBar(AkSlicer::SliceStep::FileLoad, AkSlicer::SliceStatus::Doing, percent, "");
//            QThread::msleep(timePerStep);

//        }
        

        fileLoadPercent = 0;
        connect(timer,&QTimer::timeout,[&](){
            

            timer->stop();
            fileLoadPercent++;
            setProgressBar(AkSlicer::SliceStep::FileLoad, AkSlicer::SliceStatus::Doing, fileLoadPercent, "");
            if (fileLoadPercent < 30)
            {
                 timer->start();
            }
        });
        timer->start(timePerStep);

    });

    

    //progressBar = new ProgressDialog();
    //connect(progressBar, &ProgressDialog::cancel, slicer, &AkSlicer::cancel);
    

    //connect(progressBar, &ProgressDialog::destroyed, slicer, [&]()->void { progressBar = NULL; });
    //progressBar->exec();

}
void FdmSlicer::doSave()
{
    QString srcGcodeFile = this->sliceResult.gcodeFile;
    

    //    QString savePath = QFileDialog::getSaveFileName(NULL,"save","","*.gcode");
    //    QFile::copy(srcGcodeFile, savePath);

    

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_GCODE_PARSER;
    data.msg = AkConst::Msg::OFF_SCREEN_RENDER_EXPORT;
    data.map.insert(AkConst::Param::GCODE_FILE, srcGcodeFile);
    emit sendMsg2Manager(data);
}

void FdmSlicer::doPreviewBtnClicked()
{
    //
    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_GCODE_PARSER;
    data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
    data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
    data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false); 

    emit sendMsg2Manager(data);

}


void FdmSlicer::doPreview()
{
    auto gcodeFile = this->sliceResult.gcodeFile;
    auto originalStlName = this->sliceResult.originalStlName;
    if (gcodeFile.isEmpty())
    {
        return;
    }



//    QSet<QString> excludeSet;
//    QFileInfo gcodeInfo(gcodeFile);
//    excludeSet.insert(gcodeInfo.fileName());

    

    

    //slicer->clearGcode(excludeSet, 5*AkConst::Time::MINUTE_SECOND);

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_GCODE_PARSER;
    data.msg = AkConst::Msg::PREVIEW_GCODE;
    data.map.insert(AkConst::Param::GCODE_FILE, gcodeFile);
    data.map.insert(AkConst::Param::ORIGINAL_STL_NAME, originalStlName);
    

    emit sendMsg2Manager(data);
}

//----------communication-----------

void FdmSlicer::saveStlFile()
{
    

    QSet<QString> excludeSet;
    slicer->clearStl(excludeSet, 10*AkConst::Time::MINUTE_SECOND);
    


#if defined(__APPLE__) || defined (__GNUG__)
      long long threadId =(long long)(QThread::currentThread()->currentThreadId());
#else
     int threadId = int(QThread::currentThread()->currentThreadId());
#endif

//    QString timeStamp = QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");


    QString stlPath = slicer->getStlPath();
    QDir stlDir(stlPath);
//    QString destStlPath = stlDir.absoluteFilePath(timeStamp);

//    TDebug(QString("slice path is  %1 ").arg(destStlPath));

//    QDir destDir(destStlPath);
//    if (!destDir.exists())
//    {
//        destDir.mkpath(destStlPath);
//    }
    QDir destDir = IoApi::createTimestampFolderUnderPath(stlPath);

    QString stlFile = destDir.absoluteFilePath(QString("mesh%1.stl").arg(threadId));
    

    if (QFileInfo::exists(stlFile))
    {
        stlDir.remove(stlFile);
    }

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
    data.msg = AkConst::Msg::SAVE_STL_FILE;
    data.map.insert(AkConst::Param::STL_FILE, stlFile);

    

    emit sendMsg2Manager(data);
}

void FdmSlicer::getCustomSetting()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_SETTING;
    data.msg = AkConst::Msg::GET_CUSTOM_SETTING;
    //data.map.insert(AkConst::Param::STL_FILE, stlPath);

    

    emit sendMsg2Manager(data);
}



void FdmSlicer::trySlice(){

    if (!QFileInfo::exists(slicer->getStlFile()))
    {
        return;
    }
    if (slicer->getUserSetting().isEmpty())
    {
        return;
    }

    

    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_SLICER;
    data.dest = AkConst::Plugin::FDM_SETTING;
    data.msg = AkConst::Msg::SLICE_START;
    emit sendMsg2Manager(data);

    slicer->slice();



//    progressBar = new ProgressDialog();
//    connect(progressBar, &ProgressDialog::cancel, slicer, &AkSlicer::cancel);
//    //connect(progressBar, &ProgressDialog::progressClosed, slicer, &AkSlicer::cancel);


//    connect(progressBar, &ProgressDialog::destroyed, slicer, [&]()->void { progressBar = nullptr; });
//    progressBar->exec();
}



void FdmSlicer::recMsgfromManager(PluginMessageData metaData){
    if (metaData.from == AkConst::Plugin::AK_MAIN_WINDOW
            && metaData.msg == AkConst::Msg::SAVE_STL_FILE){
        QString stlPath = metaData.map.value(AkConst::Param::STL_FILE,QString("")).toString();
        QString OStlName = metaData.map.value(AkConst::Param::ORIGINAL_STL_NAME,QString("")).toString();
        if (!QFileInfo::exists(stlPath))
        {
            qDebug() << "stlPath not exist stlPath is " << stlPath;
            //emit sliceFailed();
            doSliceFailed(AkSliceInfo());
            return;
        }

        slicer->setStlFile(stlPath);
        slicer->setOriginalStlName(OStlName);

        int visibleMeshCount = metaData.map.value(AkConst::Param::VISIBLE_MESH_COUNT, QString("0")).toInt();
        slicer->setVisibleMeshCount(visibleMeshCount);
        //这里需要获取支撑数据sptMesh开头的stl
        QStringList filter;
        filter << "sptMesh*.stl";
        QFileInfo stlFileInfo(stlPath);
        QFileInfoList fInfos = stlFileInfo.dir().entryInfoList(filter, QDir::Files|QDir::Readable,QDir::Name);
        QStringList supportMeshes;
        for(int i =0;i<fInfos.size();i++)
        {
            supportMeshes.append(fInfos[i].absoluteFilePath());
        }
        slicer->setSupportMeshes(supportMeshes);
        trySlice();
    }
    

    if (metaData.from == AkConst::Plugin::FDM_SETTING
            && metaData.msg == AkConst::Msg::GET_CUSTOM_SETTING){
        QString setting = metaData.map.value(AkConst::Param::CUSTOM_SETTING,QString("")).toString();
        slicer->setUserSetting(setting);
        trySlice();
    }

    

    if (metaData.msg == AkConst::Msg::MODEL_STATUS_CHANGED
        || metaData.msg == AkConst::Msg::MODEL_TRANSFROM_CHANGED
        || metaData.msg == AkConst::Msg::MODEL_SUPPORT_NUMBER_CHANGED
        || metaData.msg == AkConst::Msg::PARAMETER_CHANGED){

        

        if (metaData.msg == AkConst::Msg::MODEL_STATUS_CHANGED){emit appendSliceBtnStat(ESliceBtnStat::MODEL_STATUS_CHANGED);}
        else if (metaData.msg == AkConst::Msg::MODEL_TRANSFROM_CHANGED){emit appendSliceBtnStat(ESliceBtnStat::MODEL_TRANSFROM_CHANGED);}
        else if (metaData.msg == AkConst::Msg::MODEL_SUPPORT_NUMBER_CHANGED){emit appendSliceBtnStat(ESliceBtnStat::MODEL_SUPPORT_NUMBER_CHANGED);}
        else if (metaData.msg == AkConst::Msg::PARAMETER_CHANGED){emit appendSliceBtnStat(ESliceBtnStat::PARAMETER_CHANGED);}

        PluginMessageData data;
        data.from = AkConst::Plugin::FDM_SLICER;
        data.dest = AkConst::Plugin::FDM_SETTING;
        data.msg = AkConst::Msg::SLICE_RESET;
        emit sendMsg2Manager(data);
    }

    bool generateSuppoteResult = metaData.map.value(AkConst::Param::GENERATE_SUPPORT_RESULT, false).toBool();
    bool modelSuspendResult = metaData.map.value(AkConst::Param::MODEL_SUSPEND_STATUS_RESULT, false).toBool();

    

    if (metaData.from == AkConst::Plugin::FDM_SETTING
        && metaData.msg == AkConst::Msg::GET_GENERATE_SUPPORT_STATUS_RESULT)
    {
        

        if (generateSuppoteResult)
        {
            

            emit appendSliceBtnStat(ESliceBtnStat::GENERATE_SUPPORT_RESULT_T);
        }
        else
        {

            emit appendSliceBtnStat(ESliceBtnStat::GENERATE_SUPPORT_RESULT_N);
        }
    }

    

    if (metaData.msg == AkConst::Msg::MODEL_SUSPEND_STATUS)
    {
        

        if (modelSuspendResult)
        {
            

            emit appendSliceBtnStat(ESliceBtnStat::MODEL_IS_SUSPEND_STATUS);
        }
        else
        {
            emit appendSliceBtnStat(ESliceBtnStat::MODEL_NOT_SUSPEND_STATUS);
        }
    }

    if (metaData.msg == AkConst::Msg::VISIBLE_MODEL_COUNT_CHANGED){
        int visibleModelCount = metaData.map.value(AkConst::Param::VISIBLE_MODEL_COUNT, 0).toInt();
        //bool enable = (visibleModelCount == 0) ? false : true;
        if (visibleModelCount == 0)
        {
            emit appendSliceBtnStat(ESliceBtnStat::VISIBLE_MODEL_COUNT_ZERO);
        }
        else
        {
            emit appendSliceBtnStat(ESliceBtnStat::VISIBLE_MODEL_COUNT_NOT_ZERO);
        }
    }

    if (metaData.msg == AkConst::Msg::MODEL_OUT_OF_RANGE_STATE_CHANGED){
        bool allInBox = metaData.map.value(AkConst::Param::MODEL_ALL_IN_PRINT_BOX, true).toBool();
        if (allInBox)
        {
            emit appendSliceBtnStat(ESliceBtnStat::MODEL_ALL_IN_BOX);
        }
        else
        {
            emit appendSliceBtnStat(ESliceBtnStat::MODEL_NOT_ALL_IN_BOX);
        }
    }

    if (metaData.msg == AkConst::Msg::SUPPORT_PROCESSING_START){
         emit appendSliceBtnStat(ESliceBtnStat::SUPPORT_PROCESSING_START);
    }
    if (metaData.msg == AkConst::Msg::SUPPORT_PROCESSING_END){
        emit appendSliceBtnStat(ESliceBtnStat::SUPPORT_PROCESSING_END);
    }
}

