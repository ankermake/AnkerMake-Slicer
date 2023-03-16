import QtQuick 2.2
import FdmQml 1.0
// qrc:/Settings/FdmJsonObjTree_All.qml
FdmQml_Root{ id:fdmextruder_def_json; objectName: "qrc:/Settings/FdmJsonObjTree_All.qml"
    property string name: "FDM打印机基础描述"
    property int version: 2
    FdmQml_Settings{ id:settings; objectName: "settings"
        FdmQml_Category{ id:anker_fdm_setting; objectName: "anker_fdm_setting"
            fdmLabel: "安克参数"
            fdmDescription: "安克调优参数"
            FdmQml_Param{ id:international_language; objectName: "international_language"
                fdmLabel: "国际语言"
                fdmDescription: "FDM国际语言开关"
            }
            FdmQml_Param{ id:anker_param_ai_camera; objectName: "anker_param_ai_camera"
                fdmLabel: "AI摄像头"
                fdmDescription: "﻿AI摄像头"
                FdmQml_Param{ id:camera_take_picture_time; objectName: "camera_take_picture_time"
                    fdmLabel: "拍照时间间隔(毫秒)"
                    fdmDescription: "﻿拍照时间间隔(毫秒)"
                }
                FdmQml_Param{ id:camera_position; objectName: "camera_position"
                    fdmLabel: "摄像头位置"
                    fdmDescription: "﻿摄像头位置"
                    FdmQml_Param{ id:camera_position_x; objectName: "camera_position_x"
                        fdmLabel: "摄像头X坐标"
                        fdmDescription: "﻿摄像头X坐标"
                    }
                    FdmQml_Param{ id:camera_position_y; objectName: "camera_position_y"
                        fdmLabel: "摄像头Y坐标"
                        fdmDescription: "﻿摄像头Y坐标"
                    }
                    FdmQml_Param{ id:camera_position_z; objectName: "camera_position_z"
                        fdmLabel: "摄像头Z坐标"
                        fdmDescription: "﻿摄像头Z坐标"
                    }
                }
                FdmQml_Param{ id:camera_up; objectName: "camera_up"
                    fdmLabel: "相机UP方向"
                    fdmDescription: "﻿相机UP方向"
                    FdmQml_Param{ id:camera_up_x; objectName: "camera_up_x"
                        fdmLabel: "相机UP方向 X"
                        fdmDescription: "﻿相机UP方向 X"
                    }
                    FdmQml_Param{ id:camera_up_y; objectName: "camera_up_y"
                        fdmLabel: "相机UP方向 Y"
                        fdmDescription: "﻿相机UP方向 Y"
                    }
                    FdmQml_Param{ id:camera_up_z; objectName: "camera_up_z"
                        fdmLabel: "相机UP方向 Z"
                        fdmDescription: "﻿相机UP方向 Z"
                    }
                }
                FdmQml_Param{ id:camera_view_vector; objectName: "camera_view_vector"
                    fdmLabel: "视心方向"
                    fdmDescription: "﻿视心方向"
                    FdmQml_Param{ id:camera_view_vector_x; objectName: "camera_view_vector_x"
                        fdmLabel: "视心X坐标"
                        fdmDescription: "﻿视心X坐标"
                    }
                    FdmQml_Param{ id:camera_view_vector_y; objectName: "camera_view_vector_y"
                        fdmLabel: "视心Y坐标"
                        fdmDescription: "﻿视心Y坐标"
                    }
                    FdmQml_Param{ id:camera_view_vector_z; objectName: "camera_view_vector_z"
                        fdmLabel: "视心Z坐标"
                        fdmDescription: "﻿视心Z坐标"
                    }
                }
                FdmQml_Param{ id:camera_resolution; objectName: "camera_resolution"
                    fdmLabel: "截图分辨率"
                    fdmDescription: "﻿截图分辨率"
                    FdmQml_Param{ id:camera_resolution_w; objectName: "camera_resolution_w"
                        fdmLabel: "截图宽度 W"
                        fdmDescription: "﻿截图宽度 W"
                    }
                    FdmQml_Param{ id:camera_resolution_h; objectName: "camera_resolution_h"
                        fdmLabel: "截图高度 H"
                        fdmDescription: "﻿截图高度 H"
                    }
                }
                FdmQml_Param{ id:camera_field_view; objectName: "camera_field_view"
                    fdmLabel: "垂直视场角"
                    fdmDescription: "﻿垂直视场角"
                }
            }
            FdmQml_Param{ id:ak_meshfix_dynamic_resolution; objectName: "ak_meshfix_dynamic_resolution"
                fdmLabel: "动态分辨率"
                fdmDescription: "﻿动态分辨率"
            }
            FdmQml_Param{ id:meshfix_maximum_resolution; objectName: "meshfix_maximum_resolution"
                fdmLabel: "最大分辨率"
                fdmDescription: "走线部分在切片后的最小尺寸。如果提高此值，网格的分辨率将降低。这可让打印机保持处理 g-code 所需的速度，并将通过移除无法处理的网格细节提高切片速度。"
            }
            FdmQml_Param{ id:meshfix_maximum_travel_resolution; objectName: "meshfix_maximum_travel_resolution"
                fdmLabel: "空走的最大分辨率"
                fdmDescription: "切片后的旅行线路段的最小尺寸。如果你增加了这个，旅行的移动就会变得不那么平滑了。这可能使打印机能够跟上它处理g代码的速度，但是它可能导致模型的避免变得不那么准确。"
            }
            FdmQml_Param{ id:meshfix_maximum_deviation; objectName: "meshfix_maximum_deviation"
                fdmLabel: "最大偏移量"
                fdmDescription: "在最大分辨率设置中减小分辨率时，允许的最大偏移量。如果增加该值，打印作业的准确性将降低，G-code 将减小。"
            }
            FdmQml_Param{ id:relative_extrusion; objectName: "relative_extrusion"
                fdmLabel: "相对挤出"
                fdmDescription: "使用相对挤出而不是绝对挤出。 使用相对 E 步阶，对 Gcode 进行更轻松的后期处理。 但是，不是所有打印机均支持此功能，而且与绝对 E 步阶相比，它在沉积材料量上会产生非常轻微的偏差。 不论是否有此设置，挤出模式将始终设置为绝对挤出后才会输出任何 Gcode 脚本。"
            }
            FdmQml_Param{ id:ak_VAJK_K_enabled; objectName: "ak_VAJK_K_enabled"
                fdmLabel: "K值控制"
                fdmDescription: "﻿K值控制"
                FdmQml_Param{ id:ak_K_print; objectName: "ak_K_print"
                    fdmLabel: "打印K值"
                    fdmDescription: "﻿打印K值"
                    FdmQml_Param{ id:ak_K_infill; objectName: "ak_K_infill"
                        fdmLabel: "填充K值"
                        fdmDescription: "﻿填充K值"
                    }
                    FdmQml_Param{ id:ak_K_wall; objectName: "ak_K_wall"
                        fdmLabel: "墙K值"
                        fdmDescription: "﻿墙K值"
                        FdmQml_Param{ id:ak_K_wall_0; objectName: "ak_K_wall_0"
                            fdmLabel: "外墙K值"
                            fdmDescription: "﻿外墙K值"
                        }
                        FdmQml_Param{ id:ak_K_wall_x; objectName: "ak_K_wall_x"
                            fdmLabel: "内墙K值"
                            fdmDescription: "﻿内墙K值"
                        }
                    }
                    FdmQml_Param{ id:ak_K_topbottom; objectName: "ak_K_topbottom"
                        fdmLabel: "顶底K值"
                        fdmDescription: "﻿顶底K值"
                    }
                    FdmQml_Param{ id:ak_K_support; objectName: "ak_K_support"
                        fdmLabel: "支撑K值"
                        fdmDescription: "﻿支撑K值"
                    }
                    FdmQml_Param{ id:ak_K_skirt_brim; objectName: "ak_K_skirt_brim"
                        fdmLabel: "裙边K值"
                        fdmDescription: "﻿裙边K值"
                    }
                    FdmQml_Param{ id:ak_K_layer_0; objectName: "ak_K_layer_0"
                        fdmLabel: "首层K值"
                        fdmDescription: "﻿首层K值"
                    }
                }
            }
            FdmQml_Param{ id:shrink_infill_shrink_distance; objectName: "shrink_infill_shrink_distance"
                fdmLabel: "填充收缩距离"
                fdmDescription: "﻿填充收缩距离"
            }
            FdmQml_Param{ id:wall_ruler_concave_convex_enable; objectName: "wall_ruler_concave_convex_enable"
                fdmLabel: "刻度打印优化"
                fdmDescription: "﻿内凹外凸打印优化，例如内凹外凸问题。"
                FdmQml_Param{ id:wall_ruler_concave_percent; objectName: "wall_ruler_concave_percent"
                    fdmLabel: "内凹区域打印优化"
                    fdmDescription: "﻿内凹区域打印优化。"
                }
                FdmQml_Param{ id:wall_ruler_convex_percent; objectName: "wall_ruler_convex_percent"
                    fdmLabel: "外凸区域打印优化"
                    fdmDescription: "外凸区域打印优化。"
                }
                FdmQml_Param{ id:wall_ruler_separate_percent; objectName: "wall_ruler_separate_percent"
                    fdmLabel: "内外墙分离率"
                    fdmDescription: "内外墙分离率﻿。"
                }
            }
            FdmQml_Param{ id:small_hole_max_size; objectName: "small_hole_max_size"
                fdmLabel: "小孔最大尺寸"
                fdmDescription: "将使用微小特征速度打印直径小于此尺寸的孔和零件轮廓。"
                FdmQml_Param{ id:small_feature_max_length; objectName: "small_feature_max_length"
                    fdmLabel: "微小特征最大长度"
                    fdmDescription: "将使用微小特征速度打印小于此长度的特征轮廓。"
                }
                FdmQml_Param{ id:small_feature_speed_factor; objectName: "small_feature_speed_factor"
                    fdmLabel: "微小特征速度"
                    fdmDescription: "微小特征将按正常打印速度的百分比进行打印。缓慢打印有助于粘合和提高准确性。"
                }
                FdmQml_Param{ id:small_feature_speed_factor_0; objectName: "small_feature_speed_factor_0"
                    fdmLabel: "微小特征初始层速度"
                    fdmDescription: "第一层的微小特征将按正常打印速度的百分比进行打印。缓慢打印有助于粘合和提高准确性。"
                }
            }
            FdmQml_Param{ id:wall_min_extrusion; objectName: "wall_min_extrusion"
                fdmLabel: "墙最小挤出量"
                fdmDescription: "﻿墙最小挤出量，小于这个值会使用空驶代替挤出"
            }
            FdmQml_Param{ id:wall_overhang_angle; objectName: "wall_overhang_angle"
                fdmLabel: "悬垂壁角度"
                fdmDescription: "悬垂超过此角度的壁将使用悬垂壁设置打印。该值为 90 时，不会将任何壁视为悬垂。"
                FdmQml_Param{ id:wall_overhang_area_filter; objectName: "wall_overhang_area_filter"
                    fdmLabel: "最小悬垂面积"
                    fdmDescription: "﻿最小悬垂面积，小于这个值不会使用悬垂设置"
                }
                FdmQml_Param{ id:wall_overhang_extend_circle; objectName: "wall_overhang_extend_circle"
                    fdmLabel: "悬垂参数应用到整圈"
                    fdmDescription: "﻿悬垂参数应用到整圈"
                }
                FdmQml_Param{ id:wall_overhang_extend_type; objectName: "wall_overhang_extend_type"
                    fdmLabel: "悬垂降速类型"
                    fdmDescription: "﻿设置打印悬垂时的降速策略,使得打印时能更加平滑"
                    fdmOptions:{
                        "instant": "悬垂直接降速",
                        "gradually_xy": "悬垂逐渐降速",
                        "circle": "﻿悬垂整圈降速"
                    }
                    FdmQml_Param{ id:overhang_min_line_length; objectName: "overhang_min_line_length"
                        fdmLabel: "最小悬垂线长"
                        fdmDescription: "位于悬垂区域的线段要大于多少才会被当成悬垂线段处理。"
                    }
                    FdmQml_Param{ id:wall_overhang_extend_xy_distance; objectName: "wall_overhang_extend_xy_distance"
                        fdmLabel: "XY轴降速缓冲距离"
                        fdmDescription: "﻿当挤出头在XY平面距离悬垂区这个距离时开始逐渐减速到悬垂速度"
                    }
                    FdmQml_Param{ id:wall_overhang_extend_z_distance; objectName: "wall_overhang_extend_z_distance"
                        fdmLabel: "Z轴降速缓冲距离"
                        fdmDescription: "﻿当挤出头在Z轴距离悬垂区这个距离时开始逐渐减速到悬垂速度"
                    }
                    FdmQml_Param{ id:overhang_inner_wall_shrink_distance; objectName: "overhang_inner_wall_shrink_distance"
                        fdmLabel: "悬垂内墙收缩距离"
                        fdmDescription: "收缩内墙以获得更宽的外墙"
                    }
                }
                FdmQml_Param{ id:overhang_related_inner_wall_slow_down; objectName: "overhang_related_inner_wall_slow_down"
                    fdmLabel: "对悬垂内壁降速"
                    fdmDescription: "对悬垂壁相关的内壁使用降速"
                }
                FdmQml_Param{ id:wall_overhang_speed_factor; objectName: "wall_overhang_speed_factor"
                    fdmLabel: "悬垂壁速度"
                    fdmDescription: "悬垂壁将以其正常打印速度的此百分比打印。"
                }
                FdmQml_Param{ id:wall_overhang_flow_factor; objectName: "wall_overhang_flow_factor"
                    fdmLabel: "悬垂壁流量"
                    fdmDescription: "﻿悬垂壁流量，按照正常值的这个百分比打印"
                }
            }
            FdmQml_Param{ id:skin_overhang_enable; objectName: "skin_overhang_enable"
                fdmLabel: "皮肤悬垂设置"
                fdmDescription: "﻿皮肤悬垂设置"
                FdmQml_Param{ id:skin_overlap_overhang_speed; objectName: "skin_overlap_overhang_speed"
                    fdmLabel: "悬垂打印速度"
                    fdmDescription: "﻿按正常速度的这个百分比打印悬垂"
                }
                FdmQml_Param{ id:skin_overlap_overhang_flow; objectName: "skin_overlap_overhang_flow"
                    fdmLabel: "悬垂皮肤打印流量"
                    fdmDescription: "﻿按正常流量的这个百分比打印悬垂皮肤"
                }
                FdmQml_Param{ id:skin_overlap_overhang; objectName: "skin_overlap_overhang"
                    fdmLabel: "悬垂皮肤重叠百分比"
                    fdmDescription: "﻿皮肤和壁之间的重叠量，以走线宽度百分比表示。 稍微重叠可让各个壁与皮肤牢固连接。 这是皮肤线平均走线宽度和最内壁的百分比。"
                    FdmQml_Param{ id:skin_overlap_overhang_mm; objectName: "skin_overlap_overhang_mm"
                        fdmLabel: "悬垂皮肤重叠"
                        fdmDescription: "﻿皮肤和壁之间的重叠量。 稍微重叠可让各个壁与皮肤牢固连接。"
                    }
                }
            }
            FdmQml_Param{ id:optimize_wall_0_printing_order; objectName: "optimize_wall_0_printing_order"
                fdmLabel: "优化外墙打印顺序"
                fdmDescription: "﻿优化外壁打印顺序，使得外壁不是最先也不是最后打印。"
            }
            FdmQml_Param{ id:optimize_wall_hole_printing; objectName: "optimize_wall_hole_printing"
                fdmLabel: "优化孔壁打印"
                fdmDescription: "﻿优化孔壁打印"
            }
            FdmQml_Param{ id:reverse_hole_outer_wall_print_orient; objectName: "reverse_hole_outer_wall_print_orient"
                fdmLabel: "翻转孔壁打印方向"
                fdmDescription: "﻿翻转孔壁打印方向使得当打完外部打内部时能够衔接的比较顺滑"
            }
            FdmQml_Param{ id:optimize_wall_small_printing; objectName: "optimize_wall_small_printing"
                fdmLabel: "优化小区域打印"
                fdmDescription: "﻿优化小区域打印"
            }
            FdmQml_Param{ id:optimize_single_part_z_seam; objectName: "optimize_single_part_z_seam"
                fdmLabel: "单连通区域Z缝优化"
                fdmDescription: "﻿单连通区域Z缝优化"
            }
            FdmQml_Param{ id:show_machine_setting; objectName: "show_machine_setting"
                fdmLabel: "Jerk最大值设置"
                fdmDescription: "﻿Jerk最大值设置"
                FdmQml_Param{ id:machine_max_jerk_xy; objectName: "machine_max_jerk_xy"
                    fdmLabel: "默认 X-Y 平面抖动速度（Jerk）"
                    fdmDescription: "水平面移动的默认抖动速度。"
                }
                FdmQml_Param{ id:machine_max_jerk_z; objectName: "machine_max_jerk_z"
                    fdmLabel: "默认 Z 轴抖动速度（Jerk）"
                    fdmDescription: "Z 轴方向电机的默认抖动速度。"
                }
                FdmQml_Param{ id:machine_max_jerk_e; objectName: "machine_max_jerk_e"
                    fdmLabel: "默认挤出电机 Jerk"
                    fdmDescription: "耗材电机的默认抖动速度。"
                }
            }
            FdmQml_Param{ id:bridge_settings_enabled; objectName: "bridge_settings_enabled"
                fdmLabel: "启用连桥设置"
                fdmDescription: "在打印连桥时，检测连桥并修改打印速度、流量和风扇设置。"
                FdmQml_Param{ id:bridge_split_min_length; objectName: "bridge_split_min_length"
                    fdmLabel: "桥壁拆分最小长度"
                    fdmDescription: "桥壁长于此长度时，可以进行拆分使用多段速度打印"
                }
                FdmQml_Param{ id:bridge_shrink_length; objectName: "bridge_shrink_length"
                    fdmLabel: "桥面收缩长度"
                    fdmDescription: "从桥的起始点偏移多少开始应用桥设置"
                }
                FdmQml_Param{ id:bridge_wall_min_length; objectName: "bridge_wall_min_length"
                    fdmLabel: "最小桥壁长度"
                    fdmDescription: "将使用正常壁设置打印短于此长度且没有支撑的壁。将使用桥壁设置打印长于此长度且没有支撑的壁。"
                }
                FdmQml_Param{ id:bridge_skin_support_threshold; objectName: "bridge_skin_support_threshold"
                    fdmLabel: "连桥表面支撑阈值"
                    fdmDescription: "如果受支撑的表面区域小于整个区域的这一百分比，则使用连桥设置打印。否则，使用正常表面设置打印。"
                }
                FdmQml_Param{ id:bridge_sparse_infill_max_density; objectName: "bridge_sparse_infill_max_density"
                    fdmLabel: "连桥稀疏填充物最大密度"
                    fdmDescription: "填充物的最大密度被视为稀疏。稀疏填充物表面被视为不受支持，因此可被视为连桥表面。"
                }
                FdmQml_Param{ id:bridge_wall_coast; objectName: "bridge_wall_coast"
                    fdmLabel: "桥壁滑行"
                    fdmDescription: "此参数用于控制挤出机在开始打印桥壁前应该滑行的距离。在开始打印连桥之前滑行，可以降低喷嘴中的压力，并保证打印出平滑的连桥。"
                }
                FdmQml_Param{ id:bridge_wall_speed; objectName: "bridge_wall_speed"
                    fdmLabel: "桥壁速度"
                    fdmDescription: "打印桥壁的速度。"
                }
                FdmQml_Param{ id:bridge_wall_material_flow; objectName: "bridge_wall_material_flow"
                    fdmLabel: "桥壁流量"
                    fdmDescription: "打印桥壁时，将挤出的材料量乘以此值。"
                }
                FdmQml_Param{ id:bridge_skin_speed; objectName: "bridge_skin_speed"
                    fdmLabel: "连桥表面速度"
                    fdmDescription: "打印连桥表面区域的速度。"
                }
                FdmQml_Param{ id:bridge_skin_material_flow; objectName: "bridge_skin_material_flow"
                    fdmLabel: "连桥表面流量"
                    fdmDescription: "打印连桥表面区域时，将挤出的材料量乘以此值。"
                }
                FdmQml_Param{ id:bridge_skin_density; objectName: "bridge_skin_density"
                    fdmLabel: "连桥表面密度"
                    fdmDescription: "连桥表面层的密度。此值若小于 100 则会增大表面线条的缝隙。"
                }
                FdmQml_Param{ id:bridge_fan_speed; objectName: "bridge_fan_speed"
                    fdmLabel: "连桥风扇速度"
                    fdmDescription: "打印连桥表面和桥壁时使用的风扇百分比速度。"
                }
                FdmQml_Param{ id:bridge_enable_more_layers; objectName: "bridge_enable_more_layers"
                    fdmLabel: "连桥有多层"
                    fdmDescription: "如果启用此选项，则使用以下设置打印净空区域上方第二层和第三层。否则，将使用正常设置打印这些层。"
                    FdmQml_Param{ id:bridge_skin_speed_2; objectName: "bridge_skin_speed_2"
                        fdmLabel: "连桥第二层表面速度"
                        fdmDescription: "打印桥梁第二层表面时使用的打印速度。"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_2; objectName: "bridge_skin_material_flow_2"
                        fdmLabel: "连桥第二层表面流量"
                        fdmDescription: "打印连桥第二层表面时，将挤出的材料量乘以此值。"
                    }
                    FdmQml_Param{ id:bridge_skin_density_2; objectName: "bridge_skin_density_2"
                        fdmLabel: "连桥第二层表面密度"
                        fdmDescription: "连桥第二层表面的密度。此值若小于 100 则会增大表面线条的缝隙。"
                    }
                    FdmQml_Param{ id:bridge_fan_speed_2; objectName: "bridge_fan_speed_2"
                        fdmLabel: "连桥第二层表面风扇速度"
                        fdmDescription: "打印桥梁第二层表面时使用的风扇百分比速度。"
                    }
                    FdmQml_Param{ id:bridge_skin_speed_3; objectName: "bridge_skin_speed_3"
                        fdmLabel: "连桥第三层表面速度"
                        fdmDescription: "打印桥梁第三层表面时使用的打印速度。"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_3; objectName: "bridge_skin_material_flow_3"
                        fdmLabel: "连桥第三层表面流量"
                        fdmDescription: "打印连桥第三层表面时，将挤出的材料量乘以此值。"
                    }
                    FdmQml_Param{ id:bridge_skin_density_3; objectName: "bridge_skin_density_3"
                        fdmLabel: "连桥第三层表面密度"
                        fdmDescription: "连桥第三层表面的密度。此值若小于 100 则会增大表面线条的缝隙。"
                    }
                    FdmQml_Param{ id:bridge_fan_speed_3; objectName: "bridge_fan_speed_3"
                        fdmLabel: "连桥第三层表面风扇速度"
                        fdmDescription: "打印桥梁第三层表面时使用的风扇百分比速度。"
                    }
                }
            }
            FdmQml_Param{ id:machine_index_name; objectName: "machine_index_name"
                fdmLabel: "机器索引名称"
                fdmDescription: "机器索引名称"
            }
            FdmQml_Param{ id:extruder_index_name; objectName: "extruder_index_name"
                fdmLabel: "挤出机索引名称"
                fdmDescription: "挤出机索引名称"
            }
            FdmQml_Param{ id:material_index_name; objectName: "material_index_name"
                fdmLabel: "挤出机索引名称"
                fdmDescription: "挤出机索引名称"
            }
            FdmQml_Param{ id:parameter_index_name; objectName: "parameter_index_name"
                fdmLabel: "参数索引的名称"
                fdmDescription: "参数索引的名称"
            }
            FdmQml_Param{ id:extract_param_from_gcode; objectName: "extract_param_from_gcode"
                fdmLabel: "预览ankerMake gcode时展示其用到的参数"
                fdmDescription: "预览使用ankerMake生成的gcode时，将生成gcode时用到的参数展现在右侧栏参数设置界面."
            }
            FdmQml_Param{ id:coasting_enable; objectName: "coasting_enable"
                fdmLabel: "启用滑行"
                fdmDescription: "滑行会用一个空驶路径替代挤出路径的最后部分。 渗出材料用于打印挤出路径的最后部分，以便减少串接。"
                FdmQml_Param{ id:coasting_volume; objectName: "coasting_volume"
                    fdmLabel: "滑行体积"
                    fdmDescription: "该体积如不进行滑行则会渗出。 该值一般应接近喷嘴立方直径。"
                }
                FdmQml_Param{ id:coasting_min_volume; objectName: "coasting_min_volume"
                    fdmLabel: "滑行前最小体积"
                    fdmDescription: "挤出路径在可以进行滑行前应拥有的最小体积。 对于较小的挤出路径，鲍登管内累积的压力较少，因此滑行空间采用线性扩展。 该值应始终大于滑行空间。"
                }
                FdmQml_Param{ id:coasting_speed; objectName: "coasting_speed"
                    fdmLabel: "滑行速度"
                    fdmDescription: "滑行期间的移动速度，相对于挤出路径的速度。 建议采用略低于 100% 的值，因为在滑行移动期间鲍登管中的压力会下降。"
                }
            }
        }
        FdmQml_Category{ id:machine_settings; objectName: "machine_settings"
            fdmLabel: "﻿机器"
            fdmDescription: "﻿机器详细设置"
            FdmQml_Param{ id:machine_name; objectName: "machine_name"
                fdmLabel: "﻿机器类型"
                fdmDescription: "﻿您的 3D 打印机型号的名称。"
            }
            FdmQml_Param{ id:machine_show_variants; objectName: "machine_show_variants"
                fdmLabel: "﻿显示机器型号"
                fdmDescription: "这台打印机是否需要显示它在不同的 JSON 文件中所描述的不同变化。"
            }
            FdmQml_Param{ id:machine_start_gcode; objectName: "machine_start_gcode"
                fdmLabel: "开始 G-code"
                fdmDescription: "G代码命令要在最开始执行--用\\n分隔。"
            }
            FdmQml_Param{ id:machine_end_gcode; objectName: "machine_end_gcode"
                fdmLabel: "结束 G-code"
                fdmDescription: "在最后要执行的G代码命令--用\\n分隔。"
            }
            FdmQml_Param{ id:material_guid; objectName: "material_guid"
                fdmLabel: "材料 GUID"
                fdmDescription: "材料 GUID，此项为自动设置。 "
            }
            FdmQml_Param{ id:material_bed_temp_wait; objectName: "material_bed_temp_wait"
                fdmLabel: "等待打印平台加热"
                fdmDescription: "是否插入一条命令，等待开始时达到打印平台温度。"
            }
            FdmQml_Param{ id:material_print_temp_wait; objectName: "material_print_temp_wait"
                fdmLabel: "等待喷嘴加热"
                fdmDescription: "是否等待开始时达到喷嘴温度。"
            }
            FdmQml_Param{ id:material_print_temp_prepend; objectName: "material_print_temp_prepend"
                fdmLabel: "包含材料温度"
                fdmDescription: "是否在 gcode 开始部分包含喷嘴温度命令。 当 start_gcode 已包含喷嘴温度命令时，AnkerMake 前端将自动禁用此设置。"
            }
            FdmQml_Param{ id:material_bed_temp_prepend; objectName: "material_bed_temp_prepend"
                fdmLabel: "包含打印平台温度"
                fdmDescription: "是否需要在 G-code 开始部分包含检查热床温度的命令。当 start_gcode 包含热床温度命令时，AnkerMake 前端将自动禁用此设置。"
            }
            FdmQml_Param{ id:machine_width; objectName: "machine_width"
                fdmLabel: "机器宽度"
                fdmDescription: "机器可打印区域宽度（X 坐标）"
            }
            FdmQml_Param{ id:machine_depth; objectName: "machine_depth"
                fdmLabel: "机器深度"
                fdmDescription: "机器可打印区域深度（Y 坐标）"
            }
            FdmQml_Param{ id:machine_shape; objectName: "machine_shape"
                fdmLabel: "打印平台形状"
                fdmDescription: "打印平台形状（不考虑不可打印区域）。"
                fdmOptions:{
                    "rectangular": "矩形"
                }
            }
            FdmQml_Param{ id:machine_buildplate_type; objectName: "machine_buildplate_type"
                fdmLabel: "打印平台材料"
                fdmDescription: "打印平台材料已安装在打印机上。"
                fdmOptions:{
                    "glass": "玻璃",
                    "aluminum": "铝"
                }
            }
            FdmQml_Param{ id:machine_height; objectName: "machine_height"
                fdmLabel: "机器高度"
                fdmDescription: "机器可打印区域高度（Z 坐标）"
            }
            FdmQml_Param{ id:machine_heated_bed; objectName: "machine_heated_bed"
                fdmLabel: "有加热打印平台"
                fdmDescription: "机器是否有加热打印平台。"
            }
            FdmQml_Param{ id:machine_heated_build_volume; objectName: "machine_heated_build_volume"
                fdmLabel: "具有构建体积温度稳定性"
                fdmDescription: "机器是否能够稳定构建体积温度。"
                FdmQml_Param{ id:build_volume_temperature; objectName: "build_volume_temperature"
                    fdmLabel: "打印体积温度"
                    fdmDescription: "打印环境温度。若为 0，将不会调整构建体积温度。"
                }
            }
            FdmQml_Param{ id:machine_always_write_active_tool; objectName: "machine_always_write_active_tool"
                fdmLabel: "始终写入活动工具"
                fdmDescription: "将临时命令发送到非活动工具后写入活动工具。用 Smoothie 或其他具有模态工具命令的固件进行的双挤出器打印需要此项。"
            }
            FdmQml_Param{ id:machine_center_is_zero; objectName: "machine_center_is_zero"
                fdmLabel: "机器中心为零点"
                fdmDescription: "打印机零位的 X/Y 坐标是否位于可打印区域的中心。"
            }
            FdmQml_Param{ id:machine_extruder_count; objectName: "machine_extruder_count"
                fdmLabel: "挤出机数目"
                fdmDescription: "挤出机组数目。 挤出机组是指进料装置、鲍登管和喷嘴的组合。"
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
                fdmLabel: "已启用的挤出机数目"
                fdmDescription: "已启用的挤出机组数目；软件自动设置"
            }
            FdmQml_Param{ id:machine_nozzle_tip_outer_diameter; objectName: "machine_nozzle_tip_outer_diameter"
                fdmLabel: "喷嘴外径"
                fdmDescription: "喷嘴尖端的外径。"
            }
            FdmQml_Param{ id:machine_nozzle_head_distance; objectName: "machine_nozzle_head_distance"
                fdmLabel: "喷嘴长度"
                fdmDescription: "喷嘴尖端与打印头最低部分之间的高度差。"
            }
            FdmQml_Param{ id:machine_nozzle_expansion_angle; objectName: "machine_nozzle_expansion_angle"
                fdmLabel: "喷嘴角度"
                fdmDescription: "水平面与喷嘴尖端上部圆锥形之间的角度。"
            }
            FdmQml_Param{ id:machine_heat_zone_length; objectName: "machine_heat_zone_length"
                fdmLabel: "加热区长度"
                fdmDescription: "与喷嘴尖端的距离，喷嘴产生的热量在这段距离内传递到耗材中。"
            }
            FdmQml_Param{ id:machine_nozzle_temp_enabled; objectName: "machine_nozzle_temp_enabled"
                fdmLabel: "启用喷嘴温度控制"
                fdmDescription: "是否从AnkerMake控制温度。 关闭此选项，从AnkerMake外部控制喷嘴温度。"
            }
            FdmQml_Param{ id:machine_nozzle_heat_up_speed; objectName: "machine_nozzle_heat_up_speed"
                fdmLabel: "升温速度"
                fdmDescription: "喷嘴升温到平均超过正常打印温度和待机温度范围的速度 (°C/s)。"
            }
            FdmQml_Param{ id:machine_nozzle_cool_down_speed; objectName: "machine_nozzle_cool_down_speed"
                fdmLabel: "冷却速度"
                fdmDescription: "喷嘴冷却到平均超过正常打印温度和待机温度范围的速度 (°C/s)。"
            }
            FdmQml_Param{ id:machine_min_cool_heat_time_window; objectName: "machine_min_cool_heat_time_window"
                fdmLabel: "最短时间待机温度"
                fdmDescription: "挤出机必须保持不活动以便喷嘴冷却的最短时间。 挤出机必须不使用此时间以上，才可以冷却到待机温度。"
            }
            FdmQml_Param{ id:machine_gcode_flavor; objectName: "machine_gcode_flavor"
                fdmLabel: "G-code 风格"
                fdmDescription: "需要生成的 G-code 类型。"
                fdmOptions:{
                    "RepRap (Marlin/Sprinter)": "Marlin",
                    "RepRap (RepRap)": "RepRap",
                    "Griffin": "Griffin",
                    "Makerbot": "Makerbot"
                }
            }
            FdmQml_Param{ id:machine_firmware_retract; objectName: "machine_firmware_retract"
                fdmLabel: "固件收回"
                fdmDescription: "是否使用固件收回命令 (G10/G11) 而不是使用 G1 命令中的 E 属性来收回材料。"
            }
            FdmQml_Param{ id:machine_extruders_share_heater; objectName: "machine_extruders_share_heater"
                fdmLabel: "挤出器共用加热器"
                fdmDescription: "挤出器是否共用一个加热器，而不是每个挤出器都有自己的加热器。"
            }
            FdmQml_Param{ id:machine_extruders_share_nozzle; objectName: "machine_extruders_share_nozzle"
                fdmLabel: "挤出器共用喷嘴"
                fdmDescription: "挤出器是否共用一个喷嘴，而不是每个挤出器都有自己的喷嘴。当设置为 true 时，预计打印机启动 gcode 脚本会将所有挤出器正确设置为已知且相互兼容的初始缩回状态 (零根或一根细丝未缩回)；在这种情况下，会通过“machine_extruders_shared_nozzle_initial_retraction”参数描述每个挤出器的初始缩回状态。"
            }
            FdmQml_Param{ id:machine_extruders_shared_nozzle_initial_retraction; objectName: "machine_extruders_shared_nozzle_initial_retraction"
                fdmLabel: "共用喷嘴初始缩回"
                fdmDescription: "假定在打印机启动 gcode 脚本完成后，每个挤出器的细丝从共用喷嘴头缩回多少；该值应等于或大于喷嘴导管公共部分的长度。"
            }
            FdmQml_Param{ id:machine_disallowed_areas; objectName: "machine_disallowed_areas"
                fdmLabel: "不允许区域"
                fdmDescription: "包含不允许打印头进入区域的多边形列表。"
            }
            FdmQml_Param{ id:machine_head_with_fans_polygon; objectName: "machine_head_with_fans_polygon"
                fdmLabel: "机器头和风扇多边形"
                fdmDescription: "打印头 2D 轮廓图（包含风扇盖）。"
            }
            FdmQml_Param{ id:gantry_height; objectName: "gantry_height"
                fdmLabel: "十字轴高度"
                fdmDescription: "喷嘴尖端与十字轴系统（X 轴和 Y 轴）之间的高度差。"
            }
            FdmQml_Param{ id:machine_use_extruder_offset_to_offset_coords; objectName: "machine_use_extruder_offset_to_offset_coords"
                fdmLabel: "挤出机偏移量"
                fdmDescription: "将挤出器偏移量应用到坐标轴系统。影响所有挤出器。"
            }
            FdmQml_Param{ id:extruder_prime_pos_abs; objectName: "extruder_prime_pos_abs"
                fdmLabel: "绝对挤出机主要位置"
                fdmDescription: "使挤出机主要位置为绝对值，而不是与上一已知打印头位置的相对值。"
            }
            FdmQml_Param{ id:machine_max_feedrate_x; objectName: "machine_max_feedrate_x"
                fdmLabel: "X 轴最大速度"
                fdmDescription: "X 轴方向电机的最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_y; objectName: "machine_max_feedrate_y"
                fdmLabel: "Y 轴最大速度"
                fdmDescription: "Y 轴方向电机的最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_z; objectName: "machine_max_feedrate_z"
                fdmLabel: "Z 轴最大速度"
                fdmDescription: "Z 轴方向电机的最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_e; objectName: "machine_max_feedrate_e"
                fdmLabel: "最大进料速率"
                fdmDescription: "耗材的最大速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_x; objectName: "machine_max_acceleration_x"
                fdmLabel: "X 轴最大加速度"
                fdmDescription: "X 轴方向电机的最大加速度"
            }
            FdmQml_Param{ id:machine_max_acceleration_y; objectName: "machine_max_acceleration_y"
                fdmLabel: "轴最大加速度"
                fdmDescription: "Y 轴方向电机的最大加速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_z; objectName: "machine_max_acceleration_z"
                fdmLabel: "Z 轴最大加速度"
                fdmDescription: "Z 轴方向电机的最大加速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_e; objectName: "machine_max_acceleration_e"
                fdmLabel: "挤出电机最大加速度"
                fdmDescription: "耗材电机的最大加速度。"
            }
            FdmQml_Param{ id:machine_acceleration; objectName: "machine_acceleration"
                fdmLabel: "默认加速度"
                fdmDescription: "打印头移动的默认加速度。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_x; objectName: "machine_steps_per_mm_x"
                fdmLabel: "每毫米步数 (X)"
                fdmDescription: "步进电机前进多少步将导致在 X 方向移动一毫米。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_y; objectName: "machine_steps_per_mm_y"
                fdmLabel: "每毫米步数 (Y)"
                fdmDescription: "步进电机前进多少步将导致在 Y 方向移动一毫米。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_z; objectName: "machine_steps_per_mm_z"
                fdmLabel: "每毫米步数 (Z)"
                fdmDescription: "步进电机前进多少步将导致在 Z 方向移动一毫米。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_e; objectName: "machine_steps_per_mm_e"
                fdmLabel: "每毫米步数 (E)"
                fdmDescription: "步进电机前进多少步将导致进料器轮绕其周长移动一毫米。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_x; objectName: "machine_endstop_positive_direction_x"
                fdmLabel: "正向 X 限位开关"
                fdmDescription: "指定 X 轴的限位开关位于正向（高 X 轴坐标）还是负向（低 X 轴坐标）。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_y; objectName: "machine_endstop_positive_direction_y"
                fdmLabel: "正向 Y 限位开关"
                fdmDescription: "指定 Y 轴的限位开关位于正向（高 Y 轴坐标）还是负向（低 Y 轴坐标）。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_z; objectName: "machine_endstop_positive_direction_z"
                fdmLabel: "正向 Z 限位开关"
                fdmDescription: "指定 Z 轴的限位开关位于正向（高 Z 轴坐标）还是负向（低 Z 轴坐标）。"
            }
            FdmQml_Param{ id:machine_minimum_feedrate; objectName: "machine_minimum_feedrate"
                fdmLabel: "最小进料速率"
                fdmDescription: "打印头的最低移动速度。"
            }
            FdmQml_Param{ id:machine_feeder_wheel_diameter; objectName: "machine_feeder_wheel_diameter"
                fdmLabel: "进料装置驱动轮的直径"
                fdmDescription: "进料装置中材料驱动轮的直径。"
            }
        }
        FdmQml_Category{ id:extruder_settings; objectName: "extruder_settings"
            fdmLabel: "挤出机"
            fdmDescription: "挤出机设置"
            FdmQml_Param{ id:extruder_nr; objectName: "extruder_nr"
                fdmLabel: "挤出机"
                fdmDescription: "用于打印的挤出机，在多挤出机情况下适用。"
            }
            FdmQml_Param{ id:machine_nozzle_id; objectName: "machine_nozzle_id"
                fdmLabel: "喷嘴 ID"
                fdmDescription: "挤出机组的喷嘴 ID，比如\"AA 0.4\"和\"BB 0.8\"。"
            }
            FdmQml_Param{ id:machine_nozzle_size; objectName: "machine_nozzle_size"
                fdmLabel: "喷嘴直径"
                fdmDescription: "喷嘴内径，在使用非标准喷嘴尺寸时需更改此设置。"
            }
            FdmQml_Param{ id:machine_nozzle_offset_x; objectName: "machine_nozzle_offset_x"
                fdmLabel: "喷嘴 X 轴偏移量"
                fdmDescription: "喷嘴 X 轴坐标偏移。"
            }
            FdmQml_Param{ id:machine_nozzle_offset_y; objectName: "machine_nozzle_offset_y"
                fdmLabel: "喷嘴 Y 轴偏移量"
                fdmDescription: "喷嘴 Y 轴坐标偏移。"
            }
            FdmQml_Param{ id:machine_extruder_start_code; objectName: "machine_extruder_start_code"
                fdmLabel: "挤出机的开始 G-code"
                fdmDescription: "在切换到此挤出机时执行的开始 G-code。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_abs; objectName: "machine_extruder_start_pos_abs"
                fdmLabel: "挤出机起点绝对位置"
                fdmDescription: "令挤出机起始位置为绝对位置，而不根据打印头的最后位置来改变。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_x; objectName: "machine_extruder_start_pos_x"
                fdmLabel: "挤出机起始位置 X 坐标"
                fdmDescription: "打开挤出机时起始位置的 X 坐标。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_y; objectName: "machine_extruder_start_pos_y"
                fdmLabel: "挤出机起始位置 Y 坐标"
                fdmDescription: "打开挤压机时的起始位置 Y 坐标。"
            }
            FdmQml_Param{ id:machine_extruder_end_code; objectName: "machine_extruder_end_code"
                fdmLabel: "挤出机的结束 G-code"
                fdmDescription: "在切离此挤出机时执行的结束 G-code。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_abs; objectName: "machine_extruder_end_pos_abs"
                fdmLabel: "挤出机终点绝对位置"
                fdmDescription: "令挤出机结束位置为绝对位置，而不根据打印头的最后位置来改变。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_x; objectName: "machine_extruder_end_pos_x"
                fdmLabel: "挤出机结束位置 X 坐标"
                fdmDescription: "关闭挤出机时的终止位置的 X 坐标。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_y; objectName: "machine_extruder_end_pos_y"
                fdmLabel: "挤出机终点位置 Y 坐标"
                fdmDescription: "关闭挤出机时的终止位置的 Y 坐标。"
            }
            FdmQml_Param{ id:extruder_prime_pos_z; objectName: "extruder_prime_pos_z"
                fdmLabel: "挤出机初始 Z 轴位置"
                fdmDescription: "打印开始时，喷头在 Z 轴坐标上的起始位置."
            }
            FdmQml_Param{ id:machine_extruder_cooling_fan_number; objectName: "machine_extruder_cooling_fan_number"
                fdmLabel: "挤出机打印冷却风扇"
                fdmDescription: "打印冷却风扇的数量与该挤出机有关。仅在每个挤出机都对应不同的打印冷却风扇时，对默认值 0 进行更改。"
            }
            FdmQml_Param{ id:nozzle_disallowed_areas; objectName: "nozzle_disallowed_areas"
                fdmLabel: "喷嘴不允许区域"
                fdmDescription: "包含不允许喷嘴进入区域的多边形列表。"
            }
        }
        FdmQml_Category{ id:resolution; objectName: "resolution"
            fdmLabel: "质量"
            fdmDescription: "影响打印分辨率的所有设置。 这些设置会对质量（和打印时间）产生显著影响"
            FdmQml_Param{ id:layer_height; objectName: "layer_height"
                fdmLabel: "层高"
                fdmDescription: "每层的高度（以毫米为单位）。值越高，则打印速度越快，分辨率越低；值越低，则打印速度越慢，分辨率越高。"
            }
            FdmQml_Param{ id:layer_height_0; objectName: "layer_height_0"
                fdmLabel: "起始层高"
                fdmDescription: "起始层高（以毫米为单位）。起始层越厚，与打印平台的粘着越轻松。"
            }
            FdmQml_Param{ id:line_width; objectName: "line_width"
                fdmLabel: "走线宽度"
                fdmDescription: "单一走线宽度。 一般而言，每条走线的宽度应与喷嘴的宽度对应。 但是，稍微降低此值可以产生更好的打印成果。"
                FdmQml_Param{ id:wall_line_width; objectName: "wall_line_width"
                    fdmLabel: "走线宽度（壁）"
                    fdmDescription: "单一壁线宽度。"
                    FdmQml_Param{ id:wall_line_width_0; objectName: "wall_line_width_0"
                        fdmLabel: "走线宽度（外壁）"
                        fdmDescription: "最外壁线宽度。 降低此值，可打印出更高水平的细节。"
                    }
                    FdmQml_Param{ id:thin_wall_line_width_0; objectName: "thin_wall_line_width_0"
                        fdmLabel: "薄壁走线宽度（外壁）"
                        fdmDescription: "薄壁线宽度。 有一些比较薄的无法生成一个正常壁的部分，可以用这个参数生成一个薄壁，只是针对部分不是某区域线段。"
                    }

                    FdmQml_Param{ id:wall_line_width_x; objectName: "wall_line_width_x"
                        fdmLabel: "走线宽度（内壁）"
                        fdmDescription: "适用于所有壁线（最外壁线除外）的单一壁线宽度。"
                    }
                }
                FdmQml_Param{ id:skin_line_width; objectName: "skin_line_width"
                    fdmLabel: "走线宽度（顶层 / 底层）"
                    fdmDescription: "单一顶层/底层走线宽度。"
                }
                FdmQml_Param{ id:infill_line_width; objectName: "infill_line_width"
                    fdmLabel: "走线宽度（填充）"
                    fdmDescription: "单一填充走线宽度。"
                }
                FdmQml_Param{ id:skirt_brim_line_width; objectName: "skirt_brim_line_width"
                    fdmLabel: "走线宽度（Skirt / Brim）"
                    fdmDescription: "单一 skirt（裙摆）或 brim（边缘）走线宽度。"
                }
                FdmQml_Param{ id:support_line_width; objectName: "support_line_width"
                    fdmLabel: "走线宽度（支撑结构）"
                    fdmDescription: "单一支撑结构走线宽度。"
                }
                FdmQml_Param{ id:support_interface_line_width; objectName: "support_interface_line_width"
                    fdmLabel: "走线宽度（支撑接触面）"
                    fdmDescription: "支撑顶板或底板单一走线宽度。"
                    FdmQml_Param{ id:support_roof_line_width; objectName: "support_roof_line_width"
                        fdmLabel: "支撑顶板走线宽度"
                        fdmDescription: "单一支撑顶板走线宽度。"
                    }
                    FdmQml_Param{ id:support_bottom_line_width; objectName: "support_bottom_line_width"
                        fdmLabel: "支撑底板走线宽度"
                        fdmDescription: "单一支撑底板走线宽度。"
                    }
                }
                FdmQml_Param{ id:prime_tower_line_width; objectName: "prime_tower_line_width"
                    fdmLabel: "装填塔走线宽度"
                    fdmDescription: "单一装填走线宽度。"
                }
            }
            FdmQml_Param{ id:initial_layer_line_width_factor; objectName: "initial_layer_line_width_factor"
                fdmLabel: "起始层走线宽度"
                fdmDescription: "第一层走线宽度乘数。 增大此乘数可改善热床粘着。"
            }
        }
        FdmQml_Category{ id:shell; objectName: "shell"
            fdmLabel: "外壳"
            fdmDescription: "外壳"
            FdmQml_Param{ id:wall_extruder_nr; objectName: "wall_extruder_nr"
                fdmLabel: "壁挤出机"
                fdmDescription: "用于打印壁的挤出机组。 用于多重挤出。"
                FdmQml_Param{ id:wall_0_extruder_nr; objectName: "wall_0_extruder_nr"
                    fdmLabel: "外壁挤出机"
                    fdmDescription: "用于打印外壁的挤出机组。 用于多重挤出。"
                }
                FdmQml_Param{ id:wall_x_extruder_nr; objectName: "wall_x_extruder_nr"
                    fdmLabel: "内壁挤出机"
                    fdmDescription: "用于打印内壁的挤出机组。 用于多重挤出。"
                }
            }
            FdmQml_Param{ id:wall_thickness; objectName: "wall_thickness"
                fdmLabel: "壁厚"
                fdmDescription: "水平方向的壁厚度。 此值除以壁线宽度定义壁数量。"
                FdmQml_Param{ id:wall_line_count; objectName: "wall_line_count"
                    fdmLabel: "壁走线次数"
                    fdmDescription: "壁数量。 在按壁厚计算时，该值舍入为整数。"
                }
            }
            FdmQml_Param{ id:wall_0_wipe_dist; objectName: "wall_0_wipe_dist"
                fdmLabel: "外壁擦嘴长度"
                fdmDescription: "插入外壁后的空驶距离，旨在更好地隐藏 Z 缝。"
            }
            FdmQml_Param{ id:wall_0_inset; objectName: "wall_0_inset"
                fdmLabel: "外壁嵌入"
                fdmDescription: "应用在外壁路径上的嵌入。 如果外壁小于喷嘴，并且在内壁之后打印，则使用该偏移量来使喷嘴中的孔与内壁而不是模型外部重叠。"
            }
            FdmQml_Param{ id:optimize_wall_printing_order; objectName: "optimize_wall_printing_order"
                fdmLabel: "优化壁打印顺序"
                fdmDescription: "优化打印各个壁的顺序，以减少回抽次数和空驶距离。 启用此设置将对大部分零件有益，但有的则会耗费更长时间，因此请将优化和不优化的打印时间估计值进行对比。"
            }
            FdmQml_Param{ id:outer_inset_first; objectName: "outer_inset_first"
                fdmLabel: "先外壁后内壁"
                fdmDescription: "启用时以从外向内的顺序打印壁。 当使用高粘度塑料如 ABS 时，这有助于提高 X 和 Y 的尺寸精度；但是，它可能会降低外表面打印质量，特别是在悬垂部分。"
            }
            FdmQml_Param{ id:alternate_extra_perimeter; objectName: "alternate_extra_perimeter"
                fdmLabel: "交替备用壁"
                fdmDescription: "每隔一层打印一个额外的壁。 通过这种方法，填充物会卡在这些额外的壁之间，从而产生更强韧的打印质量。"
            }
            FdmQml_Param{ id:travel_compensate_overlapping_walls_enabled; objectName: "travel_compensate_overlapping_walls_enabled"
                fdmLabel: "补偿壁重叠"
                fdmDescription: "在壁已经存在时补偿所打印壁部分的流量。"
                FdmQml_Param{ id:travel_compensate_overlapping_walls_0_enabled; objectName: "travel_compensate_overlapping_walls_0_enabled"
                    fdmLabel: "补偿外壁重叠"
                    fdmDescription: "在外壁已经存在时补偿所打印外壁部分的流量。"
                }
                FdmQml_Param{ id:travel_compensate_overlapping_walls_x_enabled; objectName: "travel_compensate_overlapping_walls_x_enabled"
                    fdmLabel: "补偿内壁重叠"
                    fdmDescription: "在内壁已经存在时补偿所打印内壁部分的流量。"
                }
            }
            FdmQml_Param{ id:wall_min_flow; objectName: "wall_min_flow"
                fdmLabel: "最小壁流量"
                fdmDescription: "壁线允许的最小百分比流量。当某个壁靠近现有壁时，壁重叠补偿可减小其流量。流量小于此值的壁将由空驶替代。在使用此设置时，您必须启用壁重叠补偿并在打印内壁之前打印外壁。"
            }
            FdmQml_Param{ id:wall_min_flow_retract; objectName: "wall_min_flow_retract"
                fdmLabel: "首选回抽"
                fdmDescription: "如启用，会使用回抽而不是梳理取代流量低于最小流量阈值的壁的空驶。"
            }
            FdmQml_Param{ id:fill_perimeter_gaps; objectName: "fill_perimeter_gaps"
                fdmLabel: "填充壁之间空隙"
                fdmDescription: "在没有墙壁适合的位置填充墙壁之间的间隙。"
                fdmOptions:{
                    "nowhere": "都不填充",
                    "everywhere": "全部填充"
                }
            }
            FdmQml_Param{ id:filter_out_tiny_gaps; objectName: "filter_out_tiny_gaps"
                fdmLabel: "滤除微小的间隙"
                fdmDescription: "滤除微小的间隙以减少模型外的光点。"
            }
            FdmQml_Param{ id:fill_outline_gaps; objectName: "fill_outline_gaps"
                fdmLabel: "打印薄壁"
                fdmDescription: "打印在水平面上比喷嘴尺寸更薄的模型部件。"
            }
            FdmQml_Param{ id:xy_offset; objectName: "xy_offset"
                fdmLabel: "水平扩展"
                fdmDescription: "应用到每一层所有多边形的偏移量。 正数值可以补偿过大的孔洞；负数值可以补偿过小的孔洞。"
            }
            FdmQml_Param{ id:xy_offset_layer_0; objectName: "xy_offset_layer_0"
                fdmLabel: "起始层水平扩展"
                fdmDescription: "应用到第一层所有多边形的偏移量。 负数值可以补偿第一层的压扁量（被称为“象脚”）。"
            }
            FdmQml_Param{ id:hole_xy_offset; objectName: "hole_xy_offset"
                fdmLabel: "孔洞水平扩展"
                fdmDescription: "应用到每一层中所有孔洞的偏移量。正数值可以补偿过大的孔洞，负数值可以补偿过小的孔洞。"
            }
            FdmQml_Param{ id:z_seam_type; objectName: "z_seam_type"
                fdmLabel: "Z 缝对齐"
                fdmDescription: "一层中每条路径的起点。 当连续多层的路径从相同点开始时，则打印物上会显示一条垂直缝隙。 如果将这些路径靠近一个用户指定的位置对齐，则缝隙最容易移除。 如果随机放置，则路径起点的不精准度将较不明显。 采用最短的路径时，打印将更为快速。"
                fdmOptions:{
                    "back": "用户指定",
                    "shortest": "最短",
                    "random": "随机",
                    "sharpest_corner": "最尖角"
                }
            }
            FdmQml_Param{ id:z_seam_position; objectName: "z_seam_position"
                fdmLabel: "Z 缝位置"
                fdmDescription: "在该位置附近开始打印层中各个部分。"
                fdmOptions:{
                    "backleft": "左后方",
                    "back": "后方",
                    "backright": "右后方",
                    "right": "右侧",
                    "frontright": "右前方",
                    "front": "前方",
                    "frontleft": "左前方",
                    "left": "左侧"
                }
                FdmQml_Param{ id:z_seam_x; objectName: "z_seam_x"
                    fdmLabel: "Z 缝 X"
                    fdmDescription: "位置的 X 轴坐标，在该位置附近开始打印层中各个部分。"
                }
                FdmQml_Param{ id:z_seam_y; objectName: "z_seam_y"
                    fdmLabel: "Z 缝 Y"
                    fdmDescription: "位置的 Y 轴坐标，在该位置附近开始打印层中各个部分。"
                }
            }
            FdmQml_Param{ id:z_seam_corner; objectName: "z_seam_corner"
                fdmLabel: "缝隙角偏好设置"
                fdmDescription: "控制模型轮廓上的角是否影响缝隙的位置。“无”表示各个角不影响缝隙位置。“隐藏缝隙”会使缝隙更可能出现在内侧角上。“外露缝隙”会使缝隙更可能出现在外侧角上。“隐藏或外露缝隙”会使缝隙更可能出现在内侧或外侧角上。“智能隐藏”允许缝隙出现在内侧和外侧角上，如适当，会更多地出现在内侧角上。"
                fdmOptions:{
                    "z_seam_corner_none": "无",
                    "z_seam_corner_inner": "隐藏缝隙",
                    "z_seam_corner_outer": "外露缝隙",
                    "z_seam_corner_any": "隐藏或外露缝隙",
                    "z_seam_corner_weighted": "智能隐藏"
                }
            }
            FdmQml_Param{ id:z_seam_relative; objectName: "z_seam_relative"
                fdmLabel: "Z 缝相对"
                fdmDescription: "启用时，Z 缝坐标为相对于各个部分中心的值。 禁用时，坐标定义打印平台上的一个绝对位置。"
            }
        }
        FdmQml_Category{ id:top_bottom; objectName: "top_bottom"
            fdmLabel: "顶 / 底层"
            fdmDescription: "顶 / 底层"
            FdmQml_Param{ id:top_bottom_extruder_nr; objectName: "top_bottom_extruder_nr"
                fdmLabel: "顶部/底部挤出机"
                fdmDescription: "用于打印顶部和底部皮肤的挤出机组。 用于多重挤出。"
            }
            FdmQml_Param{ id:top_bottom_thickness; objectName: "top_bottom_thickness"
                fdmLabel: "顶层 / 底层厚度"
                fdmDescription: "打印品中顶层/底层的厚度。 该值除以层高定义顶层/底层的数量。"
                FdmQml_Param{ id:top_thickness; objectName: "top_thickness"
                    fdmLabel: "顶层厚度"
                    fdmDescription: "打印品中顶层的厚度。 该值除以层高定义顶层的数量。"
                    FdmQml_Param{ id:top_layers; objectName: "top_layers"
                        fdmLabel: "顶部层数"
                        fdmDescription: "顶层的数量。 在按顶层厚度计算时，该值舍入为整数。"
                    }
                }
                FdmQml_Param{ id:bottom_thickness; objectName: "bottom_thickness"
                    fdmLabel: "底层厚度"
                    fdmDescription: "打印品中底层的厚度。 此值除以层高定义底层数量。"
                    FdmQml_Param{ id:bottom_layers; objectName: "bottom_layers"
                        fdmLabel: "底部层数"
                        fdmDescription: "底层的数量。 在按底层厚度计算时，该值舍入为整数。"
                    }
                    FdmQml_Param{ id:initial_bottom_layers; objectName: "initial_bottom_layers"
                        fdmLabel: "初始底层数"
                        fdmDescription: "从构建板向上算起的初始底层数。在按底层厚度计算时，该值四舍五入为整数。"
                    }
                }
            }
            FdmQml_Param{ id:roofing_extruder_nr; objectName: "roofing_extruder_nr"
                fdmLabel: "顶部皮肤挤出机"
                fdmDescription: "用于打印最顶部皮肤的挤出机组。 用于多重挤出。"
            }
            FdmQml_Param{ id:roofing_layer_count; objectName: "roofing_layer_count"
                fdmLabel: "顶部表面皮肤层"
                fdmDescription: "最顶部皮肤层数。 通常只需一层最顶层就足以生成较高质量的顶部表面。"
            }
            FdmQml_Param{ id:top_bottom_pattern; objectName: "top_bottom_pattern"
                fdmLabel: "顶部 / 底部走线图案"
                fdmDescription: "顶层/底层图案。"
                fdmOptions:{
                    "lines": "直线",
                    "concentric": "同心圆",
                    "zigzag": "锯齿状"
                }
            }
            FdmQml_Param{ id:top_bottom_pattern_0; objectName: "top_bottom_pattern_0"
                fdmLabel: "底层图案起始层"
                fdmDescription: "打印品底部第一层上的图案。"
                fdmOptions:{
                    "lines": "直线",
                    "concentric": "同心圆",
                    "zigzag": "锯齿状"
                }
            }
            FdmQml_Param{ id:connect_skin_polygons; objectName: "connect_skin_polygons"
                fdmLabel: "连接顶部/底部多边形"
                fdmDescription: "在顶部/底部皮肤路径互相紧靠运行的地方连接它们。对于同心图案，启用此设置可大大减少空驶时间，但由于连接可在填充中途发生，此功能可能会降低顶部表面质量。"
            }
            FdmQml_Param{ id:skin_monotonic; objectName: "skin_monotonic"
                fdmLabel: "单调顶部/底部顺序"
                fdmDescription: "按照一定的顺序打印顶部/底部走线，使它们始终在一个方向上与相邻的走线重叠。这需要更长一些的打印时间，但会使平面看起来更一致。"
            }
            FdmQml_Param{ id:skin_angles; objectName: "skin_angles"
                fdmLabel: "顶层/底层走线方向"
                fdmDescription: "当顶层/底层采用线条或锯齿状图案时使用的整数走线方向的列表。 列表中的元素随层的进度依次使用，当达到列表末尾时，它将从头开始。 列表项以逗号分隔，整个列表包含在方括号中。 默认是一个空列表，即意味着使用传统的默认角度（45 和 135 度）。"
            }
            FdmQml_Param{ id:skin_no_small_gaps_heuristic; objectName: "skin_no_small_gaps_heuristic"
                fdmLabel: "Z 间隙内无表层"
                fdmDescription: "当模型中只有几个分层有微小垂直间隙时，通常狭窄空间的分层周围应有表层。如果垂直间隙非常小，则启用此设置不生成表层。这缩短了打印时间和切片时间，但从技术方面看，会使填充物暴露在空气中。"
            }
            FdmQml_Param{ id:skin_outline_count; objectName: "skin_outline_count"
                fdmLabel: "额外皮肤壁计数"
                fdmDescription: "用多个同心线代替顶部/底部图案的最外面部分。 使用一条或两条线改善从填充材料开始的顶板。"
            }
            FdmQml_Param{ id:top_skin_density; objectName: "top_skin_density"
                fdmLabel: "顶部皮肤层密度"
                fdmDescription: "可以调整顶部皮肤层密度，最低100"
            }
            FdmQml_Param{ id:top_surface_one_wall; objectName: "top_surface_one_wall"
                fdmLabel: "顶部使用单层墙"
                fdmDescription: "顶部使用单层墙，以使顶部留取更多的空间来填充图案"
            }
            FdmQml_Param{ id:ironing_enabled; objectName: "ironing_enabled"
                fdmLabel: "启用熨平"
                fdmDescription: "再次经过顶部表面，但这次挤出的材料非常少。这意味着将进一步熔化顶部的塑料，形成更平滑的表面。喷嘴室中的压力保持很高，确保表面折痕中也能填充材料，以保证细节。"
            }
            FdmQml_Param{ id:ironing_only_highest_layer; objectName: "ironing_only_highest_layer"
                fdmLabel: "仅熨平最高层"
                fdmDescription: "仅在网格的最后一层执行熨平。 如果较低的层不需要平滑的表面效果，这将节省时间。"
            }
            FdmQml_Param{ id:ironing_pattern; objectName: "ironing_pattern"
                fdmLabel: "熨平图案"
                fdmDescription: "用于熨平顶部表面的图案。"
                fdmOptions:{
                    "concentric": "同心",
                    "zigzag": "锯齿形"
                }
            }
            FdmQml_Param{ id:ironing_monotonic; objectName: "ironing_monotonic"
                fdmLabel: "单调熨平顺序"
                fdmDescription: "按照一定的顺序打印熨平走线，使它们始终在一个方向上与相邻的走线重叠。这需要更长一些的打印时间，但会使平面看起来更一致。"
            }
            FdmQml_Param{ id:ironing_line_spacing; objectName: "ironing_line_spacing"
                fdmLabel: "熨平走线间距"
                fdmDescription: "熨平走线之间的距离。"
            }
            FdmQml_Param{ id:ironing_flow; objectName: "ironing_flow"
                fdmLabel: "熨平流量"
                fdmDescription: "熨平期间相对于正常皮肤走线的挤出材料量。 保持喷嘴填充状态有助于填充顶层表面的一些缝隙，但如填充过多则会导致表面上过度挤出和光点。"
            }
            FdmQml_Param{ id:ironing_inset; objectName: "ironing_inset"
                fdmLabel: "熨平嵌入"
                fdmDescription: "与模型边缘保持的距离。 一直熨平至网格的边缘可能导致打印品出现锯齿状边缘。"
            }
            FdmQml_Param{ id:speed_ironing; objectName: "speed_ironing"
                fdmLabel: "熨平速度"
                fdmDescription: "通过顶部表面的速度。"
            }
            FdmQml_Param{ id:acceleration_ironing; objectName: "acceleration_ironing"
                fdmLabel: "熨平加速度"
                fdmDescription: "执行熨平的加速度。"
            }
            FdmQml_Param{ id:jerk_ironing; objectName: "jerk_ironing"
                fdmLabel: "熨平抖动速度"
                fdmDescription: "执行熨平时的最大瞬时速度变化。"
            }
            FdmQml_Param{ id:skin_overlap; objectName: "skin_overlap"
                fdmLabel: "皮肤重叠百分比"
                fdmDescription: "皮肤和壁之间的重叠量，以走线宽度百分比表示。 稍微重叠可让各个壁与皮肤牢固连接。 这是皮肤线平均走线宽度和最内壁的百分比。"
                FdmQml_Param{ id:skin_overlap_mm; objectName: "skin_overlap_mm"
                    fdmLabel: "皮肤重叠"
                    fdmDescription: "皮肤和壁之间的重叠量。 稍微重叠可让各个壁与皮肤牢固连接。"
                }
            }
            FdmQml_Param{ id:skin_preshrink; objectName: "skin_preshrink"
                fdmLabel: "肤移除宽度"
                fdmDescription: "将被移除的皮肤区域的最大宽度。 小于此值的所有皮肤区域都将消失。 这有助于限制在模型的倾斜表面打印顶部/底部皮肤时所耗用的时间和材料。"
                FdmQml_Param{ id:top_skin_preshrink; objectName: "top_skin_preshrink"
                    fdmLabel: "顶部皮肤移除宽度"
                    fdmDescription: "将被移除的顶部皮肤区域的最大宽度。 小于此值的所有皮肤区域都将消失。 这有助于限制在模型的倾斜表面打印顶部皮肤时所耗用的时间和材料。"
                }
                FdmQml_Param{ id:bottom_skin_preshrink; objectName: "bottom_skin_preshrink"
                    fdmLabel: "底部皮肤移除宽度"
                    fdmDescription: "将被移除的底部皮肤区域的最大宽度。 小于此值的所有皮肤区域都将消失。 这有助于限制在模型的倾斜表面打印底部皮肤时所耗用的时间和材料。"
                }
            }
            FdmQml_Param{ id:expand_skins_expand_distance; objectName: "expand_skins_expand_distance"
                fdmLabel: "皮肤扩展距离"
                fdmDescription: "皮肤扩展到填充物中的距离。 值越大会让皮肤与填充图案更好地附着，并让相邻层的层壁与皮肤更好地粘着。 较低的值将节省所用的材料量。"
                FdmQml_Param{ id:top_skin_expand_distance; objectName: "top_skin_expand_distance"
                    fdmLabel: "顶部皮肤扩展距离"
                    fdmDescription: "顶部皮肤扩展到填充物中的距离。 值越大会让皮肤与填充图案更好地附着，并让上方层的层壁与皮肤更好地粘着。 较低的值将节省所用的材料量。"
                }
                FdmQml_Param{ id:bottom_skin_expand_distance; objectName: "bottom_skin_expand_distance"
                    fdmLabel: "底部皮肤扩展距离"
                    fdmDescription: "底部皮肤扩展到填充物中的距离。 值越大会让皮肤与填充图案更好地附着，并让皮肤与下面层的壁更好地粘着。 较低的值将节省所用的材料量。"
                }
            }
            FdmQml_Param{ id:max_skin_angle_for_expansion; objectName: "max_skin_angle_for_expansion"
                fdmLabel: "最大扩展皮肤角度"
                fdmDescription: "如果模型的顶部和/或底部表面的角度大于此设置，则不要扩展其顶部/底部皮肤。 这会避免扩展在模型表面有接近垂直的坡度时所形成的狭窄皮肤区域。 0° 的角为水平，90° 的角为垂直。"
                FdmQml_Param{ id:min_skin_width_for_expansion; objectName: "min_skin_width_for_expansion"
                    fdmLabel: "最小扩展皮肤宽度"
                    fdmDescription: "如果皮肤区域宽度小于此值，则不会扩展。 这会避免扩展在模型表面的坡度接近垂直时所形成的狭窄皮肤区域。"
                }
            }
        }
        FdmQml_Category{ id:infill; objectName: "infill"
            fdmLabel: "填充"
            fdmDescription: "填充"
            FdmQml_Param{ id:infill_extruder_nr; objectName: "infill_extruder_nr"
                fdmLabel: "填充挤出机"
                fdmDescription: "用于打印填充的挤出机组。 用于多重挤出。"
            }
            FdmQml_Param{ id:infill_sparse_density; objectName: "infill_sparse_density"
                fdmLabel: "填充密度"
                fdmDescription: "调整打印填充的密度。"
                FdmQml_Param{ id:infill_line_distance; objectName: "infill_line_distance"
                    fdmLabel: "填充走线距离"
                    fdmDescription: "打印填充走线之间的距离。 该设置是通过填充密度和填充线宽度计算。"
                }
            }
            FdmQml_Param{ id:infill_pattern; objectName: "infill_pattern"
                fdmLabel: "填充图案"
                fdmDescription: "打印填充材料的图案。线条和锯齿形填充在交替层上交换方向，从而降低材料成本。网格、三角形、内六角、立方体、八角形、四面体、交叉和同心图案在每层完整打印。螺旋二十四面体、立方体、四面体和八角形填充随每层变化，以在各个方向提供更均衡的强度分布。"
                fdmOptions:{
                    "grid": "网格",
                    "lines": "直线",
                    "triangles": "三角形",
                    "trihexagon": "内六角",
                    "cubic": "立方体",
                    "cubicsubdiv": "立方体分区",
                    "tetrahedral": "八角形",
                    "quarter_cubic": "四面体",
                    "concentric": "同心圆",
                    "zigzag": "锯齿状",
                    "cross": "交叉",
                    "cross_3d": "交叉 3D",
                    "gyroid": "螺旋二十四面体"
                }
            }
            FdmQml_Param{ id:zig_zaggify_infill; objectName: "zig_zaggify_infill"
                fdmLabel: "连接填充走线"
                fdmDescription: "使用沿内壁形状的走线连接填充图案与内壁相接的各端。 启用此设置会使填充更好地粘着在壁上，减少填充物效果对垂直表面质量的影响。 禁用此设置可减少使用的材料量。"
            }
            FdmQml_Param{ id:connect_infill_polygons; objectName: "connect_infill_polygons"
                fdmLabel: "连接填充多边形"
                fdmDescription: "在填充路径互相紧靠运行的地方连接它们。对于包含若干闭合多边形的填充图案，启用此设置可大大减少空驶时间。"
            }
            FdmQml_Param{ id:infill_angles; objectName: "infill_angles"
                fdmLabel: "填充走线方向"
                fdmDescription: "要使用的整数走线方向列表。 列表中的元素随层的进度依次使用，当达到列表末尾时，它将从头开始。 列表项以逗号分隔，整个列表包含在方括号中。 默认是一个空列表，即意味着使用传统的默认角度（线条和锯齿形图案为 45 和 135 度，其他所有图案为 45 度）。"
            }
            FdmQml_Param{ id:infill_offset_x; objectName: "infill_offset_x"
                fdmLabel: "填充 X 轴偏移量"
                fdmDescription: "填充图案沿 X 轴移动此距离。"
            }
            FdmQml_Param{ id:infill_offset_y; objectName: "infill_offset_y"
                fdmLabel: "填充 Y 轴偏移量"
                fdmDescription: "填充图案沿 Y 轴移动此距离。"
            }
            FdmQml_Param{ id:infill_randomize_start_location; objectName: "infill_randomize_start_location"
                fdmLabel: "开始随机化填充"
                fdmDescription: "先随机化打印哪条填充线。这可以防止一个部分变强，但会导致一次额外的空驶。"
            }
            FdmQml_Param{ id:infill_multiplier; objectName: "infill_multiplier"
                fdmLabel: "填充走线乘数"
                fdmDescription: "将每个填充走线转换成这种多重走线。额外走线互相不交叉，而是互相避开。这使得填充更严格，但会增加打印时间和材料使用。"
            }
            FdmQml_Param{ id:infill_wall_line_count; objectName: "infill_wall_line_count"
                fdmLabel: "额外填充壁计数"
                fdmDescription: ""
            }
            FdmQml_Param{ id:sub_div_rad_add; objectName: "sub_div_rad_add"
                fdmLabel: "立方体分区外壳"
                fdmDescription: "从每个立方体的中心对半径进行添加，以检查模型的边界，进而决定是否应对此立方体进行分区。 值越大则模型边界附近的小型立方体外壳越厚。"
            }
            FdmQml_Param{ id:infill_overlap; objectName: "infill_overlap"
                fdmLabel: "填充重叠百分比"
                fdmDescription: "填充物和壁之间的重叠量占填充走线宽度的百分比。稍微重叠可让各个壁与填充物牢固连接。"
                FdmQml_Param{ id:infill_overlap_mm; objectName: "infill_overlap_mm"
                    fdmLabel: "填充重叠"
                    fdmDescription: "填充物和壁之间的重叠量。 稍微重叠可让各个壁与填充物牢固连接。"
                }
            }
            FdmQml_Param{ id:infill_wipe_dist; objectName: "infill_wipe_dist"
                fdmLabel: "填充物擦拭距离"
                fdmDescription: "每条填充走线后插入的空驶距离，让填充物更好地粘着到壁上。 此选项与填充重叠类似，但没有挤出，且仅位于填充走线的一端。"
            }
            FdmQml_Param{ id:infill_sparse_thickness; objectName: "infill_sparse_thickness"
                fdmLabel: "填充层厚度"
                fdmDescription: "填充材料每层的厚度。 该值应始终为层高的乘数，否则应进行舍入。"
            }
            FdmQml_Param{ id:gradual_infill_steps; objectName: "gradual_infill_steps"
                fdmLabel: "渐进填充步阶"
                fdmDescription: "在进入顶部表面以下时，将填充密度减少一半的次数。 越靠近顶面的区域密度越高，最高达到填充密度。"
            }
            FdmQml_Param{ id:gradual_infill_step_height; objectName: "gradual_infill_step_height"
                fdmLabel: "渐进填充步阶高度"
                fdmDescription: "在切换至密度的一半前指定密度的填充高度。"
            }
            FdmQml_Param{ id:infill_before_walls; objectName: "infill_before_walls"
                fdmLabel: "先填充物后壁"
                fdmDescription: "打印壁前先打印填充物。 先打印壁可能产生更精确的壁，但悬垂打印质量会较差。 先打印填充会产生更牢固的壁，但有时候填充图案会透过表面显现出来。"
            }
            FdmQml_Param{ id:min_infill_area; objectName: "min_infill_area"
                fdmLabel: "最小填充区域"
                fdmDescription: "不要生成小于此面积的填充区域（使用皮肤取代）。"
            }
            FdmQml_Param{ id:infill_support_enabled; objectName: "infill_support_enabled"
                fdmLabel: "填充支撑"
                fdmDescription: "只在模型的顶部支持打印填充结构。这样可以减少打印时间和材料的使用，但是会导致不一致的对象强度。"
            }
            FdmQml_Param{ id:infill_support_angle; objectName: "infill_support_angle"
                fdmLabel: "填充悬垂角"
                fdmDescription: "添加内填充的内部覆盖的最小角度。在一个0的值中，完全填满了填充，90将不提供任何填充。"
            }
            FdmQml_Param{ id:skin_edge_support_thickness; objectName: "skin_edge_support_thickness"
                fdmLabel: "皮肤边缘支撑厚度"
                fdmDescription: "支撑皮肤边缘的额外填充物的厚度。"
                FdmQml_Param{ id:skin_edge_support_layers; objectName: "skin_edge_support_layers"
                    fdmLabel: "皮肤边缘支撑层数"
                    fdmDescription: "支撑皮肤边缘的填充物的层数。"
                }
            }
        }
        FdmQml_Category{ id:material; objectName: "material"
            fdmLabel: "材料"
            fdmDescription: "材料"
            FdmQml_Param{ id:material_diameter; objectName: "material_diameter"
                fdmLabel: "直径"
                fdmDescription: "调整所用耗材的直径。 将此值与所用耗材的直径匹配。"
            }
            FdmQml_Param{ id:material_density; objectName: "material_density"
                fdmLabel: "密度"
                fdmDescription: "﻿调整所用耗材的密度。 将此值与所用耗材的密度匹配。"
                FdmQml_Param{ id:material_linear_density; objectName: "material_linear_density"
                    fdmLabel: "线密度"
                    fdmDescription: "﻿调整所用耗材的线密度。 将此值与所用耗材的线密度匹配。"
                }
            }
            FdmQml_Param{ id:default_material_print_temperature; objectName: "default_material_print_temperature"
                fdmLabel: "默认打印温度"
                fdmDescription: "用于打印的默认温度。 应为材料的\"基本\"温度。 所有其他打印温度均应使用基于此值的偏移量"
            }
            FdmQml_Param{ id:material_print_temperature; objectName: "material_print_temperature"
                fdmLabel: "打印温度"
                fdmDescription: "用于打印的温度。"
                FdmQml_Param{ id:before_homing_temperature; objectName: "before_homing_temperature"
                    fdmLabel: "Homeing前温度"
                    fdmDescription: "﻿Homeing前温度"
                }
                FdmQml_Param{ id:material_print_temperature_layer_0; objectName: "material_print_temperature_layer_0"
                    fdmLabel: "打印温度起始层"
                    fdmDescription: "用于打印第一层的温度。 设为 0 即禁用对起始层的特别处理。"
                }
                FdmQml_Param{ id:material_initial_print_temperature; objectName: "material_initial_print_temperature"
                    fdmLabel: "起始打印温度"
                    fdmDescription: "加热到可以开始打印的打印温度时的最低温度。"
                }
                FdmQml_Param{ id:material_final_print_temperature; objectName: "material_final_print_temperature"
                    fdmLabel: "最终打印温度"
                    fdmDescription: "打印结束前开始冷却的温度。"
                }
                FdmQml_Param{ id:default_material_bed_temperature; objectName: "default_material_bed_temperature"
                    fdmLabel: "默认打印平台温度"
                    fdmDescription: "用于加热打印平台的默认温度。这应该作为打印平台的“基础”温度。所有其他打印温度均应基于此值进行调整"
                }
                FdmQml_Param{ id:material_bed_temperature; objectName: "material_bed_temperature"
                    fdmLabel: "打印平台温度"
                    fdmDescription: "用于加热构建板的温度。如果此项为 0，则保持不加热构建板。"
                }
                FdmQml_Param{ id:material_bed_temperature_layer_0; objectName: "material_bed_temperature_layer_0"
                    fdmLabel: "打印平台温度起始层"
                    fdmDescription: "打印第一层时用于加热构建板的温度。如果此项为 0，则在打印第一层期间保持不加热构建板。"
                }
            }
            FdmQml_Param{ id:material_extrusion_cool_down_speed; objectName: "material_extrusion_cool_down_speed"
                fdmLabel: "挤出冷却速度调节器"
                fdmDescription: "挤出时喷嘴冷却的额外速度。 使用相同的值表示挤出过程中进行加热时的加热速度损失。"
            }
            FdmQml_Param{ id:material_adhesion_tendency; objectName: "material_adhesion_tendency"
                fdmLabel: "附着倾向"
                fdmDescription: "表面附着倾向。"
            }
            FdmQml_Param{ id:material_surface_energy; objectName: "material_surface_energy"
                fdmLabel: "表面能"
                fdmDescription: "表面能。"
            }
            FdmQml_Param{ id:material_shrinkage_percentage; objectName: "material_shrinkage_percentage"
                fdmLabel: "缩放因子收缩补偿"
                fdmDescription: "为了补偿材料在冷却时的收缩，将用此因子缩放模型。"
            }
            FdmQml_Param{ id:material_crystallinity; objectName: "material_crystallinity"
                fdmLabel: "晶体材料"
                fdmDescription: "该材料为受热后脱落干净的类型（晶体），还是会产生长交织状聚合物链的类型（非晶体）？"
            }
            FdmQml_Param{ id:material_anti_ooze_retracted_position; objectName: "material_anti_ooze_retracted_position"
                fdmLabel: "防渗出回抽位置"
                fdmDescription: "材料在停止渗出前所需的回抽长度。"
            }
            FdmQml_Param{ id:material_anti_ooze_retraction_speed; objectName: "material_anti_ooze_retraction_speed"
                fdmLabel: "防渗出回抽速度"
                fdmDescription: "在耗材用于防渗出过程中材料所需的回抽速率。"
            }
            FdmQml_Param{ id:material_break_preparation_retracted_position; objectName: "material_break_preparation_retracted_position"
                fdmLabel: "断裂缓冲期回抽位置"
                fdmDescription: "耗材受热拉伸但不断裂的极限长度。"
            }
            FdmQml_Param{ id:material_break_preparation_speed; objectName: "material_break_preparation_speed"
                fdmLabel: "断裂缓冲期回抽速度"
                fdmDescription: "耗材在回抽过程中恰好折断的回抽速率。"
            }
            FdmQml_Param{ id:material_break_preparation_temperature; objectName: "material_break_preparation_temperature"
                fdmLabel: "断裂缓冲期温度"
                fdmDescription: "用于清除材料的温度，应大致等于可达到的最高打印温度。"
            }
            FdmQml_Param{ id:material_break_retracted_position; objectName: "material_break_retracted_position"
                fdmLabel: "断裂回抽位置"
                fdmDescription: "为完全脱落耗材而抽回耗材的长度。"
            }
            FdmQml_Param{ id:material_break_speed; objectName: "material_break_speed"
                fdmLabel: "断裂回抽速度"
                fdmDescription: "为完全脱落耗材而抽回耗材的速度。"
            }
            FdmQml_Param{ id:material_break_temperature; objectName: "material_break_temperature"
                fdmLabel: "折断温度"
                fdmDescription: "耗材在完全脱落时的温度。"
            }
            FdmQml_Param{ id:material_flush_purge_speed; objectName: "material_flush_purge_speed"
                fdmLabel: "冲洗清除速度"
                fdmDescription: "切换到其他材料后，装填材料的速度如何。"
            }
            FdmQml_Param{ id:material_flush_purge_length; objectName: "material_flush_purge_length"
                fdmLabel: "冲洗清除长度"
                fdmDescription: "切换到其他材料时，需要使用多少材料从喷嘴中清除之前的材料（以长丝长度计）。"
            }
            FdmQml_Param{ id:material_end_of_filament_purge_speed; objectName: "material_end_of_filament_purge_speed"
                fdmLabel: "耗材末端清除速度"
                fdmDescription: "将空线轴替换为使用相同材料的新线轴后，装填材料的速度如何。"
            }
            FdmQml_Param{ id:material_end_of_filament_purge_length; objectName: "material_end_of_filament_purge_length"
                fdmLabel: "耗材末端清除长度"
                fdmDescription: "将空线轴替换为使用相同材料的新线轴后，需要使用多少材料从喷嘴中清除之前的材料（以长丝长度计）。"
            }
            FdmQml_Param{ id:material_maximum_park_duration; objectName: "material_maximum_park_duration"
                fdmLabel: "最长停放持续时间"
                fdmDescription: "材料能在干燥存储区之外安全存放多长时间。"
            }
            FdmQml_Param{ id:material_no_load_move_factor; objectName: "material_no_load_move_factor"
                fdmLabel: "空载移动系数"
                fdmDescription: "表示长丝在进料器和喷嘴室之间被压缩多少的系数，用于确定针对长丝开关将材料移动的距离。"
            }
            FdmQml_Param{ id:material_flow; objectName: "material_flow"
                fdmLabel: "流量"
                fdmDescription: "流量补偿：挤出的材料量乘以此值。"
                FdmQml_Param{ id:wall_material_flow; objectName: "wall_material_flow"
                    fdmLabel: "壁流量"
                    fdmDescription: "壁走线的流量补偿。"
                    FdmQml_Param{ id:wall_0_material_flow; objectName: "wall_0_material_flow"
                        fdmLabel: "外壁流量"
                        fdmDescription: "最外壁走线的流量补偿。"
                    }
                    FdmQml_Param{ id:wall_x_material_flow; objectName: "wall_x_material_flow"
                        fdmLabel: "内壁流量"
                        fdmDescription: "适用于所有壁走线（最外壁走线除外）的流量补偿。"
                    }
                }
                FdmQml_Param{ id:skin_material_flow; objectName: "skin_material_flow"
                    fdmLabel: "顶部/底部流量"
                    fdmDescription: "顶部/底部走线的流量补偿。"
                }
                FdmQml_Param{ id:roofing_material_flow; objectName: "roofing_material_flow"
                    fdmLabel: "顶部表层流量"
                    fdmDescription: "打印顶部区域走线的流量补偿。"
                }
                FdmQml_Param{ id:infill_material_flow; objectName: "infill_material_flow"
                    fdmLabel: "填充流量"
                    fdmDescription: "填充走线的流量补偿。"
                }
                FdmQml_Param{ id:skirt_brim_material_flow; objectName: "skirt_brim_material_flow"
                    fdmLabel: "裙边/边缘流量"
                    fdmDescription: "裙边或边缘走线的流量补偿。"
                }
                FdmQml_Param{ id:support_material_flow; objectName: "support_material_flow"
                    fdmLabel: "支撑流量"
                    fdmDescription: "支撑结构走线的流量补偿。"
                }
                FdmQml_Param{ id:support_interface_material_flow; objectName: "support_interface_material_flow"
                    fdmLabel: "支撑接触面流量"
                    fdmDescription: "支撑顶板或底板走线的流量补偿。"
                    FdmQml_Param{ id:support_roof_material_flow; objectName: "support_roof_material_flow"
                        fdmLabel: "支撑顶板流量"
                        fdmDescription: "支撑顶板走线的流量补偿。"
                    }
                    FdmQml_Param{ id:support_bottom_material_flow; objectName: "support_bottom_material_flow"
                        fdmLabel: "支撑底板流量"
                        fdmDescription: "支撑底板走线的流量补偿。"
                    }
                }
                FdmQml_Param{ id:prime_tower_flow; objectName: "prime_tower_flow"
                    fdmLabel: "装填塔流量"
                    fdmDescription: "装填塔走线的流量补偿。"
                }
            }
            FdmQml_Param{ id:material_flow_layer_0; objectName: "material_flow_layer_0"
                fdmLabel: "起始层流量"
                fdmDescription: "第一层的流量补偿：起始层挤出的材料量乘以此值。"
            }
            FdmQml_Param{ id:material_standby_temperature; objectName: "material_standby_temperature"
                fdmLabel: "待机温度"
                fdmDescription: "当另一个喷嘴正用于打印时该喷嘴的温度。"
            }
        }
        FdmQml_Category{ id:speed; objectName: "speed"
            fdmLabel: "速度"
            fdmDescription: "速度"
            FdmQml_Param{ id:speed_travel; objectName: "speed_travel"
                fdmLabel: "空驶速度"
                fdmDescription: "进行空驶的速度。"
            }
            FdmQml_Param{ id:speed_print; objectName: "speed_print"
                fdmLabel: "打印速度"
                fdmDescription: "打印发生的速度。"
                FdmQml_Param{ id:speed_infill; objectName: "speed_infill"
                    fdmLabel: "填充速度"
                    fdmDescription: "打印填充的速度。"
                }
                FdmQml_Param{ id:speed_wall; objectName: "speed_wall"
                    fdmLabel: "速度（壁）"
                    fdmDescription: "打印壁的速度。"
                    FdmQml_Param{ id:speed_wall_0; objectName: "speed_wall_0"
                        fdmLabel: "速度（外壁）"
                        fdmDescription: "打印最外壁的速度。 以较低速度打印外壁可改善最终皮肤质量。 但是，如果内壁速度和外壁速度差距过大，则将对质量产生负面影响。"
                    }
                    FdmQml_Param{ id:speed_wall_x; objectName: "speed_wall_x"
                        fdmLabel: "速度（内壁）"
                        fdmDescription: "打印所有内壁的速度。 以比外壁更快的速度打印内壁将减少打印时间。 将该值设为外壁速度和填充速度之间也可行。"
                    }
                }
                FdmQml_Param{ id:speed_topbottom; objectName: "speed_topbottom"
                    fdmLabel: "速度（顶部 / 底部）"
                    fdmDescription: "打印顶部/底部层的速度。"
                    FdmQml_Param{ id:speed_roofing; objectName: "speed_roofing"
                        fdmLabel: "顶部表面皮肤速度"
                        fdmDescription: "打印顶部表面皮肤层的速度。"
                    }
                }
                FdmQml_Param{ id:speed_support; objectName: "speed_support"
                    fdmLabel: "速度（支撑结构）"
                    fdmDescription: "打印支撑结构的速度。 以更高的速度打印支撑可极大地缩短打印时间。 支撑结构的表面质量并不重要，因为在打印后会将其移除。"
                    FdmQml_Param{ id:speed_support_infill; objectName: "speed_support_infill"
                        fdmLabel: "速度（支撑填充）"
                        fdmDescription: "打印支撑填充物的速度。 以较低的速度打印填充物可改善稳定性。"
                    }
                    FdmQml_Param{ id:speed_support_interface; objectName: "speed_support_interface"
                        fdmLabel: "支撑接触面速度"
                        fdmDescription: "打印支撑顶板和底板的速度。 以较低的速度打印可以改善悬垂质量。"
                        FdmQml_Param{ id:speed_support_roof; objectName: "speed_support_roof"
                            fdmLabel: "支撑顶板速度"
                            fdmDescription: "打印支撑顶板的速度。 以较低的速度打印可以改善悬垂质量。"
                        }
                        FdmQml_Param{ id:speed_support_bottom; objectName: "speed_support_bottom"
                            fdmLabel: "支撑底板速度"
                            fdmDescription: "打印支撑底板的速度。 以较低的速度打印可以改善支撑在模型顶部的粘着。"
                        }
                    }
                }
                FdmQml_Param{ id:speed_prime_tower; objectName: "speed_prime_tower"
                    fdmLabel: "装填塔速度"
                    fdmDescription: "打印装填塔的速度。 以较慢速度打印装填塔可以在不同耗材之间的粘着欠佳时使其更加稳定。"
                }
            }
            FdmQml_Param{ id:speed_layer_0; objectName: "speed_layer_0"
                fdmLabel: "起始层速度"
                fdmDescription: "起始层的速度。建议采用较低的值以便改善与构建板的粘着。不会影响构建板自身的粘着结构，如边沿和筏。"
                FdmQml_Param{ id:speed_print_layer_0; objectName: "speed_print_layer_0"
                    fdmLabel: "起始层打印速度"
                    fdmDescription: "打印起始层的速度。 建议采用较低的值以便改善与打印平台的粘着。"
                }
                FdmQml_Param{ id:speed_travel_layer_0; objectName: "speed_travel_layer_0"
                    fdmLabel: "起始层空驶速度"
                    fdmDescription: "起始层中的空驶速度。 建议采用较低的值，以防止将之前打印的部分从打印平台上拉离。 该设置的值可以根据空驶速度和打印速度的比率自动计算得出。"
                }
                FdmQml_Param{ id:skirt_brim_speed; objectName: "skirt_brim_speed"
                    fdmLabel: "Skirt/Brim 速度"
                    fdmDescription: "打印 skirt 和 brim 的速度。 一般情况是以起始层速度打印这些部分，但有时候您可能想要以不同速度来打印 skirt 或 brim。"
                }
            }
            FdmQml_Param{ id:speed_z_hop; objectName: "speed_z_hop"
                fdmLabel: "Z 抬升速度"
                fdmDescription: "Z 垂直移动实现抬升的速度。一般小于打印速度，因为打印平台或打印机的十字轴较难移动。"
            }
            FdmQml_Param{ id:speed_slowdown_layers; objectName: "speed_slowdown_layers"
                fdmLabel: "较慢层的数量"
                fdmDescription: "前几层的打印速度比模型的其他层慢，以便实现与打印平台的更好粘着，并改善整体的打印成功率。 该速度在这些层中会逐渐增加。"
            }
            FdmQml_Param{ id:speed_equalize_flow_enabled; objectName: "speed_equalize_flow_enabled"
                fdmLabel: "均衡耗材流量"
                fdmDescription: "以较快的速度打印比正常走线更细的走线，使每秒挤出的材料量保持相同。 模型中较薄的部分可能需要以低于设置中所提供宽度的走线宽度来打印走线。 该设置控制这些走线的速度变化。"
                FdmQml_Param{ id:speed_equalize_flow_max; objectName: "speed_equalize_flow_max"
                    fdmLabel: "均流最大速度"
                    fdmDescription: "调整打印速度以便均衡流量时的最大打印速度。"
                }
            }
            FdmQml_Param{ id:acceleration_enabled; objectName: "acceleration_enabled"
                fdmLabel: "启用加速度控制"
                fdmDescription: "启用调整打印头加速度。 提高加速度可以通过以打印质量为代价来缩短打印时间。"
                FdmQml_Param{ id:acceleration_travel; objectName: "acceleration_travel"
                    fdmLabel: "空驶加速度"
                    fdmDescription: "进行空驶的加速度。"
                }
                FdmQml_Param{ id:acceleration_print; objectName: "acceleration_print"
                    fdmLabel: "打印加速度"
                    fdmDescription: "打印发生的加速度。"
                    FdmQml_Param{ id:acceleration_infill; objectName: "acceleration_infill"
                        fdmLabel: "填充加速度"
                        fdmDescription: "打印填充物的加速度。"
                    }
                    FdmQml_Param{ id:acceleration_wall; objectName: "acceleration_wall"
                        fdmLabel: "壁加速度"
                        fdmDescription: "打印壁的加速度。"
                        FdmQml_Param{ id:acceleration_wall_0; objectName: "acceleration_wall_0"
                            fdmLabel: "外壁加速度"
                            fdmDescription: "打印最外壁的加速度。"
                        }
                        FdmQml_Param{ id:acceleration_wall_x; objectName: "acceleration_wall_x"
                            fdmLabel: "内壁加速度"
                            fdmDescription: "打印所有内壁的加速度。"
                        }
                        FdmQml_Param{ id:acceleration_topbottom; objectName: "acceleration_topbottom"
                            fdmLabel: "顶部/底部加速度"
                            fdmDescription: "打印顶部/底部层的加速度。"
                            FdmQml_Param{ id:acceleration_roofing; objectName: "acceleration_roofing"
                                fdmLabel: "顶部表面皮肤加速度"
                                fdmDescription: "打印顶部表面皮肤层的加速度。"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_support; objectName: "acceleration_support"
                        fdmLabel: "支撑加速度"
                        fdmDescription: "打印支撑结构的加速度。"
                        FdmQml_Param{ id:acceleration_support_infill; objectName: "acceleration_support_infill"
                            fdmLabel: "支撑填充加速度"
                            fdmDescription: "打印支撑填充物的加速度。"
                        }
                        FdmQml_Param{ id:acceleration_support_interface; objectName: "acceleration_support_interface"
                            fdmLabel: "支撑接触面加速度"
                            fdmDescription: "打印支撑顶板和底板的加速度。 以较低的加速度打印可以改善悬垂质量。"
                            FdmQml_Param{ id:acceleration_support_roof; objectName: "acceleration_support_roof"
                                fdmLabel: "支撑顶板加速度"
                                fdmDescription: "打印支撑顶板的加速度。 以较低的加速度打印可以改善悬垂质量。"
                            }
                            FdmQml_Param{ id:acceleration_support_bottom; objectName: "acceleration_support_bottom"
                                fdmLabel: "支撑底板加速度"
                                fdmDescription: "打印支撑底板的加速度。 以较低的加速度打印可以改善支撑在模型顶部的粘着。"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_prime_tower; objectName: "acceleration_prime_tower"
                        fdmLabel: "装填塔加速度"
                        fdmDescription: "打印装填塔的加速度。"
                    }
                }
                FdmQml_Param{ id:acceleration_layer_0; objectName: "acceleration_layer_0"
                    fdmLabel: "起始层加速度"
                    fdmDescription: "起始层的加速度。"
                    FdmQml_Param{ id:acceleration_travel_layer_0; objectName: "acceleration_travel_layer_0"
                        fdmLabel: "起始层空驶加速度"
                        fdmDescription: "起始层中的空驶加速度。"
                    }
                    FdmQml_Param{ id:acceleration_print_layer_0; objectName: "acceleration_print_layer_0"
                        fdmLabel: "起始层打印加速度"
                        fdmDescription: "打印起始层时的加速度。"
                    }
                    FdmQml_Param{ id:acceleration_skirt_brim; objectName: "acceleration_skirt_brim"
                        fdmLabel: "Skirt/Brim 加速度"
                        fdmDescription: "打印 skirt 和 brim 的加速度。 一般情况是以起始层加速度打印这些部分，但有时候您可能想要以不同加速度来打印 skirt 或 brim。"
                    }
                }
            }
            FdmQml_Param{ id:jerk_enabled; objectName: "jerk_enabled"
                fdmLabel: "启用抖动速度控制"
                fdmDescription: "启用当 X 或 Y 轴的速度变化时调整打印头的抖动速度。 提高抖动速度可以通过以打印质量为代价来缩短打印时间。"
                FdmQml_Param{ id:jerk_travel; objectName: "jerk_travel"
                    fdmLabel: "空驶抖动速度"
                    fdmDescription: "进行空驶时的最大瞬时速度变化。"
                }
                FdmQml_Param{ id:jerk_print; objectName: "jerk_print"
                    fdmLabel: "打印抖动速度"
                    fdmDescription: "打印头的最大瞬时速度变化。"
                    FdmQml_Param{ id:jerk_infill; objectName: "jerk_infill"
                        fdmLabel: "填充抖动速度"
                        fdmDescription: "打印填充物时的最大瞬时速度变化。"
                    }
                    FdmQml_Param{ id:jerk_wall; objectName: "jerk_wall"
                        fdmLabel: "壁抖动速度"
                        fdmDescription: "打印壁时的最大瞬时速度变化。"
                        FdmQml_Param{ id:jerk_wall_0; objectName: "jerk_wall_0"
                            fdmLabel: "外壁抖动速度"
                            fdmDescription: "打印最外壁时的最大瞬时速度变化。"
                        }
                        FdmQml_Param{ id:jerk_wall_x; objectName: "jerk_wall_x"
                            fdmLabel: "内壁抖动速度"
                            fdmDescription: "打印所有内壁时的最大瞬时速度变化。"
                        }
                        FdmQml_Param{ id:jerk_topbottom; objectName: "jerk_topbottom"
                            fdmLabel: "顶部/底部抖动速度"
                            fdmDescription: "打印顶部/底部层时的最大瞬时速度变化。"
                            FdmQml_Param{ id:jerk_roofing; objectName: "jerk_roofing"
                                fdmLabel: "顶部表面皮肤抖动速度"
                                fdmDescription: "打印顶部表面皮肤层时的最大瞬时速度变化。"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_support; objectName: "jerk_support"
                        fdmLabel: "支撑抖动速度"
                        fdmDescription: "打印支撑结构时的最大瞬时速度变化。"
                        FdmQml_Param{ id:jerk_support_infill; objectName: "jerk_support_infill"
                            fdmLabel: "支撑填充抖动速度"
                            fdmDescription: "打印支撑填充物时的最大瞬时速度变化。"
                        }
                        FdmQml_Param{ id:jerk_support_interface; objectName: "jerk_support_interface"
                            fdmLabel: "支撑接触面抖动速度"
                            fdmDescription: "打印支撑顶板和底板的最大瞬时速度变化。"
                            FdmQml_Param{ id:jerk_support_roof; objectName: "jerk_support_roof"
                                fdmLabel: "支撑顶板抖动速度"
                                fdmDescription: "打印支撑顶板的最大瞬时速度变化。"
                            }
                            FdmQml_Param{ id:jerk_support_bottom; objectName: "jerk_support_bottom"
                                fdmLabel: "支撑底板抖动速度"
                                fdmDescription: "打印支撑底板时的最大瞬时速度变化。"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_prime_tower; objectName: "jerk_prime_tower"
                        fdmLabel: "装填塔抖动速度"
                        fdmDescription: "打印装填塔时的最大瞬时速度变化。"
                    }
                }
                FdmQml_Param{ id:jerk_layer_0; objectName: "jerk_layer_0"
                    fdmLabel: "起始层抖动速度"
                    fdmDescription: "起始层的打印最大瞬时速度变化。"
                    FdmQml_Param{ id:jerk_travel_layer_0; objectName: "jerk_travel_layer_0"
                        fdmLabel: "起始层空驶抖动速度"
                        fdmDescription: "起始层中的空驶加速度。"
                    }
                    FdmQml_Param{ id:jerk_print_layer_0; objectName: "jerk_print_layer_0"
                        fdmLabel: "起始层打印抖动速度"
                        fdmDescription: "打印起始层时的最大瞬时速度变化。"
                    }
                    FdmQml_Param{ id:jerk_skirt_brim; objectName: "jerk_skirt_brim"
                        fdmLabel: "Skirt/Brim 抖动速度"
                        fdmDescription: "打印 skirt 和 brim 时的最大瞬时速度变化。"
                    }
                }
            }
        }
        FdmQml_Category{ id:travel; objectName: "travel"
            fdmLabel: "移动"
            fdmDescription: "空驶"
            FdmQml_Param{ id:retraction_enable; objectName: "retraction_enable"
                fdmLabel: "启用回抽"
                fdmDescription: "当喷嘴移动到非打印区域上方时回抽耗材。 "
            }
            FdmQml_Param{ id:retract_at_layer_change; objectName: "retract_at_layer_change"
                fdmLabel: "层变化时回抽"
                fdmDescription: "当喷嘴移动到下一层时回抽耗材。"
            }
            FdmQml_Param{ id:retraction_amount; objectName: "retraction_amount"
                fdmLabel: "回抽距离"
                fdmDescription: "回抽移动期间回抽的材料长度。"
            }
            FdmQml_Param{ id:retraction_speed; objectName: "retraction_speed"
                fdmLabel: "回抽速度"
                fdmDescription: "回抽移动期间耗材回抽和装填的速度。"
                FdmQml_Param{ id:retraction_retract_speed; objectName: "retraction_retract_speed"
                    fdmLabel: "回抽速度"
                    fdmDescription: "回抽移动期间耗材回抽的速度。"
                }
                FdmQml_Param{ id:retraction_prime_speed; objectName: "retraction_prime_speed"
                    fdmLabel: "回抽装填速度"
                    fdmDescription: "回抽移动期间耗材装填的速度。"
                }
            }
            FdmQml_Param{ id:retraction_extra_prime_amount; objectName: "retraction_extra_prime_amount"
                fdmLabel: "回抽额外装填量"
                fdmDescription: "有些材料可能会在空驶过程中渗出，可以在这里对其进行补偿。"
            }
            FdmQml_Param{ id:retraction_prime_min_travel; objectName: "retraction_prime_min_travel"
                fdmLabel: "回抽额外装填量最小空走距离"
                fdmDescription: "有些材料可能会在空驶一定距离时，才会过程中渗出，这个是设置最空驶距离。"
            }
						
			
            FdmQml_Param{ id:retraction_min_travel; objectName: "retraction_min_travel"
                fdmLabel: "回抽最小空驶"
                fdmDescription: "回抽发生所需的最小空驶距离。 这有助于在较小区域内实现更少的回抽。"
            }
            FdmQml_Param{ id:retraction_count_max; objectName: "retraction_count_max"
                fdmLabel: "最大回抽计数"
                fdmDescription: "此设置限制在最小挤出距离范围内发生的回抽数。 此范围内的额外回抽将会忽略。 这避免了在同一件耗材上重复回抽，从而导致耗材变扁并引起磨损问题。"
            }
            FdmQml_Param{ id:retraction_extrusion_window; objectName: "retraction_extrusion_window"
                fdmLabel: "最小挤出距离范围"
                fdmDescription: "执行最大回抽计数的范围。 该值应与回抽距离大致相同，以便一次回抽通过同一块材料的次数得到有效限制。"
            }
            FdmQml_Param{ id:limit_support_retractions; objectName: "limit_support_retractions"
                fdmLabel: "支撑限制被撤销"
                fdmDescription: "当在各个支撑间直线移动时，省略回抽。启用这个设置可以节省打印时间，但会在支撑结构中产生过多穿线。"
            }
            FdmQml_Param{ id:retraction_combing; objectName: "retraction_combing"
                fdmLabel: "梳理模式"
                fdmDescription: "梳理可在空驶时让喷嘴保持在已打印区域内。 这会使空驶距离稍微延长，但可减少回抽需求。 如果关闭梳理，则材料将回抽，且喷嘴沿着直线移动到下一个点。 也可以通过仅在填充物内进行梳理避免梳理顶部/底部皮肤区域。"
                fdmOptions:{
                    "off": "关",
                    "all": "所有",
                    "noskin": "除了皮肤",
                    "infill": "在填充物内"
                }
            }
            FdmQml_Param{ id:retraction_combing_max_distance; objectName: "retraction_combing_max_distance"
                fdmLabel: "最大梳距，无收缩"
                fdmDescription: "当大于零时，比这段距离更长的梳理空驶将会使用回抽。如果设置为零，则没有最大值，梳理空驶将不会使用回抽。"
            }
            FdmQml_Param{ id:travel_retract_before_outer_wall; objectName: "travel_retract_before_outer_wall"
                fdmLabel: "在外壁前回抽"
                fdmDescription: "在移动开始打印外壁时始终回抽。"
            }
            FdmQml_Param{ id:travel_avoid_other_parts; objectName: "travel_avoid_other_parts"
                fdmLabel: "空驶时避开已打印部分"
                fdmDescription: "喷嘴会在空驶时避开已打印的部分。 此选项仅在启用梳理功能时可用。"
            }
            FdmQml_Param{ id:travel_avoid_supports; objectName: "travel_avoid_supports"
                fdmLabel: "避免移动时支撑"
                fdmDescription: "在空走时，喷嘴避免了已打印的支撑。只有在启用了梳理时才可以使用此选项。"
            }
            FdmQml_Param{ id:travel_avoid_distance; objectName: "travel_avoid_distance"
                fdmLabel: "空驶避让距离"
                fdmDescription: "喷嘴和已打印部分之间在空驶时避让的距离。"
            }
            FdmQml_Param{ id:layer_start_x; objectName: "layer_start_x"
                fdmLabel: "层开始 X"
                fdmDescription: "位置的 X 轴坐标，在该位置附近找到开始打印每层的部分。"
            }
            FdmQml_Param{ id:layer_start_y; objectName: "layer_start_y"
                fdmLabel: "层开始 Y"
                fdmDescription: "位置的 Y 轴坐标，在该位置附近找到开始打印每层的部分。"
            }
            FdmQml_Param{ id:retraction_hop_enabled; objectName: "retraction_hop_enabled"
                fdmLabel: "回抽时 Z 抬升"
                fdmDescription: "每当回抽完成时，打印平台会降低以便在喷嘴和打印品之间形成空隙。 它可以防止喷嘴在空驶过程中撞到打印品，降低将打印品从打印平台撞掉的几率。"
            }
            FdmQml_Param{ id:retraction_hop_only_when_collides; objectName: "retraction_hop_only_when_collides"
                fdmLabel: "仅在已打印部分上 Z 抬升"
                fdmDescription: "仅在移动到无法通过“空驶时避开已打印部分”选项的水平操作避开的已打印部分上方时执行 Z 抬升。"
            }
            FdmQml_Param{ id:retraction_hop; objectName: "retraction_hop"
                fdmLabel: "Z 抬升高度"
                fdmDescription: "执行 Z 抬升的高度差。"
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch; objectName: "retraction_hop_after_extruder_switch"
                fdmLabel: "挤出机切换后的 Z 抬升"
                fdmDescription: "当机器从一个挤出机切换到另一个时，打印平台会降低以便在喷嘴和打印品之间形成空隙。 这将防止喷嘴在打印品外部留下渗出物。"
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch_height; objectName: "retraction_hop_after_extruder_switch_height"
                fdmLabel: "挤出机切换后的 Z 抬升高度"
                fdmDescription: "挤出机切换后执行 Z 抬升的高度差。"
            }
        }
        FdmQml_Category{ id:cooling; objectName: "cooling"
            fdmLabel: "冷却"
            fdmDescription: "冷却"
            FdmQml_Param{ id:cool_fan_enabled; objectName: "cool_fan_enabled"
                fdmLabel: "开启打印冷却"
                fdmDescription: "打印时启用打印冷却风扇。 风扇可以在层时间较短和有桥接/悬垂的层上提高打印质量。"
            }
            FdmQml_Param{ id:cool_fan_speed; objectName: "cool_fan_speed"
                fdmLabel: "风扇速度"
                fdmDescription: "打印冷却风扇旋转的速度。"
                FdmQml_Param{ id:cool_fan_speed_min; objectName: "cool_fan_speed_min"
                    fdmLabel: "正常风扇速度"
                    fdmDescription: "风扇旋转达到阈值前的速度。 当一层的打印速度超过阈值时，风扇速度逐渐朝最大风扇速度增加。"
                }
                FdmQml_Param{ id:cool_fan_speed_max; objectName: "cool_fan_speed_max"
                    fdmLabel: "最大风扇速度"
                    fdmDescription: "风扇在最小层时间上旋转的速度。 当达到阈值时，风扇速度在正常风扇速度和最大风扇速度之间逐渐增加。"
                }
                FdmQml_Param{ id:cool_min_layer_time_fan_speed_max; objectName: "cool_min_layer_time_fan_speed_max"
                    fdmLabel: "正常/最大风扇速度阈值"
                    fdmDescription: "设定正常风扇速度和最大风扇速度之间阈值的层时间。 打印速度低于此时间的层使用正常风扇速度。 对于更快的层，风扇速度逐渐增加到最大风扇速度。"
                }
                FdmQml_Param{ id:cool_fan_speed_0; objectName: "cool_fan_speed_0"
                    fdmLabel: "起始风扇速度"
                    fdmDescription: "风扇在打印开始时旋转的速度。 在随后的层中，风扇速度逐渐增加到对应“正常风扇速度（高度）”的水平。"
                }
                FdmQml_Param{ id:cool_fan_full_at_height; objectName: "cool_fan_full_at_height"
                    fdmLabel: "正常风扇速度（高度）"
                    fdmDescription: "风扇以正常速度旋转的高度。 在下方的层中，风扇速度逐渐从起始风扇速度增加到正常风扇速度。"
                    FdmQml_Param{ id:cool_fan_full_layer; objectName: "cool_fan_full_layer"
                        fdmLabel: "正常风扇速度（层）"
                        fdmDescription: "风扇以正常风扇速度旋转的层。 如果设置了正常风扇速度（高度），则该值将被计算并舍入为整数。"
                    }
                }
                FdmQml_Param{ id:cool_min_layer_time; objectName: "cool_min_layer_time"
                    fdmLabel: "最短单层冷却时间"
                    fdmDescription: "在层中花费的最少时间。 这会迫使打印机减速，以便至少在一层中消耗此处所规定的时间。 这会让已打印材料充分冷却后再打印下一层。 如果提升头被禁用，且如果不这么做会违反“最小速度“，则层所花时间可能仍会少于最小层时间。"
                    FdmQml_Param{ id:cool_min_speed_includes_outer_walls; objectName: "cool_min_speed_includes_outer_walls"
                        fdmLabel: "最小打印速度包括外墙"
                        fdmDescription: "﻿最小打印速度包括外墙"
                    }
                }
                FdmQml_Param{ id:cool_min_speed; objectName: "cool_min_speed"
                    fdmLabel: "最小风扇速度"
                    fdmDescription: "最低打印速度，排除因最短层时间而减速。 当打印机减速过多时，喷嘴中的压力将过低并导致较差的打印质量。"
                }
                FdmQml_Param{ id:cool_lift_head; objectName: "cool_lift_head"
                    fdmLabel: "打印头提升"
                    fdmDescription: "当因最低层时间达到最低速度时，将打印头从打印品上提升，并等候达到最低层时间。"
                }
            }
        }
        FdmQml_Category{ id:support; objectName: "support"
            fdmLabel: "支撑"
            fdmDescription: "支撑"
            FdmQml_Param{ id:support_enable; objectName: "support_enable"
                fdmLabel: "生成支撑"
                fdmDescription: "在模型的悬垂（Overhangs）部分生成支撑结构。若不这样做，这些部分在打印时将倒塌。"
            }
            FdmQml_Param{ id:support_extruder_nr; objectName: "support_extruder_nr"
                fdmLabel: "支撑用挤出机"
                fdmDescription: "用于打印支撑的挤出机组。 用于多重挤出。"
                FdmQml_Param{ id:support_infill_extruder_nr; objectName: "support_infill_extruder_nr"
                    fdmLabel: "支撑填充挤出机"
                    fdmDescription: "用于打印支撑填充物的挤出机组。 用于多重挤出。"
                }
                FdmQml_Param{ id:support_extruder_nr_layer_0; objectName: "support_extruder_nr_layer_0"
                    fdmLabel: "第一层支撑挤出机"
                    fdmDescription: "用于打印支撑填充物第一层的挤出机组。 用于多重挤出。"
                }
                FdmQml_Param{ id:support_interface_extruder_nr; objectName: "support_interface_extruder_nr"
                    fdmLabel: "支撑接触面挤出机"
                    fdmDescription: "用于打印支撑顶板和底板的挤出机组。 用于多重挤出。"
                    FdmQml_Param{ id:support_roof_extruder_nr; objectName: "support_roof_extruder_nr"
                        fdmLabel: "支撑顶板挤出机"
                        fdmDescription: "用于打印支撑顶板的挤出机组。 用于多重挤出。"
                    }
                    FdmQml_Param{ id:support_bottom_extruder_nr; objectName: "support_bottom_extruder_nr"
                        fdmLabel: "支撑底板挤出机"
                        fdmDescription: "用于打印支撑底板的挤出机组。 用于多重挤出。"
                    }
                }
            }
            FdmQml_Param{ id:support_structure; objectName: "support_structure"
                fdmLabel: "支撑结构"
                fdmDescription: "在可用于产生支撑的方法之间进行选择。“普通”支撑在悬垂部分正下方形成一个支撑结构，并直接垂下这些区域。“树形”支撑形成一些分支，它们朝向在这些分支的尖端上支撑模型的悬垂区域，并使这些分支可缠绕在模型周围以尽可能多地从构建板上支撑它。"
                fdmOptions:{
                    "normal": "正常",
                    "tree": "树形"
                }
            }
            FdmQml_Param{ id:support_tree_angle; objectName: "support_tree_angle"
                fdmLabel: "树形支撑分支角度"
                fdmDescription: "分支的角度。使用较小的角度可增加垂直度和稳定性。使用较大的角度可支撑更大范围。"
            }
            FdmQml_Param{ id:support_tree_branch_distance; objectName: "support_tree_branch_distance"
                fdmLabel: "树形支撑分支间距"
                fdmDescription: "在支撑模型时，分支之间需要多大的间距。缩小这一间距会使树形支撑与模型之间有更多接触点，带来更好的悬垂，但会使支撑更难以拆除。"
            }
            FdmQml_Param{ id:support_tree_branch_diameter; objectName: "support_tree_branch_diameter"
                fdmLabel: "树形支撑分支直径"
                fdmDescription: "树形支撑最细分支的直径。较粗的分支更坚固。接近基础的分支会比这更粗。"
            }
            FdmQml_Param{ id:support_tree_branch_diameter_angle; objectName: "support_tree_branch_diameter_angle"
                fdmLabel: "树形支撑分支直径角度"
                fdmDescription: "随着分支朝底部逐渐变粗，分支直径的角度。角度为 0 表明分支全长具有均匀的粗细度。稍微有些角度可以增加树形支撑的稳定性。"
            }
            FdmQml_Param{ id:support_tree_collision_resolution; objectName: "support_tree_collision_resolution"
                fdmLabel: "树形支撑碰撞分辨率"
                fdmDescription: "用于计算碰撞的分辨率，目的在于避免碰撞模型。将此设置得较低将产生更准确且通常较少失败的树，但是会大幅增加切片时间。"
            }
            FdmQml_Param{ id:support_type; objectName: "support_type"
                fdmLabel: "支撑放置"
                fdmDescription: "调整支撑结构的放置。 放置可以设置为支撑打印平台或全部支撑。 当设置为全部支撑时，支撑结构也将在模型上打印。"
                fdmOptions:{
                    "buildplate": "支撑打印平台",
                    "everywhere": "全部支撑"
                }
            }
            FdmQml_Param{ id:support_angle; objectName: "support_angle"
                fdmLabel: "支撑悬垂角度"
                fdmDescription: "添加支撑的最小悬垂角度。 当角度为 0° 时，将支撑所有悬垂，当角度大于 90° 时，不提供任何支撑。"
            }
            FdmQml_Param{ id:support_pattern; objectName: "support_pattern"
                fdmLabel: "支撑图案"
                fdmDescription: "打印品支撑结构的图案。 提供的不同选项可实现或牢固或易于拆除的支撑。"
                fdmOptions:{
                    "lines": "走线",
                    "grid": "网格",
                    "triangles": "三角形",
                    "concentric": "同心",
                    "zigzag": "锯齿形",
                    "cross": "交叉",
                    "gyroid": "螺旋二十四面体"
                }
            }
            FdmQml_Param{ id:support_wall_count; objectName: "support_wall_count"
                fdmLabel: "支撑墙行数"
                fdmDescription: "包围支撑的墙的数量。添加一堵墙可以使支持打印更加可靠，并且可以更好地支持挂起，但增加了打印时间和使用的材料。"
            }
            FdmQml_Param{ id:zig_zaggify_support; objectName: "zig_zaggify_support"
                fdmLabel: "连接支撑线"
                fdmDescription: "将支撑线尾端连接在一起。启用此设置会让支撑更为牢固并减少挤出不足，但需要更多材料。"
            }
            FdmQml_Param{ id:support_connect_zigzags; objectName: "support_connect_zigzags"
                fdmLabel: "连接支撑锯齿形"
                fdmDescription: "连接锯齿形。 这将增加锯齿形支撑结构的强度。"
            }
            FdmQml_Param{ id:support_infill_rate; objectName: "support_infill_rate"
                fdmLabel: "支撑密度"
                fdmDescription: "调整支撑结构的密度。 较高的值会实现更好的悬垂，但支撑将更加难以移除。"
                FdmQml_Param{ id:support_line_distance; objectName: "support_line_distance"
                    fdmLabel: "支撑走线距离"
                    fdmDescription: "已打印支撑结构走线之间的距离。 该设置通过支撑密度计算。"
                }
                FdmQml_Param{ id:support_initial_layer_line_distance; objectName: "support_initial_layer_line_distance"
                    fdmLabel: "起始层支撑走线距离"
                    fdmDescription: "已打印起始层支撑结构走线之间的距离。该设置通过支撑密度计算。"
                }
            }
            FdmQml_Param{ id:support_infill_angles; objectName: "support_infill_angles"
                fdmLabel: "支撑填充走线方向"
                fdmDescription: "要使用的整数走线方向列表。列表中的元素随层的进度依次使用，当达到列表末尾时将从头开始。列表项以逗号分隔，整个列表包含在方括号中。“默认“是一个空列表，即意味着使用默认角度 0 度。"
            }
            FdmQml_Param{ id:support_brim_enable; objectName: "support_brim_enable"
                fdmLabel: "启用支撑 Brim"
                fdmDescription: "在第一层的支撑填充区域内生成一个 Brim。此 Brim 在支撑下方打印，而非周围。启用此设置会增强支撑与打印平台的附着。"
            }
            FdmQml_Param{ id:support_brim_width; objectName: "support_brim_width"
                fdmLabel: "支撑 Brim 宽度"
                fdmDescription: "在支撑下方要打印的 Brim 的宽度。较大的 Brim 可增强与打印平台的附着，但也会增加一些额外材料成本。"
                FdmQml_Param{ id:support_brim_line_count; objectName: "support_brim_line_count"
                    fdmLabel: "支撑 Brim 走线次数"
                    fdmDescription: "用于支撑 Brim 的走线数量。更多 Brim 走线可增强与打印平台的附着，但也会增加一些额外材料成本。"
                }
            }
            FdmQml_Param{ id:support_z_distance; objectName: "support_z_distance"
                fdmLabel: "支撑 Z 距离"
                fdmDescription: "支撑结构顶部/底部到打印品之间的距离。 该间隙提供了在模型打印完成后移除支撑的空隙。 该值舍入为层高的倍数。"
                FdmQml_Param{ id:support_top_distance; objectName: "support_top_distance"
                    fdmLabel: "支撑顶部距离"
                    fdmDescription: "从支撑顶部到打印品的距离。"
                }
                FdmQml_Param{ id:support_bottom_distance; objectName: "support_bottom_distance"
                    fdmLabel: "支撑底部距离"
                    fdmDescription: "从打印品到支撑底部的距离。"
                }
            }
            FdmQml_Param{ id:support_xy_distance; objectName: "support_xy_distance"
                fdmLabel: "支撑 X/Y 距离"
                fdmDescription: "支撑结构在 X/Y 方向距打印品的距离。"
            }
            FdmQml_Param{ id:support_xy_overrides_z; objectName: "support_xy_overrides_z"
                fdmLabel: "支撑距离优先级"
                fdmDescription: "支撑 X/Y 距离是否覆盖支撑 Z 距离或反之。 当 X/Y 覆盖 Z 时，X/Y 距离可将支撑从模型上推离，影响与悬垂之间的实际 Z 距离。 我们可以通过不在悬垂周围应用 X/Y 距离来禁用此选项。"
                fdmOptions:{
                    "xy_overrides_z": "X/Y 覆盖 Z",
                    "z_overrides_xy": "Z 覆盖 X/Y"
                }
            }
            FdmQml_Param{ id:support_xy_distance_overhang; objectName: "support_xy_distance_overhang"
                fdmLabel: "最小支撑 X/Y 距离"
                fdmDescription: "支撑结构在 X/Y 方向距悬垂的距离。 "
            }
            FdmQml_Param{ id:support_bottom_stair_step_height; objectName: "support_bottom_stair_step_height"
                fdmLabel: "支撑梯步阶高度"
                fdmDescription: "停留在模型上的支撑阶梯状底部的步阶高度。 较低的值会使支撑更难于移除，但过高的值可能导致不稳定的支撑结构。 设为零可以关闭阶梯状行为。"
            }
            FdmQml_Param{ id:support_bottom_stair_step_width; objectName: "support_bottom_stair_step_width"
                fdmLabel: "支撑梯步阶最大宽度"
                fdmDescription: "停留在模型上的支撑阶梯状底部的最大步阶宽度。 较低的值会使支撑更难于移除，但过高的值可能导致不稳定的支撑结构。"
            }
            FdmQml_Param{ id:support_bottom_stair_step_min_slope; objectName: "support_bottom_stair_step_min_slope"
                fdmLabel: "支撑阶梯最小坡度角"
                fdmDescription: "使阶梯生效的区域最小坡度。该值较小可在较浅的坡度上更容易去除支撑，但该值过小可能会在模型的其他部分上产生某些很反常的结果。"
            }
            FdmQml_Param{ id:support_join_distance; objectName: "support_join_distance"
                fdmLabel: "支撑结合部距离"
                fdmDescription: "支撑结构间在 X/Y 方向的最大距离。当分离结构之间的距离小于此值时，这些结构将合并为一体。"
            }
            FdmQml_Param{ id:support_offset; objectName: "support_offset"
                fdmLabel: "支撑水平扩展"
                fdmDescription: "应用到每一层所有支撑多边形的偏移量。 正值可以让支撑区域更平滑，并产生更为牢固的支撑。"
            }
            FdmQml_Param{ id:support_infill_sparse_thickness; objectName: "support_infill_sparse_thickness"
                fdmLabel: "支撑填充层厚度"
                fdmDescription: "支撑填充材料每层的厚度。 该值应始终为层高的乘数，否则应进行舍入。"
            }
            FdmQml_Param{ id:gradual_support_infill_steps; objectName: "gradual_support_infill_steps"
                fdmLabel: "渐进支撑填充步阶"
                fdmDescription: "在进入顶层以下时，将支撑填充密度减少一半的次数。 越靠近顶面的区域密度越高，最高达到支撑填充密度。"
            }
            FdmQml_Param{ id:gradual_support_infill_step_height; objectName: "gradual_support_infill_step_height"
                fdmLabel: "渐进支撑填充步阶高度"
                fdmDescription: "在切换至密度的一半前指定密度的支撑填充高度。"
            }
            FdmQml_Param{ id:minimum_support_area; objectName: "minimum_support_area"
                fdmLabel: "最小支撑面积"
                fdmDescription: "支撑多边形的最小面积。将不会生成面积小于此值的多边形。"
            }
            FdmQml_Param{ id:support_interface_enable; objectName: "support_interface_enable"
                fdmLabel: "启用支撑接触面"
                fdmDescription: "在模型和支撑之间生成一个密集的接触面。 这会在打印模型所在的支撑顶部和模型停放的支撑底部创建一个皮肤。"
                FdmQml_Param{ id:support_roof_enable; objectName: "support_roof_enable"
                    fdmLabel: "启用支撑顶板"
                    fdmDescription: "在支撑顶部和模型之间生成一个密集的材料板。 这会在模型和支撑之间形成一个皮肤。"
                }
                FdmQml_Param{ id:support_bottom_enable; objectName: "support_bottom_enable"
                    fdmLabel: "启用支撑底板"
                    fdmDescription: "在支撑底部和模型之间生成一个密集的材料板。 这会在模型和支撑之间形成一个皮肤。"
                }
            }
            FdmQml_Param{ id:support_interface_height; objectName: "support_interface_height"
                fdmLabel: "支撑接触面厚度"
                fdmDescription: "支撑与模型在底部或顶部接触的接触面厚度。"
                FdmQml_Param{ id:support_roof_height; objectName: "support_roof_height"
                    fdmLabel: "支撑顶板厚度"
                    fdmDescription: "支撑顶板的厚度。 这会控制模型所停放的支撑顶部密集层的数量。"
                }
                FdmQml_Param{ id:support_bottom_height; objectName: "support_bottom_height"
                    fdmLabel: "支撑底板厚度"
                    fdmDescription: "支撑底板的厚度。 这会控制支撑所停放的模型顶部区域所打印的密集层数量。"
                }
            }
            FdmQml_Param{ id:support_interface_skip_height; objectName: "support_interface_skip_height"
                fdmLabel: "支撑接触面分辨率"
                fdmDescription: "在检查支撑上方或下方是否有模型时，采用指定高度的步阶。 值越低切片速度越慢，而较高的值会导致在部分应有支撑接触面的位置打印一般的支撑。"
            }
            FdmQml_Param{ id:support_interface_density; objectName: "support_interface_density"
                fdmLabel: "支撑接触面密度"
                fdmDescription: "调整支撑结构顶板和底板的密度。 较高的值会实现更好的悬垂，但支撑将更加难以移除。"
                FdmQml_Param{ id:support_roof_density; objectName: "support_roof_density"
                    fdmLabel: "支撑顶板密度"
                    fdmDescription: "支撑结构顶板的密度。 较高的值会实现更好的悬垂，但支撑将更加难以移除。"
                    FdmQml_Param{ id:support_roof_line_distance; objectName: "support_roof_line_distance"
                        fdmLabel: "支撑顶板走线距离"
                        fdmDescription: "已打印支撑顶板走线之间的距离。 该设置是通过支撑顶板密度计算，但可以单独调整。"
                    }
                }
                FdmQml_Param{ id:support_bottom_density; objectName: "support_bottom_density"
                    fdmLabel: "支撑底板密度"
                    fdmDescription: "支撑结构底板的密度。 较高的值会在模型顶部产生更好的支撑粘着。"
                    FdmQml_Param{ id:support_bottom_line_distance; objectName: "support_bottom_line_distance"
                        fdmLabel: "支撑底板走线距离"
                        fdmDescription: "已打印支撑底板走线之间的距离。 该设置是通过支撑底板密度计算，但可以单独调整。"
                    }
                }
            }
            FdmQml_Param{ id:support_interface_pattern; objectName: "support_interface_pattern"
                fdmLabel: "支撑接触面图案"
                fdmDescription: "支撑与模型之间接触面的打印图案。"
                fdmOptions:{
                    "lines": "走线",
                    "grid": "网格",
                    "triangles": "三角形",
                    "concentric": "同心",
                    "zigzag": "锯齿形"
                }
                FdmQml_Param{ id:support_roof_pattern; objectName: "support_roof_pattern"
                    fdmLabel: "支撑顶板图案"
                    fdmDescription: "打印支撑顶板的图案。"
                    fdmOptions:{
                        "lines": "直线",
                        "grid": "网格",
                        "triangles": "三角形",
                        "concentric": "同心圆",
                        "zigzag": "锯齿状"
                    }
                }
                FdmQml_Param{ id:support_bottom_pattern; objectName: "support_bottom_pattern"
                    fdmLabel: "支撑底板图案"
                    fdmDescription: "打印支撑底板的图案。"
                    fdmOptions:{
                        "lines": "走线",
                        "grid": "网格",
                        "triangles": "三角形",
                        "concentric": "同心",
                        "zigzag": "锯齿形"
                    }
                }
            }
            FdmQml_Param{ id:minimum_interface_area; objectName: "minimum_interface_area"
                fdmLabel: "最小支撑接触面面积"
                fdmDescription: "支撑接触面多边形的最小面积。面积小于此值的多边形将打印为一般支撑。"
                FdmQml_Param{ id:minimum_roof_area; objectName: "minimum_roof_area"
                    fdmLabel: "最小支撑顶板面积"
                    fdmDescription: "支撑顶板的最小面积。面积小于此值的多边形将打印为一般支撑。"
                }
                FdmQml_Param{ id:minimum_bottom_area; objectName: "minimum_bottom_area"
                    fdmLabel: "最小支撑底板面积"
                    fdmDescription: "支撑底板的最小面积。面积小于此值的多边形将打印为一般支撑。"
                }
            }
            FdmQml_Param{ id:support_interface_offset; objectName: "support_interface_offset"
                fdmLabel: "支撑接触面水平扩展"
                fdmDescription: "应用到支撑接触面多边形的偏移量。"
                FdmQml_Param{ id:support_roof_offset; objectName: "support_roof_offset"
                    fdmLabel: "支撑顶板水平扩展"
                    fdmDescription: "应用到支撑顶板的偏移量。"
                }
                FdmQml_Param{ id:support_bottom_offset; objectName: "support_bottom_offset"
                    fdmLabel: "支撑底板水平扩展"
                    fdmDescription: "应用到支撑底板的偏移量。"
                }
            }
            FdmQml_Param{ id:support_interface_angles; objectName: "support_interface_angles"
                fdmLabel: "支撑接触面走线方向"
                fdmDescription: "要使用的整数走线方向列表。列表中的元素随层的进度依次使用，当达到列表末尾时将从头开始。列表项以逗号分隔，整个列表包含在方括号中。“默认“为一个空列表，即意味着使用默认角度（如果接触面很厚或为 90 度，则在 45 度和 135 度之间交替）。"
                FdmQml_Param{ id:support_roof_angles; objectName: "support_roof_angles"
                    fdmLabel: "支撑顶板走线方向"
                    fdmDescription: "要使用的整数走线方向列表。列表中的元素随层的进度依次使用，当达到列表末尾时将从头开始。列表项以逗号分隔，整个列表包含在方括号中。“默认“为一个空列表，即意味着使用默认角度（如果接触面很厚或为 90 度，则在 45 度和 135 度之间交替）。"
                }
                FdmQml_Param{ id:support_bottom_angles; objectName: "support_bottom_angles"
                    fdmLabel: "支撑底板走线方向"
                    fdmDescription: "要使用的整数走线方向列表。列表中的元素随层的进度依次使用，当达到列表末尾时将从头开始。列表项以逗号分隔，整个列表包含在方括号中。“默认“为一个空列表，即意味着使用默认角度（如果接触面很厚或为 90 度，则在 45 度和 135 度之间交替）。"
                }
            }
            FdmQml_Param{ id:support_fan_enable; objectName: "support_fan_enable"
                fdmLabel: "风扇速度覆盖"
                fdmDescription: "启用时，会为支撑正上方的表面区域更改打印冷却风扇速度。"
            }
            FdmQml_Param{ id:support_supported_skin_fan_speed; objectName: "support_supported_skin_fan_speed"
                fdmLabel: "支撑的表面风扇速度"
                fdmDescription: "打印支撑正上方表面区域时使用的风扇百分比速度。使用高风扇速度可能使支撑更容易移除。"
            }
            FdmQml_Param{ id:support_use_towers; objectName: "support_use_towers"
                fdmLabel: "使用塔"
                fdmDescription: "使用专门的塔来支撑较小的悬垂区域。 这些塔的直径比它们所支撑的区域要大。 在靠近悬垂物时，塔的直径减小，形成顶板。"
            }
            FdmQml_Param{ id:support_tower_diameter; objectName: "support_tower_diameter"
                fdmLabel: "塔直径"
                fdmDescription: "特殊塔的直径。"
            }
            FdmQml_Param{ id:support_tower_maximum_supported_diameter; objectName: "support_tower_maximum_supported_diameter"
                fdmLabel: "最大塔支撑直径"
                fdmDescription: "将由专门的支撑塔支撑的小区域 X/Y 轴方向的最大直径。"
            }
            FdmQml_Param{ id:support_tower_roof_angle; objectName: "support_tower_roof_angle"
                fdmLabel: "塔顶板角度"
                fdmDescription: "塔顶角度。 该值越高，塔顶越尖，值越低，塔顶越平。"
            }
            FdmQml_Param{ id:support_mesh_drop_down; objectName: "support_mesh_drop_down"
                fdmLabel: "下拉式支撑网格"
                fdmDescription: "在支撑网格下方的所有位置进行支撑，让支撑网格中没有悬垂。"
            }
            FdmQml_Param{ id:support_meshes_present; objectName: "support_meshes_present"
                fdmLabel: "场景具有支撑网格"
                fdmDescription: "场景中存在支撑网格。此设置受AnkerMake控制。"
            }
        }
        FdmQml_Category{ id:platform_adhesion; objectName: "platform_adhesion"
            fdmLabel: "打印平台附着"
            fdmDescription: "附着"
            FdmQml_Param{ id:prime_blob_enable; objectName: "prime_blob_enable"
                fdmLabel: "启用装填光点"
                fdmDescription: "打印前是否装填有光点的耗材。 开启此设置将确保打印前挤出机的喷嘴处已准备好材料。 打印 Brim 或 Skirt 也可作为装填用途，这种情况下关闭此设置可以节省时间。"
            }
            FdmQml_Param{ id:extruder_prime_pos_x; objectName: "extruder_prime_pos_x"
                fdmLabel: "挤出机 X 轴坐标"
                fdmDescription: "打印开始时，喷头在 X 轴上初始位置。"
            }
            FdmQml_Param{ id:extruder_prime_pos_y; objectName: "extruder_prime_pos_y"
                fdmLabel: "挤出机 Y 轴起始位置"
                fdmDescription: "打印开始时，喷头在 Y 轴坐标上初始位置。"
            }
            FdmQml_Param{ id:adhesion_type; objectName: "adhesion_type"
                fdmLabel: "打印平台附着类型"
                fdmDescription: "帮助改善挤出装填以及与打印平台附着的不同选项。 Brim 会在模型基座周围添加单层平面区域，以防止卷翘。 Raft 会在模型下添加一个有顶板的厚网格。 Skirt 是在模型四周打印的一条线，但并不与模型连接。"
                fdmOptions:{
                    "skirt": "Skirt",
                    "brim": "Brim",
                    "raft": "Raft",
                    "none": "无"
                }
            }
            FdmQml_Param{ id:adhesion_extruder_nr; objectName: "adhesion_extruder_nr"
                fdmLabel: "打印平台附着挤出机"
                fdmDescription: "用于打印 skirt/brim/raft 的挤出机组。 用于多重挤出。"
            }
            FdmQml_Param{ id:skirt_line_count; objectName: "skirt_line_count"
                fdmLabel: "Skirt 走线计数"
                fdmDescription: "多个 Skirt 走线帮助为小型模型更好地装填您的挤出部分。 将其设为 0 将禁用 skirt。"
            }
            FdmQml_Param{ id:skirt_gap; objectName: "skirt_gap"
                fdmLabel: "Skirt 距离"
                fdmDescription: "裙子和第一层印花之间的水平距离。\n这是最小的距离。多条裙边线将从这个距离向外延伸。"
            }
            FdmQml_Param{ id:skirt_brim_minimal_length; objectName: "skirt_brim_minimal_length"
                fdmLabel: "Skirt/Brim 最小长度"
                fdmDescription: "skirt 或 brim 的最小长度。 如果所有 skirt 或 brim 走线之和都没有达到此长度，则将添加更多 skirt 或 brim 走线直至达到最小长度。 注意： 如果走线计数设为 0，则将忽略此选项。"
            }
            FdmQml_Param{ id:brim_width; objectName: "brim_width"
                fdmLabel: "Brim 宽度"
                fdmDescription: "模型到最外侧 brim 线的距离。 较大的 brim 可增强与打印平台的附着，但也会减少有效打印区域。"
                FdmQml_Param{ id:brim_line_count; objectName: "brim_line_count"
                    fdmLabel: "Brim 走线计数"
                    fdmDescription: "brim 所用走线数量。 更多 brim 走线可增强与打印平台的附着，但也会减少有效打印区域。"
                }
            }
            FdmQml_Param{ id:brim_gap; objectName: "brim_gap"
                fdmLabel: "边沿距离"
                fdmDescription: "第一条边沿线与打印件第一层轮廓之间的水平距离。较小的间隙可使边沿更容易去除，同时在散热方面仍有优势。"
            }
            FdmQml_Param{ id:brim_replaces_support; objectName: "brim_replaces_support"
                fdmLabel: "Brim 替换支撑"
                fdmDescription: "强制围绕模型打印 Brim，即使该空间本该由支撑占据。此操作会将第一层的某些支撑区域替换为 Brim 区域。"
            }
            FdmQml_Param{ id:brim_outside_only; objectName: "brim_outside_only"
                fdmLabel: "仅在外部打印 Brim"
                fdmDescription: "仅在模型外部打印 brim。 这会减少您之后需要移除的 brim 量，而不会过度影响热床附着。"
            }
            FdmQml_Param{ id:raft_margin; objectName: "raft_margin"
                fdmLabel: "Raft 留白"
                fdmDescription: "如果启用了 raft，则这是指也被提供了 raft 的模型周围的额外 raft 区域。 增加此留白将创建强度更大的 raft，但会使用更多材料，为打印品留下的空间更少。"
            }
            FdmQml_Param{ id:raft_smoothing; objectName: "raft_smoothing"
                fdmLabel: "Raft 平滑度"
                fdmDescription: "该设置控制 Raft 轮廓中的内角呈圆形的程度。 内向角被设置为半圆形，半径等于此处的值。 此设置还会移除 raft 轮廓中小于此半圆形的孔。"
            }
            FdmQml_Param{ id:raft_airgap; objectName: "raft_airgap"
                fdmLabel: "Raft 空隙"
                fdmDescription: "模型最后的 raft 层与第一层之间的间隙。 只有第一层被提高了这个量，以便降低 raft 层和模型之间的附着。 让 raft 更容易剥离。"
            }
            FdmQml_Param{ id:layer_0_z_overlap; objectName: "layer_0_z_overlap"
                fdmLabel: "起始层 Z 重叠"
                fdmDescription: "使模型的第一层和第二层在 Z 方向上重叠以补偿在空隙中损失的耗材。 第一个模型层上方的所有模型将向下移动此重叠量。"
            }
            FdmQml_Param{ id:raft_surface_layers; objectName: "raft_surface_layers"
                fdmLabel: "Raft 顶层"
                fdmDescription: "第 2 个 raft 层上方的顶层数量。 这些是模型所在的完全填充层。 第二层会产生比第一层更平滑的顶部表面。"
            }
            FdmQml_Param{ id:raft_surface_thickness; objectName: "raft_surface_thickness"
                fdmLabel: "Raft 顶层厚度"
                fdmDescription: "顶部 Raft 层的层厚度。"
            }
            FdmQml_Param{ id:raft_surface_line_width; objectName: "raft_surface_line_width"
                fdmLabel: "Raft 顶线宽度"
                fdmDescription: "Raft 顶部表面的走线宽度。 这些走线可以是细线，以便实现平滑的 Raft 顶部。"
            }
            FdmQml_Param{ id:raft_surface_line_spacing; objectName: "raft_surface_line_spacing"
                fdmLabel: "Raft 顶部间距"
                fdmDescription: "顶部 Raft 层的 Raft 走线之间的距离。 间距应等于走线宽度，以便打造坚固表面。"
            }
            FdmQml_Param{ id:raft_interface_thickness; objectName: "raft_interface_thickness"
                fdmLabel: "Raft 中间厚度"
                fdmDescription: "中间 Raft 层的层厚度。"
            }
            FdmQml_Param{ id:raft_interface_line_width; objectName: "raft_interface_line_width"
                fdmLabel: "Raft 中间线宽度"
                fdmDescription: "中间 Raft 层的走线宽度。 让第二层挤出更多会导致走线粘着在打印平台上。"
            }
            FdmQml_Param{ id:raft_interface_line_spacing; objectName: "raft_interface_line_spacing"
                fdmLabel: "Raft 中间间距"
                fdmDescription: "中间 Raft 层的 Raft 走线之间的距离。 中间的间距应足够宽，同时也要足够密集，以便支撑顶部 Raft 层。"
            }
            FdmQml_Param{ id:raft_base_thickness; objectName: "raft_base_thickness"
                fdmLabel: "Raft 基础厚度"
                fdmDescription: "基础 Raft 层的层厚度。 该层应为与打印机打印平台牢固粘着的厚层。"
            }
            FdmQml_Param{ id:raft_base_line_width; objectName: "raft_base_line_width"
                fdmLabel: "Raft 基础走线宽度"
                fdmDescription: "基础 Raft 层的走线宽度。 这些走线应该是粗线，以便协助打印平台附着。"
            }
            FdmQml_Param{ id:raft_base_line_spacing; objectName: "raft_base_line_spacing"
                fdmLabel: "Raft 基础走线间距"
                fdmDescription: "基础 Raft 层的 Raft 走线之间的距离。 宽间距方便将 Raft 从打印平台移除。"
            }
            FdmQml_Param{ id:raft_speed; objectName: "raft_speed"
                fdmLabel: "Raft 打印速度"
                fdmDescription: "打印 Raft 的速度。"
                FdmQml_Param{ id:raft_surface_speed; objectName: "raft_surface_speed"
                    fdmLabel: "Raft 顶部打印速度"
                    fdmDescription: "打印顶部 Raft 层的速度。 这些层应以较慢的速度打印，以便喷嘴缓慢地整平临近的表面走线。"
                }
                FdmQml_Param{ id:raft_interface_speed; objectName: "raft_interface_speed"
                    fdmLabel: "Raft 中间打印速度"
                    fdmDescription: "打印中间 Raft 层的速度。 该层应以很慢的速度打印，因为喷嘴所出的材料量非常高。"
                }
                FdmQml_Param{ id:raft_base_speed; objectName: "raft_base_speed"
                    fdmLabel: "Raft 基础打印速度"
                    fdmDescription: "打印基础 Raft 层的速度。 该层应以很慢的速度打印，因为喷嘴所出的材料量非常高。"
                }
            }
            FdmQml_Param{ id:raft_acceleration; objectName: "raft_acceleration"
                fdmLabel: "Raft 打印加速度"
                fdmDescription: "打印 Raft 的加速度。"
                FdmQml_Param{ id:raft_surface_acceleration; objectName: "raft_surface_acceleration"
                    fdmLabel: "Raft 顶部打印加速度"
                    fdmDescription: "打印顶部 Raft 层的加速度。"
                }
                FdmQml_Param{ id:raft_interface_acceleration; objectName: "raft_interface_acceleration"
                    fdmLabel: "Raft 中间打印加速度"
                    fdmDescription: "打印中间 Raft 层的加速度。"
                }
                FdmQml_Param{ id:raft_base_acceleration; objectName: "raft_base_acceleration"
                    fdmLabel: "Raft 基础打印加速度"
                    fdmDescription: "打印基础 Raft 层的加速度。"
                }
            }
            FdmQml_Param{ id:raft_jerk; objectName: "raft_jerk"
                fdmLabel: "Raft 打印抖动速度"
                fdmDescription: "打印 Raft 的抖动速度。"
                FdmQml_Param{ id:raft_surface_jerk; objectName: "raft_surface_jerk"
                    fdmLabel: "Raft 顶部打印抖动速度"
                    fdmDescription: "打印顶部 Raft 层的抖动速度。"
                }
                FdmQml_Param{ id:raft_interface_jerk; objectName: "raft_interface_jerk"
                    fdmLabel: "Raft 中间打印抖动速度"
                    fdmDescription: "打印中间 Raft 层的抖动速度。"
                }
                FdmQml_Param{ id:raft_base_jerk; objectName: "raft_base_jerk"
                    fdmLabel: "Raft 基础打印抖动速度"
                    fdmDescription: "打印基础 Raft 层的抖动速度。"
                }
            }
            FdmQml_Param{ id:raft_fan_speed; objectName: "raft_fan_speed"
                fdmLabel: "Raft 风扇速度"
                fdmDescription: "Raft 的风扇速度。"
                FdmQml_Param{ id:raft_surface_fan_speed; objectName: "raft_surface_fan_speed"
                    fdmLabel: "Raft 顶部风扇速度"
                    fdmDescription: "顶部 Raft 层的风扇速度。"
                }
                FdmQml_Param{ id:raft_interface_fan_speed; objectName: "raft_interface_fan_speed"
                    fdmLabel: "Raft 中间风扇速度"
                    fdmDescription: "中间 Raft 层的风扇速度。"
                }
                FdmQml_Param{ id:raft_base_fan_speed; objectName: "raft_base_fan_speed"
                    fdmLabel: "Raft 基础风扇速度"
                    fdmDescription: "基础 Raft 层的风扇速度。"
                }
            }
        }
        FdmQml_Category{ id:dual; objectName: "dual"
            fdmLabel: "双重挤出"
            fdmDescription: "利用多个挤出机进行打印所用的设置。"
            FdmQml_Param{ id:prime_tower_enable; objectName: "prime_tower_enable"
                fdmLabel: "启用装填塔"
                fdmDescription: "在打印品相邻处打印一个塔，用于在每个喷嘴切换后装填材料。"
            }
            FdmQml_Param{ id:prime_tower_size; objectName: "prime_tower_size"
                fdmLabel: "装填塔尺寸"
                fdmDescription: "装填塔的宽度。"
            }
            FdmQml_Param{ id:prime_tower_min_volume; objectName: "prime_tower_min_volume"
                fdmLabel: "装填塔最小体积"
                fdmDescription: "为了清除足够的材料，装填塔每层的最小体积。"
            }
            FdmQml_Param{ id:prime_tower_position_x; objectName: "prime_tower_position_x"
                fdmLabel: "装填塔 X 位置"
                fdmDescription: "装填塔位置的 X 坐标。"
            }
            FdmQml_Param{ id:prime_tower_position_y; objectName: "prime_tower_position_y"
                fdmLabel: "装填塔 Y 位置"
                fdmDescription: "装填塔位置的 y 坐标。"
            }
            FdmQml_Param{ id:prime_tower_wipe_enabled; objectName: "prime_tower_wipe_enabled"
                fdmLabel: "擦拭装填塔上的不活动喷嘴"
                fdmDescription: "在用一个喷嘴打印装填塔后，从装填塔上的另一个喷嘴擦去渗出的材料。"
            }
            FdmQml_Param{ id:prime_tower_brim_enable; objectName: "prime_tower_brim_enable"
                fdmLabel: "装填塔 Brim"
                fdmDescription: "装填塔可能需要 Brim 提供额外附着力，无论模型是否需要。目前不可与 'Raft' 附着类型配合使用。"
            }
            FdmQml_Param{ id:ooze_shield_enabled; objectName: "ooze_shield_enabled"
                fdmLabel: "启用渗出罩"
                fdmDescription: "启用外部渗出罩。 这将在模型周围创建一个外壳，如果与第一个喷嘴处于相同的高度，则可能会擦拭第二个喷嘴。"
            }
            FdmQml_Param{ id:ooze_shield_angle; objectName: "ooze_shield_angle"
                fdmLabel: "渗出罩角度"
                fdmDescription: "渗出罩中的一个部件将具备的最大角度。 角度 0 度时为垂直，角度 90 度时为水平。 较小的角度会降低渗出罩失效次数，但会耗费更多材料。"
            }
            FdmQml_Param{ id:ooze_shield_dist; objectName: "ooze_shield_dist"
                fdmLabel: "渗出罩距离"
                fdmDescription: "渗出罩在 X/Y 方向距打印品的距离。"
            }
            FdmQml_Param{ id:switch_extruder_retraction_amount; objectName: "switch_extruder_retraction_amount"
                fdmLabel: "喷嘴切换回抽距离"
                fdmDescription: "切换挤出机时的回抽量。设为 0，不进行任何回抽。该值通常应与加热区的长度相同。"
            }
            FdmQml_Param{ id:switch_extruder_retraction_speeds; objectName: "switch_extruder_retraction_speeds"
                fdmLabel: "喷嘴切换回抽速度"
                fdmDescription: "回抽耗材的速度。 较高的回抽速度效果较好，但回抽速度过高可能导致耗材磨损。"
                FdmQml_Param{ id:switch_extruder_retraction_speed; objectName: "switch_extruder_retraction_speed"
                    fdmLabel: "喷嘴切换回抽速度"
                    fdmDescription: "喷嘴切换回抽期间耗材回抽的速度。"
                }
                FdmQml_Param{ id:switch_extruder_prime_speed; objectName: "switch_extruder_prime_speed"
                    fdmLabel: "喷嘴切换装填速度"
                    fdmDescription: "喷嘴切换回抽后耗材被推回的速度。"
                }
            }
            FdmQml_Param{ id:switch_extruder_extra_prime_amount; objectName: "switch_extruder_extra_prime_amount"
                fdmLabel: "喷嘴切换额外装填量"
                fdmDescription: "喷嘴切换后的额外装填材料。"
            }
        }
        FdmQml_Category{ id:meshfix; objectName: "meshfix"
            fdmLabel: "网格修复"
            fdmDescription: "使网格更适合 3D 打印。"
            FdmQml_Param{ id:meshfix_union_all; objectName: "meshfix_union_all"
                fdmLabel: "联合覆盖体积"
                fdmDescription: "忽略由网格内的重叠体积产生的内部几何，并将多个部分作为一个打印。 这可能会导致意外的内部孔洞消失。"
            }
            FdmQml_Param{ id:meshfix_union_all_remove_holes; objectName: "meshfix_union_all_remove_holes"
                fdmLabel: "移除所有孔洞"
                fdmDescription: "移除每层的孔洞，仅保留外部形状。 这会忽略任何不可见的内部几何。 但是，也会忽略可从上方或下方看到的层孔洞。"
            }
            FdmQml_Param{ id:meshfix_extensive_stitching; objectName: "meshfix_extensive_stitching"
                fdmLabel: "广泛缝合"
                fdmDescription: "广泛缝合尝试通过接触多边形来闭合孔洞，以此缝合网格中的开孔。 此选项可能会产生大量的处理时间。"
            }
            FdmQml_Param{ id:meshfix_keep_open_polygons; objectName: "meshfix_keep_open_polygons"
                fdmLabel: "保留断开连接的面"
                fdmDescription: "一般情况下，AnkerMake会尝试缝合网格中的小孔，并移除层中有大孔的部分。启用此选项将保留那些无法缝合的部分。当其他所有方法都无法产生正确的 G-code 时，最后才应考虑该选项。"
            }
            FdmQml_Param{ id:multiple_mesh_overlap; objectName: "multiple_mesh_overlap"
                fdmLabel: "合并网格重叠"
                fdmDescription: "让彼此接触的网格略微重叠。 这会让它们更好地粘合在一起。"
            }
            FdmQml_Param{ id:carve_multiple_volumes; objectName: "carve_multiple_volumes"
                fdmLabel: "移除网格交叉"
                fdmDescription: "移除多个网格互相重叠的区域。 如果合并的双材料模型彼此重叠，此选项可能适用。"
            }
            FdmQml_Param{ id:alternate_carve_order; objectName: "alternate_carve_order"
                fdmLabel: "交替网格移除"
                fdmDescription: "切换为与每个层相交的网格相交体积，以便重叠的网格交织在一起。 关闭此设置将使其中一个网格获得重叠中的所有体积，同时将其从其他网格中移除。"
            }
            FdmQml_Param{ id:remove_empty_first_layers; objectName: "remove_empty_first_layers"
                fdmLabel: "移除空白第一层"
                fdmDescription: "移除第一个打印层下方的空白层（如果存在）。如果“切片公差”设置被设为“独占”或“中间”，禁用此设置可能导致空白第一层。"
            }
        }
        FdmQml_Category{ id:blackmagic; objectName: "blackmagic"
            fdmLabel: "特殊模式"
            fdmDescription: "打印模型的非传统方式。"
            FdmQml_Param{ id:print_sequence; objectName: "print_sequence"
                fdmLabel: "打印序列"
                fdmDescription: "是要一次一层地打印所有模型，还是要等待打印完一个模型后再继续打印下一个。如果 a) 仅启用了一个挤出器，并且 b) 分离所有模型的方式使得整个打印头可在这些模型间移动，并且所有模型都低于喷嘴与 X/Y 轴之间的距离，则可使用排队打印模式。"
                fdmOptions:{
                    "all_at_once": "同时打印",
                    "one_at_a_time": "排队打印"
                }
            }
            FdmQml_Param{ id:infill_mesh; objectName: "infill_mesh"
                fdmLabel: "填充网格"
                fdmDescription: "使用此网格修改与其重叠的其他网格的填充物。 利用此网格的区域替换其他网格的填充区域。 建议仅为此网格打印一个壁，而不打印顶部/底部皮肤。"
            }
            FdmQml_Param{ id:infill_mesh_order; objectName: "infill_mesh_order"
                fdmLabel: "网格处理等级"
                fdmDescription: "在考虑多个重叠的填充网格时确定此网格的优先级。其中有多个填充网格重叠的区域将采用等级最高的网格的设置。具有较高等级的填充网格将修改具有较低等级的填充网格和普通网格的填充。"
            }
            FdmQml_Param{ id:cutting_mesh; objectName: "cutting_mesh"
                fdmLabel: "切割网格"
                fdmDescription: "将此网格的体积限制在其他网格内。 您可以使用它来制作采用不同的设置以及完全不同的挤出机的网格打印的特定区域。"
            }
            FdmQml_Param{ id:mold_enabled; objectName: "mold_enabled"
                fdmLabel: "模具"
                fdmDescription: "将模型作为模具打印，可进行铸造，以便获取与打印平台上的模型类似的模型。"
            }
            FdmQml_Param{ id:mold_width; objectName: "mold_width"
                fdmLabel: "最小模具宽度"
                fdmDescription: "模具外侧和模型外侧之间的最小距离。"
            }
            FdmQml_Param{ id:mold_roof_height; objectName: "mold_roof_height"
                fdmLabel: "模具顶板高度"
                fdmDescription: "用于打印模具的模型水平部分上方的高度。"
            }
            FdmQml_Param{ id:mold_angle; objectName: "mold_angle"
                fdmLabel: "模具角度"
                fdmDescription: "为模具创建的外壁的悬垂角度。 0° 将使模具的外壳垂直，而 90° 将使模型的外部遵循模型的轮廓。"
            }
            FdmQml_Param{ id:support_mesh; objectName: "support_mesh"
                fdmLabel: "支撑网格"
                fdmDescription: "使用此网格指定支撑区域。 可用于生成支撑结构。"
            }
            FdmQml_Param{ id:anti_overhang_mesh; objectName: "anti_overhang_mesh"
                fdmLabel: "防悬网格"
                fdmDescription: "使用此网格指定模型的任何部分不应被检测为悬垂的区域。 可用于移除不需要的支撑结构。"
            }
            FdmQml_Param{ id:magic_mesh_surface_mode; objectName: "magic_mesh_surface_mode"
                fdmLabel: "表面模式"
                fdmDescription: "将模型作为仅表面、一个空间或多个具有松散表面的空间处理。 正常打印模式仅打印封闭的空间。 “表面”打印跟踪网格表面的单个壁，没有填充物，也没有顶部/底部皮肤。 \"两者都\"将封闭空间正常打印，并将任何剩余多边形作为表面打印。"
                fdmOptions:{
                    "normal": "正常",
                    "surface": "表面",
                    "both": "两者都"
                }
            }
            FdmQml_Param{ id:magic_spiralize; objectName: "magic_spiralize"
                fdmLabel: "螺旋打印外轮廓"
                fdmDescription: "螺旋打印实现外部边缘的平滑 Z 移动。 这会在整个打印上建立一个稳定的 Z 增量。 该功能会将一个实心模型转变为具有实体底部的单壁打印。 只有在当每一层仅包含一个部分时才应启用此功能。"
            }
            FdmQml_Param{ id:smooth_spiralized_contours; objectName: "smooth_spiralized_contours"
                fdmLabel: "平滑螺旋轮廓"
                fdmDescription: "平滑螺旋轮廓以减少 Z 缝的可见性（Z 缝应在打印品上几乎看不到，但在层视图中仍然可见）。 请注意，平滑操作将倾向于模糊精细的表面细节。"
            }
        }
        FdmQml_Category{ id:experimental; objectName: "experimental"
            fdmLabel: "实验性"
            fdmDescription: "尚未完全充实的功能。"
            FdmQml_Param{ id:slicing_tolerance; objectName: "slicing_tolerance"
                fdmLabel: "切片公差"
                fdmDescription: "切片层的垂直公差。一般通过穿过每层厚度的中间截取横截面而产生该层的轮廓（中间）。此外，每层均可有一些区域，这些区域落入体积内部并遍布该层的整个厚度（排除），或层具有一些区域，这些区域落入该层内的任意位置（包含）。“包含”保留最多的细节，“排除”有利于最佳贴合，而“中间”保持最接近原始表面。"
                fdmOptions:{
                    "middle": "Middle",
                    "exclusive": "Exclusive",
                    "inclusive": "Inclusive"
                }
            }
            FdmQml_Param{ id:roofing_line_width; objectName: "roofing_line_width"
                fdmLabel: "顶部表面皮肤线宽"
                fdmDescription: "打印顶部区域单一走线宽度。"
            }
            FdmQml_Param{ id:roofing_pattern; objectName: "roofing_pattern"
                fdmLabel: "顶部表面皮肤图案"
                fdmDescription: "最顶层图案。"
                fdmOptions:{
                    "lines": "走线",
                    "concentric": "同心",
                    "zigzag": "锯齿形"
                }
            }
            FdmQml_Param{ id:roofing_monotonic; objectName: "roofing_monotonic"
                fdmLabel: "单调顶部表面顺序"
                fdmDescription: "按照一定的顺序打印顶部表面走线，使它们始终在一个方向上与相邻的走线重叠。这需要更长一些的打印时间，但会使平面看起来更一致。"
            }
            FdmQml_Param{ id:roofing_angles; objectName: "roofing_angles"
                fdmLabel: "顶部表面皮肤走线方向"
                fdmDescription: "当顶部表面皮肤层采用线条或锯齿状图案时使用的整数走线方向的列表。 列表中的元素随层的进度依次使用，当达到列表末尾时，它将从头开始。 列表项以逗号分隔，整个列表包含在方括号中。 默认是一个空列表，即意味着使用传统的默认角度（45 和 135 度）。"
            }
            FdmQml_Param{ id:infill_enable_travel_optimization; objectName: "infill_enable_travel_optimization"
                fdmLabel: "填充物空驶优化"
                fdmDescription: "启用后，可优化打印填充走线的顺序，缩短空驶距离。空驶时间的缩短很大程度上取决于被切割的模型、填充图案、密度等。请注意，对于具有许多小填充区域的一些模型，分割模型的时间可能会大幅增加。"
            }
            FdmQml_Param{ id:material_flow_dependent_temperature; objectName: "material_flow_dependent_temperature"
                fdmLabel: "自动温度"
                fdmDescription: "根据每一层的平均流速自动更改每层的温度。"
            }
            FdmQml_Param{ id:material_flow_temp_graph; objectName: "material_flow_temp_graph"
                fdmLabel: "流量温度图"
                fdmDescription: "数据连接材料流量（mm3/s）到温度（摄氏度）。"
            }
            FdmQml_Param{ id:minimum_polygon_circumference; objectName: "minimum_polygon_circumference"
                fdmLabel: "最小多边形周长"
                fdmDescription: "切片层中周长小于此数值的多边形将被滤除。以切片时间为代价，较低的值可实现较高分辨率的网格。它主要用于高分辨率 SLA 打印机和包含大量细节的极小 3D 模型。"
            }
            FdmQml_Param{ id:support_skip_some_zags; objectName: "support_skip_some_zags"
                fdmLabel: "将支撑结构分拆成块状"
                fdmDescription: "跳过部分支撑线连接，让支撑结构更容易脱离。 此设置适用于锯齿形支撑结构填充图案。"
            }
            FdmQml_Param{ id:support_skip_zag_per_mm; objectName: "support_skip_zag_per_mm"
                fdmLabel: "支撑块大小"
                fdmDescription: "每隔 N 毫米在支撑线之间略去一个连接,让支撑结构更容易脱离。"
                FdmQml_Param{ id:support_zag_skip_count; objectName: "support_zag_skip_count"
                    fdmLabel: "支撑块走线数"
                    fdmDescription: "每隔 N 个连接线跳过一个连接，让支撑结构更容易脱离。"
                }
            }
            FdmQml_Param{ id:draft_shield_enabled; objectName: "draft_shield_enabled"
                fdmLabel: "启用防风罩"
                fdmDescription: "这将在模型周围创建一个壁，该壁会吸住（热）空气并遮住外部气流。 对于容易卷曲的材料尤为有用。"
            }
            FdmQml_Param{ id:draft_shield_dist; objectName: "draft_shield_dist"
                fdmLabel: "防风罩 X/Y 距离"
                fdmDescription: "防风罩在 X/Y 方向与打印品的距离。"
            }
            FdmQml_Param{ id:draft_shield_height_limitation; objectName: "draft_shield_height_limitation"
                fdmLabel: "防风罩限制"
                fdmDescription: "设置防风罩的高度。 选择在模型的完整高度或有限高度处打印防风罩。"
                fdmOptions:{
                    "full": "完整",
                    "limited": "有限"
                }
            }
            FdmQml_Param{ id:draft_shield_height; objectName: "draft_shield_height"
                fdmLabel: "防风罩高度"
                fdmDescription: "防风罩的高度限制。 在此高度以上不会打印任何防风罩。"
            }
            FdmQml_Param{ id:conical_overhang_enabled; objectName: "conical_overhang_enabled"
                fdmLabel: "使悬垂可打印"
                fdmDescription: "更改打印模型的几何，以最大程度减少需要的支撑。 陡峭的悬垂物将变浅。 悬垂区域将下降变得更垂直。"
            }
            FdmQml_Param{ id:conical_overhang_angle; objectName: "conical_overhang_angle"
                fdmLabel: "最大模型角度"
                fdmDescription: "在悬垂变得可打印后悬垂的最大角度。 当该值为 0° 时，所有悬垂将被与打印平台连接的模型的一个部分替代，如果为 90° 时，不会以任何方式更改模型。"
            }
            FdmQml_Param{ id:conical_overhang_hole_size; objectName: "conical_overhang_hole_size"
                fdmLabel: "最大悬垂孔面积"
                fdmDescription: "在“使悬垂对象可打印”将其删除之前，模型底部的孔的最大面积。小于此面积的孔将会保留。值 0 mm² 将填充模型底部的所有孔。"
            }
            FdmQml_Param{ id:cross_infill_pocket_size; objectName: "cross_infill_pocket_size"
                fdmLabel: "交叉 3D 气槽大小"
                fdmDescription: "交叉 3D 图案的四向交叉处的气槽大小，高度为图案与自身接触的位置。"
            }
            FdmQml_Param{ id:cross_infill_density_image; objectName: "cross_infill_density_image"
                fdmLabel: "交叉加密图像密度"
                fdmDescription: "在打印的填充中，亮度值决定了相应位置的最小密度的图像的文件位置。"
            }
            FdmQml_Param{ id:cross_support_density_image; objectName: "cross_support_density_image"
                fdmLabel: "交叉填充密度图象"
                fdmDescription: "一个图像的文件位置，在这个图像中，亮度值决定了在支持中相应位置的最小密度。"
            }
            FdmQml_Param{ id:support_conical_enabled; objectName: "support_conical_enabled"
                fdmLabel: "启用锥形支撑"
                fdmDescription: "使底部的支撑区域小于悬垂处的支撑区域。"
            }
            FdmQml_Param{ id:support_conical_angle; objectName: "support_conical_angle"
                fdmLabel: "锥形支撑角度"
                fdmDescription: "锥形支撑的倾斜角度。 角度 0 度时为垂直，角度 90 度时为水平。 较小的角度会让支撑更为牢固，但需要更多材料。 负角会让支撑底座比顶部宽。"
            }
            FdmQml_Param{ id:support_conical_min_width; objectName: "support_conical_min_width"
                fdmLabel: "锥形支撑最小宽度"
                fdmDescription: "锥形支撑区域底部被缩小至的最小宽度。 宽度较小可导致不稳定的支撑结构。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_enabled; objectName: "magic_fuzzy_skin_enabled"
                fdmLabel: "模糊皮肤"
                fdmDescription: "在打印外墙时随机抖动，使表面具有粗糙和模糊的外观。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_outside_only; objectName: "magic_fuzzy_skin_outside_only"
                fdmLabel: "仅外部模糊皮肤"
                fdmDescription: "仅抖动部件的轮廓，而不抖动部件的孔。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_thickness; objectName: "magic_fuzzy_skin_thickness"
                fdmLabel: "模糊皮肤厚度"
                fdmDescription: "在其中进行抖动的宽度。 建议让此值低于外壁宽度，因为内壁不会更改。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_point_density; objectName: "magic_fuzzy_skin_point_density"
                fdmLabel: "模糊皮肤密度"
                fdmDescription: "在一层中的每个多边形上引入的点的平均密度。 注意，多边形的原始点被舍弃，因此低密度导致分辨率降低。"
                FdmQml_Param{ id:magic_fuzzy_skin_point_dist; objectName: "magic_fuzzy_skin_point_dist"
                    fdmLabel: "模糊皮肤点距离"
                    fdmDescription: "在每个走线部分引入的随机点之间的平均距离。 注意，多边形的原始点被舍弃，因此高平滑度导致分辨率降低。 该值必须大于模糊皮肤厚度的一半。"
                }
            }
            FdmQml_Param{ id:flow_rate_max_extrusion_offset; objectName: "flow_rate_max_extrusion_offset"
                fdmLabel: "流量补偿最大挤出偏移值"
                fdmDescription: "移动线材以补偿流量变化的最大距离（以毫米为单位）。"
            }
            FdmQml_Param{ id:flow_rate_extrusion_offset_factor; objectName: "flow_rate_extrusion_offset_factor"
                fdmLabel: "流量补偿因子"
                fdmDescription: "为补偿流量变化而将线材移动的距离，在挤出一秒钟的情况下占线材移动距离的百分比。"
            }
            FdmQml_Param{ id:wireframe_enabled; objectName: "wireframe_enabled"
                fdmLabel: "单线打印（以下简称 WP）"
                fdmDescription: "只打印一个具有稀疏网状结构的外表面，在“稀薄的空气中”打印。 这是通过在给定的 Z 间隔水平打印模型的轮廓来实现的，这些间隔通过上行线和下行斜线连接。"
            }
            FdmQml_Param{ id:wireframe_height; objectName: "wireframe_height"
                fdmLabel: "WP 连接高度"
                fdmDescription: "两个水平部分之间上行线和下行斜线的高度。 这决定网结构的整体密度。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_roof_inset; objectName: "wireframe_roof_inset"
                fdmLabel: "WP 顶板嵌入距离"
                fdmDescription: "在从顶板轮廓向内进行连接时所覆盖的距离。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_printspeed; objectName: "wireframe_printspeed"
                fdmLabel: "WP 速度"
                fdmDescription: "挤出材料时喷嘴移动的速度。 仅应用于单线打印。"
                FdmQml_Param{ id:wireframe_printspeed_bottom; objectName: "wireframe_printspeed_bottom"
                    fdmLabel: "WP 底部打印速度"
                    fdmDescription: "打印第一层的速度，该层是唯一接触打印平台的层。 仅应用于单线打印。"
                }
                FdmQml_Param{ id:wireframe_printspeed_up; objectName: "wireframe_printspeed_up"
                    fdmLabel: "WP 上升打印速度"
                    fdmDescription: "“在稀薄空气中”向上打印走线的速度。 仅应用于单线打印。"
                }
                FdmQml_Param{ id:wireframe_printspeed_down; objectName: "wireframe_printspeed_down"
                    fdmLabel: "WP 下降打印速度"
                    fdmDescription: "打印下行斜线的速度。 仅应用于单线打印。"
                }
                FdmQml_Param{ id:wireframe_printspeed_flat; objectName: "wireframe_printspeed_flat"
                    fdmLabel: "WP 水平打印速度"
                    fdmDescription: "打印模型水平轮廓的速度。 仅应用于单线打印。"
                }
            }
            FdmQml_Param{ id:wireframe_flow; objectName: "wireframe_flow"
                fdmLabel: "WP 打印流量"
                fdmDescription: "流量补偿：挤出的材料量乘以此值。 仅应用于单线打印。"
                FdmQml_Param{ id:wireframe_flow_connection; objectName: "wireframe_flow_connection"
                    fdmLabel: "WP 连接流量"
                    fdmDescription: "向上或向下时的流量补偿。 仅应用于单线打印。"
                }
                FdmQml_Param{ id:wireframe_flow_flat; objectName: "wireframe_flow_flat"
                    fdmLabel: "WP 平面流量"
                    fdmDescription: "打印平面走线时的流量补偿。 仅应用于单线打印。"
                }
            }
            FdmQml_Param{ id:wireframe_top_delay; objectName: "wireframe_top_delay"
                fdmLabel: "WP 顶部延迟"
                fdmDescription: "向上移动后的延迟时间，以便上行走线硬化。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_bottom_delay; objectName: "wireframe_bottom_delay"
                fdmLabel: "WP 底部延迟"
                fdmDescription: "向下移动后的延迟时间。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_flat_delay; objectName: "wireframe_flat_delay"
                fdmLabel: "WP 平面延迟"
                fdmDescription: "两个水平部分之间的延迟时间。 引入这样的延迟可以在连接点处与先前的层产生更好的附着，而太长的延迟会引起下垂。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_up_half_speed; objectName: "wireframe_up_half_speed"
                fdmLabel: "WP 轻松上行"
                fdmDescription: "这是一个以半速挤压的向上移动的距离。\n这可以导致更好地粘附到前几层，同时不会对这些层的材料加热太多"
            }
            FdmQml_Param{ id:wireframe_top_jump; objectName: "wireframe_top_jump"
                fdmLabel: "WP 纽结大小"
                fdmDescription: "在上行走线的顶部创建一个小纽结，使连续的水平层有更好的机会与其连接。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_fall_down; objectName: "wireframe_fall_down"
                fdmLabel: "WP 倒塌"
                fdmDescription: "材料在向上挤出后倒塌的距离。 将对此距离进行补偿。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_drag_along; objectName: "wireframe_drag_along"
                fdmLabel: "WP 拖行"
                fdmDescription: "向上挤出材料与斜向下挤出一起拖动的距离。 将对此距离进行补偿。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_strategy; objectName: "wireframe_strategy"
                fdmLabel: "WP 使用策略"
                fdmDescription: "用于确定两个连续层在每个连接点连接的策略。 回抽可让上行走线在正确的位置硬化，但可能导致耗材磨损。 可以在上行走线的尾端进行打结以便提高与其连接的几率，并让走线冷却；但这会需要较慢的打印速度。 另一种策略是补偿上行走线顶部的下垂；然而，线条不会总是如预期的那样下降。"
                fdmOptions:{
                    "compensate": "补偿",
                    "knot": "纽结",
                    "retract": "回抽"
                }
            }
            FdmQml_Param{ id:wireframe_straight_before_down; objectName: "wireframe_straight_before_down"
                fdmLabel: "WP 拉直下行走线"
                fdmDescription: "水平走线部分所覆盖的斜下行走线的百分比。 这可以防止上行线最顶端点下垂。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_roof_fall_down; objectName: "wireframe_roof_fall_down"
                fdmLabel: "WP 顶板倒塌"
                fdmDescription: "打印时，在“稀薄空气中”打印的水平顶板走线倒塌的距离。 将对此距离进行补偿。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_roof_drag_along; objectName: "wireframe_roof_drag_along"
                fdmLabel: "WP 顶板拖行"
                fdmDescription: "向内线的端部在返回至顶板外部轮廓时被拖行的距离。 将对此距离进行补偿。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_roof_outer_delay; objectName: "wireframe_roof_outer_delay"
                fdmLabel: "WP 顶板外部延迟"
                fdmDescription: "在成为顶板的孔的外围花费的时间。 较长的时间可确保更好的连接。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:wireframe_nozzle_clearance; objectName: "wireframe_nozzle_clearance"
                fdmLabel: "WP 喷嘴间隙"
                fdmDescription: "喷嘴和水平下行线之间的距离。 较大的间隙会让斜下行线角度较平缓，进而使第二层的上行连接较少。 仅应用于单线打印。"
            }
            FdmQml_Param{ id:adaptive_layer_height_enabled; objectName: "adaptive_layer_height_enabled"
                fdmLabel: "使用自适应图层"
                fdmDescription: "自适应图层根据模型形状计算图层高度。"
            }
            FdmQml_Param{ id:adaptive_layer_height_variation; objectName: "adaptive_layer_height_variation"
                fdmLabel: "自适应图层最大变化"
                fdmDescription: "最大允许高度与基层高度不同。"
            }
            FdmQml_Param{ id:adaptive_layer_height_variation_step; objectName: "adaptive_layer_height_variation_step"
                fdmLabel: "自适应图层变化步长"
                fdmDescription: "下一层与前一层的高度差。"
            }
            FdmQml_Param{ id:adaptive_layer_height_threshold; objectName: "adaptive_layer_height_threshold"
                fdmLabel: "自适应图层地形尺寸"
                fdmDescription: "两个相邻图层之间的目标水平距离。减小此设置的值会使要使用的图层变薄，从而使图层的边缘距离更近。"
            }
            FdmQml_Param{ id:clean_between_layers; objectName: "clean_between_layers"
                fdmLabel: "图层切换后擦拭喷嘴"
                fdmDescription: "是否包括图层切换后擦拭喷嘴的 G-Code（每层最多 1 个）。启用此设置可能会影响图层变化时的回抽。请使用“擦拭回抽”设置来控制擦拭脚本将在其中工作的图层回抽。"
            }
            FdmQml_Param{ id:max_extrusion_before_wipe; objectName: "max_extrusion_before_wipe"
                fdmLabel: "擦拭之间的材料量"
                fdmDescription: "在开始下一轮喷嘴擦拭之前可挤出的最大材料量。如果此值小于层中所需的材料量，则该设置在此层中无效，即每层仅限擦拭一次。"
            }
            FdmQml_Param{ id:wipe_retraction_enable; objectName: "wipe_retraction_enable"
                fdmLabel: "启用擦拭回抽"
                fdmDescription: "当喷嘴移动到非打印区域上方时回抽耗材。"
            }
            FdmQml_Param{ id:wipe_retraction_amount; objectName: "wipe_retraction_amount"
                fdmLabel: "擦拭回抽距离"
                fdmDescription: "耗材回抽量，可避免耗材在擦拭期间渗出。"
            }
            FdmQml_Param{ id:wipe_retraction_extra_prime_amount; objectName: "wipe_retraction_extra_prime_amount"
                fdmLabel: "擦拭回抽额外装填量"
                fdmDescription: "有些材料可能会在擦拭空驶过程中渗出，可以在这里进行补偿。"
            }
            FdmQml_Param{ id:wipe_retraction_speed; objectName: "wipe_retraction_speed"
                fdmLabel: "擦拭回抽速度"
                fdmDescription: "擦拭回抽移动期间耗材回抽和装填的速度。"
                FdmQml_Param{ id:wipe_retraction_retract_speed; objectName: "wipe_retraction_retract_speed"
                    fdmLabel: "擦拭回抽期间的回抽速度"
                    fdmDescription: "擦拭回抽移动期间耗材回抽的速度。"
                }
                FdmQml_Param{ id:wipe_retraction_prime_speed; objectName: "wipe_retraction_prime_speed"
                    fdmLabel: "擦拭回抽装填速度"
                    fdmDescription: "擦拭回抽移动期间耗材装填的速度。"
                }
            }
            FdmQml_Param{ id:wipe_pause; objectName: "wipe_pause"
                fdmLabel: "擦拭暂停"
                fdmDescription: "在未回抽后暂停。"
            }
            FdmQml_Param{ id:wipe_hop_enable; objectName: "wipe_hop_enable"
                fdmLabel: "擦拭 Z 抬升"
                fdmDescription: "在擦拭时，构建板会降低以在喷嘴与打印件之间形成间隙。这样可防止喷嘴在行程中撞击打印件，降低从构建板上撞掉打印件的可能性。"
            }
            FdmQml_Param{ id:wipe_hop_amount; objectName: "wipe_hop_amount"
                fdmLabel: "擦拭 Z 抬升高度"
                fdmDescription: "执行 Z 抬升的高度差。"
            }
            FdmQml_Param{ id:wipe_hop_speed; objectName: "wipe_hop_speed"
                fdmLabel: "擦拭抬升速度"
                fdmDescription: "抬升期间移动 Z 轴的速度。"
            }
            FdmQml_Param{ id:wipe_brush_pos_x; objectName: "wipe_brush_pos_x"
                fdmLabel: "擦拭刷 X 轴坐标"
                fdmDescription: "擦拭开始处的 X 轴坐标。"
            }
            FdmQml_Param{ id:wipe_repeat_count; objectName: "wipe_repeat_count"
                fdmLabel: "擦拭重复计数"
                fdmDescription: "在擦拭刷上移动喷嘴的次数。"
            }
            FdmQml_Param{ id:wipe_move_distance; objectName: "wipe_move_distance"
                fdmLabel: "擦拭移动距离"
                fdmDescription: "在擦拭刷上来回移动喷嘴头的距离。"
            }
        }
        FdmQml_Category{ id:command_line_settings; objectName: "command_line_settings"
            fdmLabel: "命令行设置"
            fdmDescription: "未从AnkerMake前端调用AnkerMake Engine 时使用的设置。"
            FdmQml_Param{ id:center_object; objectName: "center_object"
                fdmDescription: "是否将模型放置在打印平台中心 (0,0)，而不是使用模型在其中保存的坐标系统。"
                fdmLabel: "中心点"
            }
            FdmQml_Param{ id:mesh_position_x; objectName: "mesh_position_x"
                fdmDescription: "应用在模型 x 方向上的偏移量。"
                fdmLabel: "网格X位置"
            }
            FdmQml_Param{ id:mesh_position_y; objectName: "mesh_position_y"
                fdmDescription: "应用在模型 y 方向上的偏移量。"
                fdmLabel: "网格Y位置"
            }
            FdmQml_Param{ id:mesh_position_z; objectName: "mesh_position_z"
                fdmDescription: "应用在模型 z 方向上的偏移量。 利用此选项，您可以执行过去被称为“模型沉降”的操作。"
                fdmLabel: "网格Z位置"
            }
            FdmQml_Param{ id:mesh_rotation_matrix; objectName: "mesh_rotation_matrix"
                fdmLabel: "网格旋转矩阵"
                fdmDescription: "在将模型从文件中载入时应用在模型上的转换矩阵。"
            }
        }
    }
}
