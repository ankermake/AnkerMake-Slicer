#include <libslic3r/SLA/SupportTreeBuildsteps.hpp>

#include <libslic3r/SLA/SpatIndex.hpp>
#include <libslic3r/Optimize/NLoptOptimizer.hpp>

#include <string> 
#include <fstream> 
#include <iostream> 
#include "libslic3r/SLA/SupportTreeMesher.hpp"
//#include "spdlog/cxlog_macro.h"
#include "libslic3r/SLA/SupportPointData.hpp"
//#define USE_ORIGINAL_POINTS


namespace Slic3r {
namespace sla {

using Slic3r::opt::initvals;
using Slic3r::opt::bounds;
using Slic3r::opt::StopCriteria;
using Slic3r::opt::Optimizer;
using Slic3r::opt::AlgNLoptSubplex;
using Slic3r::opt::AlgNLoptGenetic;

StopCriteria get_criteria(const SupportTreeConfig &cfg)
{
    return StopCriteria{}
        .rel_score_diff(cfg.optimizer_rel_score_diff)
        .max_iterations(cfg.optimizer_max_iterations);
}

template<class C, class Hit = IndexedMesh::hit_result>
static Hit min_hit(const C &hits)
{
    auto mit = std::min_element(hits.begin(), hits.end(),
                                [](const Hit &h1, const Hit &h2) {
                                    return h1.distance() < h2.distance();
                                });

    return *mit;
}
PillarIndex& PillarIndex::operator=( PillarIndex& cpy)
{
    m_index = cpy.guarded_clone();
    return *this;
}

SupportTreeBuildsteps::SupportTreeBuildsteps(SupportTreeBuilder& builder, const Slic3r::sla::IndexedMesh& emesh)
    : m_mesh(emesh)
    , m_builder(builder)
    , m_thr(builder.ctl().cancelfn)
{
}

SupportTreeBuildsteps::SupportTreeBuildsteps(SupportTreeBuilder &   builder,
                                    const Slic3r::sla::IndexedMesh& emesh,
                                    const SupportPoints& supportPoints, const SupportTreeConfig& config)
    : m_cfg(config)
    , m_mesh(emesh)
    , m_support_pts(supportPoints)
    , m_support_nmls(supportPoints.size(), 3)
    , m_builder(builder)
    , m_points(supportPoints.size(), 3)
    , m_thr(builder.ctl().cancelfn)
{
    // Prepare the support points in Eigen/IGL format as well, we will use
    // it mostly in this form.

    long i = 0;
    for (const SupportPoint &sp : m_support_pts) {
        m_points.row(i)(X) = double(sp.pos(X));
        m_points.row(i)(Y) = double(sp.pos(Y));
        m_points.row(i)(Z) = double(sp.pos(Z));

        m_support_nmls.row(i)(X) = double(sp.dir(X));
        m_support_nmls.row(i)(Y) = double(sp.dir(X));
        m_support_nmls.row(i)(Z) = double(sp.dir(X));

        ++i;
    }

    m_addHeadsize = m_support_pts.size();

}

void SupportTreeBuildsteps::setData(const SupportPoints& supportPoints, const SupportTreeConfig& config)
{
    m_cfg = config;
    m_support_pts = supportPoints;

    m_points.resize(m_support_pts.size(), 3);
    m_support_nmls.resize(m_support_pts.size(), 3);
    long i = 0;
    for (const SupportPoint& sp : m_support_pts) {
        m_points.row(i)(X) = double(sp.pos(X));
        m_points.row(i)(Y) = double(sp.pos(Y));
        m_points.row(i)(Z) = double(sp.pos(Z));

        m_support_nmls.row(i)(X) = double(sp.dir(X));
        m_support_nmls.row(i)(Y) = double(sp.dir(X));
        m_support_nmls.row(i)(Z) = double(sp.dir(X));

        ++i;
    }
    m_addHeadsize = m_support_pts.size();
}

bool SupportTreeBuildsteps::create(SupportTreeBuilder& builder, const TriangleMesh& mesh, const IndexedMesh& indexedMesh,
    const SupportPoints& supportPoints, const SupportTreeConfig& config)
{
    if (builder.m_supportTreeBuildstepsPtr == nullptr)
        builder.m_supportTreeBuildstepsPtr = new SupportTreeBuildsteps(builder, indexedMesh, supportPoints, config);

    builder.ground_level = indexedMesh.ground_level() - config.object_elevation_mm;

    if (supportPoints.empty())
        return false;

    SupportTreeBuildsteps& alg = *(SupportTreeBuildsteps*)builder.m_supportTreeBuildstepsPtr;
    alg.setData(supportPoints, config);

    return true;
}

bool SupportTreeBuildsteps::execute(SupportTreeBuilder& builder, const TriangleMesh& mesh, const IndexedMesh& indexedMesh,
    const SupportPoints& supportPoints, const SupportTreeConfig& config)
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    if (builder.m_supportTreeBuildstepsPtr == nullptr)
        builder.m_supportTreeBuildstepsPtr = new SupportTreeBuildsteps(builder, indexedMesh, supportPoints, config);

    builder.ground_level = indexedMesh.ground_level() - config.object_elevation_mm;

    if(supportPoints.empty())
        return false;

    SupportTreeBuildsteps& alg = *(SupportTreeBuildsteps*)builder.m_supportTreeBuildstepsPtr;
    alg.setData(supportPoints, config);

    alg.releaseSupportdata();

    // Let's define the individual steps of the processing. We can experiment
    // later with the ordering and the dependencies between them.
    enum Steps {
        BEGIN,
        FILTER,
        PINHEADS,
        CLASSIFY,
        ROUTING_GROUND,
        ROUTING_NONGROUND,
        CASCADE_PILLARS,
        MERGE_RESULT,
        DONE,
        ABORT,
        NUM_STEPS
        //...
    };

    // Collect the algorithm steps into a nice sequence
    std::array<std::function<void()>, NUM_STEPS> program = {
        [] () {
            // Begin...
            // Potentially clear up the shared data (not needed for now)
        },

        std::bind(&SupportTreeBuildsteps::filter, &alg),

        std::bind(&SupportTreeBuildsteps::add_pinheads, &alg),

#if 1
        std::bind(&SupportTreeBuildsteps::CX_classify, &alg),

        std::bind(&SupportTreeBuildsteps::CX_routing_to_ground, &alg),

        std::bind(&SupportTreeBuildsteps::CX_routing_to_model, &alg),
#else
        std::bind(&SupportTreeBuildsteps::classify, &alg),

        std::bind(&SupportTreeBuildsteps::routing_to_ground, &alg),

        std::bind(&SupportTreeBuildsteps::routing_to_model, &alg),
#endif

#if 1
        std::bind(&SupportTreeBuildsteps::interconnect_pillars, &alg),
#else
        []() {
            // CASCADE_PILLARS
        },

#endif

#if 0
        std::bind(&SupportTreeBuildsteps::merge_result, &alg),
#else
        [] () {
            // MERGE_RESULT
        },
#endif

        [] () {
            // Done
        },
		[] () {
            // Abort
        }


    };
    if (builder.m_supportType == 0x01)
    {
         program = {
        []() {
                // Begin...
                // Potentially clear up the shared data (not needed for now)
            },
            std::bind(&SupportTreeBuildsteps::filterDown, &alg),
            
            std::bind(&SupportTreeBuildsteps::add_pinheads, &alg),

    #if 1
            std::bind(&SupportTreeBuildsteps::CX_classify_simple, &alg),

            std::bind(&SupportTreeBuildsteps::CX_routing_to_ground, &alg),

            std::bind(&SupportTreeBuildsteps::CX_routing_to_model, &alg),
    #else
            std::bind(&SupportTreeBuildsteps::classify, &alg),

            std::bind(&SupportTreeBuildsteps::routing_to_ground, &alg),

            std::bind(&SupportTreeBuildsteps::routing_to_model, &alg),
    #endif

    #if 1
            std::bind(&SupportTreeBuildsteps::interconnect_pillars, &alg),
    #else
            []() {
                // CASCADE_PILLARS
            },

    #endif

    #if 0
            std::bind(&SupportTreeBuildsteps::merge_result, &alg),
    #else
            []() {
                // MERGE_RESULT
            },
    #endif

            []() {
                // Done
            },
            []() {
                // Abort
            }


        };
    }
    Steps pc = BEGIN;

    //if(sm.cfg.ground_facing_only) {
    //    program[ROUTING_NONGROUND] = []() {
    //        BOOST_LOG_TRIVIAL(info)
    //            << "Skipping model-facing supports as requested.";
    //    };
    //}

    // Let's define a simple automaton that will run our program.
    auto progress = [&builder, &pc] () {
        static const std::array<std::string, NUM_STEPS> stepstr {
            "Starting",
            "Filtering",
            "Generate pinheads",
            "Classification",
            "Routing to ground",
            "Routing supports to model surface",
            "Interconnecting pillars",
            "Merging support mesh",
            "Done",
            "Abort"
        };

        static const std::array<float, NUM_STEPS> stepstate {
            0.0,
            0.10,
            0.50,
            0.50,
            0.60,
            0.70,
            0.80,
            0.90,
            1.0,
            0.0
        };

        if(builder.ctl().stopcondition()) pc = ABORT;

        switch(pc) {
        case BEGIN: pc = FILTER; break;
        case FILTER: pc = PINHEADS; break;
        case PINHEADS: pc = CLASSIFY; break;
        case CLASSIFY: pc = ROUTING_GROUND; break;
        case ROUTING_GROUND: pc = ROUTING_NONGROUND; break;
        case ROUTING_NONGROUND: pc = CASCADE_PILLARS; break;
        case CASCADE_PILLARS: pc = MERGE_RESULT; break;
        case MERGE_RESULT: pc = DONE; break;
        case DONE:
        case ABORT: break;
        default: ;
        }

        //builder.ctl().statuscb(stepstate[pc], stepstr[pc]);
        builder.ctl().statusProgress(stepstate[pc]);
    };

