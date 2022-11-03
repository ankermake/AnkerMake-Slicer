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
//class IgnoreEvent :public QObject
//{
//public:
//    IgnoreEvent(QObject* obj)
//    {
//        m_obj = obj;
//        m_obj->installEventFilter(this);
//    }
//    ~IgnoreEvent()
//    {
//        m_obj->removeEventFilter(this);
//    }
//    bool eventFilter(QObject *obj, QEvent *event)
//    {
//        event->ignore();
//        return true;
//        return QObject::eventFilter(obj, event);
//    }
//private:
//    QObject* m_obj;
//};

FdmGcodeParser::FdmGcodeParser()
{
    preview  = new FdmGcodePreviewEntry();
    m_host = new QRemoteObjectHost(this);
    m_host->setHostUrl(QUrl("local:fdmRpc"));
    //m_host->setHostUrl(QUrl("local:networkRpc"));
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
         //data.object = this->preview->ww;
         data.object = (QWidget *)wid.toULongLong();
         this->sendMsg2Manager(data);
         m_rpc_inner->setPassParams(wid);
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
            //messageProcessing(metaData);
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
        //exportMessageProcessing(metaData);
    }

    if (metaData.msg == AkConst::Msg::AIMODE_CHANGED
        && metaData.from == AkConst::Plugin::AK_MAIN_WINDOW)
    {

//        if(pInner == nullptr)
//        {
//            return ;
//        }
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


}

void FdmGcodeParser::setUseTimes(int ut)
{
    qDebug() << "ut : " << ut;
    useTimes = ut;
}

void FdmGcodeParser::messageProcessing(PluginMessageData msgBody)
{
    AkUtil::TFunction("");
    
    if(useTimes > 10)
    {
        MessageDialog a("warning", "can't open more gcode preview widgt whice numbers must less than 10 ", 0x00400000 );
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

    //FdmGcodePreviewEntry *preview = new FdmGcodePreviewEntry();
    //FdmGcodePreviewEntry* preview = nullptr;
    preview->createView(args);

//#ifdef __APPLE__
//    qDebug() << "fdm_gcode_preview.app/Contents/MacOS/fdm_gcode_preview"<<args;
//    QString curPath = QCoreApplication::applicationDirPath();
//    curPath = curPath + "/fdm_gcode_preview";
//    pIn->start(curPath,args);


//#else
//    qDebug() << "fdm_gcode_preview"<<args;
//    //pIn->start("fdm_gcode_preview",args);


//    //std::vector<char*> vc;
//    //for (int i = 0; i < args.size(); i++)
//    //{
//    //    int x = args[i].size();
//    //    char* pStr = new char[args[i].size() + 1];
//    //    std::strcpy(pStr, args[i].toStdString().c_str());
//    //    vc.push_back(pStr);
//    //}
//    //preview->createView(vc.size(),vc.data());
//    preview->createView(args);

//#endif

//    m_pDlg = new ProgressDialog(nullptr);//(QDialog*)this
//    IgnoreEvent ign(this);
//    //boolLock orL(offRenderLock);
//    m_pDlg->setText(tr("load GCode file"));
//    m_pDlg->setCancelVisible(false);
//    connect(m_rpc,SIGNAL(progress(int)),m_pDlg,SLOT(setValue(int)));
//    m_pDlg->exec();

}

#include <QProgressDialog>
void FdmGcodeParser::reOpenGcodePreview(PluginMessageData msgBody)
{
    qDebug() << "enter FdmGcodeParser::reOpenGcodePreview";
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
    }
    QString originalStlName = msgBody.map.value(AkConst::Param::ORIGINAL_STL_NAME, QString("")).toString();
    
        m_rpc_inner->pubMsgFromFdmGcodePaser(file);
        m_rpc_inner->setOStlName(originalStlName);
        qDebug() << "preview->ww windId:" <<preview->ww->winId();
        //qDebug() << "preview->ww windowHandle:" <<preview->ww->windowHandle();
        m_pDlg = new ProgressDialog(nullptr);//(QDialog*)this
        connect(m_pDlg, &ProgressDialog::destroyed, this, [&]()->void { m_pDlg = nullptr; });
        //IgnoreEvent ign(preview->ww);
        //boolLock orL(offRenderLock);
        m_pDlg->setText(tr("load GCode file"));
        m_pDlg->setCancelVisible(false);
        connect(preview->ww->getGcodeView(),SIGNAL(setValue(int)),m_pDlg,SLOT(setValue(int)));
        m_pDlg->exec();

}

