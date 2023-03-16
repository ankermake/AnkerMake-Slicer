#ifndef slic3r_BoundingBox_hpp_
#define slic3r_BoundingBox_hpp_

#include "libslic3r.h"
#include "Exception.hpp"
#include "Point.hpp"
#include "Polygon.hpp"

namespace Slic3r {

template <class PointClass>
class BoundingBoxBase
{
public:
    PointClass bmin;
    PointClass bmax;
    bool defined;
    
    BoundingBoxBase() : bmin(PointClass::Zero()), bmax(PointClass::Zero()), defined(false) {}
    BoundingBoxBase(const PointClass &pmin, const PointClass &pmax) : 
        bmin(pmin), bmax(pmax), defined(pmin(0) < pmax(0) && pmin(1) < pmax(1)) {}
    BoundingBoxBase(const PointClass &p1, const PointClass &p2, const PointClass &p3) :
        bmin(p1), bmax(p1), defined(false) { merge(p2); merge(p3); }
    BoundingBoxBase(const std::vector<PointClass>& points) : bmin(PointClass::Zero()), bmax(PointClass::Zero())
    {
        if (points.empty()) {
            this->defined = false;
            // throw Slic3r::InvalidArgument("Empty point set supplied to BoundingBoxBase constructor");
        } else {
            typename std::vector<PointClass>::const_iterator it = points.begin();
            this->bmin = *it;
            this->bmax = *it;
            for (++ it; it != points.end(); ++ it) {
                this->bmin = this->bmin.cwiseMin(*it);
                this->bmax = this->bmax.cwiseMax(*it);
            }
            this->defined = (this->bmin(0) < this->bmax(0)) && (this->bmin(1) < this->bmax(1));
        }
    }
    void reset() { this->defined = false; this->bmin = PointClass::Zero(); this->bmax = PointClass::Zero(); }
    void merge(const PointClass &point);
    void merge(const std::vector<PointClass> &points);
    void merge(const BoundingBoxBase<PointClass> &bb);
    void scale(double factor);
    PointClass size() const;
    double radius() const;
    void translate(coordf_t x, coordf_t y) { assert(this->defined); PointClass v(x, y); this->bmin += v; this->bmax += v; }
    void translate(const Vec2d &v) { this->bmin += v; this->bmax += v; }
    void offset(coordf_t delta);
    BoundingBoxBase<PointClass> inflated(coordf_t delta) const throw() { BoundingBoxBase<PointClass> out(*this); out.offset(delta); return out; }
    PointClass center() const;
    bool contains(const PointClass &point) const {
        return point(0) >= this->bmin(0) && point(0) <= this->bmax(0)
            && point(1) >= this->bmin(1) && point(1) <= this->bmax(1);
    }
    bool contains(const BoundingBoxBase<PointClass> &other) const {
        return contains(other.bmin) && contains(other.bmax);
    }
    bool overlap(const BoundingBoxBase<PointClass> &other) const {
        return ! (this->bmax(0) < other.bmin(0) || this->bmin(0) > other.bmax(0) ||
                  this->bmax(1) < other.bmin(1) || this->bmin(1) > other.bmax(1));
    }
    bool operator==(const BoundingBoxBase<PointClass> &rhs) { return this->bmin == rhs.bmin && this->bmax == rhs.bmax; }
    bool operator!=(const BoundingBoxBase<PointClass> &rhs) { return ! (*this == rhs); }
};

template <class PointClass>
class BoundingBox3Base : public BoundingBoxBase<PointClass>
{
public:
    BoundingBox3Base() : BoundingBoxBase<PointClass>() {}
    BoundingBox3Base(const PointClass &pmin, const PointClass &pmax) : 
        BoundingBoxBase<PointClass>(pmin, pmax) 
        { if (pmin(2) >= pmax(2)) BoundingBoxBase<PointClass>::defined = false; }
    BoundingBox3Base(const PointClass &p1, const PointClass &p2, const PointClass &p3) :
        BoundingBoxBase<PointClass>(p1, p1) { merge(p2); merge(p3); }
    BoundingBox3Base(const std::vector<PointClass>& points)
    {
        if (points.empty())
            throw Slic3r::InvalidArgument("Empty point set supplied to BoundingBox3Base constructor");
        typename std::vector<PointClass>::const_iterator it = points.begin();
        this->bmin = *it;
        this->bmax = *it;
        for (++ it; it != points.end(); ++ it) {
            this->bmin = this->bmin.cwiseMin(*it);
            this->bmax = this->bmax.cwiseMax(*it);
        }
        this->defined = (this->bmin(0) < this->bmax(0)) && (this->bmin(1) < this->bmax(1)) && (this->bmin(2) < this->bmax(2));
    }
    void merge(const PointClass &point);
    void merge(const std::vector<PointClass> &points);
    void merge(const BoundingBox3Base<PointClass> &bb);
    PointClass size() const;
    double radius() const;
    void translate(coordf_t x, coordf_t y, coordf_t z) { assert(this->defined); PointClass v(x, y, z); this->bmin += v; this->bmax += v; }
    void translate(const Vec3d &v) { this->bmin += v; this->bmax += v; }
    void offset(coordf_t delta);
    BoundingBoxBase<PointClass> inflated(coordf_t delta) const throw() { BoundingBoxBase<PointClass> out(*this); out.offset(delta); return out; }
    PointClass center() const;
    coordf_t max_size() const;

