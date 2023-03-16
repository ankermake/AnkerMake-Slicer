import QtQuick 2.2
import FdmQml 1.0
// qrc:/Settings/FdmJsonObjTree_All.qml
FdmQml_Root{ id:fdmextruder_def_json; objectName: "qrc:/Settings/FdmJsonObjTree_All.qml"
    property string name: "FDM Printer Base Description"
    property int version: 2
    FdmQml_Settings{ id:settings; objectName: "settings"
        FdmQml_Category{ id:anker_fdm_setting; objectName: "anker_fdm_setting"
            fdmLabel: "AnkerMake FDM Settings"
            fdmType: "category"
            fdmDescription: "Customized for AnkerMake"
            fdmIcon: "AnkerFdmSettings"
            FdmQml_Param{ id:international_language; objectName: "international_language"
                fdmLabel: "Languages"
                fdmDescription: "International Language Switch"
                fdmType: "bool"
                fdmDefaultValue: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:anker_param_ai_camera; objectName: "anker_param_ai_camera"
                fdmLabel: "AnkerMake AI Camera"
                fdmDescription: "Take pictures or record timelapses of your 3D prints."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
                FdmQml_Param{ id:camera_take_picture_time; objectName: "camera_take_picture_time"
                    fdmLabel: "Timelapse Settings"
                    fdmDescription: "Set the parameters of your timelapse."
                    fdmUnit: "ms"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:anker_abc_1; objectName: "anker_abc_1"
                    //  ( Non-chinese will not be displayed )
                    fdmLabel: ""
                    fdmDescription: ""
                    fdmType: "[int]"
                    fdmDefaultValue: "[ ]"
                    fdmEnabled: anker_param_ai_camera.fdmValue && (fdmLabel.length > 3 && fdmDescription.length > 3)
                }
                FdmQml_Param{ id:anker_abc_2; objectName: "anker_abc_2"
                    //  ( Non-chinese will not be displayed )
                    fdmLabel: ""
                    fdmDescription: ""
                    fdmType: "[int]"
                    fdmDefaultValue: "[ ]"
                    fdmEnabled: anker_param_ai_camera.fdmValue && (fdmLabel.length > 3 && fdmDescription.length > 3)
                }
                FdmQml_Param{ id:camera_position; objectName: "camera_position"
                    fdmLabel: "Camera Position"
                    fdmDescription: "The location of your camera in relation to your print."
                    fdmType: "str"
                    fdmValue: "[" + camera_position_x.fdmValue + ", " + camera_position_y.fdmValue + ", " + camera_position_z.fdmValue + "]"
                    fdmDefaultValue: "[ ]"
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "camera_position_x,camera_position_y,camera_position_z"
                    FdmQml_Param{ id:camera_position_x; objectName: "camera_position_x"
                        fdmLabel: "X-Position"
                        fdmDescription: "The location of your camera on the X axis."
                        fdmType: "float"
                        fdmDefaultValue: 288.78
                        fdmUnit: "mm"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_position_y; objectName: "camera_position_y"
                        fdmLabel: "Y-Position"
                        fdmDescription: "The location of your camera on the Y axis."
                        fdmType: "float"
                        fdmDefaultValue: 20.27
                        fdmUnit: "mm"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_position_z; objectName: "camera_position_z"
                        fdmLabel: "Z-Position"
                        fdmDescription: "The location of your camera on the Z axis."
                        fdmDefaultValue: 9.0
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:camera_up; objectName: "camera_up"
                    fdmLabel: "Camera Vertical Movement"
                    fdmDescription: "Move the camera up or down."
                    fdmDefaultValue: ""
                    fdmType: "str"
                    fdmValue: "[" + camera_up_x.fdmValue + ", " + camera_up_y.fdmValue + ", " + camera_up_z.fdmValue + "]"
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "camera_up_x,camera_up_y,camera_up_z"
                    FdmQml_Param{ id:camera_up_x; objectName: "camera_up_x"
                        fdmLabel: "camera up dirction X"
                        fdmDescription: "camera up dirction X"
                        fdmDefaultValue: -0.5
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_up_y; objectName: "camera_up_y"
                        fdmLabel: "camera up dirction Y"
                        fdmDescription: "camera up dirction Y."
                        fdmDefaultValue: 0.0
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_up_z; objectName: "camera_up_z"
                        fdmLabel: "camera up dirction Z"
                        fdmDescription: "camera up dirction Z."
                        fdmDefaultValue: 0.866
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:camera_view_vector; objectName: "camera_view_vector"
                    fdmLabel: "visual center position"
                    fdmDescription: "visual center position."
                    fdmDefaultValue: ""
                    fdmType: "str"
                    fdmValue: "[" + camera_view_vector_x.fdmValue + ", " + camera_view_vector_y.fdmValue + ", " + camera_view_vector_z.fdmValue + "]"
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "camera_view_vector_x,camera_view_vector_y,camera_view_vector_z"
                    FdmQml_Param{ id:camera_view_vector_x; objectName: "camera_view_vector_x"
                        fdmLabel: "visual center position X"
                        fdmDescription: "visual center position X."
                        fdmDefaultValue: -0.866
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_view_vector_y; objectName: "camera_view_vector_y"
                        fdmLabel: "visual center position Y"
                        fdmDescription: "visual center position Y."
                        fdmDefaultValue: 0.0
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_view_vector_z; objectName: "camera_view_vector_z"
                        fdmLabel: "visual center position Z"
                        fdmDescription: "visual center position Z."
                        fdmDefaultValue: -0.5
                        fdmType: "float"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:camera_resolution; objectName: "camera_resolution"
                    fdmLabel: "Camera Resolution"
                    fdmDescription: "camera resolution."
                    fdmDefaultValue: ""
                    fdmType: "str"
                    fdmValue: "" + camera_resolution_w.fdmValue + " × " + camera_resolution_h.fdmValue
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "camera_resolution_w,camera_resolution_h"
                    FdmQml_Param{ id:camera_resolution_w; objectName: "camera_resolution_w"
                        fdmLabel: "Camera Resolution Width"
                        fdmDescription: "camera resolution width."
                        fdmDefaultValue: 1920
                        fdmType: "int"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:camera_resolution_h; objectName: "camera_resolution_h"
                        fdmLabel: "Camera Resolution Height"
                        fdmDescription: "camera resolution height"
                        fdmDefaultValue: 1080
                        fdmType: "int"
                        fdmEnabled: anker_param_ai_camera.fdmValue
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmSettablePerMeshgroup: false
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:camera_field_view; objectName: "camera_field_view"
                    fdmLabel: "Vertical Field View"
                    fdmDescription: "Vertical Field View."
                    fdmType: "float"
                    fdmUnit: "°"
                    fdmDefaultValue: 60.0
                    fdmEnabled: anker_param_ai_camera.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:ak_meshfix_dynamic_resolution; objectName: "ak_meshfix_dynamic_resolution"
                fdmLabel: "Dynamic Resolution"
                fdmDescription: "Input the MeshFix resolution detail."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:meshfix_maximum_resolution; objectName: "meshfix_maximum_resolution"
                fdmLabel: "Maximum Resolution"
                fdmDescription: "The minimum size of a line segment after slicing. If you increase this, the mesh will have a lower resolution. This allows the printer to keep up with its G-Code processing speed and will increase slice speed by removing details of the mesh that it can't process."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.5
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.01
                fdmMaximumValueWarning: 3.0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:meshfix_maximum_travel_resolution; objectName: "meshfix_maximum_travel_resolution"
                fdmLabel: "Maximum Travel Resolution"
                fdmDescription: "The minimum size of a travel line segment after slicing. If you increase this, the travel moves will have corners that are less smooth. This allows the printer to keep up with its G-Code processing, but it will cause model avoidance to become less accurate."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 1.0
                fdmValue: Math.min(Number(meshfix_maximum_resolution.fdmValue) * Number(speed_travel.fdmValue) / Number(speed_print.fdmValue), 2 * Number(line_width.fdmValue))
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.05
                fdmMaximumValueWarning: 10.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "meshfix_maximum_resolution,line_width,speed_travel,speed_print"
            }
            FdmQml_Param{ id:meshfix_maximum_deviation; objectName: "meshfix_maximum_deviation"
                fdmLabel: "Maximum Deviation"
                fdmDescription: "The maximum deviation allowed when reducing the Maximum Resolution setting. If you increase this, the print will be less accurate, but the G-Code will be smaller. Note, Maximum Deviation is a limit for Maximum Resolution."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.025
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.01
                fdmMaximumValueWarning: 0.3
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:relative_extrusion; objectName: "relative_extrusion"
                fdmLabel: "Relative Extrusion"
                fdmDescription: "Use relative extrusion rather than absolute extrusion. Using relative E-steps makes for easier post-processing of the g-code. However, it's not supported by all printers and it may produce very slight deviations in the amount of deposited material compared to absolute E-steps. Irrespective of this setting, the extrusion mode will always be set to absolute before any g-code script is output."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: machine_gcode_flavor.fdmValue === "RepRap (RepRap)"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_gcode_flavor"
            }
            FdmQml_Param{ id:ak_VAJK_K_enabled; objectName: "ak_VAJK_K_enabled"
                fdmLabel: "Linear Advance"
                fdmDescription: "Predicts pressure build-up in the extruder when printing at high speeds."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmAffectedById: ""
                FdmQml_Param{ id:ak_K_print; objectName: "ak_K_print"
                    fdmLabel: "Linear Advance"
                    fdmDescription: "Predicts pressure build-up in the extruder when printing at high speeds."
                    fdmDefaultValue: 0.0
                    fdmType: "float"
                    fdmEnabled: ak_VAJK_K_enabled.fdmValue
                    fdmAffectedById: ""
                    FdmQml_Param{ id:ak_K_infill; objectName: "ak_K_infill"
                        fdmLabel: "Infill"
                        fdmDescription: "Infill Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                    }
                    FdmQml_Param{ id:ak_K_wall; objectName: "ak_K_wall"
                        fdmLabel: "Wall"
                        fdmDescription: "Wall Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                        FdmQml_Param{ id:ak_K_wall_0; objectName: "ak_K_wall_0"
                            fdmLabel: "Outer Wall"
                            fdmDescription: "Outer Wall Linear Advance"
                            fdmDefaultValue: 0.0
                            fdmValue: Number(ak_K_wall.fdmValue)
                            fdmType: "float"
                            fdmEnabled: ak_VAJK_K_enabled.fdmValue
                            fdmAffectedById: "ak_K_wall"
                        }
                        FdmQml_Param{ id:ak_K_wall_x; objectName: "ak_K_wall_x"
                            fdmLabel: "Inner Wall"
                            fdmDescription: "Inner Wall Linear Advance"
                            fdmDefaultValue: 0.0
                            fdmValue: Number(ak_K_wall.fdmValue)
                            fdmType: "float"
                            fdmEnabled: ak_VAJK_K_enabled.fdmValue
                            fdmAffectedById: "ak_K_wall"
                        }
                    }
                    FdmQml_Param{ id:ak_K_topbottom; objectName: "ak_K_topbottom"
                        fdmLabel: "Skin"
                        fdmDescription: "Top & Bottom Skin Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                    }
                    FdmQml_Param{ id:ak_K_support; objectName: "ak_K_support"
                        fdmLabel: "Support"
                        fdmDescription: "Support Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                    }
                    FdmQml_Param{ id:ak_K_skirt_brim; objectName: "ak_K_skirt_brim"
                        fdmLabel: "Skirts & Brims"
                        fdmDescription: "Skirts & Brims Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                    }
                    FdmQml_Param{ id:ak_K_layer_0; objectName: "ak_K_layer_0"
                        fdmLabel: "Initial Layer"
                        fdmDescription: "Initial Layer Linear Advance"
                        fdmDefaultValue: 0.0
                        fdmValue: Number(ak_K_print.fdmValue)
                        fdmType: "float"
                        fdmEnabled: ak_VAJK_K_enabled.fdmValue
                        fdmAffectedById: "ak_K_print"
                    }
                }
            }
            FdmQml_Param{ id:shrink_infill_shrink_distance; objectName: "shrink_infill_shrink_distance"
                fdmLabel: "Infill Shrink Distance"
                fdmDescription: "Set the infill distance near the walls on your 3D print."
                fdmDefaultValue: 0.0
                fdmValue: Number(machine_nozzle_size.fdmValue) / 2
                fdmUnit: "mm"
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_nozzle_size"
            }
            FdmQml_Param{ id:wall_ruler_concave_convex_enable; objectName: "wall_ruler_concave_convex_enable"
                fdmLabel: "Concave / Convex Clarity"
                fdmDescription: "This setting increases the quality of concave and convex curves on your print."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:wall_ruler_concave_percent; objectName: "wall_ruler_concave_percent"
                    fdmLabel: "Concave Settings"
                    fdmDescription: "Input the strength of the concave setting."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmSettablePerMesh: true
                    fdmEnabled: wall_ruler_concave_convex_enable.fdmValue
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_ruler_convex_percent; objectName: "wall_ruler_convex_percent"
                    fdmLabel: "Convex Settings"
                    fdmDescription: "Input the strength of the convex setting."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmSettablePerMesh: true
                    fdmEnabled: wall_ruler_concave_convex_enable.fdmValue
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_ruler_separate_percent; objectName: "wall_ruler_separate_percent"
                    fdmLabel: "Wall Separation Rate"
                    fdmDescription: "The distance between the outer and inner wall."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmSettablePerMesh: true
                    fdmEnabled: wall_ruler_concave_convex_enable.fdmValue
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:small_hole_max_size; objectName: "small_hole_max_size"
                fdmLabel: "Small Hole Max Size"
                fdmDescription: "Holes and part outlines with a diameter smaller than this will be printed using Small Feature Speed."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:small_feature_max_length; objectName: "small_feature_max_length"
                    fdmLabel: "Small Feature Max Length"
                    fdmDescription: "Feature outlines that are shorter than this length will be printed using Small Feature Speed."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmDefaultValue: 0.0
                    fdmValue: Number(small_hole_max_size.fdmValue) *  Math.PI
                    fdmSettablePerMesh: true
                    fdmEnabled: false
                    fdmAffectedById: "small_hole_max_size"
                }
                FdmQml_Param{ id:small_feature_speed_factor; objectName: "small_feature_speed_factor"
                    fdmLabel: "Small Feature Speed"
                    fdmDescription: "Small features will be printed at this percentage of their normal print speed. Slower printing can help with adhesion and accuracy."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 50.0
                    fdmMinimumValue: 1.0
                    fdmMinimumValueWarning: 25.0
                    fdmMaximumValueWarning: 100.0
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:small_feature_speed_factor_0; objectName: "small_feature_speed_factor_0"
                    fdmLabel: "Small Feature Initial Layer Speed"
                    fdmDescription: "Small features on the first layer will be printed at this percentage of their normal print speed. Slower printing can help with adhesion and accuracy."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 50.0
                    fdmValue: Number(small_feature_speed_factor.fdmValue)
                    fdmMinimumValue: 1.0
                    fdmMinimumValueWarning: 25.0
                    fdmMaximumValueWarning: 100.0
                    fdmSettablePerMesh: true
                    fdmEnabled: false
                    fdmAffectedById: "small_feature_speed_factor"
                }
            }
            FdmQml_Param{ id:wall_min_extrusion; objectName: "wall_min_extrusion"
                fdmLabel: "Minimum Wall Extrusion"
                fdmDescription: "Minimum allowed extrusion for a wall line. Walls where the flow is less than this value will be replaced with a travel move."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmDefaultValue: 0.00003
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wall_overhang_angle; objectName: "wall_overhang_angle"
                fdmLabel: "Overhanging Wall Angle"
                fdmDescription: "Walls that overhang more than this angle will be printed using overhanging wall settings. When the value is 90, no walls will be treated as overhanging. Overhangs with supports will not be treated as overhangs."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 2.0
                fdmMaximumValue: 90.0
                fdmDefaultValue: 90.0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:wall_overhang_area_filter; objectName: "wall_overhang_area_filter"
                    fdmLabel: "Drape Wall Area Filtration"
                    fdmDescription: "Adjust the minimum area that will be treated as overhang."
                    fdmUnit: "mm²"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmDefaultValue: 0.6
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_overhang_extend_circle; objectName: "wall_overhang_extend_circle"
                    fdmLabel: "Overhang Limit"
                    fdmDescription: "Set where overhangs apply on your print."
                    fdmType: "bool"
                    fdmDefaultValue: false
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_overhang_extend_type; objectName: "wall_overhang_extend_type"
                    fdmLabel: "Wall Overhang Effect Speed"
                    fdmDescription: "Set how the printer slows down for overhangs."
                    fdmType: "enum"
                    fdmOptions:{
                        "instant": "instant",
                        "gradually_xy": "Gradually before overhang",
                        "circle": "Gradually for entire layer"
                    }
                    fdmDefaultValue: "gradually_xy"
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:overhang_min_line_length; objectName: "overhang_min_line_length"
                        fdmLabel: "Overhang Min Line Length"
                        fdmDescription: "Input the minimum length that a line will be treated as an overhang line."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 5.0
                        fdmEnabled: wall_overhang_extend_type.fdmValue === "gradually_xy"
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:wall_overhang_extend_xy_distance; objectName: "wall_overhang_extend_xy_distance"
                        fdmLabel: "Wall Overhang Speed Buffer (X/Y Axis)"
                        fdmDescription: "Input the distance where the speed will slow down when approaching overhangs on the X or Y axis."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 10.0
                        fdmEnabled: wall_overhang_extend_type.fdmValue === "gradually_xy"
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:wall_overhang_extend_z_distance; objectName: "wall_overhang_extend_z_distance"
                        fdmLabel: "Wall Overhang Speed Buffer (Z Axis)"
                        fdmDescription: "Input the distance where the speed will slow down when approaching overhangs on the Z axis."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 10.0
                        fdmEnabled: false
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:overhang_inner_wall_shrink_distance; objectName: "overhang_inner_wall_shrink_distance"
                        fdmLabel: "Overhang Inner Wall Shrink Distance"
                        fdmDescription: "This setting creates a tiny gap between the outer and inner walls to improve overhang quality."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 0.0
                        fdmValue: 0.05
                        fdmEnabled: wall_overhang_extend_type.fdmValue === "gradually_xy"
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:overhang_related_inner_wall_slow_down; objectName: "overhang_related_inner_wall_slow_down"
                    fdmLabel: "slow down overhang inner wall"
                    fdmDescription: "Print overhang inner wall with overhang speed."
                    fdmType: "bool"
                    fdmDefaultValue: true
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_overhang_speed_factor; objectName: "wall_overhang_speed_factor"
                    fdmLabel: "Overhanging Wall Speed"
                    fdmDescription: "Overhanging walls will be printed at this percentage of their normal print speed."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 5.0
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:wall_overhang_flow_factor; objectName: "wall_overhang_flow_factor"
                    fdmLabel: "Overhang Wall Flow"
                    fdmDescription: "Control the extrusion flow when printing overhangs."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 25.0
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:skin_overhang_enable; objectName: "skin_overhang_enable"
                fdmLabel: "Overhang Skin Overlap"
                fdmDescription: "This setting controls the overlap between the skin wall and its filling."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: true
                fdmAffectedById: ""
                FdmQml_Param{ id:skin_overlap_overhang_speed; objectName: "skin_overlap_overhang_speed"
                    fdmLabel: "Overhang Skin Overlap Speed Percentage"
                    fdmDescription: "Set how much the printer slows down when approaching overhang skin overlaps."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 50.0
                    fdmEnabled: skin_overhang_enable.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:skin_overlap_overhang_flow; objectName: "skin_overlap_overhang_flow"
                    fdmLabel: "Overhang Skin Overlap Flow Percentage"
                    fdmDescription: "Set the intensity of the flow when approaching overhang skin overlaps."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmValue: Number(skin_material_flow.fdmValue)
                    fdmDefaultValue: 100.0
                    fdmEnabled: skin_overhang_enable.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "skin_material_flow"
                }
                FdmQml_Param{ id:skin_overlap_overhang; objectName: "skin_overlap_overhang"
                    fdmLabel: "Overhanging Skin Overlap Percentage"
                    fdmDescription: "Adjust the amount of overlap between the walls and the endpoitns of the skin centerlines.A slight overlap allows the walls to firmly connect to the skin.Note: Percentages over 50% may cause the skin to go past the wall."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmEnabled: skin_overhang_enable.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:skin_overlap_overhang_mm; objectName: "skin_overlap_overhang_mm"
                        fdmLabel: "Overhanging Skin Overlap"
                        fdmDescription: "Adjust the amount of overlap between the walls and (the endpoints of) the skin-centerlines. A slight overlap allows the walls to connect firmly to the skin. Note that, given an equal skin and wall line-width, any value over half the width of the wall may already cause any skin to go past the wall, because at that point the position of the nozzle of the skin-extruder may already reach past the middle of the wall."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 0.02
                        fdmValue: top_bottom_pattern.fdmValue !== "concentric" ? (0.5 * (Number(skin_line_width.fdmValue) + (Number(wall_line_count.fdmValue) > 1 ? Number(wall_line_width_x.fdmValue) : Number(wall_line_width_0.fdmValue))) * Number(skin_overlap_overhang.fdmValue) / 100 ) : 0
                        fdmEnabled: skin_overhang_enable.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: "skin_overlap_overhang,wall_line_width_0,wall_line_width_x,skin_line_width,wall_line_count,top_bottom_pattern"
                    }
                }
            }
            FdmQml_Param{ id:optimize_wall_0_printing_order; objectName: "optimize_wall_0_printing_order"
                fdmLabel: "Outer Wall Print Priority"
                fdmDescription: "Decide when outer walls will be printed."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:optimize_wall_hole_printing; objectName: "optimize_wall_hole_printing"
                fdmLabel: "Holes"
                fdmDescription: "Optimize the printing of hole walls to make them smoother. Enabling this option may result in longer slicing time. It is recommended to enable this option when printing assembled parts.."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:reverse_hole_outer_wall_print_orient; objectName: "reverse_hole_outer_wall_print_orient"
                fdmLabel: "Reverse Printing for Holes"
                fdmDescription: "Print in a clockwise direction when reaching the hole's outer wall. This creates a smoother effect when transitioning away from the hole's outer wall."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:optimize_wall_small_printing; objectName: "optimize_wall_small_printing"
                fdmLabel: "Small Part Printing"
                fdmDescription: "Slow down prints when they reach small, intricate parts of the model."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:optimize_single_part_z_seam; objectName: "optimize_single_part_z_seam"
                fdmLabel: "Z Seam"
                fdmDescription: "Smooth bumps that can occur when your print moves to the next layer."
                fdmType: "int"
                fdmDefaultValue: 0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:show_machine_setting; objectName: "show_machine_setting"
                fdmLabel: "Max Jerk Parameters"
                fdmDescription: "This shows the maxium parameters of the printer's jerk setting."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmAffectedById: ""
                FdmQml_Param{ id:machine_max_jerk_xy; objectName: "machine_max_jerk_xy"
                    fdmLabel: "Default X-Y Jerk"
                    fdmDescription: "Default jerk for movement in the horizontal plane."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmEnabled: show_machine_setting.fdmValue
                    fdmDefaultValue: 30  // 20.0
                    fdmMinimumValue: 0.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:machine_max_jerk_z; objectName: "machine_max_jerk_z"
                    fdmLabel: "Default Z Jerk"
                    fdmDescription: "Default jerk for the motor of the Z-direction."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmEnabled: show_machine_setting.fdmValue
                    fdmDefaultValue: 1 // 0.4
                    fdmMinimumValue: 0.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:machine_max_jerk_e; objectName: "machine_max_jerk_e"
                    fdmLabel: "Default Filament Jerk"
                    fdmDescription: "Default jerk for the motor of the filament."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmEnabled: show_machine_setting.fdmValue
                    fdmDefaultValue: 10 // 5.0
                    fdmMinimumValue: 0.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:bridge_settings_enabled; objectName: "bridge_settings_enabled"
                fdmLabel: "Enable Bridge Settings"
                fdmDescription: "Detect bridges and modify print speed, flow, and fan settings while bridges are printed."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmResolve: bridge_settings_enabled.fdmValue
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
                FdmQml_Param{ id:bridge_split_min_length; objectName: "bridge_split_min_length"
                    fdmLabel: "Bridge split min length"
                    fdmDescription: "Split a long bridge to 3 part to perform a variable speed printing"
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmDefaultValue: 20.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_shrink_length; objectName: "bridge_shrink_length"
                    fdmLabel: "Bridge shrink length"
                    fdmDescription: "Offset from Bridge start to apply the bridge config "
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmDefaultValue: 5.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_wall_min_length; objectName: "bridge_wall_min_length"
                    fdmLabel: "Minimum Bridge Wall Length"
                    fdmDescription: "Unsupported walls shorter than this will be printed using the normal wall settings. Longer unsupported walls will be printed using the bridge wall settings."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmDefaultValue: 5.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_skin_support_threshold; objectName: "bridge_skin_support_threshold"
                    fdmLabel: "Bridge Skin Support Threshold"
                    fdmDescription: "If a skin region is supported for less than this percentage of its area, print it using the bridge settings. Otherwise it will be printed using the normal skin settings."
                    fdmUnit: "%"
                    fdmDefaultValue: 50.0
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_sparse_infill_max_density; objectName: "bridge_sparse_infill_max_density"
                    fdmLabel: "Bridge Sparse Infill Max Density"
                    fdmDescription: "Maximum density of infill considered to be sparse. Skin over sparse infill is considered to be unsupported and may be treated as a bridge skin."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 0.0
                    fdmMinimumValue: 0.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_wall_coast; objectName: "bridge_wall_coast"
                    fdmLabel: "Bridge Wall Coasting"
                    fdmDescription: "This controls the distance the extruder coasts before a bridge wall begins. Coasting before the bridge starts can reduce pressure in the nozzle and produce a flatter bridge."
                    fdmUnit: "%"
                    fdmDefaultValue: 100.0
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 500.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_wall_speed; objectName: "bridge_wall_speed"
                    fdmLabel: "Bridge Wall Speed"
                    fdmDescription: "The speed at which the bridge walls are printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: 15.0 //Number(cool_min_speed.fdmValue)
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 300.0
                    fdmDefaultValue: 15.0
                    fdmValue: 25.0 //Math.max(Number(cool_min_speed.fdmValue), Number(speed_wall_0.fdmValue) / 2)
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y"
                }
                FdmQml_Param{ id:bridge_wall_material_flow; objectName: "bridge_wall_material_flow"
                    fdmLabel: "Bridge Wall Flow"
                    fdmDescription: "When printing bridge walls, the amount of material extruded is multiplied by this value."
                    fdmUnit: "%"
                    fdmDefaultValue: 50.0
                    fdmType: "float"
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_skin_speed; objectName: "bridge_skin_speed"
                    fdmLabel: "Bridge Skin Speed"
                    fdmDescription: "The speed at which bridge skin regions are printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: 15.0 //Number(cool_min_speed.fdmValue)
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 300.0
                    fdmDefaultValue: 15.0
                    fdmValue: 25.0 //Math.max(Number(cool_min_speed.fdmValue), Number(speed_topbottom.fdmValue) / 2)
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y"
                }
                FdmQml_Param{ id:bridge_skin_material_flow; objectName: "bridge_skin_material_flow"
                    fdmLabel: "Bridge Skin Flow"
                    fdmDescription: "When printing bridge skin regions, the amount of material extruded is multiplied by this value."
                    fdmUnit: "%"
                    fdmDefaultValue: 60.0
                    fdmType: "float"
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_skin_density; objectName: "bridge_skin_density"
                    fdmLabel: "Bridge Skin Density"
                    fdmDescription: "The density of the bridge skin layer. Values less than 100 will increase the gaps between the skin lines."
                    fdmUnit: "%"
                    fdmDefaultValue: 100.0
                    fdmType: "float"
                    fdmMinimumValue: 5.0
                    fdmMaximumValue: 150.0
                    fdmMinimumValueWarning: 20.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_fan_speed; objectName: "bridge_fan_speed"
                    fdmLabel: "Bridge Fan Speed"
                    fdmDescription: "Adjusts the fan speed when printing bridge walls and skin."
                    fdmUnit: "%"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 100.0
                    fdmType: "float"
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:bridge_enable_more_layers; objectName: "bridge_enable_more_layers"
                    fdmLabel: "Bridge Has Multiple Layers"
                    fdmDescription: "If enabled, the second and third layers above the air are printed using the following settings. Otherwise, those layers are printed using the normal settings."
                    fdmType: "bool"
                    fdmDefaultValue: true
                    fdmEnabled: bridge_settings_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:bridge_skin_speed_2; objectName: "bridge_skin_speed_2"
                        fdmLabel: "Bridge Second Skin Speed"
                        fdmDescription: "Print speed when printing the second bridge skin layer."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: 15 //Number(cool_min_speed.fdmValue)
                        fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                        fdmMaximumValueWarning: 300.0
                        fdmDefaultValue: 25.0
                        fdmValue: Number(bridge_skin_speed.fdmValue)
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: "bridge_skin_speed,machine_max_feedrate_x,machine_max_feedrate_y"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_2; objectName: "bridge_skin_material_flow_2"
                        fdmLabel: "Bridge Second Skin Flow"
                        fdmDescription: "When printing the second bridge skin layer, the amount of material extruded is multiplied by this value."
                        fdmUnit: "%"
                        fdmDefaultValue: 100.0
                        fdmType: "float"
                        fdmMinimumValue: 5.0
                        fdmMaximumValue: 500.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:bridge_skin_density_2; objectName: "bridge_skin_density_2"
                        fdmLabel: "Bridge Second Skin Density"
                        fdmDescription: "The density of the second bridge skin layer. Values less than 100 will increase the gaps between the skin lines."
                        fdmUnit: "%"
                        fdmDefaultValue: 75.0
                        fdmType: "float"
                        fdmMinimumValue: 5.0
                        fdmMaximumValue: 150.0
                        fdmMinimumValueWarning: 20.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:bridge_fan_speed_2; objectName: "bridge_fan_speed_2"
                        fdmLabel: "Bridge Second Skin Fan Speed"
                        fdmDescription: "Percentage fan speed when printing the second bridge skin layer."
                        fdmUnit: "%"
                        fdmMinimumValue: 0.0
                        fdmMaximumValue: 100.0
                        fdmDefaultValue: 0.0
                        fdmType: "float"
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:bridge_skin_speed_3; objectName: "bridge_skin_speed_3"
                        fdmLabel: "Bridge Third Skin Speed"
                        fdmDescription: "Print speed when printing the third bridge skin layer."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: 15.0 //Number(cool_min_speed.fdmValue)
                        fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                        fdmMaximumValueWarning: 300.0
                        fdmDefaultValue: 15.0
                        fdmValue: Number(bridge_skin_speed.fdmValue)
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: "bridge_skin_speed,machine_max_feedrate_x,machine_max_feedrate_y"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_3; objectName: "bridge_skin_material_flow_3"
                        fdmLabel: "Bridge Third Skin Flow"
                        fdmDescription: "When printing the third bridge skin layer, the amount of material extruded is multiplied by this value."
                        fdmUnit: "%"
                        fdmDefaultValue: 110.0
                        fdmType: "float"
                        fdmMinimumValue: 5.0
                        fdmMaximumValue: 500.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:bridge_skin_density_3; objectName: "bridge_skin_density_3"
                        fdmLabel: "Bridge Third Skin Density"
                        fdmDescription: "The density of the third bridge skin layer. Values less than 100 will increase the gaps between the skin lines."
                        fdmUnit: "%"
                        fdmDefaultValue: 80.0
                        fdmType: "float"
                        fdmMinimumValue: 5.0
                        fdmMaximumValue: 150.0
                        fdmMinimumValueWarning: 20.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                    FdmQml_Param{ id:bridge_fan_speed_3; objectName: "bridge_fan_speed_3"
                        fdmLabel: "Bridge Third Skin Fan Speed"
                        fdmDescription: "Fan speed percentage when printing the third bridge skin layer."
                        fdmUnit: "%"
                        fdmMinimumValue: 0.0
                        fdmMaximumValue: 100.0
                        fdmDefaultValue: 0.0
                        fdmType: "float"
                        fdmEnabled: bridge_settings_enabled.fdmValue && bridge_enable_more_layers.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                    }
                }
            }
            FdmQml_Param{ id:machine_index_name; objectName: "machine_index_name"
                fdmLabel: "Machine Index Name"
                fdmDescription: "The name of machine index."
                fdmType: "str"
                fdmDefaultValue: "unknown"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extruder_index_name; objectName: "extruder_index_name"
                fdmLabel: "Extruder Index Name"
                fdmDescription: "The name of extruder index."
                fdmType: "str"
                fdmDefaultValue: "unknown"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_index_name; objectName: "material_index_name"
                fdmLabel: "Material Index Name"
                fdmDescription: "The name of material index."
                fdmType: "str"
                fdmDefaultValue: "unknown"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:parameter_index_name; objectName: "parameter_index_name"
                fdmLabel: "Parameter Index Name"
                fdmDescription: "The name of parameter index."
                fdmType: "str"
                fdmDefaultValue: "unknown"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extract_param_from_gcode; objectName: "extract_param_from_gcode"
                fdmLabel: "G-Code Parameter Preview"
                fdmDescription: "See the G-Code parameters when previewing G-Code files."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:check_param_lost_interval; objectName: "check_param_lost_interval"
                fdmLabel: "check param lost interval"
                fdmDescription: "check whether the parameter lost every interval seconds."
                fdmType: "int"
                fdmDefaultValue: 60
                fdmEnabled: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:coasting_enable; objectName: "coasting_enable"
                fdmLabel: "Enable Coasting"
                fdmDescription: "Coasting replaces the last part of an extrusion path with a travel path. The oozed material is used to print the last piece of the extrusion path in order to reduce stringing."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
                FdmQml_Param{ id:coasting_volume; objectName: "coasting_volume"
                    fdmLabel: "Coasting Volume"
                    fdmDescription: "The volume otherwise oozed. This value should generally be close to the nozzle diameter cubed."
                    fdmUnit: "mm³"
                    fdmType: "float"
                    fdmDefaultValue: 0.064
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue) * 5
                    fdmEnabled: coasting_enable.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_nozzle_size"
                }
                FdmQml_Param{ id:coasting_min_volume; objectName: "coasting_min_volume"
                    fdmLabel: "Minimum Volume Before Coasting"
                    fdmDescription: "The smallest volume an extrusion path should have before allowing coasting. For smaller extrusion paths, less pressure has been built up in the bowden tube and so the coasted volume is scaled linearly. This value should always be larger than the Coasting Volume."
                    fdmUnit: "mm³"
                    fdmType: "float"
                    fdmDefaultValue: 0.8
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 10.0
                    fdmEnabled: coasting_enable.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:coasting_speed; objectName: "coasting_speed"
                    fdmLabel: "Coasting Speed"
                    fdmDescription: "The speed by which to move during coasting, relative to the speed of the extrusion path. A value slightly under 100% is advised, since during the coasting move the pressure in the bowden tube drops."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 90.0
                    fdmMinimumValue: 0.0001
                    fdmMaximumValueWarning: 100.0
                    fdmEnabled: coasting_enable.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
            }
        }
        FdmQml_Category{ id:machine_settings; objectName: "machine_settings"
            fdmLabel: "Machine"
            fdmType: "category"
            fdmDescription: "Machine specific settings."
            fdmIcon: "MachinePrinter"
            FdmQml_Param{ id:machine_name; objectName: "machine_name"
                fdmLabel: "Machine Type"
                fdmDescription: "The name of your 3D printer model."
                fdmDefaultValue: "Unknown"
                fdmType: "str"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_show_variants; objectName: "machine_show_variants"
                fdmLabel: "Show Machine Variants"
                fdmDescription: "This shows the different variants of this machine, which are described in separate JSON files."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_start_gcode; objectName: "machine_start_gcode"
                fdmLabel: "Start G-Code"
                fdmDescription: "Add G-Code commands to the beginning of the file. Separate commands by \\n."
                fdmDefaultValue: "; config start gcode "
                fdmType: "str"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_end_gcode; objectName: "machine_end_gcode"
                fdmLabel: "End G-Code"
                fdmDescription: "Add G-code commands to the end of the file. Separate commands by \\n."
                fdmDefaultValue: "; config end gcode\nM104 S0\nM140 S0\n;Retract the filament\nG92 E1\nG1 E-1 F300\nG28 X0 Y0\nM84"
                fdmType: "str"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_guid; objectName: "material_guid"
                fdmLabel: "Material GUID"
                fdmDescription: "GUID of the material. This is set automatically."
                fdmDefaultValue: ""
                fdmType: "str"
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_bed_temp_wait; objectName: "material_bed_temp_wait"
                fdmLabel: "Wait for Build Plate Heatup"
                fdmDescription: "Inserts a command to wait until the build plate temperature is reached before starting."
                fdmDefaultValue: true
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_print_temp_wait; objectName: "material_print_temp_wait"
                fdmLabel: "Wait for Nozzle Heatup"
                fdmDescription: "Waits until the nozzle temperature is reached before starting."
                fdmDefaultValue: true
                fdmType: "bool"
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_print_temp_prepend; objectName: "material_print_temp_prepend"
                fdmLabel: "Include Material Temperatures"
                fdmDescription: "Place nozzle temperature commands at the start of the G-Code. When the Start G-Code already contains nozzle temperature commands, the AnkerMake frontend will automatically disable this setting."
                fdmDefaultValue: true
                fdmType: "bool"
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_bed_temp_prepend; objectName: "material_bed_temp_prepend"
                fdmLabel: "Include Build Plate Temperature"
                fdmDescription: "Place build plate temperature commands at the start of the G-Code. When the Start G-Code already contains build plate temeprature commands, the AnkerMake Slicer frontend will automatically disable this setting."
                fdmDefaultValue: true
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_width; objectName: "machine_width"
                fdmLabel: "Machine Width"
                fdmDescription: "The width (X-direction) of the printable area."
                fdmDefaultValue: 235.0  // 5x
                fdmType: "float"
                fdmMinimumValue: 0.001
                fdmMaximumValue: 2000000.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_depth; objectName: "machine_depth"
                fdmLabel: "Machine Depth"
                fdmDescription: "The depth (Y-direction) of the printable area."
                fdmDefaultValue: 235.0  // 5x
                fdmType: "float"
                fdmMinimumValue: 0.001
                fdmMaximumValue: 2000000.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_shape; objectName: "machine_shape"
                fdmLabel: "Build Plate Shape"
                fdmDescription: "The shape of the build plate without taking unprintable areas into account."
                fdmDefaultValue: "rectangular"
                fdmType: "enum"
                fdmOptions:{
                    "rectangular": "Rectangular"
                }
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_buildplate_type; objectName: "machine_buildplate_type"
                fdmLabel: "Build Plate Material"
                fdmDescription: "The material of the build plate installed on the printer."
                fdmDefaultValue: "glass"
                fdmType: "enum"
                fdmOptions:{
                    "glass": "Glass",
                    "aluminum": "Aluminum"
                }
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_height; objectName: "machine_height"
                fdmLabel: "Machine Height"
                fdmDescription: "The height (Z-direction) of the printable area."
                fdmDefaultValue: 250.0  // 5x
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_heated_bed; objectName: "machine_heated_bed"
                fdmLabel: "Has Heated Build Plate"
                fdmDescription: "Toggle if you have a heated build plate."
                fdmDefaultValue: true // 5x
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_heated_build_volume; objectName: "machine_heated_build_volume"
                fdmLabel: "Has Build Volume Temperature Stabilization"
                fdmDescription: "Toggle if your printer can stabilize the build volume temperature."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
                FdmQml_Param{ id:build_volume_temperature; objectName: "build_volume_temperature"
                    fdmLabel: "Build Volume Temperature"
                    fdmDescription: "The temperature of the ambient environment. If this is 0, the build volume temperature will not be adjusted."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 0.0
                    fdmResolve: Number(build_volume_temperature.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 0.0
                    fdmMaximumValueWarning: 285.0
                    fdmEnabled: machine_heated_build_volume.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:machine_always_write_active_tool; objectName: "machine_always_write_active_tool"
                fdmLabel: "Always Write Active Tool"
                fdmDescription: "Write active tool after sending temp commands to inactive tool. Required for Dual Extruder printing with Smoothie or other firmware with modal tool commands."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_center_is_zero; objectName: "machine_center_is_zero"
                fdmLabel: "Is Center Origin"
                fdmDescription: "Toggle if the X/Y coordinates of the zero position of the printer is at the center of the printable area."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_count; objectName: "machine_extruder_count"
                fdmLabel: "Number of Extruders"
                fdmDescription: "Number of extruder trains. An extruder train is the combination of a feeder, bowden tube, and nozzle."
                fdmDefaultValue: 1
                fdmMinimumValue: 1
                fdmMaximumValue: 1
                fdmType: "int"
                fdmOptions: machine_extruder_count.fdmMaximumValue <= 1 ? {1:1} : machine_extruder_count.fdmMaximumValue === 2 ? {1:1, 2:2} : {1:1, 2:2, 3:3}
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extruders_enabled_count; objectName: "extruders_enabled_count"
                fdmLabel: "Number of Extruders That Are Enabled"
                fdmDescription: "Number of extruder trains that are enabled; automatically set in software."
                fdmValue: Number(machine_extruder_count.fdmValue)
                fdmDefaultValue: 1
                fdmMinimumValue: 1
                fdmMaximumValue: 16
                fdmType: "int"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_extruder_count"
            }
            FdmQml_Param{ id:machine_nozzle_tip_outer_diameter; objectName: "machine_nozzle_tip_outer_diameter"
                fdmLabel: "Outer Nozzle Diameter"
                fdmDescription: "The outer diameter of the nozzle tip."
                fdmUnit: "mm"
                fdmDefaultValue: 1.0
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_head_distance; objectName: "machine_nozzle_head_distance"
                fdmLabel: "Nozzle Length"
                fdmDescription: "The height difference between the tip of the nozzle and the lowest part of the print head."
                fdmUnit: "mm"
                fdmDefaultValue: 3.0
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_expansion_angle; objectName: "machine_nozzle_expansion_angle"
                fdmLabel: "Nozzle Angle"
                fdmDescription: "The angle between the horizontal plane and the conical part, right above the tip of the nozzle."
                fdmUnit: "°"
                fdmType: "int"
                fdmDefaultValue: 45
                fdmMaximumValue: 89
                fdmMinimumValue: 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_heat_zone_length; objectName: "machine_heat_zone_length"
                fdmLabel: "Heat Zone Length"
                fdmDescription: "The distance from the tip of the nozzle where nozzle heat is transferred to the filament."
                fdmUnit: "mm"
                fdmDefaultValue: 16.0
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_temp_enabled; objectName: "machine_nozzle_temp_enabled"
                fdmLabel: "Enable Nozzle Temperature Control"
                fdmDescription: "Control the temperature from the AnkerMake Slicer. Turn this off to control the nozzle temperature from outside of AnkerMake Slicer."
                fdmDefaultValue: true
                fdmValue: true
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_heat_up_speed; objectName: "machine_nozzle_heat_up_speed"
                fdmLabel: "Heat Up Speed"
                fdmDescription: "The speed (°C/s) by which the nozzle heats up averaged over the window of normal printing temperatures and the standby temperature."
                fdmDefaultValue: 2.0
                fdmUnit: "°C/s"
                fdmType: "float"
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_cool_down_speed; objectName: "machine_nozzle_cool_down_speed"
                fdmLabel: "Cool Down Speed"
                fdmDescription: "The speed (°C/s) by which the nozzle cools down averaged over the window of normal printing temperatures and the standby temperature."
                fdmDefaultValue: 2.0
                fdmUnit: "°C/s"
                fdmType: "float"
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_min_cool_heat_time_window; objectName: "machine_min_cool_heat_time_window"
                fdmLabel: "Minimal Time Standby Temperature"
                fdmDescription: "The minimal time an extruder has to be inactive before the nozzle is cooled to the standby temperature."
                fdmDefaultValue: 50.0
                fdmUnit: "s"
                fdmType: "float"
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_gcode_flavor; objectName: "machine_gcode_flavor"
                fdmLabel: "G-Code Flavor"
                fdmDescription: "The type of G-Code generated."
                fdmType: "enum"
                fdmOptions:{
                    "RepRap (Marlin/Sprinter)": "Marlin",
                    "RepRap (RepRap)": "RepRap",
                    "Griffin": "Griffin",
                    "Makerbot": "Makerbot"
                }
                fdmDefaultValue: "RepRap (Marlin/Sprinter)"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_firmware_retract; objectName: "machine_firmware_retract"
                fdmLabel: "Firmware Retraction"
                fdmDescription: "Use the firmware retract commands (G10/G11) instead of the E property in G1 commands to retract material."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: machine_gcode_flavor.fdmValue === "RepRap (Volumetric)" || machine_gcode_flavor.fdmValue === "UltiGCode" || machine_gcode_flavor.fdmValue === "BFB"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_gcode_flavor"
            }
            FdmQml_Param{ id:machine_extruders_share_heater; objectName: "machine_extruders_share_heater"
                fdmLabel: "Extruders Share Heater"
                fdmDescription: "Toggle if your extruders share a single heater."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruders_share_nozzle; objectName: "machine_extruders_share_nozzle"
                fdmLabel: "Extruders Share Nozzle"
                fdmDescription: "Whether the extruders share a single nozzle rather than each extruder having its own nozzle. When set to true, it is expected that the printer-start gcode script properly sets up all extruders in an initial retraction state that is known and mutually compatible (either zero or one filament not retracted); in that case the initial retraction status is described, per extruder, by the 'machine_extruders_shared_nozzle_initial_retraction' parameter."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruders_shared_nozzle_initial_retraction; objectName: "machine_extruders_shared_nozzle_initial_retraction"
                fdmLabel: "Shared Nozzle Initial Retraction"
                fdmDescription: "Set how much each extruder will retract from the shared nozzle tip after the G-Code start script finishes. The value should be equal to or greater than the length of the shared nozzle duct area."
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmType: "float"
                fdmEnabled: machine_extruders_share_nozzle.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_disallowed_areas; objectName: "machine_disallowed_areas"
                fdmLabel: "Disallowed Areas"
                fdmDescription: "Set which polygons the print head will not travel to."
                fdmType: "polygons"
                fdmDefaultValue: "[]"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_head_with_fans_polygon; objectName: "machine_head_with_fans_polygon"
                fdmLabel: "Machine Head & Fan Polygon"
                fdmDescription: "A 2D silhouette of the print head (fan caps included)."
                fdmType: "polygon"
                fdmDefaultValue: "[ [ -20, 10 ], [ 10, 10 ], [ 10, -10 ], [ -20, -10 ] ]"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:gantry_height; objectName: "gantry_height"
                fdmLabel: "Gantry Height"
                fdmDescription: "The height difference between the tip of the nozzle and the gantry system (X and Y axes)."
                fdmDefaultValue: 999999.0
                fdmValue: Number(machine_height.fdmValue)
                fdmType: "float"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_height"
            }
            FdmQml_Param{ id:machine_use_extruder_offset_to_offset_coords; objectName: "machine_use_extruder_offset_to_offset_coords"
                fdmLabel: "Offset with Extruder"
                fdmDescription: "Apply the extruder offset to the coordinate system for all extruders."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extruder_prime_pos_abs; objectName: "extruder_prime_pos_abs"
                fdmLabel: "Absolute Extruder Prime Position"
                fdmDescription: "Make the extruder prime position absolute rather than relative to the last-known location of the head."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_feedrate_x; objectName: "machine_max_feedrate_x"
                fdmLabel: "Maximum Speed X"
                fdmDescription: "The maximum speed for the motor in the X direction."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_feedrate_y; objectName: "machine_max_feedrate_y"
                fdmLabel: "Maximum Speed Y"
                fdmDescription: "The maximum speed for the motor in the Y direction."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_feedrate_z; objectName: "machine_max_feedrate_z"
                fdmLabel: "Maximum Speed Z"
                fdmDescription: "The maximum speed for the motor in the Z direction."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_feedrate_e; objectName: "machine_max_feedrate_e"
                fdmLabel: "Maximum Feedrate"
                fdmDescription: "The maximum speed of the filament."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_acceleration_x; objectName: "machine_max_acceleration_x"
                fdmLabel: "Maximum Acceleration X"
                fdmDescription: "Maximum acceleration for the motor in the X direction"
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_acceleration_y; objectName: "machine_max_acceleration_y"
                fdmLabel: "Maximum Acceleration Y"
                fdmDescription: "Maximum acceleration for the motor in the Y direction."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_acceleration_z; objectName: "machine_max_acceleration_z"
                fdmLabel: "Maximum Acceleration Z"
                fdmDescription: "Maximum acceleration for the motor in the Z direction."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_max_acceleration_e; objectName: "machine_max_acceleration_e"
                fdmLabel: "Maximum Filament Acceleration"
                fdmDescription: "Maximum acceleration for the filament motor."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 999999.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_acceleration; objectName: "machine_acceleration"
                fdmLabel: "Default Acceleration"
                fdmDescription: "The default acceleration of the print head movement."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 999999.0   // 5x
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_steps_per_mm_x; objectName: "machine_steps_per_mm_x"
                fdmLabel: "Steps per Millimeter (X)"
                fdmDescription: "How many stepper motor steps will result in 1 mm of movement in the X direction."
                fdmType: "float"
                fdmDefaultValue: 80.0
                fdmMinimumValue: 0.0000001
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_steps_per_mm_y; objectName: "machine_steps_per_mm_y"
                fdmLabel: "Steps per Millimeter (Y)"
                fdmDescription: "How many stepper motor steps will result in 1 mm of movement in the Y direction."
                fdmType: "float"
                fdmDefaultValue: 80.0
                fdmMinimumValue: 0.0000001
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_steps_per_mm_z; objectName: "machine_steps_per_mm_z"
                fdmLabel: "Steps per Millimeter (Z)"
                fdmDescription: "How many stepper motor steps will result in 1 mm of movement in the Z direction."
                fdmType: "float"
                fdmDefaultValue: 400.0
                fdmMinimumValue: 0.0000001
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_steps_per_mm_e; objectName: "machine_steps_per_mm_e"
                fdmLabel: "Steps per Millimeter (E)"
                fdmDescription: "How many stepper motor steps will result in moving the feeder wheel by 1 mm around its circumference."
                fdmType: "float"
                fdmDefaultValue: 1200 //  1600 1312
                fdmMinimumValue: 0.0000001
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_x; objectName: "machine_endstop_positive_direction_x"
                fdmLabel: "X Endstop in Positive Direction"
                fdmDescription: "Toggle if you want the X-axis endstop to end in a positive direction (high X coordinate)."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_y; objectName: "machine_endstop_positive_direction_y"
                fdmLabel: "Y Endstop in Positive Direction"
                fdmDescription: "Toggle if you want the Y-axis endstop to end in a positive direction (high Y coordinate)."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_z; objectName: "machine_endstop_positive_direction_z"
                fdmLabel: "Z Endstop in Positive Direction"
                fdmDescription: "Toggle if you want Z-axis endstop to end in a positive direction (high Z coordinate)."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_minimum_feedrate; objectName: "machine_minimum_feedrate"
                fdmLabel: "Minimum Feedrate"
                fdmDescription: "The minimum movement speed of the print head."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_feeder_wheel_diameter; objectName: "machine_feeder_wheel_diameter"
                fdmLabel: "Feeder Wheel Diameter"
                fdmDescription: "The diameter of the feeder wheel."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 10.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:extruder_settings; objectName: "extruder_settings"
            fdmLabel: "Extruder"
            fdmType: "category"
            fdmDescription: "Extruder Settings"
            FdmQml_Param{ id:extruder_nr; objectName: "extruder_nr"
                fdmLabel: "Extruder"
                fdmDescription: "The extruder train used for printing. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: 0
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_id; objectName: "machine_nozzle_id"
                fdmLabel: "Nozzle ID"
                fdmDescription: "The nozzle ID for an extruder train, such as \"AA 0.4\" and \"BB 0.8\"."
                fdmType: "str"
                fdmDefaultValue: "unknown"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_size; objectName: "machine_nozzle_size"
                fdmLabel: "Nozzle Diameter"
                fdmDescription: "The inner diameter of the nozzle. Change this setting when using a non-standard nozzle size."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.4
                fdmMinimumValue: 0.001
                fdmMaximumValueWarning: 10.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_offset_x; objectName: "machine_nozzle_offset_x"
                fdmLabel: "Nozzle X Offset"
                fdmDescription: "The X coordinate of the nozzle offset."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_nozzle_offset_y; objectName: "machine_nozzle_offset_y"
                fdmLabel: "Nozzle Y Offset"
                fdmDescription: "The Y coordinate of the nozzle offset."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_start_code; objectName: "machine_extruder_start_code"
                fdmLabel: "Extruder Start G-Code"
                fdmDescription: "Execute Start G-Code when switching to this extruder."
                fdmType: "str"
                fdmDefaultValue: ""
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_start_pos_abs; objectName: "machine_extruder_start_pos_abs"
                fdmLabel: "Extruder Start Position Absolute"
                fdmDescription: "Make the extruder starting position absolute rather than relative to the last-known location of the head."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_start_pos_x; objectName: "machine_extruder_start_pos_x"
                fdmLabel: "Extruder Start Position X"
                fdmDescription: "The X-coordinate starting position when turning on the extruder."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_start_pos_y; objectName: "machine_extruder_start_pos_y"
                fdmLabel: "Extruder Start Position Y"
                fdmDescription: "The Y-coordinate starting position when turning on the extruder."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_end_code; objectName: "machine_extruder_end_code"
                fdmLabel: "Extruder End G-Code"
                fdmDescription: "Execute End G-Code when switching from this extruder."
                fdmType: "str"
                fdmDefaultValue: ""
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_end_pos_abs; objectName: "machine_extruder_end_pos_abs"
                fdmLabel: "Extruder End Position Absolute"
                fdmDescription: "Make the extruder ending position absolute rather than relative to the last-known location of the head."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_end_pos_x; objectName: "machine_extruder_end_pos_x"
                fdmLabel: "Extruder End Position X"
                fdmDescription: "The X-coordinate ending position when turning off the extruder."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:machine_extruder_end_pos_y; objectName: "machine_extruder_end_pos_y"
                fdmLabel: "Extruder End Position Y"
                fdmDescription: "The Y-coordinate ending position when turning off the extruder."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extruder_prime_pos_z; objectName: "extruder_prime_pos_z"
                fdmLabel: "Extruder Prime Z Position"
                fdmDescription: "Set the Z-coordinate position where the nozzle primes as printing begins."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValue: Number(machine_height.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_height"
            }
            FdmQml_Param{ id:machine_extruder_cooling_fan_number; objectName: "machine_extruder_cooling_fan_number"
                fdmLabel: "Extruder Print Cooling Fan"
                fdmDescription: "The number of the print cooling fan associated with this extruder. Only change this from the default value of 0 when you have a different print cooling fan for each extruder."
                fdmType: "int"
                fdmDefaultValue: 0
                fdmMinimumValue: 0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:nozzle_disallowed_areas; objectName: "nozzle_disallowed_areas"
                fdmLabel: "Nozzle Disallowed Areas"
                fdmDescription: "Polygon areas where the nozzle cannot move to."
                fdmType: "polygons"
                fdmDefaultValue: "[]"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:resolution; objectName: "resolution"
            fdmLabel: "Quality"
            fdmType: "category"
            fdmIcon: "PrintQuality"
            fdmDescription: "All settings that influence the resolution of the print. These settings greatly impact quality and print time."
            FdmQml_Param{ id:layer_height; objectName: "layer_height"
                fdmLabel: "Layer Height"
                fdmDescription: "The height of each layer in mm. Higher values produce faster prints in lower resolution, lower values produce slower prints in higher resolution."
                fdmUnit: "mm"
                fdmType: "float"
                fdmValue: machine_nozzle_size.fdmValue / 2
                fdmDefaultValue: 0.2
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.04
                fdmMaximumValueWarning: 0.8 * Number(machine_nozzle_size.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_nozzle_size"
            }
            FdmQml_Param{ id:layer_height_0; objectName: "layer_height_0"
                fdmLabel: "Initial Layer Height"
                fdmDescription: "The height of the initial layer in mm. A thicker initial layer makes adhesion to the build plate easier."
                fdmUnit: "mm"
                fdmType: "float"
                fdmValue: layer_height.fdmValue * 0.7 > 0.1 ? layer_height.fdmValue * 0.7 : 0.1
                fdmDefaultValue: 0.3
                fdmResolve: Number(layer_height_0.fdmValue)
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.1
                fdmMaximumValueWarning: 0.8 * Number(machine_nozzle_size.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_nozzle_size"
            }
            FdmQml_Param{ id:line_width; objectName: "line_width"
                fdmLabel: "Line Width"
                fdmDescription: "Width of a single line. Generally, the width of each line should correspond to the width of the nozzle. However, slightly reducing this value could produce better prints."
                fdmUnit: "mm"
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                fdmDefaultValue: 0.4
                fdmType: "float"
                fdmValue: Number(machine_nozzle_size.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size"
                FdmQml_Param{ id:wall_line_width; objectName: "wall_line_width"
                    fdmLabel: "Wall Line Width"
                    fdmDescription: "Width of a single wall line."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                    fdmValue: Number(line_width.fdmValue)
                    fdmDefaultValue: 0.4
                    fdmType: "float"
                    fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue) === Number(wall_0_extruder_nr.fdmValue) ? Number(wall_0_extruder_nr.fdmValue) : -1
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,line_width,wall_0_extruder_nr,wall_x_extruder_nr"
                    FdmQml_Param{ id:wall_line_width_0; objectName: "wall_line_width_0"
                        fdmLabel: "Outer Wall Line Width"
                        fdmDescription: "Width of the outermost wall line. By lowering this value, higher levels of detail can be printed."
                        fdmUnit: "mm"
                        fdmMinimumValue: 0.001
                        fdmMinimumValueWarning: outer_inset_first.fdmValue ? (0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)) : (0.1 * Number(machine_nozzle_size.fdmValue))
                        fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                        fdmDefaultValue: 0.4
                        fdmValue: Number(wall_line_width.fdmValue) * 1.1
                        fdmType: "float"
                        fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "machine_nozzle_size,wall_0_extruder_nr,outer_inset_first"
                    }
                    FdmQml_Param{ id:thin_wall_line_width_0; objectName: "thin_wall_line_width_0"
                        fdmLabel: "Thin Outer Wall Line Width"
                        fdmDescription: "Thin wall line. Some wall that too thin to generate one wall, can use this param to print a thin wall."
                        fdmUnit: "mm"
                        fdmMinimumValue: 0.001
                        fdmMinimumValueWarning: 0.001
                        fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                        fdmDefaultValue: 0.1
                        fdmValue: Number(wall_line_width.fdmValue) * 0.25
                        fdmType: "float"
                        fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "machine_nozzle_size,wall_line_width,wall_0_extruder_nr"
                    }
                    FdmQml_Param{ id:wall_line_width_x; objectName: "wall_line_width_x"
                        fdmLabel: "Inner Wall(s) Line Width"
                        fdmDescription: "Width of a single wall line for all wall lines except the outermost one."
                        fdmUnit: "mm"
                        fdmMinimumValue: 0.001
                        fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                        fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                        fdmDefaultValue: 0.4
                        fdmValue: Number(wall_line_width.fdmValue)
                        fdmType: "float"
                        fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "machine_nozzle_size,wall_line_width,wall_x_extruder_nr"
                    }
                }
                FdmQml_Param{ id:skin_line_width; objectName: "skin_line_width"
                    fdmLabel: "Top / Bottom Line Width"
                    fdmDescription: "Width of a single top / bottom line."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.4
                    fdmType: "float"
                    fdmValue: Number(line_width.fdmValue)
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,line_width,top_bottom_extruder_nr"
                }
                FdmQml_Param{ id:infill_line_width; objectName: "infill_line_width"
                    fdmLabel: "Infill Line Width"
                    fdmDescription: "Width of a single infill line."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 3 * Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.4
                    fdmType: "float"
                    fdmValue: Number(line_width.fdmValue)
                    fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                    fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,line_width,infill_extruder_nr"
                }
                FdmQml_Param{ id:skirt_brim_line_width; objectName: "skirt_brim_line_width"
                    fdmLabel: "Skirt / Brim Line Width"
                    fdmDescription: "Width of a single skirt or brim line."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 3 * Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.4
                    fdmType: "float"
                    fdmValue: Number(line_width.fdmValue)
                    fdmEnabled: adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || prime_tower_brim_enable.fdmValue || draft_shield_enabled.fdmValue || ooze_shield_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_nozzle_size,line_width"
                }
                FdmQml_Param{ id:support_line_width; objectName: "support_line_width"
                    fdmLabel: "Support Line Width"
                    fdmDescription: "Width of a single support structure line."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 3 * Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.4
                    fdmType: "float"
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmValue: Number(line_width.fdmValue)
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_nozzle_size,line_width,support_infill_extruder_nr"
                }
                FdmQml_Param{ id:support_interface_line_width; objectName: "support_interface_line_width"
                    fdmLabel: "Support Interface Line Width"
                    fdmDescription: "Width of a single line for the support roof or floor."
                    fdmUnit: "mm"
                    fdmDefaultValue: 0.4
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                    fdmType: "float"
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_interface_enable.fdmValue
                    fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                    fdmValue: Number(line_width.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_nozzle_size,line_width,support_interface_extruder_nr"
                    FdmQml_Param{ id:support_roof_line_width; objectName: "support_roof_line_width"
                        fdmLabel: "Support Roof Line Width"
                        fdmDescription: "Width of a single support roof line."
                        fdmUnit: "mm"
                        fdmDefaultValue: 0.4
                        fdmMinimumValue: 0.001
                        fdmMinimumValueWarning: 0.4 * Number(machine_nozzle_size.fdmValue)
                        fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                        fdmType: "float"
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_roof_enable.fdmValue
                        fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                        fdmValue: Number(support_interface_line_width.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "machine_nozzle_size,support_interface_line_width,support_roof_extruder_nr"
                    }
                    FdmQml_Param{ id:support_bottom_line_width; objectName: "support_bottom_line_width"
                        fdmLabel: "Support Floor Line Width"
                        fdmDescription: "Width of a single support floor line."
                        fdmUnit: "mm"
                        fdmDefaultValue: 0.4
                        fdmMinimumValue: 0.001
                        fdmMinimumValueWarning: 0.4 * Number(machine_nozzle_size.fdmValue)
                        fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                        fdmType: "float"
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_bottom_enable.fdmValue
                        fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                        fdmValue: Number(support_interface_line_width.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "machine_nozzle_size,support_interface_line_width,support_bottom_extruder_nr"
                    }
                }
                FdmQml_Param{ id:prime_tower_line_width; objectName: "prime_tower_line_width"
                    fdmLabel: "Prime Tower Line Width"
                    fdmDescription: "Width of a single prime tower line."
                    fdmType: "float"
                    fdmUnit: "mm"
                    fdmEnabled: prime_tower_enable.fdmValue
                    fdmDefaultValue: 0.4
                    fdmValue: Number(line_width.fdmValue)
                    fdmMinimumValue: 0.001
                    fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_nozzle_size,line_width"
                }
            }
            FdmQml_Param{ id:initial_layer_line_width_factor; objectName: "initial_layer_line_width_factor"
                fdmLabel: "Initial Layer Line Width"
                fdmDescription: "This is a multipliyer of the first layer line width. Increasing this could improve bed adhesion."
                fdmType: "float"
                fdmUnit: "%"
                fdmDefaultValue: 80.0
                fdmMinimumValue: 0.001
                fdmMaximumValueWarning: 150.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:shell; objectName: "shell"
            fdmLabel: "Walls"
            fdmIcon: "PrintShell"
            fdmDescription: "Shell"
            fdmType: "category"
            FdmQml_Param{ id:wall_extruder_nr; objectName: "wall_extruder_nr"
                fdmLabel: "Wall Extruder"
                fdmDescription: "The extruder train used for printing the walls. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: -1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmSettableGlobally: true
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                fdmAffectedById: ""
                FdmQml_Param{ id:wall_0_extruder_nr; objectName: "wall_0_extruder_nr"
                    fdmLabel: "Outer Wall Extruder"
                    fdmDescription: "The extruder train used for printing the outer wall. This is used in multi-extrusion."
                    fdmType: "int"
                    fdmValue: Number(wall_extruder_nr.fdmValue)
                    fdmDefaultValue: -1
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: true
                    fdmSettableGlobally: true
                    fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                    fdmAffectedById: "wall_extruder_nr"
                }
                FdmQml_Param{ id:wall_x_extruder_nr; objectName: "wall_x_extruder_nr"
                    fdmLabel: "Inner Wall Extruder"
                    fdmDescription: "The extruder train used for printing the inner walls. This is used in multi-extrusion."
                    fdmType: "int"
                    fdmValue: Number(wall_extruder_nr.fdmValue)
                    fdmDefaultValue: -1
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: true
                    fdmSettableGlobally: true
                    fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                    fdmAffectedById: "wall_extruder_nr"
                }
            }
            FdmQml_Param{ id:wall_thickness; objectName: "wall_thickness"
                fdmLabel: "Wall Thickness"
                fdmDescription: "The thickness of the walls in the horizontal direction. This value divided by the wall line width defines the number of walls."
                fdmUnit: "mm"
                fdmDefaultValue: 1.24
                fdmValue: magic_spiralize.fdmValue ? wall_line_width_0.fdmValue : 1.24  // (wall_line_width_0.fdmValuewall_line_width_0.fdmValue + wall_line_width_x.fdmValue * Math.max(0, wall_line_count.fdmValue-1))
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(line_width.fdmValue)
                fdmMaximumValueWarning: 10 * Number(line_width.fdmValue)
                fdmType: "float"
                fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "line_width,wall_x_extruder_nr"
                FdmQml_Param{ id:wall_line_count; objectName: "wall_line_count"
                    fdmLabel: "Wall Line Count"
                    fdmDescription: "The number of walls. When calculated by the wall thickness, this value is rounded to a whole number."
                    fdmDefaultValue: 2
                    fdmMinimumValue: 0
                    fdmMinimumValueWarning: 1
                    fdmMaximumValueWarning: 10
                    fdmType: "int"
                    fdmValue: magic_spiralize.fdmValue ? 1 : (Number(wall_thickness.fdmValue) !== 0 ? (Math.max(1, Math.round((Number(wall_thickness.fdmValue) - Number(wall_line_width_0.fdmValue)) / Number(wall_line_width_x.fdmValue)) + 1)) : 0)
                    fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_line_width_0,wall_line_width_x,wall_x_extruder_nr,wall_thickness,magic_spiralize"
                }
            }
            FdmQml_Param{ id:wall_0_wipe_dist; objectName: "wall_0_wipe_dist"
                fdmLabel: "Outer Wall Wipe Distance"
                fdmDescription: "Distance of a travel move inserted after the outer wall. This can better hide the Z seam."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.2
                fdmValue: Number(machine_nozzle_size.fdmValue) / 2
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue) * 2
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,wall_0_extruder_nr"
            }
            FdmQml_Param{ id:wall_0_inset; objectName: "wall_0_inset"
                fdmLabel: "Outer Wall Inset"
                fdmDescription: "Inset applied to the outer wall path. If the outer wall is smaller than the nozzle and printed after the inner walls, use this offset to get the nozzle to overlap with the inner walls instead of the model's outer wall."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmValue: (Number(wall_line_width_0.fdmValue) < Number(machine_nozzle_size.fdmValue) && !outer_inset_first.fdmValue) ? ((Number(machine_nozzle_size.fdmValue) - Number(wall_line_width_0.fdmValue)) / 2) : 0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,wall_line_width_0,wall_0_extruder_nr,outer_inset_first"
            }
            FdmQml_Param{ id:optimize_wall_printing_order; objectName: "optimize_wall_printing_order"
                fdmLabel: "Optimize Wall Printing Order"
                fdmDescription: "Optimize the wall print order to reduce the number of retractions and the travel distance. Most parts will benefit from this being enabled, but some may actually take longer. Please compare the print time estimates with and without optimization. The first layer is not optimized when choosing brim as the build plate adhesion type."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:outer_inset_first; objectName: "outer_inset_first"
                fdmLabel: "Outer Before Inner Walls"
                fdmDescription: "Prints walls from outside to inside when enabled. This can help improve dimensional accuracy on the X and Y planes when using a high viscosity plastic like ABS; however, it can decrease outer surface print quality, especially on overhangs."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: Number(wall_0_extruder_nr.fdmValue) === Number(wall_x_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:alternate_extra_perimeter; objectName: "alternate_extra_perimeter"
                fdmLabel: "Alternate Extra Wall"
                fdmDescription: "Prints an extra wall at every other layer. This helps the infill build up between these extra walls for stronger prints."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:travel_compensate_overlapping_walls_enabled; objectName: "travel_compensate_overlapping_walls_enabled"
                fdmLabel: "Compensate Wall Overlaps"
                fdmDescription: "Reduce the flow for parts of a wall being printed where there is already a wall in place."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_x_extruder_nr"
                FdmQml_Param{ id:travel_compensate_overlapping_walls_0_enabled; objectName: "travel_compensate_overlapping_walls_0_enabled"
                    fdmLabel: "Compensate Outer Wall Overlaps"
                    fdmDescription: "Reduce the flow for parts of an outer wall being printed where there is already a wall in place."
                    fdmType: "bool"
                    fdmDefaultValue: true
                    fdmValue: travel_compensate_overlapping_walls_enabled.fdmValue
                    fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_0_extruder_nr,travel_compensate_overlapping_walls_enabled"
                }
                FdmQml_Param{ id:travel_compensate_overlapping_walls_x_enabled; objectName: "travel_compensate_overlapping_walls_x_enabled"
                    fdmLabel: "Compensate Inner Wall Overlaps"
                    fdmDescription: "Reduce the flow for parts of an inner wall being printed where there is already a wall in place."
                    fdmType: "bool"
                    fdmDefaultValue: true
                    fdmValue: travel_compensate_overlapping_walls_enabled.fdmValue
                    fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_x_extruder_nr,travel_compensate_overlapping_walls_enabled"
                }
            }
            FdmQml_Param{ id:wall_min_flow; objectName: "wall_min_flow"
                fdmLabel: "Minimum Wall Flow"
                fdmDescription: "Minimum allowed wall line percentage flow. Walls with flows less than this value will be replaced with a travel move. When using this setting, you must enable Wall Overlap Compensation and print the outer wall before in the inner walls."
                fdmUnit: "%"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 100.0
                fdmDefaultValue: 0.0
                fdmType: "float"
                fdmEnabled: travel_compensate_overlapping_walls_0_enabled.fdmValue || travel_compensate_overlapping_walls_x_enabled.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wall_min_flow_retract; objectName: "wall_min_flow_retract"
                fdmLabel: "Prefer Retract"
                fdmDescription: "If enabled, retraction is used rather than combing for travel moves replacing walls with flows below the minimum flow threshold."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: (travel_compensate_overlapping_walls_0_enabled.fdmValue || travel_compensate_overlapping_walls_x_enabled.fdmValue) && Number(wall_min_flow.fdmValue) > 0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:fill_perimeter_gaps; objectName: "fill_perimeter_gaps"
                fdmLabel: "Fill Gaps Between Walls"
                fdmDescription: "Fills the gaps between walls where no walls fit."
                fdmType: "enum"
                fdmOptions:{
                    "nowhere": "Nowhere",
                    "everywhere": "Everywhere"
                }
                fdmDefaultValue: "everywhere"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:filter_out_tiny_gaps; objectName: "filter_out_tiny_gaps"
                fdmLabel: "Filter Out Tiny Gaps"
                fdmDescription: "Filter out tiny gaps to reduce blobs on the outside of the model."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:fill_outline_gaps; objectName: "fill_outline_gaps"
                fdmLabel: "Print Thin Walls"
                fdmDescription: "Print pieces of the model which are horizontally thinner than the nozzle size."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:xy_offset; objectName: "xy_offset"
                fdmLabel: "Horizontal Expansion"
                fdmDescription: "Amount of offset applied to all polygons in each layer. Positive values can compensate for holes too large; negative values can compensate for holes too small."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValueWarning: -1.0
                fdmMaximumValueWarning: 1.0
                fdmDefaultValue: 0.0
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:xy_offset_layer_0; objectName: "xy_offset_layer_0"
                fdmLabel: "Initial Layer Horizontal Expansion"
                fdmDescription: "Amount of offset applied to all polygons in the first layer. A negative value can compensate for squishing of the first layer known as \"elephant's foot\"."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValueWarning: -1.0
                fdmMaximumValueWarning: 1.0
                fdmDefaultValue: 0.0
                fdmValue: Number(xy_offset.fdmValue)
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr,xy_offset"
            }
            FdmQml_Param{ id:hole_xy_offset; objectName: "hole_xy_offset"
                fdmLabel: "Hole Horizontal Expansion"
                fdmDescription: "Amount of offset applied to the holes in each layer. Positive values increase the size of the holes, negative values reduce the size of the holes."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValueWarning: -1.0
                fdmMaximumValueWarning: 1.0
                fdmDefaultValue: 0.0
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:z_seam_type; objectName: "z_seam_type"
                fdmLabel: "Z Seam Alignment"
                fdmDescription: "Starting point of each path in a layer. When paths in consecutive layers start at the same point, a vertical seam may show on the print. When aligning these near a user specified location, the seam is easiest to remove. When placed randomly, the inaccuracies at the paths' starting point will be less noticeable."
                fdmType: "enum"
                fdmOptions:{
                    "back": "User Specified",
                    "shortest": "Shortest",
                    "random": "Random",
                    "sharpest_corner": "Sharpest Corner"
                }
                fdmDefaultValue: "sharpest_corner"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:z_seam_position; objectName: "z_seam_position"
                fdmLabel: "Z Seam Position"
                fdmDescription: "The position where printing begins in each part of a layer."
                fdmType: "enum"
                fdmOptions:{
                    "backleft": "Back Left",
                    "back": "Back",
                    "backright": "Back Right",
                    "right": "Right",
                    "frontright": "Front Right",
                    "front": "Front",
                    "frontleft": "Front Left",
                    "left": "Left"
                }
                fdmEnabled: z_seam_type.fdmValue === "back"
                fdmDefaultValue: "back"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
                FdmQml_Param{ id:z_seam_x; objectName: "z_seam_x"
                    fdmLabel: "Z Seam X"
                    fdmDescription: "The X coordinate of the position where each layer part begins to print."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: ((z_seam_position.fdmValue === "frontleft" || z_seam_position.fdmValue === "left" || z_seam_position.fdmValue === "backleft") ? 0 : ((z_seam_position.fdmValue === "front" || z_seam_position.fdmValue === "back") ? (Number(machine_width.fdmValue) / 2) : Number(machine_width.fdmValue)) - ((z_seam_relative.fdmValue || machine_center_is_zero.fdmValue) ? (Number(machine_width.fdmValue) / 2) : 0))
                    fdmEnabled: z_seam_type.fdmValue === "back"
                    fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_width,machine_center_is_zero,wall_0_extruder_nr,z_seam_position,z_seam_relative"
                }
                FdmQml_Param{ id:z_seam_y; objectName: "z_seam_y"
                    fdmLabel: "Z Seam Y"
                    fdmDescription: "The Y coordinate of the position where each layer part begins to print."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: ((z_seam_position.fdmValue === "frontleft" || z_seam_position.fdmValue === "front" || z_seam_position.fdmValue === "frontright") ? 0 : ((z_seam_position.fdmValue === "left" || z_seam_position.fdmValue === "right") ? (Number(machine_depth.fdmValue) / 2) : Number(machine_depth.fdmValue)) - ((z_seam_relative.fdmValue || machine_center_is_zero.fdmValue) ? (Number(machine_depth.fdmValue) / 2) : 0))
                    fdmEnabled: z_seam_type.fdmValue === "back"
                    fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_depth,machine_center_is_zero,wall_0_extruder_nr,z_seam_position,z_seam_relative"
                }
            }
            FdmQml_Param{ id:z_seam_corner; objectName: "z_seam_corner"
                fdmLabel: "Seam Corner Preference"
                fdmDescription: "Control whether corners on the model outline influence the position of the seam. None means that corners have no influence on the seam position. Hide Seam makes the seam more likely to occur on an inside corner. Expose Seam makes the seam more likely to occur on an outside corner. Hide or Expose Seam makes the seam more likely to occur at an inside or outside corner. Smart Hiding allows both inside and outside corners, but chooses inside corners more frequently, if appropriate."
                fdmType: "enum"
                fdmOptions:{
                    "z_seam_corner_none": "None",
                    "z_seam_corner_inner": "Hide Seam",
                    "z_seam_corner_outer": "Expose Seam",
                    "z_seam_corner_any": "Hide or Expose Seam",
                    "z_seam_corner_weighted": "Smart Hiding"
                }
                fdmDefaultValue: "z_seam_corner_inner"
                fdmEnabled: z_seam_type.fdmValue !== "random"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:z_seam_relative; objectName: "z_seam_relative"
                fdmLabel: "Z Seam Relative"
                fdmDescription: "When enabled, the z seam coordinates are relative to each part's center point. When disabled, the coordinates define an absolute position on the build plate."
                fdmUnit: "mm"
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: z_seam_type.fdmValue === "back"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }

            FdmQml_Param{ id:z_seam_min_angle_diff; objectName: "z_seam_min_angle_diff"
                fdmLabel: "Z Seam Min Angle Diff"
                fdmDescription: "When the angle difference between the Sharpest Corner type point and the Shortest type point is less than this value, the Shortest type point is selected."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValueWarning: 0
                fdmMaximumValueWarning: 180
                fdmDefaultValue: 36
                fdmEnabled: z_seam_type.fdmValue === "sharpest_corner"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }

            FdmQml_Param{ id:z_seam_max_angle; objectName: "z_seam_max_angle"
                fdmLabel: "Z Seam Max Angle"
                fdmDescription: "Only corners smaller than this value are considered sharp corners. If there is no sharp corner point, select the Shortest type point."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValueWarning: 0
                fdmMaximumValueWarning: 180
                fdmDefaultValue: 162
                fdmEnabled: z_seam_type.fdmValue === "sharpest_corner"
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
        }
        FdmQml_Category{ id:top_bottom; objectName: "top_bottom"
            fdmLabel: "Top / Bottom"
            fdmIcon: "PrintTopBottom"
            fdmDescription: "Differentiates between the top and bottom."
            fdmType: "category"
            FdmQml_Param{ id:top_bottom_extruder_nr; objectName: "top_bottom_extruder_nr"
                fdmLabel: "Top / Bottom Extruder"
                fdmDescription: "The extruder train used for printing the top and bottom skin. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: -1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmSettableGlobally: true
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:top_bottom_thickness; objectName: "top_bottom_thickness"
                fdmLabel: "Top / Bottom Thickness"
                fdmDescription: "The thickness of the top and bottom layers. This value divided by the layer height defines the number of top/bottom layers."
                fdmUnit: "mm"
                fdmDefaultValue: 0.8
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.6
                fdmMaximumValue: Number(machine_height.fdmValue)
                fdmType: "float"
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_height,top_bottom_extruder_nr"
                FdmQml_Param{ id:top_thickness; objectName: "top_thickness"
                    fdmLabel: "Top Thickness"
                    fdmDescription: "The thickness of the top layers. This value divided by the layer height defines the number of top layers."
                    fdmUnit: "mm"
                    fdmDefaultValue: 0.8
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 0.2 + Number(layer_height.fdmValue)
                    fdmMaximumValue: Number(machine_height.fdmValue)
                    fdmType: "float"
                    fdmValue: Number(top_bottom_thickness.fdmValue)
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_height,layer_height,top_bottom_extruder_nr,top_bottom_thickness"
                    FdmQml_Param{ id:top_layers; objectName: "top_layers"
                        fdmLabel: "Top Layers"
                        fdmDescription: "The number of top layers. When calculated by the top thickness, this value is rounded to a whole number."
                        fdmDefaultValue: 8
                        fdmMinimumValue: 0
                        fdmMaximumValueWarning: 100
                        fdmType: "int"
                        fdmMinimumValueWarning: 2
                        fdmValue: Number(infill_sparse_density.fdmValue) === 100 ? 0 : (Math.ceil (Math.round(Number(top_thickness.fdmValue) / Number(layer_height.fdmValue), 4)))
                        fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "layer_height,top_bottom_extruder_nr,top_thickness,infill_sparse_density"
                    }
                }
                FdmQml_Param{ id:bottom_thickness; objectName: "bottom_thickness"
                    fdmLabel: "Bottom Thickness"
                    fdmDescription: "The thickness of the bottom layers. This value divided by the layer height defines the number of bottom layers."
                    fdmUnit: "mm"
                    fdmDefaultValue: 0.6
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 0.2 + Number(layer_height.fdmValue)
                    fdmType: "float"
                    fdmValue: Number(top_bottom_thickness.fdmValue)
                    fdmMaximumValue: Number(machine_height.fdmValue)
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_height,layer_height,top_bottom_extruder_nr,top_bottom_thickness"
                    FdmQml_Param{ id:bottom_layers; objectName: "bottom_layers"
                        fdmLabel: "Bottom Layers"
                        fdmDescription: "The number of bottom layers. When calculated by the bottom thickness, this value is rounded to a whole number."
                        fdmMinimumValue: 0
                        fdmMinimumValueWarning: 2
                        fdmDefaultValue: 6
                        fdmType: "int"
                        fdmValue: Number(infill_sparse_density.fdmValue) === 100 ? 999999 : (Math.ceil (Math.round(Number(bottom_thickness.fdmValue) / Number(layer_height.fdmValue), 4)))
                        fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "layer_height,top_bottom_extruder_nr,bottom_thickness,infill_sparse_density"
                    }
                    FdmQml_Param{ id:initial_bottom_layers; objectName: "initial_bottom_layers"
                        fdmLabel: "Initial Bottom Layers"
                        fdmDescription: "The number of initial bottom layers from the build-plate upward. When calculated by the bottom thickness, this value is rounded to a whole number."
                        fdmMinimumValue: 0
                        fdmMinimumValueWarning: 2
                        fdmDefaultValue: 6
                        fdmType: "int"
                        fdmValue: Number(bottom_layers.fdmValue)
                        fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "top_bottom_extruder_nr,bottom_layers"
                    }
                }
            }
            FdmQml_Param{ id:roofing_extruder_nr; objectName: "roofing_extruder_nr"
                fdmLabel: "Top Surface Skin Extruder"
                fdmDescription: "The extruder train used for printing the top most skin. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: -1
                fdmValue: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmSettableGlobally: true
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1 && Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:roofing_layer_count; objectName: "roofing_layer_count"
                fdmLabel: "Top Surface Skin Layers"
                fdmDescription: "The number of top skin layers. Usually only one top layer is sufficient to generate higher quality top surfaces."
                fdmDefaultValue: 0
                fdmMinimumValue: 0
                fdmMaximumValueWarning: Number(top_layers.fdmValue) - 1
                fdmType: "int"
                fdmValue: 0
                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmEnabled: Number(top_layers.fdmValue) > 0
                fdmAffectedById: "top_layers,roofing_extruder_nr"
            }
            FdmQml_Param{ id:top_bottom_pattern; objectName: "top_bottom_pattern"
                fdmLabel: "Top / Bottom Pattern"
                fdmDescription: "The pattern of the top / bottom layers."
                fdmType: "enum"
                fdmOptions:{
                    "lines": "Lines",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag"
                }
                fdmDefaultValue: "lines"
                fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:top_bottom_pattern_0; objectName: "top_bottom_pattern_0"
                fdmLabel: "Bottom Pattern Initial Layer"
                fdmDescription: "The pattern on the bottom of the print on the first layer."
                fdmType: "enum"
                fdmOptions:{
                    "lines": "Lines",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag"
                }
                fdmDefaultValue: "lines"
                fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                fdmValue: top_bottom_pattern.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,top_bottom_pattern"
            }
            FdmQml_Param{ id:connect_skin_polygons; objectName: "connect_skin_polygons"
                fdmLabel: "Connect Top / Bottom Polygons"
                fdmDescription: "Connect top and bottom skin paths where they run next to each other. For concentric patterns, this setting greatly reduces the travel time, but can reduce top surface quality because the connections can happen midway over the infill."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: ((Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && top_bottom_pattern.fdmValue === "concentric") || (Number(initial_bottom_layers.fdmValue) > 0 && top_bottom_pattern_0.fdmValue === "concentric") || (Number(roofing_layer_count.fdmValue) > 0 && roofing_pattern.fdmValue === "concentric")
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:skin_monotonic; objectName: "skin_monotonic"
                fdmLabel: "Monotonic Top / Bottom Order"
                fdmDescription: "Print top and bottom lines in an order where they always overlap with adjacent lines in a single direction. This takes slightly more time to print, but makes flat surfaces look more consistent."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && (top_bottom_pattern.fdmValue !== "concentric" || top_bottom_pattern_0.fdmValue !== "concentric")
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:skin_angles; objectName: "skin_angles"
                fdmLabel: "Top / Bottom Line Directions"
                fdmDescription: "Use a list of integer line directions when the top / bottom layers use lines or zig-zag patterns. List elements are used sequentially as the layers progress. When the end of the list is reached, the list starts over The list items are separated by commas, and the whole list is constrained in square brackets. The default is an empty list whih uses traditional default angles of 45° and 135°."
                fdmType: "[int]"
                fdmDefaultValue: "[ ]"
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && top_bottom_pattern.fdmValue !== "concentric"
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:skin_no_small_gaps_heuristic; objectName: "skin_no_small_gaps_heuristic"
                fdmLabel: "No Skin in Z Gaps"
                fdmDescription: "This setting will not generate skin in small vertical gaps. This improves printing and slicing time, but leaves infills exposed to the air."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:skin_outline_count; objectName: "skin_outline_count"
                fdmLabel: "Extra Skin Wall Count"
                fdmDescription: "Replaces the outermost part of the top / bottom pattern with a number of concentric lines. Using one or two lines improves roofs that start on infill material."
                fdmValue: (top_bottom_pattern.fdmValue === "concentric" && top_bottom_pattern_0.fdmValue === "concentric" && Number(roofing_layer_count.fdmValue) <= 0) ? 0 : 1
                fdmDefaultValue: 1
                fdmMinimumValue: 0
                fdmMaximumValueWarning: 10
                fdmType: "int"
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && (top_bottom_pattern.fdmValue !== "concentric" || top_bottom_pattern_0.fdmValue !== "concentric" || (Number(roofing_layer_count.fdmValue) > 0 && roofing_pattern.fdmValue !== "concentric"))
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,roofing_layer_count,top_bottom_pattern,top_bottom_pattern_0"
            }
            FdmQml_Param{ id:top_skin_density; objectName: "top_skin_density"
                fdmLabel: "Top Skin Infill Density"
                fdmDescription: "Adjusts the Top Skin Infill Density."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 110.0
                fdmMinimumValue: 100.0
                fdmMaximumValueWarning: 200.0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:top_surface_one_wall; objectName: "top_surface_one_wall"
                fdmLabel: "Use One Wall On Top Surface"
                fdmDescription: "Use only one wall on flat top surface ,to give more space to the top infill pattern"
                fdmValue: (top_bottom_pattern.fdmValue === "concentric" && top_bottom_pattern_0.fdmValue === "concentric" && Number(roofing_layer_count.fdmValue) <= 0) ? false : true
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && (top_bottom_pattern.fdmValue !== "concentric" || top_bottom_pattern_0.fdmValue !== "concentric" || (Number(roofing_layer_count.fdmValue) > 0 && roofing_pattern.fdmValue !== "concentric"))
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,roofing_layer_count,top_bottom_pattern,top_bottom_pattern_0"
            }
            FdmQml_Param{ id:ironing_enabled; objectName: "ironing_enabled"
                fdmLabel: "Enable Ironing"
                fdmDescription: "Go over the top surface one additional time, but this time with very little extruded material. This better melts the top plastic to create a smooth surface. The pressure in the nozzle chamber is kept high so that surface creases are filled."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_only_highest_layer; objectName: "ironing_only_highest_layer"
                fdmLabel: "Iron Only Highest Layer"
                fdmDescription: "Only perform ironing on the last mesh layer. This saves time if the lower layers don't need a smooth surface finish."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_pattern; objectName: "ironing_pattern"
                fdmLabel: "Ironing Pattern"
                fdmDescription: "The pattern used when ironing top surfaces."
                fdmType: "enum"
                fdmOptions:{
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag"
                }
                fdmDefaultValue: "zigzag"
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_monotonic; objectName: "ironing_monotonic"
                fdmLabel: "Monotonic Ironing Order"
                fdmDescription: "Print ironing lines in an order that always overlap in a single direcrtion. This increases print times, but makes flat surfaces look consistent."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: ironing_enabled.fdmValue && ironing_pattern.fdmValue !== "concentric"
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_line_spacing; objectName: "ironing_line_spacing"
                fdmLabel: "Ironing Line Spacing"
                fdmDescription: "The distance between the ironing lines."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.1
                fdmMinimumValue: 0.001
                fdmMaximumValueWarning: Number(machine_nozzle_tip_outer_diameter.fdmValue)
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_tip_outer_diameter,top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_flow; objectName: "ironing_flow"
                fdmLabel: "Ironing Flow"
                fdmDescription: "The amount of material, relative to a normal skin line, to extrude during ironing. Keeping the nozzle filled helps filling some of the crevices of the top surface, but too much results in overextrusion and blips on the side of the surface."
                fdmType: "float"
                fdmUnit: "%"
                fdmDefaultValue: 10.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 50.0
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
            }
            FdmQml_Param{ id:ironing_inset; objectName: "ironing_inset"
                fdmLabel: "Ironing Inset"
                fdmDescription: "A distance to keep from the edges of the model. Ironing all the way to the edge of the mesh may result in a jagged edge on your print."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.35
                fdmValue: Number(wall_line_width_0.fdmValue) / 2 + (ironing_pattern.fdmValue === "concentric" ? (Number(ironing_line_spacing.fdmValue) - Number(skin_line_width.fdmValue) * (1.0 + Number(ironing_flow.fdmValue) / 100) / 2) : (Number(skin_line_width.fdmValue) * (1.0 - Number(ironing_flow.fdmValue) / 100) / 2))
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: Number(wall_line_width_0.fdmValue)
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_line_width_0,skin_line_width,top_bottom_extruder_nr,ironing_pattern,ironing_line_spacing,ironing_flow"
            }
            FdmQml_Param{ id:speed_ironing; objectName: "speed_ironing"
                fdmLabel: "Ironing Speed"
                fdmDescription: "Set top-surface passing speed."
                fdmType: "float"
                fdmUnit: "mm/s"
                fdmDefaultValue: 20.0
                fdmValue: Number(speed_topbottom.fdmValue) * 20 / 30
                fdmMinimumValue: 0.001
                fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                fdmMaximumValueWarning: 100.0
                fdmEnabled: ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,top_bottom_extruder_nr,speed_topbottom"
            }
            FdmQml_Param{ id:acceleration_ironing; objectName: "acceleration_ironing"
                fdmLabel: "Ironing Acceleration"
                fdmDescription: "How much acceleration is used when ironing."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmMinimumValue: 0.1
                fdmMinimumValueWarning: 100.0
                fdmMaximumValueWarning: 10000.0
                fdmDefaultValue: 3000.0
                fdmValue: Number(acceleration_topbottom.fdmValue)
                fdmEnabled: acceleration_enabled.fdmValue && ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,acceleration_topbottom"
            }
            FdmQml_Param{ id:jerk_ironing; objectName: "jerk_ironing"
                fdmLabel: "Ironing Jerk"
                fdmDescription: "The max instantaneous velocity change while ironing."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 50.0
                fdmDefaultValue: 20.0
                fdmValue: Number(jerk_topbottom.fdmValue)
                fdmEnabled: jerk_enabled.fdmValue && ironing_enabled.fdmValue
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,jerk_topbottom"
            }
            FdmQml_Param{ id:skin_overlap; objectName: "skin_overlap"
                fdmLabel: "Skin Overlap Percentage"
                fdmDescription: "Adjust the amount of overlap between the walls and the skin-centerline endpoints. This is a percentage of the skin lines and inntermost wall line widths.A slight overlap allows walls to connect firmly to the skin. Note that with equal skin and wall line width, percentages over 50% may cause the skin to go past the wall."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 0
                fdmMinimumValueWarning: -50.0
                fdmMaximumValueWarning: 100.0
                fdmValue: top_bottom_pattern.fdmValue !== "concentric" ? 5 : 0
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && top_bottom_pattern.fdmValue !== "concentric"
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr,top_bottom_pattern"
                FdmQml_Param{ id:skin_overlap_mm; objectName: "skin_overlap_mm"
                    fdmLabel: "Skin Overlap"
                    fdmDescription: "Adjust the overlap between the walls and the skin-centerline endpoints. A slight overlap allows walls to connect firmly to the skin.Note that with equal skin and wall line width, values over half the wall witdh may cause the skin to go past the wall."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.00
                    fdmMinimumValueWarning: -0.5 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                    fdmValue: top_bottom_pattern.fdmValue !== "concentric" ? (0.5 * (Number(skin_line_width.fdmValue) + (Number(wall_line_count.fdmValue) > 1 ? Number(wall_line_width_x.fdmValue) : Number(wall_line_width_0.fdmValue))) * Number(skin_overlap.fdmValue) / 100 ) : 0
                    fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && top_bottom_pattern.fdmValue !== "concentric"
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,wall_line_width_0,wall_line_width_x,skin_line_width,wall_line_count,top_bottom_pattern,skin_overlap"
                }
            }
            FdmQml_Param{ id:skin_preshrink; objectName: "skin_preshrink"
                fdmLabel: "Skin Removal Width"
                fdmDescription: "Eliminte any skin area smaller than this value. This helps limit the amount of time and material spent on printing top and bottom skin on the model's slanted surfaces."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmValue: Number(wall_line_width_0.fdmValue) + (Number(wall_line_count.fdmValue) - 1) * Number(wall_line_width_x.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(wall_line_width_0.fdmValue) + (Number(wall_line_count.fdmValue) - 1) * Number(wall_line_width_x.fdmValue)
                fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_line_width_0,wall_line_width_x,wall_line_count,top_bottom_extruder_nr"
                FdmQml_Param{ id:top_skin_preshrink; objectName: "top_skin_preshrink"
                    fdmLabel: "Top Skin Removal Width"
                    fdmDescription: "Eliminte any top skin area smaller than this value. This helps limit the amount of time and material spent on printing top skin on the model's slanted surfaces."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(skin_preshrink.fdmValue)
                    fdmMaximumValueWarning: Number(wall_line_width_0.fdmValue) + (Number(wall_line_count.fdmValue) - 1) * Number(wall_line_width_x.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_line_width_0,wall_line_width_x,wall_line_count,top_bottom_extruder_nr,skin_preshrink"
                }
                FdmQml_Param{ id:bottom_skin_preshrink; objectName: "bottom_skin_preshrink"
                    fdmLabel: "Bottom Skin Removal Width"
                    fdmDescription: "Eliminte any bottom skin area smaller than this value. This helps limit the amount of time and material spent on printing bottom skin on the model's slanted surfaces."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(skin_preshrink.fdmValue)
                    fdmMaximumValueWarning: Number(wall_line_width_0.fdmValue) + (Number(wall_line_count.fdmValue) - 1) * Number(wall_line_width_x.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_line_width_0,wall_line_width_x,wall_line_count,top_bottom_extruder_nr,skin_preshrink"
                }
            }
            FdmQml_Param{ id:expand_skins_expand_distance; objectName: "expand_skins_expand_distance"
                fdmLabel: "Skin Expand Distance"
                fdmDescription: "The distance the skins are expanded into the infill. Higher values makes the skin attach better to the infill pattern and makes the walls on neighboring layers adhere better to the skin. Lower values save material."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmValue: Number(wall_line_width_0.fdmValue) + (Number(wall_line_count.fdmValue) - 1) * Number(wall_line_width_x.fdmValue)
                fdmMinimumValue: -Number(skin_preshrink.fdmValue)
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_line_width_0,wall_line_width_x,wall_line_count,top_bottom_extruder_nr,skin_preshrink"
                FdmQml_Param{ id:top_skin_expand_distance; objectName: "top_skin_expand_distance"
                    fdmLabel: "Top Skin Expand Distance"
                    fdmDescription: "The distance the top skins are expanded into the infill. Higher values makes the skin attach better to the infill pattern and makes the walls on the layer above adhere better to the skin. Lower values save material."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(expand_skins_expand_distance.fdmValue)
                    fdmMinimumValue: -Number(top_skin_preshrink.fdmValue)
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "top_bottom_extruder_nr,top_skin_preshrink,expand_skins_expand_distance"
                }
                FdmQml_Param{ id:bottom_skin_expand_distance; objectName: "bottom_skin_expand_distance"
                    fdmLabel: "Bottom Skin Expand Distance"
                    fdmDescription: "The distance the bottom skins are expanded into the infill. Higher values makes the skin attach better to the infill pattern and makes the skin adhere better to the walls on the layer below. Lower values save material."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(expand_skins_expand_distance.fdmValue)
                    fdmMinimumValue: -Number(bottom_skin_preshrink.fdmValue)
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "top_bottom_extruder_nr,bottom_skin_preshrink,expand_skins_expand_distance"
                }
            }
            FdmQml_Param{ id:max_skin_angle_for_expansion; objectName: "max_skin_angle_for_expansion"
                fdmLabel: "Maximum Skin Angle for Expansion"
                fdmDescription: "Stop skin expansion for top and bottom surfaces larger than this input. Use this if you want to avoid expanding narrow skin areas created when the model surface has a near-vertical slope. An an angle of 0° is horizontal and will not have skin expansion. An angle of 90° is vertical and will lead all skin to be expanded."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 2.0
                fdmMaximumValue: 90.0
                fdmDefaultValue: 90.0
                fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && (Number(top_skin_expand_distance.fdmValue) > 0 || Number(bottom_skin_expand_distance.fdmValue) > 0)
                fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "top_bottom_extruder_nr"
                FdmQml_Param{ id:min_skin_width_for_expansion; objectName: "min_skin_width_for_expansion"
                    fdmLabel: "Minimum Skin Width for Expansion"
                    fdmDescription: "Skin areas narrower than this are not expanded. This avoids expanding the narrow skin areas that are created when the model surface has a slope close to the vertical."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.0
                    fdmValue: top_layers.fdmValue * layer_height.fdmValue / Math.tan(max_skin_angle_for_expansion.fdmValue * Math.PI / 180.0)
                    fdmMinimumValue: 0.0
                    fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && (Number(top_skin_expand_distance.fdmValue) > 0 || Number(bottom_skin_expand_distance.fdmValue) > 0)
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "top_bottom_extruder_nr"
                }
            }
        }
        FdmQml_Category{ id:infill; objectName: "infill"
            fdmLabel: "Infill"
            fdmIcon: "Infill1"
            fdmDescription: "Infill"
            fdmType: "category"
            FdmQml_Param{ id:infill_extruder_nr; objectName: "infill_extruder_nr"
                fdmLabel: "Infill Extruder"
                fdmDescription: "The extruder train used for printing infill. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: -1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmSettableGlobally: true
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:infill_sparse_density; objectName: "infill_sparse_density"
                fdmLabel: "Infill Density"
                fdmDescription: "Adjusts the density of infill of the print."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 100.0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
                FdmQml_Param{ id:infill_line_distance; objectName: "infill_line_distance"
                    fdmLabel: "Infill Line Distance"
                    fdmDescription: "Distance between the printed infill lines. This setting is calculated by the infill density and the infill line width."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 2.0
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(infill_line_width.fdmValue)
                    fdmValue: Number(infill_sparse_density.fdmValue) === 0 ? 0 : (Number(infill_line_width.fdmValue) * 100) / Number(infill_sparse_density.fdmValue) * (infill_pattern.fdmValue === "grid" ? 2 : ((infill_pattern.fdmValue === "triangles" || infill_pattern.fdmValue === "trihexagon" || infill_pattern.fdmValue === "cubic" || infill_pattern.fdmValue === "cubicsubdiv") ? 3 : ((infill_pattern.fdmValue === "tetrahedral" || infill_pattern.fdmValue === "quarter_cubic") ? 2 : ((infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d") ? 1 : 1))))
                    fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "infill_line_width,infill_extruder_nr,infill_sparse_density,infill_pattern"
                }
            }
            FdmQml_Param{ id:infill_pattern; objectName: "infill_pattern"
                fdmLabel: "Infill Pattern"
                fdmDescription: "The pattern of the infill material of the print. The line and zig zag infill swap direction on alternate layers, reducing material cost. The grid, triangle, tri-hexagon, cubic, octet, quarter cubic, cross, and concentric patterns are fully printed every layer. Gyroid, cubic, quarter cubic, and octet infill change with every layer to provide a more equal distribution of strength over each direction."
                fdmType: "enum"
                fdmOptions:{
                    "grid": "Grid",
                    "lines": "Lines",
                    "triangles": "Triangles",
                    "trihexagon": "Tri-Hexagon",
                    "cubic": "Cubic",
                    "cubicsubdiv": "Cubic Subdivision",
                    "tetrahedral": "Octet",
                    "quarter_cubic": "Quarter Cubic",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag",
                    "cross": "Cross",
                    "cross_3d": "Cross 3D",
                    "gyroid": "Gyroid"
                }
                fdmDefaultValue: "grid"
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmValue: Number(infill_sparse_density.fdmValue) > 25 ? "lines" : "grid"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_sparse_density"
            }
            FdmQml_Param{ id:zig_zaggify_infill; objectName: "zig_zaggify_infill"
                fdmLabel: "Connect Infill Lines"
                fdmDescription: "Connect the ends where the infill pattern meets the inner wall using a line which follows the shape of the inner wall. Enabling this setting can make the infill adhere to the walls better and reduce infill effects on vertical surface quality. Disabling this setting reduces the amount of material."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d"
                fdmEnabled: infill_pattern.fdmValue === "lines" || infill_pattern.fdmValue === "grid" || infill_pattern.fdmValue === "triangles" || infill_pattern.fdmValue === "trihexagon" || infill_pattern.fdmValue === "cubic" || infill_pattern.fdmValue === "tetrahedral" || infill_pattern.fdmValue === "quarter_cubic" || infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d" || infill_pattern.fdmValue === "gyroid"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_pattern"
            }
            FdmQml_Param{ id:connect_infill_polygons; objectName: "connect_infill_polygons"
                fdmLabel: "Connect Infill Polygons"
                fdmDescription: "Connect infill paths that run next to each other. For infill patterns which consist of several closed polygons, enabling this setting greatly reduces the travel time."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmValue: (infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d" || Number(infill_multiplier.fdmValue) % 2 === 0) && Number(infill_wall_line_count.fdmValue) > 0
                fdmEnabled: infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d" || infill_pattern.fdmValue === "concentric" || Number(infill_multiplier.fdmValue) % 2 === 0 || Number(infill_wall_line_count.fdmValue) > 1
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_pattern,infill_multiplier,infill_wall_line_count"
            }
            FdmQml_Param{ id:infill_angles; objectName: "infill_angles"
                fdmLabel: "Infill Line Directions"
                fdmDescription: "A list of integer line directions. Elements from this list are used sequentailly as layers progress. When the end of the list is reached, the directions begin again. The list items are separated by commas, and the whole list is contained in square brackets. The default is an empty list that uses traditional default angles of 45° and 135° for lines and zig-zag patterns. 45° is used for all other patterns."
                fdmType: "[int]"
                fdmDefaultValue: "[ ]"
                fdmEnabled: infill_pattern.fdmValue !== "concentric" && Number(infill_sparse_density.fdmValue) > 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_offset_x; objectName: "infill_offset_x"
                fdmLabel: "Infill X Offset"
                fdmDescription: "The infill pattern is moved this distance along the X axis."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmEnabled: infill_pattern.fdmValue === "grid" || infill_pattern.fdmValue === "lines" || infill_pattern.fdmValue === "triangles" || infill_pattern.fdmValue === "cubic" || infill_pattern.fdmValue === "tetrahedral" || infill_pattern.fdmValue === "quarter_cubic" || infill_pattern.fdmValue === "zigzag"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_offset_y; objectName: "infill_offset_y"
                fdmLabel: "Infill Y Offset"
                fdmDescription: "The infill pattern is moved this distance along the Y axis."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmEnabled: infill_pattern.fdmValue === "grid" || infill_pattern.fdmValue === "lines" || infill_pattern.fdmValue === "triangles" || infill_pattern.fdmValue === "cubic" || infill_pattern.fdmValue === "tetrahedral" || infill_pattern.fdmValue === "quarter_cubic" || infill_pattern.fdmValue === "zigzag"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_randomize_start_location; objectName: "infill_randomize_start_location"
                fdmLabel: "Randomize Infill Start"
                fdmDescription: "Randomize which infill line is printed first. This prevents one segment becoming the strongest, but it will increase the amount of travel moves."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValueWarning: (["grid", "triangles", "trihexagon", "cubic", "cubicsubdiv", "tetrahedral", "quarter_cubic"].indexOf(infill_pattern.fdmValue) < 0) ? true : false
                fdmEnabled:  !((infill_pattern.fdmValue === "cross" && connect_infill_polygons.fdmValue) || infill_pattern.fdmValue === "concentric")
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_multiplier; objectName: "infill_multiplier"
                fdmLabel: "Infill Line Multiplier"
                fdmDescription: "Convert each infill line to this many lines. The extra lines avoid each other to create a stiffer infill, but increases print time and material used."
                fdmDefaultValue: 1
                fdmType: "int"
                fdmMinimumValue: 1
                fdmMaximumValueWarning: Number(infill_line_distance.fdmValue) / Number(infill_line_width.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_pattern.fdmValue !== "zigzag" && (Number(gradual_infill_steps.fdmValue) === 0 || !zig_zaggify_infill.fdmValue)
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_line_width,infill_extruder_nr,infill_line_distance"
            }
            FdmQml_Param{ id:infill_wall_line_count; objectName: "infill_wall_line_count"
                fdmLabel: "Extra Infill Wall Count"
                fdmDescription: "Add extra walls around the infill area. Such walls can make top/bottom skin lines sag down less which means you need less top/bottom skin layers for the same quality at the cost of some extra material.\nThis feature can combine with the Connect Infill Polygons to connect all the infill into a single extrusion path without the need for travels or retractions if configured right."
                fdmDefaultValue: 0
                fdmType: "int"
                fdmMinimumValue: 0
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:sub_div_rad_add; objectName: "sub_div_rad_add"
                fdmLabel: "Cubic Subdivision Shell"
                fdmDescription: "An addition to the radius from the center of each cube to check for the boundary of the model, as to decide whether this cube should be subdivided. Larger values lead to a thicker shell of small cubes near the boundary of the model."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.4
                fdmValue: Number(wall_line_width_x.fdmValue)
                fdmMinimumValueWarning: -1 * Number(infill_line_distance.fdmValue)
                fdmMaximumValueWarning: 5 * Number(infill_line_distance.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_pattern.fdmValue === "cubicsubdiv"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_line_width_x,infill_extruder_nr,infill_line_distance"
            }
            FdmQml_Param{ id:infill_overlap; objectName: "infill_overlap"
                fdmLabel: "Infill Overlap Percentage"
                fdmDescription: "The amount of overlap between the infill and the walls. It's displayed as a percentage of the infill line width. A slight overlap allows the walls to connect firmly to the infill."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 10.0
                fdmValue: (Number(infill_sparse_density.fdmValue) < 95 && infill_pattern.fdmValue !== "concentric") ? 10 : 0
                fdmMinimumValueWarning: -50.0
                fdmMaximumValueWarning: 100.0
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_pattern.fdmValue !== "concentric"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_sparse_density,infill_pattern"
                FdmQml_Param{ id:infill_overlap_mm; objectName: "infill_overlap_mm"
                    fdmLabel: "Infill Overlap"
                    fdmDescription: "The amount of overlap between the infill and the walls. A slight overlap allows the walls to connect firmly to the infill."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.04
                    fdmMinimumValueWarning: -0.5 * Number(machine_nozzle_size.fdmValue)
                    fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                    fdmValue: (Number(infill_sparse_density.fdmValue) < 95 && infill_pattern.fdmValue !== "concentric") ? (0.5 * (Number(infill_line_width.fdmValue) + (Number(wall_line_count.fdmValue) > 1 ? Number(wall_line_width_x.fdmValue) : Number(wall_line_width_0.fdmValue))) * Number(infill_overlap.fdmValue) / 100 ) : 0
                    fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_pattern.fdmValue !== "concentric"
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,wall_line_width_0,wall_line_width_x,infill_line_width,wall_line_count,infill_sparse_density,infill_pattern,infill_overlap"
                }
            }
            FdmQml_Param{ id:infill_wipe_dist; objectName: "infill_wipe_dist"
                fdmLabel: "Infill Wipe Distance"
                fdmDescription: "Distance of a travel move inserted after every infill line, to make the infill stick to the walls better. This option is similar to infill overlap, but without extrusion and only on one end of the infill line."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.04
                fdmValue: Number(wall_line_count.fdmValue) === 1 ? (Number(wall_line_width_0.fdmValue) / 4) : (Number(wall_line_width_x.fdmValue) / 4)
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,wall_line_width_0,wall_line_width_x,wall_line_count,infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_sparse_thickness; objectName: "infill_sparse_thickness"
                fdmLabel: "Infill Layer Thickness"
                fdmDescription: "The thickness per layer of infill material. This value should always be a multiple of the layer height and is otherwise rounded."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.1
                fdmMinimumValue: Number(infill_line_distance.fdmValue) > 0 ? (Number(layer_height.fdmValue) / 2) : -999999
                fdmMaximumValueWarning: 0.75 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValue: Number(infill_line_distance.fdmValue) > 0 ? (Number(layer_height.fdmValue) * 8) : 999999
                fdmValue: Number(layer_height.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,layer_height,infill_extruder_nr,infill_line_distance"
            }
            FdmQml_Param{ id:gradual_infill_steps; objectName: "gradual_infill_steps"
                fdmLabel: "Gradual Infill Steps"
                fdmDescription: "Number of times to reduce the infill density by half when getting further below top surfaces. Areas which are closer to top surfaces get a higher density, up to the Infill Density."
                fdmDefaultValue: 0
                fdmType: "int"
                fdmMinimumValue: 0
                fdmMaximumValueWarning: (infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d" || support_pattern.fdmValue === "concentric") ? 1 : 5
                fdmMaximumValue: Number(infill_line_distance.fdmValue) === 0 ? 999999 : (20 - Math.log(Number(infill_line_distance.fdmValue)) / Math.log(2))
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_pattern.fdmValue !== "cubicsubdiv"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_line_distance,infill_pattern,support_pattern"
            }
            FdmQml_Param{ id:gradual_infill_step_height; objectName: "gradual_infill_step_height"
                fdmLabel: "Gradual Infill Step Height"
                fdmDescription: "The infill height with a given density before switching to half the density."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.5
                fdmMinimumValue: 0.0001
                fdmMinimumValueWarning: 3 * Number(layer_height.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && Number(gradual_infill_steps.fdmValue) > 0 && infill_pattern.fdmValue !== "cubicsubdiv"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "layer_height,infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_before_walls; objectName: "infill_before_walls"
                fdmLabel: "Infill Before Walls"
                fdmDescription: "Print the infill before printing the walls. Printing the walls first may lead to more accurate walls, but overhangs print worse. Printing the infill first leads to sturdier walls, but the infill pattern might sometimes show through the surface."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && Number(wall_x_extruder_nr.fdmValue) === Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:min_infill_area; objectName: "min_infill_area"
                fdmLabel: "Minimum Infill Area"
                fdmDescription: "Don't generate areas of infill smaller than this (use skin instead)."
                fdmUnit: "mm²"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmDefaultValue: 0.0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_support_enabled; objectName: "infill_support_enabled"
                fdmLabel: "Infill Support"
                fdmDescription: "Print infill structures only where tops of the model should be supported. Enabling this reduces print time and material usage, but leads to non-uniform object strength."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:infill_support_angle; objectName: "infill_support_angle"
                fdmLabel: "Infill Overhang Angle"
                fdmDescription: "The minimum angle of internal overhangs for which infill is added. At a value of 0°, objects are totally filled with infill, 90° will not provide any infill."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 2.0
                fdmMaximumValue: 90.0
                fdmDefaultValue: 40.0
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0 && infill_support_enabled.fdmValue
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:skin_edge_support_thickness; objectName: "skin_edge_support_thickness"
                fdmLabel: "Skin Edge Support Thickness"
                fdmDescription: "The thickness of the extra infill that supports skin edges."
                fdmUnit: "mm"
                fdmDefaultValue: 0.8
                fdmMinimumValue: 0.0
                fdmMaximumValue: Number(machine_height.fdmValue)
                fdmMaximumValueWarning: Number(infill_sparse_thickness.fdmValue) * 10
                fdmType: "float"
                fdmValue: Number(infill_sparse_density.fdmValue) > 0 ? 0 : 0
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_height,infill_extruder_nr,infill_sparse_density,infill_sparse_thickness"
                FdmQml_Param{ id:skin_edge_support_layers; objectName: "skin_edge_support_layers"
                    fdmLabel: "Skin Edge Support Layers"
                    fdmDescription: "The number of infill layers that supports skin edges."
                    fdmDefaultValue: 4
                    fdmMinimumValue: 0
                    fdmMaximumValueWarning: 10
                    fdmType: "int"
                    fdmValue:  Math.ceil (Math.round(Number(skin_edge_support_thickness.fdmValue) / Number(infill_sparse_thickness.fdmValue), 4))
                    fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                    fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                    fdmSettablePerMesh: true
                    fdmAffectedById: "infill_extruder_nr,infill_sparse_thickness,skin_edge_support_thickness"
                }
            }
        }
        FdmQml_Category{ id:material; objectName: "material"
            fdmLabel: "Material"
            fdmIcon: "Spool"
            fdmDescription: "Material"
            fdmType: "category"
            FdmQml_Param{ id:material_diameter; objectName: "material_diameter"
                fdmLabel: "Diameter"
                fdmDescription: "Adjusts the diameter of the filament used. Match this value with the diameter of the used filament."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.75
                fdmMinimumValue: 0.0001
                fdmMinimumValueWarning: 0.4
                fdmMaximumValueWarning: 3.5
                fdmEnabled: machine_gcode_flavor.fdmValue !== "UltiGCode"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_density; objectName: "material_density"
                fdmLabel: "Density"
                fdmDescription: "Adjusts the density of the filament used. Match this value with the density of the used filament."
                fdmUnit: "g/cm³"
                fdmType: "float"
                fdmDefaultValue: 1.24
                fdmMinimumValue: 0.001
                fdmEnabled: Number(material_diameter.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
                FdmQml_Param{ id:material_linear_density; objectName: "material_linear_density"
                    fdmLabel: "Linear Density"
                    fdmDescription: "Adjusts the linear density of the filament used. Match this value with your filament's linear density."
                    fdmUnit: "g/m"
                    fdmType: "float"
                    fdmDefaultValue: 2.986
                    fdmValue: Number(material_density.fdmValue) * (Number(material_diameter.fdmValue) * Number(material_diameter.fdmValue) * 0.785398163375)
                    fdmMinimumValue: 0.001
                    fdmEnabled: Number(material_diameter.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_diameter,material_density"
                }
            }
            FdmQml_Param{ id:default_material_print_temperature; objectName: "default_material_print_temperature"
                fdmLabel: "Default Printing Temperature"
                fdmDescription: "The default temperature used for printing. This should be the \"base\" temperature of a material. All other print temperatures should use offsets based on this value"
                fdmUnit: "°C"
                fdmType: "float"
                fdmDefaultValue: 225.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 285.0
                fdmMaximumValue: 365.0
                fdmEnabled: false
                fdmSettablePerExtruder: true
                fdmSettablePerMesh: false
                fdmMinimumValue: 0.0
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_print_temperature; objectName: "material_print_temperature"
                fdmLabel: "Printing Temperature"
                fdmDescription: "The temperature used for printing."
                fdmUnit: "°C"
                fdmType: "float"
                fdmDefaultValue: 225.0
                fdmValue: Number(default_material_print_temperature.fdmValue)
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 285.0
                fdmMaximumValue: 365.0
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue && !(material_flow_dependent_temperature.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "default_material_print_temperature"
                FdmQml_Param{ id:before_homing_temperature; objectName: "before_homing_temperature"
                    fdmLabel: "Pre-Homing Temperature"
                    fdmDescription: "Set the temperature of the nozzle before homing."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 225.0
                    fdmValue: Number(material_print_temperature_layer_0.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 0.0
                    fdmMaximumValueWarning: 285.0
                    fdmMaximumValue: 365.0
                    fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_print_temperature_layer_0"
                }
                FdmQml_Param{ id:material_print_temperature_layer_0; objectName: "material_print_temperature_layer_0"
                    fdmLabel: "Printing Temperature Initial Layer"
                    fdmDescription: "The temperature used for printing the first layer. Set at 0 to disable special handling of the initial layer."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 225.0
                    fdmValue: Math.max(230, Number(material_print_temperature.fdmValue) + 10)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 0.0
                    fdmMaximumValueWarning: 285.0
                    fdmMaximumValue: 365.0
                    fdmEnabled: machine_nozzle_temp_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:material_initial_print_temperature; objectName: "material_initial_print_temperature"
                    fdmLabel: "Initial Printing Temperature"
                    fdmDescription: "The minimal temperature while heating up to the Printing Temperature at which printing can already start."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 225.0
                    fdmValue: Math.max(0.0, Number(material_print_temperature.fdmValue) - 10)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(material_standby_temperature.fdmValue)
                    fdmMaximumValueWarning: Number(material_print_temperature.fdmValue)
                    fdmMaximumValue: 365.0
                    fdmEnabled: machine_nozzle_temp_enabled.fdmValue && !machine_extruders_share_heater.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_print_temperature,material_standby_temperature"
                }
                FdmQml_Param{ id:material_final_print_temperature; objectName: "material_final_print_temperature"
                    fdmLabel: "Final Printing Temperature"
                    fdmDescription: "The temperature where the print can begin cooling down before printing stops."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 195.0
                    fdmValue: Math.max(0.0, Number(material_print_temperature.fdmValue) - 15)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(material_standby_temperature.fdmValue)
                    fdmMaximumValueWarning: Number(material_print_temperature.fdmValue)
                    fdmMaximumValue: 365.0
                    fdmEnabled: machine_nozzle_temp_enabled.fdmValue && !machine_extruders_share_heater.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_print_temperature,material_standby_temperature"
                }
                FdmQml_Param{ id:default_material_bed_temperature; objectName: "default_material_bed_temperature"
                    fdmLabel: "Default Build Plate Temperature"
                    fdmDescription: "The default temperature used for the heated build plate. This should be the \"base\" temperature of a build plate. All other print temperatures should use offsets based on this value"
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmResolve: Number(default_material_bed_temperature.fdmValue)
                    fdmDefaultValue: 60.0
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(build_volume_temperature.fdmValue)
                    fdmMaximumValueWarning: 130.0
                    fdmMaximumValue: 200.0
                    fdmEnabled: false
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "build_volume_temperature"
                }
                FdmQml_Param{ id:material_bed_temperature; objectName: "material_bed_temperature"
                    fdmLabel: "Build Plate Temperature"
                    fdmDescription: "The temperature used for the heated build plate. If this is 0, the build plate is left unheated."
                    fdmUnit: "°C"
                    fdmType: "float"
                    fdmDefaultValue: 60.0
                    fdmValue: Number(default_material_bed_temperature.fdmValue)
                    //fdmResolve: Number(material_bed_temperature.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(build_volume_temperature.fdmValue)
                    fdmMaximumValueWarning: 130.0
                    fdmMaximumValue: 200.0
                    fdmEnabled: machine_heated_bed.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "build_volume_temperature,default_material_bed_temperature"
                }
                FdmQml_Param{ id:material_bed_temperature_layer_0; objectName: "material_bed_temperature_layer_0"
                    fdmLabel: "Build Plate Temperature Initial Layer"
                    fdmDescription: "The heated build plate temperature used for the first layer. If this is 0, the build plate is left unheated during the first layer."
                    fdmUnit: "°C"
                    fdmType: "float"
                    //fdmResolve: Number(material_bed_temperature_layer_0.fdmValue)
                    fdmDefaultValue: 60.0
                    fdmValue: Number(material_bed_temperature.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Math.max(Number(build_volume_temperature.fdmValue), Number(material_bed_temperature.fdmValue))
                    fdmMaximumValueWarning: 130.0
                    fdmMaximumValue: 200.0
                    fdmEnabled: machine_heated_bed.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "build_volume_temperature,material_bed_temperature"
                }
            }
            FdmQml_Param{ id:material_extrusion_cool_down_speed; objectName: "material_extrusion_cool_down_speed"
                fdmLabel: "Extrusion Cool Down Speed Modifier"
                fdmDescription: "The extra speed by which the nozzle cools while extruding. The same value is used to signify the heat up speed lost when heating up while extruding."
                fdmUnit: "°C/s"
                fdmType: "float"
                fdmDefaultValue: 0.7
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 10.0
                fdmMaximumValue: Number(machine_nozzle_heat_up_speed.fdmValue)
                fdmEnabled: material_flow_dependent_temperature.fdmValue || (Number(extruders_enabled_count.fdmValue) > 1 && Number(material_final_print_temperature.fdmValue) !== Number(material_print_temperature.fdmValue))
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_nozzle_heat_up_speed"
            }
            FdmQml_Param{ id:material_adhesion_tendency; objectName: "material_adhesion_tendency"
                fdmLabel: "Adhesion Tendency"
                fdmDescription: "Surface adhesion tendency."
                fdmType: "int"
                fdmDefaultValue: 10
                fdmMinimumValue: 0
                fdmMaximumValue: 10
                fdmEnabled: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_surface_energy; objectName: "material_surface_energy"
                fdmLabel: "Surface Energy"
                fdmDescription: "Surface energy."
                fdmUnit: "%"
                fdmType: "int"
                fdmDefaultValue: 100
                fdmMinimumValue: 0
                fdmMaximumValue: 100
                fdmEnabled: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_shrinkage_percentage; objectName: "material_shrinkage_percentage"
                fdmLabel: "Scaling Factor Shrinkage Compensation"
                fdmDescription: "To compensate for material shrinkage as it cools down, the model will be scaled with this factor."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 100.0
                fdmEnabled: false
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 100.0
                fdmMaximumValueWarning: 120.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                //fdmResolve: "sum(extruderValues(\"material_shrinkage_percentage\")) / len(extruderValues(\"material_shrinkage_percentage\"))"
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_crystallinity; objectName: "material_crystallinity"
                fdmLabel: "Crystalline Material"
                fdmDescription: "Toggle this setting if you're using crystaline material, which breaks off cleanly when heated."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_anti_ooze_retracted_position; objectName: "material_anti_ooze_retracted_position"
                fdmLabel: "Anti-Ooze Retracted Position"
                fdmDescription: "How far the material needs to be retracted before it stops oozing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: -4.0
                fdmEnabled: false
                fdmMinimumValueWarning: -Number(switch_extruder_retraction_amount.fdmValue)
                fdmMaximumValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "switch_extruder_retraction_amount"
            }
            FdmQml_Param{ id:material_anti_ooze_retraction_speed; objectName: "material_anti_ooze_retraction_speed"
                fdmLabel: "Anti-Ooze Retraction Speed"
                fdmDescription: "How fast the material needs to be retracted during a filament switch to prevent oozing."
                fdmType: "float"
                fdmUnit: "mm/s"
                fdmDefaultValue: 5.0
                fdmEnabled: false
                fdmMinimumValue: 0.0
                fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e"
            }
            FdmQml_Param{ id:material_break_preparation_retracted_position; objectName: "material_break_preparation_retracted_position"
                fdmLabel: "Break Preparation Retracted Position"
                fdmDescription: "How far heated filament can be stretched before it breaks."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: -16.0
                fdmEnabled: false
                fdmMinimumValueWarning: -Number(retraction_amount.fdmValue) * 4
                fdmMaximumValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "retraction_amount"
            }
            FdmQml_Param{ id:material_break_preparation_speed; objectName: "material_break_preparation_speed"
                fdmLabel: "Break Preparation Retraction Speed"
                fdmDescription: "How fast the filament needs to be retracted before breaking it off in a retraction."
                fdmType: "float"
                fdmUnit: "mm/s"
                fdmDefaultValue: 2.0
                fdmEnabled: false
                fdmMinimumValue: 0.0
                fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e"
            }
            FdmQml_Param{ id:material_break_preparation_temperature; objectName: "material_break_preparation_temperature"
                fdmLabel: "Break Preparation Temperature"
                fdmDescription: "The temperature used to purge material. It should be roughly equal to the highest possible printing temperature."
                fdmType: "float"
                fdmUnit: "°C"
                fdmDefaultValue: 50.0
                fdmValue: Number(material_print_temperature.fdmValue)
                fdmEnabled: false
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 300.0
                fdmMaximumValue: 365.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "material_print_temperature"
            }
            FdmQml_Param{ id:material_break_retracted_position; objectName: "material_break_retracted_position"
                fdmLabel: "Break Retracted Position"
                fdmDescription: "How far the filament retracts before it breaks cleanly."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: -50.0
                fdmEnabled: false
                fdmMinimumValueWarning: -100.0
                fdmMaximumValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_break_speed; objectName: "material_break_speed"
                fdmLabel: "Break Retraction Speed"
                fdmDescription: "The retraction speed for clean filament breaks."
                fdmType: "float"
                fdmUnit: "mm/s"
                fdmDefaultValue: 25.0
                fdmEnabled: false
                fdmMinimumValue: 0.0
                fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e"
            }
            FdmQml_Param{ id:material_break_temperature; objectName: "material_break_temperature"
                fdmLabel: "Break Temperature"
                fdmDescription: "The temperature for clean filament breaks."
                fdmType: "float"
                fdmUnit: "°C"
                fdmDefaultValue: 50.0
                fdmEnabled: false
                fdmMinimumValue: 0.0
                fdmMaximumValue: 365.0
                fdmMaximumValueWarning: 300.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_flush_purge_speed; objectName: "material_flush_purge_speed"
                fdmLabel: "Flush Purge Speed"
                fdmDescription: "How fast the material primes after switching material."
                fdmType: "float"
                fdmDefaultValue: 0.5
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_flush_purge_length; objectName: "material_flush_purge_length"
                fdmLabel: "Flush Purge Length"
                fdmDescription: "How much material is used to purge the previous material out of the nozzle to switch to another material. Input by filament length."
                fdmType: "float"
                fdmDefaultValue: 60.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_end_of_filament_purge_speed; objectName: "material_end_of_filament_purge_speed"
                fdmLabel: "End of Filament Purge Speed"
                fdmDescription: "How fast the material primes after replacing an empty spool with a fresh spool of the same material."
                fdmType: "float"
                fdmDefaultValue: 0.5
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_end_of_filament_purge_length; objectName: "material_end_of_filament_purge_length"
                fdmLabel: "End of Filament Purge Length"
                fdmDescription: "How much material is used to purge the previous material out of the nozzle when replacing an empty spool with a fresh spool of the same material. Input in lengths of filament."
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_maximum_park_duration; objectName: "material_maximum_park_duration"
                fdmLabel: "Maximum Park Duration"
                fdmDescription: "How long the material can be kept out of dry storage safely."
                fdmType: "float"
                fdmDefaultValue: 300.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_no_load_move_factor; objectName: "material_no_load_move_factor"
                fdmLabel: "No Load Move Factor"
                fdmDescription: "Indicates how much the filament gets compressed between the feeder and the nozzle chamber. Determines how far to move the material for a filament switch."
                fdmType: "float"
                fdmDefaultValue: 0.940860215
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_flow; objectName: "material_flow"
                fdmLabel: "Flow"
                fdmDescription: "The amount of material extruded is multipled by the folow compensation value.Flow compensation: the amount of material extruded is multiplied by this value."
                fdmUnit: "%"
                fdmDefaultValue: 100.0
                fdmType: "float"
                fdmMinimumValue: 5.0
                fdmMinimumValueWarning: 50.0
                fdmMaximumValueWarning: 150.0
                fdmEnabled: machine_gcode_flavor.fdmValue !== "UltiGCode"
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:wall_material_flow; objectName: "wall_material_flow"
                    fdmLabel: "Wall Flow"
                    fdmDescription: "Flow compensation on wall lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue) === Number(wall_0_extruder_nr.fdmValue) ? Number(wall_0_extruder_nr.fdmValue) : -1
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_0_extruder_nr,wall_x_extruder_nr,material_flow"
                    FdmQml_Param{ id:wall_0_material_flow; objectName: "wall_0_material_flow"
                        fdmLabel: "Outer Wall Flow"
                        fdmDescription: "Flow compensation on the outermost wall line."
                        fdmUnit: "%"
                        fdmType: "float"
                        fdmDefaultValue: 100.0
                        fdmValue: Number(wall_material_flow.fdmValue)
                        fdmMinimumValue: 5.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "wall_0_extruder_nr,wall_material_flow"
                    }
                    FdmQml_Param{ id:wall_x_material_flow; objectName: "wall_x_material_flow"
                        fdmLabel: "Inner Wall(s) Flow"
                        fdmDescription: "Flow compensation on wall lines except the outermost one."
                        fdmUnit: "%"
                        fdmType: "float"
                        fdmDefaultValue: 100.0
                        fdmValue: Number(wall_material_flow.fdmValue)
                        fdmMinimumValue: 5.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "wall_x_extruder_nr,wall_material_flow"
                    }
                }
                FdmQml_Param{ id:skin_material_flow; objectName: "skin_material_flow"
                    fdmLabel: "Top / Bottom Flow"
                    fdmDescription: "Flow compensation on top / bottom lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "top_bottom_extruder_nr,material_flow"
                }
                FdmQml_Param{ id:roofing_material_flow; objectName: "roofing_material_flow"
                    fdmLabel: "Top Surface Skin Flow"
                    fdmDescription: "Flow compensation on lines for areas on top of the print."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(skin_material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmEnabled: Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                    fdmAffectedById: "roofing_extruder_nr,skin_material_flow"
                }
                FdmQml_Param{ id:infill_material_flow; objectName: "infill_material_flow"
                    fdmLabel: "Infill Flow"
                    fdmDescription: "Flow compensation on infill lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                    fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "infill_extruder_nr,material_flow"
                }
                FdmQml_Param{ id:skirt_brim_material_flow; objectName: "skirt_brim_material_flow"
                    fdmLabel: "Skirt / Brim Flow"
                    fdmDescription: "Flow compensation on skirt or brim lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || draft_shield_enabled.fdmValue || ooze_shield_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_flow"
                }
                FdmQml_Param{ id:support_material_flow; objectName: "support_material_flow"
                    fdmLabel: "Support Flow"
                    fdmDescription: "Flow compensation on support structure lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_flow,support_infill_extruder_nr"
                }
                FdmQml_Param{ id:support_interface_material_flow; objectName: "support_interface_material_flow"
                    fdmLabel: "Support Interface Flow"
                    fdmDescription: "Flow compensation on support roof or floor lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_interface_enable.fdmValue
                    fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_flow,support_interface_extruder_nr"
                    FdmQml_Param{ id:support_roof_material_flow; objectName: "support_roof_material_flow"
                        fdmLabel: "Support Roof Flow"
                        fdmDescription: "Flow compensation on support roof lines."
                        fdmUnit: "%"
                        fdmType: "float"
                        fdmDefaultValue: 100.0
                        fdmValue: Number(support_interface_material_flow.fdmValue)
                        fdmMinimumValue: 5.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_roof_enable.fdmValue
                        fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "support_interface_material_flow,support_roof_extruder_nr"
                    }
                    FdmQml_Param{ id:support_bottom_material_flow; objectName: "support_bottom_material_flow"
                        fdmLabel: "Support Floor Flow"
                        fdmDescription: "Flow compensation on support floor lines."
                        fdmUnit: "%"
                        fdmType: "float"
                        fdmDefaultValue: 100.0
                        fdmValue: Number(support_interface_material_flow.fdmValue)
                        fdmMinimumValue: 5.0
                        fdmMinimumValueWarning: 50.0
                        fdmMaximumValueWarning: 150.0
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_bottom_enable.fdmValue
                        fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "support_interface_material_flow,support_bottom_extruder_nr"
                    }
                }
                FdmQml_Param{ id:prime_tower_flow; objectName: "prime_tower_flow"
                    fdmLabel: "Prime Tower Flow"
                    fdmDescription: "Flow compensation on prime tower lines."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmValue: Number(material_flow.fdmValue)
                    fdmMinimumValue: 5.0
                    fdmMinimumValueWarning: 50.0
                    fdmMaximumValueWarning: 150.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "material_flow"
                }
            }
            FdmQml_Param{ id:material_flow_layer_0; objectName: "material_flow_layer_0"
                fdmLabel: "Initial Layer Flow"
                fdmDescription: "Flow compensation for the first layer. The amount of material extruded on the initial layer is multiplied by this value."
                fdmUnit: "%"
                fdmDefaultValue: 150.0
                fdmType: "float"
                fdmMinimumValue: 0.0001
                fdmMinimumValueWarning: 50.0
                fdmMaximumValueWarning: 150.0
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_standby_temperature; objectName: "material_standby_temperature"
                fdmLabel: "Standby Temperature"
                fdmDescription: "The temperature of the nozzle when another nozzle is currently used for printing."
                fdmType: "float"
                fdmUnit: "°C"
                fdmDefaultValue: 150.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 260.0
                fdmMaximumValue: 365.0
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1 && machine_nozzle_temp_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:speed; objectName: "speed"
            fdmLabel: "Speed"
            fdmIcon: "SpeedOMeter"
            fdmDescription: "Set the speed of various actions."
            fdmType: "category"
            fdmMinimumValue: 0.1
            fdmMaximumValue: Math.sqrt(machine_max_feedrate_x.fdmValue ** 2 + machine_max_feedrate_y.fdmValue ** 2)
            fdmMaximumValueWarning: 1200.0 // 20x
            FdmQml_Param{ id:speed_travel; objectName: "speed_travel"
                fdmLabel: "Travel Speed"
                fdmDescription: "The travel move speed."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 250.0      // 5x
                fdmMinimumValue: speed.fdmMinimumValue
                fdmMaximumValue: speed.fdmMaximumValue
                fdmMaximumValueWarning: speed.fdmMaximumValueWarning
                //fdmValue: speed_print.fdmValue // magic_spiralize.fdmValue ? speed_print.fdmValue : 120.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:speed_print; objectName: "speed_print"
                fdmLabel: "Print Speed"
                fdmDescription: "The printing speed."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmMinimumValue: speed.fdmMinimumValue
                fdmMaximumValueWarning: speed.fdmMaximumValueWarning
                fdmMaximumValue: speed.fdmMaximumValue
                fdmDefaultValue: 250.0      // 5x
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:speed_infill; objectName: "speed_infill"
                    fdmLabel: "Infill Speed"
                    fdmDescription: "The infill printing speed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmDefaultValue: 250.0      // 5x
                    fdmValue: Number(speed_print.fdmValue)
                    fdmEnabled: Number(infill_sparse_density.fdmValue) > 0
                    fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "infill_extruder_nr,speed_print"
                }
                FdmQml_Param{ id:speed_wall; objectName: "speed_wall"
                    fdmLabel: "Wall Speed"
                    fdmDescription: "The wall printing speed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmDefaultValue: 150.0
                    fdmValue: speed_print.fdmValue * 3.0 / 5.0
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:speed_wall_0; objectName: "speed_wall_0"
                        fdmLabel: "Outer Wall Speed"
                        fdmDescription: "The print speed for the outermost walls. Printing at a lower speed improves the final skin quality for outer walls. However, a large difference between the inner wall speed and outer wall speed will negatively affect quality."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: speed_print.fdmMinimumValue
                        fdmMaximumValue: speed_print.fdmMaximumValue
                        fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                        fdmDefaultValue: 100.0
                        fdmValue: Number(speed_wall.fdmValue)
                        fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "wall_0_extruder_nr,speed_wall"
                    }
                    FdmQml_Param{ id:speed_wall_x; objectName: "speed_wall_x"
                        fdmLabel: "Inner Wall Speed"
                        fdmDescription: "The print speed for inner walls. Printing inner walls faster than outer walls will reduce printing time. For best results, set this between the outer wall speed and infill speed."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: speed_print.fdmMinimumValue
                        fdmMaximumValue: speed_print.fdmMaximumValue
                        fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                        fdmDefaultValue: 150.0
                        fdmValue: Math.min(Number(speed_wall.fdmValue) * 5/3, 250) //Math.min(Number(speed_wall.fdmValue) * 2, 250)
                        fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "wall_x_extruder_nr"
                    }
                }
                FdmQml_Param{ id:speed_topbottom; objectName: "speed_topbottom"
                    fdmLabel: "Top / Bottom Speed"
                    fdmDescription: "The speed at which the top and bottom layers are printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmDefaultValue: 150.0
                    fdmValue: Number(speed_print.fdmValue) / 2
                    fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                    fdmEnabled: Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0
                    fdmSettablePerMesh: true
                    fdmAffectedById: "top_bottom_extruder_nr,speed_print"
                    FdmQml_Param{ id:speed_roofing; objectName: "speed_roofing"
                        fdmLabel: "Top Surface Skin Speed"
                        fdmDescription: "The speed at which top surface skin layers are printed."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: speed_print.fdmMinimumValue
                        fdmMaximumValue: speed_print.fdmMaximumValue
                        fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                        fdmDefaultValue: 150.0
                        fdmValue: Number(speed_topbottom.fdmValue)
                        fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmEnabled: Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                        fdmAffectedById: "roofing_extruder_nr,speed_topbottom"
                    }
                }
                FdmQml_Param{ id:speed_support; objectName: "speed_support"
                    fdmLabel: "Support Speed"
                    fdmDescription: "The speed at which the support structure is printed. Printing supports at higher speeds greatly reduces printing time."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmDefaultValue: 250.0
                    fdmValue: Number(speed_print.fdmValue)
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(support_extruder_nr.fdmValue)
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "speed_print,support_extruder_nr"
                    FdmQml_Param{ id:speed_support_infill; objectName: "speed_support_infill"
                        fdmLabel: "Support Infill Speed"
                        fdmDescription: "The speed at which the infill support is printed. Printing the infill at lower speeds improves stability."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmDefaultValue: 250.0
                        fdmMinimumValue: speed_print.fdmMinimumValue
                        fdmMaximumValue: speed_print.fdmMaximumValue
                        fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                        fdmValue: Number(speed_support.fdmValue)
                        fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                        fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "speed_support,support_infill_extruder_nr"
                    }
                    FdmQml_Param{ id:speed_support_interface; objectName: "speed_support_interface"
                        fdmLabel: "Support Interface Speed"
                        fdmDescription: "The speed at which support roofs and floors are printed. Printing at lower speeds can improve overhang quality."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmDefaultValue: 150.0
                        fdmMinimumValue: speed_print.fdmMinimumValue
                        fdmMaximumValue: speed_print.fdmMaximumValue
                        fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                        fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                        fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                        fdmValue: Number(speed_support.fdmValue) / 1.5
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "speed_support,support_interface_extruder_nr"
                        FdmQml_Param{ id:speed_support_roof; objectName: "speed_support_roof"
                            fdmLabel: "Support Roof Speed"
                            fdmDescription: "The speed at which support roofs are printed. Printing them at lower speeds can improve overhang quality."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmDefaultValue: 150.0
                            fdmMinimumValue: speed_print.fdmMinimumValue
                            fdmMaximumValue: speed_print.fdmMaximumValue
                            fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                            fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                            fdmValue: Number(speed_support_interface.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "speed_support_interface,support_roof_extruder_nr"
                        }
                        FdmQml_Param{ id:speed_support_bottom; objectName: "speed_support_bottom"
                            fdmLabel: "Support Floor Speed"
                            fdmDescription: "The speed at which support floors are printed. Printing support floors at lower speed can improve support adhesion on top of your model."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmDefaultValue: 150.0
                            fdmMinimumValue: speed_print.fdmMinimumValue
                            fdmMaximumValue: speed_print.fdmMaximumValue
                            fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                            fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                            fdmValue: Number(speed_support_interface.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "speed_support_interface,support_bottom_extruder_nr"
                        }
                    }
                }
                FdmQml_Param{ id:speed_prime_tower; objectName: "speed_prime_tower"
                    fdmLabel: "Prime Tower Speed"
                    fdmDescription: "The speed at which the prime tower is printed. Printing the prime tower slower can make it more stable when the adhesion between  different filaments is suboptimal."
                    fdmType: "float"
                    fdmUnit: "mm/s"
                    fdmEnabled: prime_tower_enable.fdmValue
                    fdmDefaultValue: 250.0
                    fdmValue: Number(speed_print.fdmValue)
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "speed_print"
                }
            }
            FdmQml_Param{ id:speed_layer_0; objectName: "speed_layer_0"
                fdmLabel: "Initial Layer Speed"
                fdmDescription: "The initial layer speed. For improved adhesion, use a lower value. This setting does not affect build plate adehsion structures, such as brims and rafts."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmEnabled: Number(speed_slowdown_layers.fdmValue) > 0
                fdmDefaultValue: 100.0
                fdmValue: speed_print.fdmValue
                fdmMinimumValue: speed.fdmMinimumValue
                fdmMaximumValue: speed.fdmMaximumValue
                fdmMaximumValueWarning: speed.fdmMaximumValueWarning
                fdmSettablePerMesh: true
                fdmAffectedById: ""
                FdmQml_Param{ id:speed_print_layer_0; objectName: "speed_print_layer_0"
                    fdmLabel: "Initial Layer Print Speed"
                    fdmDescription: "The initial layer print speed. For improved build plate adhesion, use a lower value."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmEnabled: Number(speed_slowdown_layers.fdmValue) > 0
                    fdmDefaultValue: 100.0
                    fdmValue: Number(speed_layer_0.fdmValue)
                    fdmMinimumValue: speed.fdmMinimumValue
                    fdmMaximumValue: speed.fdmMaximumValue
                    fdmMaximumValueWarning: speed.fdmMaximumValueWarning
                    fdmSettablePerMesh: true
                    fdmAffectedById: "speed_layer_0"
                }
                FdmQml_Param{ id:speed_travel_layer_0; objectName: "speed_travel_layer_0"
                    fdmLabel: "Initial Layer Travel Speed"
                    fdmDescription: "The speed of travel moves for the initial layer. To avoid pulling previously printed parts, use a lower value. The value of this setting can automatically be calucated from the ratio between Travel Speed and Print Speed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmEnabled: Number(speed_slowdown_layers.fdmValue) > 0
                    fdmDefaultValue: 150.0
                    fdmValue: Number(speed_layer_0.fdmValue) * Number(speed_travel.fdmValue) / Number(speed_print.fdmValue)
                    fdmMinimumValue: speed.fdmMinimumValue
                    fdmMaximumValue: speed.fdmMaximumValue
                    fdmMaximumValueWarning: speed.fdmMaximumValueWarning
                    fdmSettablePerExtruder: true
                    fdmSettablePerMesh: false
                    fdmAffectedById: "speed_travel,speed_print,speed_layer_0"
                }
                FdmQml_Param{ id:skirt_brim_speed; objectName: "skirt_brim_speed"
                    fdmLabel: "Skirt / Brim Speed"
                    fdmDescription: "The skirt and brim print speed. This is normally done at the initial layer speed, but is customizable if you choose so."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 30.0
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmValue: Number(speed_layer_0.fdmValue)
                    fdmEnabled: adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || draft_shield_enabled.fdmValue || ooze_shield_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "speed_layer_0,adhesion_extruder_nr"
                }
            }
            FdmQml_Param{ id:speed_z_hop; objectName: "speed_z_hop"
                fdmLabel: "Z Hop Speed"
                fdmDescription: "The speed at which vertical Z movement is made. This is typically lower than the print speed since the build plate and gantry is harder to move."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 10.0
                fdmMinimumValue: 0.0
                fdmMaximumValue: Number(machine_max_feedrate_z.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_z"
            }
            FdmQml_Param{ id:speed_slowdown_layers; objectName: "speed_slowdown_layers"
                fdmLabel: "Number of Slower Layers"
                fdmDescription: "The first few layers are printed slower than the rest of the model for better build plate adehsion and improve the overall success rate. The speed is gradually increased over these layers."
                fdmType: "int"
                fdmDefaultValue: 2
                //fdmResolve:  Math.round(Number(speed_slowdown_layers.fdmValue) / len(Number(speed_slowdown_layers.fdmValue)))
                fdmMinimumValue: 0
                fdmMaximumValueWarning: 3.2 / Number(layer_height.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "layer_height"
            }
            FdmQml_Param{ id:speed_equalize_flow_enabled; objectName: "speed_equalize_flow_enabled"
                fdmLabel: "Equalize Filament Flow"
                fdmDescription: "Control the speed of thinner lines. By printing thinner lines faster, you keep a steady flow of extruded material, down to the second."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
                FdmQml_Param{ id:speed_equalize_flow_max; objectName: "speed_equalize_flow_max"
                    fdmLabel: "Maximum Speed for Flow Equalization"
                    fdmDescription: "Maximum print speed when adjusting the print speed in order to equalize flow."
                    fdmType: "float"
                    fdmUnit: "mm/s"
                    fdmEnabled: speed_equalize_flow_enabled.fdmValue
                    fdmDefaultValue: speed_print.fdmValue * 3.0 / 5.0 // 150.0
                    fdmMinimumValue: speed_print.fdmMinimumValue
                    fdmMaximumValue: speed_print.fdmMaximumValue
                    fdmMaximumValueWarning: speed_print.fdmMaximumValueWarning
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
            }
            FdmQml_Param{ id:acceleration_enabled; objectName: "acceleration_enabled"
                fdmLabel: "Enable Acceleration Control"
                fdmDescription: "Allows you to adjust the print head acceleration. Increasing accelerations can reduce printing time but reduces print quality."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmResolve: acceleration_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
                FdmQml_Param{ id:acceleration_travel; objectName: "acceleration_travel"
                    fdmLabel: "Travel Acceleration"
                    fdmDescription: "The acceleration of travel moves."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmDefaultValue: 2500.0
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 99999.0
                    //fdmValue: magic_spiralize.fdmValue ? Number(acceleration_print.fdmValue) : 5000
                    fdmEnabled: acceleration_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmAffectedById: "acceleration_print,magic_spiralize"
                }
                FdmQml_Param{ id:acceleration_print; objectName: "acceleration_print"
                    fdmLabel: "Print Acceleration"
                    fdmDescription: "The acceleration with which printing happens."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 99999.0
                    fdmDefaultValue: 2500.0 // 5x
                    fdmEnabled: acceleration_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:acceleration_infill; objectName: "acceleration_infill"
                        fdmLabel: "Infill Acceleration"
                        fdmDescription: "The acceleration of infill prints."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmDefaultValue: 2500.0 // 5x
                        fdmValue: Number(acceleration_print.fdmValue)
                        fdmEnabled: acceleration_enabled.fdmValue && Number(infill_sparse_density.fdmValue) > 0
                        fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "infill_extruder_nr,acceleration_print"
                    }
                    FdmQml_Param{ id:acceleration_wall; objectName: "acceleration_wall"
                        fdmLabel: "Wall Acceleration"
                        fdmDescription: "The acceleration of wall prints."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmDefaultValue: 1500.0 // 5x  (3/5)?
                        fdmValue: acceleration_print.fdmValue //  * 3.0 / 5.0
                        fdmEnabled: acceleration_enabled.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                        FdmQml_Param{ id:acceleration_wall_0; objectName: "acceleration_wall_0"
                            fdmLabel: "Outer Wall Acceleration"
                            fdmDescription: "The acceleration of the outermost wall prints."
                            fdmUnit: "mm/s²"
                            fdmType: "float"
                            fdmMinimumValue: 0.1
                            fdmMinimumValueWarning: 100.0
                            fdmMaximumValueWarning: 99999.0
                            fdmDefaultValue: 500.0 // 5x
                            fdmValue: acceleration_wall.fdmValue        // (1/3 * 3/5)?
                            fdmEnabled: acceleration_enabled.fdmValue
                            fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "wall_0_extruder_nr"
                        }
                        FdmQml_Param{ id:acceleration_wall_x; objectName: "acceleration_wall_x"
                            fdmLabel: "Inner Wall Acceleration"
                            fdmDescription: "The acceleration of the inner wall prints."
                            fdmUnit: "mm/s²"
                            fdmType: "float"
                            fdmMinimumValue: 0.1
                            fdmMinimumValueWarning: 100.0
                            fdmMaximumValueWarning: 99999.0
                            fdmDefaultValue: 1500.0 // 5x
                            fdmValue: acceleration_wall.fdmValue     // (3/5)?
                            fdmEnabled: acceleration_enabled.fdmValue
                            fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "wall_x_extruder_nr"
                        }
                        FdmQml_Param{ id:acceleration_topbottom; objectName: "acceleration_topbottom"
                            fdmLabel: "Top / Bottom Acceleration"
                            fdmDescription: "The print acceleration for the top and bottom layers."
                            fdmUnit: "mm/s²"
                            fdmType: "float"
                            fdmMinimumValue: 0.1
                            fdmMinimumValueWarning: 100.0
                            fdmMaximumValueWarning: 99999.0
                            fdmDefaultValue: 1500.0 // 5x
                            fdmValue: acceleration_wall.fdmValue
                            fdmEnabled: acceleration_enabled.fdmValue
                            fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "top_bottom_extruder_nr"
                            FdmQml_Param{ id:acceleration_roofing; objectName: "acceleration_roofing"
                                fdmLabel: "Top Surface Skin Acceleration"
                                fdmDescription: "The print acceleration for the top surface skin layers."
                                fdmUnit: "mm/s²"
                                fdmType: "float"
                                fdmMinimumValue: 0.1
                                fdmMinimumValueWarning: 100.0
                                fdmMaximumValueWarning: 99999.0
                                fdmDefaultValue: 1500.0 // 5x
                                fdmValue: acceleration_topbottom.fdmValue   // (2/5)?
                                fdmEnabled: acceleration_enabled.fdmValue && Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                                fdmSettablePerMesh: true
                                fdmAffectedById: "roofing_extruder_nr"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_support; objectName: "acceleration_support"
                        fdmLabel: "Support Acceleration"
                        fdmDescription: "The print acceleration for the support structure."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmDefaultValue: 2500.0 // 5x
                        fdmValue: Number(acceleration_print.fdmValue)
                        fdmEnabled: acceleration_enabled.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                        fdmSettablePerMesh: false
                        fdmLimitToExtruder: Number(support_extruder_nr.fdmValue)
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "acceleration_print,support_extruder_nr"
                        FdmQml_Param{ id:acceleration_support_infill; objectName: "acceleration_support_infill"
                            fdmLabel: "Support Infill Acceleration"
                            fdmDescription: "The print acceleration for the infill support."
                            fdmUnit: "mm/s²"
                            fdmType: "float"
                            fdmDefaultValue: 2500.0 // 5x
                            fdmValue: Number(acceleration_support.fdmValue)
                            fdmMinimumValue: 0.1
                            fdmMinimumValueWarning: 100.0
                            fdmMaximumValueWarning: 99999.0
                            fdmEnabled: acceleration_enabled.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "acceleration_support,support_infill_extruder_nr"
                        }
                        FdmQml_Param{ id:acceleration_support_interface; objectName: "acceleration_support_interface"
                            fdmLabel: "Support Interface Acceleration"
                            fdmDescription: "The print acceleration for support roofs and floors. Printing with lower acceleration can improve overhang quality."
                            fdmUnit: "mm/s²"
                            fdmType: "float"
                            fdmDefaultValue: 3000.0
                            fdmValue: Number(acceleration_support.fdmValue)
                            fdmMinimumValue: 0.1
                            fdmMinimumValueWarning: 100.0
                            fdmMaximumValueWarning: 99999.0
                            fdmEnabled: acceleration_enabled.fdmValue && support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "acceleration_support,support_interface_extruder_nr"
                            FdmQml_Param{ id:acceleration_support_roof; objectName: "acceleration_support_roof"
                                fdmLabel: "Support Roof Acceleration"
                                fdmDescription: "The print acceleration for support roofs. Printing with lower acceleration can improve overhang quality."
                                fdmUnit: "mm/s²"
                                fdmType: "float"
                                fdmDefaultValue: 3000.0
                                fdmValue: Number(acceleration_support_interface.fdmValue)
                                fdmMinimumValue: 0.1
                                fdmMinimumValueWarning: 100.0
                                fdmMaximumValueWarning: 99999.0
                                fdmEnabled: acceleration_enabled.fdmValue && support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                                fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                                fdmSettablePerMesh: false
                                fdmSettablePerExtruder: true
                                fdmAffectedById: "acceleration_support_interface,support_roof_extruder_nr"
                            }
                            FdmQml_Param{ id:acceleration_support_bottom; objectName: "acceleration_support_bottom"
                                fdmLabel: "Support Floor Acceleration"
                                fdmDescription: "The print acceleration for support floors. Printing with lower acceleration can improve overhang quality."
                                fdmUnit: "mm/s²"
                                fdmType: "float"
                                fdmDefaultValue: 3000.0
                                fdmValue: Number(acceleration_support_interface.fdmValue)
                                fdmMinimumValue: 0.1
                                fdmMinimumValueWarning: 100.0
                                fdmMaximumValueWarning: 99999.0
                                fdmEnabled: acceleration_enabled.fdmValue && support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                                fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                                fdmSettablePerMesh: false
                                fdmSettablePerExtruder: true
                                fdmAffectedById: "acceleration_support_interface,support_bottom_extruder_nr"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_prime_tower; objectName: "acceleration_prime_tower"
                        fdmLabel: "Prime Tower Acceleration"
                        fdmDescription: "The print acceleration for the prime tower."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmDefaultValue: 3000.0
                        fdmValue: Number(acceleration_print.fdmValue)
                        fdmEnabled: prime_tower_enable.fdmValue && acceleration_enabled.fdmValue
                        fdmSettablePerMesh: false
                        fdmAffectedById: "acceleration_print"
                    }
                }
                FdmQml_Param{ id:acceleration_layer_0; objectName: "acceleration_layer_0"
                    fdmLabel: "Initial Layer Acceleration"
                    fdmDescription: "The acceleration for the initial layer."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmDefaultValue: 3000.0
                    fdmValue: Number(acceleration_print.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 99999.0
                    fdmEnabled: acceleration_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "acceleration_print"
                    FdmQml_Param{ id:acceleration_travel_layer_0; objectName: "acceleration_travel_layer_0"
                        fdmLabel: "Initial Layer Travel Acceleration"
                        fdmDescription: "The acceleration for travel moves on the initial layer."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmDefaultValue: 3000.0
                        fdmValue: Number(acceleration_layer_0.fdmValue) * Number(acceleration_travel.fdmValue) / Number(acceleration_print.fdmValue)
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmEnabled: acceleration_enabled.fdmValue
                        fdmSettablePerExtruder: true
                        fdmSettablePerMesh: false
                        fdmAffectedById: "acceleration_travel,acceleration_print,acceleration_layer_0"
                    }
                    FdmQml_Param{ id:acceleration_print_layer_0; objectName: "acceleration_print_layer_0"
                        fdmLabel: "Initial Layer Print Acceleration"
                        fdmDescription: "The acceleration during initial layer printing."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmDefaultValue: 3000.0
                        fdmValue: Number(acceleration_layer_0.fdmValue)
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmEnabled: acceleration_enabled.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: "acceleration_layer_0"
                    }
                    FdmQml_Param{ id:acceleration_skirt_brim; objectName: "acceleration_skirt_brim"
                        fdmLabel: "Skirt / Brim Acceleration"
                        fdmDescription: "The print acceleration for the skirt and brim. Normally, this is done with the initial layer acceleration, but you can change it here if you want them printed at different accelerations."
                        fdmUnit: "mm/s²"
                        fdmType: "float"
                        fdmDefaultValue: 3000.0
                        fdmValue: Number(acceleration_layer_0.fdmValue)
                        fdmMinimumValue: 0.1
                        fdmMinimumValueWarning: 100.0
                        fdmMaximumValueWarning: 99999.0
                        fdmEnabled: acceleration_enabled.fdmValue && (adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || draft_shield_enabled.fdmValue || ooze_shield_enabled.fdmValue)
                        fdmSettablePerMesh: false
                        fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                        fdmAffectedById: "acceleration_layer_0,adhesion_extruder_nr"
                    }
                }
            }
            FdmQml_Param{ id:jerk_enabled; objectName: "jerk_enabled"
                fdmLabel: "Enable Jerk Control"
                fdmDescription: "Adjusts the print head jerk when the speed changes on the X or Y axis. Increasing the jerk can reduce print time but decreases quality."
                fdmType: "bool"
                fdmDefaultValue: true
                //fdmResolve: jerk_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
                FdmQml_Param{ id:jerk_travel; objectName: "jerk_travel"
                    fdmLabel: "Travel Jerk"
                    fdmDescription: "The maximum instant speed change for travel moves."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 25.0   //30.0
                    fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                    fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                    //fdmValue: jerk_print.fdmValue // magic_spiralize.fdmValue ? jerk_print.fdmValue : 30.0
                    fdmEnabled: jerk_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:jerk_print; objectName: "jerk_print"
                    fdmLabel: "Print Jerk"
                    fdmDescription: "The maximum instant speed change for the print head."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 99.0 // 50.0
                    fdmDefaultValue: 25.0   //20.0
                    fdmEnabled: jerk_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:jerk_infill; objectName: "jerk_infill"
                        fdmLabel: "Infill Jerk"
                        fdmDescription: "The maximum instant speed change for infill prints."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmDefaultValue: 25.0   //20.0
                        fdmValue: Number(jerk_print.fdmValue)
                        fdmEnabled: jerk_enabled.fdmValue && Number(infill_sparse_density.fdmValue) > 0
                        fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                        fdmSettablePerMesh: true
                        fdmAffectedById: "infill_extruder_nr,jerk_print"
                    }
                    FdmQml_Param{ id:jerk_wall; objectName: "jerk_wall"
                        fdmLabel: "Wall Jerk"
                        fdmDescription: "The maximum instant speed change for wall prints."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmDefaultValue: 15.0   //20.0
                        fdmValue: jerk_print.fdmValue //* 3.0 / 5.0
                        fdmEnabled: jerk_enabled.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: ""
                        FdmQml_Param{ id:jerk_wall_0; objectName: "jerk_wall_0"
                            fdmLabel: "Outer Wall Jerk"
                            fdmDescription: "The maximum instant speed change when printing the outermost walls."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                            fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                            fdmDefaultValue: 5.0   //20.0
                            fdmValue: Number(jerk_wall.fdmValue)
                            fdmEnabled: jerk_enabled.fdmValue
                            fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "wall_0_extruder_nr,jerk_wall"
                        }
                        FdmQml_Param{ id:jerk_wall_x; objectName: "jerk_wall_x"
                            fdmLabel: "Inner Wall Jerk"
                            fdmDescription: "The maximum instant speed change for inner wall prints."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                            fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                            fdmDefaultValue: 15.0   //20.0
                            fdmValue: Number(jerk_wall.fdmValue)
                            fdmEnabled: jerk_enabled.fdmValue
                            fdmLimitToExtruder: Number(wall_x_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "wall_x_extruder_nr,jerk_wall"
                        }
                        FdmQml_Param{ id:jerk_topbottom; objectName: "jerk_topbottom"
                            fdmLabel: "Top / Bottom Jerk"
                            fdmDescription: "The maximum instant speed change for top and bottom layer prints."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                            fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                            fdmDefaultValue: 15.0   //20.0
                            fdmValue: jerk_wall.fdmValue
                            fdmEnabled: (Number(top_layers.fdmValue) > 0 || Number(bottom_layers.fdmValue) > 0) && jerk_enabled.fdmValue
                            fdmLimitToExtruder: Number(top_bottom_extruder_nr.fdmValue)
                            fdmSettablePerMesh: true
                            fdmAffectedById: "top_bottom_extruder_nr"
                            FdmQml_Param{ id:jerk_roofing; objectName: "jerk_roofing"
                                fdmLabel: "Top Surface Skin Jerk"
                                fdmDescription: "The maximum instant speed change for top surface skin layer prints."
                                fdmUnit: "mm/s"
                                fdmType: "float"
                                fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                                fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                                fdmDefaultValue: 15.0   //20.0
                                fdmValue: Number(jerk_topbottom.fdmValue)
                                fdmEnabled: jerk_enabled.fdmValue && Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                                fdmSettablePerMesh: true
                                fdmAffectedById: "roofing_extruder_nr,jerk_topbottom"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_support; objectName: "jerk_support"
                        fdmLabel: "Support Jerk"
                        fdmDescription: "The maximum instant speed change for support structure prints."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmDefaultValue: 15.0   //20.0
                        fdmValue: Number(jerk_print.fdmValue)
                        fdmEnabled: jerk_enabled.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmLimitToExtruder: Number(support_extruder_nr.fdmValue)
                        fdmAffectedById: "jerk_print,support_extruder_nr"
                        FdmQml_Param{ id:jerk_support_infill; objectName: "jerk_support_infill"
                            fdmLabel: "Support Infill Jerk"
                            fdmDescription: "The maximum instant speed change for infill support prints."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmDefaultValue: 15.0   //20.0
                            fdmValue: Number(jerk_support.fdmValue)
                            fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                            fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                            fdmEnabled: jerk_enabled.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "jerk_support,support_infill_extruder_nr"
                        }
                        FdmQml_Param{ id:jerk_support_interface; objectName: "jerk_support_interface"
                            fdmLabel: "Support Interface Jerk"
                            fdmDescription: "The maximum instant speed change for support roof and floor prints."
                            fdmUnit: "mm/s"
                            fdmType: "float"
                            fdmDefaultValue: 15.0   //20.0
                            fdmValue: Number(jerk_support.fdmValue)
                            fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                            fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                            fdmEnabled: jerk_enabled.fdmValue && support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                            fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                            fdmSettablePerMesh: false
                            fdmSettablePerExtruder: true
                            fdmAffectedById: "jerk_support,support_interface_extruder_nr"
                            FdmQml_Param{ id:jerk_support_roof; objectName: "jerk_support_roof"
                                fdmLabel: "Support Roof Jerk"
                                fdmDescription: "The maximum instant speed change for support roof prints."
                                fdmUnit: "mm/s"
                                fdmType: "float"
                                fdmDefaultValue: 15.0   //20.0
                                fdmValue: Number(jerk_support_interface.fdmValue)
                                fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                                fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                                fdmEnabled: jerk_enabled.fdmValue && support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                                fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                                fdmSettablePerMesh: false
                                fdmSettablePerExtruder: true
                                fdmAffectedById: "jerk_support_interface,support_roof_extruder_nr"
                            }
                            FdmQml_Param{ id:jerk_support_bottom; objectName: "jerk_support_bottom"
                                fdmLabel: "Support Floor Jerk"
                                fdmDescription: "The maximum instant speed change for support floor prints."
                                fdmUnit: "mm/s"
                                fdmType: "float"
                                fdmDefaultValue: 15.0   //20.0
                                fdmValue: Number(jerk_support_interface.fdmValue)
                                fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                                fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                                fdmEnabled: jerk_enabled.fdmValue && support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                                fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                                fdmSettablePerMesh: false
                                fdmSettablePerExtruder: true
                                fdmAffectedById: "jerk_support_interface,support_bottom_extruder_nr"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_prime_tower; objectName: "jerk_prime_tower"
                        fdmLabel: "Prime Tower Jerk"
                        fdmDescription: "The maximum instant speed change for prime tower prints."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmDefaultValue: 25.0   //20.0
                        fdmValue: Number(jerk_print.fdmValue)
                        fdmEnabled: prime_tower_enable.fdmValue && jerk_enabled.fdmValue
                        fdmSettablePerMesh: false
                        fdmAffectedById: "jerk_print"
                    }
                }
                FdmQml_Param{ id:jerk_layer_0; objectName: "jerk_layer_0"
                    fdmLabel: "Initial Layer Jerk"
                    fdmDescription: "The maximum instant speed change when printing the initial layer."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 15.0   //20.0
                    fdmValue: Number(jerk_print.fdmValue)
                    fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                    fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                    fdmEnabled: jerk_enabled.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "jerk_print"
                    FdmQml_Param{ id:jerk_travel_layer_0; objectName: "jerk_travel_layer_0"
                        fdmLabel: "Initial Layer Travel Jerk"
                        fdmDescription: "The acceleration for travel moves in the initial layer."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmDefaultValue: 15.0   //20.0
                        fdmValue: Number(jerk_layer_0.fdmValue) * Number(jerk_travel.fdmValue) / Number(jerk_print.fdmValue)
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmEnabled: jerk_enabled.fdmValue
                        fdmSettablePerExtruder: true
                        fdmSettablePerMesh: false
                        fdmAffectedById: "jerk_travel,jerk_print,jerk_layer_0"
                    }
                    FdmQml_Param{ id:jerk_print_layer_0; objectName: "jerk_print_layer_0"
                        fdmLabel: "Initial Layer Print Jerk"
                        fdmDescription: "The maximum instant speed change when printing the initial layer."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmDefaultValue: 15.0   //20.0
                        fdmValue: Number(jerk_layer_0.fdmValue)
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmEnabled: jerk_enabled.fdmValue
                        fdmSettablePerMesh: true
                        fdmAffectedById: "jerk_layer_0"
                    }
                    FdmQml_Param{ id:jerk_skirt_brim; objectName: "jerk_skirt_brim"
                        fdmLabel: "Skirt / Brim Jerk"
                        fdmDescription: "The maximum instant speed change for skirt and brim prints."
                        fdmUnit: "mm/s"
                        fdmType: "float"
                        fdmDefaultValue: 15.0   //20.0
                        fdmMinimumValue: jerk_print.fdmMinimumValueWarning
                        fdmMaximumValueWarning: jerk_print.fdmMaximumValueWarning
                        fdmValue: Number(jerk_layer_0.fdmValue)
                        fdmEnabled: jerk_enabled.fdmValue && (adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || draft_shield_enabled.fdmValue || ooze_shield_enabled.fdmValue)
                        fdmSettablePerMesh: false
                        fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                        fdmAffectedById: "jerk_layer_0,adhesion_extruder_nr"
                    }
                }
            }
        }
        FdmQml_Category{ id:travel; objectName: "travel"
            fdmLabel: "Travel"
            fdmIcon: "PrintTravel"
            fdmDescription: "travel"
            fdmType: "category"
            FdmQml_Param{ id:retraction_enable; objectName: "retraction_enable"
                fdmLabel: "Enable Retraction"
                fdmDescription: "Retract the filament when the nozzle is moving over a non-printed area."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retract_at_layer_change; objectName: "retract_at_layer_change"
                fdmLabel: "Retract at Layer Change"
                fdmDescription: "Retract the filament when the nozzle is moving to the next layer."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_amount; objectName: "retraction_amount"
                fdmLabel: "Retraction Distance"
                fdmDescription: "The length of material retracted during a retraction move."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.5
                fdmMinimumValueWarning: -0.0001
                fdmMaximumValueWarning: 10.0
                fdmEnabled: retraction_enable.fdmValue && machine_gcode_flavor.fdmValue !== "UltiGCode"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_speed; objectName: "retraction_speed"
                fdmLabel: "Retraction Speed"
                fdmDescription: "The speed at which the filament is retracted and primed during a retraction move."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 60.0
                fdmMinimumValue: 0.0001
                fdmMinimumValueWarning: 1.0
                fdmMaximumValue: retraction_enable.fdmValue ? Number(machine_max_feedrate_e.fdmValue) : Infinity
                fdmMaximumValueWarning: 250.0
                fdmEnabled: retraction_enable.fdmValue && machine_gcode_flavor.fdmValue !== "UltiGCode"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e,retraction_enable"
                FdmQml_Param{ id:retraction_retract_speed; objectName: "retraction_retract_speed"
                    fdmLabel: "Retraction Retract Speed"
                    fdmDescription: "The speed at which the filament is retracted during a retraction move."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: retraction_speed.fdmDefaultValue
                    fdmMinimumValue: retraction_speed.fdmMinimumValue
                    fdmMaximumValue: retraction_speed.fdmMaximumValue
                    fdmMinimumValueWarning: retraction_speed.fdmMinimumValueWarning
                    fdmMaximumValueWarning: retraction_speed.fdmMaximumValueWarning
                    fdmEnabled: retraction_enable.fdmValue && machine_gcode_flavor.fdmValue !== "UltiGCode"
                    fdmValue: Number(retraction_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "retraction_speed"
                }
                FdmQml_Param{ id:retraction_prime_speed; objectName: "retraction_prime_speed"
                    fdmLabel: "Retraction Prime Speed"
                    fdmDescription: "The speed at which the filament is primed during a retraction move."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: retraction_speed.fdmDefaultValue
                    fdmMinimumValue: retraction_speed.fdmMinimumValue
                    fdmMaximumValue: retraction_speed.fdmMaximumValue
                    fdmMinimumValueWarning: retraction_speed.fdmMinimumValueWarning
                    fdmMaximumValueWarning: retraction_speed.fdmMaximumValueWarning
                    fdmEnabled: retraction_enable.fdmValue && machine_gcode_flavor.fdmValue !== "UltiGCode"
                    fdmValue: Number(retraction_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "retraction_speed"
                }
            }
            FdmQml_Param{ id:retraction_extra_prime_amount; objectName: "retraction_extra_prime_amount"
                fdmLabel: "Retraction Extra Prime Amount"
                fdmDescription: "Compensate for material that can ooze away during a travel move."
                fdmUnit: "mm³"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValueWarning: -0.0001
                fdmMaximumValueWarning: 5.0
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_prime_min_travel; objectName: "retraction_prime_min_travel"
                fdmLabel: "Retraction Extra Prime Min Travel Distance"
                fdmDescription: "Some material can ooze away during a travel move If travel distance less than this min distance, not extra amount."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmMinimumValueWarning: 10.0
                fdmMaximumValueWarning: 100.0
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_min_travel; objectName: "retraction_min_travel"
                fdmLabel: "Retraction Minimum Travel"
                fdmDescription: "The minimum distance of travel needed for a retraction. This helps prevent retractions in a small area."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.5
                fdmValue: Number(line_width.fdmValue) * 2
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(line_width.fdmValue) * 1.5
                fdmMaximumValueWarning: 10.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "line_width"
            }
            FdmQml_Param{ id:retraction_count_max; objectName: "retraction_count_max"
                fdmLabel: "Maximum Retraction Count"
                fdmDescription: "Limit the amount of retractions within a minimum extrusion distance timeframe. This avoids repeated retractions on the same piece of filament, which can faltten it and lead to grinding."
                fdmDefaultValue: 90
                fdmMinimumValue: 0
                fdmMaximumValueWarning: 100
                fdmType: "int"
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_extrusion_window; objectName: "retraction_extrusion_window"
                fdmLabel: "Minimum Extrusion Distance Window"
                fdmDescription: "The timeframe to enforce the maximum retraction amount. This value should be about the same as the retraction distance to limit the number of times a retraction passes the same area of material."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 4.5
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(retraction_amount.fdmValue) * 2
                fdmValue: Number(retraction_amount.fdmValue)
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "retraction_amount"
            }
            FdmQml_Param{ id:limit_support_retractions; objectName: "limit_support_retractions"
                fdmLabel: "Limit Support Retractions"
                fdmDescription: "Stop retractions when moving between supports in a straight line. This saves print time, but can lead to excessive stringing within the support structure."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: retraction_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_combing; objectName: "retraction_combing"
                fdmLabel: "Combing Mode"
                fdmDescription: "Keep the nozzle within printed areas when traveling. This makes travel moves longer, but reduces retractions. If Combing Mode is off, the material will retract, and the nozzle moves in a straight line to the next point. It is also possible to set this to avoid combing over top and bottom skin areas or only within the infill."
                fdmType: "enum"
                fdmOptions:{
                    "off": "Off",
                    "all": "All",
                    "noskin": "Not in Skin",
                    "infill": "Within Infill"
                }
                fdmDefaultValue: "all"
                //fdmResolve: "noskin" in retraction_combing.fdmValue ? "noskin" : ("infill.fdmValue" in retraction_combing.fdmValue ? "infill.fdmValue" : ("all" in retraction_combing.fdmValue ? "all" : "off"))
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "infill"
            }
            FdmQml_Param{ id:retraction_combing_max_distance; objectName: "retraction_combing_max_distance"
                fdmLabel: "Max Comb Distance Without Retraction"
                fdmDescription: "Comb travel moves longer than this distance will retract. If set to 0, combing moves will not retract."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmEnabled: retraction_combing.fdmValue !== "off"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:travel_retract_before_outer_wall; objectName: "travel_retract_before_outer_wall"
                fdmLabel: "Retract Before Outer Wall"
                fdmDescription: "Always retract when moving to start an outer wall."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:travel_avoid_other_parts; objectName: "travel_avoid_other_parts"
                fdmLabel: "Avoid Printed Parts When Traveling"
                fdmDescription: "The nozzle avoids printed parts when traveling. This option is only available when combing is enabled."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: retraction_combing.fdmValue !== "off"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:travel_avoid_supports; objectName: "travel_avoid_supports"
                fdmLabel: "Avoid Supports When Traveling"
                fdmDescription: "The nozzle avoids printed supports when traveling. This option is only available when combing is enabled."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: retraction_combing.fdmValue !== "off" && travel_avoid_other_parts.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:travel_avoid_distance; objectName: "travel_avoid_distance"
                fdmLabel: "Travel Avoid Distance"
                fdmDescription: "This is the avoidnce distance between the nozzle and printed parts during travel moves."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.625
                fdmValue: Number(machine_nozzle_tip_outer_diameter.fdmValue) / 2 * 1.25
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(machine_nozzle_tip_outer_diameter.fdmValue) * 0.5
                fdmMaximumValueWarning: Number(machine_nozzle_tip_outer_diameter.fdmValue) * 5
                fdmEnabled: retraction_combing.fdmValue !== "off" && travel_avoid_other_parts.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_nozzle_tip_outer_diameter"
            }
            FdmQml_Param{ id:layer_start_x; objectName: "layer_start_x"
                fdmLabel: "Layer Start X"
                fdmDescription: "The X coordinate where each layer will approximately begin to print."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: machine_center_is_zero.fdmValue ? (Number(machine_width.fdmValue) / -2) : 0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: true
                fdmAffectedById: "machine_width,machine_center_is_zero"
            }
            FdmQml_Param{ id:layer_start_y; objectName: "layer_start_y"
                fdmLabel: "Layer Start Y"
                fdmDescription: "The Y coordinate where each layer will approximately begin to print."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: machine_center_is_zero.fdmValue ? (Number(machine_depth.fdmValue) / -2) : 0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: true
                fdmAffectedById: "machine_depth,machine_center_is_zero"
            }
            FdmQml_Param{ id:retraction_hop_enabled; objectName: "retraction_hop_enabled"
                fdmLabel: "Z Hop When Retracted"
                fdmDescription: "Whenever a retraction finishes, the build plate lowers to create clearance between the nozzle and print. This prevents the nozzle from hitting the print during travel moves, reducing the chance to knock the print off the build plate."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: retraction_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_hop_only_when_collides; objectName: "retraction_hop_only_when_collides"
                fdmLabel: "Z Hop Only Over Printed Parts"
                fdmDescription: "Only perform a Z Hop when moving over printed parts which cannot be avoided by horizontal motion with the Avoid Printed Parts when Traveling setting."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: retraction_enable.fdmValue && retraction_hop_enabled.fdmValue && travel_avoid_other_parts.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_hop; objectName: "retraction_hop"
                fdmLabel: "Z Hop Height"
                fdmDescription: "The height difference when performing a Z Hop."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: retraction_enable.fdmValue && retraction_hop_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch; objectName: "retraction_hop_after_extruder_switch"
                fdmLabel: "Z Hop After Extruder Switch"
                fdmDescription: "After the machine switches to another extruder, the build plate lowers to create clearance between the nozzle and the print. This prevents the nozzle from oozing material on the outside of a print."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: retraction_hop_enabled.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch_height; objectName: "retraction_hop_after_extruder_switch_height"
                fdmLabel: "Z Hop After Extruder Switch Height"
                fdmDescription: "The height difference when performing a Z Hop after an extruder switch."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmValue: Number(retraction_hop.fdmValue)
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: retraction_enable.fdmValue && retraction_hop_after_extruder_switch.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "retraction_hop"
            }
        }
        FdmQml_Category{ id:cooling; objectName: "cooling"
            fdmLabel: "Cooling"
            fdmIcon: "Fan"
            fdmDescription: "Cooling"
            fdmType: "category"
            FdmQml_Param{ id:cool_fan_enabled; objectName: "cool_fan_enabled"
                fdmLabel: "Enable Print Cooling"
                fdmDescription: "Enable print cooling fans while printing. Fans improve the print quality on layers with short print times and bridges/overhangs."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:cool_fan_speed; objectName: "cool_fan_speed"
                fdmLabel: "Fan Speed"
                fdmDescription: "The speed at which the print cooling fans spin."
                fdmUnit: "%"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 100.0
                fdmDefaultValue: 100.0
                fdmValue: cool_fan_enabled.fdmValue ? 100.0 : 0.0
                fdmEnabled: cool_fan_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "cool_fan_enabled"
                FdmQml_Param{ id:cool_fan_speed_min; objectName: "cool_fan_speed_min"
                    fdmLabel: "Regular Fan Speed"
                    fdmDescription: "The speed that fans spin before reaching the threshold. When a layer prints faster than the threshold, the fan speed gradually increases twoard maximum fan speed."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmValue: Number(cool_fan_speed.fdmValue)
                    fdmDefaultValue: 100.0
                    fdmEnabled: cool_fan_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "cool_fan_speed"
                }
                FdmQml_Param{ id:cool_fan_speed_max; objectName: "cool_fan_speed_max"
                    fdmLabel: "Maximum Fan Speed"
                    fdmDescription: "The speed that fans spin for minimum layer time. The fan speed gradually increases between regular fan speed and maximum fan speed when the threshold is reached."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: Math.max(0, Number(cool_fan_speed_min.fdmValue))
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 100.0
                    fdmEnabled: cool_fan_enabled.fdmValue
                    fdmValue: Number(cool_fan_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "cool_fan_speed,cool_fan_speed_min"
                }
                FdmQml_Param{ id:cool_min_layer_time_fan_speed_max; objectName: "cool_min_layer_time_fan_speed_max"
                    fdmLabel: "Regular / Maximum Fan Speed Threshold"
                    fdmDescription: "This layer time sets the threshold between the regular fan speed and maximum fan speed. Layers that print slower than this time use regular fan speed. For faster layers, the fan speed graudally increases toward maximum fan speed."
                    fdmUnit: "s"
                    fdmType: "float"
                    fdmDefaultValue: 10.0
                    fdmMaximumValueWarning: 600.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:cool_fan_speed_0; objectName: "cool_fan_speed_0"
                    fdmLabel: "Initial Fan Speed"
                    fdmDescription: "The fan speed at the start of the print. For subsequent layers, the fan speed gradually increases to the layer where \"Regular Fan Speed at Height\" applies."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 0.0
                    fdmEnabled: cool_fan_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:cool_fan_full_at_height; objectName: "cool_fan_full_at_height"
                    fdmLabel: "Regular Fan Speed at Height"
                    fdmDescription: "The height at which the fans spin on regular fan speed. At the layers below the fan speed gradually increases from Initial Fan Speed to Regular Fan Speed."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.5
                    fdmValue: adhesion_type.fdmValue === "raft" ? 0 : Number(layer_height_0.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 10.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "layer_height_0,adhesion_type"
                    FdmQml_Param{ id:cool_fan_full_layer; objectName: "cool_fan_full_layer"
                        fdmLabel: "Regular Fan Speed at Layer"
                        fdmDescription: "The layer at which the fans spin on regular fan speed. If regular fan speed at height is set, this value is calculated and rounded to a whole number."
                        fdmType: "int"
                        fdmDefaultValue: 2
                        fdmMinimumValue: 1
                        fdmMaximumValueWarning: 10 / Number(layer_height.fdmValue)
                        fdmValue:  Math.max(1, Math.floor(Math.floor((Number(cool_fan_full_at_height.fdmValue) - Number(layer_height_0.fdmValue)) / Number(layer_height.fdmValue)) + 2))
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "layer_height,layer_height_0,cool_fan_full_at_height"
                    }
                }
                FdmQml_Param{ id:cool_min_layer_time; objectName: "cool_min_layer_time"
                    fdmLabel: "Minimum Layer Time"
                    fdmDescription: "The minimum time spent in a layer. This forces the printer to slow down, to at least spend the time set here in one layer. This allows the printed material to cool down properly before printing the next layer. Layers may still take shorter than the minimal layer time if Lift Head is disabled and if the Minimum Speed would otherwise be violated."
                    fdmUnit: "s"
                    fdmType: "float"
                    fdmDefaultValue: 5.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 600.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                    FdmQml_Param{ id:cool_min_speed_includes_outer_walls; objectName: "cool_min_speed_includes_outer_walls"
                        fdmLabel: "Include Outer Walls"
                        fdmDescription: "Include outer walls when printing at minmum speed."
                        fdmType: "bool"
                        fdmDefaultValue: true
                        fdmSettablePerMesh: true
                        fdmSettablePerExtruder: true
                        fdmAffectedById: ""
                    }
                }
                FdmQml_Param{ id:cool_min_speed; objectName: "cool_min_speed"
                    fdmLabel: "Minimum Speed"
                    fdmDescription: "The minimum print speed, despite slowing down due to the minimum layer time. When the printer would slow down too much, the pressure in the nozzle would be too low and result in bad print quality."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 10.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 100.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:cool_lift_head; objectName: "cool_lift_head"
                    fdmLabel: "Lift Head"
                    fdmDescription: "When the minimum speed is hit because of minimum layer time, lift the head away from the print and wait the extra time until the minimum layer time is reached."
                    fdmType: "bool"
                    fdmDefaultValue: false
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: ""
                }
            }
        }
        FdmQml_Category{ id:support; objectName: "support"
            fdmLabel: "Support"
            fdmType: "category"
            fdmIcon: "Support"
            fdmDescription: "Support"
            FdmQml_Param{ id:support_enable; objectName: "support_enable"
                fdmLabel: "Generate Support"
                fdmDescription: "Generate structures to support parts of the model which have overhangs. Without these structures, such parts would collapse during printing."
                fdmType: "bool"
                fdmEnabled: true
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_extruder_nr; objectName: "support_extruder_nr"
                fdmLabel: "Support Extruder"
                fdmDescription: "The extruder train to use for printing the support. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: 0
                fdmValue: 0
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
                FdmQml_Param{ id:support_infill_extruder_nr; objectName: "support_infill_extruder_nr"
                    fdmLabel: "Support Infill Extruder"
                    fdmDescription: "The extruder train to use for printing the infill of the support. This is used in multi-extrusion."
                    fdmType: "int"
                    fdmDefaultValue: 0
                    fdmValue: Number(support_extruder_nr.fdmValue)
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: "support_extruder_nr"
                }
                FdmQml_Param{ id:support_extruder_nr_layer_0; objectName: "support_extruder_nr_layer_0"
                    fdmLabel: "First Layer Support Extruder"
                    fdmDescription: "The extruder train to use for printing the first layer of support infill. This is used in multi-extrusion."
                    fdmType: "int"
                    fdmDefaultValue: 0
                    fdmValue: Number(support_extruder_nr.fdmValue)
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: "support_extruder_nr"
                }
                FdmQml_Param{ id:support_interface_extruder_nr; objectName: "support_interface_extruder_nr"
                    fdmLabel: "Support Interface Extruder"
                    fdmDescription: "The extruder train to use for printing the roofs and floors of the support. This is used in multi-extrusion."
                    fdmType: "int"
                    fdmDefaultValue: 0
                    fdmValue: Number(support_extruder_nr.fdmValue)
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: "support_extruder_nr"
                    FdmQml_Param{ id:support_roof_extruder_nr; objectName: "support_roof_extruder_nr"
                        fdmLabel: "Support Roof Extruder"
                        fdmDescription: "The extruder train to use for printing the roofs of the support. This is used in multi-extrusion."
                        fdmType: "int"
                        fdmDefaultValue: 0
                        fdmValue: Number(support_interface_extruder_nr.fdmValue)
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmAffectedById: "support_interface_extruder_nr"
                    }
                    FdmQml_Param{ id:support_bottom_extruder_nr; objectName: "support_bottom_extruder_nr"
                        fdmLabel: "Support Floor Extruder"
                        fdmDescription: "The extruder train to use for printing the floors of the support. This is used in multi-extrusion."
                        fdmType: "int"
                        fdmDefaultValue: 0
                        fdmValue: Number(support_interface_extruder_nr.fdmValue)
                        fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(extruders_enabled_count.fdmValue) > 1
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: false
                        fdmAffectedById: "support_interface_extruder_nr"
                    }
                }
            }
            FdmQml_Param{ id:support_structure; objectName: "support_structure"
                fdmLabel: "Support Structure"
                fdmDescription: "Chooses between the techniques available to generate support. \"Normal\" support creates a support structure directly below the overhanging parts and drops those areas straight down. \"Tree\" support creates branches towards the overhanging areas that support the model on the tips of those branches, and allows the branches to crawl around the model to support it from the build plate as much as possible."
                fdmType: "enum"
                fdmOptions:{
                    "normal": "Normal",
                    "tree": "Tree"
                }
                fdmEnabled: support_enable.fdmValue
                fdmDefaultValue: "normal"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_tree_angle; objectName: "support_tree_angle"
                fdmLabel: "Tree Support Branch Angle"
                fdmDescription: "The angle of the branches. Use a lower angle to make them more vertical and more stable. Use a higher angle to be able to have more reach."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 90.0
                fdmMaximumValueWarning: 60.0
                fdmDefaultValue: 40.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue==="tree"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tree_branch_distance; objectName: "support_tree_branch_distance"
                fdmLabel: "Tree Support Branch Distance"
                fdmDescription: "How far apart the branches need to be when they touch the model. Making this distance small will cause the tree support to touch the model at more points, causing better overhang but making support harder to remove."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.001
                fdmDefaultValue: 1.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue==="tree"
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tree_branch_diameter; objectName: "support_tree_branch_diameter"
                fdmLabel: "Tree Support Branch Diameter"
                fdmDescription: "The diameter of the thinnest branches of tree support. Thicker branches are more sturdy. Branches towards the base will be thicker than this."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: Number(support_line_width.fdmValue) * 2
                fdmDefaultValue: 2.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue==="tree"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_line_width,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tree_branch_diameter_angle; objectName: "support_tree_branch_diameter_angle"
                fdmLabel: "Tree Support Branch Diameter Angle"
                fdmDescription: "The angle of the branches' diameter as they gradually become thicker towards the bottom. An angle of 0 will cause the branches to have uniform thickness over their length. A bit of an angle can increase stability of the tree support."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 89.9999
                fdmMaximumValueWarning: 15.0
                fdmDefaultValue: 5.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue==="tree"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tree_collision_resolution; objectName: "support_tree_collision_resolution"
                fdmLabel: "Tree Support Collision Resolution"
                fdmDescription: "Resolution to compute collisions with to avoid hitting the model. Setting this lower will produce more accurate trees that fail less often, but increases slicing time dramatically."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: Number(support_line_width.fdmValue) / 4
                fdmMaximumValueWarning: Number(support_line_width.fdmValue) * 2
                fdmDefaultValue: 0.4
                fdmValue: Number(support_line_width.fdmValue) / 2
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue==="tree" && Number(support_tree_branch_diameter_angle.fdmValue) > 0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_line_width,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_type; objectName: "support_type"
                fdmLabel: "Support Placement"
                fdmDescription: "Adjusts the placement of the support structures. The placement can be set to touching build plate or everywhere. When set to everywhere the support structures will also be printed on the model."
                fdmType: "enum"
                fdmOptions:{
                    "buildplate": "Touching Buildplate",
                    "everywhere": "Everywhere"
                }
                fdmDefaultValue: "everywhere"
                //fdmResolve: "everywhere" in support_type.fdmValue ? "everywhere" : "buildplate"
                fdmEnabled: support_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_angle; objectName: "support_angle"
                fdmLabel: "Support Overhang Angle"
                fdmDescription: "The minimum angle of overhangs for which support is added. At a value of 0° all overhangs are supported, great than 90° will not provide any support."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 90.0
                fdmMaximumValueWarning: 80.0
                fdmDefaultValue: 50.0
                fdmLimitToExtruder: support_roof_enable.fdmValue ? Number(support_roof_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr,support_roof_extruder_nr,support_roof_enable"
            }
            FdmQml_Param{ id:support_pattern; objectName: "support_pattern"
                fdmLabel: "Support Pattern"
                fdmDescription: "The pattern of the support structures of the print. The different options available result in sturdy or easy to remove support."
                fdmType: "enum"
                fdmOptions:{
                    "lines": "Lines",
                    "grid": "Grid",
                    "triangles": "Triangles",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag",
                    "cross": "Cross",
                    "gyroid": "Gyroid"
                }
                fdmDefaultValue: "zigzag"
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_structure.fdmValue!=="tree"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_wall_count; objectName: "support_wall_count"
                fdmLabel: "Support Wall Line Count"
                fdmDescription: "The number of walls with which to surround support infill. Adding a wall can make support print more reliably and can support overhangs better, but increases print time and material used."
                fdmDefaultValue: 1
                fdmMinimumValue: 0
                fdmMinimumValueWarning: support_pattern.fdmValue === "concentric" ? 1 : 0
                fdmMaximumValueWarning: (support_skip_some_zags.fdmValue && support_pattern.fdmValue === "zigzag") ? 0 : 3
                fdmType: "int"
                fdmValue: (support_enable.fdmValue && support_structure.fdmValue === "tree") ? 1 : ((support_pattern.fdmValue === "grid" || support_pattern.fdmValue === "triangles" || support_pattern.fdmValue === "concentric") ? 1 : 0)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_enable,support_infill_extruder_nr,support_structure,support_pattern,support_skip_some_zags"
            }
            FdmQml_Param{ id:zig_zaggify_support; objectName: "zig_zaggify_support"
                fdmLabel: "Connect Support Lines"
                fdmDescription: "Connect the ends of the support lines together. Enabling this setting can make your support more sturdy and reduce underextrusion, but it will cost more material."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: support_pattern.fdmValue === "cross" || support_pattern.fdmValue === "gyroid"
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && (support_pattern.fdmValue === "lines" || support_pattern.fdmValue === "grid" || support_pattern.fdmValue === "triangles" || support_pattern.fdmValue === "cross" || support_pattern.fdmValue === "gyroid")
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr,support_pattern"
            }
            FdmQml_Param{ id:support_connect_zigzags; objectName: "support_connect_zigzags"
                fdmLabel: "Connect Support ZigZags"
                fdmDescription: "Connect the ZigZags. This will increase the strength of the zig zag support structure."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_pattern.fdmValue === "zigzag"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_infill_rate; objectName: "support_infill_rate"
                fdmLabel: "Support Density"
                fdmDescription: "Adjusts the density of the support structure. A higher value results in better overhangs, but the supports are harder to remove."
                fdmUnit: "%"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 100.0
                fdmDefaultValue: 15.0
                fdmValue: 15.0 // (support_enable.fdmValue && support_structure.fdmValue === "normal") ? 15 : ((support_enable.fdmValue && support_structure.fdmValue === "tree") ? 0 : 15)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_enable,support_infill_extruder_nr,support_structure"
                FdmQml_Param{ id:support_line_distance; objectName: "support_line_distance"
                    fdmLabel: "Support Line Distance"
                    fdmDescription: "Distance between the printed support structure lines. This setting is calculated by the support density."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(support_line_width.fdmValue)
                    fdmDefaultValue: 2.66
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmValue: Number(support_infill_rate.fdmValue) === 0 ? 0 : (Number(support_line_width.fdmValue) * 100) / Number(support_infill_rate.fdmValue) * (support_pattern.fdmValue === "grid" ? 2 : (support_pattern.fdmValue === "triangles" ? 3 : 1))
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_line_width,support_infill_extruder_nr,support_pattern,support_infill_rate"
                }
                FdmQml_Param{ id:support_initial_layer_line_distance; objectName: "support_initial_layer_line_distance"
                    fdmLabel: "Initial Layer Support Line Distance"
                    fdmDescription: "Distance between the printed initial layer support structure lines. This setting is calculated by the support density."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(support_line_width.fdmValue)
                    fdmDefaultValue: 2.66
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmValue: Number(support_line_distance.fdmValue)
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_line_width,support_infill_extruder_nr,support_line_distance"
                }
            }
            FdmQml_Param{ id:support_infill_angles; objectName: "support_infill_angles"
                fdmLabel: "Support Infill Line Directions"
                fdmDescription: "A list of integer line directions to use. Elements from the list are used sequentially as the layers progress and when the end of the list is reached, it starts at the beginning again. The list items are separated by commas and the whole list is contained in square brackets. Default is an empty list which means use the default angle 0 degrees."
                fdmType: "[int]"
                fdmDefaultValue: "[ ]"
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_pattern.fdmValue !== "concentric" && Number(support_infill_rate.fdmValue) > 0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_brim_enable; objectName: "support_brim_enable"
                fdmLabel: "Enable Support Brim"
                fdmDescription: "Generate a brim within the support infill regions of the first layer. This brim is printed underneath the support, not around it. Enabling this setting increases the adhesion of support to the build plate."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: support_structure.fdmValue === "tree"
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr,support_structure"
            }
            FdmQml_Param{ id:support_brim_width; objectName: "support_brim_width"
                fdmLabel: "Support Brim Width"
                fdmDescription: "The width of the brim to print underneath the support. A larger brim enhances adhesion to the build plate, at the cost of some extra material."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 8.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 50.0
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_brim_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmAffectedById: "support_infill_extruder_nr"
                FdmQml_Param{ id:support_brim_line_count; objectName: "support_brim_line_count"
                    fdmLabel: "Support Brim Line Count"
                    fdmDescription: "The number of lines used for the support brim. More brim lines enhance adhesion to the build plate, at the cost of some extra material."
                    fdmType: "int"
                    fdmDefaultValue: 20
                    fdmMinimumValue: 0
                    fdmMaximumValueWarning: 50 / Number(skirt_brim_line_width.fdmValue)
                    fdmValue:  Math.ceil(Number(support_brim_width.fdmValue) / (Number(skirt_brim_line_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100.0))
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_brim_enable.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmAffectedById: "skirt_brim_line_width,initial_layer_line_width_factor,support_infill_extruder_nr,support_brim_width"
                }
            }
            FdmQml_Param{ id:support_z_distance; objectName: "support_z_distance"
                fdmLabel: "Support Z Distance"
                fdmDescription: "Distance from the top/bottom of the support structure to the print. This gap provides clearance to remove the supports after the model is printed. This value is rounded up to a multiple of the layer height."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmDefaultValue: 0.1
                fdmLimitToExtruder: support_interface_enable.fdmValue ? Number(support_interface_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr,support_interface_extruder_nr,support_interface_enable"
                FdmQml_Param{ id:support_top_distance; objectName: "support_top_distance"
                    fdmLabel: "Support Top Distance"
                    fdmDescription: "Distance from the top of the support to the print."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.1
                    fdmType: "float"
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmValue: support_z_distance.fdmValue
                    fdmLimitToExtruder: support_roof_enable.fdmValue ? Number(support_roof_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr,support_roof_extruder_nr,support_roof_enable"
                }
                FdmQml_Param{ id:support_bottom_distance; objectName: "support_bottom_distance"
                    fdmLabel: "Support Bottom Distance"
                    fdmDescription: "Distance from the print to the bottom of the support."
                    fdmUnit: "mm"
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                    fdmDefaultValue: 0.1
                    fdmValue: support_type.fdmValue === "everywhere" ? support_z_distance.fdmValue : 0
                    fdmLimitToExtruder: support_bottom_enable.fdmValue ? Number(support_bottom_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                    fdmType: "float"
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_type.fdmValue === "everywhere"
                    fdmSettablePerMesh: true
                    fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr,support_bottom_extruder_nr,support_bottom_enable"
                }
            }
            FdmQml_Param{ id:support_xy_distance; objectName: "support_xy_distance"
                fdmLabel: "Support X/Y Distance"
                fdmDescription: "Distance of the support structure from the print in the X/Y directions."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 1.5 * Number(machine_nozzle_tip_outer_diameter.fdmValue)
                fdmDefaultValue: 0.7
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_tip_outer_diameter,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_xy_overrides_z; objectName: "support_xy_overrides_z"
                fdmLabel: "Support Distance Priority"
                fdmDescription: "Whether the Support X/Y Distance overrides the Support Z Distance or vice versa. When X/Y overrides Z the X/Y distance can push away the support from the model, influencing the actual Z distance to the overhang. We can disable this by not applying the X/Y distance around overhangs."
                fdmType: "enum"
                fdmOptions:{
                    "xy_overrides_z": "X/Y overrides Z",
                    "z_overrides_xy": "Z overrides X/Y"
                }
                fdmDefaultValue: "z_overrides_xy"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_xy_distance_overhang; objectName: "support_xy_distance_overhang"
                fdmLabel: "Minimum Support X/Y Distance"
                fdmDescription: "Distance of the support structure from the overhang in the X/Y directions."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(support_xy_distance.fdmValue) - Number(support_line_width.fdmValue) * 2
                fdmMaximumValueWarning: Number(support_xy_distance.fdmValue)
                fdmDefaultValue: 0.2
                fdmValue: Number(machine_nozzle_size.fdmValue) / 2
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_xy_overrides_z.fdmValue === "z_overrides_xy"
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,support_line_width,support_infill_extruder_nr,support_xy_distance"
            }
            FdmQml_Param{ id:support_bottom_stair_step_height; objectName: "support_bottom_stair_step_height"
                fdmLabel: "Support Stair Step Height"
                fdmDescription: "The height of the steps of the stair-like bottom of support resting on the model. A low value makes the support harder to remove, but too high values can lead to unstable support structures. Set to zero to turn off the stair-like behaviour."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.3
                fdmLimitToExtruder: support_bottom_enable.fdmValue ? Number(support_bottom_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 1.0
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr,support_bottom_extruder_nr,support_bottom_enable"
            }
            FdmQml_Param{ id:support_bottom_stair_step_width; objectName: "support_bottom_stair_step_width"
                fdmLabel: "Support Stair Step Maximum Width"
                fdmDescription: "The maximum width of the steps of the stair-like bottom of support resting on the model. A low value makes the support harder to remove, but too high values can lead to unstable support structures."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 5.0
                fdmLimitToExtruder: support_interface_enable.fdmValue ? Number(support_interface_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr,support_interface_extruder_nr,support_interface_enable"
            }
            FdmQml_Param{ id:support_bottom_stair_step_min_slope; objectName: "support_bottom_stair_step_min_slope"
                fdmLabel: "Support Stair Step Minimum Slope Angle"
                fdmDescription: "The minimum slope of the area for stair-stepping to take effect. Low values should make support easier to remove on shallower slopes, but really low values may result in some very counter-intuitive results on other parts of the model."
                fdmUnit: "°"
                fdmType: "float"
                fdmDefaultValue: 10.0
                fdmLimitToExtruder: support_bottom_enable.fdmValue ? Number(support_bottom_extruder_nr.fdmValue) : Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValue: 0.01
                fdmMaximumValue: 89.99
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr,support_bottom_extruder_nr,support_bottom_enable"
            }
            FdmQml_Param{ id:support_join_distance; objectName: "support_join_distance"
                fdmLabel: "Support Join Distance"
                fdmDescription: "The maximum distance between support structures in the X/Y directions. When separate structures are closer together than this value, the structures merge into one."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 2.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: (support_enable.fdmValue && support_structure.fdmValue === "normal") || support_meshes_present.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_offset; objectName: "support_offset"
                fdmLabel: "Support Horizontal Expansion"
                fdmDescription: "Amount of offset applied to all support polygons in each layer. Positive values can smooth out the support areas and result in more sturdy support."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValueWarning: -1 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 10 * Number(machine_nozzle_size.fdmValue)
                fdmEnabled: (support_enable.fdmValue && support_structure.fdmValue === "normal") || support_meshes_present.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_infill_sparse_thickness; objectName: "support_infill_sparse_thickness"
                fdmLabel: "Support Infill Layer Thickness"
                fdmDescription: "The thickness per layer of support infill material. This value should always be a multiple of the layer height and is otherwise rounded."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.1
                fdmMinimumValue: Number(layer_height.fdmValue)
                fdmMaximumValueWarning: 0.75 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValue: Number(layer_height.fdmValue) * 8
                fdmValue: Number(layer_height.fdmValue)
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(support_infill_rate.fdmValue) > 0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_nozzle_size,layer_height,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:gradual_support_infill_steps; objectName: "gradual_support_infill_steps"
                fdmLabel: "Gradual Support Infill Steps"
                fdmDescription: "Number of times to reduce the support infill density by half when getting further below top surfaces. Areas which are closer to top surfaces get a higher density, up to the Support Infill Density."
                fdmDefaultValue: 0
                fdmType: "int"
                fdmMinimumValue: 0
                fdmMaximumValueWarning: (support_pattern.fdmValue === "cross" || support_pattern.fdmValue === "lines" || support_pattern.fdmValue === "zigzag" || support_pattern.fdmValue === "concentric") ? 1 : 5
                fdmMaximumValue: Number(support_line_distance.fdmValue) === 0 ? 999999 : (20 - Math.log(Number(support_line_distance.fdmValue)) / Math.log(2))
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(support_infill_rate.fdmValue) > 0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr,support_pattern,support_line_distance"
            }
            FdmQml_Param{ id:gradual_support_infill_step_height; objectName: "gradual_support_infill_step_height"
                fdmLabel: "Gradual Support Infill Step Height"
                fdmDescription: "The height of support infill of a given density before switching to half the density."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmMinimumValue: 0.0001
                fdmMinimumValueWarning: 3 * Number(layer_height.fdmValue)
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && Number(support_infill_rate.fdmValue) > 0 && Number(gradual_support_infill_steps.fdmValue) > 0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "layer_height,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:minimum_support_area; objectName: "minimum_support_area"
                fdmLabel: "Minimum Support Area"
                fdmDescription: "Minimum area size for support polygons. Polygons which have an area smaller than this value will not be generated."
                fdmUnit: "mm²"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 5.0
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_structure.fdmValue === "normal"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_interface_enable; objectName: "support_interface_enable"
                fdmLabel: "Enable Support Interface"
                fdmDescription: "Generate a dense interface between the model and the support. This will create a skin at the top of the support on which the model is printed and at the bottom of the support, where it rests on the model."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_interface_extruder_nr"
                FdmQml_Param{ id:support_roof_enable; objectName: "support_roof_enable"
                    fdmLabel: "Enable Support Roof"
                    fdmDescription: "Generate a dense slab of material between the top of support and the model. This will create a skin between the model and support."
                    fdmType: "bool"
                    fdmDefaultValue: false
                    fdmValue: support_interface_enable.fdmValue
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "support_roof_extruder_nr,support_interface_enable"
                }
                FdmQml_Param{ id:support_bottom_enable; objectName: "support_bottom_enable"
                    fdmLabel: "Enable Support Floor"
                    fdmDescription: "Generate a dense slab of material between the bottom of the support and the model. This will create a skin between the model and support."
                    fdmType: "bool"
                    fdmDefaultValue: false
                    fdmValue: support_interface_enable.fdmValue
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                    fdmSettablePerMesh: true
                    fdmAffectedById: "support_bottom_extruder_nr,support_interface_enable"
                }
            }
            FdmQml_Param{ id:support_interface_height; objectName: "support_interface_height"
                fdmLabel: "Support Interface Thickness"
                fdmDescription: "The thickness of the interface of the support where it touches with the model on the bottom or the top."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.2 + Number(layer_height.fdmValue)
                fdmMaximumValueWarning: 10.0
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "layer_height,support_interface_extruder_nr"
                FdmQml_Param{ id:support_roof_height; objectName: "support_roof_height"
                    fdmLabel: "Support Roof Thickness"
                    fdmDescription: "The thickness of the support roofs. This controls the amount of dense layers at the top of the support on which the model rests."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(support_top_distance.fdmValue) + Number(layer_height.fdmValue)
                    fdmMaximumValueWarning: 10.0
                    fdmValue: Number(support_interface_height.fdmValue)
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "layer_height,support_roof_extruder_nr,support_top_distance,support_interface_height"
                }
                FdmQml_Param{ id:support_bottom_height; objectName: "support_bottom_height"
                    fdmLabel: "Support Floor Thickness"
                    fdmDescription: "The thickness of the support floors. This controls the number of dense layers that are printed on top of places of a model on which support rests."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(support_interface_height.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Math.min(Number(support_bottom_distance.fdmValue) + Number(layer_height.fdmValue), Number(support_bottom_stair_step_height.fdmValue))
                    fdmMaximumValueWarning: 10.0
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "layer_height,support_bottom_extruder_nr,support_bottom_distance,support_bottom_stair_step_height,support_interface_height"
                }
            }
            FdmQml_Param{ id:support_interface_skip_height; objectName: "support_interface_skip_height"
                fdmLabel: "Support Interface Resolution"
                fdmDescription: "When checking where there's model above and below the support, take steps of the given height. Lower values will slice slower, while higher values may cause normal support to be printed in some places where there should have been support interface."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.3
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(support_interface_height.fdmValue)
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "support_interface_extruder_nr,support_interface_height"
            }
            FdmQml_Param{ id:support_interface_density; objectName: "support_interface_density"
                fdmLabel: "Support Interface Density"
                fdmDescription: "Adjusts the density of the roofs and floors of the support structure. A higher value results in better overhangs, but the supports are harder to remove."
                fdmUnit: "%"
                fdmType: "float"
                fdmDefaultValue: 100.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 100.0
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_interface_extruder_nr"
                FdmQml_Param{ id:support_roof_density; objectName: "support_roof_density"
                    fdmLabel: "Support Roof Density"
                    fdmDescription: "The density of the roofs of the support structure. A higher value results in better overhangs, but the supports are harder to remove."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmValue: Number(support_interface_density.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_roof_extruder_nr,support_interface_density"
                    FdmQml_Param{ id:support_roof_line_distance; objectName: "support_roof_line_distance"
                        fdmLabel: "Support Roof Line Distance"
                        fdmDescription: "Distance between the printed support roof lines. This setting is calculated by the Support Roof Density, but can be adjusted separately."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 0.4
                        fdmMinimumValue: 0.0
                        fdmMinimumValueWarning: Number(support_roof_line_width.fdmValue) - 0.0001
                        fdmValue: Number(support_roof_density.fdmValue) === 0 ? 0 : (Number(support_roof_line_width.fdmValue) * 100) / Number(support_roof_density.fdmValue) * (support_roof_pattern.fdmValue === "grid" ? 2 : (support_roof_pattern.fdmValue === "triangles" ? 3 : 1))
                        fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                        fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "support_roof_line_width,support_roof_extruder_nr,support_roof_density,support_roof_pattern"
                    }
                }
                FdmQml_Param{ id:support_bottom_density; objectName: "support_bottom_density"
                    fdmLabel: "Support Floor Density"
                    fdmDescription: "The density of the floors of the support structure. A higher value results in better adhesion of the support on top of the model."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmValue: Number(support_interface_density.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_bottom_extruder_nr,support_interface_density"
                    FdmQml_Param{ id:support_bottom_line_distance; objectName: "support_bottom_line_distance"
                        fdmLabel: "Support Floor Line Distance"
                        fdmDescription: "Distance between the printed support floor lines. This setting is calculated by the Support Floor Density, but can be adjusted separately."
                        fdmUnit: "mm"
                        fdmType: "float"
                        fdmDefaultValue: 0.4
                        fdmMinimumValue: 0.0
                        fdmMinimumValueWarning: Number(support_bottom_line_width.fdmValue) - 0.0001
                        fdmValue: Number(support_bottom_density.fdmValue) === 0 ? 0 : (Number(support_bottom_line_width.fdmValue) * 100) / Number(support_bottom_density.fdmValue) * (support_bottom_pattern.fdmValue === "grid" ? 2 : (support_bottom_pattern.fdmValue === "triangles" ? 3 : 1))
                        fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                        fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                        fdmSettablePerMesh: false
                        fdmSettablePerExtruder: true
                        fdmAffectedById: "support_bottom_line_width,support_bottom_extruder_nr,support_bottom_density,support_bottom_pattern"
                    }
                }
            }
            FdmQml_Param{ id:support_interface_pattern; objectName: "support_interface_pattern"
                fdmLabel: "Support Interface Pattern"
                fdmDescription: "The pattern with which the interface of the support with the model is printed."
                fdmType: "enum"
                fdmOptions:{
                    "lines": "Lines",
                    "grid": "Grid",
                    "triangles": "Triangles",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag"
                }
                fdmDefaultValue: "concentric"
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_interface_extruder_nr"
                FdmQml_Param{ id:support_roof_pattern; objectName: "support_roof_pattern"
                    fdmLabel: "Support Roof Pattern"
                    fdmDescription: "The pattern with which the roofs of the support are printed."
                    fdmType: "enum"
                    fdmOptions:{
                        "lines": "Lines",
                        "grid": "Grid",
                        "triangles": "Triangles",
                        "concentric": "Concentric",
                        "zigzag": "Zig Zag"
                    }
                    fdmDefaultValue: "concentric"
                    fdmValue: support_interface_pattern.fdmValue
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_roof_extruder_nr,support_interface_pattern"
                }
                FdmQml_Param{ id:support_bottom_pattern; objectName: "support_bottom_pattern"
                    fdmLabel: "Support Floor Pattern"
                    fdmDescription: "The pattern with which the floors of the support are printed."
                    fdmType: "enum"
                    fdmOptions:{
                        "lines": "Lines",
                        "grid": "Grid",
                        "triangles": "Triangles",
                        "concentric": "Concentric",
                        "zigzag": "Zig Zag"
                    }
                    fdmDefaultValue: "concentric"
                    fdmValue: support_interface_pattern.fdmValue
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_bottom_extruder_nr,support_interface_pattern"
                }
            }
            FdmQml_Param{ id:minimum_interface_area; objectName: "minimum_interface_area"
                fdmLabel: "Minimum Support Interface Area"
                fdmDescription: "Minimum area size for support interface polygons. Polygons which have an area smaller than this value will be printed as normal support."
                fdmUnit: "mm²"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(minimum_support_area.fdmValue)
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "support_interface_extruder_nr,minimum_support_area"
                FdmQml_Param{ id:minimum_roof_area; objectName: "minimum_roof_area"
                    fdmLabel: "Minimum Support Roof Area"
                    fdmDescription: "Minimum area size for the roofs of the support. Polygons which have an area smaller than this value will be printed as normal support."
                    fdmUnit: "mm²"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(minimum_interface_area.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(minimum_support_area.fdmValue)
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "support_roof_extruder_nr,minimum_support_area,minimum_interface_area"
                }
                FdmQml_Param{ id:minimum_bottom_area; objectName: "minimum_bottom_area"
                    fdmLabel: "Minimum Support Floor Area"
                    fdmDescription: "Minimum area size for the floors of the support. Polygons which have an area smaller than this value will be printed as normal support."
                    fdmUnit: "mm²"
                    fdmType: "float"
                    fdmDefaultValue: 1.0
                    fdmValue: Number(minimum_interface_area.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: Number(minimum_support_area.fdmValue)
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "support_bottom_extruder_nr,minimum_support_area,minimum_interface_area"
                }
            }
            FdmQml_Param{ id:support_interface_offset; objectName: "support_interface_offset"
                fdmLabel: "Support Interface Horizontal Expansion"
                fdmDescription: "Amount of offset applied to the support interface polygons."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMaximumValue: Number(support_offset.fdmValue)
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: support_interface_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_interface_extruder_nr,support_offset"
                FdmQml_Param{ id:support_roof_offset; objectName: "support_roof_offset"
                    fdmLabel: "Support Roof Horizontal Expansion"
                    fdmDescription: "Amount of offset applied to the roofs of the support."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.0
                    fdmValue: Number(support_interface_offset.fdmValue)
                    fdmMaximumValue: Number(support_offset.fdmValue)
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: support_roof_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_roof_extruder_nr,support_offset,support_interface_offset"
                }
                FdmQml_Param{ id:support_bottom_offset; objectName: "support_bottom_offset"
                    fdmLabel: "Support Floor Horizontal Expansion"
                    fdmDescription: "Amount of offset applied to the floors of the support."
                    fdmUnit: "mm"
                    fdmType: "float"
                    fdmDefaultValue: 0.0
                    fdmValue: Number(support_interface_offset.fdmValue)
                    fdmMaximumValue: Number(support_offset.fdmValue)
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: support_bottom_enable.fdmValue && (support_enable.fdmValue || support_meshes_present.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_bottom_extruder_nr,support_offset,support_interface_offset"
                }
            }
            FdmQml_Param{ id:support_interface_angles; objectName: "support_interface_angles"
                fdmLabel: "Support Interface Line Directions"
                fdmDescription: "A list of integer line directions to use. Elements from the list are used sequentially as the layers progress and when the end of the list is reached, it starts at the beginning again. The list items are separated by commas and the whole list is contained in square brackets. Default is an empty list which means use the default angles (alternates between 45 and 135 degrees if interfaces are quite thick or 90 degrees)."
                fdmType: "[int]"
                fdmDefaultValue: "[ ]"
                fdmLimitToExtruder: Number(support_interface_extruder_nr.fdmValue)
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_interface_enable.fdmValue && support_interface_pattern.fdmValue !== "concentric"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_interface_extruder_nr"
                FdmQml_Param{ id:support_roof_angles; objectName: "support_roof_angles"
                    fdmLabel: "Support Roof Line Directions"
                    fdmDescription: "A list of integer line directions to use. Elements from the list are used sequentially as the layers progress and when the end of the list is reached, it starts at the beginning again. The list items are separated by commas and the whole list is contained in square brackets. Default is an empty list which means use the default angles (alternates between 45 and 135 degrees if interfaces are quite thick or 90 degrees)."
                    fdmType: "[int]"
                    fdmDefaultValue: "[ ]"
                    fdmValue: support_interface_angles.fdmValue
                    fdmLimitToExtruder: Number(support_roof_extruder_nr.fdmValue)
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_roof_enable.fdmValue && support_roof_pattern.fdmValue !== "concentric"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_roof_extruder_nr,support_interface_angles"
                }
                FdmQml_Param{ id:support_bottom_angles; objectName: "support_bottom_angles"
                    fdmLabel: "Support Floor Line Directions"
                    fdmDescription: "A list of integer line directions to use. Elements from the list are used sequentially as the layers progress and when the end of the list is reached, it starts at the beginning again. The list items are separated by commas and the whole list is contained in square brackets. Default is an empty list which means use the default angles (alternates between 45 and 135 degrees if interfaces are quite thick or 90 degrees)."
                    fdmType: "[int]"
                    fdmDefaultValue: "[ ]"
                    fdmValue: support_interface_angles.fdmValue
                    fdmLimitToExtruder: Number(support_bottom_extruder_nr.fdmValue)
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_bottom_enable.fdmValue && support_bottom_pattern.fdmValue !== "concentric"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_bottom_extruder_nr,support_interface_angles"
                }
            }
            FdmQml_Param{ id:support_fan_enable; objectName: "support_fan_enable"
                fdmLabel: "Fan Speed Override"
                fdmDescription: "When enabled, the print cooling fan speed is altered for the skin regions immediately above the support."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: support_enable.fdmValue || support_meshes_present.fdmValue
                fdmSettablePerMesh: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_supported_skin_fan_speed; objectName: "support_supported_skin_fan_speed"
                fdmLabel: "Supported Skin Fan Speed"
                fdmDescription: "Percentage fan speed to use when printing the skin regions immediately above the support. Using a high fan speed can make the support easier to remove."
                fdmUnit: "%"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 100.0
                fdmDefaultValue: 100.0
                fdmType: "float"
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_fan_enable.fdmValue
                fdmSettablePerMesh: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_use_towers; objectName: "support_use_towers"
                fdmLabel: "Use Towers"
                fdmDescription: "Use specialized towers to support tiny overhang areas. These towers have a larger diameter than the region they support. Near the overhang the towers' diameter decreases, forming a roof."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue === "normal"
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tower_diameter; objectName: "support_tower_diameter"
                fdmLabel: "Tower Diameter"
                fdmDescription: "The diameter of a special tower."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 3.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 20.0
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue === "normal" && support_use_towers.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_tower_maximum_supported_diameter; objectName: "support_tower_maximum_supported_diameter"
                fdmLabel: "Maximum Tower-Supported Diameter"
                fdmDescription: "Maximum diameter in the X/Y directions of a small area which is to be supported by a specialized support tower."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 3.0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 20.0
                fdmMaximumValue: Number(support_tower_diameter.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue === "normal" && support_use_towers.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr,support_tower_diameter"
            }
            FdmQml_Param{ id:support_tower_roof_angle; objectName: "support_tower_roof_angle"
                fdmLabel: "Tower Roof Angle"
                fdmDescription: "The angle of a rooftop of a tower. A higher value results in pointed tower roofs, a lower value results in flattened tower roofs."
                fdmUnit: "°"
                fdmType: "int"
                fdmMinimumValue: 0
                fdmMaximumValue: 90
                fdmDefaultValue: 65
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue === "normal" && support_use_towers.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_mesh_drop_down; objectName: "support_mesh_drop_down"
                fdmLabel: "Drop Down Support Mesh"
                fdmDescription: "Make support everywhere below the support mesh, so that there's no overhang in the support mesh."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: support_mesh.fdmValue
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_meshes_present; objectName: "support_meshes_present"
                fdmLabel: "Scene Has Support Meshes"
                fdmDescription: "There are support meshes present in the scene. This setting is controlled by AnkerMake."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:platform_adhesion; objectName: "platform_adhesion"
            fdmLabel: "Build Plate Adhesion"
            fdmType: "category"
            fdmIcon: "Adhesion"
            fdmDescription: "Adhesion"
            FdmQml_Param{ id:prime_blob_enable; objectName: "prime_blob_enable"
                fdmLabel: "Enable Prime Blob"
                fdmDescription: "Whether to prime the filament with a blob before printing. Turning this setting on will ensure that the extruder will have material ready at the nozzle before printing. Printing Brim or Skirt can act like priming too, in which case turning this setting off saves some time."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmEnabled: false
                fdmValueWarning: print_sequence.fdmValue === "one_at_a_time"
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:extruder_prime_pos_x; objectName: "extruder_prime_pos_x"
                fdmLabel: "Extruder Prime X Position"
                fdmDescription: "The X coordinate of the position where the nozzle primes at the start of printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmMinimumValueWarning: machine_center_is_zero.fdmValue ? (Number(machine_width.fdmValue) / -2) : 0
                fdmMaximumValueWarning: machine_center_is_zero.fdmValue ? (Number(machine_width.fdmValue) / 2) : Number(machine_width.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmEnabled: false
                fdmAffectedById: "machine_width,machine_center_is_zero"
            }
            FdmQml_Param{ id:extruder_prime_pos_y; objectName: "extruder_prime_pos_y"
                fdmLabel: "Extruder Prime Y Position"
                fdmDescription: "The Y coordinate of the position where the nozzle primes at the start of printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.0
                fdmMinimumValueWarning: machine_center_is_zero.fdmValue ? (Number(machine_depth.fdmValue) / -2) : 0
                fdmMaximumValueWarning: machine_center_is_zero.fdmValue ? (Number(machine_depth.fdmValue) / 2) : Number(machine_depth.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmEnabled: false
                fdmAffectedById: "machine_depth,machine_center_is_zero"
            }
            FdmQml_Param{ id:adhesion_type; objectName: "adhesion_type"
                fdmLabel: "Build Plate Adhesion Type"
                fdmDescription: "Different options that help to improve both priming your extrusion and adhesion to the build plate. Brim adds a single layer flat area around the base of your model to prevent warping. Raft adds a thick grid with a roof below the model. Skirt is a line printed around the model, but not connected to the model."
                fdmType: "enum"
                fdmOptions:{
                    "skirt": "Skirt",
                    "brim": "Brim",
                    "raft": "Raft",
                    "none": "None"
                }
                fdmDefaultValue: "brim"
                fdmResolve: adhesion_type.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:adhesion_extruder_nr; objectName: "adhesion_extruder_nr"
                fdmLabel: "Build Plate Adhesion Extruder"
                fdmDescription: "The extruder train to use for printing the skirt/brim/raft. This is used in multi-extrusion."
                fdmType: "int"
                fdmDefaultValue: 0
                fdmValue: 0
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1 && (adhesion_type.fdmValue !== "none" || prime_tower_brim_enable.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:skirt_line_count; objectName: "skirt_line_count"
                fdmLabel: "Skirt Line Count"
                fdmDescription: "Multiple skirt lines help to prime your extrusion better for small models. Setting this to 0 will disable the skirt."
                fdmType: "int"
                fdmDefaultValue: 1
                fdmMinimumValue: 0
                fdmMaximumValueWarning: 10
                fdmEnabled: adhesion_type.fdmValue === "skirt"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr"
            }
            FdmQml_Param{ id:skirt_gap; objectName: "skirt_gap"
                fdmLabel: "Skirt Distance"
                fdmDescription: "The horizontal distance between the skirt and the first layer of the print.\nThis is the minimum distance. Multiple skirt lines will extend outward from this distance."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 3.0
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 10.0
                fdmEnabled: adhesion_type.fdmValue === "skirt"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:skirt_brim_minimal_length; objectName: "skirt_brim_minimal_length"
                fdmLabel: "Skirt/Brim Minimum Length"
                fdmDescription: "The minimum length of the skirt or brim. If this length is not reached by all skirt or brim lines together, more skirt or brim lines will be added until the minimum length is reached. Note: If the line count is set to 0 this is ignored."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 250.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 25.0
                fdmMaximumValueWarning: 2500.0
                fdmEnabled: adhesion_type.fdmValue === "skirt" || adhesion_type.fdmValue === "brim" || prime_tower_brim_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:brim_width; objectName: "brim_width"
                fdmLabel: "Brim Width"
                fdmDescription: "The distance from the model to the outermost brim line. A larger brim enhances adhesion to the build plate, but also reduces the effective print area."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 8.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 50.0
                fdmMaximumValue: 0.5 * Math.min(Number(machine_width.fdmValue), Number(machine_depth.fdmValue))
                fdmEnabled: adhesion_type.fdmValue === "brim" || prime_tower_brim_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_width,machine_depth,adhesion_extruder_nr"
                FdmQml_Param{ id:brim_line_count; objectName: "brim_line_count"
                    fdmLabel: "Brim Line Count"
                    fdmDescription: "The number of lines used for a brim. More brim lines enhance adhesion to the build plate, but also reduces the effective print area."
                    fdmType: "int"
                    fdmDefaultValue: 20
                    fdmMinimumValue: 0
                    fdmMaximumValueWarning: 50 / Number(skirt_brim_line_width.fdmValue)
                    fdmMaximumValue: 0.5 * Math.min(Number(machine_width.fdmValue), Number(machine_depth.fdmValue)) / Number(skirt_brim_line_width.fdmValue)
                    fdmValue:  Math.ceil(Number(brim_width.fdmValue) / (Number(skirt_brim_line_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100.0))
                    fdmEnabled: adhesion_type.fdmValue === "brim" || prime_tower_brim_enable.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "machine_width,machine_depth,skirt_brim_line_width,initial_layer_line_width_factor,adhesion_extruder_nr,brim_width"
                }
            }
            FdmQml_Param{ id:brim_gap; objectName: "brim_gap"
                fdmLabel: "Brim Distance"
                fdmDescription: "The horizontal distance between the first brim line and the outline of the first layer of the print. A small gap can make the brim easier to remove while still providing the thermal benefits."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(skirt_brim_line_width.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "brim"
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "skirt_brim_line_width,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:brim_replaces_support; objectName: "brim_replaces_support"
                fdmLabel: "Brim Replaces Support"
                fdmDescription: "Enforce brim to be printed around the model even if that space would otherwise be occupied by support. This replaces some regions of the first layer of support by brim regions."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: adhesion_type.fdmValue === "brim" && (support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:brim_outside_only; objectName: "brim_outside_only"
                fdmLabel: "Brim Only on Outside"
                fdmDescription: "Only print the brim on the outside of the model. This reduces the amount of brim you need to remove afterwards, while it doesn't reduce the bed adhesion that much."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: adhesion_type.fdmValue === "brim"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_margin; objectName: "raft_margin"
                fdmLabel: "Raft Extra Margin"
                fdmDescription: "If the raft is enabled, this is the extra raft area around the model which is also given a raft. Increasing this margin will create a stronger raft while using more material and leaving less area for your print."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 15.0
                fdmMinimumValueWarning: Number(raft_interface_line_width.fdmValue)
                fdmMaximumValueWarning: 20.0
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "adhesion_extruder_nr,raft_interface_line_width"
            }
            FdmQml_Param{ id:raft_smoothing; objectName: "raft_smoothing"
                fdmLabel: "Raft Smoothing"
                fdmDescription: "This setting controls how much inner corners in the raft outline are rounded. Inward corners are rounded to a semi circle with a radius equal to the value given here. This setting also removes holes in the raft outline which are smaller than such a circle."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 5.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(raft_interface_line_width.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "adhesion_extruder_nr,raft_interface_line_width"
            }
            FdmQml_Param{ id:raft_airgap; objectName: "raft_airgap"
                fdmLabel: "Raft Air Gap"
                fdmDescription: "The gap between the final raft layer and the first layer of the model. Only the first layer is raised by this amount to lower the bonding between the raft layer and the model. Makes it easier to peel off the raft."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.3
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:layer_0_z_overlap; objectName: "layer_0_z_overlap"
                fdmLabel: "Initial Layer Z Overlap"
                fdmDescription: "Make the first and second layer of the model overlap in the Z direction to compensate for the filament lost in the airgap. All models above the first model layer will be shifted down by this amount."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.22
                fdmValue: Number(raft_airgap.fdmValue) / 2
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(raft_airgap.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr,raft_airgap"
            }
            FdmQml_Param{ id:raft_surface_layers; objectName: "raft_surface_layers"
                fdmLabel: "Raft Top Layers"
                fdmDescription: "The number of top layers on top of the 2nd raft layer. These are fully filled layers that the model sits on. 2 layers result in a smoother top surface than 1."
                fdmType: "int"
                fdmDefaultValue: 2
                fdmMinimumValue: 0
                fdmMaximumValueWarning: 20
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_surface_thickness; objectName: "raft_surface_thickness"
                fdmLabel: "Raft Top Layer Thickness"
                fdmDescription: "Layer thickness of the top raft layers."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.1
                fdmValue: Number(layer_height.fdmValue)
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.04
                fdmMaximumValueWarning: 0.75 * Number(machine_nozzle_size.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,layer_height,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_surface_line_width; objectName: "raft_surface_line_width"
                fdmLabel: "Raft Top Line Width"
                fdmDescription: "Width of the lines in the top surface of the raft. These can be thin lines so that the top of the raft becomes smooth."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.4
                fdmValue: Number(line_width.fdmValue)
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue) * 0.1
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue) * 2
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,line_width,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_surface_line_spacing; objectName: "raft_surface_line_spacing"
                fdmLabel: "Raft Top Spacing"
                fdmDescription: "The distance between the raft lines for the top raft layers. The spacing should be equal to the line width, so that the surface is solid."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.4
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(raft_surface_line_width.fdmValue)
                fdmMaximumValueWarning: Number(raft_surface_line_width.fdmValue) * 3
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmValue: Number(raft_surface_line_width.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr,raft_surface_line_width"
            }
            FdmQml_Param{ id:raft_interface_thickness; objectName: "raft_interface_thickness"
                fdmLabel: "Raft Middle Thickness"
                fdmDescription: "Layer thickness of the middle raft layer."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.15
                fdmValue: Number(layer_height.fdmValue) * 1.5
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.04
                fdmMaximumValueWarning: 0.75 * Number(machine_nozzle_size.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,layer_height,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_interface_line_width; objectName: "raft_interface_line_width"
                fdmLabel: "Raft Middle Line Width"
                fdmDescription: "Width of the lines in the middle raft layer. Making the second layer extrude more causes the lines to stick to the build plate."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.7
                fdmValue: Number(line_width.fdmValue) * 2
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue) * 0.5
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue) * 3
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,line_width,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_interface_line_spacing; objectName: "raft_interface_line_spacing"
                fdmLabel: "Raft Middle Spacing"
                fdmDescription: "The distance between the raft lines for the middle raft layer. The spacing of the middle should be quite wide, while being dense enough to support the top raft layers."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.9
                fdmValue: Number(raft_interface_line_width.fdmValue) + 0.2
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(raft_interface_line_width.fdmValue)
                fdmMaximumValueWarning: 15.0
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr,raft_interface_line_width"
            }
            FdmQml_Param{ id:raft_base_thickness; objectName: "raft_base_thickness"
                fdmLabel: "Raft Base Thickness"
                fdmDescription: "Layer thickness of the base raft layer. This should be a thick layer which sticks firmly to the printer build plate."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.3
                fdmValue: Number(layer_height_0.fdmValue) * 1.2
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.04
                fdmMaximumValueWarning: 0.75 * Number(raft_base_line_width.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "layer_height_0,adhesion_extruder_nr,raft_base_line_width"
            }
            FdmQml_Param{ id:raft_base_line_width; objectName: "raft_base_line_width"
                fdmLabel: "Raft Base Line Width"
                fdmDescription: "Width of the lines in the base raft layer. These should be thick lines to assist in build plate adhesion."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 0.8
                fdmMinimumValue: 0.001
                fdmValue: Number(machine_nozzle_size.fdmValue) * 2
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue) * 0.5
                fdmMaximumValueWarning: Number(machine_nozzle_size.fdmValue) * 3
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_nozzle_size,adhesion_extruder_nr"
            }
            FdmQml_Param{ id:raft_base_line_spacing; objectName: "raft_base_line_spacing"
                fdmLabel: "Raft Base Line Spacing"
                fdmDescription: "The distance between the raft lines for the base raft layer. Wide spacing makes for easy removal of the raft from the build plate."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.6
                fdmValue: Number(raft_base_line_width.fdmValue) * 2
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(raft_base_line_width.fdmValue)
                fdmMaximumValueWarning: 100.0
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr,raft_base_line_width"
            }
            FdmQml_Param{ id:raft_speed; objectName: "raft_speed"
                fdmLabel: "Raft Print Speed"
                fdmDescription: "The speed at which the raft is printed."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.1
                fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                fdmMaximumValueWarning: 200.0
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmValue: Number(speed_print.fdmValue) / 60 * 30
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,speed_print,adhesion_extruder_nr"
                FdmQml_Param{ id:raft_surface_speed; objectName: "raft_surface_speed"
                    fdmLabel: "Raft Top Print Speed"
                    fdmDescription: "The speed at which the top raft layers are printed. These should be printed a bit slower, so that the nozzle can slowly smooth out adjacent surface lines."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmMinimumValue: 0.1
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 100.0
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmValue: Number(raft_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,adhesion_extruder_nr,raft_speed"
                }
                FdmQml_Param{ id:raft_interface_speed; objectName: "raft_interface_speed"
                    fdmLabel: "Raft Middle Print Speed"
                    fdmDescription: "The speed at which the middle raft layer is printed. This should be printed quite slowly, as the volume of material coming out of the nozzle is quite high."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 15.0
                    fdmValue: Number(raft_speed.fdmValue) * 0.75
                    fdmMinimumValue: 0.1
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 150.0
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,adhesion_extruder_nr,raft_speed"
                }
                FdmQml_Param{ id:raft_base_speed; objectName: "raft_base_speed"
                    fdmLabel: "Raft Base Print Speed"
                    fdmDescription: "The speed at which the base raft layer is printed. This should be printed quite slowly, as the volume of material coming out of the nozzle is quite high."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 15.0
                    fdmMinimumValue: 0.1
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 200.0
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmValue: 0.75 * Number(raft_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,adhesion_extruder_nr,raft_speed"
                }
            }
            FdmQml_Param{ id:raft_acceleration; objectName: "raft_acceleration"
                fdmLabel: "Raft Print Acceleration"
                fdmDescription: "The acceleration with which the raft is printed."
                fdmUnit: "mm/s²"
                fdmType: "float"
                fdmDefaultValue: 3000.0
                fdmMinimumValue: 0.1
                fdmMinimumValueWarning: 100.0
                fdmMaximumValueWarning: 10000.0
                fdmValue: Number(acceleration_print.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft" && acceleration_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "acceleration_print,adhesion_extruder_nr"
                FdmQml_Param{ id:raft_surface_acceleration; objectName: "raft_surface_acceleration"
                    fdmLabel: "Raft Top Print Acceleration"
                    fdmDescription: "The acceleration with which the top raft layers are printed."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmDefaultValue: 3000.0
                    fdmValue: Number(raft_acceleration.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 10000.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && acceleration_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_acceleration"
                }
                FdmQml_Param{ id:raft_interface_acceleration; objectName: "raft_interface_acceleration"
                    fdmLabel: "Raft Middle Print Acceleration"
                    fdmDescription: "The acceleration with which the middle raft layer is printed."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmDefaultValue: 3000.0
                    fdmValue: Number(raft_acceleration.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 10000.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && acceleration_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_acceleration"
                }
                FdmQml_Param{ id:raft_base_acceleration; objectName: "raft_base_acceleration"
                    fdmLabel: "Raft Base Print Acceleration"
                    fdmDescription: "The acceleration with which the base raft layer is printed."
                    fdmUnit: "mm/s²"
                    fdmType: "float"
                    fdmDefaultValue: 3000.0
                    fdmValue: Number(raft_acceleration.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 100.0
                    fdmMaximumValueWarning: 10000.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && acceleration_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_acceleration"
                }
            }
            FdmQml_Param{ id:raft_jerk; objectName: "raft_jerk"
                fdmLabel: "Raft Print Jerk"
                fdmDescription: "The jerk with which the raft is printed."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 5.0
                fdmMaximumValueWarning: 50.0
                fdmValue: Number(jerk_print.fdmValue)
                fdmEnabled: adhesion_type.fdmValue === "raft" && jerk_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "jerk_print,adhesion_extruder_nr"
                FdmQml_Param{ id:raft_surface_jerk; objectName: "raft_surface_jerk"
                    fdmLabel: "Raft Top Print Jerk"
                    fdmDescription: "The jerk with which the top raft layers are printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmValue: Number(raft_jerk.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 5.0
                    fdmMaximumValueWarning: 100.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && jerk_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_jerk"
                }
                FdmQml_Param{ id:raft_interface_jerk; objectName: "raft_interface_jerk"
                    fdmLabel: "Raft Middle Print Jerk"
                    fdmDescription: "The jerk with which the middle raft layer is printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmValue: Number(raft_jerk.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 5.0
                    fdmMaximumValueWarning: 50.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && jerk_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_jerk"
                }
                FdmQml_Param{ id:raft_base_jerk; objectName: "raft_base_jerk"
                    fdmLabel: "Raft Base Print Jerk"
                    fdmDescription: "The jerk with which the base raft layer is printed."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 20.0
                    fdmValue: Number(raft_jerk.fdmValue)
                    fdmMinimumValue: 0.0
                    fdmMinimumValueWarning: 5.0
                    fdmMaximumValueWarning: 50.0
                    fdmEnabled: adhesion_type.fdmValue === "raft" && jerk_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_jerk"
                }
            }
            FdmQml_Param{ id:raft_fan_speed; objectName: "raft_fan_speed"
                fdmLabel: "Raft Fan Speed"
                fdmDescription: "The fan speed for the raft."
                fdmUnit: "%"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValue: 100.0
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmEnabled: adhesion_type.fdmValue === "raft"
                fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                fdmAffectedById: "adhesion_extruder_nr"
                FdmQml_Param{ id:raft_surface_fan_speed; objectName: "raft_surface_fan_speed"
                    fdmLabel: "Raft Top Fan Speed"
                    fdmDescription: "The fan speed for the top raft layers."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 0.0
                    fdmValue: Number(raft_fan_speed.fdmValue)
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_fan_speed"
                }
                FdmQml_Param{ id:raft_interface_fan_speed; objectName: "raft_interface_fan_speed"
                    fdmLabel: "Raft Middle Fan Speed"
                    fdmDescription: "The fan speed for the middle raft layer."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 0.0
                    fdmValue: Number(raft_fan_speed.fdmValue)
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_fan_speed"
                }
                FdmQml_Param{ id:raft_base_fan_speed; objectName: "raft_base_fan_speed"
                    fdmLabel: "Raft Base Fan Speed"
                    fdmDescription: "The fan speed for the base raft layer."
                    fdmUnit: "%"
                    fdmType: "float"
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: 100.0
                    fdmDefaultValue: 0.0
                    fdmValue: Number(raft_fan_speed.fdmValue)
                    fdmEnabled: adhesion_type.fdmValue === "raft"
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmLimitToExtruder: Number(adhesion_extruder_nr.fdmValue)
                    fdmAffectedById: "adhesion_extruder_nr,raft_fan_speed"
                }
            }
        }
        FdmQml_Category{ id:dual; objectName: "dual"
            fdmLabel: "Dual Extrusion"
            fdmType: "category"
            fdmIcon: "DualExtrusion"
            fdmDescription: "Settings used for printing with multiple extruders."
            FdmQml_Param{ id:prime_tower_enable; objectName: "prime_tower_enable"
                fdmLabel: "Enable Prime Tower"
                fdmDescription: "Print a tower next to the print which serves to prime the material after each nozzle switch."
                fdmType: "bool"
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                fdmDefaultValue: false
                fdmResolve: (Number(extruders_enabled_count.fdmValue) > 1) && prime_tower_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "extruders_enabled_count"
            }
            FdmQml_Param{ id:prime_tower_size; objectName: "prime_tower_size"
                fdmLabel: "Prime Tower Size"
                fdmDescription: "The width of the prime tower."
                fdmType: "float"
                fdmUnit: "mm"
                fdmEnabled: prime_tower_enable.fdmValue
                fdmDefaultValue: 20.0
                fdmResolve: Number(prime_tower_size.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValue: Math.min(0.5 * Number(machine_width.fdmValue), 0.5 * Number(machine_depth.fdmValue))
                fdmMinimumValueWarning: Number(prime_tower_line_width.fdmValue) * 2
                fdmMaximumValueWarning: 42.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_width,machine_depth,prime_tower_line_width"
            }
            FdmQml_Param{ id:prime_tower_min_volume; objectName: "prime_tower_min_volume"
                fdmLabel: "Prime Tower Minimum Volume"
                fdmDescription: "The minimum volume for each layer of the prime tower in order to purge enough material."
                fdmUnit: "mm³"
                fdmType: "float"
                fdmDefaultValue: 6.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: (prime_tower_size.fdmValue * 0.5) ** 2 * 3.14159 * layer_height.fdmValue
                fdmEnabled: prime_tower_enable.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:prime_tower_position_x; objectName: "prime_tower_position_x"
                fdmLabel: "Prime Tower X Position"
                fdmDescription: "The x coordinate of the position of the prime tower."
                fdmType: "float"
                fdmUnit: "mm"
                fdmEnabled: prime_tower_enable.fdmValue
                fdmDefaultValue: 200.0
                fdmValue: Number(machine_width.fdmValue) - Math.max((adhesion_type.fdmValue === "brim" || (prime_tower_brim_enable.fdmValue && adhesion_type.fdmValue !== "raft")) ? (Number(brim_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100) : ( (adhesion_type.fdmValue === "raft" ? Number(raft_margin.fdmValue) : (adhesion_type.fdmValue === "skirt" ? Number(skirt_gap.fdmValue) : 0)), Number(travel_avoid_distance.fdmValue))) - Number(support_offset.fdmValue) - Number(skirt_brim_line_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100 - (draft_shield_enabled.fdmValue ? Number(draft_shield_dist.fdmValue) : 0) - Number(machine_nozzle_offset_x.fdmValue) - 1
                fdmMaximumValue: machine_center_is_zero.fdmValue ? (Number(machine_width.fdmValue) / 2) : Number(machine_width.fdmValue)
                fdmMinimumValue: machine_center_is_zero.fdmValue ? (Number(prime_tower_size.fdmValue) - Number(machine_width.fdmValue) / 2) : Number(prime_tower_size.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_width,machine_center_is_zero,machine_nozzle_offset_x,skirt_brim_line_width,initial_layer_line_width_factor,travel_avoid_distance,support_offset,adhesion_type,skirt_gap,brim_width,raft_margin,prime_tower_size,prime_tower_brim_enable,draft_shield_enabled,draft_shield_dist"
            }
            FdmQml_Param{ id:prime_tower_position_y; objectName: "prime_tower_position_y"
                fdmLabel: "Prime Tower Y Position"
                fdmDescription: "The y coordinate of the position of the prime tower."
                fdmType: "float"
                fdmUnit: "mm"
                fdmEnabled: prime_tower_enable.fdmValue
                fdmDefaultValue: 200.0
                fdmValue: Number(machine_depth.fdmValue) - Number(prime_tower_size.fdmValue) - Math.max((adhesion_type.fdmValue === "brim" || (prime_tower_brim_enable.fdmValue && adhesion_type.fdmValue !== "raft")) ? (Number(brim_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100) : ( (adhesion_type.fdmValue === "raft" ? Number(raft_margin.fdmValue) : (adhesion_type.fdmValue === "skirt" ? Number(skirt_gap.fdmValue) : 0)), Number(travel_avoid_distance.fdmValue))) - Number(support_offset.fdmValue) - Number(skirt_brim_line_width.fdmValue) * Number(initial_layer_line_width_factor.fdmValue) / 100 - (draft_shield_enabled.fdmValue ? Number(draft_shield_dist.fdmValue) : 0) - Number(machine_nozzle_offset_y.fdmValue) - 3
                fdmMaximumValue: machine_center_is_zero.fdmValue ? (Number(machine_depth.fdmValue) / 2 - Number(prime_tower_size.fdmValue)) : (Number(machine_depth.fdmValue) - Number(prime_tower_size.fdmValue))
                fdmMinimumValue: machine_center_is_zero.fdmValue ? (Number(machine_depth.fdmValue) / -2) : 0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "machine_depth,machine_center_is_zero,machine_nozzle_offset_y,skirt_brim_line_width,initial_layer_line_width_factor,travel_avoid_distance,support_offset,adhesion_type,skirt_gap,brim_width,raft_margin,prime_tower_size,prime_tower_brim_enable,draft_shield_enabled,draft_shield_dist"
            }
            FdmQml_Param{ id:prime_tower_wipe_enabled; objectName: "prime_tower_wipe_enabled"
                fdmLabel: "Wipe Inactive Nozzle on Prime Tower"
                fdmDescription: "After printing the prime tower with one nozzle, wipe the oozed material from the other nozzle off on the prime tower."
                fdmType: "bool"
                fdmEnabled: prime_tower_enable.fdmValue
                fdmDefaultValue: true
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:prime_tower_brim_enable; objectName: "prime_tower_brim_enable"
                fdmLabel: "Prime Tower Brim"
                fdmDescription: "Prime-towers might need the extra adhesion afforded by a brim even if the model doesn't. Presently can't be used with the 'Raft' adhesion-type."
                fdmType: "bool"
                fdmEnabled: prime_tower_enable.fdmValue && (adhesion_type.fdmValue !== "raft")
                fdmResolve: prime_tower_enable.fdmValue && ((adhesion_type.fdmValue === "none" || adhesion_type.fdmValue ===  "skirt"))
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: "adhesion_type,prime_tower_enable"
            }
            FdmQml_Param{ id:ooze_shield_enabled; objectName: "ooze_shield_enabled"
                fdmLabel: "Enable Ooze Shield"
                fdmDescription: "Enable exterior ooze shield. This will create a shell around the model which is likely to wipe a second nozzle if it's at the same height as the first nozzle."
                fdmType: "bool"
                fdmResolve: ooze_shield_enabled.fdmValue
                fdmEnabled: Number(extruders_enabled_count.fdmValue) > 1
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:ooze_shield_angle; objectName: "ooze_shield_angle"
                fdmLabel: "Ooze Shield Angle"
                fdmDescription: "The maximum angle a part in the ooze shield will have. With 0 degrees being vertical, and 90 degrees being horizontal. A smaller angle leads to less failed ooze shields, but more material."
                fdmType: "float"
                fdmUnit: "°"
                fdmEnabled: ooze_shield_enabled.fdmValue
                fdmDefaultValue: 60.0
                fdmResolve: Number(ooze_shield_angle.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValue: 90.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:ooze_shield_dist; objectName: "ooze_shield_dist"
                fdmLabel: "Ooze Shield Distance"
                fdmDescription: "Distance of the ooze shield from the print, in the X/Y directions."
                fdmType: "float"
                fdmUnit: "mm"
                fdmEnabled: ooze_shield_enabled.fdmValue
                fdmDefaultValue: 2.0
                fdmResolve: Number(ooze_shield_dist.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 20.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:switch_extruder_retraction_amount; objectName: "switch_extruder_retraction_amount"
                fdmLabel: "Nozzle Switch Retraction Distance"
                fdmDescription: "The amount of retraction when switching extruders. Set to 0 for no retraction at all. This should generally be the same as the length of the heat zone."
                fdmType: "float"
                fdmUnit: "mm"
                fdmEnabled: retraction_enable.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                fdmDefaultValue: 20.0
                fdmValue: Number(machine_heat_zone_length.fdmValue)
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 100.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_heat_zone_length"
            }
            FdmQml_Param{ id:switch_extruder_retraction_speeds; objectName: "switch_extruder_retraction_speeds"
                fdmLabel: "Nozzle Switch Retraction Speed"
                fdmDescription: "The speed at which the filament is retracted. A higher retraction speed works better, but a very high retraction speed can lead to filament grinding."
                fdmType: "float"
                fdmUnit: "mm/s"
                fdmEnabled: retraction_enable.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.1
                fdmMinimumValueWarning: 1.0
                fdmMaximumValue: retraction_enable.fdmValue ? Number(machine_max_feedrate_e.fdmValue) : Infinity
                fdmMaximumValueWarning: 70.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e,retraction_enable"
                FdmQml_Param{ id:switch_extruder_retraction_speed; objectName: "switch_extruder_retraction_speed"
                    fdmLabel: "Nozzle Switch Retract Speed"
                    fdmDescription: "The speed at which the filament is retracted during a nozzle switch retract."
                    fdmType: "float"
                    fdmUnit: "mm/s"
                    fdmEnabled: retraction_enable.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                    fdmDefaultValue: 20.0
                    fdmValue: Number(switch_extruder_retraction_speeds.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 1.0
                    fdmMaximumValue: retraction_enable.fdmValue ? Number(machine_max_feedrate_e.fdmValue) : Infinity
                    fdmMaximumValueWarning: 70.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_max_feedrate_e,retraction_enable,switch_extruder_retraction_speeds"
                }
                FdmQml_Param{ id:switch_extruder_prime_speed; objectName: "switch_extruder_prime_speed"
                    fdmLabel: "Nozzle Switch Prime Speed"
                    fdmDescription: "The speed at which the filament is pushed back after a nozzle switch retraction."
                    fdmType: "float"
                    fdmUnit: "mm/s"
                    fdmEnabled: retraction_enable.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                    fdmDefaultValue: 20.0
                    fdmValue: Number(switch_extruder_retraction_speeds.fdmValue)
                    fdmMinimumValue: 0.1
                    fdmMinimumValueWarning: 1.0
                    fdmMaximumValue: retraction_enable.fdmValue ? Number(machine_max_feedrate_e.fdmValue) : Infinity
                    fdmMaximumValueWarning: 70.0
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_max_feedrate_e,retraction_enable,switch_extruder_retraction_speeds"
                }
            }
            FdmQml_Param{ id:switch_extruder_extra_prime_amount; objectName: "switch_extruder_extra_prime_amount"
                fdmLabel: "Nozzle Switch Extra Prime Amount"
                fdmDescription: "Extra material to prime after nozzle switching."
                fdmType: "float"
                fdmUnit: "mm³"
                fdmDefaultValue: 0.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 100.0
                fdmEnabled: retraction_enable.fdmValue && Number(extruders_enabled_count.fdmValue) > 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:meshfix; objectName: "meshfix"
            fdmLabel: "Mesh Fixes"
            fdmType: "category"
            fdmIcon: "Bandage"
            fdmDescription: "Make the meshes more suited for 3D printing."
            FdmQml_Param{ id:meshfix_union_all; objectName: "meshfix_union_all"
                fdmLabel: "Union Overlapping Volumes"
                fdmDescription: "Ignore the internal geometry arising from overlapping volumes within a mesh and print the volumes as one. This may cause unintended internal cavities to disappear."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmValue: magic_mesh_surface_mode.fdmValue !== "surface"
                fdmSettablePerMesh: true
                fdmAffectedById: "magic_mesh_surface_mode"
            }
            FdmQml_Param{ id:meshfix_union_all_remove_holes; objectName: "meshfix_union_all_remove_holes"
                fdmLabel: "Remove All Holes"
                fdmDescription: "Remove the holes in each layer and keep only the outside shape. This will ignore any invisible internal geometry. However, it also ignores layer holes which can be viewed from above or below."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:meshfix_extensive_stitching; objectName: "meshfix_extensive_stitching"
                fdmLabel: "Extensive Stitching"
                fdmDescription: "Extensive stitching tries to stitch up open holes in the mesh by closing the hole with touching polygons. This option can introduce a lot of processing time."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:meshfix_keep_open_polygons; objectName: "meshfix_keep_open_polygons"
                fdmLabel: "Keep Disconnected Faces"
                fdmDescription: "Normally AnkerMake tries to stitch up small holes in the mesh and remove parts of a layer with big holes. Enabling this option keeps those parts which cannot be stitched. This option should be used as a last resort option when everything else fails to produce proper g-code."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:multiple_mesh_overlap; objectName: "multiple_mesh_overlap"
                fdmLabel: "Merged Meshes Overlap"
                fdmDescription: "Make meshes which are touching each other overlap a bit. This makes them bond together better."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.15
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 1.0
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:carve_multiple_volumes; objectName: "carve_multiple_volumes"
                fdmLabel: "Remove Mesh Intersection"
                fdmDescription: "Remove areas where multiple meshes are overlapping with each other. This may be used if merged dual material objects overlap with each other."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmValue: Number(extruders_enabled_count.fdmValue) > 1
                fdmEnabled: magic_mesh_surface_mode.fdmValue !== "surface"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmAffectedById: "extruders_enabled_count"
            }
            FdmQml_Param{ id:alternate_carve_order; objectName: "alternate_carve_order"
                fdmLabel: "Alternate Mesh Removal"
                fdmDescription: "Switch to which mesh intersecting volumes will belong with every layer, so that the overlapping meshes become interwoven. Turning this setting off will cause one of the meshes to obtain all of the volume in the overlap, while it is removed from the other meshes."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled: false //carve_multiple_volumes.fdmValue && all(p !== "surface" for p in magic_mesh_surface_mode.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:remove_empty_first_layers; objectName: "remove_empty_first_layers"
                fdmLabel: "Remove Empty First Layers"
                fdmDescription: "Remove empty layers beneath the first printed layer if they are present. Disabling this setting can cause empty first layers if the Slicing Tolerance setting is set to Exclusive or Middle."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmEnabled:  !(support_enable.fdmValue || support_meshes_present.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:blackmagic; objectName: "blackmagic"
            fdmLabel: "Special Modes"
            fdmType: "category"
            fdmIcon: "BlackMagic"
            fdmDescription: "Non-traditional ways to print your models."
            FdmQml_Param{ id:print_sequence; objectName: "print_sequence"
                fdmLabel: "Print Sequence"
                fdmDescription: "Whether to print all models one layer at a time or to wait for one model to finish, before moving on to the next. One at a time mode is possible if a) only one extruder is enabled and b) all models are separated in such a way that the whole print head can move in between and all models are lower than the distance between the nozzle and the X/Y axes."
                fdmType: "enum"
                fdmOptions:{
                    "all_at_once": "All at Once",
                    "one_at_a_time": "One at a Time"
                }
                fdmDefaultValue: "all_at_once"
                fdmEnabled: Number(extruders_enabled_count.fdmValue) === 1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:infill_mesh; objectName: "infill_mesh"
                fdmLabel: "Infill Mesh"
                fdmDescription: "Use this mesh to modify the infill of other meshes with which it overlaps. Replaces infill regions of other meshes with regions for this mesh. It's suggested to only print one Wall and no Top/Bottom Skin for this mesh."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:infill_mesh_order; objectName: "infill_mesh_order"
                fdmLabel: "Mesh Processing Rank"
                fdmDescription: "Determines the priority of this mesh when considering multiple overlapping infill meshes. Areas where multiple infill meshes overlap will take on the settings of the mesh with the highest rank. An infill mesh with a higher rank will modify the infill of infill meshes with lower rank and normal meshes."
                fdmDefaultValue: 0
                fdmValue: infill_mesh.fdmValue ? 1 : 0
                fdmMinimumValueWarning: 1
                fdmMaximumValueWarning: 50
                fdmType: "int"
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: "infill_mesh"
            }
            FdmQml_Param{ id:cutting_mesh; objectName: "cutting_mesh"
                fdmLabel: "Cutting Mesh"
                fdmDescription: "Limit the volume of this mesh to within other meshes. You can use this to make certain areas of one mesh print with different settings and with a whole different extruder."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mold_enabled; objectName: "mold_enabled"
                fdmLabel: "Mold"
                fdmDescription: "Print models as a mold, which can be cast in order to get a model which resembles the models on the build plate."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mold_width; objectName: "mold_width"
                fdmLabel: "Minimal Mold Width"
                fdmDescription: "The minimal distance between the ouside of the mold and the outside of the model."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValueWarning: Number(wall_line_width_0.fdmValue) * 2
                fdmMaximumValueWarning: 100.0
                fdmDefaultValue: 5.0
                fdmSettablePerMesh: true
                fdmEnabled: mold_enabled.fdmValue
                fdmAffectedById: "wall_line_width_0"
            }
            FdmQml_Param{ id:mold_roof_height; objectName: "mold_roof_height"
                fdmLabel: "Mold Roof Height"
                fdmDescription: "The height above horizontal parts in your model which to print mold."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 5.0
                fdmDefaultValue: 0.5
                fdmSettablePerMesh: true
                fdmEnabled: mold_enabled.fdmValue
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mold_angle; objectName: "mold_angle"
                fdmLabel: "Mold Angle"
                fdmDescription: "The angle of overhang of the outer walls created for the mold. 0° will make the outer shell of the mold vertical, while 90° will make the outside of the model follow the contour of the model."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: -89.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: Number(support_angle.fdmValue)
                fdmMaximumValue: 90.0
                fdmDefaultValue: 40.0
                fdmSettablePerMesh: true
                fdmEnabled: mold_enabled.fdmValue
                fdmAffectedById: "support_angle"
            }
            FdmQml_Param{ id:support_mesh; objectName: "support_mesh"
                fdmLabel: "Support Mesh"
                fdmDescription: "Use this mesh to specify support areas. This can be used to generate support structure."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:anti_overhang_mesh; objectName: "anti_overhang_mesh"
                fdmLabel: "Anti Overhang Mesh"
                fdmDescription: "Use this mesh to specify where no part of the model should be detected as overhang. This can be used to remove unwanted support structure."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmSettableGlobally: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:magic_mesh_surface_mode; objectName: "magic_mesh_surface_mode"
                fdmLabel: "Surface Mode"
                fdmDescription: "Treat the model as a surface only, a volume, or volumes with loose surfaces. The normal print mode only prints enclosed volumes. \"Surface\" prints a single wall tracing the mesh surface with no infill and no top/bottom skin. \"Both\" prints enclosed volumes like normal and any remaining polygons as surfaces."
                fdmType: "enum"
                fdmOptions:{
                    "normal": "Normal",
                    "surface": "Surface",
                    "both": "Both"
                }
                fdmDefaultValue: "normal"
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:magic_spiralize; objectName: "magic_spiralize"
                fdmLabel: "Spiralize Outer Contour"
                fdmDescription: "Spiralize smooths out the Z move of the outer edge. This will create a steady Z increase over the whole print. This feature turns a solid model into a single walled print with a solid bottom. This feature should only be enabled when each layer only contains a single part."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
                FdmQml_Param{ id:magic_spiralize_print_speed; objectName: "magic_spiralize_print_speed"
                    fdmLabel: "Printing Speed of Spiralize Outer Contour"
                    fdmDescription: "Printing Speed of Spiralize Outer Contour"
                    fdmType: "float"
                    fdmDefaultValue: 30.0
                    fdmEnabled: magic_spiralize.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
                FdmQml_Param{ id:smooth_spiralized_contours; objectName: "smooth_spiralized_contours"
                    fdmLabel: "Smooth Spiralized Contours"
                    fdmDescription: "Smooth the spiralized contours to reduce the visibility of the Z seam (the Z seam should be barely visible on the print but will still be visible in the layer view). Note that smoothing will tend to blur fine surface details."
                    fdmType: "bool"
                    fdmDefaultValue: true
                    fdmEnabled: magic_spiralize.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmAffectedById: ""
                }
            }
        }
        FdmQml_Category{ id:experimental; objectName: "experimental"
            fdmLabel: "Experimental"
            fdmType: "category"
            fdmIcon: "Experiment"
            fdmDescription: "Features that haven't completely been fleshed out yet."
            FdmQml_Param{ id:slicing_tolerance; objectName: "slicing_tolerance"
                fdmLabel: "Slicing Tolerance"
                fdmDescription: "Vertical tolerance in the sliced layers. The contours of a layer are normally generated by taking cross sections through the middle of each layer's thickness (Middle). Alternatively each layer can have the areas which fall inside of the volume throughout the entire thickness of the layer (Exclusive) or a layer has the areas which fall inside anywhere within the layer (Inclusive). Inclusive retains the most details, Exclusive makes for the best fit and Middle stays closest to the original surface."
                fdmType: "enum"
                fdmOptions:{
                    "middle": "Middle",
                    "exclusive": "Exclusive",
                    "inclusive": "Inclusive"
                }
                fdmDefaultValue: "middle"
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:roofing_line_width; objectName: "roofing_line_width"
                fdmLabel: "Top Surface Skin Line Width"
                fdmDescription: "Width of a single line of the areas at the top of the print."
                fdmUnit: "mm"
                fdmMinimumValue: 0.001
                fdmMinimumValueWarning: 0.1 + 0.4 * Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 2 * Number(machine_nozzle_size.fdmValue)
                fdmDefaultValue: 0.4
                fdmType: "float"
                fdmValue: Number(skin_line_width.fdmValue)
                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmEnabled: Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                fdmAffectedById: "machine_nozzle_size,skin_line_width,roofing_extruder_nr"
            }
            FdmQml_Param{ id:roofing_pattern; objectName: "roofing_pattern"
                fdmLabel: "Top Surface Skin Pattern"
                fdmDescription: "The pattern of the top most layers."
                fdmType: "enum"
                fdmOptions:{
                    "lines": "Lines",
                    "concentric": "Concentric",
                    "zigzag": "Zig Zag"
                }
                fdmDefaultValue: "lines"
                fdmValue: top_bottom_pattern.fdmValue
                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmEnabled: Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                fdmAffectedById: "roofing_extruder_nr,top_bottom_pattern"
            }
            FdmQml_Param{ id:roofing_monotonic; objectName: "roofing_monotonic"
                fdmLabel: "Monotonic Top Surface Order"
                fdmDescription: "Print top surface lines in an ordering that causes them to always overlap with adjacent lines in a single direction. This takes slightly more time to print, but makes flat surfaces look more consistent."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmValue: skin_monotonic.fdmValue
                fdmEnabled: Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0 && roofing_pattern.fdmValue !== "concentric"
                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "roofing_extruder_nr,skin_monotonic"
            }
            FdmQml_Param{ id:roofing_angles; objectName: "roofing_angles"
                fdmLabel: "Top Surface Skin Line Directions"
                fdmDescription: "A list of integer line directions to use when the top surface skin layers use the lines or zig zag pattern. Elements from the list are used sequentially as the layers progress and when the end of the list is reached, it starts at the beginning again. The list items are separated by commas and the whole list is contained in square brackets. Default is an empty list which means use the traditional default angles (45 and 135 degrees)."
                fdmType: "[int]"
                fdmDefaultValue: "[ ]"
                fdmValue: skin_angles.fdmValue
                fdmEnabled: roofing_pattern.fdmValue !== "concentric" && Number(roofing_layer_count.fdmValue) > 0 && Number(top_layers.fdmValue) > 0
                fdmLimitToExtruder: Number(roofing_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "roofing_extruder_nr,skin_angles"
            }
            FdmQml_Param{ id:infill_enable_travel_optimization; objectName: "infill_enable_travel_optimization"
                fdmLabel: "Infill Travel Optimization"
                fdmDescription: "When enabled, the order in which the infill lines are printed is optimized to reduce the distance travelled. The reduction in travel time achieved very much depends on the model being sliced, infill pattern, density, etc. Note that, for some models that have many small areas of infill, the time to slice the model may be greatly increased."
                fdmType: "bool"
                fdmEnabled: retraction_combing.fdmValue !== "off"
                fdmDefaultValue: false
                fdmSettablePerMesh: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_flow_dependent_temperature; objectName: "material_flow_dependent_temperature"
                fdmLabel: "Auto Temperature"
                fdmDescription: "Change the temperature for each layer automatically with the average flow speed of that layer."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: machine_nozzle_temp_enabled.fdmValue && false 
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:material_flow_temp_graph; objectName: "material_flow_temp_graph"
                fdmLabel: "Flow Temperature Graph"
                fdmDescription: "Data linking material flow (in mm3 per second) to temperature (degrees Celsius)."
                fdmUnit: "[[mm³,°C]]"
                fdmType: "str"
                fdmDefaultValue: "[[3.5,200],[7.0,240]]"
                fdmEnabled:  false && machine_nozzle_temp_enabled.fdmValue && material_flow_dependent_temperature.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:minimum_polygon_circumference; objectName: "minimum_polygon_circumference"
                fdmLabel: "Minimum Polygon Circumference"
                fdmDescription: "Polygons in sliced layers that have a circumference smaller than this amount will be filtered out. Lower values lead to higher resolution mesh at the cost of slicing time. It is meant mostly for high resolution SLA printers and very tiny 3D models with a lot of details."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.05
                fdmMaximumValueWarning: 1.0
                fdmSettablePerMesh: true
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:support_skip_some_zags; objectName: "support_skip_some_zags"
                fdmLabel: "Break Up Support In Chunks"
                fdmDescription: "Skip some support line connections to make the support structure easier to break away. This setting is applicable to the Zig Zag support infill pattern."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: (support_enable.fdmValue  || support_meshes_present.fdmValue) && support_pattern.fdmValue === "zigzag"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_skip_zag_per_mm; objectName: "support_skip_zag_per_mm"
                fdmLabel: "Support Chunk Size"
                fdmDescription: "Leave out a connection between support lines once every N millimeter to make the support structure easier to break away."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(support_line_distance.fdmValue)
                fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_pattern.fdmValue === "zigzag" && support_skip_some_zags.fdmValue
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr,support_line_distance"
                FdmQml_Param{ id:support_zag_skip_count; objectName: "support_zag_skip_count"
                    fdmLabel: "Support Chunk Line Count"
                    fdmDescription: "Skip one in every N connection lines to make the support structure easier to break away."
                    fdmType: "int"
                    fdmDefaultValue: 5
                    fdmValue: Number(support_line_distance.fdmValue) === 0 ? 0 : (Math.round(Number(support_skip_zag_per_mm.fdmValue) / Number(support_line_distance.fdmValue)))
                    fdmMinimumValue: 1
                    fdmMinimumValueWarning: 3
                    fdmEnabled: (support_enable.fdmValue || support_meshes_present.fdmValue) && support_pattern.fdmValue === "zigzag" && support_skip_some_zags.fdmValue
                    fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "support_infill_extruder_nr,support_line_distance,support_skip_zag_per_mm"
                }
            }
            FdmQml_Param{ id:draft_shield_enabled; objectName: "draft_shield_enabled"
                fdmLabel: "Enable Draft Shield"
                fdmDescription: "This will create a wall around the model, which traps (hot) air and shields against exterior airflow. Especially useful for materials which warp easily."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:draft_shield_dist; objectName: "draft_shield_dist"
                fdmLabel: "Draft Shield X/Y Distance"
                fdmDescription: "Distance of the draft shield from the print, in the X/Y directions."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 10.0
                fdmDefaultValue: 10.0
                fdmResolve: Number(draft_shield_dist.fdmValue)
                fdmEnabled: draft_shield_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:draft_shield_height_limitation; objectName: "draft_shield_height_limitation"
                fdmLabel: "Draft Shield Limitation"
                fdmDescription: "Set the height of the draft shield. Choose to print the draft shield at the full height of the model or at a limited height."
                fdmType: "enum"
                fdmOptions:{
                    "full": "Full",
                    "limited": "Limited"
                }
                fdmDefaultValue: "full"
                //fdmResolve: "full" in draft_shield_height_limitation.fdmValue ? "full" : "limited"
                fdmEnabled: draft_shield_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:draft_shield_height; objectName: "draft_shield_height"
                fdmLabel: "Draft Shield Height"
                fdmDescription: "Height limitation of the draft shield. Above this height no draft shield will be printed."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 9999.0
                fdmDefaultValue: 10.0
                fdmValue: 10.0
                fdmEnabled: draft_shield_enabled.fdmValue && draft_shield_height_limitation.fdmValue === "limited"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:conical_overhang_enabled; objectName: "conical_overhang_enabled"
                fdmLabel: "Make Overhang Printable"
                fdmDescription: "Change the geometry of the printed model such that minimal support is required. Steep overhangs will become shallow overhangs. Overhanging areas will drop down to become more vertical."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:conical_overhang_angle; objectName: "conical_overhang_angle"
                fdmLabel: "Maximum Model Angle"
                fdmDescription: "The maximum angle of overhangs after the they have been made printable. At a value of 0° all overhangs are replaced by a piece of model connected to the build plate, 90° will not change the model in any way."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: -89.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValue: 89.0
                fdmDefaultValue: 50.0
                fdmEnabled: conical_overhang_enabled.fdmValue
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:conical_overhang_hole_size; objectName: "conical_overhang_hole_size"
                fdmLabel: "Maximum Overhang Hole Area"
                fdmDescription: "The maximum area of a hole in the base of the model before it's removed by Make Overhang Printable. Holes smaller than this will be retained. A value of 0 mm² will fill all holes in the models base."
                fdmUnit: "mm²"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 0.0
                fdmDefaultValue: 0.0
                fdmEnabled: conical_overhang_enabled.fdmValue
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:cross_infill_pocket_size; objectName: "cross_infill_pocket_size"
                fdmLabel: "Cross 3D Pocket Size"
                fdmDescription: "The size of pockets at four-way crossings in the cross 3D pattern at heights where the pattern is touching itself."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 2.0
                fdmValue: Number(infill_line_distance.fdmValue)
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(infill_line_distance.fdmValue) * Math.sqrt(2)
                fdmEnabled: infill_pattern.fdmValue === "cross_3d"
                fdmLimitToExtruder: Number(infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr,infill_line_distance"
            }
            FdmQml_Param{ id:cross_infill_density_image; objectName: "cross_infill_density_image"
                fdmLabel: "Cross Infill Density Image"
                fdmDescription: "The file location of an image of which the brightness values determine the minimal density at the corresponding location in the infill of the print."
                fdmType: "str"
                fdmDefaultValue: ""
                fdmEnabled: infill_pattern.fdmValue === "cross" || infill_pattern.fdmValue === "cross_3d"
                fdmLimitToExtruder: infill_extruder_nr.fdmValue
                fdmSettablePerMesh: true
                fdmAffectedById: "infill_extruder_nr"
            }
            FdmQml_Param{ id:cross_support_density_image; objectName: "cross_support_density_image"
                fdmLabel: "Cross Fill Density Image for Support"
                fdmDescription: "The file location of an image of which the brightness values determine the minimal density at the corresponding location in the support."
                fdmType: "str"
                fdmDefaultValue: ""
                fdmEnabled: support_pattern.fdmValue === "cross" || support_pattern.fdmValue === "cross_3d"
                fdmLimitToExtruder: support_infill_extruder_nr.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_conical_enabled; objectName: "support_conical_enabled"
                fdmLabel: "Enable Conical Support"
                fdmDescription: "Make support areas smaller at the bottom than at the overhang."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: support_enable.fdmValue && support_structure.fdmValue !== "tree"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_conical_angle; objectName: "support_conical_angle"
                fdmLabel: "Conical Support Angle"
                fdmDescription: "The angle of the tilt of conical support. With 0 degrees being vertical, and 90 degrees being horizontal. Smaller angles cause the support to be more sturdy, but consist of more material. Negative angles cause the base of the support to be wider than the top."
                fdmUnit: "°"
                fdmType: "float"
                fdmMinimumValue: -90.0
                fdmMinimumValueWarning: -45.0
                fdmMaximumValueWarning: 45.0
                fdmMaximumValue: 90.0
                fdmDefaultValue: 30.0
                fdmEnabled: support_conical_enabled.fdmValue && support_enable.fdmValue && support_structure.fdmValue !== "tree"
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "support_infill_extruder_nr"
            }
            FdmQml_Param{ id:support_conical_min_width; objectName: "support_conical_min_width"
                fdmLabel: "Conical Support Minimum Width"
                fdmDescription: "Minimum width to which the base of the conical support area is reduced. Small widths can lead to unstable support structures."
                fdmUnit: "mm"
                fdmDefaultValue: 5.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue) * 3
                fdmMaximumValueWarning: 100.0
                fdmType: "float"
                fdmEnabled: support_conical_enabled.fdmValue && support_enable.fdmValue && support_structure.fdmValue !== "tree" && Number(support_conical_angle.fdmValue) > 0
                fdmLimitToExtruder: Number(support_infill_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "machine_nozzle_size,support_infill_extruder_nr"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_enabled; objectName: "magic_fuzzy_skin_enabled"
                fdmLabel: "Fuzzy Skin"
                fdmDescription: "Randomly jitter while printing the outer wall, so that the surface has a rough and fuzzy look."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_outside_only; objectName: "magic_fuzzy_skin_outside_only"
                fdmLabel: "Fuzzy Skin Outside Only"
                fdmDescription: "Jitter only the parts' outlines and not the parts' holes."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmEnabled: magic_fuzzy_skin_enabled.fdmValue
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_thickness; objectName: "magic_fuzzy_skin_thickness"
                fdmLabel: "Fuzzy Skin Thickness"
                fdmDescription: "The width within which to jitter. It's advised to keep this below the outer wall width, since the inner walls are unaltered."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.3
                fdmMinimumValue: 0.001
                fdmMaximumValueWarning: Number(wall_line_width_0.fdmValue)
                fdmEnabled: magic_fuzzy_skin_enabled.fdmValue
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_line_width_0,wall_0_extruder_nr"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_point_density; objectName: "magic_fuzzy_skin_point_density"
                fdmLabel: "Fuzzy Skin Density"
                fdmDescription: "The average density of points introduced on each polygon in a layer. Note that the original points of the polygon are discarded, so a low density results in a reduction of the resolution."
                fdmType: "float"
                fdmUnit: "1/mm"
                fdmDefaultValue: 1.25
                fdmMinimumValue: 0.008
                fdmMinimumValueWarning: 0.1
                fdmMaximumValueWarning: 10.0
                fdmMaximumValue: 2 / Number(magic_fuzzy_skin_thickness.fdmValue)
                fdmEnabled: magic_fuzzy_skin_enabled.fdmValue
                fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                fdmSettablePerMesh: true
                fdmAffectedById: "wall_0_extruder_nr,magic_fuzzy_skin_thickness"
                FdmQml_Param{ id:magic_fuzzy_skin_point_dist; objectName: "magic_fuzzy_skin_point_dist"
                    fdmLabel: "Fuzzy Skin Point Distance"
                    fdmDescription: "The average distance between the random points introduced on each line segment. Note that the original points of the polygon are discarded, so a high smoothness results in a reduction of the resolution. This value must be higher than half the Fuzzy Skin Thickness."
                    fdmType: "float"
                    fdmUnit: "mm"
                    fdmDefaultValue: 0.8
                    fdmMinimumValue: Number(magic_fuzzy_skin_thickness.fdmValue) / 2
                    fdmMinimumValueWarning: 0.1
                    fdmMaximumValueWarning: 10.0
                    fdmValue: Number(magic_fuzzy_skin_point_density.fdmValue) === 0 ? 10000 : (1 / Number(magic_fuzzy_skin_point_density.fdmValue))
                    fdmEnabled: magic_fuzzy_skin_enabled.fdmValue
                    fdmLimitToExtruder: Number(wall_0_extruder_nr.fdmValue)
                    fdmSettablePerMesh: true
                    fdmAffectedById: "wall_0_extruder_nr,magic_fuzzy_skin_thickness,magic_fuzzy_skin_point_density"
                }
            }
            FdmQml_Param{ id:flow_rate_max_extrusion_offset; objectName: "flow_rate_max_extrusion_offset"
                fdmLabel: "Flow Rate Compensation Max Extrusion Offset"
                fdmDescription: "The maximum distance in mm to move the filament to compensate for changes in flow rate."
                fdmUnit: "mm"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 10.0
                fdmDefaultValue: 0.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:flow_rate_extrusion_offset_factor; objectName: "flow_rate_extrusion_offset_factor"
                fdmLabel: "Flow Rate Compensation Factor"
                fdmDescription: "How far to move the filament in order to compensate for changes in flow rate, as a percentage of how far the filament would move in one second of extrusion."
                fdmUnit: "%"
                fdmType: "float"
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 100.0
                fdmDefaultValue: 100.0
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_enabled; objectName: "wireframe_enabled"
                fdmLabel: "Wire Printing"
                fdmDescription: "Print only the outside surface with a sparse webbed structure, printing 'in thin air'. This is realized by horizontally printing the contours of the model at given Z intervals which are connected via upward and diagonally downward lines."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_height; objectName: "wireframe_height"
                fdmLabel: "WP Connection Height"
                fdmDescription: "The height of the upward and diagonally downward lines between two horizontal parts. This determines the overall density of the net structure. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 3.0
                fdmValue: Number(machine_nozzle_head_distance.fdmValue)
                fdmMinimumValue: 0.001
                fdmMaximumValueWarning: 20.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_nozzle_head_distance"
            }
            FdmQml_Param{ id:wireframe_roof_inset; objectName: "wireframe_roof_inset"
                fdmLabel: "WP Roof Inset Distance"
                fdmDescription: "The distance covered when making a connection from a roof outline inward. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 3.0
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: Number(machine_nozzle_size.fdmValue)
                fdmMaximumValueWarning: 20.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmValue: Number(wireframe_height.fdmValue)
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_nozzle_size,wireframe_height"
            }
            FdmQml_Param{ id:wireframe_printspeed; objectName: "wireframe_printspeed"
                fdmLabel: "WP Speed"
                fdmDescription: "Speed at which the nozzle moves when extruding material. Only applies to Wire Printing."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 5.0
                fdmMinimumValue: 0.05
                fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2 + Number(machine_max_feedrate_z.fdmValue) ** 2)
                fdmMaximumValueWarning: 50.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,machine_max_feedrate_z"
                FdmQml_Param{ id:wireframe_printspeed_bottom; objectName: "wireframe_printspeed_bottom"
                    fdmLabel: "WP Bottom Printing Speed"
                    fdmDescription: "Speed of printing the first layer, which is the only layer touching the build platform. Only applies to Wire Printing."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 5.0
                    fdmMinimumValue: 0.05
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 50.0
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmValue: Number(wireframe_printspeed_flat.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,wireframe_printspeed_flat"
                }
                FdmQml_Param{ id:wireframe_printspeed_up; objectName: "wireframe_printspeed_up"
                    fdmLabel: "WP Upward Printing Speed"
                    fdmDescription: "Speed of printing a line upward 'in thin air'. Only applies to Wire Printing."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 5.0
                    fdmMinimumValue: 0.05
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2 + Number(machine_max_feedrate_z.fdmValue) ** 2)
                    fdmMaximumValueWarning: 50.0
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmValue: Number(wireframe_printspeed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,machine_max_feedrate_z,wireframe_printspeed"
                }
                FdmQml_Param{ id:wireframe_printspeed_down; objectName: "wireframe_printspeed_down"
                    fdmLabel: "WP Downward Printing Speed"
                    fdmDescription: "Speed of printing a line diagonally downward. Only applies to Wire Printing."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 5.0
                    fdmMinimumValue: 0.05
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2 + Number(machine_max_feedrate_z.fdmValue) ** 2)
                    fdmMaximumValueWarning: 50.0
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmValue: Number(wireframe_printspeed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,machine_max_feedrate_z,wireframe_printspeed"
                }
                FdmQml_Param{ id:wireframe_printspeed_flat; objectName: "wireframe_printspeed_flat"
                    fdmLabel: "WP Horizontal Printing Speed"
                    fdmDescription: "Speed of printing the horizontal contours of the model. Only applies to Wire Printing."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 5.0
                    fdmMinimumValue: 0.05
                    fdmMaximumValue:  Math.sqrt(Number(machine_max_feedrate_x.fdmValue) ** 2 + Number(machine_max_feedrate_y.fdmValue) ** 2)
                    fdmMaximumValueWarning: 100.0
                    fdmValue: Number(wireframe_printspeed.fdmValue)
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "machine_max_feedrate_x,machine_max_feedrate_y,wireframe_printspeed"
                }
            }
            FdmQml_Param{ id:wireframe_flow; objectName: "wireframe_flow"
                fdmLabel: "WP Flow"
                fdmDescription: "Flow compensation: the amount of material extruded is multiplied by this value. Only applies to Wire Printing."
                fdmUnit: "%"
                fdmDefaultValue: 100.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 100.0
                fdmType: "float"
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
                FdmQml_Param{ id:wireframe_flow_connection; objectName: "wireframe_flow_connection"
                    fdmLabel: "WP Connection Flow"
                    fdmDescription: "Flow compensation when going up or down. Only applies to Wire Printing."
                    fdmUnit: "%"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 100.0
                    fdmType: "float"
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmValue: Number(wireframe_flow.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "wireframe_flow"
                }
                FdmQml_Param{ id:wireframe_flow_flat; objectName: "wireframe_flow_flat"
                    fdmLabel: "WP Flat Flow"
                    fdmDescription: "Flow compensation when printing flat lines. Only applies to Wire Printing."
                    fdmUnit: "%"
                    fdmDefaultValue: 100.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValueWarning: 100.0
                    fdmType: "float"
                    fdmEnabled: wireframe_enabled.fdmValue
                    fdmValue: Number(wireframe_flow.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: false
                    fdmSettablePerMeshgroup: false
                    fdmAffectedById: "wireframe_flow"
                }
            }
            FdmQml_Param{ id:wireframe_top_delay; objectName: "wireframe_top_delay"
                fdmLabel: "WP Top Delay"
                fdmDescription: "Delay time after an upward move, so that the upward line can harden. Only applies to Wire Printing."
                fdmUnit: "s"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 1.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_bottom_delay; objectName: "wireframe_bottom_delay"
                fdmLabel: "WP Bottom Delay"
                fdmDescription: "Delay time after a downward move. Only applies to Wire Printing."
                fdmUnit: "s"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 1.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_flat_delay; objectName: "wireframe_flat_delay"
                fdmLabel: "WP Flat Delay"
                fdmDescription: "Delay time between two horizontal segments. Introducing such a delay can cause better adhesion to previous layers at the connection points, while too long delays cause sagging. Only applies to Wire Printing."
                fdmUnit: "s"
                fdmType: "float"
                fdmDefaultValue: 0.1
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 0.5
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_up_half_speed; objectName: "wireframe_up_half_speed"
                fdmLabel: "WP Ease Upward"
                fdmDescription: "Distance of an upward move which is extruded with half speed.\nThis can cause better adhesion to previous layers, while not heating the material in those layers too much. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.3
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 5.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_top_jump; objectName: "wireframe_top_jump"
                fdmLabel: "WP Knot Size"
                fdmDescription: "Creates a small knot at the top of an upward line, so that the consecutive horizontal layer has a better chance to connect to it. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.6
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 2.0
                fdmEnabled: wireframe_enabled.fdmValue && wireframe_strategy.fdmValue === "knot"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_fall_down; objectName: "wireframe_fall_down"
                fdmLabel: "WP Fall Down"
                fdmDescription: "Distance with which the material falls down after an upward extrusion. This distance is compensated for. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.5
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(wireframe_height.fdmValue)
                fdmEnabled: wireframe_enabled.fdmValue && wireframe_strategy.fdmValue === "compensate"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "wireframe_height"
            }
            FdmQml_Param{ id:wireframe_drag_along; objectName: "wireframe_drag_along"
                fdmLabel: "WP Drag Along"
                fdmDescription: "Distance with which the material of an upward extrusion is dragged along with the diagonally downward extrusion. This distance is compensated for. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.6
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: Number(wireframe_height.fdmValue)
                fdmEnabled: wireframe_enabled.fdmValue && wireframe_strategy.fdmValue === "compensate"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "wireframe_height"
            }
            FdmQml_Param{ id:wireframe_strategy; objectName: "wireframe_strategy"
                fdmLabel: "WP Strategy"
                fdmDescription: "Strategy for making sure two consecutive layers connect at each connection point. Retraction lets the upward lines harden in the right position, but may cause filament grinding. A knot can be made at the end of an upward line to heighten the chance of connecting to it and to let the line cool; however, it may require slow printing speeds. Another strategy is to compensate for the sagging of the top of an upward line; however, the lines won't always fall down as predicted."
                fdmType: "enum"
                fdmOptions:{
                    "compensate": "Compensate",
                    "knot": "Knot",
                    "retract": "Retract"
                }
                fdmDefaultValue: "compensate"
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_straight_before_down; objectName: "wireframe_straight_before_down"
                fdmLabel: "WP Straighten Downward Lines"
                fdmDescription: "Percentage of a diagonally downward line which is covered by a horizontal line piece. This can prevent sagging of the top most point of upward lines. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "%"
                fdmDefaultValue: 20.0
                fdmMinimumValue: 0.0
                fdmMaximumValue: 100.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_roof_fall_down; objectName: "wireframe_roof_fall_down"
                fdmLabel: "WP Roof Fall Down"
                fdmDescription: "The distance which horizontal roof lines printed 'in thin air' fall down when being printed. This distance is compensated for. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 2.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: Number(wireframe_roof_inset.fdmValue)
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "wireframe_roof_inset"
            }
            FdmQml_Param{ id:wireframe_roof_drag_along; objectName: "wireframe_roof_drag_along"
                fdmLabel: "WP Roof Drag Along"
                fdmDescription: "The distance of the end piece of an inward line which gets dragged along when going back to the outer outline of the roof. This distance is compensated for. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 0.8
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_roof_outer_delay; objectName: "wireframe_roof_outer_delay"
                fdmLabel: "WP Roof Outer Delay"
                fdmDescription: "Time spent at the outer perimeters of hole which is to become a roof. Longer times can ensure a better connection. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "s"
                fdmDefaultValue: 0.2
                fdmMinimumValue: 0.0
                fdmMaximumValueWarning: 2.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wireframe_nozzle_clearance; objectName: "wireframe_nozzle_clearance"
                fdmLabel: "WP Nozzle Clearance"
                fdmDescription: "Distance between the nozzle and horizontally downward lines. Larger clearance results in diagonally downward lines with a less steep angle, which in turn results in less upward connections with the next layer. Only applies to Wire Printing."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 1.0
                fdmMinimumValueWarning: 0.0
                fdmMaximumValueWarning: 10.0
                fdmEnabled: wireframe_enabled.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:adaptive_layer_height_enabled; objectName: "adaptive_layer_height_enabled"
                fdmLabel: "Use Adaptive Layers"
                fdmDescription: "Adaptive layers computes the layer heights depending on the shape of the model."
                fdmType: "bool"
                fdmDefaultValue: false
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:adaptive_layer_height_variation; objectName: "adaptive_layer_height_variation"
                fdmLabel: "Adaptive Layers Maximum Variation"
                fdmDescription: "The maximum allowed height different from the base layer height."
                fdmType: "float"
                fdmEnabled: adaptive_layer_height_enabled.fdmValue
                fdmUnit: "mm"
                fdmDefaultValue: 0.1
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:adaptive_layer_height_variation_step; objectName: "adaptive_layer_height_variation_step"
                fdmLabel: "Adaptive Layers Variation Step Size"
                fdmDescription: "The difference in height of the next layer height compared to the previous one."
                fdmType: "float"
                fdmEnabled: adaptive_layer_height_enabled.fdmValue
                fdmDefaultValue: 0.01
                fdmUnit: "mm"
                fdmSettablePerMesh: false
                fdmMinimumValue: 0.001
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:adaptive_layer_height_threshold; objectName: "adaptive_layer_height_threshold"
                fdmLabel: "Adaptive Layers Topography Size"
                fdmDescription: "Target horizontal distance between two adjacent layers. Reducing this setting causes thinner layers to be used to bring the edges of the layers closer together."
                fdmType: "float"
                fdmEnabled: adaptive_layer_height_enabled.fdmValue
                fdmDefaultValue: 0.2
                fdmUnit: "mm"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: false
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:clean_between_layers; objectName: "clean_between_layers"
                fdmLabel: "Wipe Nozzle Between Layers"
                fdmDescription: "Whether to include nozzle wipe G-Code between layers (maximum 1 per layer). Enabling this setting could influence behavior of retract at layer change. Please use Wipe Retraction settings to control retraction at layers where the wipe script will be working."
                fdmDefaultValue: false
                fdmType: "bool"
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:max_extrusion_before_wipe; objectName: "max_extrusion_before_wipe"
                fdmLabel: "Material Volume Between Wipes"
                fdmDescription: "Maximum material that can be extruded before another nozzle wipe is initiated. If this value is less than the volume of material required in a layer, the setting has no effect in this layer, i.e. it is limited to one wipe per layer."
                fdmDefaultValue: 10.0
                fdmType: "float"
                fdmUnit: "mm³"
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wipe_retraction_enable; objectName: "wipe_retraction_enable"
                fdmLabel: "Wipe Retraction Enable"
                fdmDescription: "Retract the filament when the nozzle is moving over a non-printed area."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmValue: retraction_enable.fdmValue
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "retraction_enable"
            }
            FdmQml_Param{ id:wipe_retraction_amount; objectName: "wipe_retraction_amount"
                fdmLabel: "Wipe Retraction Distance"
                fdmDescription: "Amount to retract the filament so it does not ooze during the wipe sequence."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmValue: Number(retraction_amount.fdmValue)
                fdmMinimumValueWarning: -0.0001
                fdmMaximumValueWarning: 10.0
                fdmEnabled: wipe_retraction_enable.fdmValue && clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "retraction_amount"
            }
            FdmQml_Param{ id:wipe_retraction_extra_prime_amount; objectName: "wipe_retraction_extra_prime_amount"
                fdmLabel: "Wipe Retraction Extra Prime Amount"
                fdmDescription: "Some material can ooze away during a wipe travel moves, which can be compensated for here."
                fdmUnit: "mm³"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmValue: Number(retraction_extra_prime_amount.fdmValue)
                fdmMinimumValueWarning: -0.0001
                fdmMaximumValueWarning: 10.0
                fdmEnabled: wipe_retraction_enable.fdmValue && clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "retraction_extra_prime_amount"
            }
            FdmQml_Param{ id:wipe_retraction_speed; objectName: "wipe_retraction_speed"
                fdmLabel: "Wipe Retraction Speed"
                fdmDescription: "The speed at which the filament is retracted and primed during a wipe retraction move."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 5.0
                fdmValue: Number(retraction_speed.fdmValue)
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 1.0
                fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                fdmMaximumValueWarning: 70.0
                fdmEnabled: wipe_retraction_enable.fdmValue && clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmAffectedById: "machine_max_feedrate_e,retraction_speed"
                FdmQml_Param{ id:wipe_retraction_retract_speed; objectName: "wipe_retraction_retract_speed"
                    fdmLabel: "Wipe Retraction Retract Speed"
                    fdmDescription: "The speed at which the filament is retracted during a wipe retraction move."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 3.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                    fdmMinimumValueWarning: 1.0
                    fdmMaximumValueWarning: 70.0
                    fdmEnabled: wipe_retraction_enable.fdmValue && clean_between_layers.fdmValue
                    fdmValue: Number(wipe_retraction_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_max_feedrate_e,wipe_retraction_speed"
                }
                FdmQml_Param{ id:wipe_retraction_prime_speed; objectName: "wipe_retraction_prime_speed"
                    fdmLabel: "Wipe Retraction Prime Speed"
                    fdmDescription: "The speed at which the filament is primed during a wipe retraction move."
                    fdmUnit: "mm/s"
                    fdmType: "float"
                    fdmDefaultValue: 2.0
                    fdmMinimumValue: 0.0
                    fdmMaximumValue: Number(machine_max_feedrate_e.fdmValue)
                    fdmMinimumValueWarning: 1.0
                    fdmMaximumValueWarning: 70.0
                    fdmEnabled: wipe_retraction_enable.fdmValue && clean_between_layers.fdmValue
                    fdmValue: Number(wipe_retraction_speed.fdmValue)
                    fdmSettablePerMesh: false
                    fdmSettablePerExtruder: true
                    fdmAffectedById: "machine_max_feedrate_e,wipe_retraction_speed"
                }
            }
            FdmQml_Param{ id:wipe_pause; objectName: "wipe_pause"
                fdmLabel: "Wipe Pause"
                fdmDescription: "Pause after the unretract."
                fdmUnit: "s"
                fdmType: "float"
                fdmDefaultValue: 0.0
                fdmMinimumValue: 0.0
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wipe_hop_enable; objectName: "wipe_hop_enable"
                fdmLabel: "Wipe Z Hop"
                fdmDescription: "When wiping, the build plate is lowered to create clearance between the nozzle and the print. It prevents the nozzle from hitting the print during travel moves, reducing the chance to knock the print from the build plate."
                fdmType: "bool"
                fdmDefaultValue: true
                fdmValue: retraction_hop_enabled.fdmValue
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "retraction_hop_enabled"
            }
            FdmQml_Param{ id:wipe_hop_amount; objectName: "wipe_hop_amount"
                fdmLabel: "Wipe Z Hop Height"
                fdmDescription: "The height difference when performing a Z Hop."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 1.0
                fdmValue: Number(retraction_hop.fdmValue)
                fdmEnabled: wipe_hop_enable.fdmValue && clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "retraction_hop"
            }
            FdmQml_Param{ id:wipe_hop_speed; objectName: "wipe_hop_speed"
                fdmLabel: "Wipe Hop Speed"
                fdmDescription: "Speed to move the z-axis during the hop."
                fdmUnit: "mm/s"
                fdmType: "float"
                fdmDefaultValue: 10.0
                fdmValue: Number(speed_z_hop.fdmValue)
                fdmMinimumValue: 0.0
                fdmMinimumValueWarning: 1.0
                fdmEnabled: wipe_hop_enable.fdmValue && clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: "speed_z_hop"
            }
            FdmQml_Param{ id:wipe_brush_pos_x; objectName: "wipe_brush_pos_x"
                fdmLabel: "Wipe Brush X Position"
                fdmDescription: "X location where wipe script will start."
                fdmType: "float"
                fdmUnit: "mm"
                fdmDefaultValue: 100.0
                fdmMinimumValueWarning: 0.0
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wipe_repeat_count; objectName: "wipe_repeat_count"
                fdmLabel: "Wipe Repeat Count"
                fdmDescription: "Number of times to move the nozzle across the brush."
                fdmType: "int"
                fdmMinimumValue: 0
                fdmDefaultValue: 5
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:wipe_move_distance; objectName: "wipe_move_distance"
                fdmLabel: "Wipe Move Distance"
                fdmDescription: "The distance to move the head back and forth across the brush."
                fdmUnit: "mm"
                fdmType: "float"
                fdmDefaultValue: 20.0
                fdmEnabled: clean_between_layers.fdmValue
                fdmSettablePerMesh: false
                fdmSettablePerExtruder: true
                fdmSettablePerMeshgroup: false
                fdmAffectedById: ""
            }
        }
        FdmQml_Category{ id:command_line_settings; objectName: "command_line_settings"
            fdmLabel: "Command Line Settings"
            fdmDescription: "Settings which are only used if AnkerMake Engine isn't called from the AnkerMake frontend."
            fdmType: "category"
            fdmEnabled: false
            FdmQml_Param{ id:center_object; objectName: "center_object"
                fdmDescription: "Whether to center the object on the middle of the build platform (0,0), instead of using the coordinate system in which the object was saved."
                fdmType: "bool"
                fdmLabel: "Center Object"
                fdmDefaultValue: false
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mesh_position_x; objectName: "mesh_position_x"
                fdmDescription: "Offset applied to the object in the x direction."
                fdmType: "float"
                fdmLabel: "Mesh Position X"
                fdmDefaultValue: 0.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mesh_position_y; objectName: "mesh_position_y"
                fdmDescription: "Offset applied to the object in the y direction."
                fdmType: "float"
                fdmLabel: "Mesh Position Y"
                fdmDefaultValue: 0.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mesh_position_z; objectName: "mesh_position_z"
                fdmDescription: "Offset applied to the object in the z direction. With this you can perform what was used to be called 'Object Sink'."
                fdmType: "float"
                fdmLabel: "Mesh Position Z"
                fdmDefaultValue: 0.0
                fdmEnabled: false
                fdmAffectedById: ""
            }
            FdmQml_Param{ id:mesh_rotation_matrix; objectName: "mesh_rotation_matrix"
                fdmLabel: "Mesh Rotation Matrix"
                fdmDescription: "Transformation matrix to be applied to the model when loading it from file."
                fdmType: "str"
                fdmDefaultValue: "[[1,0,0], [0,1,0], [0,0,1]]"
                fdmEnabled: false
                fdmAffectedById: ""
            }
        }
    }
}
