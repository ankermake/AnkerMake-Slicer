#ifndef GCODEEXPORT_OPTIMIZE_CL_H
#define GCODEEXPORT_OPTIMIZE_CL_H

#include "../gcodeExport.h"
#include "../utils/string.h" // MMtoStream, PrecisionedDouble
#include "../LayerPlan.h"
#include "../utils/logoutput.h"

namespace CL {
    using Position = cura::TimeEstimateCalculator::Position;
    using Block    = cura::TimeEstimateCalculator::Block;
    using UM2MM    = cura::MMtoStream;
    using double2SS= cura::PrecisionedDouble;
    using UM       = cura::coord_t;

    template<typename BaseType>
    bool std_contain(const std::vector<BaseType> & container, const BaseType & it ){
        return std::find(container.begin(), container.end(), it) != container.end();
    }
    template<typename BaseType>
   void std_sort(std::vector<BaseType> & container){
        std::sort(container.begin(), container.end());
    }


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

    void init(const cura::Settings& settings);              
    double getK_byType(const cura::PrintFeatureType &type);
};




/*




    enum M1024_MASK: uint64{
        M1024_TakePicture = 1 << 0;    //  0b01
        M1024_AiContrast  = 1 << 1;    //  0b10
    }

    ((; layer (\d+).*\r\n)((.*\r\n){0, 10}?)(G1 X\d+.*\r\n)(G1 Z\d+.*\r\n)?)
    $1M1024 L$3/1200 3\r\n
*/
//  add @2023-02-02 by ChunLian
class SnapshotLayerCommand{
    cura::GCodeExport* m_gcodeExport{ nullptr };

    
public:
    bool snapshot_layer_M1024_enable0   {false};            const std::string key_snapshot_layer_M1024_enable0  {"anker_abc_0"};  //  add @2023-03-28 by ChunLian
    bool snapshot_layer_M1024_enable    {false};            const std::string key_snapshot_layer_M1024_enable   {"anker_param_ai_camera"};
    bool snapshot_uplayer_enable        {false};

    double               anker_camera_take_picture_time{1}; const std::string key_camera_take_picture_time      {"camera_take_picture_time"};
    std::vector<int>     anker_param_ai_uplayer_array;      const std::string key_snapshot_uplayer_array        {"anker_abc_1"};  
    std::vector<double>  anker_param_ai_height_array ;      const std::string key_snapshot_height_array         {"anker_abc_2"};  //  @2023-02-23 by ChunLian

    int snapshot_count {0};

public:

    
    void init(cura::GCodeExport* gcodeExport);

    //  Snapshot 1  GCodeExport::writeLayerCountComment
    bool snapshot_writeLayerCountComment(const size_t layer_count);

    //  Snapshot 2  GCodeExport::writeLayerComment
    bool snapshot_writeLayerComment(const int layer_nr);//  end of function snapshot_writeLayerComment

    
    bool snapshot_finalize();

};

}



class CL_GCodeExportOptinize {
    cura::GCodeExport* m_gcodeExport{ nullptr };
    CL::VAJK_K_Cmd             m_VAJK_K_Cmd;
    CL::SnapshotLayerCommand   m_M1024_Cmd;


public:
    CL_GCodeExportOptinize(cura::GCodeExport* gcodeExport) :m_gcodeExport(gcodeExport) {
        const cura::Settings& settingsScene = m_gcodeExport->getSettings_Scene();

        m_VAJK_K_Cmd.init   (settingsScene);    //  Snapshot 0
        m_M1024_Cmd.init    (m_gcodeExport);
    }

    //  Snapshot 1  GCodeExport::writeLayerCountComment
    bool GCodeExport_writeLayerCountComment(const size_t layer_count) {
        if(m_M1024_Cmd.snapshot_layer_M1024_enable){
            return m_M1024_Cmd.snapshot_writeLayerCountComment(layer_count);
        }

        {
            return false;
        }
    }

    //  Snapshot 2  GCodeExport::writeLayerComment
    bool GCodeExport_writeLayerComment(const int layer_nr){
        if(m_M1024_Cmd.snapshot_layer_M1024_enable){
            return m_M1024_Cmd.snapshot_writeLayerComment(layer_nr);
        }

        {
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index();
            auto new_line   = m_gcodeExport->new_line;
            (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line;
            return false;
        }
    }

    
    bool GCodeExport_finalize() {
        if(m_M1024_Cmd.snapshot_layer_M1024_enable){
            return m_M1024_Cmd.snapshot_finalize();
        }

        {
            auto L_Index    = m_gcodeExport->clCurrentStatus->getL_Index() + 1;
            auto new_line   = m_gcodeExport->new_line;
            (*(m_gcodeExport->output_stream)) << ";LAYER:" << L_Index << new_line;
            return false;
        }
    }

    
    void GCodeExport_writeTypeComment(const cura::PrintFeatureType &type){
        if(m_VAJK_K_Cmd.ak_VAJK_K_enabled){
            double K = m_VAJK_K_Cmd.getK_byType(type);
            m_gcodeExport->writePrintK_M900(K);
        }
    }
};


#endif // define GCODEEXPORT_OPTIMIZE_CL_H
