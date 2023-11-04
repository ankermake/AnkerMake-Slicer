#include "fdm_gcode_parser.h"
#include "common/ak_const.h"
#include <numeric>
#include <QMainWindow>
#include <QTimer>
#include <QAction>
#include <QList>
#include <QMenu>
//#include <meshlab/glarea.h>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileDialog>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include "../common/controlInterface/messageDialog.h"
#include "../common/utilities/GCodeConstant.hpp"

FdmGcodeParser::FdmGcodeParser()
{
    preview  = new FdmGcodePreviewEntry();
    m_host = new QRemoteObjectHost(this);
    m_host->setHostUrl(QUrl("local:fdmRpc"));
    m_rpc = new fdmRpcWrapper(this);
    m_host->enableRemoting(m_rpc);
    connect(m_rpc,SIGNAL(linkNumChanged(int)),this,SLOT(setUseTimes(int)));


    m_host_inner = new QRemoteObjectHost(this);
    m_host_inner->setHostUrl(QUrl("local:fdmRpcInner"));
    //m_host->setHostUrl(QUrl("local:networkRpc"));
    m_rpc_inner = new fdmRpcWrapper(this);
    m_host_inner->enableRemoting(m_rpc_inner);
    
    connect(m_rpc,&fdmRpcWrapper::widChanged,[=](QVariant wid){
         qDebug() << "wid : " << wid.toULongLong();
         m_rpc->setPassParams(wid);
    });

    connect(m_rpc_inner,&fdmRpcWrapper::widChanged,[=](QVariant wid){

         qDebug() << "wid : " << wid;
         qDebug() << "this->preview->ww : " << this->preview->ww;
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
         data.msg = AkConst::Msg::GET_PREVIEW_WID;
         data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, wid);
         data.object = (QWidget *)wid.toULongLong();
         this->sendMsg2Manager(data);
         auto pass_params = getSceneParams();
         m_rpc_inner->setPassParams(pass_params);
    });
    connect(m_rpc_inner,&fdmRpcWrapper::cheeckoutPreview,[=](bool isC){
         qDebug() << "isC : " << isC;
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
         data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
         data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, isC);
         this->sendMsg2Manager(data);
    });
    connect(m_rpc_inner,&fdmRpcWrapper::previewOpenFile,[=](){
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
         data.msg = AkConst::Msg::PREVIEW_OPEN_FILE;
         this->sendMsg2Manager(data);
    });
    connect(m_rpc_inner,&fdmRpcWrapper::sendGCodePrint,[=](const QString &msg){
         qDebug() << "sendGCodePrint : " << msg;
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::FDM_NETWORK;
         data.msg = AkConst::Msg::A_KEY_PRINT;
         data.map.insert(AkConst::Param::A_KEY_PRINT_FILE_PATH, msg);
         QVariant maxSpeed = m_rpc_inner->property("maxSpeed");
         data.map.insert(AkConst::Param::MAX_PRINT_SPEED, maxSpeed);
         AkUtil::TDebug("sendGCodePrint: "+msg);
         this->sendMsg2Manager(data);
    });
    connect(m_rpc_inner,&fdmRpcWrapper::isAiModeChanged,[=](bool isAimode){
         //no change return
        qDebug()<< "fdmRpcWrapper::isAiModeChanged " <<isAimode;
        if(isAimode == _isAiMode){
            return ;
        }
         _isAiMode = isAimode;
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
         data.msg = AkConst::Msg::AIMODE_CHANGED;
         data.map.insert(AkConst::Param::AIMODE_STATE, isAimode);
         this->sendMsg2Manager(data);
    });

}


