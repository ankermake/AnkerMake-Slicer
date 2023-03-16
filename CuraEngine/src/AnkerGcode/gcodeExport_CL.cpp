#include <assert.h>
#include <cmath>
#include <iomanip>
#include <stdarg.h>

#include "../Application.h" //To send layer view data.
#include "../ExtruderTrain.h"
#include "../gcodeExport.h"
#include "../PrintFeature.h"
#include "../RetractionConfig.h"
#include "../Slice.h"
#include "../communication/Communication.h" //To send layer view data.
#include "../settings/types/LayerIndex.h"
#include "../utils/Date.h"
#include "../utils/logoutput.h"
#include "../utils/string.h" // MMtoStream, PrecisionedDouble
#include "../WipeScriptConfig.h"

#include "gcodeExportOptimize_CL.h"

namespace cura {


int CL_CurrentStatus::akLayerNumber = -1;

GCodeExport::GCodeExport():BaseType()
{

}

GCodeExport::~GCodeExport()
{
    if(clOptinize){
        delete clOptinize;
        clOptinize = nullptr;
    }
    if(clCurrentStatus){
        delete clCurrentStatus;
        clCurrentStatus = nullptr;
    }
}


void GCodeExport::preSetup(const size_t start_extruder){
    { //  BaseType
        BaseType::preSetup(start_extruder);
    }
    clOptinize = new CL_GCodeExportOptinize(this);
    clCurrentStatus = new CL_CurrentStatus(*this);
}

void GCodeExport::writeCodeStored(const char *str,int pos, int reserve)
{
    output_stream->flush();
    std::ostream::pos_type tell = output_stream->tellp();
    output_stream->seekp(pos, std::ios::beg);

    for(int i=2; i < reserve; ++i){
        *output_stream << ' ';
    }
    *output_stream << new_line;

    output_stream->seekp(pos, std::ios::beg);
    *output_stream << str << ";" ;//<< new_line;

    //if(tell < (reserve+pos)){
    //    output_stream->seekp(reserve+pos, std::ios::beg);
    //}
    //else{
    //    //output_stream->seekp(tell, std::ios::beg);
    //    output_stream->seekp(0, std::ios::end);
    //}
    output_stream->seekp(0, std::ios::end);
    output_stream->flush();
}

void GCodeExport::writeLayerCountComment(const size_t layer_count){
    clCurrentStatus->getL_Count() = layer_count;
    clOptinize->GCodeExport_writeLayerCountComment(layer_count);
    { //  BaseType
        //*output_stream << ";LAYER_COUNT:" << layer_count << new_line;
        BaseType::writeLayerCountComment(layer_count);
    }
}

void GCodeExport::writeLayerComment(const LayerIndex layer_nr){
    //  change @2022-05-12 by CL
    clCurrentStatus->getL_Index() = layer_nr;
    clCurrentStatus->getL_Nr()++;
    //clCurrentStatus->getL_Count() += int(layer_nr < 0);

    if(clCurrentStatus->getL_Nr() <= 0 || clCurrentStatus->getL_Index() <= 0)
    { //  BaseType
        //*output_stream << ";LAYER:" << layer_nr << new_line;
        BaseType::writeLayerComment(layer_nr);
        
    }
    else{
        clOptinize->GCodeExport_writeLayerComment(layer_nr);
    }
}

void GCodeExport::writeTypeComment(const PrintFeatureType& type)
{
    clCurrentStatus->getType() = type;
    { //  BaseType
        //*output_stream << ";TYPE:WALL-OUTER" << new_line;
        BaseType::writeTypeComment(type);
    }

    clOptinize->GCodeExport_writeTypeComment(type);
}
void GCodeExport::writePrintAcceleration(const Acceleration& acceleration){
    clCurrentStatus->getA_block() = acceleration;
    { //  BaseType
        //*output_stream << "M204 S" << PrecisionedDouble{0, acceleration} << new_line;
        BaseType::writePrintAcceleration(acceleration);
    }
}
void GCodeExport::writeTravelAcceleration(const Acceleration& acceleration){
    clCurrentStatus->getA_block() = acceleration;
    { //  BaseType
        //*output_stream << "M204 S" << PrecisionedDouble{0, acceleration} << new_line;
        BaseType::writeTravelAcceleration(acceleration);
    }
}


void GCodeExport::writePrintK_M900(double K, double L, int S, int T)
{
    //  https://marlinfw.org/docs/gcode/M900.html
    bool hasK = (K >= 0.0);
    bool hasL = (L >= 0.0);
    bool hasS = (S >= 0);
    bool hasT = (T >= 0);

    if(hasK || hasL || hasS) // hasT
    {
        std::ostringstream ssline;
        ssline << "M900";
        if(hasT){ ssline << " T" << T; }
        if(hasK){ ssline << " K" << K; }
        if(hasL){ ssline << " L" << L; }
        if(hasS){ ssline << " S" << S; }
        ssline << ";add by AnkerSlicer";

        writeLine(ssline.str().c_str());
    }
}

void GCodeExport::finalize(const char* endCode){
    clOptinize->GCodeExport_finalize();
    { //  BaseType
        BaseType::finalize(endCode);
    }
}

const Settings &GCodeExport::getSettings_Scene(){
    return Application::getInstance().current_slice->scene.settings;
}

const Settings &GCodeExport::getSettings_Extruder(){
    return Application::getInstance().current_slice->scene.extruders[current_extruder].settings;
}

void GCodeExport::writeFXYZE_CL(const char *G, const Velocity &speed, const int x, const int y, const int z, const double e, const PrintFeatureType &feature)
{
    bool hasF = (currentSpeed != speed);
    bool hasX = (x != currentPosition.x);
    bool hasY = (y != currentPosition.y);
    bool hasZ = (z != currentPosition.z);
    bool hasE = (e + current_e_offset) != current_e_value;

    std::map<char, float> common;
    /*
        @2022-05-10 by CL


    */
    // (F) Z
    //std::ostringstream ssline_Z;
    //if (hasZ && !hasE)
    //{
    //    if (hasF){
    //        if (!hasG_Z__) { ssline_Z << G; hasG_Z__ = true; }
    //        ssline_Z << " F" << PrecisionedDouble{ 1, speed * 60 };
    //    }
    //    if (!hasG_Z__) { ssline_Z << G; hasG_Z__ = true; }
    //    ssline_Z << " Z" << MMtoStream{ z };
    //}

    // (F) X Y (Z) E
    std::ostringstream ssline;
    {
        // F
        Point gcode_pos = getGcodePos(x, y, current_extruder);
        total_bounding_box.include(Point3(gcode_pos.X, gcode_pos.Y, z));

        ssline << G;

        if(hasF){
            ssline << " F" << PrecisionedDouble{ 1, speed * 60 };
        }

        if(hasX){
            ssline << " X" << MMtoStream{ gcode_pos.X };
        }

        if(hasY){
            ssline << " Y" << MMtoStream{ gcode_pos.Y };
        }

        if (hasZ) {
            ssline << " Z" << MMtoStream{ z };
        }

        if(hasE){
            char E = extruder_attr[current_extruder].extruderCharacter;
            const double output_e = (relative_extrusion) ? e + current_e_offset - current_e_value : e + current_e_offset;
            ssline << " " << E << PrecisionedDouble{ 5, output_e };
        }
        if (hasZ) {
            ssline << ";Z_UP:" << MMtoStream{ z - (currentPosition.z == 23457 ? 0 : currentPosition.z) };
        }
    }

    std::string strline = ssline.str();

    if(!strline.empty()){
        *output_stream << strline << new_line;
    }

    currentPosition = Point3(x, y, z);
    current_e_value = e;
    currentSpeed = speed;
    estimateCalculator.plan(TimeEstimateCalculator::Position(INT2MM(x), INT2MM(y), INT2MM(z), eToMm(e)), speed, feature);

    if( (hasX ||hasY) && hasE ){
        clOptinize;
    }
    //logAlways("%s", strline.str().c_str());
    //fprintf(stdout, "%s", strline.str().c_str());
}

void GCodeExport::__writeFZ_CL(double z, double zDelta, const PrintFeatureType &feature)
{

    *output_stream << ";CL_TEST " << z << " " << zDelta << " " << int(feature) << new_line;
}


} //  end of namespace cura
