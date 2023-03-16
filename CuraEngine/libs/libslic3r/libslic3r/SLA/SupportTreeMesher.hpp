#ifndef SUPPORTTREEMESHER_HPP
#define SUPPORTTREEMESHER_HPP

#include "libslic3r/Point.hpp"

#include "libslic3r/SLA/SupportTreeBuilder.hpp"
#include "libslic3r/SLA/Contour3D.hpp"

namespace Slic3r { namespace sla {

using Portion = std::tuple<double, double>;

inline Portion make_portion(double a, double b)
{
    return std::make_tuple(a, b);
}

Contour3D sphere(double  rho,
                 Portion portion = make_portion(0., 2. * PI),
                 double  fa      = (2. * PI / 360.));

// Down facing cylinder in Z direction with arguments:
// r: radius
// h: Height
// ssteps: how many edges will create the base circle
// sp: starting point
Contour3D cylinder(double       r,
                   double       h,
                   size_t       steps = 45,
                   const Vec3d &sp    = Vec3d::Zero());

Contour3D pinhead(double r_pin, double r_back, double length, size_t steps = 45);

Contour3D halfcone(double       baseheight,
                   double       r_bottom,
                   double       r_top,
                   const Vec3d &pt    = Vec3d::Zero(),
                   size_t       steps = 45);
Contour3D squarecone(double       baseheight,
                   double       r_bottom,
                   double       r_top,
                   const Vec3d &pt    = Vec3d::Zero());

Contour3D ball(double radius, size_t steps = 45);

Contour3D pinHeadCylinder(double r_pin, double r_back, double length, size_t steps = 45);

inline Contour3D get_mesh(const Head &h, size_t steps)
{
    Contour3D mesh;
    if (h.toptouch_enable)
    {
        const double len = h.r_back_mm + h.width_mm + 2 * h.r_pin_mm;
        Contour3D meshBall = ball(h.r_toptouch_mm, steps);
        if ((h.width_mm + h.r_pin_mm - h.r_toptouch_mm) < EPSILON)
        {
            std::cout << "head len is too small" << std::endl;
            mesh = pinHeadCylinder(h.r_pin_mm, h.r_back_mm, h.width_mm + h.r_pin_mm, steps);
        }
        else
        {
            mesh = pinHeadCylinder(h.r_pin_mm, h.r_back_mm, h.width_mm + h.r_pin_mm - h.r_toptouch_mm, steps);

            for (auto& p : meshBall.points) p.z() += len - h.r_toptouch_mm;

            mesh.merge(meshBall);
        }
        if (len - h.penetration_mm > EPSILON)
        {
            for (auto& p : mesh.points) p.z() -= len - h.penetration_mm;

        }
        else
        {
            for (auto& p : mesh.points) p.z() -= len;

        }

    }
    else
    {
        const double len = h.r_back_mm + h.width_mm +2* h.r_pin_mm;

         //mesh = pinhead(h.r_pin_mm, h.r_back_mm, h.width_mm, steps);
         mesh = pinHeadCylinder(h.r_pin_mm, h.r_back_mm, h.width_mm+ h.r_pin_mm, steps);

        for (auto& p : mesh.points) p.z() -= (len-h.penetration_mm);
    }

    using Quaternion = Eigen::Quaternion<double>;

    // We rotate the head to the specified direction. The head's pointing
    // side is facing upwards so this means that it would hold a support
    // point with a normal pointing straight down. This is the reason of
    // the -1 z coordinate
    auto quatern = Quaternion::FromTwoVectors(Vec3d{0, 0, -1}, h.dir);

    for(auto& p : mesh.points) p = quatern * p + h.pos;

    return mesh;
}
inline Contour3D get_mesh(const Anchor& h, size_t steps)
{
    Contour3D mesh = pinhead(h.r_pin_mm, h.r_back_mm, h.width_mm, steps);

    for (auto& p : mesh.points) p.z() -= (h.fullwidth() - h.r_back_mm);

    using Quaternion = Eigen::Quaternion<double>;

    // We rotate the head to the specified direction. The head's pointing
    // side is facing upwards so this means that it would hold a support
    // point with a normal pointing straight down. This is the reason of
    // the -1 z coordinate
    auto quatern = Quaternion::FromTwoVectors(Vec3d{ 0, 0, -1 }, h.dir);

    for (auto& p : mesh.points) p = quatern * p + h.pos;

    return mesh;
}
inline Contour3D get_mesh(const Pillar &p, size_t steps)
{
    if(p.height > EPSILON) { // Endpoint is below the starting point
        // We just create a bridge geometry with the pillar parameters and
        // move the data.
        return cylinder(p.r, p.height, steps, p.endpoint());
    }

    return {};
}
inline Contour3D get_mesh(const Pillar p, const Pedestal linkpedestal, size_t steps)
{
    Contour3D mesh;
    if (p.height > EPSILON) { // Endpoint is below the starting point
        // We just create a bridge geometry with the pillar parameters and
        // move the data.
            auto& points = mesh.points;
            auto& indices = mesh.faces3;
            points.reserve(2 * steps);
            double a = 2 * PI / steps;
            Vec3d sp = p.endpoint();
            double h = p.height;
            double r = p.r;
            double r_down =  linkpedestal.r_top;
            Vec3d jp = sp;
            Vec3d endp = { sp(X), sp(Y), sp(Z) + h };
            // Upper circle points
            for (int i = 0; i < steps; ++i) {
                double phi = i * a;
                double ex = endp(X) + r * std::cos(phi);
                double ey = endp(Y) + r * std::sin(phi);
                points.emplace_back(ex, ey, endp(Z));
            }

            // Lower circle points
            for (int i = 0; i < steps; ++i) {
                double phi = i * a;
                double x = jp(X) + r_down * std::cos(phi);
                double y = jp(Y) + r_down * std::sin(phi);
                points.emplace_back(x, y, jp(Z));
            }

            // Now create long triangles connecting upper and lower circles
            indices.reserve(2 * steps);
            auto offs = steps;
            for (int i = 0; i < steps - 1; ++i) {
                indices.emplace_back(i, i + offs, offs + i + 1);
                indices.emplace_back(i, offs + i + 1, i + 1);
            }

            // Last triangle connecting the first and last vertices
            auto last = steps - 1;
            indices.emplace_back(0, last, offs);
            indices.emplace_back(last, offs + last, offs);

            // According to the slicing algorithms, we need to aid them with generating
            // a watertight body. So we create a triangle fan for the upper and lower
            // ending of the cylinder to close the geometry.
            points.emplace_back(jp); int ci = int(points.size() - 1);
            for (int i = 0; i < steps - 1; ++i)
                indices.emplace_back(i + offs + 1, i + offs, ci);

            indices.emplace_back(offs, steps + offs - 1, ci);

            points.emplace_back(endp); ci = int(points.size() - 1);
            for (int i = 0; i < steps - 1; ++i)
                indices.emplace_back(ci, i, i + 1);

            indices.emplace_back(steps - 1, 0, ci);


    }

    return mesh;
}
inline Contour3D get_mesh(const Pedestal &p, size_t steps)
{
    return halfcone(p.height, p.r_bottom, p.r_top, p.pos, steps);
}

inline Contour3D get_mesh(const Junction &j, size_t steps)
{
    Contour3D mesh = sphere(j.r, make_portion(0, PI), 2 *PI / steps);
    for(auto& p : mesh.points) p += j.pos;
    return mesh;
}

inline Contour3D get_mesh(const Bridge &br, size_t steps)
{
    using Quaternion = Eigen::Quaternion<double>;
    Vec3d v = (br.endp - br.startp);
    Vec3d dir = v.normalized();
    double d = v.norm();

    Contour3D mesh = cylinder(br.r, d, steps);

    auto quater = Quaternion::FromTwoVectors(Vec3d{0,0,1}, dir);
    for(auto& p : mesh.points) p = quater * p + br.startp;

    return mesh;
}
inline Contour3D get_mesh(const CrossBridge &br, size_t steps)
{
    using Quaternion = Eigen::Quaternion<double>;
    Vec3d v = (br.endp - br.startp);
    Vec3d dir = v.normalized();
    double d = v.norm();

    Contour3D mesh = cylinder(br.r, d, steps);

    auto quater = Quaternion::FromTwoVectors(Vec3d{0,0,1}, dir);
    for(auto& p : mesh.points) p = quater * p + br.startp;

    return mesh;
}

inline Contour3D get_mesh(const DiffBridge &br, size_t steps)
{
    double h = br.get_length();
    Contour3D mesh = halfcone(h, br.r, br.end_r, Vec3d::Zero(), steps);

    using Quaternion = Eigen::Quaternion<double>;

    // We rotate the head to the specified direction. The head's pointing
    // side is facing upwards so this means that it would hold a support
    // point with a normal pointing straight down. This is the reason of
    // the -1 z coordinate
    auto quatern = Quaternion::FromTwoVectors(Vec3d{0, 0, 1}, br.get_dir());

    for(auto& p : mesh.points) p = quatern * p + br.startp;

    return mesh;
}
inline Contour3D get_mesh(const SquarePedestal& p)
{
    return squarecone(p.height, p.r_bottom, p.r_top, p.pos);
}

}} // namespace Slic3r::sla

#endif // SUPPORTTREEMESHER_HPP
