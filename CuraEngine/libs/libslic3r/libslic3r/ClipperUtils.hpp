#ifndef slic3r_ClipperUtils_hpp_
#define slic3r_ClipperUtils_hpp_

#include "libslic3r.h"
#include "clipper3r/clipper.hpp"
#include "ExPolygon.hpp"
#include "Polygon.hpp"
#include "Surface.hpp"

// import these wherever we're included
using Clipper3r::jtMiter;
using Clipper3r::jtRound;
using Clipper3r::jtSquare;

// Factor to convert from coord_t (which is int32) to an int64 type used by the Clipper library
// for general offsetting (the offset(), offset2(), offset_ex() functions) and for the safety offset,
// which is optionally executed by other functions (union, intersection, diff).
// This scaling (cca 130t) is applied over the usual SCALING_FACTOR.
// By the way, is the scalling for offset needed at all?
// The reason to apply this scaling may be to match the resolution of the double mantissa.
#define CLIPPER_OFFSET_POWER_OF_2 17
// 2^17=131072
#define CLIPPER_OFFSET_SCALE (1 << CLIPPER_OFFSET_POWER_OF_2)
#define CLIPPER_OFFSET_SCALE_ROUNDING_DELTA ((1 << (CLIPPER_OFFSET_POWER_OF_2 - 1)) - 1)
#define CLIPPER_MAX_COORD_UNSCALED (Clipper3r::hiRange / CLIPPER_OFFSET_SCALE)

namespace Slic3r {

//-----------------------------------------------------------
// legacy code from Clipper documentation
void AddOuterPolyNodeToExPolygons(Clipper3r::PolyNode& polynode, Slic3r::ExPolygons *expolygons);
Slic3r::ExPolygons PolyTreeToExPolygons(Clipper3r::PolyTree& polytree);
//-----------------------------------------------------------

Clipper3r::Path   Slic3rMultiPoint_to_ClipperPath(const Slic3r::MultiPoint &input);
Clipper3r::Paths  Slic3rMultiPoints_to_ClipperPaths(const Polygons &input);
Clipper3r::Paths  Slic3rMultiPoints_to_ClipperPaths(const ExPolygons &input);
Clipper3r::Paths  Slic3rMultiPoints_to_ClipperPaths(const Polylines &input);
Slic3r::Polygon    ClipperPath_to_Slic3rPolygon(const Clipper3r::Path &input);
Slic3r::Polyline   ClipperPath_to_Slic3rPolyline(const Clipper3r::Path &input);
Slic3r::Polygons   ClipperPaths_to_Slic3rPolygons(const Clipper3r::Paths &input);
Slic3r::Polylines  ClipperPaths_to_Slic3rPolylines(const Clipper3r::Paths &input);
Slic3r::ExPolygons ClipperPaths_to_Slic3rExPolygons(const Clipper3r::Paths &input);

// offset Polygons
Clipper3r::Paths _offset(Clipper3r::Path &&input, Clipper3r::EndType endType, const float delta, Clipper3r::JoinType joinType, double miterLimit);
Clipper3r::Paths _offset(Clipper3r::Paths &&input, Clipper3r::EndType endType, const float delta, Clipper3r::JoinType joinType, double miterLimit);
inline Slic3r::Polygons offset(const Slic3r::Polygon &polygon, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter,  double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(Slic3rMultiPoint_to_ClipperPath(polygon), Clipper3r::etClosedPolygon, delta, joinType, miterLimit)); }
inline Slic3r::Polygons offset(const Slic3r::Polygons &polygons, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(Slic3rMultiPoints_to_ClipperPaths(polygons), Clipper3r::etClosedPolygon, delta, joinType, miterLimit)); }

// offset Polylines
inline Slic3r::Polygons offset(const Slic3r::Polyline &polyline, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtSquare, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(Slic3rMultiPoint_to_ClipperPath(polyline), Clipper3r::etOpenButt, delta, joinType, miterLimit)); }
inline Slic3r::Polygons offset(const Slic3r::Polylines &polylines, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtSquare, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(Slic3rMultiPoints_to_ClipperPaths(polylines), Clipper3r::etOpenButt, delta, joinType, miterLimit)); }

// offset expolygons and surfaces
Clipper3r::Paths _offset(const Slic3r::ExPolygon &expolygon, const float delta, Clipper3r::JoinType joinType, double miterLimit);
Clipper3r::Paths _offset(const Slic3r::ExPolygons &expolygons, const float delta, Clipper3r::JoinType joinType, double miterLimit);
inline Slic3r::Polygons offset(const Slic3r::ExPolygon &expolygon, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(expolygon, delta, joinType, miterLimit)); }
inline Slic3r::Polygons offset(const Slic3r::ExPolygons &expolygons, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rPolygons(_offset(expolygons, delta, joinType, miterLimit)); }
inline Slic3r::ExPolygons offset_ex(const Slic3r::Polygon &polygon, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rExPolygons(_offset(Slic3rMultiPoint_to_ClipperPath(polygon), Clipper3r::etClosedPolygon, delta, joinType, miterLimit)); }    
inline Slic3r::ExPolygons offset_ex(const Slic3r::Polygons &polygons, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rExPolygons(_offset(Slic3rMultiPoints_to_ClipperPaths(polygons), Clipper3r::etClosedPolygon, delta, joinType, miterLimit)); }
inline Slic3r::ExPolygons offset_ex(const Slic3r::ExPolygon &expolygon, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rExPolygons(_offset(expolygon, delta, joinType, miterLimit)); }
inline Slic3r::ExPolygons offset_ex(const Slic3r::ExPolygons &expolygons, const float delta, Clipper3r::JoinType joinType = Clipper3r::jtMiter, double miterLimit = 3)
    { return ClipperPaths_to_Slic3rExPolygons(_offset(expolygons, delta, joinType, miterLimit)); }