Q_DECLARE_METATYPE(QSurfaceFormat);
void FdmGcodeParser::recMsgfromManager(PluginMessageData metaData)
{
    if (metaData.msg == AkConst::Msg::PREVIEW_GCODE
        && (metaData.from == AkConst::Plugin::AK_MAIN_WINDOW | metaData.from == AkConst::Plugin::FDM_SLICER))
    {
    //send to filter
        if(CurMachine == ankerMachine::M5){
            QString soure_file = metaData.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
            if(soure_file.isEmpty()){
                reOpenGcodePreview(metaData);
                PluginMessageData data;
                data.from = AkConst::Plugin::FDM_GCODE_PARSER;
                data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
                data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
                data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false);
                this->sendMsg2Manager(data);
            }
            auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
            QString dirPath = writableLocation + "/AICamera";
            QFileInfo sFileInfo = QFileInfo(soure_file);
            QString des_file = dirPath + "/" +sFileInfo.fileName();
            QString searchString = ".gcode";
            QString replaceString = "_tmp.gcode";
            int index = des_file.lastIndexOf(searchString);
            if (index != -1) {
                des_file.replace(index, searchString.length(), replaceString);
            }
            PluginMessageData data;
            data.from = AkConst::Plugin::FDM_GCODE_PARSER;
            data.dest = AkConst::Plugin::FDM_GCODE_FILTER;
            data.msg = AkConst::Msg::GET_GCODE_WITH_AI;
            data.map.insert(AkConst::Param::SOURCE_GCODE_FILE, soure_file);
            data.map.insert(AkConst::Param::GCODE_FILE, des_file);
            QString originalStlName = metaData.map.value(AkConst::Param::ORIGINAL_STL_NAME, QString("")).toString();
            data.map.insert(AkConst::Param::ORIGINAL_STL_NAME, originalStlName);
            this->sendMsg2Manager(data);
        }

       if(CurMachine == ankerMachine::M5C){
           QString soure_file = metaData.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
           if(soure_file.isEmpty()){
               reOpenGcodePreview(metaData);
               PluginMessageData data;
               data.from = AkConst::Plugin::FDM_GCODE_PARSER;
               data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
               data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
               data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false);
               this->sendMsg2Manager(data);
           }
           auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
           QString dirPath = writableLocation + "/AICamera";
           QFileInfo sFileInfo = QFileInfo(soure_file);
           QString des_file = dirPath + "/" +sFileInfo.fileName();
           QString searchString = ".gcode";
           QString replaceString = "_tmp.gcode";
           int index = des_file.lastIndexOf(searchString);
           if (index != -1) {
               des_file.replace(index, searchString.length(), replaceString);
           }
           PluginMessageData data;
           data.from = AkConst::Plugin::FDM_GCODE_PARSER;
           data.dest = AkConst::Plugin::FDM_GCODE_FILTER;
           data.msg = AkConst::Msg::GET_GCODE_WITH_AI;
           data.map.insert(AkConst::Param::SOURCE_GCODE_FILE, soure_file);
           data.map.insert(AkConst::Param::GCODE_FILE, des_file);
           QString originalStlName = metaData.map.value(AkConst::Param::ORIGINAL_STL_NAME, QString("")).toString();
           data.map.insert(AkConst::Param::ORIGINAL_STL_NAME, originalStlName);
           this->sendMsg2Manager(data);
       }


    }


    if(metaData.msg == AkConst::Msg::PREVIEW_GCODE
        && (metaData.from == AkConst::Plugin::FDM_GCODE_FILTER))
    {//if form FDM_GCODE_FILTER  start to reOpenGcode
        reOpenGcodePreview(metaData);
        PluginMessageData data;
        data.from = AkConst::Plugin::FDM_GCODE_PARSER;
        data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
        data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
        data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false);
        this->sendMsg2Manager(data);

    }

    if (metaData.msg == AkConst::Msg::AIMODE_CHANGED
        && metaData.from == AkConst::Plugin::AK_MAIN_WINDOW)
    {

         _isAiMode = metaData.map.value(AkConst::Param::AIMODE_STATE).toBool();
         m_rpc_inner->setIsAiMode(_isAiMode);
    }

    if (metaData.msg == AkConst::Msg::OFF_SCREEN_RENDER_EXPORT
        && metaData.from == AkConst::Plugin::FDM_SLICER)
    {
        //exportMessageProcessing(metaData);

    }

    if(metaData.msg == AkConst::Msg::OPEN_PREVIEW_WITHOUT_FILE &&
          metaData.from == AkConst::Plugin::AK_MAIN_WINDOW )
    {
        this->openGcodePreviewWithoutFile();
    }

    
    if(metaData.msg == AkConst::Msg::OPEN_PREVIEW_IN_NETWORK &&
          metaData.from == AkConst::Plugin::FDM_NETWORK )
    {
        QString gcodeFile = metaData.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
        QString m_hostAdress = metaData.map.value(AkConst::Param::HOST_ADDRESS, QString("")).toString();
        this->openGcodePreviewInnetwork(gcodeFile, m_hostAdress);
    }

    if(metaData.msg == AkConst::Msg::GET_LOGGING_STATUS &&
            metaData.from == AkConst::Plugin::FDM_NETWORK)
    {
        bool status = metaData.map.value(AkConst::Param::LOGGING_STATUS).toBool();
        loggingStausChange(status);
    }
}

