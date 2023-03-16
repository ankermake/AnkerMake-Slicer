#ifndef AK_CONST_H
#define AK_CONST_H
#include <QString>

namespace AkConst{

namespace Time {
    const int MINUTE_SECOND = 60;
    const int MINUTE_M_SECOND = 60*1000;
}

namespace Control {
    const char M_FILE[]  =  "Menu.File";
    const char M_EDIT[]  =  "Menu.Edit";
    const char M_CONNECT[]  =  "Menu.Connect";
    const char M_SETTINGS[]  =  "Menu.Settings";
    const char M_HELP[]  =  "Menu.Help";
}


namespace Plugin{
    const QString AK_MAIN_WINDOW    = "AkMainWindow";   
    const QString FDM_EDIT_MODEL    = "FdmEditModel";
    const QString FDM_SLICER        = "FdmSlicer";
    const QString FDM_SETTING       = "FdmSetting";
    const QString FDM_GCODE_PARSER  = "FdmGcodeParser";
    const QString FDM_AI_CAMERA     = "FdmAiCamera";
    const QString FDM_NETWORK       = "FdmNetwork";
    const QString FDM_SEND2ALL      = "fdmSend2All";    
}


namespace Param {
    //InitGui
    const QString STL_FILE = "stlFile";
    const QString CUSTOM_SETTING = "customSetting";
    const QString GCODE_FILE = "gcodeFile";
    const QString AKPIC_FILE = "akpicFile";
    const QString GL_CONTEXT = "glContext";
    const QString GL_FORMAT = "glFormat";
    const QString GCODE_VIEWER = "gcodeViewer";
    const QString ORIGINAL_STL_NAME = "originalStlName";
    const QString HOST_ADDRESS = "hostAddress";
    
    const QString MODEL_ALL_IN_PRINT_BOX = "ModelAllInPrintBox";
    const QString VISIBLE_MODEL_COUNT = "VisibleModelCount";
    const QString SUPPORT_MESH_NUMBER = "SupportMeshNumber";
    const QString GLOBAL_SUPPORT_DISABLED_STATUS = "GlobalSupportDisabledStatus";
    //gcode preview Gcode wid
    const QString GCODE_PREVIEW_WID = "gcodePreviewWid";
    const QString LOGGING_STATUS = "loggingStatus";

    const QString AIMODE_STATE = "AiModeState";
    const QString LANGUAGE_STATE = "languageState";

    //Update
    const QString ANKERMAKE_VERSION = "AnkerMakeVersion";
    const QString UPDATE_DO = "UpdateDo";
    const QString ANKERMAKE_UPDATE_EXE = "AnkerMakeUpdateExe"; 

    
    const QString ANKERMAKE_MAINWINDOW_WID = "AnkerMakeMainwindowWid";
    const QString NETWORK_MAINWINDOW_WID = "NetworkMainwindowWid";

    
    const QString A_KEY_PRINT_FILE_PATH = "AKeyPrintFilePath";

//    //const QString SLICE_PANEL_STATE = "slicePanelState";

//    const QString MODEL_OUT_OF_RANGE = "ModelOutofRange";

//    const QString MODEL_CHANGE_TYPE = "ModelChangeType";
}


//enum ModelChangeType
//{

//    ModelChanged,

//    ModelTransfromChanged,
//};


namespace Msg {
    //InitGui
    const QString INIT_GUI = "Plugin UI initialization running in the QMainWindow constructor";

    const QString SAVE_STL_FILE = "SaveStlFile";

    const QString GET_CUSTOM_SETTING = "GetCustomSetting";
    
    const QString PREVIEW_GCODE = "PreviewGcode";
    const QString PREVIEW_GCODE_BY_DRAW = "PreviewGcodeByDraw";

    
    const QString GET_STL_FILE = "GetStlFile";

    
    //const QString SET_SLICE_PANEL_STATE = "setSlicePanelState";

    
    const QString OFF_SCREEN_RENDER = "OffScreenRender";

    
    const QString OFF_SCREEN_RENDER_EXPORT = "OffScreenRenderExport";

    
    //const QString DISABLE_SLICE_PANEL = "DisableSlicePanel";

    
    const QString MODEL_TRANSFROM_CHANGED = "ModelTransformChanged";
    
    const QString MODEL_STATUS_CHANGED = "ModelStatusChanged";
    
    const QString MODEL_OUT_OF_RANGE_STATE_CHANGED = "ModelOutOfRangeStateChanged";
    const QString VISIBLE_MODEL_COUNT_CHANGED = "VisibleModelCountChanged";
    
    const QString MODEL_SUPPORT_NUMBER_CHANGED = "ModelSupportNumberChanged";