void FdmGcodeParser::openGcodePreviewWithoutFile()
{
    QSettings settings;
    bool  isAIMode = settings.value("AiMode").toBool();
//    pInner = new QProcess(nullptr);
    QStringList args;
    args << "noexe";
    args<<"--imode";
    args<<"--host"<<"local:fdmRpcInner";
    if(isAIMode)
    {
        args<<"--ai";
    }
    //args<<"--pmode";
    //pInner->start("fdm_gcode_preview",args);
    
   
    //std::vector<char*> vc;
    //for (int i = 0; i < args.size(); i++)
    //{
    //    int x = args[i].size();
    //    char* pStr = new char[args[i].size() + 1];
    //    std::strcpy(pStr, args[i].toStdString().c_str());
    //    vc.push_back(pStr);
    //}
    //preview->createView(vc.size(),vc.data());
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
    if(QString::compare(this->CurrentShowFile,gcodePath) == 0){
        return true;
    }
    return false;
}

FdmGcodeParser::~FdmGcodeParser()
{
//    for (auto it = pInList.begin();it != pInList.end();it++)
//    {
//        QProcess* t =(*it);
//        t->terminate();
//        QCoreApplication::processEvents(QEventLoop::AllEvents);
//    }
//    for (auto it = pInList.begin();it != pInList.end();it++)
//    {
//        QProcess* t =(*it);
//        t->waitForFinished();
//        QCoreApplication::processEvents(QEventLoop::AllEvents);
//    }


//    pInner->deleteLater();
//    pInner = nullptr;

    if (preview != nullptr)
    {
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


//void FdmGcodeParser::exportMessageProcessing(PluginMessageData msgBody)
//{
//    AkUtil::TFunction("");
//    QString savePath = QFileDialog::getSaveFileName(NULL, "save", "", "*.gcode");
//    if (savePath.isEmpty())
//    {
//        return;
//    }
//    QSettings settings;
//    AkUtil::TDebug( QString("FdmSlicer plugins aI mode  = %1").arg(settings.value("AiMode").toBool()));

//    QString gcodeFile = msgBody.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();
//    AkUtil::TDebug(QString("gcodeFile " )+gcodeFile);

//    this->processor->reset();
//    this->processor->enable_producers(true);
//    this->processor->process_file(gcodeFile.toStdString(), false);
//    std::unique_ptr<GcodeViewer> offerRenderViewr = std::make_unique<GcodeViewer>(nullptr);
//    offerRenderViewr->setIsTRender(false);
//    offerRenderViewr->gcode_result = std::move(this->processor->extract_result());
//    offerRenderViewr->offerContext();
//    ProgressDialog progressDlg(nullptr);
//    connect(offerRenderViewr.get(),SIGNAL(setValue(int)),&progressDlg,SLOT(setValue(int)));
//    progressDlg.setText(tr("gen ai pic"));
//    progressDlg.setCancelVisible(false);
//    QTimer::singleShot(100, this, [&,savePath](){
//               offerRenderViewr->off_render(savePath,settings.value("AiMode").toBool());
//            });
////    offerRenderViewr->off_render(savePath);

//    QRegularExpression re(".+(?=.gcode)");
//    QRegularExpressionMatch match = re.match(savePath);
//    QString gcodeSave;
//    if (match.hasMatch())
//    {
//        gcodeSave = match.captured(0);
//        gcodeSave += ".gcode";
//        bool IsTrue = QFile::exists(gcodeSave);
//        if(IsTrue)
//        {
//            QFile::remove(gcodeSave);
//        }
//        QFile::copy(gcodeFile, gcodeSave);
//    }

//    progressDlg.exec();
//}
