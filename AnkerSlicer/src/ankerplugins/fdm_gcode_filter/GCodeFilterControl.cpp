#include "GCodeFilterControl.h"
#include "GCodeFilter_AIProducer.h"




bool GCodeFilterControl::Step0_InitFileIO(const QString &srcGcode, const QString &dstGcode)
{
    if(!srcGcode.isEmpty()) { fileReader. readMemoryFile(srcGcode) ;}
    if(!dstGcode.isEmpty()) { fileWriter.writeMemoryFile(dstGcode) ;}

    Q_ASSERT(fileReader .readAlready());
    Q_ASSERT(fileWriter.writeAlready());

    bool res = fileReader.readAlready() && fileWriter.writeAlready();
    return res;
}

bool GCodeFilterControl::Step1_GCodeInfo_Init()
{
    //Q_ASSERT(SuccessInPreviousStep);
    bool res = false;
    do{
        {   /// GcodeFilePath
            if(fileReader.readAlready() && fileWriter.writeAlready()){
                gcodeInfo.srcGcodeFilePath = fileReader.getQInfo().absoluteFilePath();
                gcodeInfo.dstGcodeFilePath = fileWriter.getQInfo().absoluteFilePath();
            }
            else{
                gcodeInfo.errorStr = QString("[ERROR]:Step0_InitFileIO(%1, %2);").arg(fileReader.getQInfo().filePath()).arg(fileReader.getQInfo().filePath());
                const char * error = gcodeInfo.errorStr.toLocal8Bit().data();
                Q_ASSERT_X(false, "", error);
                break; // return because of failure
            }
        }

        {   ///  NewLine
            MemRef::SetNewLine(fileReader.getMemRef());
            gcodeInfo.newLine = MemRef::NewLine;

            if(gcodeInfo.newLine.isEmpty()){
                const char * error = "[ERROR]:The newline error in the file";
                gcodeInfo.errorStr = error;
                Q_ASSERT_X(false, "", error);
                break; // return because of failure
            }
        }

        {   /// FindProducer
            MemFileRef docFileRef = fileReader.getMemRef();
            MemLineRef line = MemRef::GetLocalLine(docFileRef, docFileRef.left(1));

            const TokenMapType & tokenProducers = GC::Token_Producers();
            const QList<int> ekeys = tokenProducers.keys();

            GC::EGeneratedByProducer producer = GC::E_ProducerUnknown;

            for(int j = 0; j < 100; j++){
                if(producer == GC::E_ProducerUnknown && !!line);else{break;}    // for(int j)
                for(int ekey : ekeys){
                        if( line.contains(tokenProducers[ekey])){
                            producer = GC::EGeneratedByProducer(ekey);
                            break; // for(int ekey : ekeys)
                        }
                }
                line = MemRef::GetLocalLineDown(docFileRef, line);
            }

            if(producer != GC::E_ProducerUnknown){
                gcodeInfo.producer = producer;
            }
            else{
                const char * error = "[ERROR]:Unknown Gcode";
                gcodeInfo.errorStr = error;
                //Q_ASSERT_X(false, "", error);
                break; // return because of failure
            }
        }

        {   ///  HasAnkerThumbnail ????
            gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetAndDoNothing  ;

            const TokenMapType & tokens = GC::Token_Thumbnail();

            MemDocuRef docuFileRef = fileReader.getMemRef();
            MemFileRef filehead    = docuFileRef.left(2 * 1024);
            MemCharRef thumbnall   = filehead.findChars(tokens[GC::E_Thunbail_begin]);

            if( !!thumbnall ){
                MemLineRef lineUp  = filehead.docCutUp(thumbnall);  //  change @2023-06-21 by ChunLian
                if( false );
                else if( lineUp.contains(tokens[GC::E_Thunbail_AnkerReGen1]) ){ gcodeInfo.hasThm = GC::E_AnkerThumbnail_AddedByAnker; }
                else if( lineUp.contains(tokens[GC::E_Thunbail_AnkerReGen2]) ){ gcodeInfo.hasThm = GC::E_AnkerThumbnail_AddedByAnker; }
                else if( lineUp.contains(tokens[GC::E_Thunbail_AnkerFlag  ]) ){ gcodeInfo.hasThm = GC::E_AnkerThumbnail_AlreadyHasAnker; }
                else{
                    gcodeInfo.hasThm = GC::E_AnkerThumbnail_HasButNotAnker;
                }
            }
            else{
                if( false );
                else if(GC::isBasedOn_Anker  (gcodeInfo.producer)) { gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetAndDoNothing  ; } //  miss
                else if(GC::isBasedOn_Slic3r (gcodeInfo.producer)) { gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetButShouldAdded; }
                else if(GC::isBasedOn_Cura   (gcodeInfo.producer)) { gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetButShouldAdded; }
                else if(GC::isBasedOn_Simp3d (gcodeInfo.producer)) { gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetButShouldAdded; }
                else if(GC::isBasedOn_Bamboo (gcodeInfo.producer)) { gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetButShouldAdded; }
                else {//if(GC::isBasedOn_Unknown(gcodeInfo.producer))
                    gcodeInfo.hasThm = GC::E_AnkerThumbnail_NotYetAndDoNothing  ;
                }
            }
        }

        {   ///  HasAI ? Should be added ????
            MemDocuRef docuFileRef = fileReader.getMemRef();
            MemFileRef filehead    = docuFileRef.left (48*1024);
            MemFileRef filetail    = docuFileRef.right(48*1024);


            QByteArray tokenAI( gcodeInfo.newLine + GC::Token_AnkerAI_0() );

            if( !! filetail.findLastChars(QLatin1String(tokenAI)) ){
                if(GC::isBasedOn_Anker (gcodeInfo.producer)) {
                    gcodeInfo.hasAI = GC::E_AI_AlreadyHasAnker;
                }
                else  {
                    gcodeInfo.hasAI = GC::E_AI_AddedByAnker;
                }
            }
            else{
                //QLatin1String layerKeyWord = Token_LayerKeyWord()[gcodeInfo.producer];
                //QLatin1String role1KeyWord;
                if( false );
                else if(GC::isBasedOn_Simp3d (gcodeInfo.producer)) { gcodeInfo.hasAI = GC::E_AI_NotYetAndDoNothing; }
                else if(GC::isBasedOn_Slic3r (gcodeInfo.producer)) { gcodeInfo.hasAI = GC::E_AI_NotYetButShouldAdded; }
                else if(GC::isBasedOn_Cura   (gcodeInfo.producer)) { gcodeInfo.hasAI = GC::E_AI_NotYetButShouldAdded; }
                else {//if(GC::isBasedOn_Unknown(gcodeInfo.producer))
                    gcodeInfo.hasAI = GC::E_AI_NotYetAndDoNothing  ;
                }
            }
        }

        res = true;
    }while(false);

    return res;
}

