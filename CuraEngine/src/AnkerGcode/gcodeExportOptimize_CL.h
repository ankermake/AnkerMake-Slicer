#ifndef GCODEEXPORT_OPTIMIZE_CL_H
#define GCODEEXPORT_OPTIMIZE_CL_H

#include "../gcodeExport.h"
#include "../utils/string.h" // MMtoStream, PrecisionedDouble

namespace CL {
    using Position = cura::TimeEstimateCalculator::Position;
    //using Block = TimeEstimateCalculator::Block;


class PathTurnBackCalculator {
public:

    Position posPrevious, posCurrent, posNew;
    Position deltaVec0, deltaVec1;

    bool _hasTurnBack(Position newPos);

    bool hasTurnBack_45(Position newPos);

    bool hasTurnBack_0_90(Position newPos);
};


/*




    enum M1024_MASK: uint64{
        M1024_TakePicture = 1 << 0;    //  0b01
        M1024_AiContrast  = 1 << 1;    //  0b10
    }

    ((; layer (\d+).*\r\n)((.*\r\n){0, 10}?)(G1 X\d+.*\r\n)(G1 Z\d+.*\r\n)?)
    $1M1024 L$3/1200 3\r\n
*/
class SnapshotLayerCommand {
    int m_layer_count{  0 };    
    int m_layer_index{ -1 };    
    int m_snapCount  { 0 };     
    float step{ 1 };            
    int N     { 1 };            
    int m_layer_nr{ 0 };        

    std::stringstream ss;       
public:
    SnapshotLayerCommand();

    std::string getLayerList();

    void setLayerCount(const int layer_count);

    
    bool hasSnapshotByLayer(const int layer_nr);
    std::string getCmdSnapshotByLayer(cura::Point3 xyzPos, const int layer_nr);
    std::string getCmdSnapshotByLayer(cura::Point3 xyzPos);

    int finalize();
};


class VAJK_K_Cmd{
public:
    bool  ak_VAJK_K_enabled{false};
    double ak_K_print        {0.0};
    double ak_K_infill       {0.0};
    double ak_K_wall         {0.0};
    double ak_K_wall_0       {0.0};
    double ak_K_wall_x       {0.0};
    double ak_K_topbottom    {0.0};
    double ak_K_support      {0.0};
    double ak_K_skirt_brim   {0.0};
    double ak_K_layer_0      {0.0};

    void init(const cura::Settings& settings){
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
    };
    double getK_byType(const cura::PrintFeatureType &type);
    /*

    */
};


}



class CL_GCodeExportOptinize {
    cura::GCodeExport* m_gcodeExport{ nullptr };
    CL::PathTurnBackCalculator m_pathTurnBackCalculator;
    CL::SnapshotLayerCommand   m_snapshotLayerCommand;
    CL::VAJK_K_Cmd             m_VAJK_K_Cmd;

    bool enable_pathTurnBackCalculator;
    bool enable_snapshotLayerCommand;
    bool enable_VAJK_K;
public:
    CL_GCodeExportOptinize(cura::GCodeExport* gcodeExport) :m_gcodeExport(gcodeExport) {
        const cura::Settings& settingsScene = m_gcodeExport->getSettings_Scene();

        enable_pathTurnBackCalculator = false;
        enable_snapshotLayerCommand   = settingsScene.get<bool>("anker_param_ai_camera");
        enable_VAJK_K                 = settingsScene.get<bool>("ak_VAJK_K_enabled");

        m_VAJK_K_Cmd.init(settingsScene);
    }

    //  GCodeExport::writeFXYZE
    void TurnBackDelay_writeFXYZE(CL::Position newPos) {
        if (!enable_pathTurnBackCalculator) { return; }
        cura::Duration delayMs = 30;
        if (m_pathTurnBackCalculator.hasTurnBack_45(newPos)) {
            (*m_gcodeExport->output_stream) << "G4 P" << int(delayMs) << " ;-----------------" << m_gcodeExport->new_line;
        }
    }