    const QString PARAMETER_CHANGED = "ParameterChanged";
    const QString SLICE_START = "SliceStart";
    const QString SLICE_SUCCESS = "SliceSuccess";
    const QString SLICE_FAILED = "SliceFailed";
    const QString SLICE_RESET = "SliceReset";
    const QString GLOBAL_SUPPORT_DISABLED = "GlobalSupportDisabled";
    const QString GET_GLOBAL_SUPPORT_DISABLED_STATUS = "GetGlobalSupportDisabledStatus";

    const QString SUPPORT_PROCESSING_START = "SupportProcessingStart";
    const QString SUPPORT_PROCESSING_END = "SupportProcessingEnd";

    
    const QString GET_PREVIEW_WID = "GetPreviewWid";
    const QString OPEN_PREVIEW_WITHOUT_FILE = "OpenPreviewWithoutFile";
    const QString OPEN_PREVIEW_IN_NETWORK = "OpenPreviewInNetwork";
    const QString CHECKOUT_PREVIEW = "CheckoutPreview";
    const QString GET_LOGGING_STATUS = "getLoggingStatus";

    //AI mode
    const QString AIMODE_CHANGED = "AiMode";

    //Language
    const QString LANGUAGE_CHANGED = "Language";

    
    const QString MANUAL_UPDATE = "ManualUpdate";
    const QString AUTO_UPDATE = "AutoUpdate";

    
    const QString A_KEY_PRINT = "AKeyPrint";

    
    const QString ANKERMAKE_LOGIN = "AnkerMakeLogin";

    
    const QString NETWORK_MAINWINDOW_WID = "NetworkMainwindowWid";

    
    const QString ANKERMAKE_MAINWINDOW_PTR = "AnkerMakeMainwindowPtr";

    
    const QString SWITCH_NETWORK_WIDGET = "SwitchNetworkWidget";

    
    const QString NETWORK_DEAD = "NetworkDead";

    
    const QString ANKERMAKE_MAINWINDOW_MODAL = "AnkerMakeMainwindowModal";

    const QString MAIN_WINDOW_INIT_FINISHED = "MainWindowInitFinished";
}


namespace Category
{
    const QString BASE_PARAM = "base_param";
    const QString AK_META = "ak_meta";
    const QString AK_META_REALTIME = "ak_meta_realtime";
    const QString AK_AI = "ak_ai";
    const QString MACHINE_SETTINGS = "machine_settings";
    const QString MATERIAL = "material";
    const QString EXTRUDER_SETTINGS = "extruder_settings";
    const QString INFILL = "infill";
    const QString RESOLUTION = "resolution";
    const QString SUPPORT = "support";
    const QString PLATFORM_ADHESION = "platform_adhesion";
}

namespace SettingKey
{
    const int MAX_NAME_SIZE = 80;
    //ak_meta
    //const QString META_PROFILE_ID = "meta_profile_id";
    const QString META_PROFILE_NAME = "meta_profile_name";
    const QString META_PROFILE_VISIBLE = "meta_profile_visible";
    const QString META_MODIFY_TIME = "meta_modify_time";
    const QString META_CREATE_TIME = "meta_create_time";
    const QString META_MODIFY_TIME_STR = "meta_modify_time_str";
    const QString META_CREATE_TIME_STR = "meta_create_time_str";
    
    //const QString META_CURRENT_SELECT_NAME = "meta_current_select_name";
    //const QString META_CURRENT_PROFILE_ID = "meta_current_profile_id";
    //const QString META_CURRENT_MACHINE_ID = "meta_current_machine_id";
    //const QString META_CURRENT_MATERIAL_ID = "meta_current_material_id";
    // 
    //ak_meta_param_profile
    const QString META_CURRENT_MACHINE_NAME = "meta_current_machine_name";
    const QString META_CURRENT_MATERIAL_NAME = "meta_current_material_name";
    const QString META_CURRENT_NOZZLE_NAME = "meta_current_nozzle_name";

    //ak_meta_realtime
    const QString META_CURRENT_PROFILE_NAME = "meta_current_profile_name";

    //const QString META_CURRENT_PROFILE_GROUP = "meta_current_profile_group";
    //const QString PARAMETER_MACHINE_NAME = "parameter_machine_name";
    //const QString PARAMETER_MATERIAL_NAME = "parameter_material_name";

