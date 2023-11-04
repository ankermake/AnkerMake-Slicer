#include "messageprocessing.h"
//#include "../mainwindow.h"
#include "common/GeoAndShow/CHScene.h"
#include <common/utilities/tlogger.h>


void MessageProcessing::recMsgfromManager(PluginMessageData metadata)
{
    //Q_ASSERT(AkConst::Plugin::AK_MAIN_WINDOW == metadata.dest);

    if(AkConst::Msg::SAVE_STL_FILE == metadata.msg){

        bool suc = processingWriteAllMeshModelsToStlFile(metadata);

        
        PluginMessageData response = metadata;
        response.from = metadata.dest;
        response.dest = metadata.from;
        
        if (!suc)
        {
            response.map.insert(AkConst::Param::STL_FILE, QString(""));
            response.map.insert(AkConst::Param::ORIGINAL_STL_NAME, QString(""));
        }
        emit sendMsg2Manager(response);

        return;
    }
    if(AkConst::Msg::GET_PREVIEW_WID == metadata.msg){
      QVariant wid = metadata.map.value(AkConst::Param::GCODE_PREVIEW_WID);
      qDebug()<<"wid " <<wid;
      emit sendMsg2GetpreviewWid(metadata.object);
    }
    if(AkConst::Msg::CHECKOUT_PREVIEW == metadata.msg){
      bool iSC = metadata.map.value(AkConst::Param::GCODE_PREVIEW_WID).toBool();
      //qDebug()<<"--iSC " <<iSC;
      emit checkoutPreview(iSC);
    }

    if(AkConst::Msg::AUTO_UPDATE == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::MANUAL_UPDATE == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::START_AUTO_CHECK_SERVER_VERSION == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::CANCEL_AUTO_CHECK_SERVER_VERSION == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::CHECK_UPDATE_TEXT == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::HAVE_NEW_VERSION == metadata.msg){
        emit sendMsg2Update(metadata);
    }
    if(AkConst::Msg::A_KEY_PRINT == metadata.msg){

    }
    if(AkConst::Msg::NETWORK_MAINWINDOW_WID == metadata.msg)
    {
        //int wid = metadata.map.value(AkConst::Param::NETWORK_MAINWINDOW_WID).toInt();
        //emit networkSendWid2AnkerMake(wid);
        emit networkSendWid2AnkerMake(metadata.object);
    }
    if(AkConst::Msg::SEND_LOGIN_WIDGET_PTR == metadata.msg){
       AkUtil::TDebug("process SEND_LOGIN_WIDGET_PTR ...");
       emit networkSendLoginWidgetPtr(metadata.object);
    }
    if(AkConst::Msg::SWITCH_NETWORK_WIDGET == metadata.msg){
        emit swtichNetworkWidgetSignal();
    }
    if(AkConst::Msg::NETWORK_DEAD == metadata.msg){
        emit networkDeadSig();
    }
    if(AkConst::Msg::VIDEO_STATUS == metadata.msg){
        // AkUtil::TDebug("VIDEO_STATUS.");
        bool status = metadata.map.value(AkConst::Param::VIDEO_IS_PLAYING).toBool();
        emit videoStatusChangeSig(status);
    }
    if(AkConst::Msg::ANKERMAKE_MAINWINDOW_MODAL == metadata.msg){
        AkUtil::TDebug("mainwindowModalSig.");
        emit mainwindowModalSig(metadata.map.find(AkConst::Msg::ANKERMAKE_MAINWINDOW_MODAL).value().toInt());
    }

    if(AkConst::Msg::PREVIEW_OPEN_FILE == metadata.msg){
        emit fdmOpenPreviewFile();
    }

    if(AkConst::Msg::AIMODE_CHANGED == metadata.msg){
        bool status = metadata.map.value(AkConst::Param::AIMODE_STATE).toBool();
        emit aiModeChanged(status);
    }
}


void MessageProcessing::receiveModelObjsStatusChangedFromDoc()
{
	PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
	data.dest = AkConst::Plugin::FDM_SLICER;
	data.msg = AkConst::Msg::MODEL_STATUS_CHANGED;
	emit sendMsg2Manager(data);
}

void MessageProcessing::receiveModelSupportNumberChangedFromDoc(int sptCount)
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_SLICER;
    data.msg = AkConst::Msg::MODEL_SUPPORT_NUMBER_CHANGED;
    data.map.insert(AkConst::Param::SUPPORT_MESH_NUMBER, sptCount);
    emit sendMsg2Manager(data);

    PluginMessageData data2;
    data2.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data2.dest = AkConst::Plugin::FDM_SETTING;
    data2.msg = AkConst::Msg::MODEL_SUPPORT_NUMBER_CHANGED;
    data2.map.insert(AkConst::Param::SUPPORT_MESH_NUMBER, sptCount);
    emit sendMsg2Manager(data2);
}


