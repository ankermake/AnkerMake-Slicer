import QtQuick 2.2
import FdmQml 1.0
// qrc:/Settings/FdmJsonObjTree_All.qml
FdmQml_Root{ id:fdmextruder_def_json; objectName: "qrc:/Settings/FdmJsonObjTree_All.qml"
    property string name: "FDMプリンター概要"
    property int version: 2
    FdmQml_Settings{ id:settings; objectName: "settings"
        FdmQml_Category{ id:anker_fdm_setting; objectName: "anker_fdm_setting"
            fdmLabel: "AnkerMake FDM設定"
            fdmDescription: "AnkerMake用カスタマイズ設定"
            FdmQml_Param{ id:international_language; objectName: "international_language"
                fdmLabel: "言語"
                fdmDescription: "言語切り替えスイッチ"
            }
            FdmQml_Param{ id:param_print_mode; objectName: "param_print_mode"
                fdmLabel: "印刷モード"
                fdmDescription: "3つの印刷モード、通常の印刷、高速モード、ファインモード"
                fdmOptions:{
                    "normal": "ノーマルモード",
                    "fast": "高速モード",
                    "fine": "ファインモード"
                }
            }

            FdmQml_Param{ id:anker_param_ai_camera; objectName: "anker_param_ai_camera"
                fdmLabel: "AnkerMake AIカメラ"
                fdmDescription: "AnkerMakeの3Dプリンターで写真やタイムラプス動画を撮影"
                FdmQml_Param{ id:camera_take_picture_time; objectName: "camera_take_picture_time"
                    fdmLabel: "タイムラプス設定"
                    fdmDescription: "タイムラプスのパラメーターを設定"
                }
                FdmQml_Param{ id:anker_abc_1; objectName: "anker_abc_1"
                    //  ( Non-chinese will not be displayed )
                    fdmLabel: ""
                    fdmDescription: ""
                }
                FdmQml_Param{ id:anker_abc_2; objectName: "anker_abc_2"
                    //  ( Non-chinese will not be displayed )
                    fdmLabel: ""
                    fdmDescription: ""
                }
                FdmQml_Param{ id:camera_position; objectName: "camera_position"
                    fdmLabel: "カメラポジション"
                    fdmDescription: "モデルに対するカメラの位置"
                    FdmQml_Param{ id:camera_position_x; objectName: "camera_position_x"
                        fdmLabel: "X-ポジション"
                        fdmDescription: "X軸におけるカメラの位置"
                    }
                    FdmQml_Param{ id:camera_position_y; objectName: "camera_position_y"
                        fdmLabel: "Y-ポジション"
                        fdmDescription: "Y軸におけるカメラの位置"
                    }
                    FdmQml_Param{ id:camera_position_z; objectName: "camera_position_z"
                        fdmLabel: "Z-ポジション"
                        fdmDescription: "Z軸におけるカメラの位置"
                    }
                }
                FdmQml_Param{ id:camera_up; objectName: "camera_up"
                    fdmLabel: "Camera Vertical Movement"
                    fdmDescription: "Move the camera up or down."
                    FdmQml_Param{ id:camera_up_x; objectName: "camera_up_x"
                        fdmLabel: "camera up dirction X"
                        fdmDescription: "camera up dirction X"
                    }
                    FdmQml_Param{ id:camera_up_y; objectName: "camera_up_y"
                        fdmLabel: "camera up dirction Y"
                        fdmDescription: "camera up dirction Y."
                    }
                    FdmQml_Param{ id:camera_up_z; objectName: "camera_up_z"
                        fdmLabel: "camera up dirction Z"
                        fdmDescription: "camera up dirction Z."
                    }
                }
                FdmQml_Param{ id:camera_view_vector; objectName: "camera_view_vector"
                    fdmLabel: "visual center position"
                    fdmDescription: "visual center position."
                    FdmQml_Param{ id:camera_view_vector_x; objectName: "camera_view_vector_x"
                        fdmLabel: "visual center position X"
                        fdmDescription: "visual center position X."
                    }
                    FdmQml_Param{ id:camera_view_vector_y; objectName: "camera_view_vector_y"
                        fdmLabel: "visual center position Y"
                        fdmDescription: "visual center position Y."
                    }
                    FdmQml_Param{ id:camera_view_vector_z; objectName: "camera_view_vector_z"
                        fdmLabel: "visual center position Z"
                        fdmDescription: "visual center position Z."
                    }
                }
                FdmQml_Param{ id:camera_resolution; objectName: "camera_resolution"
                    fdmLabel: "Camera Resolution"
                    fdmDescription: "camera resolution."
                    FdmQml_Param{ id:camera_resolution_w; objectName: "camera_resolution_w"
                        fdmLabel: "Camera Resolution Width"
                        fdmDescription: "camera resolution width."
                    }
                    FdmQml_Param{ id:camera_resolution_h; objectName: "camera_resolution_h"
                        fdmLabel: "Camera Resolution Height"
                        fdmDescription: "camera resolution height"
                    }
                }
                FdmQml_Param{ id:camera_field_view; objectName: "camera_field_view"
                    fdmLabel: "Vertical Field View"
                    fdmDescription: "Vertical Field View."
                }
            }
            FdmQml_Param{ id:ak_meshfix_dynamic_resolution; objectName: "ak_meshfix_dynamic_resolution"
                fdmLabel: "ダイナミック解像度"
                fdmDescription: "メッシュの解像度を入力"
            }
            FdmQml_Param{ id:meshfix_maximum_resolution; objectName: "meshfix_maximum_resolution"
                fdmLabel: "最大解像度"
                fdmDescription: "スライス後の線分の最小サイズ。これを増やすと、メッシュの解像度が低くなります。これにより、プリンタが g コードの処理速度に追いつくことができ、処理できないメッシュの詳細を取り除いてスライス速度を速めます。"
            }
            FdmQml_Param{ id:meshfix_maximum_travel_resolution; objectName: "meshfix_maximum_travel_resolution"
                fdmLabel: "最大移動解像度"
                fdmDescription: "スライス後の移動線分の最小サイズ。これを増やすと、移動の跡が滑らかでなくなります。これにより、プリンタが g コードの処理速度に追いつくことができますが、精度が低下します。"
            }
            FdmQml_Param{ id:meshfix_maximum_deviation; objectName: "meshfix_maximum_deviation"
                fdmLabel: "最大偏差"
                fdmDescription: "最大解像度設定の解像度を下げるときに許容される最大偏差です。これを大きくするとプリントの精度は低くなりますが、g-codeは小さくなります。最大偏差は最大解像度の限度であるため、最大偏差でこの2つが競合する場合には常にtrueとなります。"
            }
            FdmQml_Param{ id:relative_extrusion; objectName: "relative_extrusion"
                fdmLabel: "相対押出"
                fdmDescription: "絶対押出ではなく、相対押出を使用します。相対Eステップを使用すると、G-codeの後処理が容易になります。ただし、すべてのプリンタでサポートされているわけではありません。絶対的Eステップと比較して、材料の量にごくわずかな偏差が生じることがあります。この設定に関係なく、G-codeスクリプトが出力される前にエクストルーダーのモードは常に絶対値にて設定されています。"
            }
            FdmQml_Param{ id:ak_VAJK_K_enabled; objectName: "ak_VAJK_K_enabled"
                fdmLabel: "リニアアドバンス"
                fdmDescription: "高速プリント中にエクストルーダー内の圧力上昇を予測"
                FdmQml_Param{ id:ak_K_print; objectName: "ak_K_print"
                    fdmLabel: "リニアアドバンス"
                    fdmDescription: "高速プリント中にエクストルーダー内の圧力上昇を予測"
                    FdmQml_Param{ id:ak_K_infill; objectName: "ak_K_infill"
                        fdmLabel: "インフィル"
                        fdmDescription: "インフィル　リニアアドバンス"
                    }
                    FdmQml_Param{ id:ak_K_wall; objectName: "ak_K_wall"
                        fdmLabel: "ウォール"
                        fdmDescription: "ウォール　リニアアドバンス"
                        FdmQml_Param{ id:ak_K_wall_0; objectName: "ak_K_wall_0"
                            fdmLabel: "外側ウォール"
                            fdmDescription: "外側ウォール　リニアアドバンス"
                        }
                        FdmQml_Param{ id:ak_K_wall_x; objectName: "ak_K_wall_x"
                            fdmLabel: "内側ウォール"
                            fdmDescription: "内側ウォール　リニアアドバンス"
                        }
                    }
                    FdmQml_Param{ id:ak_K_topbottom; objectName: "ak_K_topbottom"
                        fdmLabel: "スキン"
                        fdmDescription: "トップ＆ボトムスキン　リニアアドバンス"
                    }
                    FdmQml_Param{ id:ak_K_support; objectName: "ak_K_support"
                        fdmLabel: "サポート"
                        fdmDescription: "サポート　リニアアドバンス"
                    }
                    FdmQml_Param{ id:ak_K_skirt_brim; objectName: "ak_K_skirt_brim"
                        fdmLabel: "スカート/ブリム"
                        fdmDescription: "スカート/ブリム　リニアアドバンス"
                    }
                    FdmQml_Param{ id:ak_K_layer_0; objectName: "ak_K_layer_0"
                        fdmLabel: "初期レイヤー"
                        fdmDescription: "初期レイヤー　リニアアドバンス"
                    }
                }
            }
            FdmQml_Param{ id:shrink_infill_shrink_distance; objectName: "shrink_infill_shrink_distance"
                fdmLabel: "インフィル収縮距離"
                fdmDescription: "ウォール付近のインフィル距離を設定"
            }
            FdmQml_Param{ id:wall_ruler_concave_convex_enable; objectName: "wall_ruler_concave_convex_enable"
                fdmLabel: "凹凸認識"
                fdmDescription: "凹凸曲線の品質を向上させる設定"
                FdmQml_Param{ id:wall_ruler_concave_percent; objectName: "wall_ruler_concave_percent"
                    fdmLabel: "凹部の設定"
                    fdmDescription: "凹部の設定の強さを入力"
                }
                FdmQml_Param{ id:wall_ruler_convex_percent; objectName: "wall_ruler_convex_percent"
                    fdmLabel: "凸部の設定"
                    fdmDescription: "凸部の設定の強さを入力"
                }
                FdmQml_Param{ id:wall_ruler_separate_percent; objectName: "wall_ruler_separate_percent"
                    fdmLabel: "ウォール間距離"
                    fdmDescription: "外側ウォールと内側ウォールの距離"
                }
            }
            FdmQml_Param{ id:small_hole_max_size; objectName: "small_hole_max_size"
                fdmLabel: "小さい穴の最大サイズ"
                fdmDescription: "このサイズ未満の直径の穴や部品の輪郭を、微細特徴速度を使用して印刷します。"
                FdmQml_Param{ id:small_feature_max_length; objectName: "small_feature_max_length"
                    fdmLabel: "小型形体の最大長さ"
                    fdmDescription: "この長さ未満の特徴の輪郭を、微細特徴速度を使用して印刷します。"
                }
                FdmQml_Param{ id:small_feature_speed_factor; objectName: "small_feature_speed_factor"
                    fdmLabel: "小さな特徴の速度"
                    fdmDescription: "小型形体は通常のプリント速度に対してこの割合でプリントされます。低速でプリントすると、接着と精度が向上します。"
                }
                FdmQml_Param{ id:small_feature_speed_factor_0; objectName: "small_feature_speed_factor_0"
                    fdmLabel: "小型形体の初期レイヤー速度"
                    fdmDescription: "最初のレイヤーの小型形体は通常のプリント速度に対してこの割合でプリントされます。低速でプリントすると、接着と精度が向上します。"
                }
            }
            FdmQml_Param{ id:wall_min_extrusion; objectName: "wall_min_extrusion"
                fdmLabel: "ウォールの最小フロー"
                fdmDescription: "ウォールに対する最小のフロー (押し出し量)。フローがこの値より小さいウォールは、移動に置き換えられます。"
            }
            FdmQml_Param{ id:wall_overhang_angle; objectName: "wall_overhang_angle"
                fdmLabel: "張り出し壁アングル"
                fdmDescription: "この角度以上に張り出した壁は、オーバーハング壁設定を使用してプリントされます。値が90の場合は、オーバーハング壁として処理されません。サポートによってサポートされているオーバーハングも、オーバーハングとして処理されません。"
                FdmQml_Param{ id:wall_overhang_area_filter; objectName: "wall_overhang_area_filter"
                    fdmLabel: "ドレープウォールエリアフィルタリング"
                    fdmDescription: "オーバーハングとして処理される最小面積を調整"
                }
                FdmQml_Param{ id:wall_overhang_extend_circle; objectName: "wall_overhang_extend_circle"
                    fdmLabel: "オーバーハング最大値"
                    fdmDescription: "オーバーハングの適用場所を設定"
                }
                FdmQml_Param{ id:wall_overhang_extend_type; objectName: "wall_overhang_extend_type"
                    fdmLabel: "ウォールオーバーハング効果速度"
                    fdmDescription: "オーバーハングに対するプリントの減速方法を設定"
                    fdmOptions:{
                        "instant": "instant",
                        "gradually_xy": "オーバーハングの手前で徐々に",
                        "circle": "レイヤー全体に対して徐々に"
                    }
                    FdmQml_Param{ id:overhang_min_line_length; objectName: "overhang_min_line_length"
                        fdmLabel: "オーバーハング最小値"
                        fdmDescription: "オーバーハングとして処理される最小ラインの長さを入力"
                    }
                    FdmQml_Param{ id:wall_overhang_extend_xy_distance; objectName: "wall_overhang_extend_xy_distance"
                        fdmLabel: "ウォールオーバーハング速度バッファ (X/Y軸)"
                        fdmDescription: "X軸またはY軸のオーバーハングに近付いた時に速度が低下する距離を入力"
                    }
                    FdmQml_Param{ id:wall_overhang_extend_z_distance; objectName: "wall_overhang_extend_z_distance"
                        fdmLabel: "ウォールオーバーハング速度バッファ (Z軸)"
                        fdmDescription: "Z軸のオーバーハングに近付いた時に速度が低下する距離を入力"
                    }
                    FdmQml_Param{ id:overhang_inner_wall_shrink_distance; objectName: "overhang_inner_wall_shrink_distance"
                        fdmLabel: "ウォールオーバーハング収縮距離"
                        fdmDescription: "外側ウォールと内側ウォールの間にわずかな隙間を作り、オーバーハングの品質を高める"
                    }
                }
                FdmQml_Param{ id:overhang_related_inner_wall_slow_down; objectName: "overhang_related_inner_wall_slow_down"
                    fdmLabel: "オーバーハングの内壁を遅くする"
                    fdmDescription: "張り出し速度で張り出し内壁を印刷."
                }
                FdmQml_Param{ id:wall_overhang_speed_factor; objectName: "wall_overhang_speed_factor"
                    fdmLabel: "張り出し壁速度"
                    fdmDescription: "張り出し壁は、この割合で通常のプリント速度でプリントされます。"
                }
                FdmQml_Param{ id:wall_overhang_flow_factor; objectName: "wall_overhang_flow_factor"
                    fdmLabel: "オーバーハングウォールフロー"
                    fdmDescription: "オーバーハングのプリント時のフロー (押し出し量) を調整"
                }
            }
            FdmQml_Param{ id:skin_overhang_enable; objectName: "skin_overhang_enable"
                fdmLabel: "オーバーハングスキンオーバーラップ"
                fdmDescription: "スキンウォールと充填部の重なりを調節"
                FdmQml_Param{ id:skin_overlap_overhang_speed; objectName: "skin_overlap_overhang_speed"
                    fdmLabel: "オーバーハングスキンオーバーラップのプリント速度"
                    fdmDescription: "オーバーハングスキンオーバーラップに近付いた時のプリントの減速方法を設定"
                }
                FdmQml_Param{ id:skin_overlap_overhang_flow; objectName: "skin_overlap_overhang_flow"
                    fdmLabel: "オーバーハングスキンオーバーラップのフロー"
                    fdmDescription: "オーバーハングスキンオーバーラップに近付いた時のフロー (押し出し量) を設定"
                }
                FdmQml_Param{ id:skin_overlap_overhang; objectName: "skin_overlap_overhang"
                    fdmLabel: "オーバーハングスキンオーバーラップの割合"
                    fdmDescription: "ウォールとスキンの中心線の端の重なり具合を調整します。わずかな重なりで、壁がスキンにしっかりと接着されます。注意：50％を超えると、スキンがウォールからはみ出ることがあります。"
                    FdmQml_Param{ id:skin_overlap_overhang_mm; objectName: "skin_overlap_overhang_mm"
                        fdmLabel: "オーバーハングスキンオーバーラップ"
                        fdmDescription: "ウォールとスキンの中心線（の端点）の重なり具合を調整します。わずかなオーバーラップで、ウォールがスキンにしっかりと接着されます。なお、スキンとウォールの線幅が等しい場合、ウォールの線幅の半分以上の値を設定すると、スキンの押し出しノズルの位置がウォールの中央を越えてしまうことがあるため、スキンがウォールを越えてしまうことがあります。"
                    }
                }
            }
            FdmQml_Param{ id:index_of_role_order_in_part; objectName: "index_of_role_order_in_part"
                fdmLabel: "順序（単連結領域）"
                fdmDescription: "単一連結領域の[内側、外側、インフィル]の印刷順序"
                fdmOptions:{
                    "0" : "Order_Nothing"           ,
                    "1" : "インフィル->外側->内側",
                    "2" : "インフィル->内側->外側",
                    "3" : "外側->内側->インフィル",
                    "4" : "内側->外側->インフィル",
                    "5" : "内側->インフィル->外側",
                    "6" : "外側->インフィル->内側",
                }
            }
            FdmQml_Param{ id:optimize_wall_0_printing_order; objectName: "optimize_wall_0_printing_order"
                fdmLabel: "外側ウォールのプリント"
                fdmDescription: "外側ウォールのプリントタイミングを設定"
            }
            FdmQml_Param{ id:optimize_wall_hole_printing; objectName: "optimize_wall_hole_printing"
                fdmLabel: "穴"
                fdmDescription: "穴壁の印刷を最適化し、穴壁をより滑らかにします。このオプションを有効にするとスライシング時間が長くなる可能性があります。アセンブリ部品を印刷する場合は、このオプションを有効にすることを推奨します。"
            }
            FdmQml_Param{ id:reverse_hole_outer_wall_print_orient; objectName: "reverse_hole_outer_wall_print_orient"
                fdmLabel: "穴のリバースプリント"
                fdmDescription: "穴の外側ウォールに到達したら、時計回りにプリントします。これにより、穴の外側ウォールの壁から離れる時に、より滑らかにプリントすることができます。"
            }
            FdmQml_Param{ id:optimize_wall_small_printing; objectName: "optimize_wall_small_printing"
                fdmLabel: "スモールパーツのプリント速度"
                fdmDescription: "モデルの複雑な部分に到達すると、プリントの速度が遅くなります。"
            }
            FdmQml_Param{ id:optimize_single_part_z_seam; objectName: "optimize_single_part_z_seam"
                fdmLabel: "Zシーム"
                fdmDescription: "次のレイヤーに移る時にできる凸凹を滑らかにします"
            }
            FdmQml_Param{ id:show_machine_setting; objectName: "show_machine_setting"
                fdmLabel: "ジャークの最大値"
                fdmDescription: "ジャーク設定の最大値を表示"
                FdmQml_Param{ id:machine_max_jerk_xy; objectName: "machine_max_jerk_xy"
                    fdmLabel: "X-Yデフォルトジャーク"
                    fdmDescription: "水平面内での移動のデフォルトジャーク。"
                }
                FdmQml_Param{ id:machine_max_jerk_z; objectName: "machine_max_jerk_z"
                    fdmLabel: "Zデフォルトジャーク"
                    fdmDescription: "Z方向のモーターのデフォルトジャーク。"
                }
                FdmQml_Param{ id:machine_max_jerk_e; objectName: "machine_max_jerk_e"
                    fdmLabel: "フィラメントデフォルトジャーク"
                    fdmDescription: "フィラメントのモーターのデフォルトジャーク。"
                }
            }
            FdmQml_Param{ id:bridge_settings_enabled; objectName: "bridge_settings_enabled"
                fdmLabel: "ブリッジ設定を有効にする"
                fdmDescription: "ブリッジを検出し、ブリッジをプリントしながらてプリント速度、フロー、ファンの設定を変更します。"
                FdmQml_Param{ id:bridge_split_min_length; objectName: "bridge_split_min_length"
                    fdmLabel: "ブリッジ スプリットの最小の長さ"
                    fdmDescription: "長いブリッジを 3 つの部分に分割して可変速印刷を実行します"
                }
                FdmQml_Param{ id:bridge_shrink_length; objectName: "bridge_shrink_length"
                    fdmLabel: "ブリッジ縮小の長さ"
                    fdmDescription: "ブリッジ構成を適用するためのブリッジ開始点からのオフセット"
                }
                FdmQml_Param{ id:bridge_wall_min_length; objectName: "bridge_wall_min_length"
                    fdmLabel: "ブリッジ壁の最小長さ"
                    fdmDescription: "この値より短いサポートされていない壁が通常の壁設定でプリントされます。長いサポートされていない壁は、ブリッジ壁設定でプリントされます。"
                }
                FdmQml_Param{ id:bridge_skin_support_threshold; objectName: "bridge_skin_support_threshold"
                    fdmLabel: "ブリッジスキンサポートのしきい値"
                    fdmDescription: "対象領域に対してこのパーセンテージ未満のスキン領域がサポートされている場合、ブリッジ設定でプリントします。それ以外の場合は、通常のスキン設定でプリントします。"
                }
                FdmQml_Param{ id:bridge_sparse_infill_max_density; objectName: "bridge_sparse_infill_max_density"
                    fdmLabel: "ブリッジスパースインフィル最大密度"
                    fdmDescription: "スパース（疎）であると見なされるインフィルの最大密度。スパースインフィル上のスキンは、サポートされていないと見なされるため、ブリッジスキンとして扱われる可能性があります。"
                }
                FdmQml_Param{ id:bridge_wall_coast; objectName: "bridge_wall_coast"
                    fdmLabel: "ブリッジ壁コースティング"
                    fdmDescription: "この設定は、ブリッジ壁が始まる直前に、エクストルーダーを動かす距離を制御します。ブリッジが始まる前にコースティングすることにより、ノズル内が減圧され、ブリッジがより平らになります。"
                }
                FdmQml_Param{ id:bridge_wall_speed; objectName: "bridge_wall_speed"
                    fdmLabel: "ブリッジ壁速度"
                    fdmDescription: "ブリッジ壁をプリントする速度。"
                }
                FdmQml_Param{ id:bridge_wall_material_flow; objectName: "bridge_wall_material_flow"
                    fdmLabel: "ブリッジ壁フロー"
                    fdmDescription: "ブリッジ壁をプリントするときは、材料の吐出量をこの値で乗算します。"
                }
                FdmQml_Param{ id:bridge_skin_speed; objectName: "bridge_skin_speed"
                    fdmLabel: "ブリッジスキン速度"
                    fdmDescription: "ブリッジスキン領域がプリントされる速度。"
                }
                FdmQml_Param{ id:bridge_skin_material_flow; objectName: "bridge_skin_material_flow"
                    fdmLabel: "ブリッジスキンフロー"
                    fdmDescription: "ブリッジスキン領域をプリントするときは、材料の吐出量をこの値で乗算します。"
                }
                FdmQml_Param{ id:bridge_skin_density; objectName: "bridge_skin_density"
                    fdmLabel: "ブリッジスキンの密度"
                    fdmDescription: "ブリッジスキンレイヤーの密度。100 以下の場合は、スキンライン間のギャップを増やします。"
                }
                FdmQml_Param{ id:bridge_fan_speed; objectName: "bridge_fan_speed"
                    fdmLabel: "ブリッジファン速度"
                    fdmDescription: "ブリッジ壁とスキンをプリントする際に使用するファン速度の割合。"
                }
                FdmQml_Param{ id:bridge_enable_more_layers; objectName: "bridge_enable_more_layers"
                    fdmLabel: "ブリッジを構成する多重レイヤー"
                    fdmDescription: "有効な場合、空気上部の第二および第三レイヤーは以下の設定でプリントされます。それ以外の場合は、それらのレイヤーは通常の設定でプリントされます。"
                    FdmQml_Param{ id:bridge_skin_speed_2; objectName: "bridge_skin_speed_2"
                        fdmLabel: "ブリッジセカンドスキンの速度"
                        fdmDescription: "セカンドブリッジのスキンレイヤーをプリントする際に使用するプリント速度。"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_2; objectName: "bridge_skin_material_flow_2"
                        fdmLabel: "ブリッジセカンドスキンのフロー"
                        fdmDescription: "セカンドブリッジスキンレイヤーをプリントするときは、材料の吐出量をこの値で乗算します。"
                    }
                    FdmQml_Param{ id:bridge_skin_density_2; objectName: "bridge_skin_density_2"
                        fdmLabel: "ブリッジセカンドスキンの密度"
                        fdmDescription: "セカンドブリッジスキンレイヤーの密度。100 以下の場合は、スキンライン間のギャップを増やします。"
                    }
                    FdmQml_Param{ id:bridge_fan_speed_2; objectName: "bridge_fan_speed_2"
                        fdmLabel: "ブリッジセカンドスキンのファン速度"
                        fdmDescription: "セカンドブリッジのスキンレイヤーをプリントする際に使用するファン速度の割合。"
                    }
                    FdmQml_Param{ id:bridge_skin_speed_3; objectName: "bridge_skin_speed_3"
                        fdmLabel: "ブリッジサードスキンの速度"
                        fdmDescription: "サードブリッジのスキンレイヤーをプリントする際に使用するプリント速度。"
                    }
                    FdmQml_Param{ id:bridge_skin_material_flow_3; objectName: "bridge_skin_material_flow_3"
                        fdmLabel: "ブリッジサードスキンのフロー"
                        fdmDescription: "サードブリッジスキンレイヤーをプリントするときは、材料の吐出量をこの値で乗算します。"
                    }
                    FdmQml_Param{ id:bridge_skin_density_3; objectName: "bridge_skin_density_3"
                        fdmLabel: "ブリッジサードスキンの密度"
                        fdmDescription: "サードブリッジスキンレイヤーの密度。100 以下の場合は、スキンライン間のギャップを増やします。"
                    }
                    FdmQml_Param{ id:bridge_fan_speed_3; objectName: "bridge_fan_speed_3"
                        fdmLabel: "ブリッジサードスキンのファン速度"
                        fdmDescription: "サードブリッジのスキンレイヤーをプリントする際に使用するファン速度の割合。"
                    }
                }
            }
            FdmQml_Param{ id:machine_index_name; objectName: "machine_index_name"
                fdmLabel: "マシンインデックス名"
                fdmDescription: "マシンインデックスの名称"
            }
            FdmQml_Param{ id:extruder_index_name; objectName: "extruder_index_name"
                fdmLabel: "エクストルーダーインデックス名"
                fdmDescription: "エクストルーダーインデックスの名称"
            }
            FdmQml_Param{ id:material_index_name; objectName: "material_index_name"
                fdmLabel: "マテリアルインデックス名"
                fdmDescription: "マテリアルインデックスの名称"
            }
            FdmQml_Param{ id:parameter_index_name; objectName: "parameter_index_name"
                fdmLabel: "パラメーターインデックス名"
                fdmDescription: "パラメーター"
            }
            FdmQml_Param{ id:extract_param_from_gcode; objectName: "extract_param_from_gcode"
                fdmLabel: "Gコードのパラメータープレビュー"
                fdmDescription: "Gコードファイルのプレビュー中にGコードのパラメーターを表示"
            }
            FdmQml_Param{ id:check_param_lost_interval; objectName: "check_param_lost_interval"
                fdmLabel: "パラメータを確認する秒数"
                fdmDescription: "パラメータのステータスをチェックして、欠落しているかどうかを確認してください"
            }
            FdmQml_Param{ id:coasting_enable; objectName: "coasting_enable"
                fdmLabel: "コースティングを有効にする"
                fdmDescription: "コースティングは、それぞれの造形ラインの最後の部分をトラベルパスで置き換えます。はみ出た材料は、糸引きを減らすために造形ライン最後の部分をプリントするために使用される。"
                FdmQml_Param{ id:coasting_volume; objectName: "coasting_volume"
                    fdmLabel: "コースティングのボリューム"
                    fdmDescription: "はみ出るフィラメントのボリューム。この値は、一般に、ノズル直径の3乗に近い値でなければならない。"
                }
                FdmQml_Param{ id:coasting_min_volume; objectName: "coasting_min_volume"
                    fdmLabel: "コースティング前の最小ボリューム"
                    fdmDescription: "コースティングに必要な最小の容積。より小さい押出経路の場合、ボーデンチューブにはより少ない圧力しか蓄積されないので、コースティングの容積は比例する。この値は、常に、コースティングのボリュームよりも大きな必要があります。"
                }
                FdmQml_Param{ id:coasting_speed; objectName: "coasting_speed"
                    fdmLabel: "コースティング速度"
                    fdmDescription: "コースティング中の移動速度。プリント時の経路の速度設定に比例します。ボーデンチューブの圧力が低下するので、100％よりわずかに低い値が推奨される。"
                }
            }
        }
        FdmQml_Category{ id:machine_settings; objectName: "machine_settings"
            fdmLabel: "プリンター"
            fdmDescription: "プリンター詳細設定"
            FdmQml_Param{ id:machine_name; objectName: "machine_name"
                fdmLabel: "プリンターのタイプ"
                fdmDescription: "3Dプリンターの機種名。"
            }
            FdmQml_Param{ id:machine_show_variants; objectName: "machine_show_variants"
                fdmLabel: "プリンターのバリエーションの表示"
                fdmDescription: "このプリンターのバリエーションを表示するかどうかは、別のjsonファイルに記述されています。"
            }
            FdmQml_Param{ id:machine_start_gcode; objectName: "machine_start_gcode"
                fdmLabel: "G-Codeの開始"
                fdmDescription: "Gコードのコマンドをファイルの先頭に追加します。\\nでコマンドを区切ります。"
            }
            FdmQml_Param{ id:machine_end_gcode; objectName: "machine_end_gcode"
                fdmLabel: "G-codeの終了"
                fdmDescription: "Gコードのコマンドをファイルの末尾に追加します。\\nでコマンドを区切ります。"
            }
            FdmQml_Param{ id:material_guid; objectName: "material_guid"
                fdmLabel: "マテリアルGUID"
                fdmDescription: "マテリアルのGUID。これは自動的に設定されます。"
            }
            FdmQml_Param{ id:material_bed_temp_wait; objectName: "material_bed_temp_wait"
                fdmLabel: "ビルドプレート加熱待ち時間"
                fdmDescription: "開始時にビルドプレートが温度に達するまで待つコマンドを挿入するかどうか。"
            }
            FdmQml_Param{ id:material_print_temp_wait; objectName: "material_print_temp_wait"
                fdmLabel: "ノズル加熱待ち時間"
                fdmDescription: "開始時にノズルの温度が達するまで待つかどうか。"
            }
            FdmQml_Param{ id:material_print_temp_prepend; objectName: "material_print_temp_prepend"
                fdmLabel: "マテリアル温度を含む"
                fdmDescription: "GCodeの開始時にノズル温度設定を含めるかどうか。 既に最初のGCodeにノズル温度設定が含まれている場合、AnkerMakeフロントエンドは自動的にこの設定を無効にします。"
            }
            FdmQml_Param{ id:material_bed_temp_prepend; objectName: "material_bed_temp_prepend"
                fdmLabel: "ビルドプレート温度を含む"
                fdmDescription: "GCodeの開始時にビルドプレート温度設定を含めるかどうか。 既に最初のGCodeにビルドプレート温度設定が含まれている場合、AnkerMakeフロントエンドは自動的にこの設定を無効にします。"
            }
            FdmQml_Param{ id:machine_width; objectName: "machine_width"
                fdmLabel: "プリンターの幅"
                fdmDescription: "造形可能領域の幅（X方向）。"
            }
            FdmQml_Param{ id:machine_depth; objectName: "machine_depth"
                fdmLabel: "プリンターの奥行"
                fdmDescription: "造形可能領域の幅（Y方向）。"
            }
            FdmQml_Param{ id:machine_shape; objectName: "machine_shape"
                fdmLabel: "ビルドプレートの形状"
                fdmDescription: "造形不可領域を考慮しないビルドプレートの形状。"
                fdmOptions:{
                    "rectangular": "長方形"
                }
            }
            FdmQml_Param{ id:machine_buildplate_type; objectName: "machine_buildplate_type"
                fdmLabel: "ビルドプレートの材料"
                fdmDescription: "プリンターに取り付けられているビルドプレートの材料です。"
                fdmOptions:{
                    "glass": "ガラス",
                    "aluminum": "アルミニウム"
                }
            }
            FdmQml_Param{ id:machine_height; objectName: "machine_height"
                fdmLabel: "プリンターの高さ"
                fdmDescription: "造形可能領域の幅（Z方向）。"
            }
            FdmQml_Param{ id:machine_heated_bed; objectName: "machine_heated_bed"
                fdmLabel: "加熱式ビルドプレートあり"
                fdmDescription: "プリンターに加熱式ビルドプレートが付属しているかどうか。"
            }
            FdmQml_Param{ id:machine_heated_build_volume; objectName: "machine_heated_build_volume"
                fdmLabel: "造形温度安定化処理有り"
                fdmDescription: "機器が造形温度を安定化処理できるかどうかです。"
                FdmQml_Param{ id:build_volume_temperature; objectName: "build_volume_temperature"
                    fdmLabel: "造形温度"
                    fdmDescription: "プリントするプリンタ内の温度。これがゼロ (0) の場合、造形温度は調整できません。"
                }
            }
            FdmQml_Param{ id:machine_always_write_active_tool; objectName: "machine_always_write_active_tool"
                fdmLabel: "常にアクティブなツールを書き込む"
                fdmDescription: "非アクティブなツールに一時コマンドを送信した後にアクティブなツールを書き込みます。Smoothieまたはその他のモーダルツールコマンドを使用するファームウェアを使用したデュアルエクストルーダープリントに必要です。"
            }
            FdmQml_Param{ id:machine_center_is_zero; objectName: "machine_center_is_zero"
                fdmLabel: "中心位置"
                fdmDescription: "プリンタのゼロポジションのX / Y座標がプリント可能領域の中心にあるかどうか。"
            }
            FdmQml_Param{ id:machine_extruder_count; objectName: "machine_extruder_count"
                fdmLabel: "エクストルーダーの数"
                fdmDescription: "エクストルーダーの数。エクストルーダーの単位は、フィーダー、ボーデンチューブ、およびノズルを組合せたもの。"
                fdmOptions: machine_extruder_count.fdmMaximumValue <= 1 ? {1:1} : machine_extruder_count.fdmMaximumValue === 2 ? {1:1, 2:2} : {1:1, 2:2, 3:3}
            }
            FdmQml_Param{ id:extruders_enabled_count; objectName: "extruders_enabled_count"
                fdmLabel: "有効なエクストルーダーの数"
                fdmDescription: "有効なエクストルーダートレインの数（ソフトウェアが自動設定）"
            }
            FdmQml_Param{ id:machine_nozzle_tip_outer_diameter; objectName: "machine_nozzle_tip_outer_diameter"
                fdmLabel: "ノズル外径"
                fdmDescription: "ノズルの外径。"
            }
            FdmQml_Param{ id:machine_nozzle_head_distance; objectName: "machine_nozzle_head_distance"
                fdmLabel: "ノズル長さ"
                fdmDescription: "ノズル先端とプリントヘッドの最下部との高さの差。"
            }
            FdmQml_Param{ id:machine_nozzle_expansion_angle; objectName: "machine_nozzle_expansion_angle"
                fdmLabel: "ノズル角度"
                fdmDescription: "水平面とノズル直上の円錐部分との間の角度。"
            }
            FdmQml_Param{ id:machine_heat_zone_length; objectName: "machine_heat_zone_length"
                fdmLabel: "ノズル加熱長さ"
                fdmDescription: "ノズルからの熱がフィラメントに伝達される距離。"
            }
            FdmQml_Param{ id:machine_nozzle_temp_enabled; objectName: "machine_nozzle_temp_enabled"
                fdmLabel: "ノズルの温度管理を有効にする"
                fdmDescription: "AnkerMakeから温度を制御するかどうか。これをオフにして、AnkerMake外からノズル温度を制御することで無効化。"
            }
            FdmQml_Param{ id:machine_nozzle_heat_up_speed; objectName: "machine_nozzle_heat_up_speed"
                fdmLabel: "加熱速度"
                fdmDescription: "ノズルが加熱する速度（℃/ s）は、通常のプリント時温度とスタンバイ時温度にて平均化されています。"
            }
            FdmQml_Param{ id:machine_nozzle_cool_down_speed; objectName: "machine_nozzle_cool_down_speed"
                fdmLabel: "冷却速度"
                fdmDescription: "ノズルが冷却される速度（℃/ s）は、通常のプリント温度とスタンバイ温度のウィンドウにわたって平均化されています。"
            }
            FdmQml_Param{ id:machine_min_cool_heat_time_window; objectName: "machine_min_cool_heat_time_window"
                fdmLabel: "スタンバイ温度までの最短時間"
                fdmDescription: "ノズルが冷却される前にエクストルーダーが静止しなければならない最短時間。この時間より長時間エクストルーダーを使用しない場合にのみ、スタンバイ温度に冷却することができます。"
            }
            FdmQml_Param{ id:machine_gcode_flavor; objectName: "machine_gcode_flavor"
                fdmLabel: "G-codeフレーバー"
                fdmDescription: "生成するG-codeの種類です。"
                fdmOptions:{
                    "RepRap (Marlin/Sprinter)": "Marlin",
                    "RepRap (RepRap)": "RepRap",
                    "Griffin": "Griffin",
                    "Makerbot": "Makerbot"
                }
            }
            FdmQml_Param{ id:machine_firmware_retract; objectName: "machine_firmware_retract"
                fdmLabel: "ファームウェア引き戻し"
                fdmDescription: "材料を引き戻すためにG1コマンドのEプロパティーを使用する代わりにファームウェア引き戻しコマンド (G10/G11) を使用するかどうか。"
            }
            FdmQml_Param{ id:machine_extruders_share_heater; objectName: "machine_extruders_share_heater"
                fdmLabel: "エクストルーダーのヒーター共有"
                fdmDescription: "各エクストルーダーが独自のヒーターを持つのではなく、単一のヒーターを共有するかどうか。"
            }
            FdmQml_Param{ id:machine_extruders_share_nozzle; objectName: "machine_extruders_share_nozzle"
                fdmLabel: "エクストルーダーの共有ノズル"
                fdmDescription: "各エクストルーダーが独自のノズルを持つのではなく、単一のノズルを共有するかどうか。初期設定した場合、プリンター起動gcodeスクリプトにより、すべてのエクストルーダーが初期の引き戻し状態が互換性のあるように設定されます（引き戻されていない状態のフィラメントが0個または1個のいずれか）。この場合、初期引き戻しステータスは「machine_extruders_shared_nozzle_initial_retraction」パラメーターによってエクストルーダーごとに規定されます。"
            }
            FdmQml_Param{ id:machine_extruders_shared_nozzle_initial_retraction; objectName: "machine_extruders_shared_nozzle_initial_retraction"
                fdmLabel: "共有ノズルの初期引き戻し"
                fdmDescription: "プリンタ起動gcodeスクリプト完了時に、各エクストルーダーのフィラメントが共有ノズルの先端部分から引き戻されていると想定される量。この値は、ノズルのダクトの共通部分の長さ以上にする必要があります。"
            }
            FdmQml_Param{ id:machine_disallowed_areas; objectName: "machine_disallowed_areas"
                fdmLabel: "拒否エリア"
                fdmDescription: "プリントヘッドの領域を持つポリゴンのリストは入力できません。"
            }
            FdmQml_Param{ id:machine_head_with_fans_polygon; objectName: "machine_head_with_fans_polygon"
                fdmLabel: "プリントヘッドとファンポリゴン"
                fdmDescription: "プリントヘッドの2Dシルエット（ファンキャップが含まれています）。"
            }
            FdmQml_Param{ id:gantry_height; objectName: "gantry_height"
                fdmLabel: "ガントリーの高さ"
                fdmDescription: "ノズルの先端とガントリーシステムの高さの差（X軸とY軸）。"
            }
            FdmQml_Param{ id:machine_use_extruder_offset_to_offset_coords; objectName: "machine_use_extruder_offset_to_offset_coords"
                fdmLabel: "エクストルーダーのオフセット"
                fdmDescription: "エクストルーダーのオフセットを座標システムに適用します。すべてのエクストルーダーが影響を受けます。"
            }
            FdmQml_Param{ id:extruder_prime_pos_abs; objectName: "extruder_prime_pos_abs"
                fdmLabel: "エクストルーダーの絶対位置"
                fdmDescription: "最後のヘッドの既知位置からではなく、エクストルーダー現在位置を絶対位置にします。"
            }
            FdmQml_Param{ id:machine_max_feedrate_x; objectName: "machine_max_feedrate_x"
                fdmLabel: "最大速度X"
                fdmDescription: "X方向のモーターの最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_y; objectName: "machine_max_feedrate_y"
                fdmLabel: "最大速度Y"
                fdmDescription: "Y方向のモーターの最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_z; objectName: "machine_max_feedrate_z"
                fdmLabel: "最大速度Z"
                fdmDescription: "Z方向のモーターの最大速度。"
            }
            FdmQml_Param{ id:machine_max_feedrate_e; objectName: "machine_max_feedrate_e"
                fdmLabel: "最大送り速度"
                fdmDescription: "フィラメントの最大速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_x; objectName: "machine_max_acceleration_x"
                fdmLabel: "最大加速度X"
                fdmDescription: "X方向のモーターの最大速度"
            }
            FdmQml_Param{ id:machine_max_acceleration_y; objectName: "machine_max_acceleration_y"
                fdmLabel: "最大加速度Y"
                fdmDescription: "Y方向のモーターの最大加速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_z; objectName: "machine_max_acceleration_z"
                fdmLabel: "最大加速度Z"
                fdmDescription: "Z方向のモーターの最大加速度。"
            }
            FdmQml_Param{ id:machine_max_acceleration_e; objectName: "machine_max_acceleration_e"
                fdmLabel: "フィラメント最大加速度"
                fdmDescription: "フィラメントのモーターの最大加速度。"
            }
            FdmQml_Param{ id:machine_acceleration; objectName: "machine_acceleration"
                fdmLabel: "デフォルト加速度"
                fdmDescription: "プリントヘッド移動のデフォルトの加速度。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_x; objectName: "machine_steps_per_mm_x"
                fdmLabel: "ミリメートルあたりのステップ (X)"
                fdmDescription: "X 方向に 1 ミリメートルの移動でステップモーターが行うステップの数を示します。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_y; objectName: "machine_steps_per_mm_y"
                fdmLabel: "ミリメートルあたりのステップ (Y)"
                fdmDescription: "Y 方向に 1 ミリメートルの移動でステップモーターが行うステップの数を示します。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_z; objectName: "machine_steps_per_mm_z"
                fdmLabel: "ミリメートルあたりのステップ (Z)"
                fdmDescription: "Z 方向に 1 ミリメートルの移動でステップモーターが行うステップの数を示します。"
            }
            FdmQml_Param{ id:machine_steps_per_mm_e; objectName: "machine_steps_per_mm_e"
                fdmLabel: "ミリメートルあたりのステップ (E)"
                fdmDescription: "フィーダーホイールを円周上で1ミリメートル移動させるのに、ステップモーターが行うステップの数を示します。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_x; objectName: "machine_endstop_positive_direction_x"
                fdmLabel: "プラス方向の X エンドストップ"
                fdmDescription: "X 軸のエンドストップがプラス方向（高い X 座標）またはマイナス方向（低い X 座標）のいずれかを示します。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_y; objectName: "machine_endstop_positive_direction_y"
                fdmLabel: "プラス方向の Y エンドストップ"
                fdmDescription: "Y 軸のエンドストップがプラス方向（高い Y 座標）またはマイナス方向（低い Y 座標）のいずれかを示します。"
            }
            FdmQml_Param{ id:machine_endstop_positive_direction_z; objectName: "machine_endstop_positive_direction_z"
                fdmLabel: "プラス方向の Z エンドストップ"
                fdmDescription: "Z 軸のエンドストップがプラス方向（高い Z 座標）またはマイナス方向（低い Z 座標）のいずれかを示します。"
            }
            FdmQml_Param{ id:machine_minimum_feedrate; objectName: "machine_minimum_feedrate"
                fdmLabel: "最小送り速度"
                fdmDescription: "プリントヘッドの最小移動速度。"
            }
            FdmQml_Param{ id:machine_feeder_wheel_diameter; objectName: "machine_feeder_wheel_diameter"
                fdmLabel: "フィーダーホイール直径"
                fdmDescription: "材料をフィーダーに送るホイールの直径。"
            }
        }
        FdmQml_Category{ id:extruder_settings; objectName: "extruder_settings"
            fdmLabel: "エクストルーダー"
            fdmDescription: "エクストルーダー設定"
            FdmQml_Param{ id:extruder_nr; objectName: "extruder_nr"
                fdmLabel: "エクストルーダー"
                fdmDescription: "エクストルーダーの列。デュアルノズルプリント時に使用。"
            }
            FdmQml_Param{ id:machine_nozzle_id; objectName: "machine_nozzle_id"
                fdmLabel: "ノズルID"
                fdmDescription: "\"AA 0.4\"や\"BB 0.8\"などのノズルID。"
            }
            FdmQml_Param{ id:machine_nozzle_size; objectName: "machine_nozzle_size"
                fdmLabel: "ノズル内径"
                fdmDescription: "ノズルの内径。標準以外のノズルを使用する場合は、この設定を変更してください。"
            }
            FdmQml_Param{ id:machine_nozzle_offset_x; objectName: "machine_nozzle_offset_x"
                fdmLabel: "Xノズルオフセット"
                fdmDescription: "ノズルのX軸のオフセット。"
            }
            FdmQml_Param{ id:machine_nozzle_offset_y; objectName: "machine_nozzle_offset_y"
                fdmLabel: "Yノズルオフセット"
                fdmDescription: "ノズルのY軸のオフセット。"
            }
            FdmQml_Param{ id:machine_extruder_start_code; objectName: "machine_extruder_start_code"
                fdmLabel: "エクストルーダーがG-Codeを開始する"
                fdmDescription: "このエクストルーダーに切り替えた時に G-Code の開始を実行します。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_abs; objectName: "machine_extruder_start_pos_abs"
                fdmLabel: "エクストルーダーのスタート位置の絶対値"
                fdmDescription: "ヘッドの最後の既知位置からではなく、エクストルーダーのスタート位置を絶対位置にします。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_x; objectName: "machine_extruder_start_pos_x"
                fdmLabel: "エクストルーダー スタート位置X"
                fdmDescription: "エクストルーダーのX座標のスタート位置。"
            }
            FdmQml_Param{ id:machine_extruder_start_pos_y; objectName: "machine_extruder_start_pos_y"
                fdmLabel: "エクストルーダースタート位置Y"
                fdmDescription: "エクストルーダーのY座標のスタート位置。"
            }
            FdmQml_Param{ id:machine_extruder_end_code; objectName: "machine_extruder_end_code"
                fdmLabel: "エクストルーダーがG-Codeを終了する"
                fdmDescription: "このエクストルーダーから切り替えた時に G-Code の終了を実行します。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_abs; objectName: "machine_extruder_end_pos_abs"
                fdmLabel: "エクストルーダーのエンドポジションの絶対値"
                fdmDescription: "ヘッドの既存の認識位置よりもエクストルーダーの最終位置を絶対位置とする。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_x; objectName: "machine_extruder_end_pos_x"
                fdmLabel: "エクストルーダーエンド位置X"
                fdmDescription: "エクストルーダーを切った際のX座標の最終位置。"
            }
            FdmQml_Param{ id:machine_extruder_end_pos_y; objectName: "machine_extruder_end_pos_y"
                fdmLabel: "エクストルーダーエンド位置Y"
                fdmDescription: "エクストルーダーを切った際のY座標の最終位置。"
            }
            FdmQml_Param{ id:extruder_prime_pos_z; objectName: "extruder_prime_pos_z"
                fdmLabel: "エクストルーダーのZ座標"
                fdmDescription: "プリント開始時にノズルがポジションを確認するZ座標。"
            }
            FdmQml_Param{ id:machine_extruder_cooling_fan_number; objectName: "machine_extruder_cooling_fan_number"
                fdmLabel: "エクストルーダープリント冷却ファン"
                fdmDescription: "このエクストルーダーに関連付けられているプリント冷却ファンの数です。デフォルト値は0(ゼロ)です。各エクストルーダーに対してプリント冷却ファンが異なる場合にのみ変更します。"
            }
            FdmQml_Param{ id:nozzle_disallowed_areas; objectName: "nozzle_disallowed_areas"
                fdmLabel: "ノズル拒否エリア"
                fdmDescription: "ノズルが入ることができない領域を持つポリゴンのリスト。"
            }
        }
        FdmQml_Category{ id:resolution; objectName: "resolution"
            fdmLabel: "品質"
            fdmDescription: "プリントの解像度に影響を与えるすべての設定。これらの設定は、品質（およびプリント時間）に大きな影響を与えます。"
            FdmQml_Param{ id:layer_height; objectName: "layer_height"
                fdmLabel: "レイヤー高さ"
                fdmDescription: "各レイヤーの高さ（mm）。値を大きくすると早くプリントしますが荒くなり、小さくするとプリントが遅くなりますが造形が綺麗になります。"
            }
            FdmQml_Param{ id:layer_height_0; objectName: "layer_height_0"
                fdmLabel: "初期レイヤー高さ"
                fdmDescription: "初期レイヤーの高さ（mm）。厚い初期層はビルドプレートへの接着を容易にする。"
            }
            FdmQml_Param{ id:line_width; objectName: "line_width"
                fdmLabel: "ライン幅"
                fdmDescription: "1ラインの幅。一般に、各ラインの幅は、ノズルの幅に対応する必要があります。ただし、この値を少し小さくすると、より良い造形が得られます。"
                FdmQml_Param{ id:wall_line_width; objectName: "wall_line_width"
                    fdmLabel: "ウォールライン幅"
                    fdmDescription: "ウォールラインの幅。"
                    FdmQml_Param{ id:wall_line_width_0; objectName: "wall_line_width_0"
                        fdmLabel: "外側ウォールライン幅"
                        fdmDescription: "最も外側のウォールラインの幅。この値を下げると、より詳細なプリントできます。"
                    }
                    FdmQml_Param{ id:thin_wall_line_width_0; objectName: "thin_wall_line_width_0"
                        fdmLabel: "細い外壁線幅"
                        fdmDescription: "薄い壁の線。薄すぎて 1 つの壁を生成できない壁は、このパラメーターを使用して薄い壁を印刷できます。"
                    }
                    FdmQml_Param{ id:wall_line_width_x; objectName: "wall_line_width_x"
                        fdmLabel: "内側ウォールライン幅"
                        fdmDescription: "一番外側のウォールラインを除くすべてのウォールラインのラインの幅。"
                    }
                }
                FdmQml_Param{ id:skin_line_width; objectName: "skin_line_width"
                    fdmLabel: "上下面ライン幅"
                    fdmDescription: "上辺/底辺線のライン幅。"
                }
                FdmQml_Param{ id:infill_line_width; objectName: "infill_line_width"
                    fdmLabel: "インフィルラインの幅"
                    fdmDescription: "インフィル線の幅。"
                }
                FdmQml_Param{ id:skirt_brim_line_width; objectName: "skirt_brim_line_width"
                    fdmLabel: "スカート/ブリムラインの幅"
                    fdmDescription: "単一のスカートまたはブリムラインの幅。"
                }
                FdmQml_Param{ id:support_line_width; objectName: "support_line_width"
                    fdmLabel: "サポートライン幅"
                    fdmDescription: "単一のサポート構造のライン幅。"
                }
                FdmQml_Param{ id:support_interface_line_width; objectName: "support_interface_line_width"
                    fdmLabel: "サポート面のライン幅"
                    fdmDescription: "サポートのルーフ、フロアのライン幅。"
                    FdmQml_Param{ id:support_roof_line_width; objectName: "support_roof_line_width"
                        fdmLabel: "サポートルーフのライン幅"
                        fdmDescription: "サポートルーフのライン一幅。"
                    }
                    FdmQml_Param{ id:support_bottom_line_width; objectName: "support_bottom_line_width"
                        fdmLabel: "サポートフロアのライン幅"
                        fdmDescription: "サポートのフロアのラインの一幅。"
                    }
                }
                FdmQml_Param{ id:prime_tower_line_width; objectName: "prime_tower_line_width"
                    fdmLabel: "プライムタワーのライン幅"
                    fdmDescription: "単一のプライムタワーラインの幅。"
                }
            }
            FdmQml_Param{ id:initial_layer_line_width_factor; objectName: "initial_layer_line_width_factor"
                fdmLabel: "初期レイヤーのライン幅"
                fdmDescription: "最初のレイヤーに線幅の乗数です。この値を増やすと、ベッドの接着性が向上します。"
            }
        }
        FdmQml_Category{ id:shell; objectName: "shell"
            fdmLabel: "ウォール"
            fdmDescription: "外郭"
            FdmQml_Param{ id:wall_extruder_nr; objectName: "wall_extruder_nr"
                fdmLabel: "ウォールエクストルーダー"
                fdmDescription: "壁造形用のエクストルーダー。デュアルノズルプリント時に使用。"
                FdmQml_Param{ id:wall_0_extruder_nr; objectName: "wall_0_extruder_nr"
                    fdmLabel: "外側ウォール用エクストルーダー"
                    fdmDescription: "外側ウォールプリント用のエクストルーダー。デュアルノズルプリント時に使用。"
                }
                FdmQml_Param{ id:wall_x_extruder_nr; objectName: "wall_x_extruder_nr"
                    fdmLabel: "内側ウォール用エクストルーダー"
                    fdmDescription: "内側ウォールプリント用のエクストルーダー。デュアルノズルプリント時に使用。"
                }
            }
            FdmQml_Param{ id:wall_thickness; objectName: "wall_thickness"
                fdmLabel: "壁の厚さ"
                fdmDescription: "壁の厚さ。この値をラインの幅で割ることで壁の数が決まります。"
                FdmQml_Param{ id:wall_line_count; objectName: "wall_line_count"
                    fdmLabel: "ウォールライン数"
                    fdmDescription: "ウォールの数。厚さから計算された場合、この値は整数になります。"
                }
            }
            FdmQml_Param{ id:wall_0_wipe_dist; objectName: "wall_0_wipe_dist"
                fdmLabel: "外側ウォール移動距離"
                fdmDescription: "外側ウォールの後に挿入された移動の距離はZシームをよりよく隠します。"
            }
            FdmQml_Param{ id:wall_0_inset; objectName: "wall_0_inset"
                fdmLabel: "外側ウォールはめ込み"
                fdmDescription: "外側ウォールの経路にはめ込む。外側ウォールがノズルよりも小さく、内側ウォールの後に造形されている場合は、オフセットを使用して、ノズルの穴をモデルの外側ではなく内側ウォールと重なるようにします。"
            }
            FdmQml_Param{ id:optimize_wall_printing_order; objectName: "optimize_wall_printing_order"
                fdmLabel: "壁プリント順序の最適化"
                fdmDescription: "撤回と移動距離を減らすために、壁のプリント順序を最適化します。ほとんどの部品がこの設定を有効にしている方が良いプリント結果につながりますが、実際には時間がかかることがありますので、最適化の有無に関わらずプリント時間を比較してください。ビルドプレートの接着タイプにブリムを選択すると最初のレイヤーは最適化されません。"
            }
            FdmQml_Param{ id:outer_inset_first; objectName: "outer_inset_first"
                fdmLabel: "外側ウォール優先"
                fdmDescription: "有効にすると、壁は外側から内側に順番にプリントします。これは、ABSのような高粘度のプラスチックを使用する際、XとYの寸法精度を改善するのに役立ちます。しかし、特にオーバーハング時に、外面のプリント品質を低下させる可能性があります。"
            }
            FdmQml_Param{ id:alternate_extra_perimeter; objectName: "alternate_extra_perimeter"
                fdmLabel: "代替予備壁"
                fdmDescription: "すべてのレイヤーごとに予備の壁をプリントします。このようにして、インフィルは余分な壁の間に挟まれ、より強いプリント物が得られる。"
            }
            FdmQml_Param{ id:travel_compensate_overlapping_walls_enabled; objectName: "travel_compensate_overlapping_walls_enabled"
                fdmLabel: "壁補正"
                fdmDescription: "すでに壁が設置されている部品の壁の流れを補正します。"
                FdmQml_Param{ id:travel_compensate_overlapping_walls_0_enabled; objectName: "travel_compensate_overlapping_walls_0_enabled"
                    fdmLabel: "外側ウォール補正"
                    fdmDescription: "すでに壁が設置されている場所にプリントされている外側ウォールの部分の流れを補う。"
                }
                FdmQml_Param{ id:travel_compensate_overlapping_walls_x_enabled; objectName: "travel_compensate_overlapping_walls_x_enabled"
                    fdmLabel: "内側ウォール補正"
                    fdmDescription: "すでに壁が設置されている場所にプリントされている内側ウォールの部分の流れを補正します。"
                }
            }
            FdmQml_Param{ id:wall_min_flow; objectName: "wall_min_flow"
                fdmLabel: "最小壁フロー"
                fdmDescription: "ウォールラインに対する流れを最小割合にします。既存の壁に近い場合に、壁補正により壁の流れが減少します。壁の流れがこの値より低い場合は、移動に置き換えられます。この設定を使用する場合は、壁補正を有効にして、内装の前に外装をプリントする必要があります。"
            }
            FdmQml_Param{ id:wall_min_flow_retract; objectName: "wall_min_flow_retract"
                fdmLabel: "引き戻し優先"
                fdmDescription: "有効にすると、フローが最小フローしきい値を下回っている壁を置き換える移動量より多い場合は、引き戻しを使用します。"
            }
            FdmQml_Param{ id:fill_perimeter_gaps; objectName: "fill_perimeter_gaps"
                fdmLabel: "隙間充填"
                fdmDescription: "壁が入らない壁の隙間を埋める。"
                fdmOptions:{
                    "nowhere": "対象外",
                    "everywhere": "全対象"
                }
            }
            FdmQml_Param{ id:filter_out_tiny_gaps; objectName: "filter_out_tiny_gaps"
                fdmLabel: "小さなギャップのフィルターアウト"
                fdmDescription: "モデル外部の塊を減らすために小さなギャップをフィルターアウトします。"
            }
            FdmQml_Param{ id:fill_outline_gaps; objectName: "fill_outline_gaps"
                fdmLabel: "薄壁プリント"
                fdmDescription: "ノズルサイズよりも細い壁を作ります。"
            }
            FdmQml_Param{ id:xy_offset; objectName: "xy_offset"
                fdmLabel: "水平展開"
                fdmDescription: "各レイヤーのすべてのポリゴンに適用されるオフセットの量。正の値は大きすぎる穴を補うことができます。負の値は小さすぎる穴を補うことができます。"
            }
            FdmQml_Param{ id:xy_offset_layer_0; objectName: "xy_offset_layer_0"
                fdmLabel: "初期層水平展開"
                fdmDescription: "最初のレイヤーのポリゴンに適用されるオフセットの値。マイナスの値はelephant's footと呼ばれる第一層が潰れるを現象を軽減させます。"
            }
            FdmQml_Param{ id:hole_xy_offset; objectName: "hole_xy_offset"
                fdmLabel: "穴の水平展開"
                fdmDescription: "各穴のすべてのポリゴンに適用されるオフセットの量。正の値は穴のサイズを大きくします。負の値は穴のサイズを小さくします。"
            }
            FdmQml_Param{ id:z_seam_type; objectName: "z_seam_type"
                fdmLabel: "Zシーム合わせ"
                fdmDescription: "レイヤーの経路始点。連続するレイヤー経路が同じポイントで開始すると、縦のシームがプリントに表示されることがあります。ユーザーが指定した場所の近くでこれらを整列させる場合、継ぎ目は最も簡単に取り除くことができます。無作為に配置すると、経路開始時の粗さが目立たなくなります。最短経路をとると、プリントが速くなります。"
                fdmOptions:{
                    "back": "ユーザー指定",
                    "shortest": "最短",
                    "random": "ランダム",
                    "sharpest_corner": "鋭い角"
                }
            }
            FdmQml_Param{ id:z_seam_position; objectName: "z_seam_position"
                fdmLabel: "Zシーム位置"
                fdmDescription: "レイヤー内の各パーツのプリントを開始する場所付近の位置。"
                fdmOptions:{
                    "backleft": "後方左",
                    "back": "戻る",
                    "backright": "後方右",
                    "right": "右",
                    "frontright": "前方右",
                    "front": "前",
                    "frontleft": "前左",
                    "left": "左"
                }
                FdmQml_Param{ id:z_seam_x; objectName: "z_seam_x"
                    fdmLabel: "ZシームX"
                    fdmDescription: "各レイヤーパーツがプリントを開始する位置のX座標。"
                }
                FdmQml_Param{ id:z_seam_y; objectName: "z_seam_y"
                    fdmLabel: "ZシームY"
                    fdmDescription: "各レイヤーパーツがプリントを開始する位置のY座標。"
                }
            }
            FdmQml_Param{ id:z_seam_corner; objectName: "z_seam_corner"
                fdmLabel: "シームコーナー設定"
                fdmDescription: "モデル輪郭の角がシームの位置に影響を及ぼすかどうかを制御します。[なし] は、角がシームの位置に影響を及ぼさないことを意味します。シームを隠すにすると、シームが内側の角に生じる可能性が高くなります。シームを外側にすると、シームが外側の角に生じる可能性が高くなります。シームを隠す／外側に出すは、シームが内側または外側の角に生じる可能性が高くなります。スマート・シームを使用すると、内外両側の角を使用できますが、適切な場合には内側の角が選択される頻度が高まります。"
                fdmOptions:{
                    "z_seam_corner_none": "なし",
                    "z_seam_corner_inner": "シーム非表示",
                    "z_seam_corner_outer": "シーム表示",
                    "z_seam_corner_any": "シーム表示/非表示",
                    "z_seam_corner_weighted": "スマート・シーム"
                }
            }
            FdmQml_Param{ id:z_seam_relative; objectName: "z_seam_relative"
                fdmLabel: "相対Zシーム"
                fdmDescription: "有効時は、Zシームは各パーツの真ん中に設定されます。無効時はプラットフォームのサイズによって設定されます。"
            }
            FdmQml_Param{ id:z_seam_min_angle_diff; objectName: "z_seam_min_angle_diff"
                fdmLabel: "Zジョイントの最大角度差"
                fdmDescription: "有効になった場合、最も先の尖った角度がこの値よりも小さい場合、最短の原則を選択します。"
            }
            FdmQml_Param{ id:z_seam_max_angle; objectName: "z_seam_max_angle"
                fdmLabel: "Zジョイントの最大の鋭いコーナー"
                fdmDescription: "この値よりも小さい場合、鋭いコーナーがあると見なされ、この値よりも大きい場合、最短の原則が有効になります。"
            }
        }
        FdmQml_Category{ id:top_bottom; objectName: "top_bottom"
            fdmLabel: "トップ/ボトム"
            fdmDescription: "トップ/ボトム"
            FdmQml_Param{ id:top_bottom_extruder_nr; objectName: "top_bottom_extruder_nr"
                fdmLabel: "上部/底面エクストルーダー"
                fdmDescription: "上部と下部の表面をプリントする時に使われるエクストルーダー。デュアルノズルプリント時に使用。"
            }
            FdmQml_Param{ id:top_bottom_thickness; objectName: "top_bottom_thickness"
                fdmLabel: "上部/底面の厚さ"
                fdmDescription: "プリント時の最上面、最底面の厚み。これを積層ピッチで割った値で最上面、最低面のレイヤーの数を定義します。"
                FdmQml_Param{ id:top_thickness; objectName: "top_thickness"
                    fdmLabel: "上部厚さ"
                    fdmDescription: "プリント時の最上面の厚み。これを積層ピッチで割った値で最上面のレイヤーの数を定義します。"
                    FdmQml_Param{ id:top_layers; objectName: "top_layers"
                        fdmLabel: "上部レイヤー"
                        fdmDescription: "最上面のレイヤー数。トップの厚さを計算する場合、この値は整数になります。"
                    }
                }
                FdmQml_Param{ id:bottom_thickness; objectName: "bottom_thickness"
                    fdmLabel: "底面厚さ"
                    fdmDescription: "プリント時の最底面の厚み。これを積層ピッチで割った値で最低面のレイヤーの数を定義します。"
                    FdmQml_Param{ id:bottom_layers; objectName: "bottom_layers"
                        fdmLabel: "底面レイヤー"
                        fdmDescription: "最底面のレイヤー数。下の厚さで計算すると、この値は整数に変換されます。"
                    }
                    FdmQml_Param{ id:initial_bottom_layers; objectName: "initial_bottom_layers"
                        fdmLabel: "初期底面レイヤー"
                        fdmDescription: "ビルドプレートから上にある初期底面レイヤーの数。下の厚さで計算すると、この値は整数に変換されます。"
                    }
                }
            }
            FdmQml_Param{ id:roofing_extruder_nr; objectName: "roofing_extruder_nr"
                fdmLabel: "上部表面用エクストルーダー"
                fdmDescription: "上部の表面プリント用のエクストルーダー。デュアルノズルプリント時に使用。"
            }
            FdmQml_Param{ id:roofing_layer_count; objectName: "roofing_layer_count"
                fdmLabel: "上部表面レイヤー"
                fdmDescription: "上部表面のレイヤー数。通常一層で綺麗に出来上がります。"
            }
            FdmQml_Param{ id:top_bottom_pattern; objectName: "top_bottom_pattern"
                fdmLabel: "上層/底層パターン"
                fdmDescription: "上層/底層のパターン。"
                fdmOptions:{
                    "lines": "直線",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ"
                }
            }
            FdmQml_Param{ id:top_bottom_pattern_0; objectName: "top_bottom_pattern_0"
                fdmLabel: "底層初期レイヤー"
                fdmDescription: "第1層のプリントの底部のパターン。"
                fdmOptions:{
                    "lines": "直線",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ"
                }
            }
            FdmQml_Param{ id:connect_skin_polygons; objectName: "connect_skin_polygons"
                fdmLabel: "上層/底層ポリゴンに接合"
                fdmDescription: "互いに次に実行する上層/底層スキンパスに接合します。同心円のパターンの場合、この設定を有効にすることにより、移動時間が短縮されますが、インフィルまでの途中で接合があるため、この機能で上層面の品質が損なわれることがあります。"
            }
            FdmQml_Param{ id:skin_monotonic; objectName: "skin_monotonic"
                fdmLabel: "上面/底面の方向一貫性"
                fdmDescription: "上面/底面のラインを、隣接するラインと常に一方向で重なるような順序でプリントします。これにより、プリントにかかる時間は少し長くなりますが、平らな面の見た目の一貫性が高まります。"
            }
            FdmQml_Param{ id:skin_angles; objectName: "skin_angles"
                fdmLabel: "上層/底層ラインの向き"
                fdmDescription: "上/下のレイヤーが線またはジグザグパターンを使う際の整数線の方向のリスト。リストの要素は、層が進行するにつれて順番に使用され、リストの終わりに達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは、従来のデフォルト角度（45度と135度）を使用する空のリストです。"
            }
            FdmQml_Param{ id:skin_no_small_gaps_heuristic; objectName: "skin_no_small_gaps_heuristic"
                fdmLabel: "Z 軸ギャップにスキンなし"
                fdmDescription: "モデルの垂直方向に少数層のみの小さなギャップがある場合、通常は、その狭いスペース内にある層の周囲にスキンが存在する必要があります。垂直方向のギャップが非常に小さい場合は、この設定を有効にしてスキンが生成されないようにします。これにより、プリント時間とスライス時間が向上しますが、技術的には空気にさらされたインフィルを残します。"
            }
            FdmQml_Param{ id:skin_outline_count; objectName: "skin_outline_count"
                fdmLabel: "外側表面の数"
                fdmDescription: "上部/下部パターンの最も外側の部分を同心円の線で置き換えます。 1つまたは2つの線を使用すると、トップ部分の造形が改善されます。"
            }
            FdmQml_Param{ id:top_skin_density; objectName: "top_skin_density"
                fdmLabel: "トップ スキン インフィル密度"
                fdmDescription: "トップ スキンのインフィル密度を調整します."
            }
            FdmQml_Param{ id:top_surface_one_wall; objectName: "top_surface_one_wall"
                fdmLabel: "上面に 1 つの壁を使用"
                fdmDescription: "上部の充填パターンにより多くのスペースを与えるために、平らな上面に 1 つの壁のみを使用してください"
            }
            FdmQml_Param{ id:ironing_enabled; objectName: "ironing_enabled"
                fdmLabel: "アイロン有効"
                fdmDescription: "微量の材料のみを吐出して、再度上部表面を動きます。これにより上部のプラスティックが溶かされ、よりスムースな表面になります。ノズルチャンバーには高い圧力が保たれるため、表面上のしわが材料で埋められます。"
            }
            FdmQml_Param{ id:ironing_only_highest_layer; objectName: "ironing_only_highest_layer"
                fdmLabel: "最上層のみアイロン"
                fdmDescription: "メッシュの最後のレイヤーでのみアイロンをかけます。下層にて滑らかな表面仕上げを必要としない場合、時間を節約します。"
            }
            FdmQml_Param{ id:ironing_pattern; objectName: "ironing_pattern"
                fdmLabel: "アイロンパターン"
                fdmDescription: "アイロンのパターン。"
                fdmOptions:{
                    "concentric": "同心円",
                    "zigzag": "ジグザグ"
                }
            }
            FdmQml_Param{ id:ironing_monotonic; objectName: "ironing_monotonic"
                fdmLabel: "アイロン方向一貫性"
                fdmDescription: "アイロンラインを、隣接するラインと常に一方向で重なるような順序でプリントします。これにより、プリントにかかる時間は少し長くなりますが、平らな面の見た目の一貫性が高まります。"
            }
            FdmQml_Param{ id:ironing_line_spacing; objectName: "ironing_line_spacing"
                fdmLabel: "アイロン線のスペース"
                fdmDescription: "アイロンライン同士の距離。"
            }
            FdmQml_Param{ id:ironing_flow; objectName: "ironing_flow"
                fdmLabel: "アイロンフロー"
                fdmDescription: "アイロン時にノズルから出しておくフィラメントの量。多少出しておくと裂け目を綺麗にします。ただ出し過ぎると吐出過多になり、端が荒れます。"
            }
            FdmQml_Param{ id:ironing_inset; objectName: "ironing_inset"
                fdmLabel: "アイロンインセット"
                fdmDescription: "モデルの端からの距離。端までアイロンをすると、端が荒れる場合があります。"
            }
            FdmQml_Param{ id:speed_ironing; objectName: "speed_ironing"
                fdmLabel: "アイロン速度"
                fdmDescription: "上部表面通過時の速度。"
            }
            FdmQml_Param{ id:acceleration_ironing; objectName: "acceleration_ironing"
                fdmLabel: "アイロン加速度"
                fdmDescription: "アイロン時の加速度。"
            }
            FdmQml_Param{ id:jerk_ironing; objectName: "jerk_ironing"
                fdmLabel: "アイロンジャーク"
                fdmDescription: "アイロン時の最大加速度。"
            }
            FdmQml_Param{ id:skin_overlap; objectName: "skin_overlap"
                fdmLabel: "表面公差量"
                fdmDescription: "壁とスキンの中央ライン（のエンドポイント）が交差する量（スキンラインのライン幅と壁の最内部に対する割合）を調整します。わずかな交差によって、壁がスキンにしっかりつながります。スキンと壁のライン幅が同じで、割合が50%を超えると、スキンが壁を通過している可能性があります。これは、その時点で、スキン押出機のノズルの位置が、すでに壁の真ん中を過ぎている可能性があるためです。"
                FdmQml_Param{ id:skin_overlap_mm; objectName: "skin_overlap_mm"
                    fdmLabel: "表面公差"
                    fdmDescription: "壁とスキンの中央ライン（のエンドポイント）が交差する量を調整します。わずかな交差によって、壁がスキンにしっかりつながります。スキンと壁のライン幅が同じで、壁の幅が半分以上の値になると、スキンが壁を通過している可能性があります。これは、その時点で、スキン押出機のノズルの位置が、すでに壁の真ん中を過ぎている可能性があるためです。"
                }
            }
            FdmQml_Param{ id:skin_preshrink; objectName: "skin_preshrink"
                fdmLabel: "表面除去幅"
                fdmDescription: "取り除くスキンエリアの最大幅。この値より小さいすべてのスキンエリアは消えます。これは、モデルの傾斜表面の上部/下部スキンに費やした時間のや材料の量を制限することができます。"
                FdmQml_Param{ id:top_skin_preshrink; objectName: "top_skin_preshrink"
                    fdmLabel: "上面除去幅"
                    fdmDescription: "取り除くスキンエリアの最大幅。この値より小さいすべてのスキンエリアは消えます。これは、モデルの傾斜表面の上部/下部スキンに費やした時間のや材料の量を制限することができます。"
                }
                FdmQml_Param{ id:bottom_skin_preshrink; objectName: "bottom_skin_preshrink"
                    fdmLabel: "底面除去幅"
                    fdmDescription: "取り除くスキンエリアの最大幅。この値より小さいすべてのスキンエリアは消えます。これは、モデルの傾斜表面の上部/下部スキンに費やした時間のや材料の量を制限することができます。"
                }
            }
            FdmQml_Param{ id:expand_skins_expand_distance; objectName: "expand_skins_expand_distance"
                fdmLabel: "表面展開距離"
                fdmDescription: "スキンがインフィルまで到達する距離です。高い数値の場合、スキンはインフィルのパターンに隣接しやすく、近接する壁のレイヤーもスキンに密着しやすくなります。低値の場合、材料の使用量を節約します。"
                FdmQml_Param{ id:top_skin_expand_distance; objectName: "top_skin_expand_distance"
                    fdmLabel: "上面展開距離"
                    fdmDescription: "スキンがインフィルまで到達する距離です。高い数値の場合、スキンはインフィルのパターンに隣接しやすく、近接する壁のレイヤーもスキンに密着しやすくなります。低値の場合、材料の使用量を節約します。"
                }
                FdmQml_Param{ id:bottom_skin_expand_distance; objectName: "bottom_skin_expand_distance"
                    fdmLabel: "底面展開距離"
                    fdmDescription: "スキンがインフィルまで到達する距離です。高い数値の場合、スキンはインフィルのパターンに隣接しやすく、近接する壁のレイヤーもスキンに密着しやすくなります。低値の場合、材料の使用量を節約します。"
                }
            }
            FdmQml_Param{ id:max_skin_angle_for_expansion; objectName: "max_skin_angle_for_expansion"
                fdmLabel: "表面展開最大角"
                fdmDescription: "この設定より大きい角を持つオブジェクトの上部または底部の表面は、その表面のスキンを拡大しません。これにより、モデルの表面に垂直に近い斜面がある場合に作成される狭いスキン領域の拡大を回避します。0°の角度は水平方向で、スキンは拡大しません。90°の角度は垂直方向で、すべてのスキンが拡大します。"
                FdmQml_Param{ id:min_skin_width_for_expansion; objectName: "min_skin_width_for_expansion"
                    fdmLabel: "表面展開最小角"
                    fdmDescription: "これより狭いスキン領域は展開されません。モデル表面に、垂直に近い斜面がある場合に作成される狭いスキン領域の拡大を回避するためです。"
                }
            }
        }
        FdmQml_Category{ id:infill; objectName: "infill"
            fdmLabel: "インフィル"
            fdmDescription: "インフィル"
            FdmQml_Param{ id:infill_extruder_nr; objectName: "infill_extruder_nr"
                fdmLabel: "インフィルエクストルーダー"
                fdmDescription: "インフィル造形時に使われるExtruder。デュアルノズルの場合に利用します。"
            }
            FdmQml_Param{ id:infill_sparse_density; objectName: "infill_sparse_density"
                fdmLabel: "インフィル密度"
                fdmDescription: "プリントのインフィルの密度を調整します。"
                FdmQml_Param{ id:infill_line_distance; objectName: "infill_line_distance"
                    fdmLabel: "インフィルライン距離"
                    fdmDescription: "造形されたインフィルラインの距離。この設定は、インフィル密度およびライン幅によって計算される。"
                }
            }
            FdmQml_Param{ id:infill_pattern; objectName: "infill_pattern"
                fdmLabel: "インフィルパターン"
                fdmDescription: "プリント用インフィル材料のパターン。代替層のラインとジグザグの面詰めスワップ方向、材料コストを削減します。グリッド、トライアングル、トライ六角、キュービック、オクテット、クォーターキュービック、クロスと同心円のパターンは、すべてのレイヤーを完全にプリントされます。ジャイロイド、キュービック、クォーターキュービック、オクテットのインフィルは、各レイヤーを変更して各方向の強度をより均等な分布にします。"
                fdmOptions:{
                    "grid": "グリッド",
                    "lines": "ライン",
                    "triangles": "トライアングル",
                    "trihexagon": "トライヘキサゴン",
                    "cubic": "キュービック",
                    "cubicsubdiv": "キュービックサブディビジョン",
                    "tetrahedral": "オクテット",
                    "quarter_cubic": "クォーターキュービック",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ",
                    "cross": "クロス",
                    "cross_3d": "3Dクロス",
                    "gyroid": "ジャイロイド"
                }
            }
            FdmQml_Param{ id:zig_zaggify_infill; objectName: "zig_zaggify_infill"
                fdmLabel: "インフィルライン接合"
                fdmDescription: "内側ウォールの形状に沿ったラインを使用してインフィルパターンと内側ウォールが合うところで接合します。この設定を有効にすると、インフィルが壁により密着するようになり、垂直面の品質に対するインフィルの影響が軽減します。この設定を無効にすると、材料の使用量が減ります。"
            }
            FdmQml_Param{ id:connect_infill_polygons; objectName: "connect_infill_polygons"
                fdmLabel: "インフィルポリゴン接合"
                fdmDescription: "互いに次に実行するインフィルパスに接合します。いくつかの閉じられたポリゴンを含むインフィルパターンの場合、この設定を有効にすることにより、移動時間が大幅に短縮されます。"
            }
            FdmQml_Param{ id:infill_angles; objectName: "infill_angles"
                fdmLabel: "インフィルラインの向き"
                fdmDescription: "使用する整数線の方向のリスト。リストの要素は、レイヤの層に合わせて順番に使用され、リストの末尾に達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは空のリストです。これは、従来のデフォルト角度（線とジグザグのパターンでは45と135度、他のすべてのパターンでは45度）を使用することを意味します。"
            }
            FdmQml_Param{ id:infill_offset_x; objectName: "infill_offset_x"
                fdmLabel: "インフィルXオフセット"
                fdmDescription: "インフィルパターンはX軸に沿ってこの距離を移動します。"
            }
            FdmQml_Param{ id:infill_offset_y; objectName: "infill_offset_y"
                fdmLabel: "インフィルYオフセット"
                fdmDescription: "インフィルパターンはY軸に沿ってこの距離を移動します。"
            }
            FdmQml_Param{ id:infill_randomize_start_location; objectName: "infill_randomize_start_location"
                fdmLabel: "インフィル開始のランダム化"
                fdmDescription: "どのインフィルラインが最初にプリントされるかをランダム化します。これによって1つのセグメントが強くなることを回避しますが、追加の移動距離が必要となります。"
            }
            FdmQml_Param{ id:infill_multiplier; objectName: "infill_multiplier"
                fdmLabel: "インフィルライン乗算"
                fdmDescription: "各インフィルラインをこの多重ラインに変換します。余分なラインが互いに交差せず、互いを避け合います。これによりインフィルが硬くなり、プリント時間と材料使用量が増えます。"
            }
            FdmQml_Param{ id:infill_wall_line_count; objectName: "infill_wall_line_count"
                fdmLabel: "外側インフィル壁の数"
                fdmDescription: "インフィルエリアの周囲にウォールを追加します。このウォールによって、トップ/ボトムスキンラインのたるみが少なくなり、同じ品質を保つために必要なマテリアルの消費を防ぐことができます。この機能が正しく設定されていれば、インフィルポリゴンの接合との組み合わせですべてのインフィルを1つのエクストルージョンパスに接続することができ、移動または後退の必要がなくなります。"
            }
            FdmQml_Param{ id:sub_div_rad_add; objectName: "sub_div_rad_add"
                fdmLabel: "キュービックサブディビジョンシェル"
                fdmDescription: "この立方体を細分するかどうかを決定するために、各立方体の中心から半径に加えてモデルの境界を調べます。値を大きくすると、モデルの境界付近に小さな立方体のより厚いシェルができます。"
            }
            FdmQml_Param{ id:infill_overlap; objectName: "infill_overlap"
                fdmLabel: "インフィル公差量"
                fdmDescription: "インフィルと壁のオーバーラップ量 (インフィルライン幅に対する%)。少しのオーバーラップによって壁がインフィルにしっかりつながります。"
                FdmQml_Param{ id:infill_overlap_mm; objectName: "infill_overlap_mm"
                    fdmLabel: "インフィル公差"
                    fdmDescription: "インフィルと壁が交差する量、わずかな交差によって壁がインフィルにしっかりつながります。"
                }
            }
            FdmQml_Param{ id:infill_wipe_dist; objectName: "infill_wipe_dist"
                fdmLabel: "インフィル移動距離"
                fdmDescription: "インフィルラインごとに挿入された移動距離は壁のインフィルへの接着をより良くします。このオプションは、インフィルオーバーラップに似ていますが、押出なく、インフィルラインの片側にのみあります。"
            }
            FdmQml_Param{ id:infill_sparse_thickness; objectName: "infill_sparse_thickness"
                fdmLabel: "インフィル層の厚さ"
                fdmDescription: "インフィルマテリアルの層ごとの厚さ。この値は常にレイヤーの高さの倍数でなければなりません。"
            }
            FdmQml_Param{ id:gradual_infill_steps; objectName: "gradual_infill_steps"
                fdmLabel: "インフィル半減回数"
                fdmDescription: "天井面の表面に近づく際にインフィル密度が半減する回数。天井面に近い領域ほど高い密度となり、インフィル密度まで達します。"
            }
            FdmQml_Param{ id:gradual_infill_step_height; objectName: "gradual_infill_step_height"
                fdmLabel: "インフィル半減高さ"
                fdmDescription: "密度が半分に切り替わる前の所定のインフィルの高さ。"
            }
            FdmQml_Param{ id:infill_before_walls; objectName: "infill_before_walls"
                fdmLabel: "インフィル優先"
                fdmDescription: "ウォールをプリントする前に、インフィルをプリントしてください。ウォールを先にプリントすると、ウォールの精度は高くなりますが、はみ出しのプリントが悪くなります。インフィルを先にプリントすると、より頑丈なウォールになりますが、インフィルのパターンが表面に現れることがあります。"
            }
            FdmQml_Param{ id:min_infill_area; objectName: "min_infill_area"
                fdmLabel: "最小インフィル領域"
                fdmDescription: "これより小さいインフィルの領域を生成しないでください (代わりにスキンを使用してください)。"
            }
            FdmQml_Param{ id:infill_support_enabled; objectName: "infill_support_enabled"
                fdmLabel: "インフィルサポート"
                fdmDescription: "面材構造をプリントするには、モデルの上部がサポートされている必要があります。これを有効にすると、プリント時間と材料の使用量が減少しますが、オブジェクトの強度が不均一になります。"
            }
            FdmQml_Param{ id:infill_support_angle; objectName: "infill_support_angle"
                fdmLabel: "インフィルオーバーハング角度"
                fdmDescription: "インフィルが追加される内部オーバーハングの最小角度。0° のとき、対象物は完全にインフィルが充填され、90° ではインフィルが提供されません。"
            }
            FdmQml_Param{ id:skin_edge_support_thickness; objectName: "skin_edge_support_thickness"
                fdmLabel: "スキンエッジサポートの厚さ"
                fdmDescription: "スキンエッジをサポートする追加のインフィルの厚さ。"
                FdmQml_Param{ id:skin_edge_support_layers; objectName: "skin_edge_support_layers"
                    fdmLabel: "スキンエッジサポートレイヤー"
                    fdmDescription: "スキンエッジをサポートするインフィルレイヤーの数。"
                }
            }
        }
        FdmQml_Category{ id:material; objectName: "material"
            fdmLabel: "マテリアル"
            fdmDescription: "マテリアル"
            FdmQml_Param{ id:material_diameter; objectName: "material_diameter"
                fdmLabel: "直径"
                fdmDescription: "使用するフィラメントの太さの調整 この値を使用するフィラメントの太さと一致させてください。"
            }
            FdmQml_Param{ id:material_density; objectName: "material_density"
                fdmLabel: "密度"
                fdmDescription: "使用するフィラメントの密度を調整。この値は、使用するフィラメントの密度に合わせます。"
                FdmQml_Param{ id:material_linear_density; objectName: "material_linear_density"
                    fdmLabel: "線密度"
                    fdmDescription: "使用するフィラメントの線密度を調整。この値は、使用するフィラメントの線密度に合わせます。"
                }
            }
            FdmQml_Param{ id:default_material_print_temperature; objectName: "default_material_print_temperature"
                fdmLabel: "デフォルトプリント温度"
                fdmDescription: "プリント中のデフォルトの温度。これはマテリアルの基本温度となります。他のすべての造形温度はこの値に基づいてオフセットする必要があります"
            }
            FdmQml_Param{ id:material_print_temperature; objectName: "material_print_temperature"
                fdmLabel: "プリント温度"
                fdmDescription: "プリント中の温度。"
                FdmQml_Param{ id:before_homing_temperature; objectName: "before_homing_temperature"
                    fdmLabel: "ホーミング前の温度"
                    fdmDescription: "ホーミングを実施する前のノズルの温度を設定"
                }
                FdmQml_Param{ id:material_print_temperature_layer_0; objectName: "material_print_temperature_layer_0"
                    fdmLabel: "初期レイヤープリント温度"
                    fdmDescription: "最初のレイヤーをプリントする温度。初期レイヤーのみ特別設定が必要ない場合は 0 に設定します。"
                }
                FdmQml_Param{ id:material_initial_print_temperature; objectName: "material_initial_print_temperature"
                    fdmLabel: "初期プリント温度"
                    fdmDescription: "加熱中、プリントを開始することができる最低の温度。"
                }
                FdmQml_Param{ id:material_final_print_temperature; objectName: "material_final_print_temperature"
                    fdmLabel: "最終プリント温度"
                    fdmDescription: "プリント終了直前に冷却を開始する温度。"
                }
                FdmQml_Param{ id:default_material_bed_temperature; objectName: "default_material_bed_temperature"
                    fdmLabel: "ビルドプレートのデフォルト温度"
                    fdmDescription: "加熱式ビルドプレートのデフォルト温度。これはビルドプレートの「基本」温度でます。他のすべてのプリント温度はこの値に基づいてオフセットする必要があります"
                }
                FdmQml_Param{ id:material_bed_temperature; objectName: "material_bed_temperature"
                    fdmLabel: "ビルドプレート温度"
                    fdmDescription: "加熱式ビルドプレートの温度。これが0の場合、ビルドプレートは加熱されないままになります。"
                }
                FdmQml_Param{ id:material_bed_temperature_layer_0; objectName: "material_bed_temperature_layer_0"
                    fdmLabel: "初期レイヤーのビルドプレート温度"
                    fdmDescription: "最初のレイヤープリント時の加熱式ビルドプレートの温度。これが0の場合、最初のレイヤープリント時のビルドプレートは加熱されないままになります。"
                }
            }
            FdmQml_Param{ id:material_extrusion_cool_down_speed; objectName: "material_extrusion_cool_down_speed"
                fdmLabel: "押出クールダウン速度修正"
                fdmDescription: "プリント中にノズルが冷える際の速度。同じ値が、加熱する際の加熱速度に割り当てられます。"
            }
            FdmQml_Param{ id:material_adhesion_tendency; objectName: "material_adhesion_tendency"
                fdmLabel: "接着傾向"
                fdmDescription: "表面の接着傾向。"
            }
            FdmQml_Param{ id:material_surface_energy; objectName: "material_surface_energy"
                fdmLabel: "表面エネルギー"
                fdmDescription: "表面エネルギー。"
            }
            FdmQml_Param{ id:material_shrinkage_percentage; objectName: "material_shrinkage_percentage"
                fdmLabel: "スケールファクタ収縮補正"
                fdmDescription: "材料の冷却時の収縮を補正するために、モデルはこのスケールファクタでスケールされます。"
            }
            FdmQml_Param{ id:material_crystallinity; objectName: "material_crystallinity"
                fdmLabel: "結晶性材料"
                fdmDescription: "この材料は加熱時にきれいに分解するタイプ (結晶性) または長く絡み合ったポリマー鎖 (非結晶) を作り出すタイプのいずれですか?"
            }
            FdmQml_Param{ id:material_anti_ooze_retracted_position; objectName: "material_anti_ooze_retracted_position"
                fdmLabel: "滲出防止引戻し位置"
                fdmDescription: "滲出を止めるには材料をどこまで引き戻す必要があるか。"
            }
            FdmQml_Param{ id:material_anti_ooze_retraction_speed; objectName: "material_anti_ooze_retraction_speed"
                fdmLabel: "滲出防止引戻し速度"
                fdmDescription: "滲出を防止するにはフィラメントスイッチ中に材料をどの程度速く引き戻す必要があるか。"
            }
            FdmQml_Param{ id:material_break_preparation_retracted_position; objectName: "material_break_preparation_retracted_position"
                fdmLabel: "フィラメントの引き出し準備引戻し位置"
                fdmDescription: "加熱中にフィラメントの引き出しが生じる距離。"
            }
            FdmQml_Param{ id:material_break_preparation_speed; objectName: "material_break_preparation_speed"
                fdmLabel: "フィラメント引き出し準備引戻し速度"
                fdmDescription: "フィラメントの引き出しが起こるための引き戻しの距離。"
            }
            FdmQml_Param{ id:material_break_preparation_temperature; objectName: "material_break_preparation_temperature"
                fdmLabel: "フィラメント引き出し準備温度"
                fdmDescription: "材料のパージに使用する温度は、許容最高プリンティング温度とほぼ等しくなければなりません。"
            }
            FdmQml_Param{ id:material_break_retracted_position; objectName: "material_break_retracted_position"
                fdmLabel: "フィラメント引き出しの引戻し位置"
                fdmDescription: "フィラメントをきれいに引き出すにはフィラメントをどこまで引き戻すか。"
            }
            FdmQml_Param{ id:material_break_speed; objectName: "material_break_speed"
                fdmLabel: "フィラメント引き出しの引戻し速度"
                fdmDescription: "フィラメントをきれいに引き出すために維持すべきフィラメントの引戻し速度。"
            }
            FdmQml_Param{ id:material_break_temperature; objectName: "material_break_temperature"
                fdmLabel: "フィラメント引き出し温度"
                fdmDescription: "フィラメントがきれいに引き出される温度。"
            }
            FdmQml_Param{ id:material_flush_purge_speed; objectName: "material_flush_purge_speed"
                fdmLabel: "フラッシュパージ速度"
                fdmDescription: "材料を切り替えた後に、材料の下準備をする速度。"
            }
            FdmQml_Param{ id:material_flush_purge_length; objectName: "material_flush_purge_length"
                fdmLabel: "フラッシュパージ長さ"
                fdmDescription: "材料を切り替えたときに、ノズルから前の材料をパージするために使用する材料の量（フィラメントの長さ）。"
            }
            FdmQml_Param{ id:material_end_of_filament_purge_speed; objectName: "material_end_of_filament_purge_speed"
                fdmLabel: "フィラメントパージ速度の後"
                fdmDescription: "空のスプールを同じ材料の新しいスプールに交換した後に、材料の下準備をする速度。"
            }
            FdmQml_Param{ id:material_end_of_filament_purge_length; objectName: "material_end_of_filament_purge_length"
                fdmLabel: "フィラメントパージ長さの後"
                fdmDescription: "空のスプールを同じ材料の新しいスプールに交換したときに、ノズルから前の材料をパージするために使用する材料の量（フィラメントの長さ）。"
            }
            FdmQml_Param{ id:material_maximum_park_duration; objectName: "material_maximum_park_duration"
                fdmLabel: "最大留め期間"
                fdmDescription: "材料を乾燥保管容器の外に置くことができる期間。"
            }
            FdmQml_Param{ id:material_no_load_move_factor; objectName: "material_no_load_move_factor"
                fdmLabel: "無負荷移動係数"
                fdmDescription: "フィーダーとノズルチャンバーの間でフィラメントが圧縮される量を示す係数。フィラメントスイッチの材料を移動する距離を決めるために使用されます。"
            }
            FdmQml_Param{ id:material_flow; objectName: "material_flow"
                fdmLabel: "フロー"
                fdmDescription: "流れの補修: 押出されるマテリアルの量は、この値から乗算されます。"
                FdmQml_Param{ id:wall_material_flow; objectName: "wall_material_flow"
                    fdmLabel: "壁のフロー"
                    fdmDescription: "壁のフロー補正。"
                    FdmQml_Param{ id:wall_0_material_flow; objectName: "wall_0_material_flow"
                        fdmLabel: "外側ウォールのフロー"
                        fdmDescription: "最外側ウォールのフロー補正。"
                    }
                    FdmQml_Param{ id:wall_x_material_flow; objectName: "wall_x_material_flow"
                        fdmLabel: "内側ウォールのフロー"
                        fdmDescription: "最外側ウォール以外の壁のフロー補正。"
                    }
                }
                FdmQml_Param{ id:skin_material_flow; objectName: "skin_material_flow"
                    fdmLabel: "上面/下面フロー"
                    fdmDescription: "上面/下面のフロー補正。"
                }
                FdmQml_Param{ id:roofing_material_flow; objectName: "roofing_material_flow"
                    fdmLabel: "上部表面スキンフロー"
                    fdmDescription: "プリント物の上部表面のフロー補正。"
                }
                FdmQml_Param{ id:infill_material_flow; objectName: "infill_material_flow"
                    fdmLabel: "インフィルフロー"
                    fdmDescription: "インフィルのフロー補正。"
                }
                FdmQml_Param{ id:skirt_brim_material_flow; objectName: "skirt_brim_material_flow"
                    fdmLabel: "スカート/ブリムのフロー"
                    fdmDescription: "スカートまたはブリムのフロー補正。"
                }
                FdmQml_Param{ id:support_material_flow; objectName: "support_material_flow"
                    fdmLabel: "支持材のフロー"
                    fdmDescription: "支持材のフロー補正。"
                }
                FdmQml_Param{ id:support_interface_material_flow; objectName: "support_interface_material_flow"
                    fdmLabel: "支持材界面フロー"
                    fdmDescription: "支持材の天井面または床面のフロー補正。"
                    FdmQml_Param{ id:support_roof_material_flow; objectName: "support_roof_material_flow"
                        fdmLabel: "支持材天井面フロー"
                        fdmDescription: "支持材天井面のフロー補正。"
                    }
                    FdmQml_Param{ id:support_bottom_material_flow; objectName: "support_bottom_material_flow"
                        fdmLabel: "支持材床面フロー"
                        fdmDescription: "支持材床面のフロー補正。"
                    }
                }
                FdmQml_Param{ id:prime_tower_flow; objectName: "prime_tower_flow"
                    fdmLabel: "プライムタワーのフロー"
                    fdmDescription: "プライムタワーのフロー補正。"
                }
            }
            FdmQml_Param{ id:material_flow_layer_0; objectName: "material_flow_layer_0"
                fdmLabel: "初期レイヤーフロー"
                fdmDescription: "初期レイヤーの流量補修:初期レイヤーの マテリアル吐出量はこの値の乗算で計算されます。"
            }
            FdmQml_Param{ id:material_standby_temperature; objectName: "material_standby_temperature"
                fdmLabel: "スタンバイ温度"
                fdmDescription: "プリントしていないノズルの温度（もう一方のノズルがプリント中）"
            }
        }
        FdmQml_Category{ id:speed; objectName: "speed"
            fdmLabel: "スピード"
            fdmDescription: "スピード"
            FdmQml_Param{ id:speed_travel; objectName: "speed_travel"
                fdmLabel: "移動速度"
                fdmDescription: "移動中のスピード。"
            }
            FdmQml_Param{ id:speed_print; objectName: "speed_print"
                fdmLabel: "プリント速度"
                fdmDescription: "プリントスピード"
                FdmQml_Param{ id:speed_infill; objectName: "speed_infill"
                    fdmLabel: "インフィル速度"
                    fdmDescription: "インフィルをプリントする速度。"
                }
                FdmQml_Param{ id:speed_wall; objectName: "speed_wall"
                    fdmLabel: "ウォール速度"
                    fdmDescription: "ウォールをプリントする速度。"
                    FdmQml_Param{ id:speed_wall_0; objectName: "speed_wall_0"
                        fdmLabel: "外側ウォール速度"
                        fdmDescription: "最も外側のウォールをプリントする速度。外側の壁を低速でプリントすると表面の質が改善しますが、内側ウォールと外側ウォールのプリント速度の差が大きすぎると、プリントの質が悪化します。"
                    }
                    FdmQml_Param{ id:speed_wall_x; objectName: "speed_wall_x"
                        fdmLabel: "内側ウォール速度"
                        fdmDescription: "内側のウォールをプリントする速度 外側ウォールより内側ウォールを高速でプリントすると、プリント時間の短縮になります。外側ウォールのプリント速度とインフィルのプリント速度の中間に設定するのが適切です。"
                    }
                }
                FdmQml_Param{ id:speed_topbottom; objectName: "speed_topbottom"
                    fdmLabel: "上面/底面速度"
                    fdmDescription: "トップ/ボトムのレイヤーのプリント速度。"
                    FdmQml_Param{ id:speed_roofing; objectName: "speed_roofing"
                        fdmLabel: "最上面速度"
                        fdmDescription: "上部表面プリント時の速度。"
                    }
                }
                FdmQml_Param{ id:speed_support; objectName: "speed_support"
                    fdmLabel: "サポート速度"
                    fdmDescription: "サポート材をプリントする速度です。高速でサポートをプリントすると、プリント時間を大幅に短縮できます。サポート材はプリント後に削除されますので、サポート構造の品質はそれほど重要ではありません。"
                    FdmQml_Param{ id:speed_support_infill; objectName: "speed_support_infill"
                        fdmLabel: "サポートインフィル速度"
                        fdmDescription: "サポート材のインフィルをプリントする速度 低速でプリントすると安定性が向上します。"
                    }
                    FdmQml_Param{ id:speed_support_interface; objectName: "speed_support_interface"
                        fdmLabel: "サポートインタフェース速度"
                        fdmDescription: "ルーフとフロアのサポート材をプリントする速度。低速でプリントするとオーバーハングの品質を向上できます。"
                        FdmQml_Param{ id:speed_support_roof; objectName: "speed_support_roof"
                            fdmLabel: "サポートルーフ速度"
                            fdmDescription: "ルーフとフロアのサポート材をプリントする速度 これらを低速でプリントするとオーバーハングの品質を向上できます。"
                        }
                        FdmQml_Param{ id:speed_support_bottom; objectName: "speed_support_bottom"
                            fdmLabel: "サポートフロア速度"
                            fdmDescription: "フロアのサポートがプリントされる速度。低速でプリントすることで、サポートの接着性を向上させることができます。"
                        }
                    }
                }
                FdmQml_Param{ id:speed_prime_tower; objectName: "speed_prime_tower"
                    fdmLabel: "プライムタワープリント速度"
                    fdmDescription: "プライムタワーをプリントする速度です。異なるフィラメントのプリントで密着性が最適ではない場合、低速にてプライム タワーをプリントすることでより安定させることができます。"
                }
            }
            FdmQml_Param{ id:speed_layer_0; objectName: "speed_layer_0"
                fdmLabel: "初期レイヤー速度"
                fdmDescription: "初期レイヤーでの速度。ビルドプレートへの接着を改善するため低速を推奨します。ブリムやラフトなどのビルドプレート接着構造自体には影響しません。"
                FdmQml_Param{ id:speed_print_layer_0; objectName: "speed_print_layer_0"
                    fdmLabel: "初期レイヤープリント速度"
                    fdmDescription: "一層目をプリントする速度 ビルトプレートへの接着を向上するため低速を推奨します。"
                }
                FdmQml_Param{ id:speed_travel_layer_0; objectName: "speed_travel_layer_0"
                    fdmLabel: "初期レイヤー移動速度"
                    fdmDescription: "最初のレイヤーをプリントする際のトラベルスピード。低速の方が、ビルドプレート剥がれるリスクを軽減することができます。この設定の値は、移動速度とプリント速度の比から自動的に計算されます。"
                }
                FdmQml_Param{ id:skirt_brim_speed; objectName: "skirt_brim_speed"
                    fdmLabel: "スカート/ブリム速度"
                    fdmDescription: "スカートとブリムのプリント速度 通常は一層目のスピードと同じですが、異なる速度でスカートやブリムをプリントしたい場合に設定してください。"
                }
            }
            FdmQml_Param{ id:speed_z_hop; objectName: "speed_z_hop"
                fdmLabel: "Z 軸ホップ速度"
                fdmDescription: "Z 軸ホップに対して垂直 Z 軸方向の動きが行われる速度。これは通常、ビルドプレートまたはマシンのガントリーが動きにくいため、プリント速度よりも低くなります。"
            }
            FdmQml_Param{ id:speed_slowdown_layers; objectName: "speed_slowdown_layers"
                fdmLabel: "遅いレイヤーの数"
                fdmDescription: "最初の数層はプリント失敗の可能性を軽減させるために、設定したプリントスピードよりも遅くプリントされます。"
            }
            FdmQml_Param{ id:speed_equalize_flow_enabled; objectName: "speed_equalize_flow_enabled"
                fdmLabel: "フィラメント均一速度"
                fdmDescription: "通常より細いラインを高速プリントするので、時間当たりのフィラメント押出量は同じです。薄いモデル部品は、設定よりも細い線幅でプリントすることが要求される場合があり、本設定はそのような線の速度を変更します。"
                FdmQml_Param{ id:speed_equalize_flow_max; objectName: "speed_equalize_flow_max"
                    fdmLabel: "均一フローの最大速度"
                    fdmDescription: "吐出を均一にするための調整時の最高スピード。"
                }
            }
            FdmQml_Param{ id:acceleration_enabled; objectName: "acceleration_enabled"
                fdmLabel: "加速度制御を有効にする"
                fdmDescription: "プリントヘッドのスピード調整の有効化 加速度の増加はプリント時間を短縮しますがプリントの質を損ねます。"
                FdmQml_Param{ id:acceleration_travel; objectName: "acceleration_travel"
                    fdmLabel: "移動加速度"
                    fdmDescription: "移動中の加速度。"
                }
                FdmQml_Param{ id:acceleration_print; objectName: "acceleration_print"
                    fdmLabel: "プリント加速度"
                    fdmDescription: "プリントの加速スピードです。"
                    FdmQml_Param{ id:acceleration_infill; objectName: "acceleration_infill"
                        fdmLabel: "インフィル加速度"
                        fdmDescription: "インフィルのプリントの加速スピード。"
                    }
                    FdmQml_Param{ id:acceleration_wall; objectName: "acceleration_wall"
                        fdmLabel: "ウォール加速度"
                        fdmDescription: "ウォールをプリントする際の加速度。"
                        FdmQml_Param{ id:acceleration_wall_0; objectName: "acceleration_wall_0"
                            fdmLabel: "外側ウォール加速度"
                            fdmDescription: "最も外側の壁をプリントする際の加速度。"
                        }
                        FdmQml_Param{ id:acceleration_wall_x; objectName: "acceleration_wall_x"
                            fdmLabel: "内側ウォール加速度"
                            fdmDescription: "内側のウォールがが出力される際のスピード。"
                        }
                        FdmQml_Param{ id:acceleration_topbottom; objectName: "acceleration_topbottom"
                            fdmLabel: "上面/底面加速度"
                            fdmDescription: "トップとボトムのレイヤーのプリント加速度。"
                            FdmQml_Param{ id:acceleration_roofing; objectName: "acceleration_roofing"
                                fdmLabel: "最上面加速度"
                                fdmDescription: "上部表面プリント時の加速度。"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_support; objectName: "acceleration_support"
                        fdmLabel: "サポート加速度"
                        fdmDescription: "サポート材プリント時の加速スピード。"
                        FdmQml_Param{ id:acceleration_support_infill; objectName: "acceleration_support_infill"
                            fdmLabel: "サポートインフィル加速度"
                            fdmDescription: "インフィルのサポート材のプリント時の加速度。"
                        }
                        FdmQml_Param{ id:acceleration_support_interface; objectName: "acceleration_support_interface"
                            fdmLabel: "サポートインタフェース加速度"
                            fdmDescription: "サポートの上面と下面がプリントされる加速度。低加速度でプリントするとオーバーハングの品質が向上します。"
                            FdmQml_Param{ id:acceleration_support_roof; objectName: "acceleration_support_roof"
                                fdmLabel: "サポートルーフ加速度"
                                fdmDescription: "サポートの上面がプリントされる加速度、低加速度でプリントするとオーバーハングの品質が向上します。"
                            }
                            FdmQml_Param{ id:acceleration_support_bottom; objectName: "acceleration_support_bottom"
                                fdmLabel: "サポートフロア加速度"
                                fdmDescription: "サポートのフロアがプリントされる加速度。より低い加速度でプリントすると、モデル上のサポートの接着性を向上させることができます。"
                            }
                        }
                    }
                    FdmQml_Param{ id:acceleration_prime_tower; objectName: "acceleration_prime_tower"
                        fdmLabel: "プライムタワー加速度"
                        fdmDescription: "プライムタワーのプリント時のスピード。"
                    }
                }
                FdmQml_Param{ id:acceleration_layer_0; objectName: "acceleration_layer_0"
                    fdmLabel: "初期レイヤー加速度"
                    fdmDescription: "初期レイヤーの加速度。"
                    FdmQml_Param{ id:acceleration_travel_layer_0; objectName: "acceleration_travel_layer_0"
                        fdmLabel: "初期レイヤー移動加速度"
                        fdmDescription: "最初のレイヤー時の加速度。"
                    }
                    FdmQml_Param{ id:acceleration_print_layer_0; objectName: "acceleration_print_layer_0"
                        fdmLabel: "初期レイヤープリント加速度"
                        fdmDescription: "初期レイヤーのプリント中の加速度。"
                    }
                    FdmQml_Param{ id:acceleration_skirt_brim; objectName: "acceleration_skirt_brim"
                        fdmLabel: "スカート/ブリム加速度"
                        fdmDescription: "スカートとブリムプリント時の加速度。通常、初期レイヤーのプリントスピードにて適用されるが、異なる速度でスカートやブリムをプリントしたい場合使用できる。"
                    }
                }
            }
            FdmQml_Param{ id:jerk_enabled; objectName: "jerk_enabled"
                fdmLabel: "ジャーク制御を有効にする"
                fdmDescription: "X または Y 軸の速度が変更する際、プリントヘッドのジャークを調整することができます。ジャークを増やすことは、プリント時間を短縮できますがプリントの質を損ねます。"
                FdmQml_Param{ id:jerk_travel; objectName: "jerk_travel"
                    fdmLabel: "移動ジャーク"
                    fdmDescription: "移動する際の最大瞬時速度の変更。"
                }
                FdmQml_Param{ id:jerk_print; objectName: "jerk_print"
                    fdmLabel: "プリントジャーク"
                    fdmDescription: "プリントヘッドの最大瞬間速度の変更。"
                    FdmQml_Param{ id:jerk_infill; objectName: "jerk_infill"
                        fdmLabel: "インフィルジャーク"
                        fdmDescription: "インフィルのプリント時の瞬間速度の変更。"
                    }
                    FdmQml_Param{ id:jerk_wall; objectName: "jerk_wall"
                        fdmLabel: "ウォールジャーク"
                        fdmDescription: "ウォールのプリント時の最大瞬間速度を変更。"
                        FdmQml_Param{ id:jerk_wall_0; objectName: "jerk_wall_0"
                            fdmLabel: "外側ウォールジャーク"
                            fdmDescription: "外側のウォールが出力される際の最大瞬間速度の変更。"
                        }
                        FdmQml_Param{ id:jerk_wall_x; objectName: "jerk_wall_x"
                            fdmLabel: "内側ウォールジャーク"
                            fdmDescription: "内側のウォールがプリントされれう際の最大瞬間速度の変更。"
                        }
                        FdmQml_Param{ id:jerk_topbottom; objectName: "jerk_topbottom"
                            fdmLabel: "上面/下面ジャーク"
                            fdmDescription: "トップとボトムのレイヤーをプリントする際の最大瞬間速度の変更。"
                            FdmQml_Param{ id:jerk_roofing; objectName: "jerk_roofing"
                                fdmLabel: "最上面ジャーク"
                                fdmDescription: "上部表面プリント時の最大加速度。"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_support; objectName: "jerk_support"
                        fdmLabel: "サポートジャーク"
                        fdmDescription: "サポート材のプリント時の最大瞬間速度の変更。"
                        FdmQml_Param{ id:jerk_support_infill; objectName: "jerk_support_infill"
                            fdmLabel: "サポートインフィルジャーク"
                            fdmDescription: "サポート材のプリント時、最大瞬間速度の変更。"
                        }
                        FdmQml_Param{ id:jerk_support_interface; objectName: "jerk_support_interface"
                            fdmLabel: "サポートインタフェースジャーク"
                            fdmDescription: "どのルーフとフロアのサポート部分をプリントするかによって最大瞬間速度は変化します。"
                            FdmQml_Param{ id:jerk_support_roof; objectName: "jerk_support_roof"
                                fdmLabel: "サポートルーフジャーク"
                                fdmDescription: "どのサポートのルーフ部分をプリントするかによって最大瞬間速度は変化します。"
                            }
                            FdmQml_Param{ id:jerk_support_bottom; objectName: "jerk_support_bottom"
                                fdmLabel: "サポートフロアジャーク"
                                fdmDescription: "どのサポートのフロア部分をプリントするかによって最大瞬間速度は変化します。"
                            }
                        }
                    }
                    FdmQml_Param{ id:jerk_prime_tower; objectName: "jerk_prime_tower"
                        fdmLabel: "プライムタワージャーク"
                        fdmDescription: "プライムタワーがプリントされる際の最大瞬間速度を変更します。"
                    }
                }
                FdmQml_Param{ id:jerk_layer_0; objectName: "jerk_layer_0"
                    fdmLabel: "初期レイヤージャーク"
                    fdmDescription: "初期レイヤーの最大瞬時速度の変更。"
                    FdmQml_Param{ id:jerk_travel_layer_0; objectName: "jerk_travel_layer_0"
                        fdmLabel: "初期レイヤー移動ジャーク"
                        fdmDescription: "移動加速度は最初のレイヤーに適用されます。"
                    }
                    FdmQml_Param{ id:jerk_print_layer_0; objectName: "jerk_print_layer_0"
                        fdmLabel: "初期レイヤープリントジャーク"
                        fdmDescription: "初期レイヤープリント中の最大瞬時速度の変化。"
                    }
                    FdmQml_Param{ id:jerk_skirt_brim; objectName: "jerk_skirt_brim"
                        fdmLabel: "スカート/ブリムジャーク"
                        fdmDescription: "スカートとブリムがプリントされる最大瞬時速度の変更。"
                    }
                }
                FdmQml_Param{ id:ak_VAJK_J_E_enabled; objectName: "ak_VAJK_J_E_enabled"
                    fdmLabel: "ジャーク"
                    fdmDescription: "ジャークは、瞬時に発生する可能性のある速度の最大変化(mm /秒)です。また、加速された(瞬間的ではない)動きとして行われる速度の最小変化と考えることもできます。"
                    FdmQml_Param{ id:ak_J_E_print; objectName: "ak_J_E_print"
                        fdmLabel: "ジャーク"
                        fdmDescription: "ジャーク"
                        FdmQml_Param{ id:ak_J_E_infill; objectName: "ak_J_E_infill"
                            fdmLabel: "インフィル"
                            fdmDescription: "インフィル　ジャーク"
                        }
                        FdmQml_Param{ id:ak_J_E_wall; objectName: "ak_J_E_wall"
                            fdmLabel: "ウォール"
                            fdmDescription: "ウォール　ジャーク"
                            FdmQml_Param{ id:ak_J_E_wall_0; objectName: "ak_J_E_wall_0"
                                fdmLabel: "外側ウォール"
                                fdmDescription: "外側ウォール　ジャーク"
                            }
                            FdmQml_Param{ id:ak_J_E_wall_x; objectName: "ak_J_E_wall_x"
                                fdmLabel: "内側ウォール"
                                fdmDescription: "内側ウォール　ジャーク"
                            }
                        }
                        FdmQml_Param{ id:ak_J_E_topbottom; objectName: "ak_J_E_topbottom"
                            fdmLabel: "スキン"
                            fdmDescription: "トップ＆ボトムスキン　ジャーク"
                        }
                        FdmQml_Param{ id:ak_J_E_support; objectName: "ak_J_E_support"
                            fdmLabel: "サポート"
                            fdmDescription: "サポート　ジャーク"
                        }
                        FdmQml_Param{ id:ak_J_E_skirt_brim; objectName: "ak_J_E_skirt_brim"
                            fdmLabel: "スカート/ブリム"
                            fdmDescription: "スカート/ブリム　ジャーク"
                        }
                        FdmQml_Param{ id:ak_J_E_layer_0; objectName: "ak_J_E_layer_0"
                            fdmLabel: "初期レイヤー"
                            fdmDescription: "初期レイヤー　ジャーク"
                        }
                    }
                }
            }
        }
        FdmQml_Category{ id:travel; objectName: "travel"
            fdmLabel: "移動"
            fdmDescription: "移動"
            FdmQml_Param{ id:retraction_enable; objectName: "retraction_enable"
                fdmLabel: "引き戻し有効"
                fdmDescription: "ノズルがプリントしないで良い領域を移動する際にフィラメントを引き戻す。"
            }
            FdmQml_Param{ id:retract_at_layer_change; objectName: "retract_at_layer_change"
                fdmLabel: "レイヤー変更時に引き戻す"
                fdmDescription: "ノズルは次の層に移動するときフィラメントを引き戻します。"
            }
            FdmQml_Param{ id:retraction_amount; objectName: "retraction_amount"
                fdmLabel: "引き戻し距離"
                fdmDescription: "引き戻されるマテリアルの長さ。"
            }
            FdmQml_Param{ id:retraction_speed; objectName: "retraction_speed"
                fdmLabel: "引き戻し速度"
                fdmDescription: "フィラメントが引き戻される時のスピード。"
                FdmQml_Param{ id:retraction_retract_speed; objectName: "retraction_retract_speed"
                    fdmLabel: "引き戻し速度の取り消し"
                    fdmDescription: "フィラメントが引き戻される時のスピード。"
                }
                FdmQml_Param{ id:retraction_prime_speed; objectName: "retraction_prime_speed"
                    fdmLabel: "押し戻し速度の取り消し"
                    fdmDescription: "フィラメントが引き戻される時のスピード。"
                }
            }
            FdmQml_Param{ id:retraction_extra_prime_amount; objectName: "retraction_extra_prime_amount"
                fdmLabel: "余分な押し戻し量の引き戻し"
                fdmDescription: "マテリアルによっては、移動中に滲み出てきてしまうときがあり、ここで調整することができます。"
            }
            FdmQml_Param{ id:retraction_prime_min_travel; objectName: "retraction_prime_min_travel"
                fdmLabel: "後退最小ストローク距離"
                fdmDescription: "移動距離がこの最小距離未満の場合、余分な量ではなく、移動中に一部の材料がにじみ出る可能性があります."
            }
            FdmQml_Param{ id:retraction_min_travel; objectName: "retraction_min_travel"
                fdmLabel: "引き戻し最小移動距離"
                fdmDescription: "フィラメントを引き戻す際に必要な最小移動距離。これは、短い距離内での引き戻しの回数を減らすために役立ちます。"
            }
            FdmQml_Param{ id:retraction_count_max; objectName: "retraction_count_max"
                fdmLabel: "最大引き戻し回数"
                fdmDescription: "この設定は、決められた距離の中で起こる引き戻しの回数を制限します。制限数以上の引き戻しは無視されます。これによりフィーダーでフィラメントを誤って削ってしまう問題を軽減させます。"
            }
            FdmQml_Param{ id:retraction_extrusion_window; objectName: "retraction_extrusion_window"
                fdmLabel: "最小抽出距離範囲"
                fdmDescription: "最大の引き戻し回数。この値は引き戻す距離と同じであることで、引き戻しが効果的に行われます。"
            }
            FdmQml_Param{ id:limit_support_retractions; objectName: "limit_support_retractions"
                fdmLabel: "サポート引き戻し限界"
                fdmDescription: "支持材から支持材に直線移動する場合は、引戻しを省略します。この設定を有効にすると、プリント時間は節約できますが、支持材内で過剰な糸引きが発生する可能性があります。"
            }
            FdmQml_Param{ id:retraction_combing; objectName: "retraction_combing"
                fdmLabel: "コーミングモード"
                fdmDescription: "コーミングは、走行時にすでにプリントされた領域内にノズルを保ちます。その結果、移動距離はわずかに長くなりますが、引き込みの必要性は減ります。コーミングがオフの場合、フィラメントの引き戻しを行い、ノズルは次のポイントまで直線移動します。また、インフィルのみにてコーミングすることにより、トップとボトムのスキン領域上での櫛通りを回避できます。"
                fdmOptions:{
                    "off": "オフ",
                    "all": "すべて",
                    "noskin": "スキン内にない",
                    "infill": "インフィル内"
                }
            }
            FdmQml_Param{ id:retraction_combing_max_distance; objectName: "retraction_combing_max_distance"
                fdmLabel: "引き戻しのない最大コム距離"
                fdmDescription: "ゼロを超える場合、この距離より長い移動量をコーミングすると、引き戻しが使用されます。ゼロに設定した場合、最大値はなく、コーミング移動では引き戻しを使用しません。"
            }
            FdmQml_Param{ id:travel_retract_before_outer_wall; objectName: "travel_retract_before_outer_wall"
                fdmLabel: "外側ウォールの前に引き戻す"
                fdmDescription: "移動して外側のウォールをプリントする際、毎回引き戻しをします。"
            }
            FdmQml_Param{ id:travel_avoid_other_parts; objectName: "travel_avoid_other_parts"
                fdmLabel: "移動はプリントしたパーツを回避する"
                fdmDescription: "ノズルは、移動時に既にプリントされたパーツを避けます。このオプションは、コーミングが有効な場合にのみ使用できます。"
            }
            FdmQml_Param{ id:travel_avoid_supports; objectName: "travel_avoid_supports"
                fdmLabel: "移動はサポートを回避する"
                fdmDescription: "ノズルは、移動時に既にプリントされたサポートを避けます。このオプションは、コーミングが有効な場合にのみ使用できます。"
            }
            FdmQml_Param{ id:travel_avoid_distance; objectName: "travel_avoid_distance"
                fdmLabel: "移動回避距離"
                fdmDescription: "ノズルが既にプリントされた部分を移動する際の間隔。"
            }
            FdmQml_Param{ id:layer_start_x; objectName: "layer_start_x"
                fdmLabel: "レイヤー始点X"
                fdmDescription: "各レイヤーのプリントを開始する部分をしめすX座標。"
            }
            FdmQml_Param{ id:layer_start_y; objectName: "layer_start_y"
                fdmLabel: "レイヤー始点Y"
                fdmDescription: "各レイヤーのプリントを開始する部分をしめすY座標。"
            }
            FdmQml_Param{ id:retraction_hop_enabled; objectName: "retraction_hop_enabled"
                fdmLabel: "引き戻し時のZホップ"
                fdmDescription: "引き戻しが完了すると、ビルドプレートが下降してノズルとプリントの間に隙間ができます。ノズルの走行中に造形物に当たるのを防ぎ、造形物をビルドプレートから剥がしてしまう現象を減らします。"
            }
            FdmQml_Param{ id:retraction_hop_only_when_collides; objectName: "retraction_hop_only_when_collides"
                fdmLabel: "プリントパーツに対するZホップ"
                fdmDescription: "走行時にプリント部品への衝突を避けるため、水平移動で回避できない造形物上を移動するときは、Zホップを実行します。"
            }
            FdmQml_Param{ id:retraction_hop; objectName: "retraction_hop"
                fdmLabel: "Zホップ高さ"
                fdmDescription: "Zホップを実行するときの高さ。"
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch; objectName: "retraction_hop_after_extruder_switch"
                fdmLabel: "エクストルーダースイッチ後のZホップ"
                fdmDescription: "マシーンが1つのエクストルーダーからもう一つのエクストルーダーに切り替えられた際、ビルドプレートが下降して、ノズルとプリント物との間に隙間が形成される。これによりノズルが造形物の外側にはみ出たマテリアルを残さないためである。"
            }
            FdmQml_Param{ id:retraction_hop_after_extruder_switch_height; objectName: "retraction_hop_after_extruder_switch_height"
                fdmLabel: "エクストルーダースイッチ高さ後のZホップ"
                fdmDescription: "エクストルーダースイッチ後のZホップを実行するときの高さの違い。"
            }
        }
        FdmQml_Category{ id:cooling; objectName: "cooling"
            fdmLabel: "冷却"
            fdmDescription: "冷却"
            FdmQml_Param{ id:cool_fan_enabled; objectName: "cool_fan_enabled"
                fdmLabel: "プリント中の冷却を有効にする"
                fdmDescription: "プリント中の冷却ファンを有効にします。ファンは、短いレイヤープリントやブリッジ/オーバーハングのレイヤーがあるプリント物の品質を向上させます。"
            }
            FdmQml_Param{ id:cool_fan_speed; objectName: "cool_fan_speed"
                fdmLabel: "ファン速度"
                fdmDescription: "冷却ファンが回転する速度。"
                FdmQml_Param{ id:cool_fan_speed_min; objectName: "cool_fan_speed_min"
                    fdmLabel: "標準ファン速度"
                    fdmDescription: "しきい値に達する前のファンの回転スピード。プリント速度がしきい値より速くなると、ファンの速度は上がっていきます。"
                }
                FdmQml_Param{ id:cool_fan_speed_max; objectName: "cool_fan_speed_max"
                    fdmLabel: "最大ファン速度"
                    fdmDescription: "最小積層時間でファンが回転する速度。しきい値に達すると、通常のファンの速度と最速の間でファン速度が徐々に加速しはじめます。"
                }
                FdmQml_Param{ id:cool_min_layer_time_fan_speed_max; objectName: "cool_min_layer_time_fan_speed_max"
                    fdmLabel: "標準/最大ファン速度のしきい値"
                    fdmDescription: "通常速度と最速の間でしきい値を設定する積層時間。この時間よりも遅くプリントする積層は、通常速度を使用します。より速い層の場合、ファンは最高速度に向かって徐々に加速します。"
                }
                FdmQml_Param{ id:cool_fan_speed_0; objectName: "cool_fan_speed_0"
                    fdmLabel: "初期ファン速度"
                    fdmDescription: "プリント開始時にファンが回転する速度。後続のレイヤーでは、ファン速度は、高さに応じて早くなります。"
                }
                FdmQml_Param{ id:cool_fan_full_at_height; objectName: "cool_fan_full_at_height"
                    fdmLabel: "標準ファン速度時の高さ"
                    fdmDescription: "通常速度でファンが回転するときの高さ。ここより下層レイヤーでは初期ファンのスピードから通常の速度まで徐々に増加します。"
                    FdmQml_Param{ id:cool_fan_full_layer; objectName: "cool_fan_full_layer"
                        fdmLabel: "標準ファン速度時のレイヤー"
                        fdmDescription: "ファンが通常の速度で回転する時のレイヤー。通常速度のファンの高さが設定されている場合、この値が計算され、整数に変換されます。"
                    }
                }
                FdmQml_Param{ id:cool_min_layer_time; objectName: "cool_min_layer_time"
                    fdmLabel: "最小レイヤー時間"
                    fdmDescription: "一つのレイヤーに最低限費やす時間。1つの層に必ず設定された時間を費やすため、場合によってはプリントに遅れが生じます。しかしこれにより、次の層をプリントする前に造形物を適切に冷却することができます。 Lift Headが無効になっていて、最小速度を下回った場合、最小レイヤー時間よりも短くなる場合があります。"
                    FdmQml_Param{ id:cool_min_speed_includes_outer_walls; objectName: "cool_min_speed_includes_outer_walls"
                        fdmLabel: "外側ウォールを含む"
                        fdmDescription: "最低速度でプリントする場合、外側ウォールを含む"
                    }
                }
                FdmQml_Param{ id:cool_min_speed; objectName: "cool_min_speed"
                    fdmLabel: "最低速度"
                    fdmDescription: "最遅プリント速度。プリントの速度が遅すぎると、ノズル内の圧力が低すぎてプリント品質が低下します。"
                }
                FdmQml_Param{ id:cool_lift_head; objectName: "cool_lift_head"
                    fdmLabel: "ヘッド持ち上げ"
                    fdmDescription: "レイヤーの最小プリント時間より早くプリントが終わった場合、ヘッド部分を持ち上げてレイヤーの最小プリント時間に到達するまで待機します。"
                }
            }
        }
        FdmQml_Category{ id:support; objectName: "support"
            fdmLabel: "サポート"
            fdmDescription: "サポート"
            FdmQml_Param{ id:support_enable; objectName: "support_enable"
                fdmLabel: "サポート開始"
                fdmDescription: "オーバーハングするモデルのサポートパーツの構造を形成します。これらのサポートがなければ、プリントは失敗します。"
            }
            FdmQml_Param{ id:support_extruder_nr; objectName: "support_extruder_nr"
                fdmLabel: "サポート用エクストルーダー"
                fdmDescription: "サポート材をプリントするためのエクストルーダー。複数のエクストルーダーがある場合に使用されます。"
                FdmQml_Param{ id:support_infill_extruder_nr; objectName: "support_infill_extruder_nr"
                    fdmLabel: "サポート用インフィルエクストルーダー"
                    fdmDescription: "サポート材のインフィルをプリントに使用するためのエクストルーダー。複数のエクストルーダーがある場合に使用されます。"
                }
                FdmQml_Param{ id:support_extruder_nr_layer_0; objectName: "support_extruder_nr_layer_0"
                    fdmLabel: "最初のレイヤー用サポートエクストルーダー"
                    fdmDescription: "サポートのインフィルの最初の層をプリントに使用するエクストルーダー。複数のエクストルーダーがある場合に使用されます。"
                }
                FdmQml_Param{ id:support_interface_extruder_nr; objectName: "support_interface_extruder_nr"
                    fdmLabel: "サポートインタフェースエクストルーダー"
                    fdmDescription: "サポートのルーフおよび底面をプリントするために使用するエクストルーダーの列。デュアルノズル時に使用されます。"
                    FdmQml_Param{ id:support_roof_extruder_nr; objectName: "support_roof_extruder_nr"
                        fdmLabel: "サポートルーフエクストルーダー"
                        fdmDescription: "サポートのルーフ面をプリントする際のエクストルーダー列。デュアルノズル時に使用します。"
                    }
                    FdmQml_Param{ id:support_bottom_extruder_nr; objectName: "support_bottom_extruder_nr"
                        fdmLabel: "サポートフロアエクストルーダー"
                        fdmDescription: "サポートのフロア面をプリントする際に使用するエクストルーダーの列。デュアルノズル時に使用します。"
                    }
                }
            }
            FdmQml_Param{ id:support_structure; objectName: "support_structure"
                fdmLabel: "サポート構造"
                fdmDescription: "サポートを生成するために利用できる技術を選択します。「標準」のサポート構造はオーバーハング部品のすぐ下に作成し、そのエリアを真下に生成します。「ツリー」サポートはオーバーハングエリアに向かって枝を作成し、モデルを枝の先端で支えます。枝をモデルのまわりにはわせて、できる限りビルドプレートから支えます。"
                fdmOptions:{
                    "normal": "標準",
                    "tree": "ツリー"
                }
            }
            FdmQml_Param{ id:support_tree_angle; objectName: "support_tree_angle"
                fdmLabel: "ツリーサポート枝角度"
                fdmDescription: "枝の角度。枝を垂直で安定したものにするためには小さい角度を使用します。高さを得るためには大きい角度を使用します。"
            }
            FdmQml_Param{ id:support_tree_branch_distance; objectName: "support_tree_branch_distance"
                fdmLabel: "ツリーサポート枝間隔"
                fdmDescription: "枝がモデルに接触するところで確保する枝の間隔。この間隔を小さくするとツリーサポートがモデルに接触する点が増え、支える効果が高まりますが、サポートの取り外しが難しくなります。"
            }
            FdmQml_Param{ id:support_tree_branch_diameter; objectName: "support_tree_branch_diameter"
                fdmLabel: "ツリーサポート枝直径"
                fdmDescription: "ツリーサポートの最も細い枝の直径。枝は太いほど丈夫です。基部に近いところでは、枝はこれよりも太くなります。"
            }
            FdmQml_Param{ id:support_tree_branch_diameter_angle; objectName: "support_tree_branch_diameter_angle"
                fdmLabel: "ツリーサポート枝直径角度"
                fdmDescription: "基部に向かって徐々に太くなる枝の直径の角度。角度が0の場合、枝の太さは全長にわたって同じになります。少し角度を付けると、ツリーサポートの安定性が高まります。"
            }
            FdmQml_Param{ id:support_tree_collision_resolution; objectName: "support_tree_collision_resolution"
                fdmLabel: "ツリーサポート衝突精細度"
                fdmDescription: "モデルに干渉しないようにする衝突計算の精細度。小さい値を設定すると、失敗の少ない正確なツリーが生成されますが、スライス時間は大きく増加します。"
            }
            FdmQml_Param{ id:support_type; objectName: "support_type"
                fdmLabel: "サポート配置"
                fdmDescription: "サポート構造物の配置を調整します。配置はビルドプレートに接触するように設定することもできますし、あらゆる場所に設置することもできます。あらゆる場所に設置する場合、サポート構造物はモデル上にも印刷されます。"
                fdmOptions:{
                    "buildplate": "ビルドプレートにタッチ",
                    "everywhere": "全対象"
                }
            }
            FdmQml_Param{ id:support_angle; objectName: "support_angle"
                fdmLabel: "サポートオーバーハング角度"
                fdmDescription: "サポート材がつくオーバーハングの最小角度。0° のときはすべてのオーバーハングにサポートが生成され、90° ではサポートが生成されません。"
            }
            FdmQml_Param{ id:support_pattern; objectName: "support_pattern"
                fdmLabel: "サポートパターン"
                fdmDescription: "サポート材の形。サポート材の除去の方法を頑丈または容易にする設定が可能です。"
                fdmOptions:{
                    "lines": "ライン",
                    "grid": "グリッド",
                    "triangles": "トライアングル",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ",
                    "cross": "クロス",
                    "gyroid": "ジャイロイド"
                }
            }
            FdmQml_Param{ id:support_wall_count; objectName: "support_wall_count"
                fdmLabel: "サポートウォールライン数"
                fdmDescription: "サポートインフィルを囲むウォールの数。ウォールを加えることにより、サポートのプリントの信頼性が高まり、オーバーハングを支えやすくなりますが、プリント時間が長くなり、使用する材料の量が増えます。"
            }
            FdmQml_Param{ id:zig_zaggify_support; objectName: "zig_zaggify_support"
                fdmLabel: "サポートライン接続"
                fdmDescription: "サポートライン両端を接続します。この設定を有効にすると、より確実なサポートで抽出不足を解消しますが、材料の費用がかさみます。"
            }
            FdmQml_Param{ id:support_connect_zigzags; objectName: "support_connect_zigzags"
                fdmLabel: "サポートジグザグ接続"
                fdmDescription: "ジグザグを接続します。ジグザグ形のサポート材の強度が上がります。"
            }
            FdmQml_Param{ id:support_infill_rate; objectName: "support_infill_rate"
                fdmLabel: "サポート密度"
                fdmDescription: "サポート材の密度を調整します。大きな値ではオーバーハングが良くなりますが、サポート材が除去しにくくなります。"
                FdmQml_Param{ id:support_line_distance; objectName: "support_line_distance"
                    fdmLabel: "サポートライン距離"
                    fdmDescription: "プリントされたサポート材の間隔。この設定は、サポート材の密度によって算出されます。"
                }
                FdmQml_Param{ id:support_initial_layer_line_distance; objectName: "support_initial_layer_line_distance"
                    fdmLabel: "初期層サポートラインの距離"
                    fdmDescription: "プリントした初期層間の距離が構造ライをサポートします。この設定は、対応濃度で算出されます。"
                }
            }
            FdmQml_Param{ id:support_infill_angles; objectName: "support_infill_angles"
                fdmLabel: "サポートインフィルラインの向き"
                fdmDescription: "使用する整数線の方向のリスト。リストの要素は、層が進行するにつれて順番に使用され、リストの終わりに達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは空のリストであり、デフォルト角度の0度を使用します。"
            }
            FdmQml_Param{ id:support_brim_enable; objectName: "support_brim_enable"
                fdmLabel: "サポートブリムを有効にする"
                fdmDescription: "最初の層のインフィルエリア内ブリムを生成します。このブリムは、サポートの周囲ではなく、サポートの下にプリントされます。この設定を有効にすると、サポートのビルドプレートへの吸着性が高まります。"
            }
            FdmQml_Param{ id:support_brim_width; objectName: "support_brim_width"
                fdmLabel: "サポートブリムの幅"
                fdmDescription: "サポートの下にプリントされるブリムの幅。ブリムが大きいほど、追加材料の費用でビルドプレートへの接着性が強化されます。"
                FdmQml_Param{ id:support_brim_line_count; objectName: "support_brim_line_count"
                    fdmLabel: "サポートブリムのライン数"
                    fdmDescription: "サポートブリムに使用される線の数。ブリムの線数を増やすと、追加材料の費用でビルドプレートへの接着性が強化されます。"
                }
            }
            FdmQml_Param{ id:support_z_distance; objectName: "support_z_distance"
                fdmLabel: "サポートZ距離"
                fdmDescription: "サポート材のトップ/ボトム部分とプリント物との距離。この幅がプリント後のサポート材を除去する隙間を作ります。値は積層ピッチの倍数にて計算されます。"
                FdmQml_Param{ id:support_top_distance; objectName: "support_top_distance"
                    fdmLabel: "サポート上部距離"
                    fdmDescription: "サポートの上部からプリント物までの距離。"
                }
                FdmQml_Param{ id:support_bottom_distance; objectName: "support_bottom_distance"
                    fdmLabel: "サポート底部距離"
                    fdmDescription: "プリント物とサポート材底部までの距離。"
                }
            }
            FdmQml_Param{ id:support_xy_distance; objectName: "support_xy_distance"
                fdmLabel: "サポートX/Y距離"
                fdmDescription: "プリント物からX/Y方向へのサポート材との距離。"
            }
            FdmQml_Param{ id:support_xy_overrides_z; objectName: "support_xy_overrides_z"
                fdmLabel: "サポート距離優先順位"
                fdmDescription: "X /Y方向のサポートの距離がZ方向のサポートの距離を上書きしようとする時やまたその逆も同様。X または Y がZを上書きする際、X Y 方向の距離はプリント物からオーバーハングする Z 方向の距離に影響を及ぼしながらサポートを押しのけようとします。オーバー ハング周りのX Yの距離を無効にすることで、無効にできる。"
                fdmOptions:{
                    "xy_overrides_z": "X/YがZを上書き",
                    "z_overrides_xy": "ZがX/Yを上書き"
                }
            }
            FdmQml_Param{ id:support_xy_distance_overhang; objectName: "support_xy_distance_overhang"
                fdmLabel: "最小サポートX/Y距離"
                fdmDescription: "X/Y方向におけるオーバーハングからサポートまでの距離。"
            }
            FdmQml_Param{ id:support_bottom_stair_step_height; objectName: "support_bottom_stair_step_height"
                fdmLabel: "サポート階段高さ"
                fdmDescription: "モデルにのっている階段状のサポートの底のステップの高さ。値を小さくするとサポートを除去するのが困難になりますが、値が大きすぎるとサポートの構造が不安定になる可能性があります。ゼロに設定すると、階段状の動作をオフにします。"
            }
            FdmQml_Param{ id:support_bottom_stair_step_width; objectName: "support_bottom_stair_step_width"
                fdmLabel: "サポート階段最大幅"
                fdmDescription: "モデルにのっている階段のような下部のサポートのステップの最大幅。低い値にするサポートの除去が困難になり、高すぎる値は不安定なサポート構造につながります。"
            }
            FdmQml_Param{ id:support_bottom_stair_step_min_slope; objectName: "support_bottom_stair_step_min_slope"
                fdmLabel: "サポートステアステップ最小傾斜角度"
                fdmDescription: "ステアステップ効果を発揮するための、エリアの最小スロープです。小さい値を指定すると勾配が緩くなりサポートを取り除きやすくなりますが、値が非常に小さいと、モデルの他の部品に直感的に非常にわかりにくい結果が表れる場合があります。"
            }
            FdmQml_Param{ id:support_join_distance; objectName: "support_join_distance"
                fdmLabel: "サポート接合距離"
                fdmDescription: "支持材間における X/Y 軸方向の最大距離。個別の支持材間の距離がこの値よりも近い場合、支持材は 1 つにマージされます。"
            }
            FdmQml_Param{ id:support_offset; objectName: "support_offset"
                fdmLabel: "サポート水平展開"
                fdmDescription: "各レイヤーのサポート用ポリゴンに適用されるオフセットの量。正の値はサポート領域を円滑にし、より丈夫なサポートにつながります。"
            }
            FdmQml_Param{ id:support_infill_sparse_thickness; objectName: "support_infill_sparse_thickness"
                fdmLabel: "サポートインフィルレイヤー厚さ"
                fdmDescription: "サポートのインフィルの厚さ。この値はレイヤーの倍数にする必要があり、違う場合は倍数に近い値に設定されます。"
            }
            FdmQml_Param{ id:gradual_support_infill_steps; objectName: "gradual_support_infill_steps"
                fdmLabel: "サポートインフィル半減回数"
                fdmDescription: "天井面より下に遠ざかる際にサポートのインフィル密度が半減する回数 天井面に近い領域ほど高い密度となり、サポートのインフィル密度になります。"
            }
            FdmQml_Param{ id:gradual_support_infill_step_height; objectName: "gradual_support_infill_step_height"
                fdmLabel: "サポートインフィル半減前の高さ"
                fdmDescription: "密度が半分に切り替える前の所定のサポートのインフィルの高さ。"
            }
            FdmQml_Param{ id:minimum_support_area; objectName: "minimum_support_area"
                fdmLabel: "最小サポート領域"
                fdmDescription: "ポリゴンをサポートする最小領域サイズ。この領域よりポリゴンが小さい場合は生成されません。"
            }
            FdmQml_Param{ id:support_interface_enable; objectName: "support_interface_enable"
                fdmLabel: "サポートインタフェースを有効にする"
                fdmDescription: "モデルとサポートの間に密なインターフェースを生成します。これにより、モデルがプリントされているサポートの上部、モデル上のサポートの下部にスキンが作成されます。"
                FdmQml_Param{ id:support_roof_enable; objectName: "support_roof_enable"
                    fdmLabel: "サポートルーフを有効にする"
                    fdmDescription: "サポートの上部とモデルの間に高密度の厚板を形成します。モデルとサポート材の間にスキンが作成されます。"
                }
                FdmQml_Param{ id:support_bottom_enable; objectName: "support_bottom_enable"
                    fdmLabel: "サポートフロアを有効にする"
                    fdmDescription: "サポートの上部とモデルの間に高密度の厚板を形成します。モデルとサポート材の間にスキンが作成されます。"
                }
            }
            FdmQml_Param{ id:support_interface_height; objectName: "support_interface_height"
                fdmLabel: "サポートインタフェース厚さ"
                fdmDescription: "底面または上部のモデルと接触するサポートのインターフェイスの厚さ。"
                FdmQml_Param{ id:support_roof_height; objectName: "support_roof_height"
                    fdmLabel: "サポートルーフ厚さ"
                    fdmDescription: "サポートのルーフの厚さ。これは、モデルの下につくサポートの上部にある密度の量を制御します。"
                }
                FdmQml_Param{ id:support_bottom_height; objectName: "support_bottom_height"
                    fdmLabel: "サポートフロア厚さ"
                    fdmDescription: "サポート材の底部の厚さ。これは、サポートが置かれるモデル上の積層密度を制御します。"
                }
            }
            FdmQml_Param{ id:support_interface_skip_height; objectName: "support_interface_skip_height"
                fdmLabel: "サポートインタフェース解像度"
                fdmDescription: "サポートの上下にモデルがあるかどうか確認するには、特定のサポートの高さを見ます。低い値はスライスに時間がかかり、高い値にするとサポートのインターフェイスがある場所に通常のサポートをプリントする可能性があります。"
            }
            FdmQml_Param{ id:support_interface_density; objectName: "support_interface_density"
                fdmLabel: "サポートインタフェース密度"
                fdmDescription: "サポート材のルーフとフロアの密度を調整します 大きな値ではオーバーハングでの成功率があがりますが、サポート材が除去しにくくなります。"
                FdmQml_Param{ id:support_roof_density; objectName: "support_roof_density"
                    fdmLabel: "サポートルーフ密度"
                    fdmDescription: "サポート材のルーフの部分の密度を調整します 大きな値ではオーバーハングの成功率があがりますが、サポート材が除去しにくくなります。"
                    FdmQml_Param{ id:support_roof_line_distance; objectName: "support_roof_line_distance"
                        fdmLabel: "サポートルーフライン距離"
                        fdmDescription: "プリントされたサポートルーフ線間の距離。この設定は、サポート密度によって計算されますが、個別に調整することもできます。"
                    }
                }
                FdmQml_Param{ id:support_bottom_density; objectName: "support_bottom_density"
                    fdmLabel: "サポートフロア密度"
                    fdmDescription: "サポート構造のフロアの密度です。高い値は、サポートのよりよい接着を促します。"
                    FdmQml_Param{ id:support_bottom_line_distance; objectName: "support_bottom_line_distance"
                        fdmLabel: "サポートフロアライン距離"
                        fdmDescription: "プリントされたサポートのフロアのライン間の距離。この設定は、密度によって計算されますが、個別に調整することもできます。"
                    }
                }
            }
            FdmQml_Param{ id:support_interface_pattern; objectName: "support_interface_pattern"
                fdmLabel: "サポートインタフェースパターン"
                fdmDescription: "モデルとサポートのインタフェースがプリントされるパターン。"
                fdmOptions:{
                    "lines": "ライン",
                    "grid": "グリッド",
                    "triangles": "トライアングル",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ"
                }
                FdmQml_Param{ id:support_roof_pattern; objectName: "support_roof_pattern"
                    fdmLabel: "サポートルーフパターン"
                    fdmDescription: "サポートのルーフがプリントされるパターン。"
                    fdmOptions:{
                        "lines": "ライン",
                        "grid": "グリッド",
                        "triangles": "トライアングル",
                        "concentric": "同心円",
                        "zigzag": "ジグザグ"
                    }
                }
                FdmQml_Param{ id:support_bottom_pattern; objectName: "support_bottom_pattern"
                    fdmLabel: "サポートフロアパターン"
                    fdmDescription: "サポートのフロアがプリントされるパターン。"
                    fdmOptions:{
                        "lines": "ライン",
                        "grid": "グリッド",
                        "triangles": "トライアングル",
                        "concentric": "同心円",
                        "zigzag": "ジグザグ"
                    }
                }
            }
            FdmQml_Param{ id:minimum_interface_area; objectName: "minimum_interface_area"
                fdmLabel: "最小サポートインターフェイス領域"
                fdmDescription: "サポートインターフェイスポリゴンの最小領域サイズ。この値より小さい領域のポリゴンは通常のサポートとしてプリントされます。"
                FdmQml_Param{ id:minimum_roof_area; objectName: "minimum_roof_area"
                    fdmLabel: "最小サポートルーフ領域"
                    fdmDescription: "サポートのルーフの最小領域サイズ。この値より小さい領域のポリゴンは通常のサポートとしてプリントされます。"
                }
                FdmQml_Param{ id:minimum_bottom_area; objectName: "minimum_bottom_area"
                    fdmLabel: "最小サポートフロア領域"
                    fdmDescription: "サポートのフロアの最小領域サイズ。この値より小さい領域のポリゴンは通常のサポートとしてプリントされます。"
                }
            }
            FdmQml_Param{ id:support_interface_offset; objectName: "support_interface_offset"
                fdmLabel: "サポートインターフェイス水平展開"
                fdmDescription: "サポートインターフェイスポリゴンに適用されるオフセット量。"
                FdmQml_Param{ id:support_roof_offset; objectName: "support_roof_offset"
                    fdmLabel: "サポートルーフ水平展開"
                    fdmDescription: "サポートのルーフに適用されるオフセット量。"
                }
                FdmQml_Param{ id:support_bottom_offset; objectName: "support_bottom_offset"
                    fdmLabel: "サポートフロア水平展開"
                    fdmDescription: "サポートのフロアに適用されるオフセット量。"
                }
            }
            FdmQml_Param{ id:support_interface_angles; objectName: "support_interface_angles"
                fdmLabel: "サポート面のライン方向"
                fdmDescription: "使用する整数線の方向のリスト。リストの要素は、レイヤの層に合わせて順番に使用され、リストの末尾に達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは空のリストとなり、デフォルトの角度を使用します（面がかなり厚い場合には45度と135度を交互に使用、それ以外では90度を使用）。"
                FdmQml_Param{ id:support_roof_angles; objectName: "support_roof_angles"
                    fdmLabel: "サポートルーフライン方向"
                    fdmDescription: "使用する整数線の方向のリスト。リストの要素は、レイヤの層に合わせて順番に使用され、リストの末尾に達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは空のリストとなり、デフォルトの角度を使用します（面がかなり厚い場合には45度と135度を交互に使用、それ以外では90度を使用）。"
                }
                FdmQml_Param{ id:support_bottom_angles; objectName: "support_bottom_angles"
                    fdmLabel: "サポートフロアライン方向"
                    fdmDescription: "使用する整数線の方向のリスト。リストの要素は、レイヤの層に合わせて順番に使用され、リストの末尾に達すると、最初から再び開始されます。リスト項目はコンマで区切られ、リスト全体は大括弧で囲まれています。デフォルトは空のリストとなり、デフォルトの角度を使用します（面がかなり厚い場合には45度と135度を交互に使用、それ以外では90度を使用）。"
                }
            }
            FdmQml_Param{ id:support_fan_enable; objectName: "support_fan_enable"
                fdmLabel: "ファン速度上書き"
                fdmDescription: "有効にすると、サポートを超えた直後にプリント冷却ファンの速度がスキン領域に対して変更されます。"
            }
            FdmQml_Param{ id:support_supported_skin_fan_speed; objectName: "support_supported_skin_fan_speed"
                fdmLabel: "サポート対象スキンファン速度"
                fdmDescription: "サポートを超えた直後にスキン領域に印字するときに使用するファン速度を割合で示します。高速ファンを使用すると、サポートが取り外しやすくなります。"
            }
            FdmQml_Param{ id:support_use_towers; objectName: "support_use_towers"
                fdmLabel: "使用タワー"
                fdmDescription: "特殊なタワーを使用して、小さなオーバーハングしているエリアをサポートします。これらの塔は、サポートできる領域より大きな直径を支えれます。オーバーハング付近では塔の直径が減少し、ルーフを形成します。"
            }
            FdmQml_Param{ id:support_tower_diameter; objectName: "support_tower_diameter"
                fdmLabel: "タワー直径"
                fdmDescription: "特別な塔の直径。"
            }
            FdmQml_Param{ id:support_tower_maximum_supported_diameter; objectName: "support_tower_maximum_supported_diameter"
                fdmLabel: "最大タワーサポート直径"
                fdmDescription: "特殊なサポートタワーにより支持される小さな領域のX / Y方向の最小直径。"
            }
            FdmQml_Param{ id:support_tower_roof_angle; objectName: "support_tower_roof_angle"
                fdmLabel: "タワールーフ角度"
                fdmDescription: "タワーの屋上の角度。値が高いほど尖った屋根が得られ、値が低いほど屋根が平らになります。"
            }
            FdmQml_Param{ id:support_mesh_drop_down; objectName: "support_mesh_drop_down"
                fdmLabel: "サポートメッシュの下処理"
                fdmDescription: "サポートメッシュの下のサポート材を全箇所に作ります、これはサポートメッシュ下にてオーバーハングしないようにするためです。"
            }
            FdmQml_Param{ id:support_meshes_present; objectName: "support_meshes_present"
                fdmLabel: "シーンにサポートメッシュがある"
                fdmDescription: "シーンにはサポートメッシュがあります。この設定はAnkerMakeで制御されます。"
            }
        }
        FdmQml_Category{ id:platform_adhesion; objectName: "platform_adhesion"
            fdmLabel: "ビルドプレート密着性"
            fdmDescription: "密着性"
            FdmQml_Param{ id:prime_blob_enable; objectName: "prime_blob_enable"
                fdmLabel: "プライムボルブを有効にする"
                fdmDescription: "プリントする前にフィラメントの小さな塊を作るかどうか。この設定をオンにすると、エクストルーダーがノズルにおいてプリント予定のマテリアルの下準備をします。プリント後ブリムまたはスカートも、上記と同じような意味を持ちます。この設定をオフにすると時間の節約にはなります。"
            }
            FdmQml_Param{ id:extruder_prime_pos_x; objectName: "extruder_prime_pos_x"
                fdmLabel: "エクストルーダープライムX位置"
                fdmDescription: "プリント開始時のノズルの位置を表すX座標。"
            }
            FdmQml_Param{ id:extruder_prime_pos_y; objectName: "extruder_prime_pos_y"
                fdmLabel: "エクストルーダープライムY位置"
                fdmDescription: "プリント開始時にノズル位置を表すY座標。"
            }
            FdmQml_Param{ id:adhesion_type; objectName: "adhesion_type"
                fdmLabel: "ビルドプレート接着タイプ"
                fdmDescription: "エクストルーダーとビルドプレートへの接着両方を改善するのに役立つさまざまなオプション。 Brimは、モデルのベースの周りに単一レイヤーを平面的に追加して、ワーピングを防止します。 Raftは、モデルの下に太いグリッドを追加します。スカートはモデルの周りにプリントされたラインですが、モデルには接続されていません。"
                fdmOptions:{
                    "skirt": "スカート",
                    "brim": "ブリム",
                    "raft": "ラフト",
                    "autobrim": "オートブリム",
                    "none": "なし"
                }
            }
            FdmQml_Param{ id:adhesion_extruder_nr; objectName: "adhesion_extruder_nr"
                fdmLabel: "ビルドプレート接着エクストルーダー"
                fdmDescription: "スカート/ブリム/ラフトをプリントする際のエクストルーダー。これはマルチエクストルージョン時に使用されます。"
            }
            FdmQml_Param{ id:skirt_line_count; objectName: "skirt_line_count"
                fdmLabel: "スカートライン数"
                fdmDescription: "複数のスカートラインを使用すると、小さなモデル形成時の射出をより良く行うことができます。これを0に設定するとスカートが無効になります。"
            }
            FdmQml_Param{ id:skirt_gap; objectName: "skirt_gap"
                fdmLabel: "スカート距離"
                fdmDescription: "スカートと初期レイヤーとの水平距離。この距離から外側に複数のスカート線が伸びていきます。"
            }
            FdmQml_Param{ id:skirt_brim_minimal_length; objectName: "skirt_brim_minimal_length"
                fdmLabel: "スカート/ブリム最小長さ"
                fdmDescription: "スカートまたはブリム最短の長さ。この長さにすべてのスカートまたはブリムが達していない場合は、最小限の長さに達するまで、スカートまたはブリムラインが追加されます。注：行数が0に設定されている場合、これは無視されます。"
            }
            FdmQml_Param{ id:brim_width; objectName: "brim_width"
                fdmLabel: "ブリム幅"
                fdmDescription: "モデルから最外線のブリムまでの距離。大きなブリムは、ビルドプレートへの接着を高めますが、有効なプリント面積も減少させます。"
                FdmQml_Param{ id:brim_line_count; objectName: "brim_line_count"
                    fdmLabel: "ブリムライン数"
                    fdmDescription: "ブリムに使用される線数。ブリムの線数は、ビルドプレートへの接着性を向上させるだけでなく、有効なプリント面積を減少させる。"
                }
            }
            FdmQml_Param{ id:min_contact_area; objectName: "min_contact_area"
                fdmLabel: "最小接触面積"
                fdmDescription: "印刷領域と構築プラットフォームの接触面積がこの最小面積よりも小さい場合、印刷領域の周囲にブリムを追加することがトリガーされます。これにより、印刷物と構築プラットフォームの接触面積が増え、印刷の成功率が向上します。"
            }
            FdmQml_Param{ id:brim_gap; objectName: "brim_gap"
                fdmLabel: "ブリム距離"
                fdmDescription: "最初のブリムラインとプリントの最初のレイヤーの輪郭との間の水平距離。小さなギャップがあると、ブリムの取り外しが容易になり、断熱性の面でもメリットがあります。"
            }
            FdmQml_Param{ id:brim_replaces_support; objectName: "brim_replaces_support"
                fdmLabel: "ブリム交換サポート"
                fdmDescription: "スペースがサポートで埋まっている場合でも、モデルの周辺にブリムをプリントします。これにより、サポートの最初の層の一部のエリアがブリムになります。"
            }
            FdmQml_Param{ id:brim_outside_only; objectName: "brim_outside_only"
                fdmLabel: "外側にブリムのみプリント"
                fdmDescription: "モデルの外側のみにブリムをプリントします。これにより、後で取り除くブリムの量が減少します。またプレートへの接着力はそれほど低下しません。"
            }
            FdmQml_Param{ id:raft_margin; objectName: "raft_margin"
                fdmLabel: "ラフトの余分なマージン"
                fdmDescription: "ラフトが有効になっている場合、モデルの周りに余分なラフト領域ができます。値を大きくするとより強力なラフトができますが、多くの材料を使用し、造形範囲は少なくなります。"
            }
            FdmQml_Param{ id:raft_smoothing; objectName: "raft_smoothing"
                fdmLabel: "ラフト補整"
                fdmDescription: "この設定は、ラフトの輪郭の内側の角がどの程度丸められるかを制御します。内側の角は、ここで指定した値と等しい半径の半円に丸められます。この設定は、そのような円よりも小さいラフトの輪郭の穴を削除します。"
            }
            FdmQml_Param{ id:raft_airgap; objectName: "raft_airgap"
                fdmLabel: "ラフト間のラップ"
                fdmDescription: "モデルの第一層のラフトと最終ラフト層の隙間。この値で第1層のみを上げることで、ラフトとモデルとの間の結合を低下させる。結果ラフトを剥がしやすくします。"
            }
            FdmQml_Param{ id:layer_0_z_overlap; objectName: "layer_0_z_overlap"
                fdmLabel: "初期レイヤーZのオーバーラップ"
                fdmDescription: "エアギャップ内で失われたフィラメントを補うために、モデルの第1層と第2層をZ方向にオーバーラップさせます。この値によって、最初のモデルレイヤーがシフトダウンされます。"
            }
            FdmQml_Param{ id:raft_surface_layers; objectName: "raft_surface_layers"
                fdmLabel: "ラフト最上層"
                fdmDescription: "第2ラフト層の上の最上層の数。これらは、モデルが置かれる完全に塗りつぶされた積層です。 2つの層は、1よりも滑らかな上面をもたらす。"
            }
            FdmQml_Param{ id:raft_surface_thickness; objectName: "raft_surface_thickness"
                fdmLabel: "ラフト最上層厚さ"
                fdmDescription: "トップラフト層の層厚。"
            }
            FdmQml_Param{ id:raft_surface_line_width; objectName: "raft_surface_line_width"
                fdmLabel: "ラフト最上ライン幅"
                fdmDescription: "ラフトの上面の線の幅。これらは細い線で、ラフトの頂部が滑らかになります。"
            }
            FdmQml_Param{ id:raft_surface_line_spacing; objectName: "raft_surface_line_spacing"
                fdmLabel: "ラフト最上面スペース"
                fdmDescription: "上のラフト層とラフト線の間の距離。間隔は線の幅と同じにして、サーフェスがソリッドになるようにします。"
            }
            FdmQml_Param{ id:raft_interface_thickness; objectName: "raft_interface_thickness"
                fdmLabel: "ラフト中央厚さ"
                fdmDescription: "中間のラフト層の層の厚さ。"
            }
            FdmQml_Param{ id:raft_interface_line_width; objectName: "raft_interface_line_width"
                fdmLabel: "ラフト中央ライン幅"
                fdmDescription: "中間ラフト層の線の幅。第2層をより押し出すと、ラインがビルドプレートに固着します。"
            }
            FdmQml_Param{ id:raft_interface_line_spacing; objectName: "raft_interface_line_spacing"
                fdmLabel: "ラフト中間スペース"
                fdmDescription: "中間ラフト層とラフト線の間の距離。中央の間隔はかなり広くなければならず、トップラフト層を支えるために十分な密度でなければならない。"
            }
            FdmQml_Param{ id:raft_base_thickness; objectName: "raft_base_thickness"
                fdmLabel: "ラフトベース厚さ"
                fdmDescription: "ベースラフト層の層厚さ。プリンタのビルドプレートにしっかりと固着する厚い層でなければなりません。"
            }
            FdmQml_Param{ id:raft_base_line_width; objectName: "raft_base_line_width"
                fdmLabel: "ラフトベースライン幅"
                fdmDescription: "ベースラフト層の線幅。ビルドプレートの接着のため太い線でなければなりません。"
            }
            FdmQml_Param{ id:raft_base_line_spacing; objectName: "raft_base_line_spacing"
                fdmLabel: "ラフトベースラインスペース"
                fdmDescription: "ベースラフト層のラフトライン間の距離。広い間隔は、ブルドプレートからのラフトの除去を容易にする。"
            }
            FdmQml_Param{ id:raft_speed; objectName: "raft_speed"
                fdmLabel: "ラフトプリント速度"
                fdmDescription: "ラフトがプリントされる速度。"
                FdmQml_Param{ id:raft_surface_speed; objectName: "raft_surface_speed"
                    fdmLabel: "ラフト上層プリント速度"
                    fdmDescription: "トップラフト層がプリントされる速度。この値はノズルが隣接するサーフェスラインをゆっくりと滑らかにするために、少し遅くプリントする必要があります。"
                }
                FdmQml_Param{ id:raft_interface_speed; objectName: "raft_interface_speed"
                    fdmLabel: "ラフト中間プリント速度"
                    fdmDescription: "ミドルラフト層がプリントされる速度。ノズルから出てくるマテリアルの量がかなり多いので、ゆっくりとプリントされるべきである。"
                }
                FdmQml_Param{ id:raft_base_speed; objectName: "raft_base_speed"
                    fdmLabel: "ラフトベースプリント速度"
                    fdmDescription: "ベースラフト層がプリントされる速度。これは、ノズルから出てくるマテリアルの量がかなり多いので、ゆっくりとプリントされるべきである。"
                }
            }
            FdmQml_Param{ id:raft_acceleration; objectName: "raft_acceleration"
                fdmLabel: "ラフトプリント加速度"
                fdmDescription: "ラフトプリント時の加速度。"
                FdmQml_Param{ id:raft_surface_acceleration; objectName: "raft_surface_acceleration"
                    fdmLabel: "ラフト上層層プリント加速度"
                    fdmDescription: "ラフトのトッププリント時の加速度。"
                }
                FdmQml_Param{ id:raft_interface_acceleration; objectName: "raft_interface_acceleration"
                    fdmLabel: "ラフト中間層プリント加速度"
                    fdmDescription: "ラフトの中間層プリント時の加速度。"
                }
                FdmQml_Param{ id:raft_base_acceleration; objectName: "raft_base_acceleration"
                    fdmLabel: "ラフトベースプリント加速度"
                    fdmDescription: "ラフトの底面プリント時の加速度。"
                }
            }
            FdmQml_Param{ id:raft_jerk; objectName: "raft_jerk"
                fdmLabel: "ラフトプリントジャーク"
                fdmDescription: "ラフトがプリント時のジャーク。"
                FdmQml_Param{ id:raft_surface_jerk; objectName: "raft_surface_jerk"
                    fdmLabel: "ラフト上層プリントジャーク"
                    fdmDescription: "トップラフト層プリント時のジャーク。"
                }
                FdmQml_Param{ id:raft_interface_jerk; objectName: "raft_interface_jerk"
                    fdmLabel: "ラフト中間層プリントジャーク"
                    fdmDescription: "ミドルラフト層プリント時のジャーク。"
                }
                FdmQml_Param{ id:raft_base_jerk; objectName: "raft_base_jerk"
                    fdmLabel: "ラフトベースプリントジャーク"
                    fdmDescription: "ベースラフト層プリント時のジャーク。"
                }
            }
            FdmQml_Param{ id:raft_fan_speed; objectName: "raft_fan_speed"
                fdmLabel: "ラフトファン速度"
                fdmDescription: "ラフトプリント時のファンの速度。"
                FdmQml_Param{ id:raft_surface_fan_speed; objectName: "raft_surface_fan_speed"
                    fdmLabel: "ラフト上層ファン速度"
                    fdmDescription: "トップラフトプリント時のファンの速度。"
                }
                FdmQml_Param{ id:raft_interface_fan_speed; objectName: "raft_interface_fan_speed"
                    fdmLabel: "ラフト中間層ファン速度"
                    fdmDescription: "ミドルラフトプリント時のファンの速度。"
                }
                FdmQml_Param{ id:raft_base_fan_speed; objectName: "raft_base_fan_speed"
                    fdmLabel: "ラフトベースファン速度"
                    fdmDescription: "ベースラフト層プリント時のファン速度。"
                }
            }
        }
        FdmQml_Category{ id:dual; objectName: "dual"
            fdmLabel: "デュアルエクストルーダー"
            fdmDescription: "デュアルエクストルーダーでプリントするための設定。"
            FdmQml_Param{ id:prime_tower_enable; objectName: "prime_tower_enable"
                fdmLabel: "プライムタワーを有効にする"
                fdmDescription: "プリント物の横にタワーを造形して、ノズル交換後にフィラメントの調整をします。"
            }
            FdmQml_Param{ id:prime_tower_size; objectName: "prime_tower_size"
                fdmLabel: "プライムタワーのサイズ"
                fdmDescription: "プライムタワーの幅。"
            }
            FdmQml_Param{ id:prime_tower_min_volume; objectName: "prime_tower_min_volume"
                fdmLabel: "プライムタワー最小容積"
                fdmDescription: "プライムタワーの各層の最小容積。"
            }
            FdmQml_Param{ id:prime_tower_position_x; objectName: "prime_tower_position_x"
                fdmLabel: "プライムタワーX位置"
                fdmDescription: "プライムタワーの位置のx座標。"
            }
            FdmQml_Param{ id:prime_tower_position_y; objectName: "prime_tower_position_y"
                fdmLabel: "プライムタワーY位置"
                fdmDescription: "プライムタワーの位置のy座標。"
            }
            FdmQml_Param{ id:prime_tower_wipe_enabled; objectName: "prime_tower_wipe_enabled"
                fdmLabel: "プライムタワーノズル拭き取り"
                fdmDescription: "1本のノズルでプライムタワーをプリントした後、もう片方のノズルから滲み出した材料をプライムタワーが拭き取ります。"
            }
            FdmQml_Param{ id:prime_tower_brim_enable; objectName: "prime_tower_brim_enable"
                fdmLabel: "プライムタワーブリム"
                fdmDescription: "モデルがない場合でも、プライムタワーには、ブリムによって与えられる追加の付着が必要なことがあります。現在は「ラフト」密着型では使用できません。"
            }
            FdmQml_Param{ id:ooze_shield_enabled; objectName: "ooze_shield_enabled"
                fdmLabel: "Ooze Shieldを有効にする"
                fdmDescription: "モデルの周りに壁（ooze shield）を作る。これを生成することで、一つ目のノズルの高さと2つ目のノズルが同じ高さであったとき、2つ目のノズルを綺麗にします。"
            }
            FdmQml_Param{ id:ooze_shield_angle; objectName: "ooze_shield_angle"
                fdmLabel: "Ooze Shield角度"
                fdmDescription: "壁（ooze shield）作成時の最大の角度。 0度は垂直であり、90度は水平である。角度を小さくすると、壁が少なくなりますが、より多くの材料が使用されます。"
            }
            FdmQml_Param{ id:ooze_shield_dist; objectName: "ooze_shield_dist"
                fdmLabel: "Ooze Shield距離"
                fdmDescription: "壁（ooze shield）の造形物からの距離。"
            }
            FdmQml_Param{ id:switch_extruder_retraction_amount; objectName: "switch_extruder_retraction_amount"
                fdmLabel: "ノズルスイッチ引き戻し距離"
                fdmDescription: "エクストルーダー切り替え時の引き込み量。引き込みを行わない場合は0に設定します。これは通常、ヒートゾーンの長さと同じに設定します。"
            }
            FdmQml_Param{ id:switch_extruder_retraction_speeds; objectName: "switch_extruder_retraction_speeds"
                fdmLabel: "ノズルスイッチ引き戻し速度"
                fdmDescription: "フィラメントを引き戻す速度。速度が早い程良いが早すぎるとフィラメントを削ってしまう可能性があります。"
                FdmQml_Param{ id:switch_extruder_retraction_speed; objectName: "switch_extruder_retraction_speed"
                    fdmLabel: "ノズルスイッチ引き込み速度"
                    fdmDescription: "ノズル切り替え中のフィラメントの引き込み速度。"
                }
                FdmQml_Param{ id:switch_extruder_prime_speed; objectName: "switch_extruder_prime_speed"
                    fdmLabel: "ノズルスイッチ押し戻し速度"
                    fdmDescription: "ノズル スイッチ後にフィラメントが押し戻される速度。"
                }
            }
            FdmQml_Param{ id:switch_extruder_extra_prime_amount; objectName: "switch_extruder_extra_prime_amount"
                fdmLabel: "ノズル切替え後のプライムに必要な余剰量"
                fdmDescription: "ノズル切替え後のプライムに必要な余剰材料。"
            }
        }
        FdmQml_Category{ id:meshfix; objectName: "meshfix"
            fdmLabel: "メッシュ修正"
            fdmDescription: "3Dプリンティングにさらに適したメッシュを作成します。"
            FdmQml_Param{ id:meshfix_union_all; objectName: "meshfix_union_all"
                fdmLabel: "重複量"
                fdmDescription: "メッシュ内の重なり合うボリュームから生じる内部ジオメトリを無視し、ボリュームを1つとしてプリントします。これにより、意図しない内部空洞が消えることがあります。"
            }
            FdmQml_Param{ id:meshfix_union_all_remove_holes; objectName: "meshfix_union_all_remove_holes"
                fdmLabel: "全穴除去"
                fdmDescription: "各レイヤーの穴を消し、外形のみを保持します。これにより、見えない部分の不要な部分が無視されますが、表面上にある穴も全て造形されなくなります。"
            }
            FdmQml_Param{ id:meshfix_extensive_stitching; objectName: "meshfix_extensive_stitching"
                fdmLabel: "強めのスティッチング"
                fdmDescription: "強めのスティッチングは、穴をメッシュで塞いでデータを作成します。このオプションは、長い処理時間が必要となります。"
            }
            FdmQml_Param{ id:meshfix_keep_open_polygons; objectName: "meshfix_keep_open_polygons"
                fdmLabel: "スティッチできない部分を保持"
                fdmDescription: "通常、AnkerMakeはメッシュ内の小さな穴をスティッチし、大きな穴のあるレイヤーの部分を削除しようとします。このオプションを有効にすると、スティッチできない部分が保持されます。このオプションは、他のすべてが適切なG-codeを生成できない場合の最後の手段として使用する必要があります。"
            }
            FdmQml_Param{ id:multiple_mesh_overlap; objectName: "multiple_mesh_overlap"
                fdmLabel: "重複メッシュのマージ"
                fdmDescription: "触れているメッシュを少し重ねてください。これによって、より良い接着をします。"
            }
            FdmQml_Param{ id:carve_multiple_volumes; objectName: "carve_multiple_volumes"
                fdmLabel: "重複メッシュの削除"
                fdmDescription: "複数のメッシュが重なっている領域を削除します。これは、結合された2つのマテリアルのオブジェクトが互いに重なっている場合に使用されます。"
            }
            FdmQml_Param{ id:alternate_carve_order; objectName: "alternate_carve_order"
                fdmLabel: "代替メッシュの削除"
                fdmDescription: "交差するメッシュがどのレイヤーに属しているかを切り替えることで、オーバーラップしているメッシュを絡み合うようにします。この設定をオフにすると、一方のメッシュはオーバーラップ内のすべてのボリュームを取得し、他方のメッシュは他から削除されます。"
            }
            FdmQml_Param{ id:remove_empty_first_layers; objectName: "remove_empty_first_layers"
                fdmLabel: "空の最初のメッシュの削除"
                fdmDescription: "最初にプリントしたレイヤーの下に空のレイヤーがある場合は取り除きます。この設定を無効にすると、スライストレランスが「排他」または「中間」に設定されている場合に最初のレイヤーが空になる原因になります。"
            }
        }
        FdmQml_Category{ id:blackmagic; objectName: "blackmagic"
            fdmLabel: "特別モード"
            fdmDescription: "これまでにないモデルのプリント方法です。"
            FdmQml_Param{ id:print_sequence; objectName: "print_sequence"
                fdmLabel: "プリント頻度"
                fdmDescription: "すべてのモデルをレイヤーごとにプリントするか、1つのモデルがプリント完了するのを待ち次のモデルに移動するかどうか。a）エクストルーダーが1つだけ有効であり、b）プリントヘッド全体がモデル間を通ることができるようにすべてのモデルが離れていて、すべてのモデルがノズルとX/Y軸間の距離よりも小さい場合、1つずつプリントする事ができます。"
                fdmOptions:{
                    "all_at_once": "一度にすべて",
                    "one_at_a_time": "1つずつ"
                }
            }
            FdmQml_Param{ id:infill_mesh; objectName: "infill_mesh"
                fdmLabel: "インフィルメッシュ"
                fdmDescription: "このメッシュを使用して、重なる他のメッシュのインフィルを変更します。他のメッシュのインフィル領域を改なメッシュに置き換えます。これを利用する場合、1つのWallだけをプリントしTop / Bottom Skinは使用しないことをお勧めします。"
            }
            FdmQml_Param{ id:infill_mesh_order; objectName: "infill_mesh_order"
                fdmLabel: "メッシュ処理ランク"
                fdmDescription: "インフィルメッシュの重なりが複数生じた場合のこのメッシュの優先度を決定します。複数のインフィルメッシュの重なりがあるエリアでは、最もランクが高いメッシュの設定になります。ランクが高いインフィルメッシュは、ランクが低いインフィルメッシュのインフィルと通常のメッシュを変更します。"
            }
            FdmQml_Param{ id:cutting_mesh; objectName: "cutting_mesh"
                fdmLabel: "メッシュ切断"
                fdmDescription: "このメッシュの大きさをを他のメッシュ内に制限します。この設定を使用することで、1つの特定のメッシュ領域の設定を、、全く別のエクストルーダーで作成することができます。"
            }
            FdmQml_Param{ id:mold_enabled; objectName: "mold_enabled"
                fdmLabel: "型"
                fdmDescription: "型を取るためプリントし、ビルドプレート上の同じようなモデルを得るためにキャスト用のプリントをします。"
            }
            FdmQml_Param{ id:mold_width; objectName: "mold_width"
                fdmLabel: "最小型幅"
                fdmDescription: "型用とモデルの外側の最短距離。"
            }
            FdmQml_Param{ id:mold_roof_height; objectName: "mold_roof_height"
                fdmLabel: "型ルーフ高さ"
                fdmDescription: "型をプリントするためのモデルの水平部分上の高さ。"
            }
            FdmQml_Param{ id:mold_angle; objectName: "mold_angle"
                fdmLabel: "型角度"
                fdmDescription: "型の外側の壁のオーバーハングの角度です。0度にすると垂直の外殻をつくります。 90度は輪郭に従いモデルの外側の外殻をつくります。"
            }
            FdmQml_Param{ id:support_mesh; objectName: "support_mesh"
                fdmLabel: "サポートメッシュ"
                fdmDescription: "このメッシュを使用してサポート領域を指定します。これは、サポート構造を生成するために使用できます。"
            }
            FdmQml_Param{ id:anti_overhang_mesh; objectName: "anti_overhang_mesh"
                fdmLabel: "メッシュオーバーハング例外"
                fdmDescription: "このメッシュを使用して、モデルのどの部分をオーバーハングとして検出する必要がないかを指定します。これは、不要なサポート構造を削除するために使用できます。"
            }
            FdmQml_Param{ id:magic_mesh_surface_mode; objectName: "magic_mesh_surface_mode"
                fdmLabel: "表面モード"
                fdmDescription: "モデルを表面のみ、ボリューム、または緩い表面のボリュームとして扱います。通常のプリントモードでは、囲まれた内部がプリントされます。 「Surface」は表面のみプリントをして、インフィルもトップもボトムもプリントしません。 \"Both\"は通常と同様に囲まれた内部をプリントし残りのポリゴンをサーフェスとしてプリントします。"
                fdmOptions:{
                    "normal": "標準",
                    "surface": "表面",
                    "both": "両方"
                }
            }
            FdmQml_Param{ id:magic_spiralize; objectName: "magic_spiralize"
                fdmLabel: "滑らかな外側輪郭"
                fdmDescription: "Z軸の外側のエッジの動きを滑らかにします。全体のプリントに安定したZの動きを促し、この機能によりソリッドのモデルを固定した底辺と単一のウォールのプリントにします。この機能は各レイヤーが単一の部品を含んでいる場合のみに有効です。"
                FdmQml_Param{ id:magic_spiralize_print_speed; objectName: "magic_spiralize_print_speed"
                    fdmLabel: "滑らかな外側輪郭速度"
                    fdmDescription: "滑らかな外側輪郭速度"
                }
                FdmQml_Param{ id:smooth_spiralized_contours; objectName: "smooth_spiralized_contours"
                    fdmLabel: "滑らかな輪郭"
                    fdmDescription: "らせん状の輪郭を滑らかにしてZシームの視認性を低下させます (Zシームはプリント物上でほとんどみえませんが、層ビューでは確認できます)。スムージングは、細かい表面の詳細をぼかす傾向があることに注意してください。"
                }
            }
        }
        FdmQml_Category{ id:experimental; objectName: "experimental"
            fdmLabel: "実験"
            fdmDescription: "これからもっと充実させていく機能です。"
            FdmQml_Param{ id:slicing_tolerance; objectName: "slicing_tolerance"
                fdmLabel: "スライス公差"
                fdmDescription: "スライスされたレイヤーにおける垂直方向の公差です。レイヤーの輪郭は通常、各レイヤーの厚さの中間を通る断面で生成されます（中間）。代わりに、レイヤーごとに、ボリューム内にレイヤーの厚さの分だけ入り込んだエリアにしたり（排他）、レイヤー内の任意の位置まで入り込んだエリアにしたりする（包括）こともできます。排他は最も細かく、包括は最もフィットし、中間は元の表面に最も近くなります。"
                fdmOptions:{
                    "middle": "中間",
                    "exclusive": "排他",
                    "inclusive": "包括"
                }
            }
            FdmQml_Param{ id:roofing_line_width; objectName: "roofing_line_width"
                fdmLabel: "最上面のライン幅"
                fdmDescription: "プリントの上部の 線の幅。"
            }
            FdmQml_Param{ id:roofing_pattern; objectName: "roofing_pattern"
                fdmLabel: "上部表面パターン"
                fdmDescription: "上層のパターン。"
                fdmOptions:{
                    "lines": "直線",
                    "concentric": "同心円",
                    "zigzag": "ジグザグ"
                }
            }
            FdmQml_Param{ id:roofing_monotonic; objectName: "roofing_monotonic"
                fdmLabel: "上面方向一貫性"
                fdmDescription: "上面のラインを、隣接するラインと常に一方向で重なるような順序でプリントします。これにより、プリントにかかる時間は少し長くなりますが、平らな面の見た目の一貫性が高まります。"
            }
            FdmQml_Param{ id:roofing_angles; objectName: "roofing_angles"
                fdmLabel: "最上面のラインの向き"
                fdmDescription: "トップ表面層に縦かジグザグパターンを利用する時に使用する整数の行方向のリスト。リスト内から順番に使われていき、リストの最後に達するとまた最初の設定値に戻ります。リストアイテムはカンマで区切られ、全体はカッコで括られています。デフォルトでは何も入っておらず、設定角度は (45 度と 135 度)になっています。"
            }
            FdmQml_Param{ id:infill_enable_travel_optimization; objectName: "infill_enable_travel_optimization"
                fdmLabel: "インフィル移動最適化"
                fdmDescription: "有効化すると、移動距離が減少するようにインフィルラインをプリントする順序が最適化されます。移動時間の削減は、スライスするモデル、インフィルパターン、密度などに大きく依存します。特に、インフィルを行う小さなエリアが多数あるモデルの場合、モデルをスライスする時間が大きく増えることがあります。"
            }
            FdmQml_Param{ id:material_flow_dependent_temperature; objectName: "material_flow_dependent_temperature"
                fdmLabel: "自動温度"
                fdmDescription: "その画層の平均流速で自動的にレイヤーごとに温度を変更します。"
            }
            FdmQml_Param{ id:material_flow_temp_graph; objectName: "material_flow_temp_graph"
                fdmLabel: "フロー温度グラフ"
                fdmDescription: "マテリアルフロー(毎秒 3mm) と温度 (° c) をリンクします。"
            }
            FdmQml_Param{ id:minimum_polygon_circumference; objectName: "minimum_polygon_circumference"
                fdmLabel: "最小ポリゴン円周"
                fdmDescription: "この量よりも小さい円周を持つスライスレイヤーのポリゴンは、除外されます。値を小さくすると、スライス時間のコストで、メッシュの解像度が高くなります。つまり、ほとんどが高解像 SLA プリンター、極小多機能 3D モデルです。"
            }
            FdmQml_Param{ id:support_skip_some_zags; objectName: "support_skip_some_zags"
                fdmLabel: "接続部分のサポート分割"
                fdmDescription: "サポートラインの接続部分をスキップし、サポート材部分を壊れやすくします。この設定はジグザクのサポートインフィル材のパターンにて適用できます。"
            }
            FdmQml_Param{ id:support_skip_zag_per_mm; objectName: "support_skip_zag_per_mm"
                fdmLabel: "サポート分割サイズ"
                fdmDescription: "サポート毎行Nミリ時に、サポートの接続をわざと外し、後のサポート材の構造をもろくし、壊れやすくする。"
                FdmQml_Param{ id:support_zag_skip_count; objectName: "support_zag_skip_count"
                    fdmLabel: "サポート分割ライン数"
                    fdmDescription: "毎行Nミリ時に、サポートの接続をわざとスキップし、後のサポート材の構造をもろくし、壊れやすくする。"
                }
            }
            FdmQml_Param{ id:draft_shield_enabled; objectName: "draft_shield_enabled"
                fdmLabel: "ドラフトシールドを有効にする"
                fdmDescription: "これにより、モデルの周囲に壁ができ、熱を閉じ込め、外気の流れを遮蔽します。特に反りやすい材料に有効です。"
            }
            FdmQml_Param{ id:draft_shield_dist; objectName: "draft_shield_dist"
                fdmLabel: "ドラフトシールドとX/Yの距離"
                fdmDescription: "ドラフトシールドと造形物のX / Y方向の距離。"
            }
            FdmQml_Param{ id:draft_shield_height_limitation; objectName: "draft_shield_height_limitation"
                fdmLabel: "ドラフトシールドの制限"
                fdmDescription: "ドラフトシールドの高さを設定します。ドラフトシールドは、モデルの全高、または限られた高さでプリントするように選択します。"
                fdmOptions:{
                    "full": "制限なし",
                    "limited": "制限あり"
                }
            }
            FdmQml_Param{ id:draft_shield_height; objectName: "draft_shield_height"
                fdmLabel: "ドラフトシールドの高さ"
                fdmDescription: "ドラフトシールドの高さ制限。この高さを超えるとドラフトシールドがプリントされません。"
            }
            FdmQml_Param{ id:conical_overhang_enabled; objectName: "conical_overhang_enabled"
                fdmLabel: "オーバーハングプリント可能"
                fdmDescription: "最小限のサポートが必要となるようにプリントモデルのジオメトリを変更します。急なオーバーハングは浅いオーバーハングになります。オーバーハングした領域は、より垂直になるように下がります。"
            }
            FdmQml_Param{ id:conical_overhang_angle; objectName: "conical_overhang_angle"
                fdmLabel: "最大モデル角度"
                fdmDescription: "プリント可能になったオーバーハングの最大角度。 0°の値では、すべてのオーバーハングがビルドプレートに接続されたモデルの一部に置き換えられます。90°では、モデルは決して変更されません。"
            }
            FdmQml_Param{ id:conical_overhang_hole_size; objectName: "conical_overhang_hole_size"
                fdmLabel: "オーバーハングした穴の最大領域"
                fdmDescription: "モデル底部にある穴の最大領域（「オーバーハングプリント可能」で削除する前の値）。これより小さい穴は保持されます。値が0 mm²の場合、モデル底部にあるすべての穴は充填されます。"
            }
            FdmQml_Param{ id:cross_infill_pocket_size; objectName: "cross_infill_pocket_size"
                fdmLabel: "3Dクロスポケットのサイズ"
                fdmDescription: "四方でクロス3Dパターンが交差するポケットの大きさはそのパターンが触れている高さ。"
            }
            FdmQml_Param{ id:cross_infill_density_image; objectName: "cross_infill_density_image"
                fdmLabel: "クロス画像のインフィル密度"
                fdmDescription: "画像ファイルの位置。この画像の輝度値でプリントのインフィル内の対象箇所における最小密度が決まります。"
            }
            FdmQml_Param{ id:cross_support_density_image; objectName: "cross_support_density_image"
                fdmLabel: "サポート用クロス画像のインフィル密度"
                fdmDescription: "画像ファイルの位置。この画像の輝度値でサポートの対象箇所における最小密度が決まります。"
            }
            FdmQml_Param{ id:support_conical_enabled; objectName: "support_conical_enabled"
                fdmLabel: "円錐サポートを有効にする"
                fdmDescription: "オーバーハング部分よりも底面の支持領域を小さくする。"
            }
            FdmQml_Param{ id:support_conical_angle; objectName: "support_conical_angle"
                fdmLabel: "円錐サポートの角度"
                fdmDescription: "円錐形のサポートの傾きの角度。 0度は垂直であり、90度は水平である。角度が小さいと、サポートはより頑丈になりますが、より多くのマテリアルが必要になります。負の角度は、サポートのベースがトップよりも広くなります。"
            }
            FdmQml_Param{ id:support_conical_min_width; objectName: "support_conical_min_width"
                fdmLabel: "円錐サポートの最大幅"
                fdmDescription: "円錐形のサポート領域のベースが縮小される最小幅。幅が狭いと、サポートが不安定になる可能性があります。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_enabled; objectName: "magic_fuzzy_skin_enabled"
                fdmLabel: "ファジースキン"
                fdmDescription: "外側ウォールをプリントする際に振動が起こり、表面が粗くてぼやける。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_outside_only; objectName: "magic_fuzzy_skin_outside_only"
                fdmLabel: "ファジースキン外のみ"
                fdmDescription: "部品の輪郭のみに振動が起こり、部品の穴には起こりません。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_thickness; objectName: "magic_fuzzy_skin_thickness"
                fdmLabel: "ファジースキンの厚さ"
                fdmDescription: "振動が起こる幅。内側ウォールは変更されていないので、これを外側ウォールの幅より小さく設定することをお勧めします。"
            }
            FdmQml_Param{ id:magic_fuzzy_skin_point_density; objectName: "magic_fuzzy_skin_point_density"
                fdmLabel: "ファジースキンの密度"
                fdmDescription: "レイヤー内の各ポリゴンに導入されたポイントの平均密度。ポリゴンの元の点は破棄されるため、密度が低いと解像度が低下します。"
                FdmQml_Param{ id:magic_fuzzy_skin_point_dist; objectName: "magic_fuzzy_skin_point_dist"
                    fdmLabel: "ファジースキン点間距離"
                    fdmDescription: "各線分に導入されたランダム点間の平均距離。ポリゴンの元の点は破棄されるので、積層の値を低くすることで、なめらかな仕上がりになります。この値は、ファジースキンの厚さの半分よりも大きくなければなりません。"
                }
            }
            FdmQml_Param{ id:flow_rate_max_extrusion_offset; objectName: "flow_rate_max_extrusion_offset"
                fdmLabel: "流量補正時の最大抽出オフセット"
                fdmDescription: "流量の変化を補正するためにフィラメントを移動する最大距離（mm）。"
            }
            FdmQml_Param{ id:flow_rate_extrusion_offset_factor; objectName: "flow_rate_extrusion_offset_factor"
                fdmLabel: "流量補正要因"
                fdmDescription: "流量の変化を補正するためにフィラメントを移動する距離。フィラメントが1秒の押出で移動する距離の割合として指定します。"
            }
            FdmQml_Param{ id:wireframe_enabled; objectName: "wireframe_enabled"
                fdmLabel: "ワイヤープリント"
                fdmDescription: "薄い空気中にプリントし、疎なウエブ構造で外面のみをプリントします。これは、上向きおよび斜め下向きの線を介して接続された所定のZ間隔でモデルの輪郭を水平にプリントすることによって実現される。"
            }
            FdmQml_Param{ id:wireframe_height; objectName: "wireframe_height"
                fdmLabel: "WPの高さ"
                fdmDescription: "2つの水平なパーツ間の、上向きおよび斜め下向きの線の高さ。これは、ネット構造の全体密度を決定します。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_roof_inset; objectName: "wireframe_roof_inset"
                fdmLabel: "WPルーフ距離のオフセット"
                fdmDescription: "ルーフから内側に輪郭を描くときの距離。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_printspeed; objectName: "wireframe_printspeed"
                fdmLabel: "WP速度"
                fdmDescription: "マテリアルを押し出すときにノズルが動く速度。ワイヤープリントのみに適用されます。"
                FdmQml_Param{ id:wireframe_printspeed_bottom; objectName: "wireframe_printspeed_bottom"
                    fdmLabel: "WP底面印字速度"
                    fdmDescription: "ブルドプラットフォームに接触する第1層のプリント速度。ワイヤープリントのみに適用されます。"
                }
                FdmQml_Param{ id:wireframe_printspeed_up; objectName: "wireframe_printspeed_up"
                    fdmLabel: "WP上向き印字速度"
                    fdmDescription: "薄い空気の中で上向きに線をプリントする速度。ワイヤープリントのみに適用されます。"
                }
                FdmQml_Param{ id:wireframe_printspeed_down; objectName: "wireframe_printspeed_down"
                    fdmLabel: "WP下向き印字速度"
                    fdmDescription: "斜め下方に線をプリントする速度。ワイヤープリントのみに適用されます。"
                }
                FdmQml_Param{ id:wireframe_printspeed_flat; objectName: "wireframe_printspeed_flat"
                    fdmLabel: "WP水平印字速度"
                    fdmDescription: "モデルの水平輪郭をプリントする速度。ワイヤープリントのみに適用されます。"
                }
            }
            FdmQml_Param{ id:wireframe_flow; objectName: "wireframe_flow"
                fdmLabel: "WPフロー"
                fdmDescription: "流れ補正：押出されたマテリアルの量はこの値の乗算になります。ワイヤープリントのみに適用されます。"
                FdmQml_Param{ id:wireframe_flow_connection; objectName: "wireframe_flow_connection"
                    fdmLabel: "WP接続フロー"
                    fdmDescription: "上下に動くときの吐出補正。ワイヤープリントのみに適用されます。"
                }
                FdmQml_Param{ id:wireframe_flow_flat; objectName: "wireframe_flow_flat"
                    fdmLabel: "WPフラットフロー"
                    fdmDescription: "フラットラインをプリントする際の吐出補正。ワイヤープリントのみに適用されます。"
                }
            }
            FdmQml_Param{ id:wireframe_top_delay; objectName: "wireframe_top_delay"
                fdmLabel: "WP上面遅延"
                fdmDescription: "上向きの線が硬くなるように、上向きの動きの後の時間を遅らせる。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_bottom_delay; objectName: "wireframe_bottom_delay"
                fdmLabel: "WP底面遅延"
                fdmDescription: "下降後の遅延時間。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_flat_delay; objectName: "wireframe_flat_delay"
                fdmLabel: "WP水平遅延"
                fdmDescription: "2つの水平セグメント間の遅延時間。このような遅延を挿入すると、前のレイヤーとの接着性が向上することがありますが、遅延が長すぎると垂れ下がりが発生します。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_up_half_speed; objectName: "wireframe_up_half_speed"
                fdmLabel: "WP低速移動距離"
                fdmDescription: "半分の速度で上方へ押し出される距離。過度にマテリアルを加熱することなく、前のレイヤーとの接着を作ります。ワイヤープリントにのみ適用されます。"
            }
            FdmQml_Param{ id:wireframe_top_jump; objectName: "wireframe_top_jump"
                fdmLabel: "WPノットサイズ"
                fdmDescription: "上向きの線の上端に小さな結び目を作成し、連続する水平レイヤーを接着力を高めます。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_fall_down; objectName: "wireframe_fall_down"
                fdmLabel: "WP落下距離"
                fdmDescription: "上向き押出後にマテリアルが落下する距離。この距離は補正される。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_drag_along; objectName: "wireframe_drag_along"
                fdmLabel: "WP引きづり距離"
                fdmDescription: "斜め下方への押出に伴い上向き押出しているマテリアルが引きずられる距離。この距離は補正される。ワイヤープリントのみに適用されます。"
            }
            FdmQml_Param{ id:wireframe_strategy; objectName: "wireframe_strategy"
                fdmLabel: "WPストラテジー"
                fdmDescription: "各接続ポイントで2つの連続したレイヤーが密着していることを確認するためのストラテジー。収縮すると上向きの線が正しい位置で硬化しますが、フィラメントの研削が行われる可能性があります。上向きの線の終わりに結び目をつけて接続する機会を増やし、線を冷やすことができます。ただし、プリント速度が遅くなることがあります。別の方法は、上向きの線の上端のたるみを補償することである。しかし、予測どおりにラインが必ずしも落ちるとは限りません。"
                fdmOptions:{
                    "compensate": "補正",
                    "knot": "ノット",
                    "retract": "引き戻し"
                }
            }
            FdmQml_Param{ id:wireframe_straight_before_down; objectName: "wireframe_straight_before_down"
                fdmLabel: "WP下向き直線ライン"
                fdmDescription: "水平方向の直線部分で覆われた斜めに下降線の割合です。これは上向きラインのほとんどのポイント、上部のたるみを防ぐことができます。ワイヤープリントにのみ適用されます。"
            }
            FdmQml_Param{ id:wireframe_roof_fall_down; objectName: "wireframe_roof_fall_down"
                fdmLabel: "WPルーフ落下距離"
                fdmDescription: "水平ルーフが ”薄い空気”にプリントされ落ちる距離。この距離は補正されています。ワイヤープリントに適用されます。"
            }
            FdmQml_Param{ id:wireframe_roof_drag_along; objectName: "wireframe_roof_drag_along"
                fdmLabel: "WPルーフ引きずり距離"
                fdmDescription: "ルーフの外側の輪郭に戻る際に引きずる内側ラインの終わり部分の距離。この距離は補正されていてワイヤープリントのみ適用されます。"
            }
            FdmQml_Param{ id:wireframe_roof_outer_delay; objectName: "wireframe_roof_outer_delay"
                fdmLabel: "WPルーフ外側処理時間"
                fdmDescription: "トップレイヤーにある穴の外側に掛ける時間。長い時間の方はより良い密着を得られます。ワイヤープリントにのみ適用されます。"
            }
            FdmQml_Param{ id:wireframe_nozzle_clearance; objectName: "wireframe_nozzle_clearance"
                fdmLabel: "WPノズル隙間"
                fdmDescription: "ノズルと水平方向に下向きの線間の距離。大きな隙間がある場合、急な角度で斜め下方線となり、次の層が上方接続しずらくなる。ワイヤープリントにのみ適用されます。"
            }
            FdmQml_Param{ id:adaptive_layer_height_enabled; objectName: "adaptive_layer_height_enabled"
                fdmLabel: "適応レイヤーの使用"
                fdmDescription: "適応レイヤーは、レイヤーの高さをモデルの形状に合わせて計算します。"
            }
            FdmQml_Param{ id:adaptive_layer_height_variation; objectName: "adaptive_layer_height_variation"
                fdmLabel: "適応レイヤー最大差分"
                fdmDescription: "基準レイヤー高さと比較して許容される最大の高さ。"
            }
            FdmQml_Param{ id:adaptive_layer_height_variation_step; objectName: "adaptive_layer_height_variation_step"
                fdmLabel: "適応レイヤー差分ステップサイズ"
                fdmDescription: "次のレイヤーの高さを前のレイヤーの高さと比べた差。"
            }
            FdmQml_Param{ id:adaptive_layer_height_threshold; objectName: "adaptive_layer_height_threshold"
                fdmLabel: "適応レイヤーのトポグラフィーサイズ"
                fdmDescription: "隣接する2つのレイヤー間の目標水平距離。この設定を小さくすると、レイヤーのエッジが近づくように薄いレイヤーが使用されます。"
            }
            FdmQml_Param{ id:clean_between_layers; objectName: "clean_between_layers"
                fdmLabel: "レイヤー間のノズル拭き取り"
                fdmDescription: "レイヤー間にノズル拭き取りG-Codeを含むかどうか（レイヤーごとに最大1つ）。この設定を有効にすると、レイヤー変更時の引き戻し動作に影響する可能性があります。拭き取りスクリプトが動作するレイヤーでの押し戻しを制御するには、ワイプ引き戻し設定を使用してください。"
            }
            FdmQml_Param{ id:max_extrusion_before_wipe; objectName: "max_extrusion_before_wipe"
                fdmLabel: "ワイプ間の材料の量"
                fdmDescription: "別のノズル拭き取りを行う前に押し出せる材料の最大量。この値がレイヤーに必要な材料の量よりも小さい場合、この設定はこのレイヤーには影響しません。つまり、レイヤーごとに1つの拭き取りに制限されます。"
            }
            FdmQml_Param{ id:wipe_retraction_enable; objectName: "wipe_retraction_enable"
                fdmLabel: "ワイプリトラクト有効"
                fdmDescription: "ノズルがプリントしないで良い領域を移動する際にフィラメントを引き戻す。"
            }
            FdmQml_Param{ id:wipe_retraction_amount; objectName: "wipe_retraction_amount"
                fdmLabel: "ワイプリトラクト無効"
                fdmDescription: "拭き取りシーケンス中に出ないように押し戻すフィラメントの量。"
            }
            FdmQml_Param{ id:wipe_retraction_extra_prime_amount; objectName: "wipe_retraction_extra_prime_amount"
                fdmLabel: "ワイプ引き戻し時の余分押し戻し量"
                fdmDescription: "いくつかの材料は、ワイプ移動中ににじみ出るためここで補償することができます。"
            }
            FdmQml_Param{ id:wipe_retraction_speed; objectName: "wipe_retraction_speed"
                fdmLabel: "ワイプリトラクト速度"
                fdmDescription: "ワイプ引き戻し中にフィラメントが引き戻される時の速度。"
                FdmQml_Param{ id:wipe_retraction_retract_speed; objectName: "wipe_retraction_retract_speed"
                    fdmLabel: "ワイプ引き戻し速度"
                    fdmDescription: "ワイプ引き戻し移動時にフィラメントが引き戻される速度。"
                }
                FdmQml_Param{ id:wipe_retraction_prime_speed; objectName: "wipe_retraction_prime_speed"
                    fdmLabel: "ワイプ引き戻し下準備速度"
                    fdmDescription: "ワイプ引き戻し移動時にフィラメントが押し戻されるスピード。"
                }
            }
            FdmQml_Param{ id:wipe_pause; objectName: "wipe_pause"
                fdmLabel: "ワイプ一時停止"
                fdmDescription: "引き戻し前に一時停止します。"
            }
            FdmQml_Param{ id:wipe_hop_enable; objectName: "wipe_hop_enable"
                fdmLabel: "ワイプZホップ"
                fdmDescription: "拭き取りの際、ビルドプレートが下降してノズルとプリントの間に隙間ができます。これは、ノズルの走行中にプリントに当たるのを防ぎ、プリントをビルドプレートから剥がしてしまう可能性を減らします。"
            }
            FdmQml_Param{ id:wipe_hop_amount; objectName: "wipe_hop_amount"
                fdmLabel: "ワイプZホップ高さ"
                fdmDescription: "Zホップを実行するときの高さ。"
            }
            FdmQml_Param{ id:wipe_hop_speed; objectName: "wipe_hop_speed"
                fdmLabel: "ワイプホップ速度"
                fdmDescription: "ホップ中に z 軸を移動する速度。"
            }
            FdmQml_Param{ id:wipe_brush_pos_x; objectName: "wipe_brush_pos_x"
                fdmLabel: "ワイプブラシXの位置"
                fdmDescription: "ワイプスクリプトを開始するX位置。"
            }
            FdmQml_Param{ id:wipe_repeat_count; objectName: "wipe_repeat_count"
                fdmLabel: "ワイプ繰り返し回数"
                fdmDescription: "ブラシ全体をノズルが移動する回数。"
            }
            FdmQml_Param{ id:wipe_move_distance; objectName: "wipe_move_distance"
                fdmLabel: "ワイプ移動距離"
                fdmDescription: "ブラシ全体でヘッド前後に動かす距離。"
            }
        }
        FdmQml_Category{ id:command_line_settings; objectName: "command_line_settings"
            fdmLabel: "コマンドライン設定"
            fdmDescription: "AnkerMakeエンジンがAnkerMakeフロントエンドから呼び出されない場合のみ使用される設定。"
            FdmQml_Param{ id:center_object; objectName: "center_object"
                fdmDescription: "オブジェクトが保存された座標系を使用する代わりにビルドプラットフォームの中間(0,0)にオブジェクトを配置するかどうか。"
                fdmLabel: "オブジェクト中心配置"
            }
            FdmQml_Param{ id:mesh_position_x; objectName: "mesh_position_x"
                fdmDescription: "オブジェクトの X 方向に適用されたオフセット。"
                fdmLabel: "メッシュ位置X"
            }
            FdmQml_Param{ id:mesh_position_y; objectName: "mesh_position_y"
                fdmDescription: "オブジェクトのY 方向適用されたオフセット。"
                fdmLabel: "メッシュ位置Y"
            }
            FdmQml_Param{ id:mesh_position_z; objectName: "mesh_position_z"
                fdmDescription: "オブジェクトの Z 方向に適用されたオフセット。この 'オブジェクト シンク' と呼ばれていたものを再現できます。"
                fdmLabel: "メッシュ位置Z"
            }
            FdmQml_Param{ id:mesh_rotation_matrix; objectName: "mesh_rotation_matrix"
                fdmLabel: "メッシュ回転マトリックス"
                fdmDescription: "ファイルから読み込むときに、モデルに適用するトランスフォーメーションマトリックス。"
            }
        }
    }
}