void FdmGcodeParser::setUseTimes(int ut)
{
    qDebug() << "ut : " << ut;
    useTimes = ut;
}


void FdmGcodeParser::loggingStausChange(bool status)
{
    if(this->m_rpc_inner != nullptr){
        m_rpc_inner->setLoggingStatus(status);
    }
}

void FdmGcodeParser::messageProcessing(PluginMessageData msgBody)
{
    AkUtil::TFunction("");
    
    if(useTimes > 10)
    {
        MessageDialog a(tr("Warning"), tr("Can't open more GCode preview widgets which numbers must be less than 10."), 0x00400000 );
        a.exec();
        return ;
    }

    
    QString gcodeFile = msgBody.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
    QString originalStlName = msgBody.map.value(AkConst::Param::ORIGINAL_STL_NAME, QString("")).toString();
    SceneParam iniSceneParam;
    if (globalParams != NULL)
    {
        const RichParameter& param = globalParams->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM);
        const Value& sceneValue = param.value();

        qDebug() << "messageProcessing m_printMachineBox: (" << sceneValue.getSceneParam().m_printMachineBox.m_length
                 << ", " << sceneValue.getSceneParam().m_printMachineBox.m_width
                 << ", " << sceneValue.getSceneParam().m_printMachineBox.m_height << ")";
        iniSceneParam = sceneValue.getSceneParam();
    }
    qRegisterMetaTypeStreamOperators<passSceneParam>("passSceneParam");
    passSceneParam test_in;
    test_in.m_eye = iniSceneParam.m_eye;
    test_in.m_front = iniSceneParam.m_front;
    test_in.m_up = iniSceneParam.m_up;
    test_in.m_backgroundColor = iniSceneParam.m_backgroundColor;
    test_in.m_printMachineBox = iniSceneParam.m_printMachineBox;
    qDebug() << "iniSceneParam.m_front" << iniSceneParam.m_front;
    qDebug() << "iniSceneParam.m_eye" << iniSceneParam.m_eye;
    qDebug() << "iniSceneParam.m_up" << iniSceneParam.m_up;
    qDebug() << "iniSceneParam.m_backgroundColor" << iniSceneParam.m_backgroundColor;
    qDebug() << "iniSceneParam.m_printMachineBox heigt " <<iniSceneParam.m_printMachineBox.m_height;
    qDebug() << "originalStlName " << originalStlName;
    QVariant testV ;
    testV.setValue(test_in);
    ++useTimes;
    m_rpc->setLinkNum(useTimes);
    m_rpc->setPassParams(testV);

    QSettings settings;
    bool  isAIMode = settings.value("AiMode").toBool();