Clipper3r::Paths _offset2(const Slic3r::Polygons &polygons, const float delta1,
    const float delta2, Clipper3r::JoinType joinType = Clipper3r::jtMiter, 
    double miterLimit = 3);
Slic3r::Polygons offset2(const Slic3r::Polygons &polygons, const float delta1,
    const float delta2, Clipper3r::JoinType joinType = Clipper3r::jtMiter, 
    double miterLimit = 3);
Slic3r::ExPolygons offset2_ex(const Slic3r::Polygons &polygons, const float delta1,
    const float delta2, Clipper3r::JoinType joinType = Clipper3r::jtMiter, 
    double miterLimit = 3);
Slic3r::ExPolygons offset2_ex(const Slic3r::ExPolygons &expolygons, const float delta1,
    const float delta2, Clipper3r::JoinType joinType = Clipper3r::jtMiter, 
    double miterLimit = 3);

Slic3r::Polygons _clipper(Clipper3r::ClipType clipType,
    const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false);
Slic3r::ExPolygons _clipper_ex(Clipper3r::ClipType clipType,
    const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false);
Slic3r::Polylines _clipper_pl(Clipper3r::ClipType clipType,
    const Slic3r::Polylines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false);
Slic3r::Polylines _clipper_pl(Clipper3r::ClipType clipType,
    const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false);
Slic3r::Lines _clipper_ln(Clipper3r::ClipType clipType,
    const Slic3r::Lines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false);

// diff
inline Slic3r::Polygons
diff(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctDifference, subject, clip, safety_offset_);
}

inline Slic3r::ExPolygons
diff_ex(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctDifference, subject, clip, safety_offset_);
}

inline Slic3r::ExPolygons
diff_ex(const Slic3r::ExPolygons &subject, const Slic3r::ExPolygons &clip, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctDifference, to_polygons(subject), to_polygons(clip), safety_offset_);
}

inline Slic3r::Polygons
diff(const Slic3r::ExPolygons &subject, const Slic3r::ExPolygons &clip, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctDifference, to_polygons(subject), to_polygons(clip), safety_offset_);
}

inline Slic3r::Polylines
diff_pl(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_pl(Clipper3r::ctDifference, subject, clip, safety_offset_);
}

inline Slic3r::Polylines
diff_pl(const Slic3r::Polylines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_pl(Clipper3r::ctDifference, subject, clip, safety_offset_);
}

inline Slic3r::Lines
diff_ln(const Slic3r::Lines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_ln(Clipper3r::ctDifference, subject, clip, safety_offset_);
}

// intersection
inline Slic3r::Polygons
intersection(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctIntersection, subject, clip, safety_offset_);
}

inline Slic3r::ExPolygons
intersection_ex(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctIntersection, subject, clip, safety_offset_);
}

inline Slic3r::ExPolygons
intersection_ex(const Slic3r::ExPolygons &subject, const Slic3r::ExPolygons &clip, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctIntersection, to_polygons(subject), to_polygons(clip), safety_offset_);
}

inline Slic3r::Polygons
intersection(const Slic3r::ExPolygons &subject, const Slic3r::ExPolygons &clip, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctIntersection, to_polygons(subject), to_polygons(clip), safety_offset_);
}

inline Slic3r::Polylines
intersection_pl(const Slic3r::Polygons &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_pl(Clipper3r::ctIntersection, subject, clip, safety_offset_);
}

inline Slic3r::Polylines
intersection_pl(const Slic3r::Polylines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_pl(Clipper3r::ctIntersection, subject, clip, safety_offset_);
}

inline Slic3r::Lines intersection_ln(const Slic3r::Lines &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    return _clipper_ln(Clipper3r::ctIntersection, subject, clip, safety_offset_);
}

inline Slic3r::Lines intersection_ln(const Slic3r::Line &subject, const Slic3r::Polygons &clip, bool safety_offset_ = false)
{
    Slic3r::Lines lines;
    lines.emplace_back(subject);
    return _clipper_ln(Clipper3r::ctIntersection, lines, clip, safety_offset_);
}

// union
inline Slic3r::Polygons union_(const Slic3r::Polygons &subject, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctUnion, subject, Slic3r::Polygons(), safety_offset_);
}

inline Slic3r::Polygons union_(const Slic3r::Polygons &subject, const Slic3r::Polygons &subject2, bool safety_offset_ = false)
{
    return _clipper(Clipper3r::ctUnion, subject, subject2, safety_offset_);
}

