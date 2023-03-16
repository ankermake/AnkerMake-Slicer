#ifndef slic3r_Polyline_hpp_
#define slic3r_Polyline_hpp_

#include "libslic3r.h"
#include "Line.hpp"
#include "MultiPoint.hpp"
#include <string>
#include <vector>

namespace Slic3r {

class Polyline;
class ThickPolyline;
typedef std::vector<Polyline> Polylines;
typedef std::vector<ThickPolyline> ThickPolylines;

class Polyline : public MultiPoint {
public:
    Polyline() {};
    Polyline(const Polyline &other) : MultiPoint(other.points) {}
    Polyline(Polyline &&other) : MultiPoint(std::move(other.points)) {}
    Polyline(std::initializer_list<Point> list) : MultiPoint(list) {}
    explicit Polyline(const Point &p1, const Point &p2) { points.reserve(2); points.emplace_back(p1); points.emplace_back(p2); }
    explicit Polyline(const Points &points) : MultiPoint(points) {}
    explicit Polyline(Points &&points) : MultiPoint(std::move(points)) {}
    Polyline& operator=(const Polyline &other) { points = other.points; return *this; }
    Polyline& operator=(Polyline &&other) { points = std::move(other.points); return *this; }
	static Polyline new_scale(const std::vector<Vec2d> &points) {
		Polyline pl;
		pl.points.reserve(points.size());
		for (const Vec2d &pt : points)
			pl.points.emplace_back(Point::new_scale(pt(0), pt(1)));
		return pl;
    }
    
    void append(const Point &point) { this->points.push_back(point); }
    void append(const Points &src) { this->append(src.begin(), src.end()); }
    void append(const Points::const_iterator &begin, const Points::const_iterator &end) { this->points.insert(this->points.end(), begin, end); }
    void append(Points &&src)
    {
        if (this->points.empty()) {
            this->points = std::move(src);
        } else {
            this->points.insert(this->points.end(), src.begin(), src.end());
            src.clear();
        }
    }
    void append(const Polyline &src) 
    { 
        points.insert(points.end(), src.points.begin(), src.points.end());
    }

    void append(Polyline &&src) 
    {
        if (this->points.empty()) {
            this->points = std::move(src.points);
        } else {
            this->points.insert(this->points.end(), src.points.begin(), src.points.end());
            src.points.clear();
        }
    }

    explicit operator Polylines() const;
    explicit operator Line() const;
    const Point& last_point() const override { return this->points.back(); }

    const Point& leftmost_point() const;
    Lines lines() const override;
    void clip_end(double distance);
    void clip_start(double distance);
    void extend_end(double distance);
    void extend_start(double distance);
    Points equally_spaced_points(double distance) const;
    void simplify(double tolerance);
    template <class T> void simplify_by_visibility(const T &area);
    void split_at(const Point &point, Polyline* p1, Polyline* p2) const;
    bool is_straight() const;
};

// Don't use this class in production code, it is used exclusively by the Perl binding for unit tests!
#ifdef PERL_UCHAR_MIN
class PolylineCollection
{
public:
    Polylines polylines;
};
#endif /* PERL_UCHAR_MIN */

extern BoundingBox get_extents(const Polyline &polyline);
extern BoundingBox get_extents(const Polylines &polylines);

inline double total_length(const Polylines &polylines) {
    double total = 0;
    for (const Polyline &pl : polylines)
        total += pl.length();
    return total;
}

inline Lines to_lines(const Polyline &poly) 
{
    Lines lines;
    if (poly.points.size() >= 2) {
        lines.reserve(poly.points.size() - 1);
        for (Points::const_iterator it = poly.points.begin(); it != poly.points.end()-1; ++it)
            lines.push_back(Line(*it, *(it + 1)));
    }
    return lines;
}

inline Lines to_lines(const Polylines &polys) 
{
    size_t n_lines = 0;
    for (size_t i = 0; i < polys.size(); ++ i)
        if (polys[i].points.size() > 1)
            n_lines += polys[i].points.size() - 1;
    Lines lines;
    lines.reserve(n_lines);
    for (size_t i = 0; i < polys.size(); ++ i) {
        const Polyline &poly = polys[i];
        for (Points::const_iterator it = poly.points.begin(); it != poly.points.end()-1; ++it)
            lines.push_back(Line(*it, *(it + 1)));
    }
    return lines;
}

inline void polylines_append(Polylines &dst, const Polylines &src) 
{ 
    dst.insert(dst.end(), src.begin(), src.end());
}

inline void polylines_append(Polylines &dst, Polylines &&src) 
{
    if (dst.empty()) {
        dst = std::move(src);
    } else {
        std::move(std::begin(src), std::end(src), std::back_inserter(dst));
        src.clear();
    }
}

const Point& leftmost_point(const Polylines &polylines);

bool remove_degenerate(Polylines &polylines);

class ThickPolyline : public Polyline {
public:
    ThickPolyline() : endpoints(std::make_pair(false, false)) {}
    ThickLines thicklines() const;
    void reverse() {
        Polyline::reverse();
        std::reverse(this->width.begin(), this->width.end());
        std::swap(this->endpoints.first, this->endpoints.second);
    }

    std::vector<coordf_t> width;
    std::pair<bool,bool>  endpoints;
};

class Polyline3 : public MultiPoint3
{
public:
    virtual Lines3 lines() const;
};

typedef std::vector<Polyline3> Polylines3;

}

#endif