    //ak_ai
    const QString AI_QUALITY_INDEX = "ai_quality_index";
    const QString AI_INFILL_DENSITY_INDEX = "ai_infill_density_index";
    const QString AI_QUALITY_LIST = "ai_quality_list";
    const QString AI_INFILL_DENSITY = "ai_infill_density";
    const QString AI_GLOBAL_SUPPORT_STATE = "ai_global_support_state";
    const QString AI_GENERATE_ADHESION_STATE = "ai_Generate_adhesion_state";
    const QString AI_ADHESION_TYPE = "ai_adhesion_type";

    const QString LAYER_HEIGHT = "layer_height";
    const QString LAYER_HEIGHT_0 = "layer_height_0";
    const QString INFILL_SPARSE_DENSITY = "infill_sparse_density";
    const QString MACHINE_EXTRUDER_COUNT = "machine_extruder_count";
    const QString EXTRUDERS_ENABLED_COUNT = "extruders_enabled_count";
    //const QString AK_MACHINE_ID = "ak_machine_id";
    //const QString AK_MATERIAL_ID = "ak_material_id";
    const QString MACHINE_NAME = "machine_name";
    const QString MATERIAL_NAME = "material_name";

    const QString SUPPORT_ENABLE = "support_enable";
    const QString ADHESION_TYPE = "adhesion_type";

    const QString MACHINE_NOZZLE_SIZE = "machine_nozzle_size";

    const QString EXTRACT_PARAM_FROM_GCODE = "extract_param_from_gcode";
}

namespace GlobalParameterKeys{
     const QString ANKER_MACHINE_SIZE   = "AnkerMake::Machine::Size";
     const QString ANKER_MACHINE_CAMERA = "AnkerMake::Machine::Camera";
     const QString ANKER_SUPPORT_ENABLE = "AnkerMake::Support::SupportEnable";
     const QString ANKER_SUPPORT_ANGLE  = "AnkerMake::Support::SupportAngle";
	 const QString ANKER_SCENE_PARAM = "AnkerMake::Scene::Params";
}

namespace ProfileName{
     const QString EXPERT_MODE   = "Expert Mode";
     const QString SIMPLE_MODE       = "Easy Mode";
     const QString More     = "More...";

     const QString X5_ABS = "X5_ABS";
     const QString X5_PLA_PLUS = "X5_PLA_PLUS";
     const QString X5_PETG = "X5_PETG";
     const QString X5_TPU = "X5_TPU";
}
namespace MachineName {
    const QString M5 = "AnkerMake M5";
    const QString M10 = "AnkerMake M10";
}
namespace MaterialName {
    const QString ABS = "ABS";
    const QString PLAPLUS = "PLA+";
    const QString PETG = "PETG";
    const QString TPU = "TPU";
}
namespace NozzleSizeName {
    const QString SIZE2 = "0.2 mm";
    const QString SIZE4 = "0.4 mm";
    const QString SIZE6 = "0.6 mm";
    const QString SIZE8 = "0.8 mm";
}

enum EGlobalSupportForbiddenState
{
    Forbidden = 0,
    Permit,
};


enum EGlobalSupportState
{
    Unactived = 0,
    Selected ,
    UnSelected	,
};


enum EGlobalSupportTextCode
{
    
    GlobalSupportUnactived = 0,
    
    GlobalSupportAndManualSupport,
    
    GlobalSupportOnly,
    
    GlobalSupportUnSelected,
};


enum EGenerateAdhesionState
{
    GenAdhesionSelected = 0,
    GenAdhesionUnSelected,
};


//fdmOptions:{
//    "skirt": "Skirt",
//    "brim": "Brim",
//    "raft": "Raft",
//    "none": "None"
//}

namespace EBuildPlateAdhesionType {
    const QString Skirt = "skirt";
    const QString Brim = "brim";
    const QString Raft = "raft";
    const QString None = "none";
}
//

//enum EBuildPlateAdhesionType
//{
//    AdType_Skirt = 0,
//    AdType_Brim,
//    AdType_Raft,
//    AdType_None
////    AdType_Brim = 0,
////    AdType_None,
////    AdType_Raft,
////    AdType_Skirt
//};




enum EWidgetType
{
    General     = 0,
    Machine	,
    Material,
    Parameter
};


enum EFdmRightWidgetIndex
{
    SettingPanel = 0,
    SavePanel,
    SlicePanel
};

enum EOpenPreferenceSource
{
    Init = 0,
    RightSetting,
    Menu,
};

enum FDMMeshTransForm
{
    Move = 0,
    Zoom,
    Rotate,
    Mirror,
    Support,
};

enum SupportModeIndex
{
    SptModeTree = 0,
    SptModeCuboid,
};

enum NetworkUpdateType
{
    NetworkUpdateType_ManualUpdate = 0,
    NetworkUpdateType_AutoUpdate,
};

//#define More "More..."

}
#endif // AK_CONST_H