    // Just here we run the computation...
    while(pc < DONE) {
        progress();
        program[pc]();
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    double ttrack = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    std::cout <<__LINE__<< " support excute  hold-up time==" << ttrack << std::endl;

    return pc == ABORT;
}

IndexedMesh::hit_result SupportTreeBuildsteps::pinhead_mesh_intersect(
    const Vec3d &s,
    const Vec3d &dir,
    double       r_pin,
    double       r_back,
    double       width,
    double       sd)
{
    static const size_t SAMPLES = 8;

    // Move away slightly from the touching point to avoid raycasting on the
    // inner surface of the mesh.

    auto& m = m_mesh;
    using HitResult = IndexedMesh::hit_result;

    // Hit results
    std::array<HitResult, SAMPLES> hits;
    std::array<HitResult, SAMPLES> hitSeconds;

    struct Rings {
        double rpin;
        double rback;
        Vec3d  spin;
        Vec3d  sback;
        PointRing<SAMPLES> ring;

        Vec3d backring(size_t idx) { return ring.get(idx, sback, rback); }
        Vec3d pinring(size_t idx) { return ring.get(idx, spin, rpin); }
    } rings {r_pin + sd, r_back + sd, s, s + width * dir, dir};

    // We will shoot multiple rays from the head pinpoint in the direction
    // of the pinhead robe (side) surface. The result will be the smallest
    // hit distance.

    ccr::for_each(size_t(0), hits.size(),
                  [&m, &rings, sd, &hits](size_t i) {

       // Point on the circle on the pin sphere
       Vec3d ps = rings.pinring(i);
       // This is the point on the circle on the back sphere
       Vec3d p = rings.backring(i);

       auto &hit = hits[i];

       // Point ps is not on mesh but can be inside or
       // outside as well. This would cause many problems
       // with ray-casting. To detect the position we will
       // use the ray-casting result (which has an is_inside
       // predicate).

       Vec3d n = (p - ps).normalized();
       auto  q = m.query_ray_hit(ps + sd * n, n);

       if (q.is_inside()) { // the hit is inside the model
           if (q.distance() > rings.rpin) {
               // If we are inside the model and the hit
               // distance is bigger than our pin circle
               // diameter, it probably indicates that the
               // support point was already inside the
               // model, or there is really no space
               // around the point. We will assign a zero
               // hit distance to these cases which will
               // enforce the function return value to be
               // an invalid ray with zero hit distance.
               // (see min_element at the end)
               //hit = HitResult(0.0);
               hit = q;
               hit.m_t_real = hit.m_t;
               hit.m_t = 0.0;

           } else {
               // re-cast the ray from the outside of the
               // object. The starting point has an offset
               // of 2*safety_distance because the
               // original ray has also had an offset
               auto q2 = m.query_ray_hit(ps + (q.distance() + 2 * sd) * n, n);
               hit = q2;
           }
       } else
           hit = q;
    });

    ccr::for_each(size_t(0), hitSeconds.size(),
                  [&m, &rings, sd, &hitSeconds](size_t i) {

       // Point on the circle on the pin sphere
       Vec3d ps = rings.pinring(i);
       // This is the point on the circle on the back sphere
       Vec3d p = rings.backring((i+4)% SAMPLES);

       auto &hit = hitSeconds[i];

       // Point ps is not on mesh but can be inside or
       // outside as well. This would cause many problems
       // with ray-casting. To detect the position we will
       // use the ray-casting result (which has an is_inside
       // predicate).

       Vec3d n = (p - ps).normalized();
       auto  q = m.query_ray_hit(ps + sd * n, n);

       if (q.is_inside()) { // the hit is inside the model
           if (q.distance() > rings.rpin) {
               // If we are inside the model and the hit
               // distance is bigger than our pin circle
               // diameter, it probably indicates that the
               // support point was already inside the
               // model, or there is really no space
               // around the point. We will assign a zero
               // hit distance to these cases which will
               // enforce the function return value to be
               // an invalid ray with zero hit distance.
               // (see min_element at the end)
               hit = HitResult(0.0);
               //hit = q;
               //hit.m_t_real = hit.m_t;
               //hit.m_t = 0.0;

           } else {
               // re-cast the ray from the outside of the
               // object. The starting point has an offset
               // of 2*safety_distance because the
               // original ray has also had an offset
               auto q2 = m.query_ray_hit(ps + (q.distance() + 2 * sd) * n, n);
               hit = q2;
           }
       } else
           hit = q;
    });
    //////
    {
        std::array<HitResult, SAMPLES*2> hitTotals;
        for (int i = 0; i < hits.size(); i++)
            hitTotals[i] = hits[i];
        for (int i = 0; i < hitSeconds.size(); i++)
            hitTotals[SAMPLES +i] = hitSeconds[i];

        return min_hit(hitTotals);
    }
}
IndexedMesh::hit_result SupportTreeBuildsteps::cone_mesh_intersect(
    const Vec3d& s,
    const Vec3d& dir,
    double       r_pin,
    double       r_back,
    double       width,
    double       sd)
{
    static const size_t SAMPLES = 8;

    // Move away slightly from the touching point to avoid raycasting on the
    // inner surface of the mesh.

    auto& m = m_mesh;
    using HitResult = IndexedMesh::hit_result;

    // Hit results
    std::array<HitResult, SAMPLES> hits;
    std::array<HitResult, SAMPLES> hitSeconds;

    struct Rings {
        double rpin;
        double rback;
        Vec3d  spin;
        Vec3d  sback;
        PointRing<SAMPLES> ring;

        Vec3d backring(size_t idx) { return ring.get(idx, sback, rback); }
        Vec3d pinring(size_t idx) { return ring.get(idx, spin, rpin); }
    } rings{ r_pin + sd , r_back + sd, s, s + width * dir, dir };

    // We will shoot multiple rays from the head pinpoint in the direction
    // of the pinhead robe (side) surface. The result will be the smallest
    // hit distance.

    ccr::for_each(size_t(0), hits.size(),
        [&m, &rings, &hits](size_t i) {

            // Point on the circle on the pin sphere
            Vec3d ps = rings.pinring(i);
            // This is the point on the circle on the back sphere
            Vec3d p = rings.backring(i);

            auto& hit = hits[i];

            // Point ps is not on mesh but can be inside or
            // outside as well. This would cause many problems
            // with ray-casting. To detect the position we will
            // use the ray-casting result (which has an is_inside
            // predicate).

            Vec3d n = (p - ps).normalized();
            auto  q = m.query_ray_hit(ps, n);
            hit = q;
        });

    ccr::for_each(size_t(0), hitSeconds.size(),
        [&m, &rings, &hitSeconds](size_t i) {

            // Point on the circle on the pin sphere
            Vec3d ps = rings.pinring(i);
            // This is the point on the circle on the back sphere
            Vec3d p = rings.backring((i + 4) % SAMPLES);

            auto& hit = hitSeconds[i];

            // Point ps is not on mesh but can be inside or
            // outside as well. This would cause many problems
            // with ray-casting. To detect the position we will
            // use the ray-casting result (which has an is_inside
            // predicate).

            Vec3d n = (p - ps).normalized();
            auto  q = m.query_ray_hit(ps , n);
            hit = q;
        });
    //////
    {
        std::array<HitResult, SAMPLES * 2> hitTotals;
        for (int i = 0; i < hits.size(); i++)
            hitTotals[i] = hits[i];
        for (int i = 0; i < hitSeconds.size(); i++)
            hitTotals[SAMPLES + i] = hitSeconds[i];

        return min_hit(hitTotals);
    }
}
IndexedMesh::hit_result SupportTreeBuildsteps::bridge_mesh_intersect(
    const Vec3d &src, const Vec3d &dir, double r, double sd)
{
    static const size_t SAMPLES = 8;
    PointRing<SAMPLES> ring{dir};

    using Hit = IndexedMesh::hit_result;

    // Hit results
    std::array<Hit, SAMPLES> hits;

    ccr::for_each(size_t(0), hits.size(),
                 [this, r, src, /*ins_check,*/ &ring, dir, sd, &hits] (size_t i)
    {
        Hit &hit = hits[i];

        // Point on the circle on the pin sphere
        Vec3d p = ring.get(i, src, r + sd);

        auto hr = m_mesh.query_ray_hit(p + r * dir, dir);

        if(/*ins_check && */hr.is_inside()) {
            if (hr.distance() > 2 * r + sd)
            {
                hit = Hit(0.0);
                //hit = hr;
                //hit.m_t_real = hit.m_t;
                //hit.m_t = 0.0;
            }
            else {
                // re-cast the ray from the outside of the object
                hit = m_mesh.query_ray_hit(p + (hr.distance() + EPSILON) * dir, dir);
            }
        } else hit = hr;
    });

    return min_hit(hits);
}
IndexedMesh::hit_result SupportTreeBuildsteps::square_mesh_intersect(
    const Vec3d& s,
    const Vec3d& dir,
    double       r_pin,
    double       r_back,
    double       width,
    double       sd)
{
    static const size_t SAMPLES = 4;

    // Move away slightly from the touching point to avoid raycasting on the
    // inner surface of the mesh.

    auto& m = m_mesh;
    using HitResult = IndexedMesh::hit_result;

    // Hit results
    std::array<HitResult, SAMPLES> hits;
    std::array<HitResult, SAMPLES> hitSeconds;

    struct Rings {
        double rpin;
        double rback;
        Vec3d  spin;
        Vec3d  sback;
        PointRing<SAMPLES> ring;

        Vec3d backring(size_t idx) { return ring.get(idx, sback, rback); }
        Vec3d pinring(size_t idx) { return ring.get(idx, spin, rpin); }
    } rings{ r_pin + sd , r_back + sd, s, s + width * dir, dir };

    // We will shoot multiple rays from the head pinpoint in the direction
    // of the pinhead robe (side) surface. The result will be the smallest
    // hit distance.

    ccr::for_each(size_t(0), hits.size(),
        [&m, &rings, &hits](size_t i) {

            // Point on the circle on the pin sphere
            Vec3d ps = rings.pinring(i);
            // This is the point on the circle on the back sphere
            Vec3d p = rings.backring(i);

            auto& hit = hits[i];

            // Point ps is not on mesh but can be inside or
            // outside as well. This would cause many problems
            // with ray-casting. To detect the position we will
            // use the ray-casting result (which has an is_inside
            // predicate).

            Vec3d n = (p - ps).normalized();
            auto  q = m.query_ray_hit(ps, n);
            hit = q;
        });

    ccr::for_each(size_t(0), hitSeconds.size(),
        [&m, &rings, &hitSeconds](size_t i) {

            // Point on the circle on the pin sphere
            Vec3d ps = rings.pinring(i);
            // This is the point on the circle on the back sphere
            Vec3d p = rings.backring((i + 4) % SAMPLES);

            auto& hit = hitSeconds[i];

            // Point ps is not on mesh but can be inside or
            // outside as well. This would cause many problems
            // with ray-casting. To detect the position we will
            // use the ray-casting result (which has an is_inside
            // predicate).

            Vec3d n = (p - ps).normalized();
            auto  q = m.query_ray_hit(ps, n);
            hit = q;
        });
    //////
    {
        std::array<HitResult, SAMPLES * 2> hitTotals;
        for (int i = 0; i < hits.size(); i++)
            hitTotals[i] = hits[i];
        for (int i = 0; i < hitSeconds.size(); i++)
            hitTotals[SAMPLES + i] = hitSeconds[i];

        return min_hit(hitTotals);
    }
}
bool SupportTreeBuildsteps::interconnect( Pillar &pillar,
                                          Pillar &nextpillar, std::vector<SupportTreeNode2MeshLink>& outintermeshlink)
{
    // We need to get the starting point of the zig-zag pattern. We have to
    // be aware that the two head junctions are at different heights. We
    // may start from the lowest junction and call it a day but this
    // strategy would leave unconnected a lot of pillar duos where the
    // shorter pillar is too short to start a new bridge but the taller
    // pillar could still be bridged with the shorter one.
    bool was_connected = false;

    Vec3d supper = pillar.startpoint();
    Vec3d slower = nextpillar.startpoint();
    Vec3d eupper = pillar.endpoint();
    Vec3d elower = nextpillar.endpoint();

    double bridge_distance_scope = m_cfg.max_bridge_distance;

    double zmin = m_builder.ground_level + m_cfg.base_height_mm+ m_cfg.min_bridge_height_mm;
    eupper(Z) = std::max(eupper(Z), zmin);
    elower(Z) = std::max(elower(Z), zmin);

    // The usable length of both pillars should be positive
    if(slower(Z) - elower(Z) < 0) return false;
    if(supper(Z) - eupper(Z) < 0) return false;
    outintermeshlink.clear();
    double pillar_dist = distance(Vec2d{slower(X), slower(Y)},
                                  Vec2d{supper(X), supper(Y)});
    double bridge_distance = pillar_dist / std::cos(-m_cfg.bridge_slope);
    double zstep = pillar_dist * std::tan(-m_cfg.bridge_slope);

    if(pillar_dist < 2 * m_cfg.head_back_radius_mm ||
        pillar_dist > m_cfg.max_pillar_link_distance_mm) return false;

    if(supper(Z) < slower(Z)) supper.swap(slower);
    if(eupper(Z) < elower(Z)) eupper.swap(elower);

    double startz = 0, endz = 0;

    startz = slower(Z) - zstep < supper(Z) ? slower(Z) - zstep : slower(Z);
    endz = eupper(Z) + zstep > elower(Z) ? eupper(Z) + zstep : eupper(Z);

    if(slower(Z) - eupper(Z) < std::abs(zstep)) {
        // no space for even one cross

        // Get max available space
        startz = std::min(supper(Z), slower(Z) - zstep);
        endz = std::max(eupper(Z) + zstep, elower(Z));

        // Align to center
        double available_dist = (startz - endz);
        double rounds = std::floor(available_dist / std::abs(zstep));
        startz -= 0.5 * (available_dist - rounds * std::abs(zstep));
    }

    //auto pcm = m_cfg.pillar_connection_mode;
    //bool docrosses =
    //    pcm == PillarConnectionMode::cross ||
    //    (pcm == PillarConnectionMode::dynamic &&
    //     pillar_dist > 2*m_cfg.base_radius_mm);

    bool docrosses = pillar_dist > m_cfg.simple_cross_split ? true : false;

    double cha = -2 * bridge_distance_scope - 2 * zstep;
    cha = cha < 0 ? cha : 0;

    // 'sj' means starting junction, 'ej' is the end junction of a bridge.
    // They will be swapped in every iteration thus the zig-zag pattern.
    // According to a config parameter, a second bridge may be added which
    // results in a cross connection between the pillars.
    Vec3d sj = supper, ej = slower; sj(Z) = startz; ej(Z) = sj(Z) + zstep;

    // TODO: This is a workaround to not have a faulty last bridge
    while(ej(Z) >= eupper(Z) /*endz*/) {
        if(bridge_mesh_distance(sj, dirv(sj, ej), pillar.r) >= bridge_distance)
        {
            CrossBridge crossbridge =m_builder.add_crossbridge(sj, ej, pillar.r);
            was_connected = true;
            if (pillar.id < 0)
            {
                pillar = m_builder.pillar(m_builder.add_pillar(pillar));
            }
            if (nextpillar.id < 0)
            {
                nextpillar = m_builder.pillar(m_builder.add_pillar(nextpillar));
            }

            {
                m_builder.buildMeshLinkRelation(m_builder.crossbridge(crossbridge.id), m_builder.pillar(pillar.id));
                m_builder.buildMeshLinkRelation(m_builder.crossbridge(crossbridge.id), m_builder.pillar(nextpillar.id));
                double pillar_dist1 = distance(Vec2d{ crossbridge.startp(X), crossbridge.startp(Y) }, Vec2d{ m_builder.pillar(pillar.id).endpt(X),  m_builder.pillar(pillar.id).endpt(Y) });
                double pillar_dist2 = distance(Vec2d{ crossbridge.startp(X), crossbridge.startp(Y) }, Vec2d{ m_builder.pillar(nextpillar.id).endpt(X), m_builder.pillar(nextpillar.id).endpt(Y) });
                if (pillar_dist1 < pillar_dist2)
                {
                    m_builder.crossbridge(crossbridge.id).setbriStartplink(m_builder.pillar(pillar.id));
                }
                else
                {
                    m_builder.crossbridge(crossbridge.id).setbriStartplink(m_builder.pillar(nextpillar.id));
                }
            }
            {
                SupportTreeNode2MeshLink templink;
                templink.type = crossbridge.treeNodeType;
                templink.id = crossbridge.id;
                outintermeshlink.emplace_back(templink);
            }

        }

        // double bridging: (crosses)
        if(docrosses) {
            Vec3d sjback(ej(X), ej(Y), sj(Z));
            Vec3d ejback(sj(X), sj(Y), ej(Z));
            if (sjback(Z) <= slower(Z) && ejback(Z) >= eupper(Z) &&
                bridge_mesh_distance(sjback, dirv(sjback, ejback),
                                      pillar.r) >= bridge_distance) {
                // need to check collision for the cross stick
                CrossBridge crossbridge=m_builder.add_crossbridge(sjback, ejback, pillar.r);
                was_connected = true;

                if (pillar.id < 0)
                {
                    pillar = m_builder.pillar(m_builder.add_pillar(pillar));
                }
                if (nextpillar.id < 0)
                {
                    nextpillar = m_builder.pillar(m_builder.add_pillar(nextpillar));
                }

                {
                    m_builder.buildMeshLinkRelation(m_builder.crossbridge(crossbridge.id), m_builder.pillar(pillar.id));
                    m_builder.buildMeshLinkRelation(m_builder.crossbridge(crossbridge.id), m_builder.pillar(nextpillar.id));
                    double pillar_dist1 = distance(Vec2d{ crossbridge.startp(X), crossbridge.startp(Y) }, Vec2d{ m_builder.pillar(pillar.id).endpt(X),  m_builder.pillar(pillar.id).endpt(Y) });
                    double pillar_dist2 = distance(Vec2d{ crossbridge.startp(X), crossbridge.startp(Y) }, Vec2d{ m_builder.pillar(nextpillar.id).endpt(X), m_builder.pillar(nextpillar.id).endpt(Y) });
                    if (pillar_dist1 < pillar_dist2)
                    {
                        m_builder.crossbridge(crossbridge.id).setbriStartplink(m_builder.pillar(pillar.id));
                    }
                    else
                    {
                        m_builder.crossbridge(crossbridge.id).setbriStartplink(m_builder.pillar(nextpillar.id));
                    }
                }
                {
                    SupportTreeNode2MeshLink templink;
                    templink.type = crossbridge.treeNodeType;
                    templink.id = crossbridge.id;
                    outintermeshlink.emplace_back(templink);
                }
            }
        }

        sj.swap(ej);
        ej(Z) = sj(Z) + zstep;

        sj(Z) += cha;
        ej(Z) += cha;
    }
    return was_connected;
}
bool SupportTreeBuildsteps::connect_to_nearpillar(const Head& head, long  nearpillar_id)
{
    auto nearpillar = [this, nearpillar_id]() -> const Pillar& {
        return m_builder.pillar(nearpillar_id);
    };

    if (m_builder.bridgecount(nearpillar()) >= m_cfg.max_bridges_on_pillar)
        return false;

    Vec3d headjp = head.junction_point();
    Vec3d nearjp_u = nearpillar().startpoint();
    Vec3d nearjp_l = nearpillar().endpoint();

    double r = head.r_back_mm;
    double d2d = distance(to_2d(headjp), to_2d(nearjp_u));
    double d3d = distance(headjp, nearjp_u);

    double hdiff = nearjp_u(Z) - headjp(Z);
    double slope = std::atan2(hdiff, d2d);

    Vec3d bridgestart = headjp;
    Vec3d bridgeend = nearjp_u;
    double max_len = r * m_cfg.max_bridge_length_mm / m_cfg.head_back_radius_mm;
    double max_slope = m_cfg.bridge_slope;
    double zdiff = 0.0;

    // check the default situation if feasible for a bridge
    if(d3d > max_len || slope > -max_slope) {
        // not feasible to connect the two head junctions. We have to search
        // for a suitable touch point.

        double Zdown = headjp(Z) + d2d * std::tan(-max_slope);
        Vec3d touchjp = bridgeend; touchjp(Z) = Zdown;
        double D = distance(headjp, touchjp);
        zdiff = Zdown - nearjp_u(Z);

        if(zdiff > 0) {
            Zdown -= zdiff;
            bridgestart(Z) -= zdiff;
            touchjp(Z) = Zdown;

            double t = bridge_mesh_distance(headjp, DOWN, r);

            // We can't insert a pillar under the source head to connect
            // with the nearby pillar's starting junction
            if(t < zdiff) return false;
        }

        if(Zdown <= nearjp_u(Z) && Zdown >= nearjp_l(Z) && D < max_len)
            bridgeend(Z) = Zdown;
        else
            return false;
    }

    // There will be a minimum distance from the ground where the
    // bridge is allowed to connect. This is an empiric value.
    double minz = m_builder.ground_level + 4 * head.r_back_mm;
    if(bridgeend(Z) < minz) return false;

    double t = bridge_mesh_distance(bridgestart, dirv(bridgestart, bridgeend), r);

    // Cannot insert the bridge. (further search might not worth the hassle)
    if(t < distance(bridgestart, bridgeend)) return false;

    std::lock_guard<ccr::BlockingMutex> lk(m_bridge_mutex);

    if (m_builder.bridgecount(nearpillar()) < m_cfg.max_bridges_on_pillar) {
        // A partial pillar is needed under the starting head.
        if(zdiff > 0) {
            m_builder.add_pillar(head.id, headjp.z() - bridgestart.z());
            m_builder.add_junction(bridgestart, r);
            m_builder.add_bridge(bridgestart, bridgeend, r);
			        } else {
            m_builder.add_bridge(head.id, bridgeend);
        }

        m_builder.increment_bridges(nearpillar());
    } else return false;

    return true;
}


bool SupportTreeBuildsteps::create_ground_pillar(const Vec3d &hjp,
                                                 const Vec3d &sourcedir,
                                                 double       radius,
                                                 long         head_id,
                                                 std::vector<SupportTreeNode2MeshLink> &outTreeNodelink
                                                 )
{
    SupportTreeNode2MeshLink newMehslink;
    Vec3d  jp           = hjp, endp = jp, dir = sourcedir;
    long   pillar_id    = SupportTreeNode::ID_UNSET;
    bool   can_add_base = true, non_head = false;
    double gndlvl = 0.; // The Z level where pedestals should be
    double jp_gnd = 0.; // The lowest Z where a junction center can be
    double gap_dist = 0.; // The gap distance between the model and the pad

    auto to_floor = [&gndlvl](const Vec3d &p) { return Vec3d{p.x(), p.y(), gndlvl}; };

    auto eval_limits = [this, &radius, &can_add_base, &gndlvl, &gap_dist, &jp_gnd]
        (bool base_en = true)
    {
       //can_add_base  = base_en && radius >= m_cfg.head_back_radius_mm;
        double base_r =  m_cfg.base_radius_mm ;
        gndlvl        = m_builder.ground_level;
        //if (!can_add_base) gndlvl -= m_mesh.ground_level_offset();
        //jp_gnd   = gndlvl + (can_add_base ? 0. : m_cfg.head_back_radius_mm);
        jp_gnd   = gndlvl ;
        gap_dist = m_cfg.pillar_base_safety_distance_mm + base_r + EPSILON;
    };

    eval_limits();
    outTreeNodelink.clear();
    // We are dealing with a mini pillar that's potentially too long
    if (endp(Z) < (m_builder.ground_level + 2*radius + EPSILON))
        return false;


    if (m_cfg.object_elevation_mm < EPSILON)
    {
        // get a suitable direction for the corrector bridge. It is the
        // original sourcedir's azimuth but the polar angle is saturated to the
        // configured bridge slope.
        auto [polar, azimuth] = dir_to_spheric(dir);
        polar = PI - m_cfg.bridge_slope;
        Vec3d d = spheric_to_dir(polar, azimuth).normalized();
        double t = bridge_mesh_distance(endp, dir, radius);
        double tmax = std::min(m_cfg.max_bridge_length_mm, t);
        t = 0.;

        double zd = endp.z() - jp_gnd;
        double tmax2 = zd / std::sqrt(1 - m_cfg.bridge_slope * m_cfg.bridge_slope);
        tmax = std::min(tmax, tmax2);

        Vec3d nexp = endp;
        double dlast = 0.;
        while (((dlast = std::sqrt(m_mesh.squared_distance(to_floor(nexp)))) < gap_dist ||
                !std::isinf(bridge_mesh_distance(nexp, DOWN, radius))) && t < tmax) {
            t += radius;
            nexp = endp + t * d;
        }

        if (dlast < gap_dist && can_add_base) {
            nexp         = endp;
            t            = 0.;
            //can_add_base = false;
            eval_limits(can_add_base);

            zd = endp.z() - jp_gnd;
            tmax2 = zd / std::sqrt(1 - m_cfg.bridge_slope * m_cfg.bridge_slope);
            tmax = std::min(tmax, tmax2);

            while (((dlast = std::sqrt(m_mesh.squared_distance(to_floor(nexp)))) < gap_dist ||
                    !std::isinf(bridge_mesh_distance(nexp, DOWN, radius))) && t < tmax) {
                t += radius;
                nexp = endp + t * d;
            }
        }

        // Could not find a path to avoid the pad gap
        if (dlast < gap_dist) return false;
        if (is_approx((endp - nexp).normalized().norm(), 1.))//防止相等异常
            if (bridge_mesh_distance(endp, (endp - nexp).normalized(), radius)< distance(endp - nexp)) return false;
        if (t > 0.) { // Need to make additional bridge
            Junction jun;
            const Bridge& br = m_builder.add_bridge(endp, nexp, radius);
            {
                newMehslink.id = br.id;
                newMehslink.type = MeshTypeElement::Bridge;
                outTreeNodelink.emplace_back(newMehslink);
            }

            if (head_id >= 0) m_builder.head(head_id).bridge_id = br.id;

            jun=m_builder.add_junction(nexp, radius);
            {
                newMehslink.id = jun.id;
                newMehslink.type = MeshTypeElement::Junction;
                outTreeNodelink.emplace_back(newMehslink);
            }
            m_builder.buildMeshLinkRelation(m_builder.bridge(br.id), m_builder.junction(jun.id));
            endp = nexp;
            non_head = true;
        }
    }

    Vec3d gp = to_floor(endp);
    double h = endp.z() - gp.z();

    pillar_id = head_id >= 0 && !non_head ? m_builder.add_pillar(head_id, h) :
                                            m_builder.add_pillar(gp, h, radius);

    {
        if (newMehslink.id > SupportTreeNode::ID_UNSET)
        {
            SupportTreeNode* treeNodeobjPtr = NULL;
            if (m_builder.supportTreeNodeIsValid(newMehslink, &treeNodeobjPtr))
            {
                m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.pillar(pillar_id));
            }
        }
        newMehslink.id = pillar_id;
        newMehslink.type = MeshTypeElement::Pillar;
        outTreeNodelink.emplace_back(newMehslink);
    }

