#include "SupportTreeNode.h"
#include <libslic3r/MTUtils.hpp>

namespace Slic3r
{
	namespace sla
	{
        Head::Head(double       r_big_mm,
            double       r_small_mm,
            double       length_mm,
            double       penetration,
            const Vec3d& direction,
            const Vec3d& offset)
            : dir(direction)
            , pos(offset)
            , r_back_mm(r_big_mm)
            , r_pin_mm(r_small_mm)
            , width_mm(length_mm)
            , penetration_mm(penetration)
        {
            treeNodeType = MeshTypeElement::Head;
        }
        Anchor::Anchor(double       r_big_mm,
            double       r_small_mm,
            double       length_mm,
            double       penetration,
            const Vec3d& direction,
            const Vec3d& offset)
            : dir(direction)
            , pos(offset)
            , r_back_mm(r_big_mm)
            , r_pin_mm(r_small_mm)
            , width_mm(length_mm)
            , penetration_mm(penetration)
        {
            treeNodeType = MeshTypeElement::Anchor;

        }
        Pad::Pad(const TriangleMesh& support_mesh,
            const ExPolygons& model_contours,
            double              ground_level,
            const PadConfig& pcfg,
            ThrowOnCancel       thr)
            : cfg(pcfg)
            //, zlevel(ground_level + pcfg.full_height() - pcfg.required_elevation())
            , zlevel(ground_level)
        {
            thr();

            ExPolygons sup_contours;

            float zstart = float(zlevel);
            float zend = zstart + float(pcfg.full_height() - EPSILON);
            pad_blueprint(support_mesh, sup_contours, grid(zstart, zend, (zend - zstart) / 3), thr);
            if (!support_mesh.empty())
            {
                create_pad(sup_contours, model_contours, tmesh, pcfg);
            }
            else
            {
                ExPolygons empty_model_contours;
                create_pad(sup_contours, empty_model_contours, tmesh, pcfg);

            }

            tmesh.translate(0, 0, float(zlevel) + pcfg.full_height());
            if (!tmesh.empty()) tmesh.require_shared_vertices();
        }
	}
}