inline Slic3r::ExPolygons union_ex(const Slic3r::Polygons &subject, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctUnion, subject, Slic3r::Polygons(), safety_offset_);
}

inline Slic3r::ExPolygons union_ex(const Slic3r::ExPolygons &subject, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctUnion, to_polygons(subject), Slic3r::Polygons(), safety_offset_);
}

inline Slic3r::ExPolygons union_ex(const Slic3r::Surfaces &subject, bool safety_offset_ = false)
{
    return _clipper_ex(Clipper3r::ctUnion, to_polygons(subject), Slic3r::Polygons(), safety_offset_);
}

Clipper3r::PolyTree union_pt(const Slic3r::Polygons &subject, bool safety_offset_ = false);
Clipper3r::PolyTree union_pt(const Slic3r::ExPolygons &subject, bool safety_offset_ = false);
Clipper3r::PolyTree union_pt(Slic3r::Polygons &&subject, bool safety_offset_ = false);
Clipper3r::PolyTree union_pt(Slic3r::ExPolygons &&subject, bool safety_offset_ = false);

Slic3r::Polygons union_pt_chained_outside_in(const Slic3r::Polygons &subject, bool safety_offset_ = false);

Clipper3r::PolyNodes order_nodes(const Clipper3r::PolyNodes &nodes);

// Implementing generalized loop (foreach) over a list of nodes which can be
// ordered or unordered (performance gain) based on template parameter
enum class e_ordering {
    ON,
    OFF
};

// Create a template struct, template functions can not be partially specialized
template<e_ordering o, class Fn> struct _foreach_node {
    void operator()(const Clipper3r::PolyNodes &nodes, Fn &&fn);
};

// Specialization with NO ordering
template<class Fn> struct _foreach_node<e_ordering::OFF, Fn> {
    void operator()(const Clipper3r::PolyNodes &nodes, Fn &&fn)
    {
        for (auto &n : nodes) fn(n);    
    }
};

// Specialization with ordering
template<class Fn> struct _foreach_node<e_ordering::ON, Fn> {
    void operator()(const Clipper3r::PolyNodes &nodes, Fn &&fn)
    {
        auto ordered_nodes = order_nodes(nodes);
        for (auto &n : nodes) fn(n);    
    }
};

// Wrapper function for the foreach_node which can deduce arguments automatically
template<e_ordering o, class Fn>
void foreach_node(const Clipper3r::PolyNodes &nodes, Fn &&fn)
{
    _foreach_node<o, Fn>()(nodes, std::forward<Fn>(fn));
}

// Collecting polygons of the tree into a list of Polygons, holes have clockwise
// orientation.
template<e_ordering ordering = e_ordering::OFF>
void traverse_pt(const Clipper3r::PolyNode *tree, Polygons *out)
{
    if (!tree) return; // terminates recursion
    
    // Push the contour of the current level
    out->emplace_back(ClipperPath_to_Slic3rPolygon(tree->Contour));
    
    // Do the recursion for all the children.
    traverse_pt<ordering>(tree->Childs, out);
}

// Collecting polygons of the tree into a list of ExPolygons.
template<e_ordering ordering = e_ordering::OFF>
void traverse_pt(const Clipper3r::PolyNode *tree, ExPolygons *out)
{
    if (!tree) return;
    else if(tree->IsHole()) {
        // Levels of holes are skipped and handled together with the
        // contour levels.
        traverse_pt<ordering>(tree->Childs, out);
        return;
    }
    
    ExPolygon level;
    level.contour = ClipperPath_to_Slic3rPolygon(tree->Contour);
    
    foreach_node<ordering>(tree->Childs, 
                           [out, &level] (const Clipper3r::PolyNode *node) {
        
        // Holes are collected here. 
        level.holes.emplace_back(ClipperPath_to_Slic3rPolygon(node->Contour));
        
        // By doing a recursion, a new level expoly is created with the contour
        // and holes of the lower level. Doing this for all the childs.
        traverse_pt<ordering>(node->Childs, out);
    }); 
    
    out->emplace_back(level);
}

template<e_ordering o = e_ordering::OFF, class ExOrJustPolygons>
void traverse_pt(const Clipper3r::PolyNodes &nodes, ExOrJustPolygons *retval)
{
    foreach_node<o>(nodes, [&retval](const Clipper3r::PolyNode *node) {
        traverse_pt<o>(node, retval);
    });
}


/* OTHER */
Slic3r::Polygons simplify_polygons(const Slic3r::Polygons &subject, bool preserve_collinear = false);
Slic3r::ExPolygons simplify_polygons_ex(const Slic3r::Polygons &subject, bool preserve_collinear = false);

void safety_offset(Clipper3r::Paths* paths);

Polygons top_level_islands(const Slic3r::Polygons &polygons);

Polygons  variable_offset_inner(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit = 2.);
Polygons  variable_offset_outer(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit = 2.);
ExPolygons variable_offset_outer_ex(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit = 2.);
ExPolygons variable_offset_inner_ex(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit = 2.);

}

#endif