    if (can_add_base)
    {
        add_pillar_base(pillar_id);
        std::vector < SupportTreeNode2MeshLink> searchmeshlinks;
        m_builder.searchRightTreeElement(m_builder.pillar(pillar_id), searchmeshlinks);
        if (searchmeshlinks.size() > 0)
            outTreeNodelink.insert(outTreeNodelink.end(), searchmeshlinks.begin() + 1, searchmeshlinks.end());
    }

    if (pillar_id >= 0) // Save the pillar endpoint in the spatial index
    {
        m_pillar_index.guarded_insert(m_builder.pillar(pillar_id).endpt,
            unsigned(pillar_id));
        m_pillar_index_start.guarded_insert(m_builder.pillar(pillar_id).startpoint(),
            unsigned(pillar_id));
    }

    return true;
}
bool SupportTreeBuildsteps::create_direct_ground_pillar(const Vec3d& hjp,
    const Vec3d& sourcedir,
    double       radius,
    std::vector<SupportTreeNode2MeshLink>& outTreeNodelink
)
{
    SupportTreeNode2MeshLink newMehslink;
    Vec3d  jp = hjp, endp = jp, dir = sourcedir;
    long   pillar_id = SupportTreeNode::ID_UNSET;
    bool   can_add_base = true, non_head = false;
    double gndlvl = 0.; // The Z level where pedestals should be
    double jp_gnd = 0.; // The lowest Z where a junction center can be
    double gap_dist = 0.; // The gap distance between the model and the pad

    auto to_floor = [&gndlvl](const Vec3d& p) { return Vec3d{ p.x(), p.y(), gndlvl }; };

    auto eval_limits = [this, &radius, &can_add_base, &gndlvl, &gap_dist, &jp_gnd]
    (bool base_en = true)
    {
        //can_add_base  = base_en && radius >= m_cfg.head_back_radius_mm;
        double base_r = m_cfg.base_radius_mm;
        gndlvl = m_builder.ground_level;
        //if (!can_add_base) gndlvl -= m_mesh.ground_level_offset();
        //jp_gnd   = gndlvl + (can_add_base ? 0. : m_cfg.head_back_radius_mm);
        jp_gnd = gndlvl;
        gap_dist = base_r + EPSILON;
    };

    eval_limits();
    outTreeNodelink.clear();
    // We are dealing with a mini pillar that's potentially too long

    if (std::sqrt(m_mesh.squared_distance(to_floor(endp))) < gap_dist)
        return false;
    Vec3d gp = to_floor(endp);
    double h = endp.z() - gp.z();

    pillar_id =m_builder.add_pillar(gp, h, radius);

    {
        if (newMehslink.id > SupportTreeNode::ID_UNSET)
        {
            SupportTreeNode* treeNodeobjPtr = NULL;
            if (m_builder.supportTreeNodeIsValid(newMehslink, &treeNodeobjPtr))
            {
                m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.pillar(pillar_id));
            }
        }
        newMehslink.id = pillar_id;
        newMehslink.type = MeshTypeElement::Pillar;
        outTreeNodelink.emplace_back(newMehslink);
    }

    if (can_add_base)
    {
        std::vector < SupportTreeNode2MeshLink> searchmeshlinks;
        add_pillar_base(pillar_id);
        m_builder.searchRightTreeElement(m_builder.pillar(pillar_id), searchmeshlinks);
        if (searchmeshlinks.size() > 0)
            outTreeNodelink.insert(outTreeNodelink.end(), searchmeshlinks.begin() + 1, searchmeshlinks.end());

    }

    if (pillar_id >= 0) // Save the pillar endpoint in the spatial index
    {
        m_pillar_index.guarded_insert(m_builder.pillar(pillar_id).endpt,
            unsigned(pillar_id));
        m_pillar_index_start.guarded_insert(m_builder.pillar(pillar_id).startpoint(),
            unsigned(pillar_id));
    }

    return true;
}
std::optional<DiffBridge> SupportTreeBuildsteps::search_widening_path(
    const Vec3d &jp, const Vec3d &dir, double radius, double new_radius)
{
    double w = radius + 2 * m_cfg.head_back_radius_mm;
    double stopval = w + jp.z() - m_builder.ground_level;
    Optimizer<AlgNLoptSubplex> solver(get_criteria(m_cfg).stop_score(stopval));

    auto [polar, azimuth] = dir_to_spheric(dir);

    double fallback_ratio = radius / m_cfg.head_back_radius_mm;

    auto oresult = solver.to_max().optimize(
        [this, jp, radius, new_radius](const opt::Input<3> &input) {
            auto &[plr, azm, t] = input;

            auto   d   = spheric_to_dir(plr, azm).normalized();
            double ret = pinhead_mesh_intersect(jp, d, radius, new_radius, t)
                             .distance();
            double down = bridge_mesh_distance(jp + t * d, d, new_radius);

            if (ret > t && std::isinf(down))
                ret += jp.z() - m_builder.ground_level;
            else
                ret = 0.0;

            return ret;
        },
        initvals({polar, azimuth, w}), // start with what we have
        bounds({
            {PI - m_cfg.bridge_slope, PI}, // Must not exceed the slope limit
            {-PI, PI}, // azimuth can be a full search
            {radius + m_cfg.head_back_radius_mm,
                  fallback_ratio * m_cfg.max_bridge_length_mm}
        }));

    if (oresult.score >= stopval) {
        polar       = std::get<0>(oresult.optimum);
        azimuth     = std::get<1>(oresult.optimum);
        double t    = std::get<2>(oresult.optimum);
        Vec3d  endp = jp + t * spheric_to_dir(polar, azimuth);

        return DiffBridge(jp, endp, radius, m_cfg.head_back_radius_mm);
    }

    return {};
}
void SupportTreeBuildsteps::filter()
{
    std::cout << __LINE__ << "  " << " step_1 " << std::endl;
    // Get the points that are too close to each other and keep only the
    // first one
    m_thr();

    auto aliases = cluster(m_points, m_cfg.head_front_radius_mm /* D_SP*/, 2);

    PtIndices filtered_indices;
    filtered_indices.reserve(aliases.size());
    m_iheads.reserve(aliases.size());
    m_iheadless.reserve(aliases.size());
    for(auto& a : aliases) {
        // Here we keep only the front point of the cluster.
        filtered_indices.emplace_back(a.front());
    }

    // calculate the normals to the triangles for filtered points
    auto nmls = sla::normals(m_points, m_mesh, 0.1,
                             m_thr, filtered_indices);
    //auto nmls=m_support_nmls;
    // Not all of the support points have to be a valid position for
    // support creation. The angle may be inappropriate or there may
    // not be enough space for the pinhead. Filtering is applied for
    // these reasons.
    m_addHeadsize = m_support_pts.size();

    std::vector<Head> heads; heads.reserve(m_support_pts.size());
    for (const SupportPoint &sp : m_support_pts) {
        m_thr();
        heads.emplace_back(
            m_cfg.head_back_radius_mm,
            m_cfg.head_front_radius_mm,
            m_cfg.head_width_mm,
            m_cfg.head_penetration_mm,
            Vec3d::Zero(),         // dir
            sp.pos.cast<double>() // displacement
            );
        heads.back().setToptouchRadius(m_cfg.head_front_touch_radius_mm, m_cfg.head_front_touch_enable);
        heads.back().postype = sp.icfType;
        if (heads.back().postype == (int)Slic3r::sla::IslandType::isTypeOverhangs_minArea 
           // || heads.back().postype == (int)Slic3r::sla::IslandType::isTypeNew
            )
        {
            double r_pin_mm =  heads.back().r_pin_mm / 2.0;
            heads.back().width_mm = heads.back().width_mm +2* r_pin_mm;
            heads.back().r_pin_mm = r_pin_mm;
        }


    }

    std::function<void(unsigned, size_t, double)> filterfn;
    filterfn = [this, &nmls, &heads, &filterfn](unsigned fidx, size_t i, double back_r) {
        m_thr();
        Head& h = heads[fidx];
        auto n = nmls.row(Eigen::Index(i));
#ifdef USE_ORIGINAL_POINTS
        {
            h.id = fidx; h.dir = n; h.width_mm = 0.1; h.r_back_mm = back_r;
            return;
        }
#endif
        if (h.postype == (int)Slic3r::sla::IslandType::isTypeNew)
        {
            n=DOWN;
            double w = h.width_mm+ 2 * back_r + 2 * h.r_pin_mm -
                h.penetration_mm;

            IndexedMesh::hit_result t = pinhead_mesh_intersect(h.pos, n.normalized(), h.r_pin_mm,
                back_r, m_cfg.head_width_mm);
            if (t.distance() > w&&
                h.junction_point()(Z)<(m_builder.ground_level+EPSILON))
            {
                h.id = fidx; 
                h.dir = n;  
                h.r_back_mm = back_r;
                return;
            }

        }
        //n = DOWN;

        // for all normals we generate the spherical coordinates and
        // saturate the polar angle to 45 degrees from the bottom then
        // convert back to standard coordinates to get the new normal.
        // Then we just create a quaternion from the two normals
        // (Quaternion::FromTwoVectors) and apply the rotation to the
        // arrow head.

        auto [polar, azimuth] = dir_to_spheric(n);

        // skip if the tilt is not sane
        if (polar < PI - m_cfg.normal_cutoff_angle) return;

        // We saturate the polar angle to 3pi/4
        polar = std::max(polar, PI - m_cfg.bridge_slope);

        // save the head (pinpoint) position
        Vec3d hp = m_points.row(fidx);

        double lmin = m_cfg.head_width_mm, lmax = lmin;

        if (back_r < m_cfg.head_back_radius_mm) {
            lmin = 0., lmax = m_cfg.head_penetration_mm;
        }

        // The distance needed for a pinhead to not collide with model.
        double w = lmin + 2 * back_r + 2 * m_cfg.head_front_radius_mm -
                   m_cfg.head_penetration_mm;

        double pin_r = double(m_support_pts[fidx].head_front_radius);

        // Reassemble the now corrected normal
        auto nn = spheric_to_dir(polar, azimuth).normalized();

        // check available distance
        IndexedMesh::hit_result t = pinhead_mesh_intersect(hp, nn, pin_r,
                                                           back_r, w);

        if (t.distance() < w) {
            // Let's try to optimize this angle, there might be a
            // viable normal that doesn't collide with the model
            // geometry and its very close to the default.

            Optimizer<AlgNLoptGenetic> solver(get_criteria(m_cfg));
            solver.seed(0); // we want deterministic behavior

            auto oresult = solver.to_max().optimize(
                [this, pin_r, back_r, hp](const opt::Input<3> &input)
                {
                    auto &[plr, azm, l] = input;

                    auto dir = spheric_to_dir(plr, azm).normalized();

                    return pinhead_mesh_intersect(
                        hp, dir, pin_r, back_r, l).distance();
                },
                initvals({polar, azimuth, (lmin + lmax) / 2.}), // start with what we have
                bounds({
                    {PI - m_cfg.bridge_slope, PI},    // Must not exceed the slope limit
                    {-PI, PI}, // azimuth can be a full search
                    {lmin, lmax}
                }));

            if(oresult.score > w) {
                polar = std::get<0>(oresult.optimum);
                azimuth = std::get<1>(oresult.optimum);
                nn = spheric_to_dir(polar, azimuth).normalized();
                lmin = std::get<2>(oresult.optimum);
                t = IndexedMesh::hit_result(oresult.score);
            }
        }

        if (t.distance() > w &&
            (hp(Z) + w * nn(Z) >(m_builder.ground_level+ back_r + EPSILON))
            ) 
        {
            h.id = fidx; h.dir = nn; h.width_mm = lmin; h.r_back_mm = back_r;
        //} else if (back_r > m_cfg.head_fallback_radius_mm) {
        //    filterfn(fidx, i, m_cfg.head_fallback_radius_mm);
        }
        else {
            //auto hittest = bridge_mesh_intersect(hp, n, back_r);
            //if (hittest.distance() > EPSILON)//将该类型的点直接流到下一流程，最终可能会生成小支撑
            {
                h.id = fidx;
                h.dir = nn;
                h.width_mm = EPSILON;
                h.r_back_mm = back_r;
            }

        }
    };

    ccr::for_each(size_t(0), filtered_indices.size(),
                  [this, &filterfn, &filtered_indices] (size_t i) {
                      filterfn(filtered_indices[i], i, m_cfg.head_back_radius_mm);
                  },8);

    for (size_t i = 0; i < heads.size(); ++i)
        if (heads[i].is_valid()) {
            m_builder.add_head(i, heads[i]);
            m_iheads.emplace_back(i);
        }

    m_thr();
}
void SupportTreeBuildsteps::filterDown()
{
    std::cout << __LINE__ << "  " << " step_1 " << std::endl;
    // Get the points that are too close to each other and keep only the
    // first one
    m_thr();

    auto aliases = cluster(m_points, m_cfg.head_front_radius_mm /* D_SP*/, 2);

    PtIndices filtered_indices;
    filtered_indices.reserve(aliases.size());
    m_iheads.reserve(aliases.size());
    m_iheadless.reserve(aliases.size());
    for (auto& a : aliases) {
        // Here we keep only the front point of the cluster.
        filtered_indices.emplace_back(a.front());
    }

    // calculate the normals to the triangles for filtered points
    //auto nmls = sla::normals(m_points, m_mesh, 0.1,
    //    m_thr, filtered_indices);
    //auto nmls=m_support_nmls;
    // Not all of the support points have to be a valid position for
    // support creation. The angle may be inappropriate or there may
    // not be enough space for the pinhead. Filtering is applied for
    // these reasons.
    m_addHeadsize = m_support_pts.size();

    std::vector<Head> heads; heads.reserve(m_support_pts.size());
    for (const SupportPoint& sp : m_support_pts) {
        m_thr();
        heads.emplace_back(
            m_cfg.head_back_radius_mm,
            m_cfg.head_front_radius_mm,
            m_cfg.head_width_mm,
            m_cfg.head_penetration_mm,
            Vec3d::Zero(),         // dir
            sp.pos.cast<double>() // displacement
        );
        heads.back().setToptouchRadius(m_cfg.head_front_touch_radius_mm, m_cfg.head_front_touch_enable);
        heads.back().postype = sp.icfType;
        if (heads.back().postype == (int)Slic3r::sla::IslandType::isTypeOverhangs_minArea
            // || heads.back().postype == (int)Slic3r::sla::IslandType::isTypeNew
            )
        {
            double r_pin_mm = heads.back().r_pin_mm / 2.0;
            heads.back().width_mm = heads.back().width_mm + 2 * r_pin_mm;
            heads.back().r_pin_mm = r_pin_mm;
        }


    }

    std::function<void(unsigned, size_t, double)> filterfn;
    filterfn = [this, &heads, &filterfn](unsigned fidx, size_t i, double back_r) {
        m_thr();
        Head& h = heads[fidx];
        Vec3d n = DOWN;//force down dir
        ///
        if (h.postype == (int)Slic3r::sla::IslandType::isTypeNew)
        {
            n = DOWN;
            double w = h.width_mm + 2 * back_r + 2 * h.r_pin_mm -
                h.penetration_mm;

            IndexedMesh::hit_result t = pinhead_mesh_intersect(h.pos, n.normalized(), h.r_pin_mm,
                back_r, m_cfg.head_width_mm);
            if (t.distance() > w &&
                h.junction_point()(Z) < (m_builder.ground_level + EPSILON))
            {
                h.id = fidx;
                h.dir = n;
                h.r_back_mm = back_r;
                return;
            }

        }

        // save the head (pinpoint) position
        Vec3d hp = m_points.row(fidx);

        double lmin = m_cfg.head_width_mm, lmax = lmin;

        if (back_r < m_cfg.head_back_radius_mm) {
            lmin = 0., lmax = m_cfg.head_penetration_mm;
        }

        // The distance needed for a pinhead to not collide with model.
        double w = lmin + 2 * back_r + 2 * m_cfg.head_front_radius_mm -
            m_cfg.head_penetration_mm;

        double pin_r = double(m_support_pts[fidx].head_front_radius);

        // check available distance
        IndexedMesh::hit_result t = pinhead_mesh_intersect(hp, n, pin_r,
            back_r, w);

        if (t.distance() < w) {
            // Let's try to optimize this angle, there might be a
            // viable normal that doesn't collide with the model
            // geometry and its very close to the default.

            Optimizer<AlgNLoptGenetic> solver(get_criteria(m_cfg));
            solver.seed(0); // we want deterministic behavior

            auto oresult = solver.to_max().optimize(
                [this, pin_r, back_r, n, hp](const opt::Input<1>& input)
                {
                    auto& [l] = input;

                    auto dir = n;

                    return pinhead_mesh_intersect(
                        hp, dir, pin_r, back_r, l).distance();
                },
                initvals({  (lmin + lmax) / 2. }), // start with what we have
                    bounds({
                        {lmin, lmax}
                        }));

            if (oresult.score > w) {
                lmin = std::get<0>(oresult.optimum);
                t = IndexedMesh::hit_result(oresult.score);
            }
        }

        if (t.distance() > w &&
            (hp(Z) + w * n(Z) > (m_builder.ground_level + back_r + EPSILON))
            )
        {
            h.id = fidx; h.dir = n; h.width_mm = lmin; h.r_back_mm = back_r;
            //} else if (back_r > m_cfg.head_fallback_radius_mm) {
            //    filterfn(fidx, i, m_cfg.head_fallback_radius_mm);
        }
        else {
            //auto hittest = bridge_mesh_intersect(hp, n, back_r);
            //if (hittest.distance() > EPSILON)//将该类型的点直接流到下一流程，最终可能会生成小支撑
            {
                h.id = fidx;
                h.dir = n;
                h.width_mm = EPSILON;
                h.r_back_mm = back_r;
            }

        }
    };

    ccr::for_each(size_t(0), filtered_indices.size(),
        [this, &filterfn, &filtered_indices](size_t i) {
            filterfn(filtered_indices[i], i, m_cfg.head_back_radius_mm);
        }, 8);

    for (size_t i = 0; i < heads.size(); ++i)
        if (heads[i].is_valid()) {
            m_builder.add_head(i, heads[i]);
            m_iheads.emplace_back(i);
        }

    m_thr();
}