//    pIn = new QProcess(this);
//    pInList.append(pIn);
    QStringList args;
    args << "noexe" << gcodeFile;
    args<<"-p"<<gcodeFile;
    args<<"-o"<<originalStlName;
    
    //args<<"--pmode";
    //args<<"--host"<< "local:fdmRpc";
    if(isAIMode)
    {
        args<<"--ai";
    }
    args<<"-n"<<QString::number(useTimes);
    preview->createView(args);
}

#include <QProgressDialog>
void FdmGcodeParser::reOpenGcodePreview(PluginMessageData msgBody)
{
    QString src_file = msgBody.map.value(AkConst::Param::SOURCE_GCODE_FILE, QString("")).toString();
    QString file = msgBody.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();

    //  Improve robustness add  @2023-05-08 by ChunLian
    QVariant gcodeInfoVar = msgBody.map.value(AkConst::Param::GCODE_INFO);
    bool bFiltterOk = false;
    GC::GCodeInfo gcodeInfo;
    if(gcodeInfoVar.canConvert<GC::GCodeInfo>()){
        gcodeInfo = gcodeInfoVar.value<GC::GCodeInfo>();
        if(gcodeInfo.srcGcodeFilePath.isEmpty() || gcodeInfo.dstGcodeFilePath.isEmpty()){
            bFiltterOk = false;
        }
        else{
            bFiltterOk = true;
            src_file = gcodeInfo.srcGcodeFilePath;
            file     = gcodeInfo.dstGcodeFilePath;
        }
    }

    bool isSameFile = checkOpenFile(file);
    if(isSameFile){
        PluginMessageData data;
        data.from = AkConst::Plugin::FDM_GCODE_PARSER;
        data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
        data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
        data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false);
        this->sendMsg2Manager(data);
        return ;
    }else{
        this->CurrentShowFile = file;
        QFileInfo cFileInfo(file);
        this->CurrentShowFileSize = cFileInfo.size();

        PluginMessageData data;
        data.from = AkConst::Plugin::FDM_GCODE_PARSER;
        data.dest = AkConst::Plugin::FDM_NETWORK;
        data.msg = AkConst::Msg::OPEN_NEW_GCODE;
        this->sendMsg2Manager(data);
    }
    QString originalStlName = msgBody.map.value(AkConst::Param::ORIGINAL_STL_NAME, QString("")).toString();

    m_rpc_inner->pubMsgFromFdmGcodePaser(file);
    m_rpc_inner->setOStlName(originalStlName);
    auto pass_params = getSceneParams();
    m_rpc_inner->setPassParams(pass_params);
    m_rpc_inner->setAnkerMachine(static_cast<int>(this->CurMachine));

    m_rpc_inner->setOStlGcodePath("null_name");

    if(bFiltterOk){
        QString ostGcodePath = src_file;
        qDebug()<< "ostGcodePath "<<ostGcodePath;
        GC::EGeneratedByProducer curproducer = gcodeInfo.producer;
        auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QString dirPath = writableLocation + "/AICamera";
        QFileInfo sFileInfo = QFileInfo(ostGcodePath);
        QString des_file = dirPath + "/" +sFileInfo.fileName();
        QString searchString = ".gcode";
        QString replaceString = "_source_tmp.gcode";
        int index = des_file.lastIndexOf(searchString);
        if (index != -1) {
            des_file.replace(index, searchString.length(), replaceString);
        }
        QFile::copy(ostGcodePath, des_file);
        qDebug()<<"des_file" <<des_file;
        m_rpc_inner->setOStlGcodePath(des_file);
    }else{
        AkUtil::TDebug("filtter pass fail!");
    }

    queryLoggingStatus();
    m_pDlg = new ProgressDialog(nullptr);//(QDialog*)this
    connect(m_pDlg, &ProgressDialog::destroyed, this, [&]()->void { m_pDlg = nullptr; });
    m_pDlg->setText(tr("Load G-Code"));
    m_pDlg->setCancelVisible(false);
    connect(preview->ww->getGcodeView(),SIGNAL(setValue(int)),m_pDlg,SLOT(setValue(int)));
    m_pDlg->exec();
}