    bool contains(const PointClass &point) const {
        return BoundingBoxBase<PointClass>::contains(point) && point(2) >= this->bmin(2) && point(2) <= this->bmax(2);
    }

    bool contains(const BoundingBox3Base<PointClass>& other) const {
        return contains(other.bmin) && contains(other.bmax);
    }

    bool intersects(const BoundingBox3Base<PointClass>& other) const {
        return (this->bmin(0) < other.bmax(0)) && (this->bmax(0) > other.bmin(0)) && (this->bmin(1) < other.bmax(1)) && (this->bmax(1) > other.bmin(1)) && (this->bmin(2) < other.bmax(2)) && (this->bmax(2) > other.bmin(2));
    }
};

// Will prevent warnings caused by non existing definition of template in hpp
extern template void     BoundingBoxBase<Point>::scale(double factor);
extern template void     BoundingBoxBase<Vec2d>::scale(double factor);
extern template void     BoundingBoxBase<Vec3d>::scale(double factor);
extern template void     BoundingBoxBase<Point>::offset(coordf_t delta);
extern template void     BoundingBoxBase<Vec2d>::offset(coordf_t delta);
extern template void     BoundingBoxBase<Point>::merge(const Point &point);
extern template void     BoundingBoxBase<Vec2f>::merge(const Vec2f &point);
extern template void     BoundingBoxBase<Vec2d>::merge(const Vec2d &point);
extern template void     BoundingBoxBase<Point>::merge(const Points &points);
extern template void     BoundingBoxBase<Vec2d>::merge(const Pointfs &points);
extern template void     BoundingBoxBase<Point>::merge(const BoundingBoxBase<Point> &bb);
extern template void     BoundingBoxBase<Vec2f>::merge(const BoundingBoxBase<Vec2f> &bb);
extern template void     BoundingBoxBase<Vec2d>::merge(const BoundingBoxBase<Vec2d> &bb);
extern template Point    BoundingBoxBase<Point>::size() const;
extern template Vec2f    BoundingBoxBase<Vec2f>::size() const;
extern template Vec2d    BoundingBoxBase<Vec2d>::size() const;
extern template double   BoundingBoxBase<Point>::radius() const;
extern template double   BoundingBoxBase<Vec2d>::radius() const;
extern template Point    BoundingBoxBase<Point>::center() const;
extern template Vec2f    BoundingBoxBase<Vec2f>::center() const;
extern template Vec2d    BoundingBoxBase<Vec2d>::center() const;
extern template void     BoundingBox3Base<Vec3f>::merge(const Vec3f &point);
extern template void     BoundingBox3Base<Vec3d>::merge(const Vec3d &point);
extern template void     BoundingBox3Base<Vec3d>::merge(const Pointf3s &points);
extern template void     BoundingBox3Base<Vec3d>::merge(const BoundingBox3Base<Vec3d> &bb);
extern template Vec3f    BoundingBox3Base<Vec3f>::size() const;
extern template Vec3d    BoundingBox3Base<Vec3d>::size() const;
extern template double   BoundingBox3Base<Vec3d>::radius() const;
extern template void     BoundingBox3Base<Vec3d>::offset(coordf_t delta);
extern template Vec3f    BoundingBox3Base<Vec3f>::center() const;
extern template Vec3d    BoundingBox3Base<Vec3d>::center() const;
extern template coordf_t BoundingBox3Base<Vec3f>::max_size() const;
extern template coordf_t BoundingBox3Base<Vec3d>::max_size() const;

class BoundingBox : public BoundingBoxBase<Point>
{
public:
    void polygon(Polygon* polygon) const;
    Polygon polygon() const;
    BoundingBox rotated(double angle) const;
    BoundingBox rotated(double angle, const Point &center) const;
    void rotate(double angle) { (*this) = this->rotated(angle); }
    void rotate(double angle, const Point &center) { (*this) = this->rotated(angle, center); }
    // Align the bmin corner to a grid of cell_size x cell_size cells,
    // to encompass the original bounding box.
    void align_to_grid(const coord_t cell_size);
    
