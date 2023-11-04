#include "GCodeFilter_AIBase.h"
#include "common/utilities/string.h"
#include "common/utilities/AkPoint3i.h"
#include "common/mlapplication.h"
#include <QByteArray>
#include "common/parameters/rich_parameter.h"
#include "fdm_gcode_filter.h"


//the main flow
bool GCodeFilter_AIBase::running()
{
    getMachineSize();
    parseGCodeParams();
    segments = splitGcode(getLayerKeyWord());
    adjustDoc(segments);
    infillAiGcode(segments);
    writeFile(segments);
    return true;
}


QList<MemRefDecorate> GCodeFilter_AIBase::splitGcode(QLatin1String express)
{
    MemFileRef memFileRef = fileReader.getMemRef();
    QList<MemRef> memRefList =  MemRef::DocFindLineAll_Zebra(memFileRef, express);
    bool isLayerLine = memRefList.size() > 0 && memRefList[0].contains(express);
    QList<MemRefDecorate> memRefDecorates;
    for(int i = 0;i <memRefList.size();i++)
    {
        MemRefDecorate seg;
        seg.willAddM1024 = isLayerLine;
        seg.oriSegment = memRefList[i];
        isLayerLine=!isLayerLine;
        memRefDecorates.append(seg);
    }
    return memRefDecorates;
}

//first segment adjust
void GCodeFilter_AIBase::adjustDoc(QList<MemRefDecorate> &segments)
{
    if (segments.size() == 0)
    {
         return;
    }
    bool addThm = (gcodeInfo.hasThm == GC::E_AnkerThumbnail_NotYetButShouldAdded || gcodeInfo.hasThm == GC::E_AnkerThumbnail_HasButNotAnker);
    if(addThm){
         MemRef gcodeFileHead_BeforeLayer0 = segments[0].oriSegment;
         QLatin1String producer = getProducer();

         QList<MemLineRef> segList = MemRef::DocFindLineAll_Zebra(gcodeFileHead_BeforeLayer0, producer);
         QList<MemRefDecorate> headParts;
         for(int i = 0;i <segList.size();i++)
         {
             MemLineRef & seg = segList[i];
             MemRefDecorate refDec;
             refDec.willAddM1024 = false;
             refDec.oriSegment = seg;
             headParts.push_back(refDec);
             if(seg.contains(producer))
             {
                 //;ReGenerated by AnkerMake for thumbnail
                 MemRefDecorate generatorInfo;
                 generatorInfo.willAddM1024 = false;
                 decorateStore.append(QLatin1String(";ReGenerated by AnkerMake for thumbnail") + MemRef::NewLine);
                 generatorInfo.oriSegment = MemRef(QLatin1String(decorateStore[decorateStore.size() - 1]));
                 headParts.push_back(generatorInfo);
             }
         }
         segments.removeFirst();
         segments = headParts + segments;
    }
}

//for first N layers . there has subLayer to take picture
bool GCodeFilter_AIBase::infillAiGcode(QList<MemRefDecorate>& segments)
{
    QTime runningTimer;
    double time_ms = runningTimer.restart();

    if(gcodeInfo.hasAI == GC::E_AI_NotYetButShouldAdded)
    {
        int layerCount = 0;
        for (auto & item : segments){
            if(item.willAddM1024){
                //take picture after the first layer be printed
                item.willAddM1024 = false; // !line.isLayerLine  exclude firstLayer
                item.decorateSegment = item.oriSegment;
                break;
            }
        }

        QVector<QPair<int, MemRefDecorate&>> segments_isLayerLine; segments_isLayerLine.reserve((segments.size() + 1) / 2);
        for (auto & item : segments){
            if(item.willAddM1024){
                ++layerCount;
                segments_isLayerLine.append({layerCount, item});
            }
        }

        QVector<QByteArray> decorateStoreLocal(layerCount);

        if(OMP_GET_MAX_THREADS() > 1){
            #pragma omp parallel for schedule(dynamic)
            for (int i = 0; i < layerCount; ++i)
            {
                auto & item = segments_isLayerLine[i];
                writeLayerComment(item.second, item.first, layerCount, &(decorateStoreLocal[i]));
            }
        }
        else{
            for (int i = 0; i < segments_isLayerLine.size(); i++)
            {
                auto & item = segments_isLayerLine[i];
                writeLayerComment(item.second, item.first, layerCount, &(decorateStoreLocal[i]));
            }
        }

        decorateStore.append(decorateStoreLocal);

        time_ms = runningTimer.restart();
        qDebug().nospace() << FUNC_LINE << "  [INFO]: " << OMP_GET_MAX_THREADS() << " layer count = " << segments_isLayerLine.size() << " , time = "  << time_ms << " ms";
    }
    else{   //  E_AI_NotYetAndDoNothing
        for(auto & seg : segments){
            seg.willAddM1024 = false;
        }
    }
    return true;
}

