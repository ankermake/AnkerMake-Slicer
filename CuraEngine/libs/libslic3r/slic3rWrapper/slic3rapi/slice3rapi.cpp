#include"slice3rapi.h"

namespace cura {

    Slic3r::Polygon ClipperPath_to_Slic3rPolygon(const ClipperLib::Path& input)
    {
        Slic3r::Polygon retval;
        for (ClipperLib::Path::const_iterator pit = input.begin(); pit != input.end(); ++pit)
            retval.points.emplace_back(pit->X, pit->Y);
        return std::move(retval);
    }

    // legacy code from Clipper documentation
    void AddOuterPolyNodeToExPolygons(ClipperLib::PolyNode& polynode, Slic3r::ExPolygons* expolygons)
    {
        size_t cnt = expolygons->size();
        expolygons->resize(cnt + 1);
        (*expolygons)[cnt].contour = ClipperPath_to_Slic3rPolygon(polynode.Contour);
        (*expolygons)[cnt].holes.resize(polynode.ChildCount());
        for (int i = 0; i < polynode.ChildCount(); ++i)
        {
            (*expolygons)[cnt].holes[i] = ClipperPath_to_Slic3rPolygon(polynode.Childs[i]->Contour);
            //Add outer polygons contained by (nested within) holes ...
            for (int j = 0; j < polynode.Childs[i]->ChildCount(); ++j)
                AddOuterPolyNodeToExPolygons(*polynode.Childs[i]->Childs[j], expolygons);
        }
    }

    Slic3r::ExPolygons PolyTreeToExPolygons(ClipperLib::PolyTree& polytree)
    {
        Slic3r::ExPolygons retval;
        for (int i = 0; i < polytree.ChildCount(); ++i)
            AddOuterPolyNodeToExPolygons(*polytree.Childs[i], &retval);
        return std::move(retval);
    }

    Slic3r::ExPolygons ClipperPaths_to_Slic3rExPolygons(const ClipperLib::Paths& input)
    {
        // init Clipper
        ClipperLib::Clipper clipper;
        clipper.Clear();

        // perform union
        clipper.AddPaths(input, ClipperLib::ptSubject, true);
        ClipperLib::PolyTree polytree;
        clipper.Execute(ClipperLib::ctUnion, polytree, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);  // offset results work with both EvenOdd and NonZero

        // write to ExPolygons object
        return std::move(PolyTreeToExPolygons(polytree));
    }
}
