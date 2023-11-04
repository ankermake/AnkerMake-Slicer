#ifndef SLIC3R_SUPPORTTREEHEADER_1637566127928_H
#define SLIC3R_SUPPORTTREEHEADER_1637566127928_H
#include <Eigen/Geometry>
#include <libslic3r/SLA/IndexedMesh.hpp>
#include <libslic3r/SLA/SupportPoint.hpp>
#include <libslic3r/SLA/Pad.hpp>
#include <math.h>

namespace Slic3r
{
    namespace sla
    {
        enum SupportPosType
        {
            SUPPORT_NO = 0,
            SUPPORT_VERTEX = 0x01,
            SUPPORT_EDGE = 0x01 << 1,
            SUPPORT_FACE = 0x01 << 2,
        };

        enum class PillarConnectionMode
        {
            zigzag,
            cross,
            dynamic
        };

        struct SupportTreeConfig
        {
            bool   enabled = true;
            bool   enabled_pad = true;
            // Radius in mm of the pointing side of the head.
            double head_front_touch_radius_mm = 0.2;
            // Enable head front extern touch obj
            bool  head_front_touch_enable = false;

            // Radius in mm of the pointing side of the head.
            double head_front_radius_mm = 0.2;

            // How much the pinhead has to penetrate the model surface
            double head_penetration_mm = 0.1;

            // Radius of the back side of the 3d arrow.
            double head_back_radius_mm = 0.5;

            double head_fallback_radius_mm = 0.25;

            // Width in mm from the back sphere center to the front sphere center.
            double head_width_mm = 2.0;

            double head_width_min_mm = 0.1;//最小长度
            // How to connect pillars
            PillarConnectionMode pillar_connection_mode = PillarConnectionMode::zigzag;

            // Only generate pillars that can be routed to ground
            bool ground_facing_only = false;

            // TODO: unimplemented at the moment. This coefficient will have an impact
            // when bridges and pillars are merged. The resulting pillar should be a bit
            // thicker than the ones merging into it. How much thicker? I don't know
            // but it will be derived from this value.
            double pillar_widening_factor = 0.5;

            // Radius in mm of the pillar base.
            double base_top_radius_mm = 2.0;
            // Radius in mm of the pillar base.
            double base_radius_mm = 2.0;

            // The height of the pillar base cone in mm.
            double base_height_mm = 1.0;

            // Radius in mm of the pillar base.
            double squarebase_top_radius_mm = 3.0;
            // Radius in mm of the pillar base.
            double squarebase_radius_mm = 3.0;

            // The height of the pillar base cone in mm.
            double squarebase_height_mm = 1.0;

            // The default angle for connecting support sticks and junctions.
            double bridge_slope = M_PI / 4;

            // The max length of a bridge in mm
            double max_bridge_length_mm = 10.0;
            // The min  height of a bridge where is created begin in mm
            double min_bridge_height_mm = 3.0;

            // The max distance of a pillar to pillar link.
            double max_pillar_link_distance_mm = 10.0;

            // The elevation in Z direction upwards. This is the space between the pad
            // and the model object's bounding box bottom.
            double object_elevation_mm = 10;

            // The shortest distance between a pillar base perimeter from the model
            // body. This is only useful when elevation is set to zero.
            double pillar_base_safety_distance_mm = 0.5;

            unsigned max_bridges_on_pillar = 3;

            double head_fullwidth() const {
                return 2 * head_front_radius_mm + head_width_mm +
                    2 * head_back_radius_mm - head_penetration_mm;
            }

            // /////////////////////////////////////////////////////////////////////////
            // Compile time configuration values (candidates for runtime)
            // /////////////////////////////////////////////////////////////////////////

            // The max Z angle for a normal at which it will get completely ignored.
            static const double constexpr normal_cutoff_angle = 150.0 * M_PI / 180.0;

            static const double constexpr normal_move_cutoff_angle = 100.0 * M_PI / 180.0;
            // The shortest distance of any support structure from the model surface
            static const double constexpr safety_distance_mm = 0.5;

            static const double constexpr max_pillar_scaled_height_mm = 20.0;//加粗主支撑的最小高度阙值

            static const double constexpr max_solo_pillar_height_mm = 15.0;
            static const double constexpr max_dual_pillar_height_mm = 35.0;
            static const double constexpr optimizer_rel_score_diff = 1e-6;
#ifdef _DEBUG
            unsigned  optimizer_max_iterations = 10;//1000;
#else
            unsigned  optimizer_max_iterations = 100;//1000;
#endif
            static const unsigned constexpr pillar_cascade_neighbors = 2;

            static const unsigned constexpr simple_cross_split = 5;
            static const unsigned constexpr max_bridge_distance = 8;

        };

        enum class MeshType { Support, Pad };
        enum class MeshTypeElement {
            BaseType,
            Head,
            Pillar,
            Junction,
            Bridge,
            CrossBridge,
            DiffBridge,
            Pedestal,
            Anchor,
            Pad,
            SquarePedestal,
            TypeMax
        };

        struct MoveOffSet
        {
            Vec3d ptStart;
            Vec3d ptEnd;
        };
        enum class SUPPORT_AJUST_EVENT {
            EVENT_DELETE = 0x01,
            EVENT_MOVE = 0x01 << 1,
            EVENT_ADD = 0x01 << 2,
            EVENT_NONE = 0xff
        };
        enum class SUPPORT_EVENT_FLAG {
            EVENT_FLAG_NONE = 0x00,
            EVENT_FLAG_MOVE_START = 0x01,
            EVENT_FLAG_MOVE_MOVE = 0x01 << 1,
            EVENT_FLAG_MOVE_END = 0x01 << 2, 
        };
        struct SupportTreeNode2MeshLink
        {
            long id = -1;
            MeshTypeElement type = MeshTypeElement::BaseType;
            std::shared_ptr<TriangleMesh> triangleMesh = NULL;
            SUPPORT_EVENT_FLAG  eventFlg = SUPPORT_EVENT_FLAG::EVENT_FLAG_NONE;
        };

        template<class Vec> double distance(const Vec& p) {
            return std::sqrt(p.transpose() * p);
        }

        template<class Vec> double distance(const Vec& pp1, const Vec& pp2) {
            auto p = pp2 - pp1;
            return distance(p);
        }

        const Vec3d DOWN = { 0.0, 0.0, -1.0 };

    }
}

#endif // SLIC3R_SUPPORTTREEHEADER_1637566127928_H