void SupportTreeBuildsteps::add_pinheads()
{
    std::cout << __LINE__ << "  " << " step_2 " << std::endl;

}

void SupportTreeBuildsteps::classify()
{
    // We should first get the heads that reach the ground directly
    PtIndices ground_head_indices;
    ground_head_indices.reserve(m_iheads.size());
    m_iheads_onmodel.reserve(m_iheads.size());

    // First we decide which heads reach the ground and can be full
    // pillars and which shall be connected to the model surface (or
    // search a suitable path around the surface that leads to the
    // ground -- TODO)
    for(unsigned i : m_iheads) {
        m_thr();

        Head &head = m_builder.head(i);
        double r = head.r_back_mm;
        Vec3d headjp = head.junction_point();

        // collision check
        auto hit = bridge_mesh_intersect(headjp, DOWN, r);

        if(std::isinf(hit.distance())) ground_head_indices.emplace_back(i);
        else if(m_cfg.ground_facing_only)  head.invalidate();
        else m_iheads_onmodel.emplace_back(i);

        m_head_to_ground_scans[i] = hit;
    }

    // We want to search for clusters of points that are far enough
    // from each other in the XY plane to not cross their pillar bases
    // These clusters of support points will join in one pillar,
    // possibly in their centroid support point.

    auto pointfn = [this](unsigned i) {
        return m_builder.head(i).junction_point();
    };

    auto predicate = [this](const PointIndexEl &e1,
                            const PointIndexEl &e2) {
        double d2d = distance(to_2d(e1.first), to_2d(e2.first));
        double d3d = distance(e1.first, e2.first);
        return d2d < 2 * m_cfg.base_radius_mm
               && d3d < m_cfg.max_bridge_length_mm;
    };

    m_pillar_clusters = cluster(ground_head_indices, pointfn, predicate,
                                m_cfg.max_bridges_on_pillar);
}

void SupportTreeBuildsteps::routing_to_ground()
{
    ClusterEl cl_centroids;
    cl_centroids.reserve(m_pillar_clusters.size());

    for (auto &cl : m_pillar_clusters) {
        m_thr();

        // place all the centroid head positions into the index. We
        // will query for alternative pillar positions. If a sidehead
        // cannot connect to the cluster centroid, we have to search
        // for another head with a full pillar. Also when there are two
        // elements in the cluster, the centroid is arbitrary and the
        // sidehead is allowed to connect to a nearby pillar to
        // increase structural stability.

        if (cl.empty()) continue;

        // get the current cluster centroid
        auto &      thr    = m_thr;
        const auto &points = m_points;

        long lcid = cluster_centroid(
            cl, [&points](size_t idx) { return points.row(long(idx)); },
            [thr](const Vec3d &p1, const Vec3d &p2) {
                thr();
                return distance(Vec2d(p1(X), p1(Y)), Vec2d(p2(X), p2(Y)));
            });

        assert(lcid >= 0);
        unsigned hid = cl[size_t(lcid)]; // Head ID

        cl_centroids.emplace_back(hid);

        Head &h = m_builder.head(hid);
        std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
        if (!create_ground_pillar(h.junction_point(), h.dir, h.r_back_mm, h.id, outTreeNodelink)) {
            m_iheads_onmodel.emplace_back(h.id);
            continue;
        }
    }

    // now we will go through the clusters ones again and connect the
    // sidepoints with the cluster centroid (which is a ground pillar)
    // or a nearby pillar if the centroid is unreachable.
    size_t ci = 0;
    for (auto cl : m_pillar_clusters) {
        m_thr();

        auto cidx = cl_centroids[ci++];

        auto q = m_pillar_index.query(m_builder.head(cidx).junction_point(), 1);
        if (!q.empty()) {
            long centerpillarID = q.front().second;
            for (auto c : cl) {
                m_thr();
                if (c == cidx) continue;

                auto &sidehead = m_builder.head(c);

                if (!connect_to_nearpillar(sidehead, centerpillarID) &&
                    !search_pillar_and_connect(sidehead)) {
                    Vec3d pstart = sidehead.junction_point();
                    // Vec3d pend = Vec3d{pstart(X), pstart(Y), gndlvl};
                    // Could not find a pillar, create one
                    std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
                    create_ground_pillar(pstart, sidehead.dir, sidehead.r_back_mm, sidehead.id, outTreeNodelink);
                }
            }
        }
    }
}

bool SupportTreeBuildsteps::connect_to_ground(Head &head, const Vec3d &dir)
{
    auto hjp = head.junction_point();
    double r = head.r_back_mm;
    double t = bridge_mesh_distance(hjp, dir, head.r_back_mm);
    double d = 0, tdown = 0;
    t = std::min(t, m_cfg.max_bridge_length_mm * r / m_cfg.head_back_radius_mm);

    while (d < t && !std::isinf(tdown = bridge_mesh_distance(hjp + d * dir, DOWN, r)))
        d += r;

    if(!std::isinf(tdown)) return false;

    Vec3d endp = hjp + d * dir;
    bool ret = false;
    std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
    if ((ret = create_ground_pillar(endp, dir, head.r_back_mm, SupportTreeNode::ID_UNSET, outTreeNodelink))) {
        m_builder.add_bridge(head.id, endp);
        m_builder.add_junction(endp, head.r_back_mm);
    }

    return ret;
}

bool SupportTreeBuildsteps::connect_to_ground(Head &head)
{
    if (connect_to_ground(head, head.dir)) return true;

    // Optimize bridge direction:
    // Straight path failed so we will try to search for a suitable
    // direction out of the cavity.
    auto [polar, azimuth] = dir_to_spheric(head.dir);

    Optimizer<AlgNLoptGenetic> solver(get_criteria(m_cfg).stop_score(1e6));
    solver.seed(0); // we want deterministic behavior

    double r_back = head.r_back_mm;
    Vec3d hjp = head.junction_point();
    auto oresult = solver.to_max().optimize(
        [this, hjp, r_back](const opt::Input<2> &input) {
            auto &[plr, azm] = input;
            Vec3d n = spheric_to_dir(plr, azm).normalized();
            return bridge_mesh_distance(hjp, n, r_back);
        },
        initvals({polar, azimuth}),  // let's start with what we have
        bounds({ {PI - m_cfg.bridge_slope, PI}, {-PI, PI} })
    );

    Vec3d bridgedir = spheric_to_dir(oresult.optimum).normalized();
    return connect_to_ground(head, bridgedir);
}

bool SupportTreeBuildsteps::connect_to_model_body(Head &head)
{
    if (head.id <= SupportTreeNode::ID_UNSET) 
        return false;
    //std::cout << __FILE__ << "  " << __LINE__ << "  " << __FUNCTION__ << "  " << std::endl;
    //auto it = m_head_to_ground_scans.find(unsigned(head.id));
    //if (it == m_head_to_ground_scans.end()) return false;

    auto &hit = head.hit;

    if (!hit.is_hit()) {
        // TODO scan for potential anchor points on model surface
        return false;
    }

    Vec3d hjp = head.junction_point();
    double zangle = std::asin(hit.direction()(Z));
    zangle = std::max(zangle, PI/4);
    double h = std::sin(zangle) * head.fullwidth();

    // The width of the tail head that we would like to have...
    h = std::min(hit.distance() - head.r_back_mm, h);

    // If this is a mini pillar dont bother with the tail width, can be 0.
    if (head.r_back_mm < m_cfg.head_back_radius_mm) h = std::max(h, 0.);
    else if (h <= 0.) return false;

    Vec3d endp{hjp(X), hjp(Y), hjp(Z) - hit.distance() + h};
    auto center_hit = m_mesh.query_ray_hit(hjp, DOWN);

    double hitdiff = center_hit.distance() - hit.distance();
    Vec3d hitp = std::abs(hitdiff) < 2*head.r_back_mm?
                     center_hit.position() : hit.position();

    long pillar_id = m_builder.add_pillar(head.id, hjp.z() - endp.z());
    Pillar &pill = m_builder.pillar(pillar_id);

    Vec3d taildir = (endp - hitp).normalized();
    double dist = (hitp - endp).norm() + m_cfg.head_penetration_mm;
    double w = dist - 2 * head.r_pin_mm - head.r_back_mm;

    if (w < 0.) {
        ////BOOST_LOG_TRIVIAL(error) << "Pinhead width is negative!";
        //CXLogError("Pinhead width is negative!");
        w = 0.;
    }

    m_builder.add_anchor(head.r_back_mm, head.r_pin_mm, w,
                         m_cfg.head_penetration_mm, taildir, hitp);

    m_pillar_index.guarded_insert(pill.endpoint(), pill.id);
    m_pillar_index_start.guarded_insert(pill.startpoint(),
        unsigned(pill.id));

    return true;
}

bool SupportTreeBuildsteps::search_pillar_and_connect(const Head &source)
{
    // Hope that a local copy takes less time than the whole search loop.
    // We also need to remove elements progressively from the copied index.
    PointIndex spindex = m_pillar_index.guarded_clone();

    long nearest_id = SupportTreeNode::ID_UNSET;

    Vec3d querypt = source.junction_point();

    while(nearest_id < 0 && !spindex.empty()) { m_thr();
        // loop until a suitable head is not found
        // if there is a pillar closer than the cluster center
        // (this may happen as the clustering is not perfect)
        // than we will bridge to this closer pillar

        Vec3d qp(querypt(X), querypt(Y), m_builder.ground_level);
        auto qres = spindex.nearest(qp, 1);
        if(qres.empty()) break;

        auto ne = qres.front();
        nearest_id = ne.second;

        if(nearest_id >= 0) {
            if (size_t(nearest_id) < m_builder.pillarcount()) {
                if(!connect_to_nearpillar(source, nearest_id) ||
                    m_builder.pillar(nearest_id).r < source.r_back_mm) {
                    nearest_id = SupportTreeNode::ID_UNSET;    // continue searching
                    spindex.remove(ne);       // without the current pillar
                }
            }
        }
    }

    return nearest_id >= 0;
}
void SupportTreeBuildsteps::routing_to_model()
{
    // We need to check if there is an easy way out to the bed surface.
    // If it can be routed there with a bridge shorter than
    // min_bridge_distance.
    std::cout << __LINE__ << "  " << " step_4 " << std::endl;

    ccr::for_each(m_iheads_onmodel.begin(), m_iheads_onmodel.end(),
                  [this] (const unsigned idx) {
        m_thr();

        auto& head = m_builder.head(idx);

        // Search nearby pillar
        if (search_pillar_and_connect(head)) { return; }

        // Cannot connect to nearby pillar. We will try to search for
        // a route to the ground.
        if (connect_to_ground(head)) { return; }

        // No route to the ground, so connect to the model body as a last resort
        if (connect_to_model_body(head)) { return; }

        // We have failed to route this head.
        ////BOOST_LOG_TRIVIAL(warning)
       //         << "Failed to route model facing support point. ID: " << idx;
        //CXLogWarn("Failed to route model facing support point. ID: %d ",idx);

        head.invalidate();
    });
}

