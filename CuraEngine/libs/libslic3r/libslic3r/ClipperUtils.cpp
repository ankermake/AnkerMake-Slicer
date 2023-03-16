#include "ClipperUtils.hpp"
#include "Geometry.hpp"
#include "ShortestPath.hpp"

// #define CLIPPER_UTILS_DEBUG

#ifdef CLIPPER_UTILS_DEBUG
#include "SVG.hpp"
#endif /* CLIPPER_UTILS_DEBUG */

// Profiling support using the Shiny intrusive profiler
//#define CLIPPER_UTILS_PROFILE
#if defined(SLIC3R_PROFILE) && defined(CLIPPER_UTILS_PROFILE)
	#include <Shiny/Shiny.h>
	#define CLIPPERUTILS_PROFILE_FUNC() PROFILE_FUNC()
	#define CLIPPERUTILS_PROFILE_BLOCK(name) PROFILE_BLOCK(name)
#else
	#define CLIPPERUTILS_PROFILE_FUNC()
	#define CLIPPERUTILS_PROFILE_BLOCK(name)
#endif

#define CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR (0.005f)

namespace Slic3r {

#ifdef CLIPPER_UTILS_DEBUG
bool clipper_export_enabled = false;
// For debugging the Clipper library, for providing bug reports to the Clipper author.
bool export_clipper_input_polygons_bin(const char *path, const Clipper3r::Paths &input_subject, const Clipper3r::Paths &input_clip)
{
    FILE *pfile = fopen(path, "wb");
    if (pfile == NULL)
        return false;

    uint32_t sz = uint32_t(input_subject.size());
    fwrite(&sz, 1, sizeof(sz), pfile);
    for (size_t i = 0; i < input_subject.size(); ++i) {
        const Clipper3r::Path &path = input_subject[i];
        sz = uint32_t(path.size());
        ::fwrite(&sz, 1, sizeof(sz), pfile);
        ::fwrite(path.data(), sizeof(Clipper3r::IntPoint), sz, pfile);
    }
    sz = uint32_t(input_clip.size());
    ::fwrite(&sz, 1, sizeof(sz), pfile);
    for (size_t i = 0; i < input_clip.size(); ++i) {
        const Clipper3r::Path &path = input_clip[i];
        sz = uint32_t(path.size());
        ::fwrite(&sz, 1, sizeof(sz), pfile);
        ::fwrite(path.data(), sizeof(Clipper3r::IntPoint), sz, pfile);
    }
    ::fclose(pfile);
    return true;

err:
    ::fclose(pfile);
    return false;
}
#endif /* CLIPPER_UTILS_DEBUG */

void scaleClipperPolygon(Clipper3r::Path &polygon)
{
    CLIPPERUTILS_PROFILE_FUNC();
    for (Clipper3r::Path::iterator pit = polygon.begin(); pit != polygon.end(); ++pit) {
        pit->X <<= CLIPPER_OFFSET_POWER_OF_2;
        pit->Y <<= CLIPPER_OFFSET_POWER_OF_2;
    }
}

void scaleClipperPolygons(Clipper3r::Paths &polygons)
{
    CLIPPERUTILS_PROFILE_FUNC();
    for (Clipper3r::Paths::iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (Clipper3r::Path::iterator pit = (*it).begin(); pit != (*it).end(); ++pit) {
            pit->X <<= CLIPPER_OFFSET_POWER_OF_2;
            pit->Y <<= CLIPPER_OFFSET_POWER_OF_2;
        }
}

void unscaleClipperPolygon(Clipper3r::Path &polygon)
{
    CLIPPERUTILS_PROFILE_FUNC();
    for (Clipper3r::Path::iterator pit = polygon.begin(); pit != polygon.end(); ++pit) {
        pit->X += CLIPPER_OFFSET_SCALE_ROUNDING_DELTA;
        pit->Y += CLIPPER_OFFSET_SCALE_ROUNDING_DELTA;
        pit->X >>= CLIPPER_OFFSET_POWER_OF_2;
        pit->Y >>= CLIPPER_OFFSET_POWER_OF_2;
    }
}

void unscaleClipperPolygons(Clipper3r::Paths &polygons)
{
    CLIPPERUTILS_PROFILE_FUNC();
    for (Clipper3r::Paths::iterator it = polygons.begin(); it != polygons.end(); ++it)
        for (Clipper3r::Path::iterator pit = (*it).begin(); pit != (*it).end(); ++pit) {
            pit->X += CLIPPER_OFFSET_SCALE_ROUNDING_DELTA;
            pit->Y += CLIPPER_OFFSET_SCALE_ROUNDING_DELTA;
            pit->X >>= CLIPPER_OFFSET_POWER_OF_2;
            pit->Y >>= CLIPPER_OFFSET_POWER_OF_2;
        }
}

//-----------------------------------------------------------
// legacy code from Clipper documentation
void AddOuterPolyNodeToExPolygons(Clipper3r::PolyNode& polynode, ExPolygons* expolygons)
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
 
ExPolygons PolyTreeToExPolygons(Clipper3r::PolyTree& polytree)
{
    ExPolygons retval;
    for (int i = 0; i < polytree.ChildCount(); ++i)
        AddOuterPolyNodeToExPolygons(*polytree.Childs[i], &retval);
    return retval;
}
//-----------------------------------------------------------

Slic3r::Polygon ClipperPath_to_Slic3rPolygon(const Clipper3r::Path &input)
{
    Polygon retval;
    for (Clipper3r::Path::const_iterator pit = input.begin(); pit != input.end(); ++pit)
        retval.points.emplace_back(pit->X, pit->Y);
    return retval;
}

Slic3r::Polyline ClipperPath_to_Slic3rPolyline(const Clipper3r::Path &input)
{
    Polyline retval;
    for (Clipper3r::Path::const_iterator pit = input.begin(); pit != input.end(); ++pit)
        retval.points.emplace_back(pit->X, pit->Y);
    return retval;
}

Slic3r::Polygons ClipperPaths_to_Slic3rPolygons(const Clipper3r::Paths &input)
{
    Slic3r::Polygons retval;
    retval.reserve(input.size());
    for (Clipper3r::Paths::const_iterator it = input.begin(); it != input.end(); ++it)
        retval.emplace_back(ClipperPath_to_Slic3rPolygon(*it));
    return retval;
}

Slic3r::Polylines ClipperPaths_to_Slic3rPolylines(const Clipper3r::Paths &input)
{
    Slic3r::Polylines retval;
    retval.reserve(input.size());
    for (Clipper3r::Paths::const_iterator it = input.begin(); it != input.end(); ++it)
        retval.emplace_back(ClipperPath_to_Slic3rPolyline(*it));
    return retval;
}

ExPolygons ClipperPaths_to_Slic3rExPolygons(const Clipper3r::Paths &input)
{
    // init Clipper
    Clipper3r::Clipper clipper;
    clipper.Clear();
    
    // perform union
    clipper.AddPaths(input, Clipper3r::ptSubject, true);
    Clipper3r::PolyTree polytree;
    clipper.Execute(Clipper3r::ctUnion, polytree, Clipper3r::pftEvenOdd, Clipper3r::pftEvenOdd);  // offset results work with both EvenOdd and NonZero
    
    // write to ExPolygons object
    return PolyTreeToExPolygons(polytree);
}

Clipper3r::Path Slic3rMultiPoint_to_ClipperPath(const MultiPoint &input)
{
    Clipper3r::Path retval;
    for (Points::const_iterator pit = input.points.begin(); pit != input.points.end(); ++pit)
        retval.emplace_back((*pit)(0), (*pit)(1));
    return retval;
}

Clipper3r::Path Slic3rMultiPoint_to_ClipperPath_reversed(const Slic3r::MultiPoint &input)
{
    Clipper3r::Path output;
    output.reserve(input.points.size());
    for (Slic3r::Points::const_reverse_iterator pit = input.points.rbegin(); pit != input.points.rend(); ++pit)
        output.emplace_back((*pit)(0), (*pit)(1));
    return output;
}

Clipper3r::Paths Slic3rMultiPoints_to_ClipperPaths(const Polygons &input)
{
    Clipper3r::Paths retval;
    for (Polygons::const_iterator it = input.begin(); it != input.end(); ++it)
        retval.emplace_back(Slic3rMultiPoint_to_ClipperPath(*it));
    return retval;
}

Clipper3r::Paths  Slic3rMultiPoints_to_ClipperPaths(const ExPolygons &input)
{
    Clipper3r::Paths retval;
    for (auto &ep : input) {
        retval.emplace_back(Slic3rMultiPoint_to_ClipperPath(ep.contour));
        
        for (auto &h : ep.holes)
            retval.emplace_back(Slic3rMultiPoint_to_ClipperPath(h));
    }
        
    return retval;
}

Clipper3r::Paths Slic3rMultiPoints_to_ClipperPaths(const Polylines &input)
{
    Clipper3r::Paths retval;
    for (Polylines::const_iterator it = input.begin(); it != input.end(); ++it)
        retval.emplace_back(Slic3rMultiPoint_to_ClipperPath(*it));
    return retval;
}

Clipper3r::Paths _offset(Clipper3r::Paths &&input, Clipper3r::EndType endType, const float delta, Clipper3r::JoinType joinType, double miterLimit)
{
    // scale input
    scaleClipperPolygons(input);
    
    // perform offset
    Clipper3r::ClipperOffset co;
    if (joinType == jtRound)
        co.ArcTolerance = miterLimit;
    else
        co.MiterLimit = miterLimit;
    float delta_scaled = delta * float(CLIPPER_OFFSET_SCALE);
    co.ShortestEdgeLength = double(std::abs(delta_scaled * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR));
    co.AddPaths(input, joinType, endType);
    Clipper3r::Paths retval;
    co.Execute(retval, delta_scaled);
    
    // unscale output
    unscaleClipperPolygons(retval);
    return retval;
}

Clipper3r::Paths _offset(Clipper3r::Path &&input, Clipper3r::EndType endType, const float delta, Clipper3r::JoinType joinType, double miterLimit)
{
    Clipper3r::Paths paths;
    paths.emplace_back(std::move(input));
	return _offset(std::move(paths), endType, delta, joinType, miterLimit);
}

// This is a safe variant of the polygon offset, tailored for a single ExPolygon:
// a single polygon with multiple non-overlapping holes.
// Each contour and hole is offsetted separately, then the holes are subtracted from the outer contours.
Clipper3r::Paths _offset(const Slic3r::ExPolygon &expolygon, const float delta,
    Clipper3r::JoinType joinType, double miterLimit)
{
//    printf("new ExPolygon offset\n");
    // 1) Offset the outer contour.
    const float delta_scaled = delta * float(CLIPPER_OFFSET_SCALE);
    Clipper3r::Paths contours;
    {
        Clipper3r::Path input = Slic3rMultiPoint_to_ClipperPath(expolygon.contour);
        scaleClipperPolygon(input);
        Clipper3r::ClipperOffset co;
        if (joinType == jtRound)
            co.ArcTolerance = miterLimit * double(CLIPPER_OFFSET_SCALE);
        else
            co.MiterLimit = miterLimit;
        co.ShortestEdgeLength = double(std::abs(delta_scaled * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR));
        co.AddPath(input, joinType, Clipper3r::etClosedPolygon);
        co.Execute(contours, delta_scaled);
    }

    // 2) Offset the holes one by one, collect the results.
    Clipper3r::Paths holes;
    {
        holes.reserve(expolygon.holes.size());
        for (Polygons::const_iterator it_hole = expolygon.holes.begin(); it_hole != expolygon.holes.end(); ++ it_hole) {
            Clipper3r::Path input = Slic3rMultiPoint_to_ClipperPath_reversed(*it_hole);
            scaleClipperPolygon(input);
            Clipper3r::ClipperOffset co;
            if (joinType == jtRound)
                co.ArcTolerance = miterLimit * double(CLIPPER_OFFSET_SCALE);
            else
                co.MiterLimit = miterLimit;
            co.ShortestEdgeLength = double(std::abs(delta_scaled * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR));
            co.AddPath(input, joinType, Clipper3r::etClosedPolygon);
            Clipper3r::Paths out;
            co.Execute(out, - delta_scaled);
            holes.insert(holes.end(), out.begin(), out.end());
        }
    }

    // 3) Subtract holes from the contours.
    Clipper3r::Paths output;
    if (holes.empty()) {
        output = std::move(contours);
    } else {
        Clipper3r::Clipper clipper;
        clipper.Clear();
        clipper.AddPaths(contours, Clipper3r::ptSubject, true);
        clipper.AddPaths(holes, Clipper3r::ptClip, true);
        clipper.Execute(Clipper3r::ctDifference, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
    }
    
    // 4) Unscale the output.
    unscaleClipperPolygons(output);
    return output;
}

// This is a safe variant of the polygons offset, tailored for multiple ExPolygons.
// It is required, that the input expolygons do not overlap and that the holes of each ExPolygon don't intersect with their respective outer contours.
// Each ExPolygon is offsetted separately, then the offsetted ExPolygons are united.
Clipper3r::Paths _offset(const Slic3r::ExPolygons &expolygons, const float delta,
    Clipper3r::JoinType joinType, double miterLimit)
{
    const float delta_scaled = delta * float(CLIPPER_OFFSET_SCALE);
    // Offsetted ExPolygons before they are united.
    Clipper3r::Paths contours_cummulative;
    contours_cummulative.reserve(expolygons.size());
    // How many non-empty offsetted expolygons were actually collected into contours_cummulative?
    // If only one, then there is no need to do a final union.
    size_t expolygons_collected = 0;
    for (Slic3r::ExPolygons::const_iterator it_expoly = expolygons.begin(); it_expoly != expolygons.end(); ++ it_expoly) {
        // 1) Offset the outer contour.
        Clipper3r::Paths contours;
        {
            Clipper3r::Path input = Slic3rMultiPoint_to_ClipperPath(it_expoly->contour);
            scaleClipperPolygon(input);
            Clipper3r::ClipperOffset co;
            if (joinType == jtRound)
                co.ArcTolerance = miterLimit * double(CLIPPER_OFFSET_SCALE);
            else
                co.MiterLimit = miterLimit;
            co.ShortestEdgeLength = double(std::abs(delta_scaled * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR));
            co.AddPath(input, joinType, Clipper3r::etClosedPolygon);
            co.Execute(contours, delta_scaled);
        }
        if (contours.empty())
            // No need to try to offset the holes.
            continue;

        if (it_expoly->holes.empty()) {
            // No need to subtract holes from the offsetted expolygon, we are done.
            contours_cummulative.insert(contours_cummulative.end(), contours.begin(), contours.end());
            ++ expolygons_collected;
        } else {
            // 2) Offset the holes one by one, collect the offsetted holes.
            Clipper3r::Paths holes;
            {
                for (Polygons::const_iterator it_hole = it_expoly->holes.begin(); it_hole != it_expoly->holes.end(); ++ it_hole) {
                    Clipper3r::Path input = Slic3rMultiPoint_to_ClipperPath_reversed(*it_hole);
                    scaleClipperPolygon(input);
                    Clipper3r::ClipperOffset co;
                    if (joinType == jtRound)
                        co.ArcTolerance = miterLimit * double(CLIPPER_OFFSET_SCALE);
                    else
                        co.MiterLimit = miterLimit;
                    co.ShortestEdgeLength = double(std::abs(delta_scaled * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR));
                    co.AddPath(input, joinType, Clipper3r::etClosedPolygon);
                    Clipper3r::Paths out;
                    co.Execute(out, - delta_scaled);
                    holes.insert(holes.end(), out.begin(), out.end());
                }
            }

            // 3) Subtract holes from the contours.
            if (holes.empty()) {
                // No hole remaining after an offset. Just copy the outer contour.
                contours_cummulative.insert(contours_cummulative.end(), contours.begin(), contours.end());
                ++ expolygons_collected;
            } else if (delta < 0) {
                // Negative offset. There is a chance, that the offsetted hole intersects the outer contour. 
                // Subtract the offsetted holes from the offsetted contours.
                Clipper3r::Clipper clipper;
                clipper.Clear();
                clipper.AddPaths(contours, Clipper3r::ptSubject, true);
                clipper.AddPaths(holes, Clipper3r::ptClip, true);
                Clipper3r::Paths output;
                clipper.Execute(Clipper3r::ctDifference, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
                if (! output.empty()) {
                    contours_cummulative.insert(contours_cummulative.end(), output.begin(), output.end());
                    ++ expolygons_collected;
                } else {
                    // The offsetted holes have eaten up the offsetted outer contour.
                }
            } else {
                // Positive offset. As long as the Clipper offset does what one expects it to do, the offsetted hole will have a smaller
                // area than the original hole or even disappear, therefore there will be no new intersections.
                // Just collect the reversed holes.
                contours_cummulative.reserve(contours.size() + holes.size());
                contours_cummulative.insert(contours_cummulative.end(), contours.begin(), contours.end());
                // Reverse the holes in place.
                for (size_t i = 0; i < holes.size(); ++ i)
                    std::reverse(holes[i].begin(), holes[i].end());
                contours_cummulative.insert(contours_cummulative.end(), holes.begin(), holes.end());
                ++ expolygons_collected;
            }
        }
    }

    // 4) Unite the offsetted expolygons.
    Clipper3r::Paths output;
    if (expolygons_collected > 1 && delta > 0) {
        // There is a chance that the outwards offsetted expolygons may intersect. Perform a union.
        Clipper3r::Clipper clipper;
        clipper.Clear(); 
        clipper.AddPaths(contours_cummulative, Clipper3r::ptSubject, true);
        clipper.Execute(Clipper3r::ctUnion, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
    } else {
        // Negative offset. The shrunk expolygons shall not mutually intersect. Just copy the output.
        output = std::move(contours_cummulative);
    }
    
    // 4) Unscale the output.
    unscaleClipperPolygons(output);
    return output;
}

Clipper3r::Paths
_offset2(const Polygons &polygons, const float delta1, const float delta2,
    const Clipper3r::JoinType joinType, const double miterLimit)
{
    // read input
    Clipper3r::Paths input = Slic3rMultiPoints_to_ClipperPaths(polygons);
    
    // scale input
    scaleClipperPolygons(input);
    
    // prepare ClipperOffset object
    Clipper3r::ClipperOffset co;
    if (joinType == jtRound) {
        co.ArcTolerance = miterLimit;
    } else {
        co.MiterLimit = miterLimit;
    }
    float delta_scaled1 = delta1 * float(CLIPPER_OFFSET_SCALE);
    float delta_scaled2 = delta2 * float(CLIPPER_OFFSET_SCALE);
    co.ShortestEdgeLength = double(std::max(std::abs(delta_scaled1), std::abs(delta_scaled2)) * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR);
    
    // perform first offset
    Clipper3r::Paths output1;
    co.AddPaths(input, joinType, Clipper3r::etClosedPolygon);
    co.Execute(output1, delta_scaled1);
    
    // perform second offset
    co.Clear();
    co.AddPaths(output1, joinType, Clipper3r::etClosedPolygon);
    Clipper3r::Paths retval;
    co.Execute(retval, delta_scaled2);
    
    // unscale output
    unscaleClipperPolygons(retval);
    return retval;
}

Polygons
offset2(const Polygons &polygons, const float delta1, const float delta2,
    const Clipper3r::JoinType joinType, const double miterLimit)
{
    // perform offset
    Clipper3r::Paths output = _offset2(polygons, delta1, delta2, joinType, miterLimit);
    
    // convert into ExPolygons
    return ClipperPaths_to_Slic3rPolygons(output);
}

ExPolygons
offset2_ex(const Polygons &polygons, const float delta1, const float delta2,
    const Clipper3r::JoinType joinType, const double miterLimit)
{
    // perform offset
    Clipper3r::Paths output = _offset2(polygons, delta1, delta2, joinType, miterLimit);
    
    // convert into ExPolygons
    return ClipperPaths_to_Slic3rExPolygons(output);
}

//FIXME Vojtech: This functon may likely be optimized to avoid some of the Slic3r to Clipper 
// conversions and unnecessary Clipper calls.
ExPolygons offset2_ex(const ExPolygons &expolygons, const float delta1,
    const float delta2, Clipper3r::JoinType joinType, double miterLimit)
{
    Polygons polys;
    for (const ExPolygon &expoly : expolygons)
        append(polys, 
               offset(offset_ex(expoly, delta1, joinType, miterLimit), 
                      delta2, joinType, miterLimit));
    return union_ex(polys);
}

template<class T, class TSubj, class TClip>
T _clipper_do(const Clipper3r::ClipType     clipType,
              TSubj &&                        subject,
              TClip &&                        clip,
              const Clipper3r::PolyFillType fillType,
              const bool                     safety_offset_)
{
    // read input
    Clipper3r::Paths input_subject = Slic3rMultiPoints_to_ClipperPaths(std::forward<TSubj>(subject));
    Clipper3r::Paths input_clip    = Slic3rMultiPoints_to_ClipperPaths(std::forward<TClip>(clip));
    
    // perform safety offset
    if (safety_offset_) {
        if (clipType == Clipper3r::ctUnion) {
            safety_offset(&input_subject);
        } else {
            safety_offset(&input_clip);
        }
    }
    
    // init Clipper
    Clipper3r::Clipper clipper;
    clipper.Clear();
    
    // add polygons
    clipper.AddPaths(input_subject, Clipper3r::ptSubject, true);
    clipper.AddPaths(input_clip,    Clipper3r::ptClip,    true);
    
    // perform operation
    T retval;
    clipper.Execute(clipType, retval, fillType, fillType);
    return retval;
}

// Fix of #117: A large fractal pyramid takes ages to slice
// The Clipper library has difficulties processing overlapping polygons.
// Namely, the function Clipper3r::JoinCommonEdges() has potentially a terrible time complexity if the output
// of the operation is of the PolyTree type.
// This function implmenets a following workaround:
// 1) Peform the Clipper operation with the output to Paths. This method handles overlaps in a reasonable time.
// 2) Run Clipper Union once again to extract the PolyTree from the result of 1).
inline Clipper3r::PolyTree _clipper_do_polytree2(const Clipper3r::ClipType clipType, const Polygons &subject, 
    const Polygons &clip, const Clipper3r::PolyFillType fillType, const bool safety_offset_)
{
    // read input
    Clipper3r::Paths input_subject = Slic3rMultiPoints_to_ClipperPaths(subject);
    Clipper3r::Paths input_clip    = Slic3rMultiPoints_to_ClipperPaths(clip);
    
    // perform safety offset
    if (safety_offset_)
        safety_offset((clipType == Clipper3r::ctUnion) ? &input_subject : &input_clip);
    
    Clipper3r::Clipper clipper;
    clipper.AddPaths(input_subject, Clipper3r::ptSubject, true);
    clipper.AddPaths(input_clip,    Clipper3r::ptClip,    true);
    // Perform the operation with the output to input_subject.
    // This pass does not generate a PolyTree, which is a very expensive operation with the current Clipper library
    // if there are overapping edges.
    clipper.Execute(clipType, input_subject, fillType, fillType);
    // Perform an additional Union operation to generate the PolyTree ordering.
    clipper.Clear();
    clipper.AddPaths(input_subject, Clipper3r::ptSubject, true);
    Clipper3r::PolyTree retval;
    clipper.Execute(Clipper3r::ctUnion, retval, fillType, fillType);
    return retval;
}

Clipper3r::PolyTree _clipper_do_pl(const Clipper3r::ClipType clipType, const Polylines &subject, 
    const Polygons &clip, const Clipper3r::PolyFillType fillType,
    const bool safety_offset_)
{
    // read input
    Clipper3r::Paths input_subject = Slic3rMultiPoints_to_ClipperPaths(subject);
    Clipper3r::Paths input_clip    = Slic3rMultiPoints_to_ClipperPaths(clip);
    
    // perform safety offset
    if (safety_offset_) safety_offset(&input_clip);
    
    // init Clipper
    Clipper3r::Clipper clipper;
    clipper.Clear();
    
    // add polygons
    clipper.AddPaths(input_subject, Clipper3r::ptSubject, false);
    clipper.AddPaths(input_clip,    Clipper3r::ptClip,    true);
    
    // perform operation
    Clipper3r::PolyTree retval;
    clipper.Execute(clipType, retval, fillType, fillType);
    return retval;
}

Polygons _clipper(Clipper3r::ClipType clipType, const Polygons &subject, const Polygons &clip, bool safety_offset_)
{
    return ClipperPaths_to_Slic3rPolygons(_clipper_do<Clipper3r::Paths>(clipType, subject, clip, Clipper3r::pftNonZero, safety_offset_));
}

ExPolygons _clipper_ex(Clipper3r::ClipType clipType, const Polygons &subject, const Polygons &clip, bool safety_offset_)
{
    Clipper3r::PolyTree polytree = _clipper_do_polytree2(clipType, subject, clip, Clipper3r::pftNonZero, safety_offset_);
    return PolyTreeToExPolygons(polytree);
}

Polylines _clipper_pl(Clipper3r::ClipType clipType, const Polylines &subject, const Polygons &clip, bool safety_offset_)
{
    Clipper3r::Paths output;
    Clipper3r::PolyTreeToPaths(_clipper_do_pl(clipType, subject, clip, Clipper3r::pftNonZero, safety_offset_), output);
    return ClipperPaths_to_Slic3rPolylines(output);
}

Polylines _clipper_pl(Clipper3r::ClipType clipType, const Polygons &subject, const Polygons &clip, bool safety_offset_)
{
    // transform input polygons into polylines
    Polylines polylines;
    polylines.reserve(subject.size());
    for (Polygons::const_iterator polygon = subject.begin(); polygon != subject.end(); ++polygon)
        polylines.emplace_back(polygon->operator Polyline());  // implicit call to split_at_first_point()
    
    // perform clipping
    Polylines retval = _clipper_pl(clipType, polylines, clip, safety_offset_);
    
    /* If the split_at_first_point() call above happens to split the polygon inside the clipping area
       we would get two consecutive polylines instead of a single one, so we go through them in order
       to recombine continuous polylines. */
    for (size_t i = 0; i < retval.size(); ++i) {
        for (size_t j = i+1; j < retval.size(); ++j) {
            if (retval[i].points.back() == retval[j].points.front()) {
                /* If last point of i coincides with first point of j,
                   append points of j to i and delete j */
                retval[i].points.insert(retval[i].points.end(), retval[j].points.begin()+1, retval[j].points.end());
                retval.erase(retval.begin() + j);
                --j;
            } else if (retval[i].points.front() == retval[j].points.back()) {
                /* If first point of i coincides with last point of j,
                   prepend points of j to i and delete j */
                retval[i].points.insert(retval[i].points.begin(), retval[j].points.begin(), retval[j].points.end()-1);
                retval.erase(retval.begin() + j);
                --j;
            } else if (retval[i].points.front() == retval[j].points.front()) {
                /* Since Clipper does not preserve orientation of polylines, 
                   also check the case when first point of i coincides with first point of j. */
                retval[j].reverse();
                retval[i].points.insert(retval[i].points.begin(), retval[j].points.begin(), retval[j].points.end()-1);
                retval.erase(retval.begin() + j);
                --j;
            } else if (retval[i].points.back() == retval[j].points.back()) {
                /* Since Clipper does not preserve orientation of polylines, 
                   also check the case when last point of i coincides with last point of j. */
                retval[j].reverse();
                retval[i].points.insert(retval[i].points.end(), retval[j].points.begin()+1, retval[j].points.end());
                retval.erase(retval.begin() + j);
                --j;
            }
        }
    }
    return retval;
}

Lines
_clipper_ln(Clipper3r::ClipType clipType, const Lines &subject, const Polygons &clip,
    bool safety_offset_)
{
    // convert Lines to Polylines
    Polylines polylines;
    polylines.reserve(subject.size());
    for (const Line &line : subject)
        polylines.emplace_back(Polyline(line.a, line.b));
    
    // perform operation
    polylines = _clipper_pl(clipType, polylines, clip, safety_offset_);
    
    // convert Polylines to Lines
    Lines retval;
    for (Polylines::const_iterator polyline = polylines.begin(); polyline != polylines.end(); ++polyline)
        retval.emplace_back(polyline->operator Line());
    return retval;
}

Clipper3r::PolyTree union_pt(const Polygons &subject, bool safety_offset_)
{
    return _clipper_do<Clipper3r::PolyTree>(Clipper3r::ctUnion, subject, Polygons(), Clipper3r::pftEvenOdd, safety_offset_);
}

Clipper3r::PolyTree union_pt(const ExPolygons &subject, bool safety_offset_)
{
    return _clipper_do<Clipper3r::PolyTree>(Clipper3r::ctUnion, subject, Polygons(), Clipper3r::pftEvenOdd, safety_offset_);
}

Clipper3r::PolyTree union_pt(Polygons &&subject, bool safety_offset_)
{
    return _clipper_do<Clipper3r::PolyTree>(Clipper3r::ctUnion, std::move(subject), Polygons(), Clipper3r::pftEvenOdd, safety_offset_);
}

Clipper3r::PolyTree union_pt(ExPolygons &&subject, bool safety_offset_)
{
    return _clipper_do<Clipper3r::PolyTree>(Clipper3r::ctUnion, std::move(subject), Polygons(), Clipper3r::pftEvenOdd, safety_offset_);
}

// Simple spatial ordering of Polynodes
Clipper3r::PolyNodes order_nodes(const Clipper3r::PolyNodes &nodes)
{
    // collect ordering points
    Points ordering_points;
    ordering_points.reserve(nodes.size());
    
    for (const Clipper3r::PolyNode *node : nodes)
        ordering_points.emplace_back(
            Point(node->Contour.front().X, node->Contour.front().Y));

    // perform the ordering
    Clipper3r::PolyNodes ordered_nodes =
        chain_clipper_polynodes(ordering_points, nodes);

    return ordered_nodes;
}

static void traverse_pt_noholes(const Clipper3r::PolyNodes &nodes, Polygons *out)
{
    foreach_node<e_ordering::ON>(nodes, [&out](const Clipper3r::PolyNode *node) 
    {
        traverse_pt_noholes(node->Childs, out);
        out->emplace_back(ClipperPath_to_Slic3rPolygon(node->Contour));
        if (node->IsHole()) out->back().reverse(); // ccw
    });
}

static void traverse_pt_outside_in(const Clipper3r::PolyNodes &nodes, Polygons *retval)
{
    // collect ordering points
    Points ordering_points;
    ordering_points.reserve(nodes.size());
    for (const Clipper3r::PolyNode *node : nodes)
        ordering_points.emplace_back(node->Contour.front().X, node->Contour.front().Y);

    // Perform the ordering, push results recursively.
    //FIXME pass the last point to chain_clipper_polynodes?
    for (const Clipper3r::PolyNode *node : chain_clipper_polynodes(ordering_points, nodes)) {
        retval->emplace_back(ClipperPath_to_Slic3rPolygon(node->Contour));
        if (node->IsHole()) 
            // Orient a hole, which is clockwise oriented, to CCW.
            retval->back().reverse();
        // traverse the next depth
        traverse_pt_outside_in(node->Childs, retval);
    }
}

Polygons union_pt_chained_outside_in(const Polygons &subject, bool safety_offset_)
{
    Clipper3r::PolyTree polytree = union_pt(subject, safety_offset_);
    
    Polygons retval;
    traverse_pt_outside_in(polytree.Childs, &retval);
    return retval;
}

Polygons simplify_polygons(const Polygons &subject, bool preserve_collinear)
{
    // convert into Clipper polygons
    Clipper3r::Paths input_subject = Slic3rMultiPoints_to_ClipperPaths(subject);
    
    Clipper3r::Paths output;
    if (preserve_collinear) {
        Clipper3r::Clipper c;
        c.PreserveCollinear(true);
        c.StrictlySimple(true);
        c.AddPaths(input_subject, Clipper3r::ptSubject, true);
        c.Execute(Clipper3r::ctUnion, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
    } else {
        Clipper3r::SimplifyPolygons(input_subject, output, Clipper3r::pftNonZero);
    }
    
    // convert into Slic3r polygons
    return ClipperPaths_to_Slic3rPolygons(output);
}

ExPolygons simplify_polygons_ex(const Polygons &subject, bool preserve_collinear)
{
    if (! preserve_collinear)
        return union_ex(simplify_polygons(subject, false));

    // convert into Clipper polygons
    Clipper3r::Paths input_subject = Slic3rMultiPoints_to_ClipperPaths(subject);
    
    Clipper3r::PolyTree polytree;
    
    Clipper3r::Clipper c;
    c.PreserveCollinear(true);
    c.StrictlySimple(true);
    c.AddPaths(input_subject, Clipper3r::ptSubject, true);
    c.Execute(Clipper3r::ctUnion, polytree, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
    
    // convert into ExPolygons
    return PolyTreeToExPolygons(polytree);
}

void safety_offset(Clipper3r::Paths* paths)
{
    CLIPPERUTILS_PROFILE_FUNC();

    // scale input
    scaleClipperPolygons(*paths);
    
    // perform offset (delta = scale 1e-05)
    Clipper3r::ClipperOffset co;
#ifdef CLIPPER_UTILS_DEBUG
    if (clipper_export_enabled) {
        static int iRun = 0;
        export_clipper_input_polygons_bin(debug_out_path("safety_offset-polygons-%d", ++iRun).c_str(), *paths, Clipper3r::Paths());
    }
#endif /* CLIPPER_UTILS_DEBUG */
    Clipper3r::Paths out;
    for (size_t i = 0; i < paths->size(); ++ i) {
        Clipper3r::Path &path = (*paths)[i];
        co.Clear();
        co.MiterLimit = 2;
        bool ccw = Clipper3r::Orientation(path);
        if (! ccw)
            std::reverse(path.begin(), path.end());
        {
            CLIPPERUTILS_PROFILE_BLOCK(safety_offset_AddPaths);
            co.AddPath((*paths)[i], Clipper3r::jtMiter, Clipper3r::etClosedPolygon);
        }
        {
            CLIPPERUTILS_PROFILE_BLOCK(safety_offset_Execute);
            // offset outside by 10um
            Clipper3r::Paths out_this;
            co.Execute(out_this, ccw ? 10.f * float(CLIPPER_OFFSET_SCALE) : -10.f * float(CLIPPER_OFFSET_SCALE));
            if (! ccw) {
                // Reverse the resulting contours once again.
                for (Clipper3r::Paths::iterator it = out_this.begin(); it != out_this.end(); ++ it)
                    std::reverse(it->begin(), it->end());
            }
            if (out.empty())
                out = std::move(out_this);
            else
                std::move(std::begin(out_this), std::end(out_this), std::back_inserter(out));
        }
    }
    *paths = std::move(out);
    
    // unscale output
    unscaleClipperPolygons(*paths);
}

Polygons top_level_islands(const Slic3r::Polygons &polygons)
{
    // init Clipper
    Clipper3r::Clipper clipper;
    clipper.Clear();
    // perform union
    clipper.AddPaths(Slic3rMultiPoints_to_ClipperPaths(polygons), Clipper3r::ptSubject, true);
    Clipper3r::PolyTree polytree;
    clipper.Execute(Clipper3r::ctUnion, polytree, Clipper3r::pftEvenOdd, Clipper3r::pftEvenOdd); 
    // Convert only the top level islands to the output.
    Polygons out;
    out.reserve(polytree.ChildCount());
    for (int i = 0; i < polytree.ChildCount(); ++i)
        out.emplace_back(ClipperPath_to_Slic3rPolygon(polytree.Childs[i]->Contour));
    return out;
}

// Outer offset shall not split the input contour into multiples. It is expected, that the solution will be non empty and it will contain just a single polygon.
Clipper3r::Paths fix_after_outer_offset(
	const Clipper3r::Path 		&input, 
													// combination of default prameters to correspond to void ClipperOffset::Execute(Paths& solution, double delta)
													// to produce a CCW output contour from CCW input contour for a positive offset.
	Clipper3r::PolyFillType 	 filltype, 			// = Clipper3r::pftPositive
	bool 						 reverse_result)	// = false
{
  	Clipper3r::Paths solution;
  	if (! input.empty()) {
		Clipper3r::Clipper clipper;
	  	clipper.AddPath(input, Clipper3r::ptSubject, true);
		clipper.ReverseSolution(reverse_result);
		clipper.Execute(Clipper3r::ctUnion, solution, filltype, filltype);
	}
    return solution;
}

// Inner offset may split the source contour into multiple contours, but one resulting contour shall not lie inside the other.
Clipper3r::Paths fix_after_inner_offset(
	const Clipper3r::Path 		&input, 
													// combination of default prameters to correspond to void ClipperOffset::Execute(Paths& solution, double delta)
													// to produce a CCW output contour from CCW input contour for a negative offset.
	Clipper3r::PolyFillType 	 filltype, 			// = Clipper3r::pftNegative
	bool 						 reverse_result) 	// = true
{
  	Clipper3r::Paths solution;
  	if (! input.empty()) {
		Clipper3r::Clipper clipper;
		clipper.AddPath(input, Clipper3r::ptSubject, true);
		Clipper3r::IntRect r = clipper.GetBounds();
		r.left -= 10; r.top -= 10; r.right += 10; r.bottom += 10;
		if (filltype == Clipper3r::pftPositive)
			clipper.AddPath({ Clipper3r::IntPoint(r.left, r.bottom), Clipper3r::IntPoint(r.left, r.top), Clipper3r::IntPoint(r.right, r.top), Clipper3r::IntPoint(r.right, r.bottom) }, Clipper3r::ptSubject, true);
		else
			clipper.AddPath({ Clipper3r::IntPoint(r.left, r.bottom), Clipper3r::IntPoint(r.right, r.bottom), Clipper3r::IntPoint(r.right, r.top), Clipper3r::IntPoint(r.left, r.top) }, Clipper3r::ptSubject, true);
		clipper.ReverseSolution(reverse_result);
		clipper.Execute(Clipper3r::ctUnion, solution, filltype, filltype);
		if (! solution.empty())
			solution.erase(solution.begin());
	}
	return solution;
}

Clipper3r::Path mittered_offset_path_scaled(const Points &contour, const std::vector<float> &deltas, double miter_limit)
{
	assert(contour.size() == deltas.size());

#ifndef NDEBUG
	// Verify that the deltas are either all positive, or all negative.
	bool positive = false;
	bool negative = false;
	for (float delta : deltas)
		if (delta < 0.f)
			negative = true;
		else if (delta > 0.f)
			positive = true;
	assert(! (negative && positive));
#endif /* NDEBUG */

	Clipper3r::Path out;

	if (deltas.size() > 2)
	{
		out.reserve(contour.size() * 2);

		// Clamp miter limit to 2.
		miter_limit = (miter_limit > 2.) ? 2. / (miter_limit * miter_limit) : 0.5;
		
		// perpenduclar vector
		auto   perp = [](const Vec2d &v) -> Vec2d { return Vec2d(v.y(), - v.x()); };

		// Add a new point to the output, scale by CLIPPER_OFFSET_SCALE and round to Clipper3r::cInt.
		auto   add_offset_point = [&out](Vec2d pt) {
			pt *= double(CLIPPER_OFFSET_SCALE);
			pt += Vec2d(0.5 - (pt.x() < 0), 0.5 - (pt.y() < 0));
			out.emplace_back(Clipper3r::cInt(pt.x()), Clipper3r::cInt(pt.y()));
		};

		// Minimum edge length, squared.
		double lmin  = *std::max_element(deltas.begin(), deltas.end()) * CLIPPER_OFFSET_SHORTEST_EDGE_FACTOR;
		double l2min = lmin * lmin;
		// Minimum angle to consider two edges to be parallel.
		// Vojtech's estimate.
//		const double sin_min_parallel = EPSILON + 1. / double(CLIPPER_OFFSET_SCALE);
		// Implementation equal to Clipper.
		const double sin_min_parallel = 1.;

		// Find the last point further from pt by l2min.
		Vec2d  pt     = contour.front().cast<double>();
		size_t iprev  = contour.size() - 1;
		Vec2d  ptprev;
		for (; iprev > 0; -- iprev) {
			ptprev = contour[iprev].cast<double>();
			if ((ptprev - pt).squaredNorm() > l2min)
				break;
		}

		if (iprev != 0) {
			size_t ilast = iprev;
			// Normal to the (pt - ptprev) segment.
			Vec2d nprev = perp(pt - ptprev).normalized();
			for (size_t i = 0; ; ) {
				// Find the next point further from pt by l2min.
				size_t j = i + 1;
				Vec2d ptnext;
				for (; j <= ilast; ++ j) {
					ptnext = contour[j].cast<double>();
					double l2 = (ptnext - pt).squaredNorm();
					if (l2 > l2min)
						break;
				}
				if (j > ilast) {
					assert(i <= ilast);
					// If the last edge is too short, merge it with the previous edge.
					i = ilast;
					ptnext = contour.front().cast<double>();
				}

				// Normal to the (ptnext - pt) segment.
				Vec2d nnext  = perp(ptnext - pt).normalized();

				double delta  = deltas[i];
				double sin_a  = clamp(-1., 1., cross2(nprev, nnext));
				double convex = sin_a * delta;
				if (convex <= - sin_min_parallel) {
					// Concave corner.
					add_offset_point(pt + nprev * delta);
					add_offset_point(pt);
					add_offset_point(pt + nnext * delta);
				} else {
					double dot = nprev.dot(nnext);
					if (convex < sin_min_parallel && dot > 0.) {
						// Nearly parallel.
						add_offset_point((nprev.dot(nnext) > 0.) ? (pt + nprev * delta) : pt);
					} else {
						// Convex corner, possibly extremely sharp if convex < sin_min_parallel.
						double r = 1. + dot;
					  	if (r >= miter_limit)
							add_offset_point(pt + (nprev + nnext) * (delta / r));
					  	else {
							double dx = std::tan(std::atan2(sin_a, dot) / 4.);
							Vec2d  newpt1 = pt + (nprev - perp(nprev) * dx) * delta;
							Vec2d  newpt2 = pt + (nnext + perp(nnext) * dx) * delta;
#ifndef NDEBUG
							Vec2d vedge = 0.5 * (newpt1 + newpt2) - pt;
							double dist_norm = vedge.norm();
							assert(std::abs(dist_norm - std::abs(delta)) < SCALED_EPSILON);
#endif /* NDEBUG */
							add_offset_point(newpt1);
							add_offset_point(newpt2);
					  	}
					}
				}

				if (i == ilast)
					break;

				ptprev = pt;
				nprev  = nnext;
				pt     = ptnext;
				i = j;
			}
		}
	}

#if 0
	{
		Clipper3r::Path polytmp(out);
		unscaleClipperPolygon(polytmp);
		Slic3r::Polygon offsetted = ClipperPath_to_Slic3rPolygon(polytmp);
		BoundingBox bbox = get_extents(contour);
		bbox.merge(get_extents(offsetted));
		static int iRun = 0;
		SVG svg(debug_out_path("mittered_offset_path_scaled-%d.svg", iRun ++).c_str(), bbox);
		svg.draw_outline(Polygon(contour), "blue", scale_(0.01));
		svg.draw_outline(offsetted, "red", scale_(0.01));
		svg.draw(contour, "blue", scale_(0.03));
		svg.draw((Points)offsetted, "blue", scale_(0.03));
	}
#endif

	return out;
}

Polygons variable_offset_inner(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit)
{
#ifndef NDEBUG
	// Verify that the deltas are all non positive.
	for (const std::vector<float> &ds : deltas)
		for (float delta : ds)
			assert(delta <= 0.);
	assert(expoly.holes.size() + 1 == deltas.size());
#endif /* NDEBUG */

	// 1) Offset the outer contour.
	Clipper3r::Paths contours = fix_after_inner_offset(mittered_offset_path_scaled(expoly.contour.points, deltas.front(), miter_limit), Clipper3r::pftNegative, true);
#ifndef NDEBUG	
	for (auto &c : contours)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 2) Offset the holes one by one, collect the results.
	Clipper3r::Paths holes;
	holes.reserve(expoly.holes.size());
	for (const Polygon& hole : expoly.holes)
		append(holes, fix_after_outer_offset(mittered_offset_path_scaled(hole.points, deltas[1 + &hole - expoly.holes.data()], miter_limit), Clipper3r::pftNegative, false));
#ifndef NDEBUG	
	for (auto &c : holes)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 3) Subtract holes from the contours.
	Clipper3r::Paths output;
	if (holes.empty())
		output = std::move(contours);
	else {
		Clipper3r::Clipper clipper;
		clipper.Clear();
		clipper.AddPaths(contours, Clipper3r::ptSubject, true);
		clipper.AddPaths(holes, Clipper3r::ptClip, true);
		clipper.Execute(Clipper3r::ctDifference, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
	}

	// 4) Unscale the output.
	unscaleClipperPolygons(output);
	return ClipperPaths_to_Slic3rPolygons(output);
}

Polygons variable_offset_outer(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit)
{
#ifndef NDEBUG
	// Verify that the deltas are all non positive.
for (const std::vector<float>& ds : deltas)
		for (float delta : ds)
			assert(delta >= 0.);
	assert(expoly.holes.size() + 1 == deltas.size());
#endif /* NDEBUG */

	// 1) Offset the outer contour.
	Clipper3r::Paths contours = fix_after_outer_offset(mittered_offset_path_scaled(expoly.contour.points, deltas.front(), miter_limit), Clipper3r::pftPositive, false);
#ifndef NDEBUG
	for (auto &c : contours)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 2) Offset the holes one by one, collect the results.
	Clipper3r::Paths holes;
	holes.reserve(expoly.holes.size());
	for (const Polygon& hole : expoly.holes)
		append(holes, fix_after_inner_offset(mittered_offset_path_scaled(hole.points, deltas[1 + &hole - expoly.holes.data()], miter_limit), Clipper3r::pftPositive, true));
#ifndef NDEBUG
	for (auto &c : holes)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 3) Subtract holes from the contours.
	Clipper3r::Paths output;
	if (holes.empty())
		output = std::move(contours);
	else {
		Clipper3r::Clipper clipper;
		clipper.Clear();
		clipper.AddPaths(contours, Clipper3r::ptSubject, true);
		clipper.AddPaths(holes, Clipper3r::ptClip, true);
		clipper.Execute(Clipper3r::ctDifference, output, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
	}

	// 4) Unscale the output.
	unscaleClipperPolygons(output);
	return ClipperPaths_to_Slic3rPolygons(output);
}

ExPolygons variable_offset_outer_ex(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit)
{
#ifndef NDEBUG
	// Verify that the deltas are all non positive.
for (const std::vector<float>& ds : deltas)
		for (float delta : ds)
			assert(delta >= 0.);
	assert(expoly.holes.size() + 1 == deltas.size());
#endif /* NDEBUG */

	// 1) Offset the outer contour.
	Clipper3r::Paths contours = fix_after_outer_offset(mittered_offset_path_scaled(expoly.contour.points, deltas.front(), miter_limit), Clipper3r::pftPositive, false);
#ifndef NDEBUG
	for (auto &c : contours)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 2) Offset the holes one by one, collect the results.
	Clipper3r::Paths holes;
	holes.reserve(expoly.holes.size());
	for (const Polygon& hole : expoly.holes)
		append(holes, fix_after_inner_offset(mittered_offset_path_scaled(hole.points, deltas[1 + &hole - expoly.holes.data()], miter_limit), Clipper3r::pftPositive, true));
#ifndef NDEBUG
	for (auto &c : holes)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 3) Subtract holes from the contours.
	unscaleClipperPolygons(contours);
	ExPolygons output;
	if (holes.empty()) {
		output.reserve(contours.size());
		for (Clipper3r::Path &path : contours) 
			output.emplace_back(ClipperPath_to_Slic3rPolygon(path));
	} else {
		Clipper3r::Clipper clipper;
		unscaleClipperPolygons(holes);
		clipper.AddPaths(contours, Clipper3r::ptSubject, true);
		clipper.AddPaths(holes, Clipper3r::ptClip, true);
	    Clipper3r::PolyTree polytree;
		clipper.Execute(Clipper3r::ctDifference, polytree, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
	    output = PolyTreeToExPolygons(polytree);
	}

	return output;
}


ExPolygons variable_offset_inner_ex(const ExPolygon &expoly, const std::vector<std::vector<float>> &deltas, double miter_limit)
{
#ifndef NDEBUG
	// Verify that the deltas are all non positive.
	for (const std::vector<float>& ds : deltas)
		for (float delta : ds)
			assert(delta <= 0.);
	assert(expoly.holes.size() + 1 == deltas.size());
#endif /* NDEBUG */

	// 1) Offset the outer contour.
	Clipper3r::Paths contours = fix_after_inner_offset(mittered_offset_path_scaled(expoly.contour.points, deltas.front(), miter_limit), Clipper3r::pftNegative, true);
#ifndef NDEBUG
	for (auto &c : contours)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 2) Offset the holes one by one, collect the results.
	Clipper3r::Paths holes;
	holes.reserve(expoly.holes.size());
	for (const Polygon& hole : expoly.holes)
		append(holes, fix_after_outer_offset(mittered_offset_path_scaled(hole.points, deltas[1 + &hole - expoly.holes.data()], miter_limit), Clipper3r::pftNegative, false));
#ifndef NDEBUG
	for (auto &c : holes)
		assert(Clipper3r::Area(c) > 0.);
#endif /* NDEBUG */

	// 3) Subtract holes from the contours.
	unscaleClipperPolygons(contours);
	ExPolygons output;
	if (holes.empty()) {
		output.reserve(contours.size());
		for (Clipper3r::Path &path : contours) 
			output.emplace_back(ClipperPath_to_Slic3rPolygon(path));
	} else {
		Clipper3r::Clipper clipper;
		unscaleClipperPolygons(holes);
		clipper.AddPaths(contours, Clipper3r::ptSubject, true);
		clipper.AddPaths(holes, Clipper3r::ptClip, true);
	    Clipper3r::PolyTree polytree;
		clipper.Execute(Clipper3r::ctDifference, polytree, Clipper3r::pftNonZero, Clipper3r::pftNonZero);
	    output = PolyTreeToExPolygons(polytree);
	}

	return output;
}

}
