//Copyright (c) 2021 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include "ZSeamConfig.h" //The definitions we're implementing.

namespace cura
{

ZSeamConfig::ZSeamConfig()
    : type(EZSeamType::SHORTEST)
    , pos(Point(0, 0))
    , corner_pref(EZSeamCornerPrefType::Z_SEAM_CORNER_PREF_NONE)
    , z_seam_min_angle_diff(36.f)
    , z_seam_max_angle(162.f)
{}

ZSeamConfig::ZSeamConfig(const EZSeamType type, const Point pos, const EZSeamCornerPrefType corner_pref, float z_seam_min_angle_diff, float z_seam_max_angle)
    : type(type)
    , pos(pos)
    , corner_pref(corner_pref)
    , z_seam_min_angle_diff(36.f)
    , z_seam_max_angle(162.f)
{}

}