bool GCodeFilterControl::Step2_AiFilter_IsNeed()
{
    bool addThm = (gcodeInfo.hasThm == GC::E_AnkerThumbnail_NotYetButShouldAdded || gcodeInfo.hasThm == GC::E_AnkerThumbnail_HasButNotAnker);
    bool addAI  = (gcodeInfo.hasAI == GC::E_AI_NotYetButShouldAdded);
    return addThm || addAI;
}

bool GCodeFilterControl::Step3_AiFilter_Create()
{
    bool res = true;
    switch (gcodeInfo.producer) {
        case GC::EGeneratedByProducer::E_AnkerMake:
        case GC::EGeneratedByProducer::E_AnkerSlicer:{
            gCodeFilterAIImpl = new GCodeFilter_AIAnkerMake  (fileReader, fileWriter, gcodeInfo);
            break;
        }
        case GC::EGeneratedByProducer::E_Cura:{
            gCodeFilterAIImpl = new GCodeFilter_AICura       (fileReader, fileWriter, gcodeInfo);
            break;
        }
        case GC::EGeneratedByProducer::E_AnkerPrusa:{
            gCodeFilterAIImpl = new GCodeFilter_AIAnkerPrusa (fileReader, fileWriter, gcodeInfo);
            break;
        }
        case GC::EGeneratedByProducer::E_PrusaSlicer:{
            gCodeFilterAIImpl = new GCodeFilter_AIPrusa      (fileReader, fileWriter, gcodeInfo);
            break;
        }
        case GC::EGeneratedByProducer::E_SuperSlicer:{
            gCodeFilterAIImpl = new GCodeFilter_AISuper      (fileReader, fileWriter, gcodeInfo);
            break;
        }
        case GC::EGeneratedByProducer::E_Simplify3D:{
            gCodeFilterAIImpl = new GCodeFilterAI_Simplify3D (fileReader, fileWriter, gcodeInfo);
            break;
        }
        default:{
            res = false;
        }
    }
    return res;
}

bool GCodeFilterControl::Step4_AiFilter_Running()
{
    Q_ASSERT(gCodeFilterAIImpl);
    bool res = true;
    try
    {
        res = gCodeFilterAIImpl->running();
    }
    catch (const std::exception& e) {
        res = false;
        gcodeInfo.errorStr = gcodeInfo.errorStr + "\n" + e.what();
        qDebug() << e.what();
    }
    return res;
}



bool GCodeFilterControl::Step9_ReGeneratedByAnkerMake()
{
    fileReader.close();
    fileWriter.close();

    bool addThm = (gcodeInfo.hasThm == GC::E_AnkerThumbnail_NotYetButShouldAdded || gcodeInfo.hasThm == GC::E_AnkerThumbnail_HasButNotAnker);
    bool addAI  = (gcodeInfo.hasAI == GC::E_AI_NotYetButShouldAdded);
    if(addThm){   // addThm && !addAI
        gcodeInfo.srcGcodeFilePath = gcodeInfo.dstGcodeFilePath;
    }

    return true;
}

bool GCodeFilterControl::Step9_UsingRawFile()
{
    fileReader.close();
    fileWriter.close();
    fileWriter.remove();

    gcodeInfo.dstGcodeFilePath = gcodeInfo.srcGcodeFilePath;
    return true;
}