    //  Snapshot 1  GCodeExport::writeLayerCountComment
    bool Snapshot_writeLayerCountComment(const size_t layer_count) {
        m_snapshotLayerCommand.setLayerCount(layer_count);
        return true;
    }

    //  Snapshot 2  GCodeExport::writeLayerComment
    bool Snapshot_writeLayerComment(const int layer_nr){
        if (!enable_snapshotLayerCommand) {
            //  fix @2022-05-25 by CL
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index();
            auto new_line   = m_gcodeExport->new_line;

            (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line;
            
            return false;
        }

        {   //  chenge @2022-05-12 by CL
            const cura::Settings& settingsScene = m_gcodeExport->getSettings_Scene();

            auto L_Posion   = m_gcodeExport->getPosition();
            auto Z          = m_gcodeExport->getPositionZ();
            auto L_E        = m_gcodeExport->clCurrentStatus->getE_block();
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index();
            auto L_Nr       = m_gcodeExport->clCurrentStatus->getL_Nr();
            auto L_Count    = m_gcodeExport->clCurrentStatus->getL_Count();
            auto new_line   = m_gcodeExport->new_line;
            auto retract_speed = settingsScene.get<cura::Velocity>("retraction_retract_speed");
            auto prime_speed = settingsScene.get<cura::Velocity>("retraction_prime_speed");
            auto travel_speed = settingsScene.get<cura::Velocity>("speed_travel");

            auto relative_extrusion = settingsScene.get<bool>("relative_extrusion");

            
            double ms = settingsScene.get<double>("camera_take_picture_time"); // = min *60
            if(ms < 1){ms=1;}
            double oldF = m_gcodeExport->clCurrentStatus->getF_block();
            double F = 1.0;    //  120 mm/min = 2mm/s   um/ms == mm/s
            int Z_um = F * ms;

            
            
            if(layer_nr == 1){ 
                
                cura::coord_t first_photo_z_offset = 50 * 1000;
                (*(m_gcodeExport->output_stream)) << "; Z_TakePictureStart: " << L_Nr << new_line;

                //2022/12/14  Binary for AI wipe 
                (*(m_gcodeExport->output_stream)) << "G1 F" << cura::PrecisionedDouble{ 1, retract_speed * 60 } << " E" << cura::PrecisionedDouble{ 5, relative_extrusion?-3:L_E-3} << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I3 J3 P1  F"<< cura::PrecisionedDouble{ 1, travel_speed * 60 }<< new_line;
                (*(m_gcodeExport->output_stream)) << "G0 X203 Y233 F" << cura::PrecisionedDouble{ 1, travel_speed * 60 } << new_line;

                (*(m_gcodeExport->output_stream)) << "G0  Z0.0 F600" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2 F600" << new_line;

                (*(m_gcodeExport->output_stream)) << "G0  Z0.1 F600" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2  F600" << new_line;

                (*(m_gcodeExport->output_stream)) << "G0  Z0.0 F600" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2  F600" << new_line;

                (*(m_gcodeExport->output_stream)) << "G0  Z0.1 F600" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2  F600" << new_line;

                (*(m_gcodeExport->output_stream)) << "G0  Z0.0 F600" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2  F600" << new_line;
                

                (*(m_gcodeExport->output_stream)) << "G0 F1200 Z"<< cura::MMtoStream{ Z } << new_line;

                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{ 1, travel_speed * 60 } << " X" << cura::MMtoStream{ L_Posion.x } << " Y" << cura::MMtoStream{ L_Posion.y }<< new_line;
                (*(m_gcodeExport->output_stream)) << "G0 F1200 Z" << cura::MMtoStream{ Z + first_photo_z_offset + Z_um } << new_line;
 
                (*(m_gcodeExport->output_stream)) << "M1024 L" << L_Index << "/" << L_Count << " 3" << new_line; // (res ? " 3" : " 1")
                (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line; // writeLayerComment(layer_nr);
                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{ 1,    F * 60 } << " Z" << cura::MMtoStream{ Z + first_photo_z_offset } << new_line;
                //G0 X210 Y233 F15000  G0 F1200 Z0.1
                (*(m_gcodeExport->output_stream)) << "G0 X210 Y233 F" << cura::PrecisionedDouble{ 1, travel_speed * 60 } << new_line;
                (*(m_gcodeExport->output_stream)) << "G0 F600 Z0.0" << new_line;
                (*(m_gcodeExport->output_stream)) << "G3 I2 J2 P2  F600" << new_line;



                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{1,   20 * 60} <<  " Z" << cura::MMtoStream{Z} << new_line;
                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{ 1, travel_speed * 60 } << " X" << cura::MMtoStream{ L_Posion.x } << " Y" << cura::MMtoStream{ L_Posion.y } << new_line;
                (*(m_gcodeExport->output_stream)) << "G1 F" << cura::PrecisionedDouble{ 1, retract_speed * 60 } << " E" << cura::PrecisionedDouble{ 5, relative_extrusion?3:L_E} << new_line;

                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{1, oldF * 60} << new_line;
            }
            else{
                (*(m_gcodeExport->output_stream)) << "; Z_TakePictureStart: " << L_Nr << new_line;
                (*(m_gcodeExport->output_stream)) << "M1024 L" << L_Index << "/" << L_Count << " 3" << new_line; // (res ? " 3" : " 1")
                (*(m_gcodeExport->output_stream)) << "G0 Z" << cura::MMtoStream{Z +Z_um} << new_line;
                (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line; // writeLayerComment(layer_nr);
                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{1,    F * 60} <<  " Z" << cura::MMtoStream{Z} << new_line;
                (*(m_gcodeExport->output_stream)) << "G0 F" << cura::PrecisionedDouble{1, oldF * 60} << new_line;
            }
            return true;
        }

        //if(clCurrentStatus->getL_Nr() > 0)

        //cura::Point3 xyzPos = m_gcodeExport->getPosition();
        //std::string m1024cmd = m_snapshotLayerCommand.getCmdSnapshotByLayer(xyzPos, layer_nr);
        //m_gcodeExport->writeLine(m1024cmd.c_str());
    }

    
    bool Snapshot_finalize() {
        if (!enable_snapshotLayerCommand) {
            //  fix @2022-05-25 by CL
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index() + 1;
            auto new_line   = m_gcodeExport->new_line;

            (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line;
            //(*(m_gcodeExport->output_stream)) << ";End Of Model Gcode" << new_line;
            
            return false;
        }


        {   //  chenge @2022-05-12 by CL
            auto Z          = m_gcodeExport->getPositionZ();
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index() + 1;
            auto L_Nr       = m_gcodeExport->clCurrentStatus->getL_Nr() + 1;
            auto L_Count    = m_gcodeExport->clCurrentStatus->getL_Count();
            auto new_line   = m_gcodeExport->new_line;

            (*(m_gcodeExport->output_stream)) << "M1024 L" << L_Index << "/" << L_Count << " 3" << new_line; // (res ? " 3" : " 1")
            (*(m_gcodeExport->output_stream)) << "G0 Z" << cura::MMtoStream{Z + 2000} << "; Z_TakePictureStart: " << L_Nr << new_line;
            (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line; // writeLayerComment(layer_nr);
            return true;
        }

        //int layer_nr = m_snapshotLayerCommand.finalize();
        //cura::Point3 xyzPos = m_gcodeExport->getPosition();
        //std::string m1024cmd = m_snapshotLayerCommand.getCmdSnapshotByLayer(xyzPos);
        //m_gcodeExport->writeLine(m1024cmd.c_str());
        //this->writeComment(m_snapshotLayerCommand.getLayerList());
    }

    
    void VAJK_K_writeTypeComment(const cura::PrintFeatureType &type){
        if(!enable_VAJK_K){return;}

        double K = m_VAJK_K_Cmd.getK_byType(type);
        m_gcodeExport->writePrintK_M900(K);
    }
};


#endif // define GCODEEXPORT_OPTIMIZE_CL_H
