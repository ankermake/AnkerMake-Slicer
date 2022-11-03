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

bool PathTurnBackCalculator::_hasTurnBack(Position newPos) {
    posPrevious = posCurrent; posCurrent = posNew; posNew = newPos;

    if (posPrevious.isZero() || posCurrent.isZero() || posNew.isZero()) { return false; }
    for (int axis_id = 0; axis_id < 4; ++axis_id) {
        deltaVec0[axis_id] = posCurrent[axis_id] - posPrevious[axis_id];
        deltaVec1[axis_id] = posNew[axis_id] - posCurrent[axis_id];
    }
    return true;
}
bool PathTurnBackCalculator::hasTurnBack_45(Position newPos) {
    if (!_hasTurnBack(newPos)) { return false; }
    double A0, B0, A1, B1;
    A0 = deltaVec0.x + deltaVec0.y; B0 = deltaVec0.x - deltaVec0.y;
    A1 = deltaVec1.x + deltaVec1.y; B1 = deltaVec1.x - deltaVec1.y;

    if (std::abs(A0) > 0.001 && A0 * A1 < 1.0e-8) { return true; }
    if (std::abs(B0) > 0.001 && B0 * B1 < 1.0e-8) { return true; }
    return false;
    // if return true, insert a line : "G4 P30\n"
}
bool PathTurnBackCalculator::hasTurnBack_0_90(Position newPos) {
    if (!_hasTurnBack(newPos)) { return false; }
    double xx = deltaVec0.x * deltaVec1.x;
    double yy = deltaVec0.y * deltaVec1.y;
    return (xx < 1.0e-8) || (yy < 1.0e-8);  // <= 0
    // if return true, insert a line : "G4 P30\n"
}


SnapshotLayerCommand::SnapshotLayerCommand(){
    ss << "[";
}

std::string SnapshotLayerCommand::getLayerList() {
    return ss.str();
}

void SnapshotLayerCommand::setLayerCount(const int layer_count)
{
    m_layer_count = layer_count;
    m_layer_index = -1;
    m_snapCount   = 0;
    step = m_layer_count / (100.0-1.0 * N); 
}

bool SnapshotLayerCommand::hasSnapshotByLayer(const int layer_nr)
{
    //return true; // add @2022-03-15 by CL

    m_layer_index++;
    m_layer_nr = layer_nr;

    bool res = false;
    if(true){ 
        if (m_layer_nr < 0) {
            m_layer_count++;        
        }
        m_snapCount++;
        res = true;
    }else 
        do{
        if(m_layer_index == 0){
            res = false;    
            break;
        }
        if (m_layer_nr < 0) {
            m_layer_count++;        
            m_snapCount++;            
            step = m_layer_count / (100.0-1.0 * m_snapCount);     
            res = true;
            break;
        }
        if(m_snapCount < N && (1 <= m_layer_nr && m_layer_nr <= N)){
            m_snapCount++;
            step = m_layer_count / (100.0-1.0 * m_snapCount);
            res = true;     
            break;
        }

        
        int todoLayer = m_layer_count - step * (100 - (m_snapCount + 1));
        if(m_layer_index >= todoLayer){
            m_snapCount++;
            res = true;
            break;
        }
    }while(false);

    if(res){
        ss << " (N" << m_layer_nr  << ", I" << m_layer_index <<
              ", P" << m_snapCount << ", C" << m_layer_count << ")";
    }

    return res;
}

std::string SnapshotLayerCommand::getCmdSnapshotByLayer(cura::Point3 xyzPos, const int layer_nr)
{
    bool res = hasSnapshotByLayer(layer_nr);
    std::stringstream m1024ss;

    m1024ss << "M1024 L" << m_layer_index << "/" << m_layer_count << (res ? " 3" : " 1");
    m1024ss << "\n";
    m1024ss << ";(N" << m_layer_nr  << ", I" << m_layer_index <<
               ", P" << m_snapCount << ", C" << m_layer_count << ")";

    return m1024ss.str();
}

std::string SnapshotLayerCommand::getCmdSnapshotByLayer(cura::Point3 xyzPos)
{
    //        bool res = hasSnapshotByLayer(m_layer_nr + 1);
    //        std::stringstream m1024ss;

    //        m1024ss << "M1024 L" << m_layer_index << "/" << m_layer_count << (res ? " 3" : " 1");
    //        m1024ss << "\n";
    //        m1024ss << ";(N" << m_layer_nr  << ", I" << m_layer_index <<
    //                   ", P" << m_snapCount << ", C" << m_layer_count;

    std::stringstream m1024ss;
    m_layer_index++;
    m_layer_nr++;
    m_snapCount++;

    m1024ss << "M1024 L" << m_layer_index << "/" << m_layer_count << " 3";
    m1024ss << "\n";
    m1024ss << ";(N" << m_layer_nr  << ", I" << m_layer_index <<
               ", P" << m_snapCount << ", C" << m_layer_count;

    return m1024ss.str();
}

int SnapshotLayerCommand::finalize() {
    ss << (m_layer_count+1) << "]";
    std::cout << "\n\n" << ss.str() << "\n\n" << std::endl;
    return m_layer_count;
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

}