void MessageProcessing::receiveModelSuspendStatusChangedFromDoc(bool susStatus)
{

    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_SLICER;
    data.msg = AkConst::Msg::MODEL_SUSPEND_STATUS;
    data.map.insert(AkConst::Param::MODEL_SUSPEND_STATUS_RESULT, susStatus);
    emit sendMsg2Manager(data);


}


//void MessageProcessing::receiveGenerateSupportButtonStatusFromDoc()
//{
//    PluginMessageData data;
//    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
//    data.dest = AkConst::Plugin::FDM_SETTING;
//    data.msg = AkConst::Msg::GET_GENERATE_SUPPORT_STATUS;
//    emit sendMsg2Manager(data);
//}

void MessageProcessing::sendMsg2Preview()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_GCODE_PARSER;
    data.msg = AkConst::Msg::OPEN_PREVIEW_WITHOUT_FILE;
    qDebug()<<"sendMsg2Preview";
    emit sendMsg2Manager(data);

}

void MessageProcessing::sendMsg2NetworkAKeyPrint(const QString &filePath)
{
    PluginMessageData data;
    data.from = AkConst::Plugin::FDM_GCODE_PARSER;
    data.dest = AkConst::Plugin::FDM_NETWORK;
    data.msg =  AkConst::Msg::A_KEY_PRINT;
    data.map.insert(AkConst::Param::A_KEY_PRINT_FILE_PATH, filePath);
    emit sendMsg2Manager(data);
}

void MessageProcessing::sendMsg2NetworkLogin()
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_NETWORK;
    data.msg =  AkConst::Msg::ANKERMAKE_LOGIN;
    //data.map.insert(AkConst::Param::ANKERMAKE_MAINWINDOW_WID, wid);
    emit sendMsg2Manager(data);
}

void MessageProcessing::receiveModelTransformChangedFromDoc()
{
	PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
	data.dest = AkConst::Plugin::FDM_SLICER;
    data.msg = AkConst::Msg::OPEN_PREVIEW_WITHOUT_FILE;
	emit sendMsg2Manager(data);
}

void MessageProcessing::receiveModelOutOfRangeChangedFromDoc(bool allInPrintBox)
{
	PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
	data.dest = AkConst::Plugin::FDM_SLICER;
	data.msg = AkConst::Msg::MODEL_OUT_OF_RANGE_STATE_CHANGED;
	data.map.insert(AkConst::Param::MODEL_ALL_IN_PRINT_BOX, allInPrintBox);
	emit sendMsg2Manager(data);
}

void MessageProcessing::receiveVisibleModelCountChangedFromDoc(int modelCount)
{
    PluginMessageData data;
    data.from = AkConst::Plugin::AK_MAIN_WINDOW;
    data.dest = AkConst::Plugin::FDM_SLICER;
    data.msg = AkConst::Msg::VISIBLE_MODEL_COUNT_CHANGED;
    data.map.insert(AkConst::Param::VISIBLE_MODEL_COUNT, modelCount);
    emit sendMsg2Manager(data);
}

bool MessageProcessing::processingWriteAllMeshModelsToStlFile(PluginMessageData& metadata)
{
//    auto getMeshDoc = [this]() -> MeshDocument*
//    {
//        MainWindow * maiWindow = qobject_cast<MainWindow *>(parent());
//        if(!maiWindow){return nullptr;}
//        MultiViewer_Container *multiViewer_Container = maiWindow->currentViewContainer();
//        if(!multiViewer_Container){return nullptr;}
//        return &multiViewer_Container->meshDoc;
//    };

//    auto writeAllMeshModelsToStlFile = [](MeshDocument* meshDocPtr, QString path, bool binary) ->bool
//    {
//        //return false;
//        return meshDocPtr->writeAllMeshModelsToStlFile(path, binary);
//    };

    QString stlPath = metadata.map.value(AkConst::Param::STL_FILE,QString("")).toString();
    QString originalStlName = metadata.map.value(AkConst::Param::ORIGINAL_STL_NAME,QString("")).toString();
    //qDebug() <<"getDoc()->m_printObjs.size() " << getDoc()->m_printObjs.size();
    if(getDoc()->m_printObjs.size() > 0)
    {
        originalStlName = (*getDoc()->m_printObjs.begin())->getObjectName();
        metadata.map.insert(AkConst::Param::ORIGINAL_STL_NAME, originalStlName);
    }
    //qDebug() <<"originalStlName" << originalStlName;
    //return getMeshDoc()->writeAllMeshModelsToStlFile(stlPath,true);
    //return getDoc()->writeAllMeshModelsToStlFile(stlPath, true);
    //return getDoc()->writeVisibleMeshesToStlFile(stlPath, true);
    //return writeAllMeshModelsToStlFile(getMeshDoc(), stlPath, true);

    int visibleMeshCount= 0;
    bool writeSuc =  getDoc()->writeVisibleMeshesToStlFile(stlPath,visibleMeshCount, true);
    metadata.map.insert(AkConst::Param::VISIBLE_MESH_COUNT, visibleMeshCount);
    return writeSuc;
}
