#ifndef GCODEFILTERCONTROL_H
#define GCODEFILTERCONTROL_H
#include "common/utilities/GCodeConstant.hpp"
#include "QFileMemoryMap.h"
#include "GCodeFilter_AIBase.h"
#include "GCodeFilter_Color.h"

//factory
class GCodeFilterControl
{
public:
    GCodeFilterControl(){}

    virtual ~GCodeFilterControl(){
        if (nullptr != gCodeFilterAIImpl)
        {
            delete  gCodeFilterAIImpl;
            gCodeFilterAIImpl = nullptr;
        }
        if (nullptr != gCodeFilterAIImpl)
        {
            delete  gCodeFilterAIImpl;
            gCodeFilterAIImpl = nullptr;
        }
    }

    bool StepAll(const QString & srcGcode, const QString & dstGcode){
        bool OkAndGoing = true, OkAndGoing1 = true, OkAndGoing2 = true;
        OkAndGoing = OkAndGoing && Step0_InitFileIO(srcGcode, dstGcode);
        OkAndGoing = OkAndGoing && Step1_GCodeInfo_Init();


        if(OkAndGoing && Step2_AiFilter_IsNeed()){
            OkAndGoing1 = OkAndGoing1 && Step3_AiFilter_Create ();
            OkAndGoing1 = OkAndGoing1 && Step4_AiFilter_Running();
        }
        else{
            OkAndGoing1 = false;
        }

        if(OkAndGoing && Step2_ColorFilter_IsNeed()){
            OkAndGoing2 = OkAndGoing2 && Step3_ColorFilter_Create ();
            OkAndGoing2 = OkAndGoing2 && Step4_ColorFilter_Running();
        }
        else{
            OkAndGoing2 = false;
        }

        (OkAndGoing && (OkAndGoing1 || OkAndGoing2)) ? Step9_ReGeneratedByAnkerMake() : Step9_UsingRawFile();

        if(true){
            qDebug() << "srcGcodeFilePath =" << gcodeInfo.srcGcodeFilePath;
            qDebug() << "dstGcodeFilePath =" << gcodeInfo.dstGcodeFilePath;

            qDebug() << "newLine   =" << gcodeInfo.newLine  ;
            qDebug() << "producer  =" << gcodeInfo.producer ;
            qDebug() << "hasAI     =" << gcodeInfo.hasAI    ;
            qDebug() << "hasThm    =" << gcodeInfo.hasThm   ;
            qDebug() << "errorStr  =" << gcodeInfo.errorStr ;
        }
        return OkAndGoing;
    }

    bool Step0_InitFileIO(const QString & srcGcode, const QString & dstGcode);
    bool Step1_GCodeInfo_Init();

    bool Step2_AiFilter_IsNeed();
    bool Step3_AiFilter_Create();
    bool Step4_AiFilter_Running();  //convert

    bool Step2_ColorFilter_IsNeed() {return true ;}
    bool Step3_ColorFilter_Create() {return false;}
    bool Step4_ColorFilter_Running(){return false;}

    bool Step9_ReGeneratedByAnkerMake();
    bool Step9_UsingRawFile();


    GC::GCodeInfo getGcodeInfo(){return gcodeInfo;}

private:
    //  GCodeFilter_Base
    GCodeFilter_Base      *gCodeFilterColorImpl{ nullptr };
    GCodeFilter_AIBase     *gCodeFilterAIImpl   { nullptr };
    QFileMemoryMapRead  fileReader;
    QFileMemoryMapWrite fileWriter;
    GC::GCodeInfo       gcodeInfo;
};

#endif // GCODEFILTERCONTROL_H
