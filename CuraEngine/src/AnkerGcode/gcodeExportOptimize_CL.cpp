#include "gcodeExportOptimize_CL.h"
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


namespace CL {


void VAJK_K_Cmd::init(const cura::Settings &settings){
    ak_VAJK_K_enabled= settings.get<bool>("ak_VAJK_K_enabled");
    ak_K_print       = settings.get<double>("ak_K_print");
    ak_K_infill      = settings.get<double>("ak_K_infill");
    ak_K_wall        = settings.get<double>("ak_K_wall");
    ak_K_wall_0      = settings.get<double>("ak_K_wall_0");
    ak_K_wall_x      = settings.get<double>("ak_K_wall_x");
    ak_K_support     = settings.get<double>("ak_K_support");
    ak_K_topbottom   = settings.get<double>("ak_K_topbottom");
    ak_K_skirt_brim  = settings.get<double>("ak_K_skirt_brim");
    ak_K_layer_0     = settings.get<double>("ak_K_layer_0");
}

double VAJK_K_Cmd::getK_byType(const cura::PrintFeatureType &type)
{
    switch (type)
    {
    case cura::PrintFeatureType::OuterWall:
        return ak_K_wall_0;
    case cura::PrintFeatureType::InnerWall:
        return ak_K_wall_x;
    case cura::PrintFeatureType::Skin:
        return ak_K_topbottom;
    case cura::PrintFeatureType::SkirtBrim:
        return ak_K_skirt_brim;
    case cura::PrintFeatureType::Infill:
        return ak_K_infill;
    case cura::PrintFeatureType::Support:
    case cura::PrintFeatureType::SupportInfill:
    case cura::PrintFeatureType::SupportInterface:
        return ak_K_support;
    case cura::PrintFeatureType::PrimeTower:
        return ak_K_print;
    case cura::PrintFeatureType::MoveCombing:
    case cura::PrintFeatureType::MoveRetraction:
    case cura::PrintFeatureType::NoneType:
    case cura::PrintFeatureType::NumPrintFeatureTypes:
    default:
        return -1;
    }
}



void SnapshotLayerCommand::init(cura::GCodeExport *gcodeExport){
    m_gcodeExport = gcodeExport;
    const cura::Settings& settingsScene = m_gcodeExport->getSettings_Scene();

    snapshot_layer_M1024_enable = false;
    snapshot_uplayer_enable     = false;

    do{
        if(settingsScene.has(key_snapshot_layer_M1024_enable)) ; else {break;}

        snapshot_layer_M1024_enable    = settingsScene.get<bool>(key_snapshot_layer_M1024_enable);
        anker_camera_take_picture_time = settingsScene.get<double>(key_camera_take_picture_time);

        if(snapshot_layer_M1024_enable == true); else {break;}

        bool hasParam = false;
        bool has1 = settingsScene.has(key_snapshot_uplayer_array);
        bool has2 = settingsScene.has(key_snapshot_height_array );
        if(has1 && has2){
            anker_param_ai_uplayer_array = settingsScene.get< std::vector<int>    >(key_snapshot_uplayer_array);
            anker_param_ai_height_array  = settingsScene.get< std::vector<double> >(key_snapshot_height_array);
            hasParam = (anker_param_ai_uplayer_array.size() * anker_param_ai_height_array.size()) > 0 ;
        }
        else if(has1 || has2){break;}

        if(!hasParam)
        {   
            anker_param_ai_uplayer_array = {1, 2, 3, 4, 5};
            anker_param_ai_height_array  = {70, 80, 90, 100, 110, 120};
        }

        std_sort( anker_param_ai_uplayer_array );
        for(auto uplayer = anker_param_ai_uplayer_array.begin(); uplayer != anker_param_ai_uplayer_array.end(); ){
            if(*uplayer < 1)
                uplayer = anker_param_ai_uplayer_array.erase(uplayer);
            else
                ++uplayer;

            //cura::logCL("$CL$ snapshot_uplayer_array, %d, %s", *uplayer,  (*uplayer < 1 ) ? "false" : "true" );
        }

        std_sort( anker_param_ai_height_array );
        for(auto height = anker_param_ai_height_array.begin(); height != anker_param_ai_height_array.end(); ){
            if(*height < 0.1)
                height = anker_param_ai_height_array.erase(height);
            else
                ++height;

           // cura::logCL("$CL$ snapshot_height_array , %d, %s", *height ,  (*height < 0.1) ? "false" : "true" );
        }

        if(anker_param_ai_uplayer_array.size() * anker_param_ai_height_array.size() > 0) ; else { break; }

        snapshot_uplayer_enable = true;
    }while(false);
}

bool SnapshotLayerCommand::snapshot_writeLayerCountComment(const size_t layer_count) {
    snapshot_count = layer_count;
    if(snapshot_layer_M1024_enable && snapshot_uplayer_enable){
        snapshot_count += anker_param_ai_uplayer_array.size() * anker_param_ai_height_array.size();
        return true;
    }
    return false;
}

bool SnapshotLayerCommand::snapshot_writeLayerComment(const int layer_nr){

    //  chenge @2022-05-12 by CL
    const cura::Settings& settingsScene = m_gcodeExport->getSettings_Scene();

    auto L_Posion   = m_gcodeExport->getPosition();
    auto L_Z        = m_gcodeExport->getPositionZ();
    auto L_E        = m_gcodeExport->clCurrentStatus->getE_block();
    auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index();
    auto&L_Nr       = m_gcodeExport->clCurrentStatus->getL_Nr();
    auto&L_Count    = m_gcodeExport->clCurrentStatus->getL_Count();
    auto new_line   = m_gcodeExport->new_line;
    auto retract_speed = settingsScene.get<cura::Velocity>("retraction_retract_speed");
    auto prime_speed   = settingsScene.get<cura::Velocity>("retraction_prime_speed");
    auto travel_speed  = settingsScene.get<cura::Velocity>("speed_travel");
    auto relative      = settingsScene.get<bool>("relative_extrusion");
    auto machine_X= settingsScene.get<UM>("machine_width");
    auto machine_Y= settingsScene.get<UM>("machine_depth");
    auto machine_Z= settingsScene.get<UM>("machine_height");

    double retraction = settingsScene.get<double>("retraction_amount");
    //retraction = retraction > 2 ? (retraction - 1) : retraction;

    
    double ms     = anker_camera_take_picture_time ;  if(ms < 30){ms=30;} // = min *60
    double oldF   = m_gcodeExport->clCurrentStatus->getF_block();
    double delayF = 1.0;      //  120 mm/min = 2mm/s   um/ms == mm/s
    double upF    = 20.0;     
    int    Z_um   = delayF * ms ;


    
    auto generalTakePicture1 = [&](bool upAgain){
        (*(m_gcodeExport->output_stream)) << "M1024 L" << L_Nr << "/" << snapshot_count << " 3"                                         << new_line;
        (*(m_gcodeExport->output_stream)) << "G0" << " F" << double2SS{1,   upF  * 60} <<  " Z" << UM2MM{ L_Z + (upAgain ? 0 : Z_um) }  << new_line;
        (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Nr                                                                          << new_line;
        (*(m_gcodeExport->output_stream)) << "G0" << " F" << double2SS{1, delayF * 60} <<  " Z" << UM2MM{ L_Z + (upAgain ? Z_um : 0) }  << new_line;
        if( !upAgain ) (*(m_gcodeExport->output_stream)) << "G0" << " F" << double2SS{1,   oldF * 60}                                   << new_line;
    };

    /// 2022/12/14  Binary for AI wipe
    
    auto wipeExtruderCircle = [&](){
        (*(m_gcodeExport->output_stream)) << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -2 : L_E-2 } << new_line;   //  -2
        (*(m_gcodeExport->output_stream)) << "G3" << " F" << double2SS{ 1, travel_speed  * 60 } << " I3 J0 P1"                                   << new_line;
        (*(m_gcodeExport->output_stream)) << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -1 : L_E-3 } << new_line;   //  -3
        (*(m_gcodeExport->output_stream)) << "G2" << " F" << double2SS{ 1, travel_speed  * 60 } << " I3 J0 P1"                                   << new_line;
        (*(m_gcodeExport->output_stream)) << "G0" << " X" << UM2MM{ L_Posion.x } << " Y" << UM2MM{ L_Posion.y }                                  << new_line;
    };

    
    auto wipeExtruderLine = [&](){
        //  #define ANKER_HOMING_SCRIPT_ABSOLUTE    "G92 E0\nG1 X200 Y237 E10 F3000\nG1 X117.5 Y236.2 E15 F3000\nG1 E5 F3600\nG92 E0\n"
        //  #define ANKER_HOMING_SCRIPT_NO_ABSOLUTE "G92 E0\nG1 X200 Y237 E10 F3000\nG1 X117.5 Y236.2 E5 F3000\nG1 E-10 F3600\nG92 E0\n
        UM _0X = machine_X - 120000 - (L_Nr*2-1)*10000 ; // 220 200, 110 90 70 50 30 10
        UM _1X = machine_X - 120000 - (L_Nr*2-0)*10000 ; // 210 190, 100 80 60 40 20
        UM _0Y = machine_Y -   2000;
        UM _0Z = 20;

        (*(m_gcodeExport->output_stream)) << "G0" << " F" << double2SS{ 1, travel_speed * 60 } << " X" << UM2MM{ _0X } << " Y" << UM2MM{ _0Y  } << new_line;
        (*(m_gcodeExport->output_stream)) << "G0" << " F" << double2SS{ 1,          upF * 60 } << " Z" << UM2MM{ _0Z }                          << new_line;
        (*(m_gcodeExport->output_stream)) << "G3" << " I2 J0 P2"                               << " E" << double2SS{ 5, relative ?  3 : L_E   } << new_line;    //  0
        (*(m_gcodeExport->output_stream)) << "G0" << " X" << UM2MM{_1X} << " Y" << UM2MM{_0Y } << " E" << double2SS{ 5, relative ?  0 : L_E + retraction }<< new_line;
        (*(m_gcodeExport->output_stream)) << "G2" << " I-2 J0" << new_line << "G2" << " I-2 J0" << new_line<<"G2" << " I-2 J0" << new_line;    //
        (*(m_gcodeExport->output_stream)) << "G1" << " F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? -1 : L_E + retraction - 1 } << new_line;   //  -0.5
    };

    
    auto recoveryPosition = [&](){
        (*(m_gcodeExport->output_stream)) << "G0 F" << double2SS{ 1,           upF * 60 } << " Z" << UM2MM{ L_Z }                                       << new_line;
        (*(m_gcodeExport->output_stream)) << "G0 F" << double2SS{ 1,  travel_speed * 60 } << " X" << UM2MM{ L_Posion.x } << " Y" << UM2MM{ L_Posion.y } << new_line;
        //(*(m_gcodeExport->output_stream)) << "G1 F" << double2SS{ 1, retract_speed * 60 } << " E" << double2SS{ 5, relative ? 2 : L_E}                << new_line;    //  0
        (*(m_gcodeExport->output_stream)) << "G0 F" << double2SS{ 1,          oldF * 60 }                                                               << new_line;
    };


    
    
    
    
    