void SupportTreeBuildsteps::interconnect_pillars()
{
    std::cout << __LINE__ << "  " << " step_6 " << std::endl;

    // Now comes the algorithm that connects pillars with each other.
    // Ideally every pillar should be connected with at least one of its
    // neighbors if that neighbor is within max_pillar_link_distance

    // Pillars with height exceeding H1 will require at least one neighbor
    // to connect with. Height exceeding H2 require two neighbors.
    double H1 = m_cfg.max_solo_pillar_height_mm;
    double H2 = m_cfg.max_dual_pillar_height_mm;
    double d = m_cfg.max_pillar_link_distance_mm;

    //A connection between two pillars only counts if the height ratio is
    // bigger than 50%
    double min_height_ratio = 0.5;

    std::set<unsigned long> pairs;

    // A function to connect one pillar with its neighbors. THe number of
    // neighbors is given in the configuration. This function if called
    // for every pillar in the pillar index. A pair of pillar will not
    // be connected multiple times this is ensured by the 'pairs' set which
    // remembers the processed pillar pairs
    auto cascadefn =
        [this, d, &pairs, min_height_ratio, H1] (const PointIndexEl& el)
    {
        Vec3d qp = el.first;    // endpoint of the pillar

         Pillar& pillar = m_builder.pillar(el.second); // actual pillar

        // Get the max number of neighbors a pillar should connect to
        unsigned neighbors = m_cfg.pillar_cascade_neighbors;

        // connections are already enough for the pillar
        if(pillar.links >= neighbors) return;

        double max_d = d * pillar.r / m_cfg.head_back_radius_mm;
        // Query all remaining points within reach
        auto qres = m_pillar_index.query([qp, max_d](const PointIndexEl& e){
            return distance(e.first, qp) < max_d;
        });

        // sort the result by distance (have to check if this is needed)
        std::sort(qres.begin(), qres.end(),
                  [qp](const PointIndexEl& e1, const PointIndexEl& e2){
                      return distance(e1.first, qp) < distance(e2.first, qp);
                  });

        for(auto& re : qres) { // process the queried neighbors

            if(re.second == el.second) continue; // Skip self

            auto a = el.second, b = re.second;

            // Get unique hash for the given pair (order doesn't matter)
            auto hashval = pairhash(a, b);

            // Search for the pair amongst the remembered pairs
            if(pairs.find(hashval) != pairs.end()) continue;

             Pillar& neighborpillar = m_builder.pillar(re.second);
            std::vector<SupportTreeNode2MeshLink> outintermeshlink;
            // this neighbor is occupied, skip
            if (neighborpillar.links >= neighbors) continue;
            if (neighborpillar.r < pillar.r) continue;
            if(interconnect(pillar, neighborpillar, outintermeshlink)) {
                pairs.insert(hashval);

                // If the interconnection length between the two pillars is
                // less than 50% of the longer pillar's height, don't count
                if(pillar.height < H1 ||
                    neighborpillar.height / pillar.height > min_height_ratio)
                    m_builder.increment_links(pillar);

                if(neighborpillar.height < H1 ||
                    pillar.height / neighborpillar.height > min_height_ratio)
                    m_builder.increment_links(neighborpillar);

            }

            // connections are enough for one pillar
            if(pillar.links >= neighbors) break;
        }
    };

    // Run the cascade for the pillars in the index
    m_pillar_index.foreach(cascadefn);

    // We would be done here if we could allow some pillars to not be
    // connected with any neighbors. But this might leave the support tree
    // unprintable.
    //
    // The current solution is to insert additional pillars next to these
    // lonely pillars. One or even two additional pillar might get inserted
    // depending on the length of the lonely pillar.

    size_t pillarcount = m_builder.pillarcount();

    // Again, go through all pillars, this time in the whole support tree
    // not just the index.
    for(size_t pid = 0; pid < pillarcount; pid++) {
        auto pillar = [this, pid]() { return m_builder.pillar(pid); };
        auto radius = m_cfg.head_back_radius_mm;
        // Decide how many additional pillars will be needed:

        unsigned needpillars = 0;
        if (pillar().bridges >= m_cfg.max_bridges_on_pillar)
            needpillars = 3;
        else if (pillar().links < 2 && pillar().height > H2) {
            // Not enough neighbors to support this pillar
            needpillars = 2;
        } else if (pillar().links < 1 && pillar().height > H1) {
            // No neighbors could be found and the pillar is too long.
            needpillars = 1;
        }

        needpillars = std::max(pillar().links, needpillars) - pillar().links;
        if (needpillars == 0) continue;

        // Search for new pillar locations:

        bool   found    = false;
        double alpha    = 0; // goes to 2Pi
        double r        = 2 * m_cfg.base_radius_mm;
        Vec3d  pillarsp = pillar().startpoint();

        // temp value for starting point detection
        Vec3d sp(pillarsp(X), pillarsp(Y), pillarsp(Z) - r);

        // A vector of bool for placement feasbility
        std::vector<bool>  canplace(needpillars, false);
        std::vector<Vec3d> spts(needpillars); // vector of starting points

        double gnd      = m_builder.ground_level;
        double min_dist = m_cfg.pillar_base_safety_distance_mm +
                          m_cfg.base_radius_mm + EPSILON;

        while(!found && alpha < 2*PI) {
            for (unsigned n = 0;
                 n < needpillars && (!n || canplace[n - 1]);
                 n++)
            {
                double a = alpha + n * PI / 3;
                Vec3d  s = sp;
                s(X) += std::cos(a) * r;
                s(Y) += std::sin(a) * r;
                spts[n] = s;

                // Check the path vertically down
                Vec3d check_from = s + Vec3d{0., 0., pillar().r};
                auto hr = bridge_mesh_intersect(check_from, DOWN, radius);
                Vec3d gndsp{s(X), s(Y), gnd};

                // If the path is clear, check for pillar base collisions
#if 0           //对支撑在模型上的主支撑也添加周边护柱
                canplace[n] = std::isinf(hr.distance()) &&
                              std::sqrt(m_mesh.squared_distance(gndsp)) >
                                  min_dist;
#else
                canplace[n] = (hr.distance() > H1);
#endif
            }

            found = std::all_of(canplace.begin(), canplace.end(),
                                [](bool v) { return v; });

            // 20 angles will be tried...
            alpha += 0.1 * PI;
        }

        std::vector<long> newpills;
        newpills.reserve(needpillars);

        if (found)
            for (unsigned n = 0; n < needpillars; n++) {
                Vec3d s = spts[n];
                Pillar p;// (Vec3d{ s.x(), s.y(), gnd }, s.z() - gnd, pillar().r);

                auto hr = bridge_mesh_intersect(s, DOWN, radius);
                double h = 0.0;
                if (hr.is_hit())
                {
                    double width_mm = m_cfg.head_back_radius_mm + 2 * m_cfg.head_front_radius_mm + m_cfg.head_penetration_mm;
                    double zangle = std::asin(hr.direction()(Z));
                    zangle = std::max(zangle, PI / 4);
                    double h = std::sin(zangle) * width_mm;
                    // The width of the tail head that we would like to have...
                    h = std::min(hr.distance() - radius, h);

                    if (h < EPSILON)
                    {
                        continue;
                    }
                    
                     p= Pillar(Vec3d{s.x(), s.y(), hr .position().z()+h}, hr.distance()-h, radius);
                }
                else
                     p= Pillar(Vec3d{ s.x(), s.y(), gnd }, s.z() - gnd, radius);


                std::vector<SupportTreeNode2MeshLink> outintermeshlink;
                auto pp = pillar();
                if (interconnect(pp, p, outintermeshlink)) {
                    //Pillar &pp = m_builder.pillar(m_builder.add_pillar(p));
                    Pillar &ppNew = m_builder.pillar(p.id);
                    Junction jun ;

                    if (hr.is_hit())
                    {
                        double dist = (hr.position() - ppNew.endpt).norm() + m_cfg.head_penetration_mm;
                        double w = dist - 2 * m_cfg.head_front_radius_mm - ppNew.r;

                        Anchor achr=m_builder.add_anchor(ppNew.r, m_cfg.head_front_radius_mm, w, m_cfg.head_penetration_mm, (ppNew.endpt-hr.position()).normalized(), hr.position());
                        m_builder.buildMeshLinkRelation(m_builder.pillar(ppNew.id), m_builder.anchor(achr.id));
                        jun = m_builder.add_junction(ppNew.startpoint(), ppNew.r);
                    }
                    else
                    {
                        add_pillar_base(ppNew.id);
                        jun = m_builder.add_junction(ppNew.startpoint(), ppNew.r);
                    }

                    m_builder.buildMeshLinkRelation(m_builder.junction(jun.id), m_builder.pillar(ppNew.id));
                   
                    m_pillar_index.insert(ppNew.endpoint(), unsigned(ppNew.id));
                    m_pillar_index_start.guarded_insert(ppNew.startpoint(),unsigned(ppNew.id));

                    double t = bridge_mesh_intersect(pillarsp, dirv(pillarsp, ppNew.startpoint()),
                                                    pillar().r,0.0).distance();
                    if (t-distance(pillarsp, ppNew.startpoint())>EPSILON)
                    //if (0)
                    {
                        CrossBridge bri = m_builder.add_crossbridge(pillar().startpoint(), ppNew.startpoint(), pillar().r);

                        m_builder.buildMeshLinkRelation(m_builder.crossbridge(bri.id), m_builder.pillar(pillar().id));
                        m_builder.buildMeshLinkRelation(m_builder.crossbridge(bri.id), m_builder.pillar(ppNew.id));
                        m_builder.crossbridge(bri.id).setbriStartplink(m_builder.pillar(pillar().id));
                    }
                    //if (pillar().endpoint()(Z) > m_builder.ground_level + pillar().r)
                    //    m_builder.add_junction(pillar().endpoint(), pillar().r);

                    newpills.emplace_back(ppNew.id);
                    m_builder.increment_links(pillar());
                    m_builder.increment_links(ppNew);
                }
            }

        if(!newpills.empty()) {
            for(auto it = newpills.begin(), nx = std::next(it);
                 nx != newpills.end(); ++it, ++nx) {
                 Pillar& itpll = m_builder.pillar(*it);
                 Pillar& nxpll = m_builder.pillar(*nx);
                std::vector<SupportTreeNode2MeshLink> outintermeshlink;
                if(interconnect(itpll, nxpll, outintermeshlink)) {
                    m_builder.increment_links(itpll);
                    m_builder.increment_links(nxpll);
                }
            }

            m_pillar_index.foreach(cascadefn);
        }
    }
}
void SupportTreeBuildsteps::dealOneHeadPre(std::vector<Head> &heads)
{
    std::cout << __LINE__ << "  " << " manual add head " << std::endl;
    // Get the points that are too close to each other and keep only the
    // first one
    //auto aliases = cluster(m_points, D_SP, 2);


    PtIndices filtered_indices;
    PointSet pointsets(heads.size(), 3);
    PointSet nmls(heads.size(), 3);
    filtered_indices.reserve(heads.size());
    for (int i = 0; i < heads.size(); i++) {
        // Here we keep only the front point of the cluster.
        filtered_indices.emplace_back((unsigned)i);
        pointsets.row(i)(X) = double(heads[i].pos(X));
        pointsets.row(i)(Y) = double(heads[i].pos(Y));
        pointsets.row(i)(Z) = double(heads[i].pos(Z));

        nmls.row(i)(X) = double(heads[i].dir.x());
        nmls.row(i)(Y) = double(heads[i].dir.y());
        nmls.row(i)(Z) = double(heads[i].dir.z());
        i++;
    }

    // calculate the normals to the triangles for filtered points
     //nmls = sla::normals(pointsets, m_mesh, 2.0,
     //   m_thr, filtered_indices);

    // Not all of the support points have to be a valid position for
    // support creation. The angle may be inappropriate or there may
    // not be enough space for the pinhead. Filtering is applied for
    // these reasons.

    std::function<void(unsigned, size_t)> filterfn;
    filterfn = [this, &nmls, &heads, &filterfn](unsigned fidx, size_t i) {
        m_thr();
        Head& h = heads[fidx];
        auto n = nmls.row(Eigen::Index(i));
        //n = n.normalized();
        //n.normalize();
        // for all normals we generate the spherical coordinates and
        // saturate the polar angle to 45 degrees from the bottom then
        // convert back to standard coordinates to get the new normal.
        // Then we just create a quaternion from the two normals
        // (Quaternion::FromTwoVectors) and apply the rotation to the
        // arrow head.
        //对于所有法线，我们生成球坐标并将极坐标从底部饱和到45度，
        //然后转换回标准坐标以获得新的法线。
        //然后我们从两个法线创建一个四元数 (Quaternion:: fromtwoovectors)和应用旋转到箭头。
        auto [polar, azimuth] = dir_to_spheric(n);

        // skip if the tilt is not sane
        if (polar < PI - m_cfg.normal_cutoff_angle)
        {
            std::cout << __FILE__ << __LINE__ << "cut off angle polar==" << polar * 180.0 / PI << std::endl;
            h.id = SupportTreeNode::ID_UNSET;
            return;
        }

        // We saturate the polar angle to 3pi/4
        polar = std::max(polar, PI - m_cfg.bridge_slope);

        // save the head (pinpoint) position
        Vec3d hp = h.pos;

        //double lmin = h.fullwidth();
        //double lmax = 2.0 * m_cfg.head_front_radius_mm + m_cfg.head_width_mm + 2.0 * m_cfg.head_back_radius_mm;
        //double w = h.real_width();


        double lmin = h.width_mm, lmax = m_cfg.head_width_mm;

        if (h.r_back_mm < m_cfg.head_back_radius_mm) {
            lmin = EPSILON, lmax = m_cfg.head_penetration_mm;
        }

        // The distance needed for a pinhead to not collide with model.
        double w = lmin + 2 * h.r_pin_mm + 2 * h.r_back_mm - h.penetration_mm;

        if (w < EPSILON)
        {
            std::cout << __FILE__ << __LINE__ << "too samll w==" << w << std::endl;
            h.id = SupportTreeNode::ID_UNSET;
            return;
        }
        double pin_r = h.r_pin_mm;
        double back_r = h.r_back_mm;
        // Reassemble the now corrected normal
        auto nn = spheric_to_dir(polar, azimuth).normalized();

        // check available distance
        //IndexedMesh::hit_result t = pinhead_mesh_intersect(hp, nn, pin_r, back_r, w, m_cfg.safety_distance_mm);
        IndexedMesh::hit_result t = bridge_mesh_intersect(hp, nn, back_r);
        if (t.distance() < w) {

            Optimizer<AlgNLoptGenetic> solver(get_criteria(m_cfg));
            solver.seed(0); // we want deterministic behavior

            auto oresult = solver.to_max().optimize(
                [this, pin_r, back_r, hp](const opt::Input<3>& input)
                {
                    auto& [plr, azm, l] = input;

                    auto dir = spheric_to_dir(plr, azm).normalized();

                    auto hitInner = pinhead_mesh_intersect(
                        hp, dir, pin_r, back_r, l, m_cfg.safety_distance_mm);
                    return hitInner.distance();
                },
                initvals({ polar, azimuth,(lmin + lmax) / 2.0 }), // start with what we have
                    bounds({
                        {PI - m_cfg.bridge_slope, PI},    // Must not exceed the slope limit
                        {-PI, PI}, // azimuth can be a full search
                        {lmin, lmax}
                        }));

            polar = std::get<0>(oresult.optimum);
            azimuth = std::get<1>(oresult.optimum);
            auto nntemp = spheric_to_dir(polar, azimuth).normalized();

            if (oresult.score > w&& 
                (hp(Z) + w * nntemp(Z) >= m_builder.ground_level)
                ) 
            {
                nn = nntemp;
                lmin = std::get<2>(oresult.optimum);
                t = IndexedMesh::hit_result(oresult.score);
                //std::cout << "oresult.score==" << oresult.score << std::endl;

            }
        }
       // t = pinhead_mesh_intersect(hp, nn, pin_r, back_r, lmin - 2.0 * pin_r - 2.0 * back_r, m_cfg.safety_distance_mm);
        //std::cout << "t.distance==" << t.distance() << std::endl;
        if (t.distance() > w &&
            (hp(Z) + w * nn(Z) >= m_builder.ground_level+ back_r+EPSILON)
            )
        {
                h.dir = nn;
                h.width_mm = lmin;
                h.r_pin_mm = pin_r;
                h.r_back_mm = back_r;
        }
        else if (h.width_mm > m_cfg.head_width_min_mm)//优先减少长度
        {
            h.width_mm -= m_cfg.head_width_min_mm;
            filterfn(fidx, i);
        }
        else if (h.r_pin_mm > m_cfg.head_fallback_radius_mm)//优先减少长度
        {
            h.r_pin_mm = m_cfg.head_fallback_radius_mm;
            filterfn(fidx, i);
        }
        else if (h.r_back_mm >  m_cfg.head_fallback_radius_mm)//长度为零时，减少支撑半径之类
        {
            h.r_back_mm = m_cfg.head_fallback_radius_mm;
            filterfn(fidx, i);
        }
        else if (h.penetration_mm >  m_cfg.head_width_min_mm)//减少支撑嵌入深度
        {
            h.penetration_mm  -= m_cfg.head_width_min_mm;
            filterfn(fidx, i);
        }
        else
        {
            std::cout << __FILE__ << __LINE__ << "too samll w==" << w << std::endl;
            h.id = SupportTreeNode::ID_UNSET;
        }
    };

    ccr::for_each(size_t(0), filtered_indices.size(),
        [this, &filterfn, &filtered_indices](size_t i) {
            filterfn(filtered_indices[i], i);
        });


}

bool SupportTreeBuildsteps::add_pinheadSupport(const Head& source, int id)
{
    bool ret = false;
    std::vector<Head> addheads;
    addheads.emplace_back(source);
    addheads.back().id = id;
    dealOneHeadPre(addheads);
    for (size_t i = 0; i < addheads.size(); ++i)
        if (addheads[i].is_valid()) {
            m_builder.add_head(addheads[i].id, addheads[i]);
            m_iheads.emplace_back(addheads[i].id);
        }

    //m_support_pts.emplace_back(pts);
    for (int index=0; index< addheads.size();index++)
    {
        if (addheads[index].id == SupportTreeNode::ID_UNSET)
        {
            continue;
        }
		auto& hd = m_builder.head(addheads[index].id);
        if (!hd.is_valid())
            continue;
        std::cout << __LINE__ << " head pos===" << hd.pos << std::endl;

        
        {
            PointIndex pointIndex = m_pillar_index_start.guarded_clone();
            std::cout << "m_pillar_index size==" << pointIndex.size() << std::endl;
            std::vector<PointIndexEl> ptsindex;
            Vec3d ptStart = hd.junction_point();
            double radius = m_cfg.head_back_radius_mm;//hd.r_back_mm;
            PointIndex pointIndexFirst = pointIndex;
            PointIndexEl idfirstE1;
            long idFirst = searchSidePillarPoint(pointIndexFirst, ptStart, idfirstE1);
            PointIndexEl e1 = idfirstE1;
            while (idFirst > SupportTreeNode::ID_UNSET)
            {
                pointIndexFirst.remove(idfirstE1);
                PointIndex pointIndexSecond = pointIndexFirst;
                PointIndexEl idsecondE1;
                long idSecond = searchSidePillarPoint(pointIndexSecond, ptStart, idsecondE1);
                while (idSecond > SupportTreeNode::ID_UNSET)
                {

                    Vec3d pt1(ptStart(X), ptStart(Y), 0.0);
                    Vec3d pt2(idfirstE1.first(X), idfirstE1.first(Y), 0.0);
                    Vec3d pt3(idsecondE1.first(X), idsecondE1.first(Y), 0.0);
                    Vec3d pt12 = pt2 - pt1;
                    Vec3d pt13 = pt3 - pt1;
                    pt12.normalize();
                    pt13.normalize();
                    float angle = std::acos(pt12.dot(pt13)) * 180.0 / PI;
                    pointIndexSecond.remove(idsecondE1);

                    if (angle > 120.0)
                    {
                        ptsindex.emplace_back(idfirstE1);
                        ptsindex.emplace_back(idsecondE1);
                        goto SEARCH_END;
                    }
                    else
                    {
                        idSecond = searchSidePillarPoint(pointIndexSecond, ptStart, idsecondE1);
                    }
                }

                idFirst = searchSidePillarPoint(pointIndexFirst, ptStart, idfirstE1);

            }
        SEARCH_END:
            switch (ptsindex.size())
            {
            case 2:
            {
                Vec3d posNew = ptStart;
                double deltZ = 0;
                for (auto& ptindex : ptsindex)
                {
                    if (ptStart(Z) > ptindex.first(Z))
                    {
                        posNew(Z) = ptindex.first(Z);
                    }
                }
                deltZ = ptStart(Z) - posNew(Z);
                if (deltZ < 3.0)
                {
                    deltZ = 3.0;
                }
                posNew = ptStart;
                posNew(Z) -= deltZ;
                auto centerhit = bridge_mesh_intersect(ptStart, DOWN, radius);
                if (centerhit.distance() > deltZ)
                {
                    long firstSidepillarID = ptsindex[0].second;
                    long secondSidepillarID = ptsindex[1].second;
                    std::vector<SupportTreeNode2MeshLink> outbrilinkfirst;
                    std::vector<SupportTreeNode2MeshLink> outbrilinksecond;
                    bool firstret = bridge_to_nearpillar(posNew, radius, firstSidepillarID, outbrilinkfirst);
                    bool secondret = bridge_to_nearpillar(posNew, radius, secondSidepillarID, outbrilinksecond);
                    if (firstret == true ||
                        secondret == true
                        )
                    {
                        Junction junc;
                        long id = SupportTreeNode::ID_UNSET;
                        junc = m_builder.add_junction(ptStart, radius);
                        m_builder.buildMeshLinkRelation(hd, m_builder.junction(junc.id));
                        id = m_builder.add_pillar(posNew, deltZ, radius);
                        m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.pillar(id));
                        junc = m_builder.add_junction(posNew, radius);
                        m_builder.buildMeshLinkRelation(m_builder.pillar(id), m_builder.junction(junc.id));
                        if (firstret == true)
                        {
                            SupportTreeNode* treeNodeobjPtr = NULL;
                            if (m_builder.supportTreeNodeIsValid(outbrilinkfirst[0], &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                                }
                                break;
                                default:
                                    break;
                                }
                            }

                        }
                        if (secondret == true)
                        {
                            SupportTreeNode* treeNodeobjPtr = NULL;
                            if (m_builder.supportTreeNodeIsValid(outbrilinksecond[0], &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                                }
                                break;
                                default:
                                    break;
                                }
                            }

                        }

                        ret = true;
                    }
                }

            }
            break;
            default:
            {
                PointIndexEl idfirstE1;
                long id_first = searchSidePillarPoint(pointIndex, ptStart, idfirstE1);
                if (id_first > SupportTreeNode::ID_UNSET)
                {
                    Vec3d posNew = idfirstE1.first;
                    long firstSidepillarID = idfirstE1.second;
                    double deltZ = 0;
                    deltZ = ptStart(Z) - posNew(Z);
                    if (deltZ < 3.0)
                    {
                        deltZ = 3.0;
                    }
                    posNew = ptStart;
                    posNew(Z) -= deltZ;
                    auto centerhit = bridge_mesh_intersect(ptStart, DOWN, radius);
                    if (centerhit.distance() > deltZ)
                    {
                        std::vector<SupportTreeNode2MeshLink> outbrilinkfirst;
                        bool firstret = bridge_to_nearpillar(posNew, radius, firstSidepillarID, outbrilinkfirst);
                        if (firstret)
                        {
                            Junction junc;
                            long id = SupportTreeNode::ID_UNSET;
                            junc = m_builder.add_junction(ptStart, radius);
                            m_builder.buildMeshLinkRelation(hd, m_builder.junction(junc.id));
                            id = m_builder.add_pillar(posNew, deltZ, radius);
                            m_builder.pillar(id).bridgePillar = true;
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.pillar(id));
                            junc = m_builder.add_junction(posNew, radius);
                            m_builder.buildMeshLinkRelation(m_builder.pillar(id), m_builder.junction(junc.id));
                            SupportTreeNode* treeNodeobjPtr = NULL;
                            if (m_builder.supportTreeNodeIsValid(outbrilinkfirst[0], &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                                }
                                break;
                                default:
                                    break;
                                }
                            }
                            ret = true;
                        }
                    }
                }
            }
            break;
            }
        }
        if (ret == false)
        {
            std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
            auto ptStarthit = bridge_mesh_intersect(hd.junction_point(), DOWN, m_cfg.head_back_radius_mm);
            if (std::isinf(ptStarthit.distance()))
            {
                if (create_ground_pillar(hd.junction_point(), DOWN, hd.r_back_mm, SupportTreeNode::ID_UNSET, outTreeNodelink))
                {
                    std::cout << "search_pillar_and_connect" << std::endl;
                    Junction junc;
                    junc = m_builder.add_junction(hd.junction_point(), hd.r_back_mm);
                    SupportTreeNode* treeNodeobjPtr = NULL;
                    m_builder.buildMeshLinkRelation(m_builder.head(hd.id), m_builder.junction(junc.id));
                    if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                    {
                        if (treeNodeobjPtr)
                        {
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                            switch (treeNodeobjPtr->treeNodeType)
                            {
                            case MeshTypeElement::Bridge:
                            {
                                m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                            }
                            break;
                            case MeshTypeElement::DiffBridge:
                            {
                                m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                            }
                            break;
                            default:
                                break;
                            }
                        }
                    }

                    ret = true;
                }
            }
        }
        if (ret == false)
        {
            std::optional<DiffBridge> diffbr =
                search_widening_path_pillar(hd.junction_point(), hd.dir.normalized(), m_cfg.head_back_radius_mm, m_cfg.head_back_radius_mm);
            bool differbrValiable = false;
            if (diffbr && (diffbr->endp.z() > m_cfg.min_bridge_height_mm))
            {
                auto diffhit = bridge_mesh_intersect(diffbr->endp, DOWN, diffbr->end_r);
                auto hdhit = pinhead_mesh_intersect(diffbr->startp, (diffbr->endp - diffbr->startp).normalized(), diffbr->r, diffbr->end_r, (diffbr->endp - diffbr->startp).norm());
                if (std::isinf(diffhit.distance()) &&
                    std::isinf(hdhit.distance())
                    )
                {
                    std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
                    if (create_ground_pillar(diffbr->endp, DOWN, diffbr->end_r, SupportTreeNode::ID_UNSET, outTreeNodelink))
                    {
                        std::cout << "search_pillar_and_connect" << std::endl;
                        Junction junc;
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        DiffBridge bri;

                        junc = m_builder.add_junction(hd.junction_point(), hd.r_back_mm);
                        bri = m_builder.add_diffbridge(*diffbr);
                        m_builder.buildMeshLinkRelation(m_builder.head(hd.id), m_builder.junction(junc.id));
                        m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.diffbridge(bri.id));
                        m_builder.diffbridge(bri.id).setbriStartplink(m_builder.junction(junc.id));

                        junc = m_builder.add_junction(bri.endp, bri.end_r);
                        m_builder.buildMeshLinkRelation(m_builder.diffbridge(bri.id), m_builder.junction(junc.id));
                        if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                        {
                            if (treeNodeobjPtr)
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                                }
                                break;
                                default:
                                    break;
                                }
                            }
                        }

                        ret = true;
                        differbrValiable = true;
                    }


                }
            }
        }
        if (ret == false)
        {
            std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
            PointIndex pointIndex = m_pillar_index_start.guarded_clone();
            std::vector<PointIndexEl> ptsindex;
            Vec3d ptStart = hd.junction_point();
            double radius = hd.r_back_mm;
            if (connect_to_model_body(ptStart, radius, outTreeNodelink))
            {
                Junction junc;
                junc = m_builder.add_junction(ptStart, radius);

                SupportTreeNode* treeNodeobjPtr = NULL;
                m_builder.buildMeshLinkRelation(hd, m_builder.junction(junc.id));
                if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                {
                    m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                }
                ret = true;
            }


        }
        if (ret == false)
        {
            //std::cout << __LINE__ << "Failed to route model facing support point. ID: " << source.id << std::endl;

            hd.invalidate();
            /////////////////////////////////////////////////////
            auto item = std::find_if(m_builder.m_heads.begin(), m_builder.m_heads.end(),
                [hd](Head i)->bool {
                    return (hd.treeNodeType == i.treeNodeType) && (hd.id == i.id);
                });
            if (item != m_builder.m_heads.end())
            {
                m_builder.m_heads.erase(item);

                m_builder.m_head_indices.clear();
                m_iheads.clear();

                for (long index = 0; index < m_builder.m_heads.size(); index++)
                {
                    Head& hdsrc = m_builder.m_heads[index];
                    if (hdsrc.id > SupportTreeNode::ID_UNSET)
                    {
                        if (hdsrc.id >= m_builder.m_head_indices.size())
                            m_builder.m_head_indices.resize(hdsrc.id + 1);
                        m_builder.m_head_indices[hdsrc.id] = index;
                        m_iheads.emplace_back(hdsrc.id);

                    }
                }

            }

        }

    }
    return ret;

}