void FdmGcodeParser::openGcodePreviewWithoutFile()
{
    QSettings settings;
    bool  isAIMode = settings.value("AiMode").toBool();
    QStringList args;
    args << "noexe";
    
    args<<"--imode";
    args<<"--host"<<"local:fdmRpcInner";
    if(isAIMode)
    {
        args<<"--ai";
    }
    preview->createView(args);
    m_rpc_inner->setIsAiMode(isAIMode);
    m_rpc_inner->setAnkerMachine(static_cast<int>(this->CurMachine));
}


void FdmGcodeParser::openGcodePreviewInnetwork(const QString& file,const QString& m_hostAdress)
{
    FdmGcodePreviewEntry* previewNetwork = nullptr;
    if(previewNetwork == nullptr)
    {
        previewNetwork = new FdmGcodePreviewEntry();
    }
    QStringList args;
    args << "noexe";
    args<< "--pmode";
    args<<"-p"<<file;
    args<<"--host"<<m_hostAdress;
    previewNetwork->createView(args);
    previewNetworkList.append(previewNetwork);

}

bool FdmGcodeParser::checkOpenFile(QString gcodePath)
{
    QFileInfo cFileInfo(gcodePath);
    if(QString::compare(this->CurrentShowFile,gcodePath) == 0  && CurrentShowFileSize == cFileInfo.size() && preview->ww->checkLastFileComplete()){
        return true;
    }
    return false;
}

void FdmGcodeParser::queryLoggingStatus()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_GCODE_PARSER;
    data.dest = AkConst::Plugin::FDM_NETWORK;
    data.msg = AkConst::Msg::GET_LOGGING_STATUS;
    this->sendMsg2Manager(data);
}

QVariant FdmGcodeParser::getSceneParams()
{
    SceneParam iniSceneParam;
    if (globalParams != NULL)
    {
        if(globalParams->hasParameter(AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM))
        {
            const RichParameter& param = globalParams->getParameterByName(AkConst::GlobalParameterKeys::ANKER_SCENE_PARAM);
            const Value& sceneValue = param.value();

            qDebug() << "getSceneParams m_printMachineBox: (" << sceneValue.getSceneParam().m_printMachineBox.m_length
                     << ", " << sceneValue.getSceneParam().m_printMachineBox.m_width
                     << ", " << sceneValue.getSceneParam().m_printMachineBox.m_height << ")";
            iniSceneParam = sceneValue.getSceneParam();
            qDebug() << "logoMesh vert size: " << iniSceneParam.logoMesh.vert.size();
            iniSceneParam.logo.points.clear();
            iniSceneParam.logo.nors.clear();
            iniSceneParam.logo.faces.clear();
            iniSceneParam.logo.points.resize(iniSceneParam.logoMesh.vert.size());
            iniSceneParam.logo.nors.resize(iniSceneParam.logoMesh.vert.size());
            iniSceneParam.logo.faces.resize(iniSceneParam.logoMesh.face.size());
            int i = 0;
            for(vcgTriMesh::VertexIterator it = iniSceneParam.logoMesh.vert.begin(); it != iniSceneParam.logoMesh.vert.end(); it++)
            {
                iniSceneParam.logo.points[i][0] = it->P().X();
                iniSceneParam.logo.points[i][1] = it->P().Y();
                iniSceneParam.logo.points[i][2] = it->P().Z();

                iniSceneParam.logo.nors[i][0] = it->N().X();
                iniSceneParam.logo.nors[i][1] = it->N().Y();
                iniSceneParam.logo.nors[i][2] = it->N().Z();
                i++;
            }

            int p = 0;
            for(vcgTriMesh::FaceIterator it = iniSceneParam.logoMesh.face.begin(); it != iniSceneParam.logoMesh.face.end(); it++)
            {
                iniSceneParam.logo.faces[p].m_index1 = it->V(0) - iniSceneParam.logoMesh.vert.data();
                iniSceneParam.logo.faces[p].m_index2 = it->V(1) - iniSceneParam.logoMesh.vert.data();
                iniSceneParam.logo.faces[p].m_index3 = it->V(2) - iniSceneParam.logoMesh.vert.data();
                p++;
            }
            iniSceneParam.logo.trans = iniSceneParam.logoMesh.m_trans;
        }
        else
        {
            iniSceneParam.m_printMachineBox.m_color = QColor(0.313725 * 255, 0.313725 * 255, 0.313725 * 255,1);
            iniSceneParam.m_printMachineBox.m_height = 250;
            iniSceneParam.m_printMachineBox.m_length = 235;
            iniSceneParam.m_printMachineBox.m_width = 235;
            iniSceneParam.m_printMachineBox.num = 30;
            iniSceneParam.m_printMachineBox.m_lineWidth = 1;
            iniSceneParam.m_eye = QVector3D(0, 0, 0);
            iniSceneParam.m_front = QVector3D(0, 0, 0);
            iniSceneParam.m_up = QVector3D(0, 0, 0);
        }
    }
    qRegisterMetaTypeStreamOperators<passSceneParam>("passSceneParam");
    passSceneParam pSP;
    pSP.m_eye = iniSceneParam.m_eye;
    pSP.m_front = iniSceneParam.m_front;
    pSP.m_up = iniSceneParam.m_up;
    pSP.m_backgroundColor = iniSceneParam.m_backgroundColor;
    pSP.m_printMachineBox = iniSceneParam.m_printMachineBox;
    pSP.logo = iniSceneParam.logo;
    QVariant pspV ;
    pspV.setValue(pSP);
    return pspV;
}