void GCodeFilter_AIBase::getMachineSize()
{
    machineSize = QVector3D(235,235,250);
    if (nullptr == FdmGcodeFilter::s_globalParameterList)
    {
        return;
    }
    const RichPoint3f& param = static_cast<const RichPoint3f&>(FdmGcodeFilter::s_globalParameterList->getParameterByName(AkConst::GlobalParameterKeys::ANKER_MACHINE_SIZE));
    if (!param.value().isPoint3f())
    {
        return;
    }
    machineSize.setX(param.value().getPoint3f().X());
    machineSize.setY(param.value().getPoint3f().Y());
    machineSize.setZ(param.value().getPoint3f().Z());
    //delete this code because GcodeFilterAiBase is a tmp object
//    QObject::connect(param.qobj.data(), &RichParameterQObject::valueChange, [&](const Value& value)->void{
//        machineSize.setX(value.getPoint3f().X());
//        machineSize.setY(value.getPoint3f().Y());
//        machineSize.setZ(value.getPoint3f().Z());
//    });
}

void GCodeFilter_AIBase::writeLayerComment(MemRefDecorate& LayerLineItem, int layerNumber, int layerCount, QByteArray* storeLocal)
{
    //layers to take pictures
    std::vector<int> anker_param_ai_uplayer_array = { 1, 2, 3, 4, 5 };
    //the upper distance when take picture on every ai layer
    std::vector<double> anker_param_ai_height_array = { 70, 80, 90, 100, 110, 120 };
    int lastLayerToTakePic = anker_param_ai_uplayer_array[anker_param_ai_uplayer_array.size() - 1];
    int scaleFactor = 1000;
    std::stringstream ss;
    if (needNotInsertAiCodeAfterFirstNLayers() && layerNumber >lastLayerToTakePic)
    {
        //ss << QString(LayerLineItem.oriSegment()).toStdString();
        LayerLineItem.decorateSegment = LayerLineItem.oriSegment;
        return;
    }
    using double2SS = PrecisionedDouble;
    using UM2MM = MMtoStream;
    typedef signed long long UM;
    double anker_camera_take_picture_time = 30;

    int snapshot_count = layerCount + anker_param_ai_uplayer_array.size() * anker_param_ai_height_array.size();

    auto getLastN = [&](QLatin1String str, const char* fmt)->double {
        MemFileRef memFileRef = fileReader.getMemRef();
        MemRef upperLine = MemRef::GetLocalLineUp(memFileRef, LayerLineItem.oriSegment);
        while (!!upperLine)
        {
            if (!upperLine.startWith('G'))
            {
                upperLine = MemRef::GetLocalLineUp(memFileRef, upperLine);
                continue;
            }
            MemCharRef idx = upperLine.findChars(str);
            if( !!idx ){
                return std::strtof(idx.end(), nullptr);
            }
            upperLine = MemRef::GetLocalLineUp(memFileRef, upperLine);
        }
        return 0;
    };

    //the first x,y,z,e,f when go upper from currentline
    auto getLastXYZEF = [&](AkPoint3i& point, double& E, double& F)->bool {
        point.setX(0.5 + scaleFactor * getLastN(QLatin1String(" X"), "X%f"));
        point.setY(0.5 + scaleFactor * getLastN(QLatin1String(" Y"), "Y%f"));
        point.setZ(0.5 + scaleFactor * getLastN(QLatin1String(" Z"), "Z%f"));
        E = getLastN(QLatin1String(" E"), " E%f ");
        F = getLastN(QLatin1String(" F"), " F%f ");
        return true;
    };
    //auto getLayrNumber = [](MemRef& layerLine)->int {
    //    int layerNumber = 0;
    //    sscanf(layerLine.m_data, ";LAYER:%d", &layerNumber);
    //    return layerNumber;
    //};

    AkPoint3i currentPosition;
    double currentE = 0;
    double currentF = 0;
    getLastXYZEF(currentPosition, currentE, currentF);

    //auto&L_Count    = m_gcodeExport->clCurrentStatus->getL_Count();
    auto new_line = std::string(gcodeInfo.newLine.data());

    //write a const speed . it can works will on ankermake machine
    double retract_speed = 60;
    //auto prime_speed   = settingsScene.get<cura::Velocity>("retraction_prime_speed");
    double travel_speed = 250;
    auto relative = gcodeInfo.relative_extrusion;

    //can get from message
//    auto machine_X= settingsScene.get<UM>("machine_width");
//    auto machine_Y= settingsScene.get<UM>("machine_depth");
//    auto machine_Z= settingsScene.get<UM>("machine_height");
    auto machine_X = machineSize.x() * scaleFactor;
    auto machine_Y = machineSize.y() * scaleFactor;
    auto machine_Z = machineSize.z() * scaleFactor;

    double retraction = 3;

    //  ms = z / f, 10  = 1000 * 0.05 / (300 / 60)   10ms = 0.05mm/300mm/min = 0.05mm/5mm/s = 0.01s
    double ms = anker_camera_take_picture_time;  if (ms < 30) { ms = 30; } // = min *60
    //double oldF   = m_gcodeExport->clCurrentStatus->getF_block();
    double delayF = 1.0;      //  120 mm/min = 2mm/s   um/ms == mm/s
    double upF = 20.0;     //  z speed 20mm/s @2022-10-13 by CL
    int    Z_um = delayF * ms;

    //take picture every layer
    auto generalTakePicture1 = [&](bool upAgain) {
        ss << "M1024 L" << layerNumber << "/" << snapshot_count << " 3" << new_line;
        ss << "G0" << " F" << double2SS{ 1,   upF * 60 } << " Z" << UM2MM{ currentPosition.z() + (upAgain ? 0 : Z_um) } << new_line;
        //ss << ";LAYER:" << layerNumber << new_line;
        ss << QString(LayerLineItem.oriSegment()).toStdString();
        //ss << ";LAYER:" << layerNumber << new_line;
        ss << "G0" << " F" << double2SS{ 1, delayF * 60 } << " Z" << UM2MM{ currentPosition.z() + (upAgain ? Z_um : 0) } << new_line;
        if (!upAgain) ss << "G0" << " F" << double2SS{ 1,   currentF } << new_line;
    };

    auto wipeExtruderCircle = [&]() {
        ss << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -2 : currentE - 2 } << new_line;   //  -2
        ss << "G3" << " F" << double2SS{ 1, travel_speed * 60 } << " I3 J0 P1" << new_line;
        ss << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -1 : currentE - 3 } << new_line;   //  -3
        ss << "G2" << " F" << double2SS{ 1, travel_speed * 60 } << " I3 J0 P1" << new_line;
        ss << "G0" << " X" << UM2MM{ currentPosition.x() } << " Y" << UM2MM{ currentPosition.y() } << new_line;
    };

    auto wipeExtruderLine = [&]() {
        //  #define ANKER_HOMING_SCRIPT_ABSOLUTE    "G92 E0\nG1 X200 Y237 E10 F3000\nG1 X117.5 Y236.2 E15 F3000\nG1 E5 F3600\nG92 E0\n"
        //  #define ANKER_HOMING_SCRIPT_NO_ABSOLUTE "G92 E0\nG1 X200 Y237 E10 F3000\nG1 X117.5 Y236.2 E5 F3000\nG1 E-10 F3600\nG92 E0\n
        UM _0X = machine_X - 120000 - (layerNumber * 2 - 1) * 10000; // 220 200, 110 90 70 50 30 10
        UM _1X = machine_X - 120000 - (layerNumber * 2 - 0) * 10000; // 210 190, 100 80 60 40 20
        UM _0Y = machine_Y - 2000;
        UM _0Z = 20;

        ss << "G0" << " F" << double2SS{ 1, travel_speed * 60 } << " X" << UM2MM{ _0X } << " Y" << UM2MM{ _0Y } << new_line;
        ss << "G0" << " F" << double2SS{ 1,          upF * 60 } << " Z" << UM2MM{ _0Z } << new_line;
        ss << "G3" << " I2 J0 P2" << " E" << double2SS{ 5, relative ? 3 : currentE } << new_line;    //  0
        ss << "G0" << " X" << UM2MM{ _1X } << " Y" << UM2MM{ _0Y } << " E" << double2SS{ 5, relative ? 0 : currentE + retraction } << new_line;
        ss << "G2" << " I-2 J0" << new_line << "G2" << " I-2 J0" << new_line << "G2" << " I-2 J0" << new_line;    //
        ss << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -1 : currentE + retraction - 1 } << new_line;   //  -0.5
    };

    auto recoveryPosition = [&]() {
        ss << "G0 F" << double2SS{ 1,           upF * 60 } << " Z" << UM2MM{ currentPosition.z() } << new_line;
        ss << "G0 F" << double2SS{ 1,  travel_speed * 60 } << " X" << UM2MM{ currentPosition.x() } << " Y" << UM2MM{ currentPosition.y() } << new_line;
        //ss << "G1 F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? 2 : L_E}                << new_line;    //  0
        ss << "G0 F" << double2SS{ 1,          currentF } << new_line;
    };

    ss << "; Z_TakePictureStart: " << layerNumber << new_line;


    int coust = 0;
    bool upAgain = std::find(anker_param_ai_uplayer_array.begin(), anker_param_ai_uplayer_array.end(), layerNumber) != anker_param_ai_uplayer_array.end();
    generalTakePicture1(upAgain);

    if (upAgain) {
        wipeExtruderCircle();
        for (double height : anker_param_ai_height_array) {
            char layerNumStr[16]{ 0 };
            sprintf(layerNumStr, "%d.%02d", layerNumber, ++coust);
            UM pic_z_offset = static_cast<UM>(height * 1000);

            if ((currentPosition.z() + pic_z_offset + Z_um) >= machine_Z)
                pic_z_offset = machine_Z - currentPosition.z() - Z_um;

            ss << "G0 F" << double2SS{ 1,    upF * 60 } << " Z" << UM2MM{ currentPosition.z() + pic_z_offset } << new_line;
            ss << "M1024 L" << layerNumStr << "/" << snapshot_count << " 3" << new_line; // (res ? " 3" : " 1")
            ss << ";SUBLAYER:" << layerNumStr << new_line; // writeLayerComment(layer_nr);
            ss << "G0 F" << double2SS{ 1, delayF * 60 } << " Z" << UM2MM{ currentPosition.z() + pic_z_offset + Z_um + (coust * 10) } << new_line;
        }

        wipeExtruderLine();
        recoveryPosition();
    }
    ss << "; Z_TakePictureEnd: " << layerNumber << new_line;
    QByteArray tmp(ss.str().c_str());
    LayerLineItem.decorateSegment = MemRef(QLatin1String(tmp));
    (*storeLocal) = tmp;
}

