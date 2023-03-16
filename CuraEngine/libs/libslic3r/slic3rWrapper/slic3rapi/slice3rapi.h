#include"libslic3r/ExPolygon.hpp"
#include"clipper.hpp"

namespace cura {

    Slic3r::Polygon ClipperPath_to_Slic3rPolygon(const ClipperLib::Path& input);

    // legacy code from Clipper documentation
    void AddOuterPolyNodeToExPolygons(ClipperLib::PolyNode& polynode, Slic3r::ExPolygons* expolygons);

    Slic3r::ExPolygons PolyTreeToExPolygons(ClipperLib::PolyTree& polytree);

    Slic3r::ExPolygons ClipperPaths_to_Slic3rExPolygons(const ClipperLib::Paths& input);
}