    (*(m_gcodeExport->output_stream)) << "; Z_TakePictureStart: " << L_Nr << new_line;

    int coust = 0;
    bool upAgain = std_contain(anker_param_ai_uplayer_array, layer_nr);
    generalTakePicture1(upAgain);   

    if( upAgain ){      

        wipeExtruderCircle();       

        for(double height : anker_param_ai_height_array){
            char L_Nr_str[16]{0};
            sprintf(L_Nr_str, "%d.%02d", L_Nr, ++coust);      
            UM pic_z_offset = static_cast<UM>(height * 1000);

            if((L_Z + pic_z_offset + Z_um) >= machine_Z)
                pic_z_offset = machine_Z - L_Z - Z_um;

            (*(m_gcodeExport->output_stream)) << "G0 F" << double2SS{ 1,    upF * 60 } << " Z" << UM2MM{ L_Z + pic_z_offset } << new_line;
            (*(m_gcodeExport->output_stream)) << "M1024 L"    << L_Nr_str << "/" << snapshot_count << " 3" << new_line; // (res ? " 3" : " 1")
            (*(m_gcodeExport->output_stream)) << ";SUBLAYER:" << L_Nr_str << new_line; // writeLayerComment(layer_nr);
            (*(m_gcodeExport->output_stream)) << "G0 F" << double2SS{ 1, delayF * 60 } << " Z" << UM2MM{ L_Z + pic_z_offset + Z_um + (coust * 20) } << new_line;
        }

        wipeExtruderLine();         

        recoveryPosition();         
    }
    (*(m_gcodeExport->output_stream)) << "; Z_TakePictureEnd: " << L_Nr << new_line;

    return true;
}

bool SnapshotLayerCommand::snapshot_finalize() {
    //  chenge @2022-05-12 by CL
    auto L_Z        = m_gcodeExport->getPositionZ();
    auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index() + 1;
    auto L_Nr       = m_gcodeExport->clCurrentStatus->getL_Nr() + 1;
    auto L_Count    = m_gcodeExport->clCurrentStatus->getL_Count();
    auto new_line   = m_gcodeExport->new_line;

    (*(m_gcodeExport->output_stream)) << "M1024 L" << L_Nr << "/" << L_Count << " 3" << new_line; // (res ? " 3" : " 1")
    (*(m_gcodeExport->output_stream)) << "G0 Z" << UM2MM{L_Z + 2000} << "; Z_TakePictureStart: " << L_Nr << new_line;
    (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Nr << new_line; // writeLayerComment(layer_nr);
    return true;
}



}//  end of namespace CL