QString GCodeFilter_AIBase::getFileEndInfo()
{
    if (segments.size() == 0)
    {
        return "";
    }
    //if there has no end. add end
    auto lastSegment = segments[segments.size()-1];
    auto indexOfVersion = lastSegment.oriSegment.findLastChars(QLatin1String(";AnkerMake version:"));
    if ( !!indexOfVersion )
    {
        return "";
    }

    QStringList paramList;
    paramList << ";AnkerMake version: " + MeshLabApplication::appVer();
    paramList << ";End of Gcode";
    auto data = paramList.join(MemRef::NewLine);
    return data;
}

//write gcode
void GCodeFilter_AIBase::writeFile(QList<MemRefDecorate>& segments){

    QList<MemCharRef> toWriteList; toWriteList.reserve(segments.size() + 1);    //  add @2023-03-15 by ChunLian
    for(int i = 0;i < segments.size(); i++)
    {
        MemRefDecorate& segment= segments[i];
        toWriteList.append      (segment.willAddM1024 ? segment.decorateSegment : segment.oriSegment);
    }

    //add version
    QString endInfo = getFileEndInfo();
    if(!endInfo.isEmpty())
    {
        decorateStore.append(QByteArray(endInfo.toStdString().c_str()));
        toWriteList.append      (QLatin1String(decorateStore.last()));
    }

    fileWriter.writeAllOneTime(toWriteList);
    fileWriter.writeFinish();

    {
        bool addThm = (gcodeInfo.hasThm == GC::E_AnkerThumbnail_NotYetButShouldAdded || gcodeInfo.hasThm == GC::E_AnkerThumbnail_HasButNotAnker);
        if(addThm){
            QList<MemCharRef> toWriteList; toWriteList.reserve(segments.size() + 1);    //  add @2023-03-15 by ChunLian
            for(int i = 0;i < segments.size(); i++)
            {
                MemRefDecorate& segment= segments[i];
                toWriteList.append(segment.oriSegment);
            }

            QFileMemoryMapWrite justAddThm;
            justAddThm.writeMemoryFile(fileWriter.getQInfo().absoluteFilePath() + "_.gcode");

            justAddThm.writeAllOneTime(toWriteList);
            justAddThm.writeFinish();

            gcodeInfo.srcGcodeFilePath = justAddThm.getQInfo().absoluteFilePath();
        }
    }
}
