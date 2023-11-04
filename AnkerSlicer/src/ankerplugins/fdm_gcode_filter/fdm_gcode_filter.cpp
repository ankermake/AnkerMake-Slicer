#include "fdm_gcode_filter.h"
#include "GCodeFilterControl.h"
#include <cassert>
#include <QTime>
RichParameterList* FdmGcodeFilter::s_globalParameterList = nullptr;
void FdmGcodeFilter::initialize(ControlInterface *, RichParameterList *globalParameterList){
    s_globalParameterList = globalParameterList;
}

void FdmGcodeFilter::recMsgfromManager(PluginMessageData msgBody){
    do{
        if(msgBody.dest != AkConst::Plugin::FDM_GCODE_FILTER){break;}

        QString gcodeFilePathRead  = msgBody.map.value(AkConst::Param::SOURCE_GCODE_FILE, QString("")).toString();
        QString gcodeFilePathWrite = msgBody.map.value(AkConst::Param::GCODE_FILE, QString("")).toString();

        if(msgBody.msg == AkConst::Msg::GET_GCODE_WITH_AI){
            //qDebug() << FUNC_LINE << msgBody;
            QTime time;time.start();

            GC::GCodeInfo gcodeInfo;
            {
                //  run  filter
                GCodeFilterControl gcodeFilterAI;
                gcodeFilterAI.StepAll(gcodeFilePathRead, gcodeFilePathWrite);
                gcodeInfo = gcodeFilterAI.getGcodeInfo();
            }

            //  send msg to FDM_GCODE_PARSER
            PluginMessageData metaData = msgBody;
            metaData.from = AkConst::Plugin::FDM_GCODE_FILTER;  //msgBody.dest;
            metaData.dest = AkConst::Plugin::FDM_GCODE_PARSER;  //msgBody.from;
            metaData.msg  = AkConst::Msg::PREVIEW_GCODE;

            metaData.map[AkConst::Param::GCODE_INFO] = QVariant::fromValue(gcodeInfo);

            if(metaData.map[AkConst::Param::GCODE_FILE].toString() != gcodeInfo.dstGcodeFilePath){
                assert( gcodeInfo.srcGcodeFilePath == gcodeInfo.dstGcodeFilePath );
                assert( gcodeInfo.srcGcodeFilePath == msgBody.map[AkConst::Param::SOURCE_GCODE_FILE].toString());
                metaData.map[AkConst::Param::GCODE_FILE] = msgBody.map[AkConst::Param::SOURCE_GCODE_FILE];
            }

            qDebug() << "========== filter run time: " << time.elapsed() << "ms ==========";
           // qDebug() << FUNC_LINE << metaData;
            //QTimer::singleShot(10, [this, metaData](){
                this->sendMsg2Manager(metaData);
            //});
        }

    }while(false);
}   //  end of recMsgfromManager