bool SupportTreeBuildsteps::createSmallPillar(SupportTreeNode2MeshLink hdlk,std::vector<SupportTreeNode2MeshLink> &outnewmeshlk)
{
    auto templk = hdlk;
    bool ret = false;
    if ((templk.type == MeshTypeElement::Head) && (templk.id > SupportTreeNode::ID_UNSET))
    {
        Head hd = m_builder.head(templk.id);
        auto pstart = hd.pos;
        // if (hd.postype == (int)Slic3r::sla::IslandType::isTypeNew ||
        //     hd.postype == (int)Slic3r::sla::IslandType::isTypeOverhangs)
        {
            float radius = m_cfg.head_back_radius_mm;
            //std::cout << "*****new radius==" << radius << std::endl;
            //std::cout << "*****head_fallback_radius_mm radius==" << m_cfg.head_fallback_radius_mm << std::endl;

            while (radius > m_cfg.head_fallback_radius_mm)
            {
                //std::cout << "radius==" << radius << std::endl;

                Vec3d endpt = pstart;
                double len = 0.0;
                //auto brihit = pinhead_mesh_intersect(ptStart, DOWN, radius, radius, width,0.0);
                auto brihit = bridge_mesh_intersect(pstart, DOWN, radius, 0.0);
                if (brihit.distance() < m_cfg.head_width_min_mm)
                {
                    radius -= 0.1;
                    continue;
                }
                endpt = brihit.position();
                if (std::isinf(brihit.distance()))
                {
                    endpt = Vec3d(pstart(X), pstart(Y), m_builder.ground_level);
                }
                len = pstart(Z) - endpt(Z);
                //std::cout << "len==" << len << std::endl;

                if (len < m_cfg.head_width_mm)
                {
                    auto testhit = bridge_mesh_intersect(pstart, DOWN, m_cfg.head_fallback_radius_mm, 0.0);
                    if ((testhit.distance() > EPSILON &&
                        testhit.is_hit() &&
                        brihit.is_hit())
                        )
                    {
                        radius = m_cfg.head_fallback_radius_mm;
                        brihit = testhit;
                        endpt = brihit.position();
                        if (std::isinf(brihit.distance()))
                        {
                            endpt = Vec3d(pstart(X), pstart(Y), m_builder.ground_level);
                        }
                        len = pstart(Z) - endpt(Z);

                    }

                }
                if (len > EPSILON)
                {
                    if (std::isinf(brihit.distance()))
                    {
                        long id = -1;
                        if (len > m_cfg.head_width_min_mm)
                        {
                            id = m_builder.add_pillar(endpt, len, radius);
                            m_builder.pillar(id).selfType = 1;
                            if ((m_builder.pillar(id).startpoint()(Z) - m_builder.ground_level) > radius)
                            {
                                Junction juncUp = m_builder.add_junction(m_builder.pillar(id).startpoint(), radius);
                                m_builder.buildMeshLinkRelation(m_builder.junction(juncUp.id), m_builder.pillar(id));
                            }
                            if ((m_builder.pillar(id).startpoint()(Z) - m_builder.ground_level) > m_cfg.base_height_mm)
                            {
                                add_pillar_base(id);
                            }
                            m_pillar_index.insert(m_builder.pillar(id).endpoint(), id);
                            m_pillar_index_start.guarded_insert(m_builder.pillar(id).startpoint(), id);
                            ret = true;
                        }
                        goto WHILE_END;

                    }
                    else
                    {
                        long id = -1;
                        if (len < m_cfg.head_width_min_mm)
                        {
                            IndexedMesh::hit_result rayhit = m_builder.m_supportTreeBuildstepsPtr->m_mesh.query_ray_hit(pstart, DOWN);
                            if (rayhit.distance() < m_cfg.head_width_min_mm)
                            {
                                goto WHILE_END;
                            }
                        }
                        else
                        {
                            id = m_builder.add_pillar(endpt, len, radius);
                            m_builder.pillar(id).selfType = 1;
                            if ((m_builder.pillar(id).startpoint()(Z) - m_builder.ground_level) > radius)
                            {
                                Junction juncUp = m_builder.add_junction(m_builder.pillar(id).startpoint(), radius);
                                m_builder.buildMeshLinkRelation(m_builder.junction(juncUp.id), m_builder.pillar(id));
                            }
                            if ((m_builder.pillar(id).endpoint()(Z) - m_builder.ground_level) > radius)
                            {
                                Junction juncDown = m_builder.add_junction(m_builder.pillar(id).endpoint(), radius);
                                m_builder.buildMeshLinkRelation(m_builder.pillar(id), m_builder.junction(juncDown.id));
                            }

                            m_pillar_index.insert(m_builder.pillar(id).endpoint(), id);
                            m_pillar_index_start.guarded_insert(m_builder.pillar(id).startpoint(), id);
                            ret = true;
                            goto WHILE_END;
                        }

                    }
                }
                radius -= 0.1;
            }
        WHILE_END:
            ;
        }
    }
    else
    {
        std::cout << __LINE__ << " invalidate type==" << (int)templk.type << std::endl;

    }
    return ret;
}


void SupportTreeBuildsteps::releaseSupportdata()
{
    m_iheads.clear();            // support points with pinhead
    m_iheads_onmodel.clear();
    m_iheadless.clear();         // headless support points
   // m_iheadsAdd.clear();
    m_head_to_ground_scans.clear();
    // normals for support points from model faces.
    //m_support_nmls.resize(0);
    PillarIndex temp;
    m_pillar_index= temp;
    m_pillar_index_start= temp;

    m_pillar_clusters.clear();
    //m_points.resize(0);
    std::cout << "SupportTreeBuildsteps release!!!! " << std::endl;
}


void SupportTreeBuildsteps::updateSupportParamsCfg(const SupportTreeConfig& cfg)
{
    m_cfg = cfg;
}
bool SupportTreeBuildsteps::bridge_to_nearpillar(const Vec3d& pt,double radius, long  nearpillar_id, std::vector<SupportTreeNode2MeshLink>& outbrilinks)
{
    auto nearpillar = [this, nearpillar_id]() -> const Pillar& {
        return m_builder.pillar(nearpillar_id);
    };
    if (m_builder.bridgecount(nearpillar()) >= m_cfg.max_bridges_on_pillar)
        return false;
    Vec3d headjp = pt;
    Vec3d nearjp_u = nearpillar().startpoint();
    Vec3d nearjp_l = nearpillar().endpoint();
    Vec3d bridgestart = headjp;
    Vec3d bridgeend = nearjp_u;

    double r = radius;
    double d2d = distance(to_2d(headjp), to_2d(nearjp_u));
    double d3d = distance(headjp, nearjp_u);


    double max_len =  m_cfg.max_bridge_length_mm;
    double max_slope = m_cfg.bridge_slope;
    double hdiff = nearjp_u(Z) - headjp(Z);
    double slope = std::atan2(hdiff, d2d);

    //bridgestart(Z) -= bridgePillarLen;
    double Zdown = headjp(Z)+ d2d * std::tan(-max_slope);//斜着角度的垂直高度
    Vec3d touchjp = bridgeend;
    touchjp(Z) = Zdown;

    double D = distance(bridgestart, touchjp);

    if (Zdown <= nearjp_u(Z) && Zdown >= nearjp_l(Z) && D < max_len)
        bridgeend(Z) = Zdown;
    else
        return false;


    // There will be a minimum distance from the ground where the
    // bridge is allowed to connect. This is an empiric value.
    double minz = m_builder.ground_level + 4 * r;
    if (bridgeend(Z) < minz)
        return false;
    if (!is_approx(dirv(bridgestart, bridgeend).norm(), 1.))
        return false;
    double t = bridge_mesh_distance(bridgestart, dirv(bridgestart, bridgeend), r);

    // Cannot insert the bridge. (further search might not worth the hassle)
    if (t > distance(bridgestart, bridgeend)&&(slope > -max_slope))
     {
        

        std::lock_guard<ccr::BlockingMutex> lk(m_bridge_mutex);
        if (r < m_cfg.head_back_radius_mm|| nearpillar().r< m_cfg.head_back_radius_mm)
        {
            auto brihit = pinhead_mesh_intersect(bridgestart, dirv(bridgestart, bridgeend), r, m_builder.pillar(nearpillar_id).r, distance(bridgestart, bridgeend));
            if (!brihit.is_hit())
            {
                DiffBridge bri = m_builder.add_diffbridge(DiffBridge(bridgestart, bridgeend, r, m_builder.pillar(nearpillar_id).r));

                m_builder.buildMeshLinkRelation(m_builder.diffbridge(bri.id), m_builder.pillar(nearpillar_id));
                {
                    SupportTreeNode2MeshLink meshlinktemp;
                    meshlinktemp.id = bri.id;
                    meshlinktemp.type = bri.treeNodeType;
                    outbrilinks.emplace_back(meshlinktemp);
                }
            }
            else
                return false;
        }
        else
        {
            Bridge bri = m_builder.add_bridge(bridgestart, bridgeend, r);

            m_builder.buildMeshLinkRelation(m_builder.bridge(bri.id), m_builder.pillar(nearpillar_id));
            {
                SupportTreeNode2MeshLink meshlinktemp;
                meshlinktemp.id = bri.id;
                meshlinktemp.type = bri.treeNodeType;
                outbrilinks.emplace_back(meshlinktemp);
            }
        }

        m_builder.increment_bridges(nearpillar());//hyt
    }
    else 
        return false;

    return true;
}
std::optional<DiffBridge> SupportTreeBuildsteps::search_widening_path_pillar(
    const Vec3d& jp, const Vec3d& dir, double radius, double new_radius)
{
    double w = radius + 2 * new_radius;
    double wMax = m_cfg.max_bridge_length_mm;
    double stopval = w;
    //do
    {
        Optimizer<AlgNLoptGenetic> solver(get_criteria(m_cfg).stop_score(stopval));
        StopCriteria  stopCriteria=solver.get_criteria();
        stopCriteria.max_iterations(100);
        solver.set_criteria(stopCriteria);
        solver.seed(0); // we want deterministic behavior

        //std::cout << "wMax==" << wMax << std::endl;
        auto [polar, azimuth] = dir_to_spheric(dir);

        auto oresult = solver.to_max().optimize(
            [this, jp, radius, new_radius](const opt::Input<3>& input) {
                auto& [plr, azm, t] = input;

                auto   d = spheric_to_dir(plr, azm).normalized();
                double ret = 0.0;
                auto pinheadhit = cone_mesh_intersect(jp, d, radius, new_radius, t, m_cfg.safety_distance_mm);
                IndexedMesh::hit_result downhit = bridge_mesh_intersect(jp + t * d, DOWN, new_radius);
                double down = downhit.distance();
                if ((pinheadhit.distance()>t) &&
                    (!pinheadhit.is_inside()) &&
                    std::isinf(down) && 
                    (t< m_cfg.max_bridge_length_mm)
                    )
                    ret = t;

                return ret;
            },
            initvals({ polar, azimuth,  wMax- w}), // start with what we have
                bounds({
                    {PI - m_cfg.bridge_slope, PI}, // Must not exceed the slope limit
                    {-PI, PI}, // azimuth can be a full search
                    {w,wMax}
                    }));

        if (oresult.score > stopval) {
            polar = std::get<0>(oresult.optimum);
            azimuth = std::get<1>(oresult.optimum);
            double t = std::get<2>(oresult.optimum);
            Vec3d  endp = jp + (t - new_radius) * spheric_to_dir(polar, azimuth);

            return DiffBridge(jp, endp, radius, new_radius);
        }
        //    wMax += m_cfg.max_bridge_length_mm / 2;
    }; //while (wMax < m_cfg.max_bridge_length_mm * 10);
    return {};
}
long SupportTreeBuildsteps::search_pillar(PointIndex &spindex,Vec3d pt)
{
    // Hope that a local copy takes less time than the whole search loop.
    // We also need to remove elements progressively from the copied index.
   // PointIndex spindex = m_pillar_index.guarded_clone();
    long nearest_id = SupportTreeNode::ID_UNSET;
    Vec3d querypt = pt;

    while (nearest_id < 0 && !spindex.empty()) 
    {
        m_thr();
        // loop until a suitable head is not found
        // if there is a pillar closer than the cluster center
        // (this may happen as the clustering is not perfect)
        // than we will bridge to this closer pillar

        //Vec3d qp(querypt(X), querypt(Y), m_builder.ground_level);
        Vec3d qp = querypt;
        auto qres = spindex.nearest(qp, 1);
        if (qres.empty()) break;

        auto ne = qres.front();
        nearest_id = ne.second;

        if (!m_builder.pillar(nearest_id).is_valid())
        {
            nearest_id = SupportTreeNode::ID_UNSET;    // continue searching
            spindex.remove(ne);       // without the current pillar
        }

        if (nearest_id >= 0) {
            if (size_t(nearest_id) > m_builder.pillarcount()) 
                {
                    nearest_id = SupportTreeNode::ID_UNSET;    // continue searching
                    spindex.remove(ne);       // without the current pillar
                }
            else
                spindex.remove(ne);       // without the current pillar

        }
    }

    return nearest_id ;
}