    BoundingBox() : BoundingBoxBase<Point>() {}
    BoundingBox(const Point &pmin, const Point &pmax) : BoundingBoxBase<Point>(pmin, pmax) {}
    BoundingBox(const Points &points) : BoundingBoxBase<Point>(points) {}

    BoundingBox inflated(coordf_t delta) const throw() { BoundingBox out(*this); out.offset(delta); return out; }

    friend BoundingBox get_extents_rotated(const Points &points, double angle);
};

class BoundingBox3  : public BoundingBox3Base<Vec3crd> 
{
public:
    BoundingBox3() : BoundingBox3Base<Vec3crd>() {}
    BoundingBox3(const Vec3crd &pmin, const Vec3crd &pmax) : BoundingBox3Base<Vec3crd>(pmin, pmax) {}
    BoundingBox3(const Points3& points) : BoundingBox3Base<Vec3crd>(points) {}
};

class BoundingBoxf : public BoundingBoxBase<Vec2d> 
{
public:
    BoundingBoxf() : BoundingBoxBase<Vec2d>() {}
    BoundingBoxf(const Vec2d &pmin, const Vec2d &pmax) : BoundingBoxBase<Vec2d>(pmin, pmax) {}
    BoundingBoxf(const std::vector<Vec2d> &points) : BoundingBoxBase<Vec2d>(points) {}
};

class BoundingBoxf3 : public BoundingBox3Base<Vec3d> 
{
public:
    BoundingBoxf3() : BoundingBox3Base<Vec3d>() {}
    BoundingBoxf3(const Vec3d &pmin, const Vec3d &pmax) : BoundingBox3Base<Vec3d>(pmin, pmax) {}
    BoundingBoxf3(const std::vector<Vec3d> &points) : BoundingBox3Base<Vec3d>(points) {}

    BoundingBoxf3 transformed(const Transform3d& matrix) const;
};

template<typename VT>
inline bool empty(const BoundingBoxBase<VT> &bb)
{
    return ! bb.defined || bb.bmin(0) >= bb.bmax(0) || bb.bmin(1) >= bb.bmax(1);
}

template<typename VT>
inline bool empty(const BoundingBox3Base<VT> &bb)
{
    return ! bb.defined || bb.bmin(0) >= bb.bmax(0) || bb.bmin(1) >= bb.bmax(1) || bb.bmin(2) >= bb.bmax(2);
}

inline BoundingBox scaled(const BoundingBoxf &bb) { return {scaled(bb.bmin), scaled(bb.bmax)}; }
inline BoundingBox3 scaled(const BoundingBoxf3 &bb) { return {scaled(bb.bmin), scaled(bb.bmax)}; }
inline BoundingBoxf unscaled(const BoundingBox &bb) { return {unscaled(bb.bmin), unscaled(bb.bmax)}; }
inline BoundingBoxf3 unscaled(const BoundingBox3 &bb) { return {unscaled(bb.bmin), unscaled(bb.bmax)}; }

template<class Tout, class Tin>
auto cast(const BoundingBoxBase<Tin> &b)
{
    return BoundingBoxBase<Vec<3, Tout>>{b.bmin.template cast<Tout>(),
                                         b.bmax.template cast<Tout>()};
}

template<class Tout, class Tin>
auto cast(const BoundingBox3Base<Tin> &b)
{
    return BoundingBox3Base<Vec<3, Tout>>{b.bmin.template cast<Tout>(),
                                          b.bmax.template cast<Tout>()};
}

} // namespace Slic3r

// Serialization through the Cereal library
namespace cereal {
	template<class Archive> void serialize(Archive& archive, Slic3r::BoundingBox   &bb) { archive(bb.bmin, bb.bmax, bb.defined); }
	template<class Archive> void serialize(Archive& archive, Slic3r::BoundingBox3  &bb) { archive(bb.bmin, bb.bmax, bb.defined); }
	template<class Archive> void serialize(Archive& archive, Slic3r::BoundingBoxf  &bb) { archive(bb.bmin, bb.bmax, bb.defined); }
	template<class Archive> void serialize(Archive& archive, Slic3r::BoundingBoxf3 &bb) { archive(bb.bmin, bb.bmax, bb.defined); }
}

#endif
