#include <libslic3r/SLA/ConcaveHull.hpp>
#include <libslic3r/SLA/SpatIndex.hpp>

#include <libslic3r/MTUtils.hpp>
#include <libslic3r/ClipperUtils.hpp>

#include <boost/log/trivial.hpp>

namespace Slic3r {
namespace sla {

inline Vec3d to_vec3(const Vec2crd &v2) { return {double(v2(X)), double(v2(Y)), 0.}; }
inline Vec3d to_vec3(const Vec2d &v2) { return {v2(X), v2(Y), 0.}; }
inline Vec2crd to_vec2(const Vec3d &v3) { return {coord_t(v3(X)), coord_t(v3(Y))}; }

Point ConcaveHull::centroid(const Points &pp)
{
    Point c;
    switch(pp.size()) {
    case 0: break;
    case 1: c = pp.front(); break;
    case 2: c = (pp[0] + pp[1]) / 2; break;
    default: {
        auto MAX = std::numeric_limits<Point::coord_type>::max();
        auto MIN = std::numeric_limits<Point::coord_type>::min();
        Point min = {MAX, MAX}, max = {MIN, MIN};

        for(auto& p : pp) {
            if(p(0) < min(0)) min(0) = p(0);
            if(p(1) < min(1)) min(1) = p(1);
            if(p(0) > max(0)) max(0) = p(0);
            if(p(1) > max(1)) max(1) = p(1);
        }
        c(0) = min(0) + (max(0) - min(0)) / 2;
        c(1) = min(1) + (max(1) - min(1)) / 2;
        break;
    }
    }

    return c;
}

// As it shows, the current offset_ex in ClipperUtils hangs if used in jtRound
// mode
static Clipper3r::Paths fast_offset(const Clipper3r::Paths &paths,
                                     coord_t                  delta,
                                     Clipper3r::JoinType     jointype)
{
    using Clipper3r::ClipperOffset;
    using Clipper3r::etClosedPolygon;
    using Clipper3r::Paths;
    using Clipper3r::Path;

    ClipperOffset offs;
    offs.ArcTolerance = scaled<double>(0.01);

    for (auto &p : paths)
        // If the input is not at least a triangle, we can not do this algorithm
        if(p.size() < 3) {
            ////BOOST_LOG_TRIVIAL(error) << "Invalid geometry for offsetting!";
            return {};
        }

    offs.AddPaths(paths, jointype, etClosedPolygon);

    Paths result;
    offs.Execute(result, static_cast<double>(delta));

    return result;
}

Points ConcaveHull::calculate_centroids() const
{
    // We get the centroids of all the islands in the 2D slice
    Points centroids = reserve_vector<Point>(m_polys.size());
    std::transform(m_polys.begin(), m_polys.end(),
                   std::back_inserter(centroids),
                   [](const Polygon &poly) { return centroid(poly); });

    return centroids;
}

void ConcaveHull::merge_polygons() { m_polys = get_contours(union_ex(m_polys)); }

void ConcaveHull::add_connector_rectangles(const Points &centroids,
                                           coord_t       max_dist,
                                           ThrowOnCancel thr)
{
    //�����������������ļ���
#if 0
    // Centroid of the centroids of islands. This is where the additional
    // connector sticks are routed.
    Point cc = centroid(centroids);

    PointIndex ctrindex;
    unsigned  idx = 0;
    for(const Point &ct : centroids) ctrindex.insert(to_vec3(ct), idx++);

    m_polys.reserve(m_polys.size() + centroids.size());

    idx = 0;
    for (const Point &c : centroids) {
        thr();

        double dx = c.x() - cc.x(), dy = c.y() - cc.y();
        double l  = std::sqrt(dx * dx + dy * dy);
        double nx = dx / l, ny = dy / l;

        const Point &ct = centroids[idx];

        std::vector<PointIndexEl> result = ctrindex.nearest(to_vec3(ct), 2);

        double dist = max_dist;
        for (const PointIndexEl &el : result)
            if (el.second != idx) {
                dist = Line(to_vec2(el.first), ct).length();
                break;
            }

        idx++;

        if (dist >= max_dist) return;

        Polygon r;
        r.points.reserve(3);
        r.points.emplace_back(cc);

        Point n(scaled(nx), scaled(ny));
        r.points.emplace_back(c + Point(n.y(), -n.x()));
        r.points.emplace_back(c + Point(-n.y(), n.x()));
        offset(r, scaled<float>(1.));

        m_polys.emplace_back(r);
    }
#else

    PointIndex ctrindex;
    unsigned  idx = 0;
    for (const Point& ct : centroids) ctrindex.insert(to_vec3(ct), idx++);

    m_polys.reserve(m_polys.size() + centroids.size());

    idx = 0;

    for (const Point& c : centroids) 
    {
        thr();
        auto filterfun = [centroids, max_dist, idx](const PointIndexEl& el)
        {
            const Point& ct = centroids[idx];
            double dist = Line(to_vec2(el.first), ct).length();
            if (dist >= max_dist)
                return false;
            return true;
        };
        std::vector < PointIndexEl > clusterPt = ctrindex.query(filterfun);
        if (clusterPt.size() >= 2)
        {
            Points queryPts;
            for (const PointIndexEl& ptEl : clusterPt)
            {
                queryPts.emplace_back(to_vec2(ptEl.first));
                ctrindex.remove(ptEl);

            }

            Point cc = centroid(queryPts);

            ctrindex.insert(to_vec3(cc), ctrindex.size());

            for (const Point& queryPt : queryPts) {
                thr();

                double dx = queryPt.x() - cc.x(), dy = queryPt.y() - cc.y();
                double l = std::sqrt(dx * dx + dy * dy);
                double nx = dx / l, ny = dy / l;

                Polygon r;
                r.points.reserve(3);
                r.points.emplace_back(cc);

                Point n(scaled(nx), scaled(ny));
                r.points.emplace_back(queryPt + Point(n.y(), -n.x()));
                r.points.emplace_back(queryPt + Point(-n.y(), n.x()));
                offset(r, scaled<float>(1.));

                m_polys.emplace_back(r);
            }
        }
            idx++;
    }
#endif
}

ConcaveHull::ConcaveHull(const Polygons &polys, double mergedist, ThrowOnCancel thr)
{
    if(polys.empty()) return;

    m_polys = polys;
    merge_polygons();

    if(m_polys.size() == 1) return;

    Points centroids = calculate_centroids();

    add_connector_rectangles(centroids, scaled(mergedist), thr);

    merge_polygons();
}

ExPolygons ConcaveHull::to_expolygons() const
{
    auto ret = reserve_vector<ExPolygon>(m_polys.size());
    for (const Polygon &p : m_polys) ret.emplace_back(ExPolygon(p));
    return ret;
}

ExPolygons offset_waffle_style_ex(const ConcaveHull &hull, coord_t delta)
{
    Clipper3r::Paths paths = Slic3rMultiPoints_to_ClipperPaths(hull.polygons());
    paths = fast_offset(paths, 2 * delta, Clipper3r::jtRound);
    paths = fast_offset(paths, -delta, Clipper3r::jtRound);
    ExPolygons ret = ClipperPaths_to_Slic3rExPolygons(paths);
    for (ExPolygon &p : ret) p.holes = {};
    return ret;
}

Polygons offset_waffle_style(const ConcaveHull &hull, coord_t delta)
{
    return to_polygons(offset_waffle_style_ex(hull, delta));
}

}} // namespace Slic3r::sla