long SupportTreeBuildsteps::searchSidePillarPoint(PointIndex& spindex, Vec3d pt, PointIndexEl &outptIndexE1)
{
    // Hope that a local copy takes less time than the whole search loop.
    // We also need to remove elements progressively from the copied index.
   // PointIndex spindex = m_pillar_index.guarded_clone();
    long nearest_id = SupportTreeNode::ID_UNSET;
    PointIndexEl ret = std::make_pair(pt, nearest_id);
    auto qres = spindex.nearest(pt, 1);
    if (!qres.empty())
    {
        auto ne = qres.front();

        double d3d = distance(pt, ne.first);
        //double d2d = distance(to_2d(pt), to_2d(ne.first));
        //bool suitableflg = d3d < m_cfg.max_pillar_link_distance_mm &&
        //                   d2d>2*m_cfg.base_radius_mm;
        bool suitableflg = d3d < m_cfg.max_pillar_link_distance_mm;

        if (suitableflg)
        {
            outptIndexE1 = ne;
            nearest_id = outptIndexE1.second;
        }

    }

    return nearest_id;
}
std::vector<PointIndexEl> SupportTreeBuildsteps::searchSidePillarPoint(PointIndex& spindex, const PointIndexEl& e1)
{
    auto predicate = [this,e1](const PointIndexEl& e2) {
            double d2d = distance(to_2d(e1.first), to_2d(e2.first));
            double d3d = distance(e1.first, e2.first);
            return d2d < m_cfg.max_pillar_link_distance_mm
                && d3d < m_cfg.max_bridge_length_mm;
    };    
    return spindex.query(predicate);
}
bool SupportTreeBuildsteps::connect_to_model_body(const Vec3d& ptStart, double radius, std::vector<SupportTreeNode2MeshLink>& outTreeNodelink)
{
    SupportTreeNode2MeshLink newMehslink;
    
    auto hit = bridge_mesh_intersect(ptStart, DOWN, radius);

    double r_pin_mm = m_cfg.head_front_radius_mm;
    double r_back_mm = radius;
    double width_mm = r_back_mm+ 2*r_back_mm+ m_cfg.head_penetration_mm;
    outTreeNodelink.clear();
    if (hit.distance() < m_cfg.head_width_min_mm)
        return false;
    if (std::isinf(hit.distance())) {
        // TODO scan for potential anchor points on model surface
        //std::cout << __FILE__ << "  " << __LINE__ << " " << __FUNCTION__ << std::endl;
        //std::cout << "should hit model"<< std::endl;
        if (create_ground_pillar(ptStart, DOWN, radius, SupportTreeNode::ID_UNSET, outTreeNodelink))
        {
            std::cout  << __LINE__ <<"should hit model,but create pillar" << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }
    double zangle = std::asin(hit.direction()(Z));
    zangle = std::max(zangle, PI / 4);
    double h = std::sin(zangle) * width_mm;
    // The width of the tail head that we would like to have...
    h = std::min(hit.distance() - r_back_mm, h);

    // If this is a mini pillar dont bother with the tail width, can be 0.
    if (h <= 0.)
    {
       // std::cout << __FILE__ << "  " << __LINE__ << " " << __FUNCTION__ << std::endl;
       // std::cout << " h should be large" << std::endl;

        return false;

    }

    Vec3d endp{ ptStart(X), ptStart(Y), ptStart(Z) - hit.distance() + h };
    auto center_hit = m_mesh.query_ray_hit(ptStart, DOWN);

    double hitdiff = center_hit.distance() - hit.distance();
    Vec3d hitp = std::abs(hitdiff) < 2 * r_back_mm ?
        center_hit.position() : hit.position();

    long pillar_id = m_builder.addPillarBasePt(endp, ptStart(Z) - endp(Z), r_back_mm);
    Pillar& pill = m_builder.pillar(pillar_id);
    {
        newMehslink.id = pillar_id;
        newMehslink.type = MeshTypeElement::Pillar;
        outTreeNodelink.emplace_back(newMehslink);
    }

    Vec3d taildir = (endp - hitp).normalized();
    double dist = (hitp - endp).norm() + m_cfg.head_penetration_mm;
    double w = dist - 2 * r_pin_mm - r_back_mm;

    if (w < 0.) {
        //std::cout << __FILE__ << "  " << __LINE__ << " " << __FUNCTION__ << std::endl;
        //std::cout  << "Pinhead width is negative!" << std::endl;;
        w = 0.;
    }

    Anchor& achr=m_builder.add_anchor(r_back_mm, r_pin_mm, w,m_cfg.head_penetration_mm, taildir, hitp);
    {
        newMehslink.id = achr.id;
        newMehslink.type = MeshTypeElement::Anchor;
        outTreeNodelink.emplace_back(newMehslink);
    }
    m_builder.buildMeshLinkRelation(m_builder.pillar(pill.id), m_builder.anchor(achr.id));
    m_pillar_index.guarded_insert(pill.endpoint(), pill.id);
    m_pillar_index_start.guarded_insert(pill.startpoint(), pill.id);

    return true;
}
void SupportTreeBuildsteps::CX_routing_to_model()
{
    std::cout << __LINE__ << "  " << " step_5 " << std::endl;
#ifdef USE_ORIGINAL_POINTS
    {
        return;
    }
#endif
    for (int index=0;index< m_TreeNodesNext.size();index++)
    {
            m_thr();
            Vec3d  pstart;
            Vec3d posNew = pstart;
            double deltZ = 0.0;
            double radius = 0.0;
            std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
            const SupportTreeNode2MeshLink &ptTreeNode = m_TreeNodesNext[index];
            switch (ptTreeNode.type)
            {
            case MeshTypeElement::Head:
            {
                auto& obj = m_builder.head(ptTreeNode.id);
                pstart = obj.junction_point();
                radius = obj.r_back_mm;
            }
            break;
            case MeshTypeElement::Pillar:
            {
                auto& obj = m_builder.pillar(ptTreeNode.id);
                pstart = obj.endpoint();
                radius = obj.r;

            }
            break;
            case MeshTypeElement::Bridge:
            {
                auto& obj = m_builder.bridge(ptTreeNode.id);
                pstart = obj.endp;
                radius = obj.r;

            }
            break;
            case MeshTypeElement::DiffBridge:
            {
                auto& obj = m_builder.diffbridge(ptTreeNode.id);
                pstart = obj.endp;
                radius = obj.end_r;

            }
            break;
            case MeshTypeElement::Junction:
            {
                auto& obj = m_builder.junction(ptTreeNode.id);
                pstart = obj.pos;
                radius = obj.r;
            }
            break;
            default:
                std::cout << "unknown ptTreeNode->treeNodeType==" << (int)ptTreeNode.type << std::endl;
                break;
            }
            if (m_cfg.ground_facing_only)
            {
                SupportTreeNode* treeNodePtr = NULL;
                if (m_builder.supportTreeNodeIsValid(ptTreeNode, &treeNodePtr))
                {
                    std::vector < SupportTreeNode2MeshLink> outmeshlinks;
                    m_builder.searchLeftTreeElement(*treeNodePtr, outmeshlinks);
                    for (auto& templk : outmeshlinks)
                    {
                        //break;
                        if ((templk.type == MeshTypeElement::Head) && (templk.id > SupportTreeNode::ID_UNSET))
                        {
                            Head hd = m_builder.head(templk.id);
                            pstart = hd.pos;
                            auto brihit = bridge_mesh_intersect(pstart, DOWN, radius, 0.0);
                            while (radius > m_cfg.head_fallback_radius_mm)
                            {
                                if (std::isinf(brihit.distance()))
                                {
                                    long id = -1;
                                    Vec3d endpt = pstart;
                                    double len = 0.0;

                                    endpt = Vec3d(pstart(X), pstart(Y), m_builder.ground_level);
                                    len = pstart(Z) - endpt(Z);
                                    if (len > EPSILON)
                                    {
                                        id = m_builder.add_pillar(endpt, len, radius);
                                        m_builder.pillar(id).selfType = 1;
                                        if ((m_builder.pillar(id).startpoint()(Z) - m_builder.ground_level) > radius)
                                        {
                                            Junction juncUp = m_builder.add_junction(m_builder.pillar(id).startpoint(), radius);
                                            m_builder.buildMeshLinkRelation(m_builder.junction(juncUp.id), m_builder.pillar(id));
                                        }
                                        if ((m_builder.pillar(id).startpoint()(Z) - m_builder.ground_level) > m_cfg.base_height_mm)
                                        {
                                            add_pillar_base(id);
                                        }
                                        m_pillar_index.insert(m_builder.pillar(id).endpoint(), id);
                                        m_pillar_index_start.guarded_insert(m_builder.pillar(id).startpoint(), id);
                                    }
                                    goto GROUND_FACE_WHILE_END;

                                }
                                else
                                    radius -= 0.1;
                            }
                        }
                    }
                GROUND_FACE_WHILE_END:
                    m_builder.invalidateMeshLinks(outmeshlinks);
                }
                
                continue;
            }

            if (connect_to_model_body(pstart, radius, outTreeNodelink))
            {
                auto junc = m_builder.add_junction(pstart, radius);
                SupportTreeNode* treeNodeobjPtr = NULL;
                if (m_builder.supportTreeNodeIsValid(ptTreeNode, &treeNodeobjPtr))
                {
                    m_builder.buildMeshLinkRelation(*treeNodeobjPtr,m_builder.junction(junc.id));
                }
                if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                {
                    m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                }

            }
            else
            {
               // std::cout << __LINE__ << "start invalidate type==" << (int)ptTreeNode.type << std::endl;
                SupportTreeNode* treeNodePtr=NULL;
                if (m_builder.supportTreeNodeIsValid(ptTreeNode, &treeNodePtr))
                {
                    std::vector < SupportTreeNode2MeshLink> outmeshlinks;
                    std::vector < SupportTreeNode2MeshLink> forcemeshlinks;
                    m_builder.searchLeftTreeElement(*treeNodePtr, outmeshlinks);
                    for (auto& templk : outmeshlinks)
                    {
                        //break;
                        if (templk.type == MeshTypeElement::Head)
                        {
                            // std::cout << __LINE__ << "start invalidate type==" << (int)ptTreeNode.type << std::endl;
							std::vector<SupportTreeNode2MeshLink> outnewmeshlk;
                            createSmallPillar(templk,outnewmeshlk);
                        }
                        else
                        {
                            std::cout << __LINE__ << " invalidate type==" << (int)templk.type << std::endl;

                        }
                    }
                  m_builder.invalidateMeshLinks(outmeshlinks,false);

                }
            }
        }
}
void SupportTreeBuildsteps::CX_classify()
{
    std::cout << __LINE__ << "  " << " step_3 " << std::endl;
#ifdef USE_ORIGINAL_POINTS
    {
        return;
    }
#endif

    // We should first get the heads that reach the ground directly
    PtIndices ground_head_indices;
    ground_head_indices.reserve(m_iheads.size());
    m_iheads_onmodel.reserve(m_iheads.size());

    // First we decide which heads reach the ground and can be full
    // pillars and which shall be connected to the model surface (or
    // search a suitable path around the surface that leads to the
    // ground -- TODO)
    PtIndices iheads_onmodel;
    for(int index=0;index< m_iheads.size();index++) 
    {
        m_thr();
        const unsigned& i = m_iheads[index];
        Head &head = m_builder.head(i);
        double r = head.r_back_mm;
        Vec3d headjp = head.junction_point();

        // collision check
        auto hit = bridge_mesh_intersect(headjp, DOWN, r);


        if (std::isinf(hit.distance()))
        {
            ground_head_indices.emplace_back(i);

        }
        else if(m_cfg.ground_facing_only)  
            head.invalidate();
        else 
            iheads_onmodel.emplace_back(i);

       // m_head_to_ground_scans[i] = hit;
        head.hit = hit;
    }

    // We want to search for clusters of points that are far enough
    // from each other in the XY plane to not cross their pillar bases
    // These clusters of support points will join in one pillar,
    // possibly in their centroid support point.

    auto pointfn = [this](unsigned i) {
        return m_builder.head(i).junction_point();
    };
    //std::cout <<"max_pillar_link_distance_mm=="<< m_cfg.max_pillar_link_distance_mm << std::endl;
    //std::cout <<"max_bridge_length_mm=="<< m_cfg.max_bridge_length_mm << std::endl;
    //std::cout <<"max_bridges_on_pillar=="<< m_cfg.max_bridges_on_pillar << std::endl;
    //std::cout <<"pillar_cascade_neighbors=="<< m_cfg.pillar_cascade_neighbors << std::endl;
    //std::cout <<"head_front_radius_mm=="<< m_cfg.head_front_radius_mm << std::endl;
    //std::cout <<"head_back_radius_mm=="<< m_cfg.head_back_radius_mm << std::endl;


    std::vector<SupportTreeNode2MeshLink> pillarMeshlinkTotal;
    pillarMeshlinkTotal.reserve(iheads_onmodel.size() + ground_head_indices.size()+1);


    for (int index=0;index<iheads_onmodel.size();index++)
    {
        unsigned& i = iheads_onmodel[index];
        Head hd = m_builder.head(i);
        SupportTreeNode2MeshLink mehslinktemp;
        std::optional<DiffBridge> diffbr =
            search_widening_path_pillar(hd.junction_point(), hd.dir.normalized(), hd.r_back_mm, hd.r_back_mm);

        if (diffbr && (diffbr->endp.z() > m_cfg.min_bridge_height_mm))
        {
            auto diffhit = bridge_mesh_intersect(diffbr->endp, DOWN, diffbr->r);
            auto hdhit = pinhead_mesh_intersect(diffbr->startp, (diffbr->endp - diffbr->startp).normalized(), diffbr->r, diffbr->end_r, (diffbr->endp - diffbr->startp).norm());
            if (std::isinf(diffhit.distance()) &&
                std::isinf(hdhit.distance())
                )
            {
                Junction junc;
                DiffBridge bri;

                junc=m_builder.add_junction(hd.junction_point(), hd.r_back_mm);
                bri=m_builder.add_diffbridge(*diffbr);
                m_builder.buildMeshLinkRelation(m_builder.head(i), m_builder.junction(junc.id));
                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.diffbridge(bri.id));

                m_builder.diffbridge(bri.id).setbriStartplink(m_builder.junction(junc.id));

                mehslinktemp.id = bri.id;
                mehslinktemp.type = bri.treeNodeType;
                pillarMeshlinkTotal.emplace_back(mehslinktemp);

            }
			else
            {
                mehslinktemp.id = hd.id;
                mehslinktemp.type = hd.treeNodeType;
                pillarMeshlinkTotal.emplace_back(mehslinktemp);
                //std::cout << "shoul not go here,head onmodel  to ground id==" << i << std::endl;
            }


        }
        else
        {
            mehslinktemp.id = hd.id;
            mehslinktemp.type = hd.treeNodeType;
            pillarMeshlinkTotal.emplace_back(mehslinktemp);
        }
    }

    for (int index = 0; index < ground_head_indices.size(); index++)
    {
        unsigned& i = ground_head_indices[index];
        Head& hd = m_builder.head(i);
        SupportTreeNode2MeshLink mehslinktemp;
        mehslinktemp.id = hd.id;
        mehslinktemp.type = hd.treeNodeType;
        pillarMeshlinkTotal.emplace_back(mehslinktemp);
    }
    
    PointSet pillarPoints(pillarMeshlinkTotal.size(),3);
    PillarIndex  pillarPointindex;

    for (int i= 0;i< pillarMeshlinkTotal.size();i++)
    {
        Vec3d pos;
        SupportTreeNode2MeshLink &meshlink = pillarMeshlinkTotal[i];
        switch (meshlink.type)
        {
        case MeshTypeElement::Head:
        {
            auto& obj = m_builder.head(meshlink.id);
            pos = obj.junction_point();
        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            auto& obj = m_builder.diffbridge(meshlink.id);
            pos = obj.endp;
        }
        break;
        default :
           std::cout <<__LINE__<< " unknown ptTreeNode->treeNodeType==" << (int)meshlink.type << std::endl;
           assert(0);
           break;
        }

        {
        pillarPoints.row(i)(X) = double(pos(X));
        pillarPoints.row(i)(Y) = double(pos(Y));
        pillarPoints.row(i)(Z) = double(pos(Z));
        pillarPointindex.insert(pos, i);
        }
    }
    //std::cout << "pillarRadiusTotal size==" << pillarRadiusTotal.size() << std::endl;
    //std::cout << "m_ihead size==" << m_iheads.size();
    m_pillarPoints = std::make_pair(pillarPoints, pillarMeshlinkTotal);//保存起当前数据，供下阶段使用

    PointIndex pointIndex=pillarPointindex.guarded_clone();
    std::vector<bool> clusterflg(pillarPoints.rows(), false);
    //提取m_pillar_clusters数据
    for (int i = 0; i < pillarPoints.rows(); i++)
    {
        PtIndices ptindex;
        Vec3d pt(pillarPoints.row(i)(X), pillarPoints.row(i)(Y), pillarPoints.row(i)(Z));
        if (clusterflg[i])
            continue;
        clusterflg[i] = true;
        ptindex.emplace_back(i);
        PointIndexEl e1 = std::make_pair(pillarPoints.row(i),i);
        pointIndex.remove(e1);//移除，防止搜索自已
        PointIndex pointIndexFirst = pointIndex;
        PointIndexEl idfirstE1;
        long id_first= searchSidePillarPoint(pointIndexFirst, pt, idfirstE1);

        while (id_first > SupportTreeNode::ID_UNSET)//搜索到第一个
        {

            long id_second  = SupportTreeNode::ID_UNSET;
            pointIndexFirst.remove(pillarPoints.row(id_first), id_first);//移除搜索到的第一个对象，继续搜索第二个
            PointIndex pointIndexSecond = pointIndexFirst;
            PointIndexEl idsecondE2;
            id_second = searchSidePillarPoint(pointIndexSecond, pt,idsecondE2);
            while (id_second > SupportTreeNode::ID_UNSET)
            {
                Vec3d pt1(pt(X), pt(Y), 0);
                Vec3d pt2(pillarPoints.row(id_first)(X), pillarPoints.row(id_first)(Y), 0);
                Vec3d pt3(pillarPoints.row(id_second)(X), pillarPoints.row(id_second)(Y), 0);
                Vec3d pt12 = pt2 - pt1;
                Vec3d pt13 = pt3 - pt1;
                pt12.normalize();
                pt13.normalize();
                float angle = std::acos(pt12.dot(pt13));
                pointIndexSecond.remove(pillarPoints.row(id_second), id_second);

                if (angle * 180.0 / PI > 120.0)
                {
                    ptindex.emplace_back(id_first);
                    ptindex.emplace_back(id_second);
                    clusterflg[id_second] = true;
                    clusterflg[id_first] = true;
                    goto SEARCH_END;
                }
                else //角度不满足需求，续续搜索
                {
                    id_second = searchSidePillarPoint(pointIndexSecond, pt, idsecondE2);
                }
            }
            id_first = searchSidePillarPoint(pointIndexFirst, pt, idfirstE1);
        }
    SEARCH_END:
        if(ptindex.size()>0)
            m_pillar_clusters.emplace_back(ptindex);
    }
}
void SupportTreeBuildsteps::CX_classify_simple()
{
    std::cout << __LINE__ << "  " << " step_3 " << std::endl;
#ifdef USE_ORIGINAL_POINTS
    {
        return;
    }
#endif

    // We should first get the heads that reach the ground directly
    PtIndices ground_head_indices;
    ground_head_indices.reserve(m_iheads.size());
    m_iheads_onmodel.reserve(m_iheads.size());

    // First we decide which heads reach the ground and can be full
    // pillars and which shall be connected to the model surface (or
    // search a suitable path around the surface that leads to the
    // ground -- TODO)
    PtIndices iheads_onmodel;
    for (int index = 0; index < m_iheads.size(); index++)
    {
        m_thr();
        const unsigned& i = m_iheads[index];
        Head& head = m_builder.head(i);
        double r = head.r_back_mm;
        Vec3d headjp = head.junction_point();

        // collision check
        auto hit = bridge_mesh_intersect(headjp, DOWN, r);


        if (std::isinf(hit.distance()))
        {
            ground_head_indices.emplace_back(i);

        }
        else if (m_cfg.ground_facing_only)
            head.invalidate();
        else
            iheads_onmodel.emplace_back(i);

        // m_head_to_ground_scans[i] = hit;
        head.hit = hit;
    }

    // We want to search for clusters of points that are far enough
    // from each other in the XY plane to not cross their pillar bases
    // These clusters of support points will join in one pillar,
    // possibly in their centroid support point.

    auto pointfn = [this](unsigned i) {
        return m_builder.head(i).junction_point();
    };
    //std::cout <<"max_pillar_link_distance_mm=="<< m_cfg.max_pillar_link_distance_mm << std::endl;
    //std::cout <<"max_bridge_length_mm=="<< m_cfg.max_bridge_length_mm << std::endl;
    //std::cout <<"max_bridges_on_pillar=="<< m_cfg.max_bridges_on_pillar << std::endl;
    //std::cout <<"pillar_cascade_neighbors=="<< m_cfg.pillar_cascade_neighbors << std::endl;
    //std::cout <<"head_front_radius_mm=="<< m_cfg.head_front_radius_mm << std::endl;
    //std::cout <<"head_back_radius_mm=="<< m_cfg.head_back_radius_mm << std::endl;


    std::vector<SupportTreeNode2MeshLink> pillarMeshlinkTotal;
    pillarMeshlinkTotal.reserve(iheads_onmodel.size() + ground_head_indices.size() + 1);


    for (int index = 0; index < iheads_onmodel.size(); index++)
    {
        unsigned& i = iheads_onmodel[index];
        Head hd = m_builder.head(i);
        SupportTreeNode2MeshLink mehslinktemp;

        {
            mehslinktemp.id = hd.id;
            mehslinktemp.type = hd.treeNodeType;
            pillarMeshlinkTotal.emplace_back(mehslinktemp);
        }
    }

    for (int index = 0; index < ground_head_indices.size(); index++)
    {
        unsigned& i = ground_head_indices[index];
        Head& hd = m_builder.head(i);
        SupportTreeNode2MeshLink mehslinktemp;
        mehslinktemp.id = hd.id;
        mehslinktemp.type = hd.treeNodeType;
        pillarMeshlinkTotal.emplace_back(mehslinktemp);
    }

    PointSet pillarPoints(pillarMeshlinkTotal.size(), 3);
    PillarIndex  pillarPointindex;

    for (int i = 0; i < pillarMeshlinkTotal.size(); i++)
    {
        Vec3d pos;
        SupportTreeNode2MeshLink& meshlink = pillarMeshlinkTotal[i];
        switch (meshlink.type)
        {
        case MeshTypeElement::Head:
        {
            auto& obj = m_builder.head(meshlink.id);
            pos = obj.junction_point();
        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            auto& obj = m_builder.diffbridge(meshlink.id);
            pos = obj.endp;
        }
        break;
        default:
            std::cout << __LINE__ << " unknown ptTreeNode->treeNodeType==" << (int)meshlink.type << std::endl;
            assert(0);
            break;
        }

        {
            pillarPoints.row(i)(X) = double(pos(X));
            pillarPoints.row(i)(Y) = double(pos(Y));
            pillarPoints.row(i)(Z) = double(pos(Z));
            pillarPointindex.insert(pos, i);
        }
    }
    //std::cout << "pillarRadiusTotal size==" << pillarRadiusTotal.size() << std::endl;
    //std::cout << "m_ihead size==" << m_iheads.size();
    m_pillarPoints = std::make_pair(pillarPoints, pillarMeshlinkTotal);//保存起当前数据，供下阶段使用

    PointIndex pointIndex = pillarPointindex.guarded_clone();
    std::vector<bool> clusterflg(pillarPoints.rows(), false);
    //提取m_pillar_clusters数据
    for (int i = 0; i < pillarPoints.rows(); i++)
    {
        PtIndices ptindex;
        ptindex.emplace_back(i);

        if (ptindex.size() > 0)
            m_pillar_clusters.emplace_back(ptindex);
    }
}
void SupportTreeBuildsteps::CX_routing_to_ground()
{
    std::cout << __LINE__ << "  " << " step_4 " << std::endl;
#ifdef USE_ORIGINAL_POINTS
    {
        return;
    }
#endif

    ClusterEl cl_centroids;
    cl_centroids.reserve(m_pillar_clusters.size());
    std::vector<PtIndices> sidepillarclusters;
    sidepillarclusters.resize(m_pillar_clusters.size());
    int PtIndicesIndex=0;

    const PointSet &points= m_pillarPoints.first;
    const std::vector<SupportTreeNode2MeshLink>& ptStartMeshlinks = m_pillarPoints.second;
    std::vector<SupportTreeNode2MeshLink> treeNodeLinksNext;//存储流入到下阶段的信息
    for (auto& cl : m_pillar_clusters) {
        m_thr();
        PtIndices sidePillid;
        std::vector<SupportTreeNode2MeshLink> outTreeNodelink;
        if (cl.empty())
        {
            sidepillarclusters[PtIndicesIndex] = sidePillid;
            PtIndicesIndex++;
            std::cout << __LINE__ << " cl is emptty" << std::endl;
            continue;
        }

        // get the current cluster centroid
        auto& thr = m_thr;
        long lcid = 0;//上一流程中，默认数组第一项为中心点
        unsigned hid = cl[size_t(lcid)]; // Head ID

        cl_centroids.emplace_back(hid);
        //先创建两边对象
        for (auto& c : cl)
        {
            Vec3d  pstart(points.row(c));
            double radius = 0.0;
            const SupportTreeNode2MeshLink & startMeshlink = ptStartMeshlinks[c];
            std::vector<SupportTreeNode2MeshLink> meshlinksL;
            if (c == hid)
                continue;
            //可能已有对象创建完成了，搜索主支撑
            switch (startMeshlink.type)
            {
            case MeshTypeElement::Head:
            {
                auto& obj = m_builder.head(startMeshlink.id);
                radius = obj.r_back_mm;
                m_builder.searchRightTreeElement(obj, meshlinksL);
            }
            break;
            case MeshTypeElement::DiffBridge:
            {
                auto& obj = m_builder.diffbridge(startMeshlink.id);
                radius = obj.end_r;
                m_builder.searchRightTreeElement(obj, meshlinksL);
            }
                break;
            default:
                std::cout << __LINE__ << " unknown ptTreeNode->treeNodeType==" << (int)startMeshlink.type << std::endl;
                std::cout << __LINE__ << " unknown c==" << (int)c << std::endl;
                assert(0);
                break;
            }
            auto pstarthit = bridge_mesh_intersect(pstart, DOWN, radius);
            if (meshlinksL.size() > 0)
            {
                for (auto& lkl : meshlinksL)
                {
                    switch (lkl.type)
                    {
                    case MeshTypeElement::Pillar:
                    {
                        if (!m_builder.pillar(lkl.id).bridgePillar)
                        {
                            auto& obj = m_builder.pillar(lkl.id);
                            radius = obj.r;
                            sidePillid.emplace_back(obj.id);
                            goto CIRCLE_MESHLINKL_END;
                        }
                    }
                    break;
                    default:
                        break;
                    }
                }

            }
             if (!std::isinf(pstarthit.distance()))
            {
                if (connect_to_model_body(pstart, radius, outTreeNodelink))
                {
                    Junction junc;
                    junc = m_builder.add_junction(pstart, radius);
                    SupportTreeNode* treeNodeobjPtr = NULL;
                    if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                    {
                        m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                    }
                    if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                    {
                        m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                        switch (treeNodeobjPtr->treeNodeType)
                        {
                        case MeshTypeElement::Bridge:
                        {
                            m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                        }
                        break;
                        default:
                            break;
                        }
                    }
                    for (auto& lk : outTreeNodelink)
                    {
                        if (lk.type == MeshTypeElement::Pillar)
                        {
                            sidePillid.emplace_back(lk.id);
                        }
                    }

                }
                else
                {
                    //std::cout << __LINE__ << "  invalidate type==" << (int)startMeshlink.type << std::endl;
                    treeNodeLinksNext.emplace_back(startMeshlink);
                }
            }
            else
            {
                if (create_ground_pillar(pstart, DOWN, radius, SupportTreeNode::ID_UNSET, outTreeNodelink))
                {
                    Junction junc;
                    junc = m_builder.add_junction(pstart, radius);

                    SupportTreeNode* treeNodeobjPtr = NULL;
                    if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                    {
                        m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                    }
                    if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                    {
                        m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                        switch (treeNodeobjPtr->treeNodeType)
                        {
                        case MeshTypeElement::Bridge:
                        {
                            m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                        }
                        break;
                        default:
                            break;
                        }
                    }

                    for (auto& lk : outTreeNodelink)
                    {
                        if (lk.type == MeshTypeElement::Pillar)
                        {
                            sidePillid.emplace_back(lk.id);
                        }
                    }
                }
                else
                {
                    treeNodeLinksNext.emplace_back(startMeshlink);
                }

            }
         CIRCLE_MESHLINKL_END:
             ;
        }
        sidepillarclusters[PtIndicesIndex++]= sidePillid;
    }

    // now we will go through the clusters ones again and connect the
    // sidepoints with the cluster centroid (which is a ground pillar)
    // or a nearby pillar if the centroid is unreachable.
    size_t ci = 0;
    PointIndex spindex = m_pillar_index.guarded_clone();
    PtIndicesIndex = 0;
    //create center pillar
    for (auto &cl : m_pillar_clusters) {
        m_thr();

        auto cidx = cl_centroids[ci++];
        PtIndices pillid = sidepillarclusters[PtIndicesIndex];

        Vec3d  pstart(points.row(cidx));
        Vec3d posNew = pstart;
        double deltZ = 0.0;
        double radius = 0.0;
        const SupportTreeNode2MeshLink& startMeshlink = ptStartMeshlinks[cidx];
        switch (startMeshlink.type)
        {
        case MeshTypeElement::Head:
        {
            auto& obj = m_builder.head(startMeshlink.id);
            radius = obj.r_back_mm;
        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            auto& obj = m_builder.diffbridge(startMeshlink.id);
            radius = obj.end_r;
        }
        break;
        default:
            std::cout << __LINE__ << " unknown ptTreeNode->treeNodeType==" << (int)startMeshlink.type << std::endl;
            std::cout << __LINE__ << " cidx==" << cidx << std::endl;
            assert(0);
            break;

        }
        for (auto& c : cl)
        {
            if (posNew(Z) > points.row(c)(Z))
            {
                posNew(Z) = points.row(c)(Z);
            }
        }
        deltZ = pstart(Z) - posNew(Z);
        if (deltZ < 3.0)
        {
            deltZ = 3.0;
        }
        posNew = pstart;
        posNew(Z) -= deltZ;
        auto centerhit = bridge_mesh_intersect(pstart, DOWN, radius);
        std::vector<SupportTreeNode2MeshLink> outTreeNodelink;

        if (centerhit.distance()<= deltZ)//中心点向下空间不够
        {
            treeNodeLinksNext.emplace_back(startMeshlink);
        }
        else
        {
            if (pillid.size() == 2)
            {

                long firstSidepillarID = pillid[0];
                long secondSidepillarID = pillid[1];
                std::vector<SupportTreeNode2MeshLink> outbrilinkfirst;
                std::vector<SupportTreeNode2MeshLink> outbrilinksecond;
                bool firstret = bridge_to_nearpillar(posNew, radius, firstSidepillarID, outbrilinkfirst);
                bool secondret = bridge_to_nearpillar(posNew, radius, secondSidepillarID, outbrilinksecond);
                if (firstret == false &&
                    secondret == false)
                {
                    if (!std::isinf(centerhit.distance()))
                    {
                        treeNodeLinksNext.emplace_back(startMeshlink);
                    }
                    else
                    {

                        if (create_ground_pillar(pstart, DOWN, radius, SupportTreeNode::ID_UNSET, outTreeNodelink))
                        {
                            auto junc = m_builder.add_junction(pstart, radius);
                            SupportTreeNode* treeNodeobjPtr = NULL;
                            if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                            }
                            if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                default:
                                    break;
                                }
                            }
                        }
                        else
                        {
                            std::cout << __LINE__ << "  Pillar cannot be created for support point " << std::endl;
                            treeNodeLinksNext.emplace_back(startMeshlink);

                        }
                    }
                }
                else
                {
                    Junction junc;
                    long id=SupportTreeNode::ID_UNSET;
                    junc=m_builder.add_junction(pstart, radius);
                    {
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                        }
                    }
                    id=m_builder.add_pillar(posNew, deltZ, radius);
                    m_builder.pillar(id).bridgePillar = true;
                    m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.pillar(id));
                    junc= m_builder.add_junction(posNew, radius);
                    m_builder.buildMeshLinkRelation( m_builder.pillar(id), m_builder.junction(junc.id));
                    if (firstret == true)
                    {
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(outbrilinkfirst[0], &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                            switch (treeNodeobjPtr->treeNodeType)
                            {
                            case MeshTypeElement::Bridge:
                            {
                                m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            case MeshTypeElement::DiffBridge:
                            {
                                m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            default:
                                break;
                            }
                        }

                    }
                    if (secondret == true)
                    {
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(outbrilinksecond[0], &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                            switch (treeNodeobjPtr->treeNodeType)
                            {
                            case MeshTypeElement::Bridge:
                            {
                                m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            case MeshTypeElement::DiffBridge:
                            {
                                m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            default:
                                break;
                            }
                        }

                    }
                }

            }
            else if (pillid.size() == 1)
            {
                long firstSidepillarID = SupportTreeNode::ID_UNSET;
                firstSidepillarID = pillid[0];
                std::vector<SupportTreeNode2MeshLink> outbrilinks;
                bool firstret = bridge_to_nearpillar(posNew, radius, firstSidepillarID, outbrilinks);
                if (firstret == false)
                {
                    // Vec3d pend = Vec3d{pstart(X), pstart(Y), gndlvl};
                    // Could not find a pillar, create one
                    if (!std::isinf(centerhit.distance()))
                    {
                        treeNodeLinksNext.emplace_back(startMeshlink);
                    }
                    else 
                    {
                        if (create_ground_pillar(pstart, DOWN, radius, SupportTreeNode::ID_UNSET, outTreeNodelink))
                        {
                            Junction junc;
                            long id = SupportTreeNode::ID_UNSET;
                            junc = m_builder.add_junction(pstart, radius);

                            SupportTreeNode* treeNodeobjPtr = NULL;
                            if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation( *treeNodeobjPtr, m_builder.junction(junc.id));
                            }
                            if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                            {
                                m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                                switch (treeNodeobjPtr->treeNodeType)
                                {
                                case MeshTypeElement::Bridge:
                                {
                                    m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                case MeshTypeElement::DiffBridge:
                                {
                                    m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                                }
                                break;
                                default:
                                    break;
                                }
                            }

                        }
                        else
                        {
                            std::cout << __LINE__ << "  Pillar cannot be created for support point " << std::endl;
                            treeNodeLinksNext.emplace_back(startMeshlink);

                        }
                    }

                }
                else
                {
                    Junction junc;
                    long id = SupportTreeNode::ID_UNSET;
                    junc = m_builder.add_junction(pstart, radius);
                    {
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                        }
                    }
                    id = m_builder.add_pillar(posNew, deltZ, radius);
                    m_builder.pillar(id).bridgePillar = true;
                    m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), m_builder.pillar(id));
                    junc = m_builder.add_junction(posNew, radius);
                    m_builder.buildMeshLinkRelation(m_builder.pillar(id), m_builder.junction(junc.id));
                    {
                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(outbrilinks[0], &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                            switch (treeNodeobjPtr->treeNodeType)
                            {
                            case MeshTypeElement::Bridge:
                            {
                                m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                            }
                            break;
                            case MeshTypeElement::DiffBridge:
                            {
                                m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));

                            }
                            break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                if (!std::isinf(centerhit.distance()))
                {
                    treeNodeLinksNext.emplace_back(startMeshlink);
                }
                else 
                {
                    if (create_ground_pillar(pstart, DOWN, radius, SupportTreeNode::ID_UNSET, outTreeNodelink))
                    {
                        Junction junc;
                        long id = SupportTreeNode::ID_UNSET;
                        junc = m_builder.add_junction(pstart, radius);

                        SupportTreeNode* treeNodeobjPtr = NULL;
                        if (m_builder.supportTreeNodeIsValid(startMeshlink, &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(*treeNodeobjPtr, m_builder.junction(junc.id));
                        }
                        if (m_builder.supportTreeNodeIsValid(outTreeNodelink[0], &treeNodeobjPtr))
                        {
                            m_builder.buildMeshLinkRelation(m_builder.junction(junc.id), *treeNodeobjPtr);
                            switch (treeNodeobjPtr->treeNodeType)
                            {
                            case MeshTypeElement::Bridge:
                            {
                                m_builder.bridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            case MeshTypeElement::DiffBridge:
                            {
                                m_builder.diffbridge(treeNodeobjPtr->id).setbriStartplink(m_builder.junction(junc.id));
                            }
                            break;
                            default:
                                break;
                            }
                        }

                    }
                    else
                    {
                        //std::cout << __LINE__ << "  Pillar cannot be created for support point " << std::endl;
                        treeNodeLinksNext.emplace_back(startMeshlink);

                    }
                }

            }
        }
       PtIndicesIndex++;
    }

    m_TreeNodesNext.swap(treeNodeLinksNext);
}

////
}} // namespace Slic3r::sla
