#include "BoundingBox.hpp"
#include <algorithm>
#include <assert.h>

#include <Eigen/Dense>

namespace Slic3r {

template BoundingBoxBase<Point>::BoundingBoxBase(const std::vector<Point> &points);
template BoundingBoxBase<Vec2d>::BoundingBoxBase(const std::vector<Vec2d> &points);

template BoundingBox3Base<Vec3d>::BoundingBox3Base(const std::vector<Vec3d> &points);

void BoundingBox::polygon(Polygon* polygon) const
{
    polygon->points.clear();
    polygon->points.resize(4);
    polygon->points[0](0) = this->bmin(0);
    polygon->points[0](1) = this->bmin(1);
    polygon->points[1](0) = this->bmax(0);
    polygon->points[1](1) = this->bmin(1);
    polygon->points[2](0) = this->bmax(0);
    polygon->points[2](1) = this->bmax(1);
    polygon->points[3](0) = this->bmin(0);
    polygon->points[3](1) = this->bmax(1);
}

Polygon BoundingBox::polygon() const
{
    Polygon p;
    this->polygon(&p);
    return p;
}

BoundingBox BoundingBox::rotated(double angle) const
{
    BoundingBox out;
    out.merge(this->bmin.rotated(angle));
    out.merge(this->bmax.rotated(angle));
    out.merge(Point(this->bmin(0), this->bmax(1)).rotated(angle));
    out.merge(Point(this->bmax(0), this->bmin(1)).rotated(angle));
    return out;
}

BoundingBox BoundingBox::rotated(double angle, const Point &center) const
{
    BoundingBox out;
    out.merge(this->bmin.rotated(angle, center));
    out.merge(this->bmax.rotated(angle, center));
    out.merge(Point(this->bmin(0), this->bmax(1)).rotated(angle, center));
    out.merge(Point(this->bmax(0), this->bmin(1)).rotated(angle, center));
    return out;
}

template <class PointClass> void
BoundingBoxBase<PointClass>::scale(double factor)
{
    this->bmin *= factor;
    this->bmax *= factor;
}
template void BoundingBoxBase<Point>::scale(double factor);
template void BoundingBoxBase<Vec2d>::scale(double factor);
template void BoundingBoxBase<Vec3d>::scale(double factor);

template <class PointClass> void
BoundingBoxBase<PointClass>::merge(const PointClass &point)
{
    if (this->defined) {
        this->bmin = this->bmin.cwiseMin(point);
        this->bmax = this->bmax.cwiseMax(point);
    } else {
        this->bmin = point;
        this->bmax = point;
        this->defined = true;
    }
}
template void BoundingBoxBase<Point>::merge(const Point &point);
template void BoundingBoxBase<Vec2f>::merge(const Vec2f &point);
template void BoundingBoxBase<Vec2d>::merge(const Vec2d &point);

template <class PointClass> void
BoundingBoxBase<PointClass>::merge(const std::vector<PointClass> &points)
{
    this->merge(BoundingBoxBase(points));
}
template void BoundingBoxBase<Point>::merge(const Points &points);
template void BoundingBoxBase<Vec2d>::merge(const Pointfs &points);

template <class PointClass> void
BoundingBoxBase<PointClass>::merge(const BoundingBoxBase<PointClass> &bb)
{
    assert(bb.defined || bb.bmin(0) >= bb.bmax(0) || bb.bmin(1) >= bb.bmax(1));
    if (bb.defined) {
        if (this->defined) {
            this->bmin = this->bmin.cwiseMin(bb.bmin);
            this->bmax = this->bmax.cwiseMax(bb.bmax);
        } else {
            this->bmin = bb.bmin;
            this->bmax = bb.bmax;
            this->defined = true;
        }
    }
}
template void BoundingBoxBase<Point>::merge(const BoundingBoxBase<Point> &bb);
template void BoundingBoxBase<Vec2f>::merge(const BoundingBoxBase<Vec2f> &bb);
template void BoundingBoxBase<Vec2d>::merge(const BoundingBoxBase<Vec2d> &bb);

template <class PointClass> void
BoundingBox3Base<PointClass>::merge(const PointClass &point)
{
    if (this->defined) {
        this->bmin = this->bmin.cwiseMin(point);
        this->bmax = this->bmax.cwiseMax(point);
    } else {
        this->bmin = point;
        this->bmax = point;
        this->defined = true;
    }
}
template void BoundingBox3Base<Vec3f>::merge(const Vec3f &point);
template void BoundingBox3Base<Vec3d>::merge(const Vec3d &point);

template <class PointClass> void
BoundingBox3Base<PointClass>::merge(const std::vector<PointClass> &points)
{
    this->merge(BoundingBox3Base(points));
}
template void BoundingBox3Base<Vec3d>::merge(const Pointf3s &points);

template <class PointClass> void
BoundingBox3Base<PointClass>::merge(const BoundingBox3Base<PointClass> &bb)
{
    assert(bb.defined || bb.bmin(0) >= bb.bmax(0) || bb.bmin(1) >= bb.bmax(1) || bb.bmin(2) >= bb.bmax(2));
    if (bb.defined) {
        if (this->defined) {
            this->bmin = this->bmin.cwiseMin(bb.bmin);
            this->bmax = this->bmax.cwiseMax(bb.bmax);
        } else {
            this->bmin = bb.bmin;
            this->bmax = bb.bmax;
            this->defined = true;
        }
    }
}
template void BoundingBox3Base<Vec3d>::merge(const BoundingBox3Base<Vec3d> &bb);

template <class PointClass> PointClass
BoundingBoxBase<PointClass>::size() const
{
    return PointClass(this->bmax(0) - this->bmin(0), this->bmax(1) - this->bmin(1));
}
template Point BoundingBoxBase<Point>::size() const;
template Vec2f BoundingBoxBase<Vec2f>::size() const;
template Vec2d BoundingBoxBase<Vec2d>::size() const;

template <class PointClass> PointClass
BoundingBox3Base<PointClass>::size() const
{
    return PointClass(this->bmax(0) - this->bmin(0), this->bmax(1) - this->bmin(1), this->bmax(2) - this->bmin(2));
}
template Vec3f BoundingBox3Base<Vec3f>::size() const;
template Vec3d BoundingBox3Base<Vec3d>::size() const;

template <class PointClass> double BoundingBoxBase<PointClass>::radius() const
{
    assert(this->defined);
    double x = this->bmax(0) - this->bmin(0);
    double y = this->bmax(1) - this->bmin(1);
    return 0.5 * sqrt(x*x+y*y);
}
template double BoundingBoxBase<Point>::radius() const;
template double BoundingBoxBase<Vec2d>::radius() const;

template <class PointClass> double BoundingBox3Base<PointClass>::radius() const
{
    double x = this->bmax(0) - this->bmin(0);
    double y = this->bmax(1) - this->bmin(1);
    double z = this->bmax(2) - this->bmin(2);
    return 0.5 * sqrt(x*x+y*y+z*z);
}
template double BoundingBox3Base<Vec3d>::radius() const;

template <class PointClass> void
BoundingBoxBase<PointClass>::offset(coordf_t delta)
{
    PointClass v(delta, delta);
    this->bmin -= v;
    this->bmax += v;
}
template void BoundingBoxBase<Point>::offset(coordf_t delta);
template void BoundingBoxBase<Vec2d>::offset(coordf_t delta);

template <class PointClass> void
BoundingBox3Base<PointClass>::offset(coordf_t delta)
{
    PointClass v(delta, delta, delta);
    this->bmin -= v;
    this->bmax += v;
}
template void BoundingBox3Base<Vec3d>::offset(coordf_t delta);

template <class PointClass> PointClass
BoundingBoxBase<PointClass>::center() const
{
    return (this->bmin + this->bmax) / 2;
}
template Point BoundingBoxBase<Point>::center() const;
template Vec2f BoundingBoxBase<Vec2f>::center() const;
template Vec2d BoundingBoxBase<Vec2d>::center() const;

template <class PointClass> PointClass
BoundingBox3Base<PointClass>::center() const
{
    return (this->bmin + this->bmax) / 2;
}
template Vec3f BoundingBox3Base<Vec3f>::center() const;
template Vec3d BoundingBox3Base<Vec3d>::center() const;

template <class PointClass> coordf_t
BoundingBox3Base<PointClass>::max_size() const
{
    PointClass s = size();
    return std::max(s(0), std::max(s(1), s(2)));
}
template coordf_t BoundingBox3Base<Vec3f>::max_size() const;
template coordf_t BoundingBox3Base<Vec3d>::max_size() const;

// Align a coordinate to a grid. The coordinate may be negative,
// the aligned value will never be bigger than the original one.
static inline coord_t _align_to_grid(const coord_t coord, const coord_t spacing) {
    // Current C++ standard defines the result of integer division to be rounded to zero,
    // for both positive and negative numbers. Here we want to round down for negative
    // numbers as well.
    coord_t aligned = (coord < 0) ?
            ((coord - spacing + 1) / spacing) * spacing :
            (coord / spacing) * spacing;
    assert(aligned <= coord);
    return aligned;
}

void BoundingBox::align_to_grid(const coord_t cell_size)
{
    if (this->defined) {
        bmin(0) = _align_to_grid(bmin(0), cell_size);
        bmin(1) = _align_to_grid(bmin(1), cell_size);
    }
}

BoundingBoxf3 BoundingBoxf3::transformed(const Transform3d& matrix) const
{
    typedef Eigen::Matrix<double, 3, 8, Eigen::DontAlign> Vertices;

    Vertices src_vertices;
    src_vertices(0, 0) = bmin(0); src_vertices(1, 0) = bmin(1); src_vertices(2, 0) = bmin(2);
    src_vertices(0, 1) = bmax(0); src_vertices(1, 1) = bmin(1); src_vertices(2, 1) = bmin(2);
    src_vertices(0, 2) = bmax(0); src_vertices(1, 2) = bmax(1); src_vertices(2, 2) = bmin(2);
    src_vertices(0, 3) = bmin(0); src_vertices(1, 3) = bmax(1); src_vertices(2, 3) = bmin(2);
    src_vertices(0, 4) = bmin(0); src_vertices(1, 4) = bmin(1); src_vertices(2, 4) = bmax(2);
    src_vertices(0, 5) = bmax(0); src_vertices(1, 5) = bmin(1); src_vertices(2, 5) = bmax(2);
    src_vertices(0, 6) = bmax(0); src_vertices(1, 6) = bmax(1); src_vertices(2, 6) = bmax(2);
    src_vertices(0, 7) = bmin(0); src_vertices(1, 7) = bmax(1); src_vertices(2, 7) = bmax(2);

    Vertices dst_vertices = matrix * src_vertices.colwise().homogeneous();

    Vec3d v_min(dst_vertices(0, 0), dst_vertices(1, 0), dst_vertices(2, 0));
    Vec3d v_max = v_min;

    for (int i = 1; i < 8; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            v_min(j) = std::min(v_min(j), dst_vertices(j, i));
            v_max(j) = std::max(v_max(j), dst_vertices(j, i));
        }
    }

    return BoundingBoxf3(v_min, v_max);
}

}