FdmGcodeParser::~FdmGcodeParser()
{
    AkUtil::TFunction("");
    if (preview != nullptr)
    {
        AkUtil::TDebug("~inner preview");
        delete preview;
        preview = nullptr;
    }

    for (auto it = previewNetworkList.begin();it != previewNetworkList.end();it++)
    {
        if ((*it) != nullptr)
        {
            delete *it;
            *it = nullptr;
        }
    }
};

void FdmGcodeParser::initialize(ControlInterface *controlmanager, RichParameterList *globalParameterList) {
    qDebug() << "controlmanager " <<controlmanager;
    if (globalParameterList)
        globalParams = globalParameterList;
    else
        return;

    {
        const RichParameter& param = globalParams->getParameterByName(AkConst::SettingKey::MACHINE_NAME);
        QObject::connect(param.qobj.data(),  &RichParameterQObject::valueChange,[&param, this](const Value& value){
            this->_machineType = value.getString();
            if(this->_machineType==AkConst::MachineName::M5){
                this->CurMachine =  ankerMachine::M5;
                if(m_rpc_inner != nullptr)
                {
                    m_rpc_inner->setAnkerMachine(static_cast<int>(this->CurMachine));
                }

            }

            if(AkConst::MachineName::M5C_SERIES.contains(this->_machineType)){
                this->CurMachine =  ankerMachine::M5C;
                if(m_rpc_inner != nullptr)
                {
                    m_rpc_inner->setAnkerMachine(static_cast<int>(this->CurMachine));
                }
            }
            qDebug() << __FUNCTION__ << __LINE__ << param.name() << value.getString();
        });
    }

//    {
//        const RichParameter& param = globalParams->getParameterByName(AkConst::SettingKey::MACHINE_AI_CAMERA);
//        QObject::connect(param.qobj.data(),  &RichParameterQObject::valueChange,[&param, this](const Value& value){
//            //maybe need't to get value friva
//            // this->_isAiMode = value.getBool();
//            qDebug() << __FUNCTION__ << __LINE__ << param.name() << value.getBool();
//        } );
//    }

}


