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
#include <QFileDialog>
#include <QRegularExpression>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include "../common/controlInterface/messageDialog.h"


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
    connect(m_rpc_inner,&fdmRpcWrapper::sendGCodePrint,[=](const QString &msg){
         qDebug() << "sendGCodePrint : " << msg;
         PluginMessageData data;
         data.from = AkConst::Plugin::FDM_GCODE_PARSER;
         data.dest = AkConst::Plugin::FDM_NETWORK;
         data.msg = AkConst::Msg::A_KEY_PRINT;
         data.map.insert(AkConst::Param::A_KEY_PRINT_FILE_PATH, msg);
         this->sendMsg2Manager(data);
    });

}


Q_DECLARE_METATYPE(QSurfaceFormat);
void FdmGcodeParser::recMsgfromManager(PluginMessageData metaData)
{
    if (metaData.msg == AkConst::Msg::PREVIEW_GCODE
        && (metaData.from == AkConst::Plugin::AK_MAIN_WINDOW | metaData.from == AkConst::Plugin::FDM_SLICER))
    {
        if(metaData.from == AkConst::Plugin::AK_MAIN_WINDOW){
            reOpenGcodePreview(metaData);
            PluginMessageData data;
            data.from = AkConst::Plugin::FDM_GCODE_PARSER;
            data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
            data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
            data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false); 
            this->sendMsg2Manager(data);

        }else{
            
            reOpenGcodePreview(metaData);
            PluginMessageData data;
            data.from = AkConst::Plugin::FDM_GCODE_PARSER;
            data.dest = AkConst::Plugin::AK_MAIN_WINDOW;
            data.msg = AkConst::Msg::CHECKOUT_PREVIEW;
            data.map.insert(AkConst::Param::GCODE_PREVIEW_WID, false); 
            this->sendMsg2Manager(data);
        }
    }

    if (metaData.msg == AkConst::Msg::AIMODE_CHANGED
        && metaData.from == AkConst::Plugin::AK_MAIN_WINDOW)
    {

         bool _isAiMode = metaData.map.value(AkConst::Param::AIMODE_STATE).toBool();
         qDebug() << "aimode --------:" <<_isAiMode ;
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
        MessageDialog a(tr("Warning"), tr("can't open more gcode preview widgt whice numbers must less than 10."), 0x00400000 );
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

        qDebug() << "m_printMachineBox: (" << sceneValue.getSceneParam().m_printMachineBox.m_length
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
    QString file = msgBody.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
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
        qDebug() << "pass_params: " << pass_params;
        m_rpc_inner->setPassParams(pass_params);
        
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

            qDebug() << "m_printMachineBox: (" << sceneValue.getSceneParam().m_printMachineBox.m_length
                     << ", " << sceneValue.getSceneParam().m_printMachineBox.m_width
                     << ", " << sceneValue.getSceneParam().m_printMachineBox.m_height << ")";
            iniSceneParam = sceneValue.getSceneParam();
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


