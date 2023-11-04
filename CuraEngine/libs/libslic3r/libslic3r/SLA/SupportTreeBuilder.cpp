#define NOMINMAX

#include <libslic3r/SLA/SupportTreeBuilder.hpp>
#include <libslic3r/SLA/SupportTreeBuildsteps.hpp>
#include <libslic3r/SLA/SupportTreeMesher.hpp>
#include <libslic3r/SLA/Contour3D.hpp>

#include <libslic3r/SLA/SupportTreeData.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
//#include  <direct.h>  
#include  <stdio.h> 

namespace Slic3r {
namespace sla {
    static void printMeshType(MeshTypeElement type)
    {
        switch (type)
        {
        case MeshTypeElement::Head:
            std::cout << "MeshTypeElement::Head" << std::endl;
            break;
        case MeshTypeElement::Pillar:
            std::cout << "MeshTypeElement::Pillar" << std::endl;
            break;
        case MeshTypeElement::Junction:
            std::cout << "MeshTypeElement::Junction" << std::endl;
            break;
        case MeshTypeElement::Bridge:
            std::cout << "MeshTypeElement::Bridge" << std::endl;
            break;
        case MeshTypeElement::CrossBridge:
            std::cout << "MeshTypeElement::CrossBridge" << std::endl;
            break;
        case MeshTypeElement::DiffBridge:
            std::cout << "MeshTypeElement::DiffBridge" << std::endl;
            break;
        case MeshTypeElement::Pedestal:
            std::cout << "MeshTypeElement::Pedestal" << std::endl;
            break;
        case MeshTypeElement::Anchor:
            std::cout << "MeshTypeElement::Anchor" << std::endl;
            break;
        case MeshTypeElement::SquarePedestal:
            std::cout << "MeshTypeElement::SquarePedestal" << std::endl;
            break;
        default:
            std::cout << "MeshTypeElement unkown type==="<<(int)type << std::endl;
            break;
        }

    }

const TriangleMesh &SupportTreeBuilder::add_pad(const ExPolygons &modelbase,
                                                const PadConfig & cfg)
{
    if(m_meshPadCache_valid==false)
        m_pad = Pad{ merged_meshforPad(), modelbase, ground_level, cfg, ctl().cancelfn};
    return m_pad.tmesh;
}
SupportTreeBuilder::SupportTreeBuilder()
{
    m_supportTreeBuildstepsPtr = nullptr;
}
SupportTreeBuilder::SupportTreeBuilder(SupportTreeBuilder &&o)
    : m_heads(std::move(o.m_heads))
    , m_head_indices{std::move(o.m_head_indices)}
    , m_pillars{std::move(o.m_pillars)}
    , m_bridges{std::move(o.m_bridges)}
    , m_crossbridges{std::move(o.m_crossbridges)}
    , m_pad{std::move(o.m_pad)}
    , m_meshcache{std::move(o.m_meshcache)}
    , m_meshcache_valid{o.m_meshcache_valid}
    , m_model_height{o.m_model_height}
    , ground_level{o.ground_level}
{
    m_supportTreeBuildstepsPtr = nullptr;
	m_supportType=0x00;
}

SupportTreeBuilder::SupportTreeBuilder(const SupportTreeBuilder &o)
    : m_heads(o.m_heads)
    , m_head_indices{o.m_head_indices}
    , m_pillars{o.m_pillars}
    , m_bridges{o.m_bridges}
    , m_crossbridges{o.m_crossbridges}
    , m_pad{o.m_pad}
    , m_meshcache{o.m_meshcache}
    , m_meshcache_valid{o.m_meshcache_valid}
    , m_model_height{o.m_model_height}
    , ground_level{o.ground_level}
{
    m_supportTreeBuildstepsPtr = nullptr;
	m_supportType=0x00;
}

SupportTreeBuilder &SupportTreeBuilder::operator=(SupportTreeBuilder &&o)
{
    m_heads = std::move(o.m_heads);
    m_head_indices = std::move(o.m_head_indices);
    m_pillars = std::move(o.m_pillars);
    m_bridges = std::move(o.m_bridges);
    m_crossbridges = std::move(o.m_crossbridges);
    m_pad = std::move(o.m_pad);
    m_meshcache = std::move(o.m_meshcache);
    m_meshcache_valid = o.m_meshcache_valid;
    m_model_height = o.m_model_height;
    ground_level = o.ground_level;
    return *this;
}

SupportTreeBuilder &SupportTreeBuilder::operator=(const SupportTreeBuilder &o)
{
    m_heads = o.m_heads;
    m_head_indices = o.m_head_indices;
    m_pillars = o.m_pillars;
    m_bridges = o.m_bridges;
    m_crossbridges = o.m_crossbridges;
    m_pad = o.m_pad;
    m_meshcache = o.m_meshcache;
    m_meshcache_valid = o.m_meshcache_valid;
    m_model_height = o.m_model_height;
    ground_level = o.ground_level;
    return *this;
}
SupportTreeBuilder::~SupportTreeBuilder()
{
    if (m_supportTreeBuildstepsPtr)
        delete m_supportTreeBuildstepsPtr;
}

void SupportTreeBuilder::setCtl(JobController jc)
{
    m_ctl = jc;
    if (m_supportTreeBuildstepsPtr)
    {
        m_supportTreeBuildstepsPtr->setThrowOnCancel(m_ctl.cancelfn);
    }
}

void SupportTreeBuilder::add_pillar_base(long pid, double baseheight, double top_radius, double bt_radius)
{
    std::lock_guard<Mutex> lk(m_mutex);
    assert(pid >= 0 && size_t(pid) < m_pillars.size());
    Pillar& pll = m_pillars[size_t(pid)];
    double r_top = std::max(top_radius, pll.r);
    double r_bottom = std::max(bt_radius, pll.r);
    m_pedestals.emplace_back(pll.endpt, std::min(baseheight, pll.height),r_bottom, r_top);

    m_pedestals.back().id = m_pedestals.size() - 1;
    m_pedestals.back().pillar_id = pid;
    m_meshcache_valid = false;

    buildMeshLinkRelation(pll, m_pedestals.back());

    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    if (algPtr)
    {
        Vec3d startpt = pll.endpt;
        startpt(Z) += algPtr->m_cfg.squarebase_height_mm;
        IndexedMesh::hit_result hitret=algPtr->pinhead_mesh_intersect(startpt, Vec3d(0.0,0.0,-1.0), algPtr->m_cfg.squarebase_radius_mm, algPtr->m_cfg.squarebase_radius_mm,algPtr->m_cfg.squarebase_height_mm);
        if (!hitret.is_hit())
        {
            m_squarePedestals.emplace_back(pll.endpt, algPtr->m_cfg.squarebase_height_mm, algPtr->m_cfg.squarebase_radius_mm, algPtr->m_cfg.squarebase_top_radius_mm);

            m_squarePedestals.back().id = m_squarePedestals.size() - 1;

            buildMeshLinkRelation(m_pedestals.back(), m_squarePedestals.back());
        }
    }

}
const TriangleMesh& SupportTreeBuilder::merged_meshforPad(size_t steps) const
{
    Contour3D merged;
    //if (m_meshPadCache_valid)
    //{
    //    //if (!m_meshPadCacheNew.empty())
    //    //{
    //    //    m_meshPadCache.merge(m_meshPadCacheNew);
    //    //    m_meshPadCache.require_shared_vertices();
    //    //    //BoundingBoxf3&& bb = m_meshPadCache.bounding_box();
    //    //    //m_model_height = bb.max(Z) - bb.min(Z);
    //    //    m_meshPadCacheNew.clear();
    //    //}
    //    return m_meshPadCache;

    //}


    //std::cout << "m_pillars size==" << m_pillars.size() << std::endl;
    //for (auto& pill : m_pillars) {
    //    if (ctl().stopcondition()) 
    //        break;
    //    for (auto& lktype: pill.linkR)
    //    {
    //        if (lktype.type == MeshTypeElement::Pedestal)
    //            merged.merge(get_mesh(pill, steps));
    //    }
    //}
    std::cout << "***************************************" << std::endl;
    std::cout << "***************************************" << std::endl;
    #ifdef _OPENMP
    std::cout << "_OPENMP enable"  << std::endl;
    #endif
    std::cout << "m_pedestals size==" << m_pedestals.size() << std::endl;
    std::cout << "***************************************" << std::endl;
    std::cout << "***************************************" << std::endl;
    for (auto& pedest : m_pedestals) 
    {
        if (ctl().stopcondition()) 
            break;
        if(pedest.is_valid())
            merged.merge(get_mesh(pedest, steps));
    }

    m_meshPadCache = to_triangle_mesh(merged);

    // The mesh will be passed by const-pointer to TriangleMeshSlicer,
    // which will need this.
    if (!m_meshPadCache.empty()) 
        m_meshPadCache.require_shared_vertices();

    //BoundingBoxf3&& bb = m_meshPadCache.bounding_box();
    //m_model_height = bb.max(Z) - bb.min(Z);

    m_meshPadCache_valid = true;
    m_meshPadCacheNew.clear();
    return m_meshPadCache;
}

const TriangleMesh &SupportTreeBuilder::merged_mesh(size_t steps) const
{
    
    Contour3D merged;
    
    for (auto &head : m_heads) {
        if (ctl().stopcondition()) break;
        if (head.is_valid()) merged.merge(get_mesh(head, steps));
    }
    
    for (auto &pill : m_pillars) {
        if (ctl().stopcondition()) break;
        if (pill.is_valid()) merged.merge(get_mesh(pill, steps));
    }

    
    for (auto &j : m_junctions) {
        if (ctl().stopcondition()) break;
        if (j.is_valid()) merged.merge(get_mesh(j, steps));
    }

    for (auto &bs : m_bridges) {
        if (ctl().stopcondition()) break;
        if (bs.is_valid()) merged.merge(get_mesh(bs, steps));
    }
    
    for (auto &bs : m_crossbridges) {
        if (ctl().stopcondition()) break;
        if (bs.is_valid()) merged.merge(get_mesh(bs, steps));
    }

    for (auto &bs : m_diffbridges) {
        if (ctl().stopcondition()) break;
        if (bs.is_valid()) merged.merge(get_mesh(bs, steps));
    }

    for (auto &anch : m_anchors) {
        if (ctl().stopcondition()) break;
        if (anch.is_valid()) merged.merge(get_mesh(anch, steps));
    }
    for (auto &pedestal : m_pedestals) {
        if (ctl().stopcondition()) break;
        if (pedestal.is_valid()) merged.merge(get_mesh(pedestal, steps));
    }
    for (auto &pedestal : m_squarePedestals) {
        if (ctl().stopcondition()) break;
        if (pedestal.is_valid()) merged.merge(get_mesh(pedestal));
    }
    

    m_meshcache = to_triangle_mesh(merged);
    
    // The mesh will be passed by const-pointer to TriangleMeshSlicer,
    // which will need this.
    if (!m_meshcache.empty()) m_meshcache.require_shared_vertices();
    
    BoundingBoxf3 &&bb = m_meshcache.bounding_box();
    m_model_height       = bb.bmax(Z) - bb.bmin(Z);
    
    m_meshcache_valid = true;
    return m_meshcache;
}

double SupportTreeBuilder::full_height() const
{
    if (merged_mesh().empty() && !pad().empty())
        return pad().cfg.full_height();
    
    double h = mesh_height();
    if (!pad().empty()) h += pad().cfg.required_elevation();
    return h;
}

const TriangleMesh &SupportTreeBuilder::merge_and_cleanup()
{
    // in case the mesh is not generated, it should be...
    auto &ret = merged_mesh(); 
    
    // Doing clear() does not garantee to release the memory.
    m_heads = {};
    m_head_indices = {};
    m_pillars = {};
    m_junctions = {};
    m_bridges = {};
    
    return ret;
}

const TriangleMesh &SupportTreeBuilder::retrieve_mesh(MeshType meshtype) const
{
    switch(meshtype) {
    case MeshType::Support: return merged_mesh();
    case MeshType::Pad:     return pad().tmesh;
    }
    
    return m_meshcache;
}
void SupportTreeBuilder::retrieve_meshElements(MeshTypeElement meshtype, std::vector<TriangleMesh*>& triangleMeshes)
{
    size_t steps = CONTOUR_STEPS; //45;
    switch (meshtype)
    {
    case MeshTypeElement::Head:
    {
        for (auto& head : m_heads)
        {
            if (ctl().stopcondition())
                break;
            if (head.is_valid() && (head.is_created()))
            {
                Contour3D merged;
                merged.merge(get_mesh(head, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                head.setCreatedFlg(false);

            }
        }
    }
    break;
    case MeshTypeElement::Pillar:
    {

        for (auto& pill : m_pillars)
        {
            if (ctl().stopcondition())
                break;
            if (pill.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(pill, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                pill.setCreatedFlg(false);
            }
        }

    }
    break;
    case MeshTypeElement::Junction:
    {
        for (auto& j : m_junctions)
        {
            if (ctl().stopcondition())
                break;
            if (j.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(j, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                j.setCreatedFlg(false);
            }
        }

    }
    break;
    case MeshTypeElement::Bridge:
    {
        for (auto& bs : m_bridges)
        {
            if (ctl().stopcondition())
                break;
            if (bs.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                bs.setCreatedFlg(false);
            }
        }
    }
    break;
    case MeshTypeElement::CrossBridge:
    {

        for (auto& bs : m_crossbridges)
        {
            if (ctl().stopcondition())
                break;
            if (bs.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                bs.setCreatedFlg(false);
            }
        }
    }
    break;
    case MeshTypeElement::DiffBridge:
    {

        for (auto& bs : m_diffbridges)
        {
            if (ctl().stopcondition())
                break;
            if (bs.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                bs.setCreatedFlg(false);
            }
        }
    }
    break;
    case MeshTypeElement::Pedestal:
    {

        for (auto& pedest : m_pedestals)
        {
            if (ctl().stopcondition())
                break;
            if (pedest.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(pedest, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                pedest.setCreatedFlg(false);
            }
        }
    }
    break;

    case MeshTypeElement::Anchor:
    {
        for (auto& anch : m_anchors)
        {
            if (ctl().stopcondition())
                break;
            if (anch.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(anch, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                anch.setCreatedFlg(false);
            }
        }
    }
    break;
    case MeshTypeElement::SquarePedestal:
    {

        for (auto& pedest : m_squarePedestals)
        {
            if (ctl().stopcondition())
                break;
            if (pedest.is_created())
            {
                Contour3D merged;
                merged.merge(get_mesh(pedest));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    TriangleMesh* ptr = new TriangleMesh();

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    triangleMeshes.emplace_back(ptr);
                }
                pedest.setCreatedFlg(false);
            }
        }
    }
    break;
    default:
        std::cout << "error!!!unknow mesh type!meshtype==" << (int)meshtype << std::endl;

    }

}

void SupportTreeBuilder::retrieve_meshLinkElements(MeshTypeElement meshtype, std::vector < SupportTreeNode2MeshLink>& meshlinks)
{
    size_t steps = CONTOUR_STEPS; //45;
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;

#if 0
    for (const Pillar& obj : m_pillars)
    {

        SupportTreeNode2MeshLink src;
        std::vector < SupportTreeNode2MeshLink> outmeshlinks;
        if (obj.is_valid())
        {
            src.type = obj.treeNodeType;
            src.id = obj.id;
            searchMeshLinks(src, outmeshlinks);
            invalidateMeshLinks(outmeshlinks);
            //invalidateTreeElement(src, outmeshlinks);

        }
    }
#endif
    switch(meshtype) 
    {
    case MeshTypeElement::Head:
    {
        for (auto& head : m_heads)
        {
            if (ctl().stopcondition())
                break;
            if (head.is_valid()&&(head.is_created()))
            {
                Contour3D merged;
                if (algPtr != nullptr)
                {
                 Vec3d newpos = head.pos + EPSILON * (-head.dir);
                 IndexedMesh::hit_result hit;
                 if (head.toptouch_enable == true)
                 {
                     hit = algPtr->ray_mesh_intersect(newpos, -head.dir);
                 }
                 else
                 {
                     hit = algPtr->cone_mesh_intersect(newpos, -head.dir, head.r_pin_mm, head.r_pin_mm, head.penetration_mm, 0.0);
                 }
                 if (hit.is_inside())
                 {
                     double len = hit.distance();
                     //std::cout << "len==" << len<< std::endl;
                     if (len < head.penetration_mm)
                     {
                         len = len>EPSILON?len: EPSILON;
                         head.width_mm -= (head.penetration_mm - len);
                         head.penetration_mm = len;
                     }
                 }
                }
                
                merged.merge(get_mesh(head, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    std::unique_ptr<TriangleMesh> ptr (new TriangleMesh());

                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id =head.id;
                    meshlinks.emplace_back(meshlink);
                    head.setCreatedFlg(false);
                }

            }
        }
    }
    break;
    case MeshTypeElement::Pillar:
    {
        //Contour3D mergedforPad;
        for (auto& pill : m_pillars) 
        {
            if (ctl().stopcondition()) 
                break;
            if (pill.is_valid() && (pill.is_created()))
            {
                Contour3D merged;
                //merged.merge(get_mesh(pill, steps));
                bool linkPedestalflg = false;
                long pedestalId = -1;
                for (SupportTreeNode2MeshLink &lktype : pill.linkR)
                {
                    if (lktype.type == MeshTypeElement::Pedestal)
                    {
                        linkPedestalflg = true;
                        pedestalId = lktype.id;
                    }
                }
                if (linkPedestalflg&&(pedestalId< m_pedestals.size()))
                {
                    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
                    if (algPtr)
                    {
                        if ((pill.height > algPtr->m_cfg.max_pillar_scaled_height_mm) &&
                            (algPtr->m_cfg.base_top_radius_mm > pill.r)
                            )
                        {
                            double  radiusDown = std::max(algPtr->m_cfg.base_top_radius_mm, algPtr->m_cfg.base_radius_mm);
                            double  newpillHeight = pill.height * (1.0 - 0.618);//按黄金比例拆分
                            Vec3d pillarStartpoint = pill.startpoint();
                            Vec3d pillarEndpoint = pill.endpoint();
                            Vec3d newsPedtartpoint = pillarStartpoint;
                            newsPedtartpoint(Z) -= newpillHeight;
                            auto pillarhit = algPtr->bridge_mesh_intersect(newsPedtartpoint,
                                DOWN,
                                radiusDown);
                            if (std::isinf(pillarhit.distance()))
                            {
                                double  newPedheight = newsPedtartpoint(Z) - pillarEndpoint(Z);
                                Pillar pillarnew = pill;
                                pillarnew.endpt = newsPedtartpoint;
                                pillarnew.height = newpillHeight;

                                //pedestal(pedestalId).r_top = radiusDown;
                                pedestal(pedestalId).height = newPedheight;
                                merged.merge(get_mesh(pillarnew, pedestal(pedestalId), steps));
                            }
                            else
                            {
                                double r_top = std::max(algPtr->m_cfg.base_top_radius_mm, pill.r);
                                double r_bottom = std::max(r_top,algPtr->m_cfg.base_radius_mm);

                                pedestal(pedestalId).height = std::min(algPtr->m_cfg.base_height_mm, pill.height);
                                pedestal(pedestalId).r_top = r_top;
                                pedestal(pedestalId).r_bottom = r_bottom;

                                merged.merge(get_mesh(pill, steps));
                            }

                        }
                        else
                            merged.merge(get_mesh(pill, steps));
                    }

                }
                else
                {
                    merged.merge(get_mesh(pill, steps));
                }

                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    meshcache.require_shared_vertices();
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);

                    meshlink.type = meshtype;
                    meshlink.id = pill.id;
                    meshlinks.emplace_back(meshlink);
                    pill.setCreatedFlg(false);
                    //if (linkPedestalflg)
                    //{
                    //   mergedforPad.merge(merged);
                    //}
               }

            }
        }
        //for pad cache
        //if (!mergedforPad.empty())
        //{
        //    TriangleMesh meshcache = to_triangle_mesh(mergedforPad);
        //    if (!meshcache.empty())
        //    {
        //        meshcache.require_shared_vertices();
        //        m_meshPadCacheNew.merge(meshcache);
        //        m_meshPadCacheNew.require_shared_vertices();
        //        m_meshPadCache_valid = true;
        //    }
        //}


    }
    break;
    case MeshTypeElement::Junction:
    {
        for (auto& j : m_junctions) 
        {
            if (ctl().stopcondition()) 
                break;
            if (j.is_valid() && (j.is_created()))
            {
                Contour3D merged;
                merged.merge(get_mesh(j, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    meshcache.require_shared_vertices();
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = j.id;
                    meshlinks.emplace_back(meshlink);
                    j.setCreatedFlg(false);
                }
            }
        }

    }
    break;
    case MeshTypeElement::Bridge:
    {
        for (auto& bs : m_bridges) 
        {
            if (ctl().stopcondition())
                break;
            if (bs.is_valid() && (bs.is_created()))
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh =std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = bs.id;
                    meshlinks.emplace_back(meshlink);
                    bs.setCreatedFlg(false);
                }
            }
        }
    }
    break;
    case MeshTypeElement::CrossBridge:
    {

        for (auto& bs : m_crossbridges) 
        {
            if (ctl().stopcondition()) 
                break;
            if (bs.is_valid() && (bs.is_created()))
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = bs.id;
                    meshlinks.emplace_back(meshlink);
                    bs.setCreatedFlg(false);
                }
            }
        }
    }
    break;
    case MeshTypeElement::DiffBridge:
    {

        for (auto& bs : m_diffbridges) 
        {
            if (ctl().stopcondition()) 
                break;
            if (bs.is_valid() && (bs.is_created()))
            {
                Contour3D merged;
                merged.merge(get_mesh(bs, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;

                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = bs.id;
                    meshlinks.emplace_back(meshlink);
                    bs.setCreatedFlg(false);
                }
            }
        }
    }
    break;
    case MeshTypeElement::Pedestal:
    {
        //Contour3D mergedforPad;
        for (auto& pedest : m_pedestals) 
        {
            if (ctl().stopcondition()) 
                break;
            if (pedest.is_valid() && (pedest.is_created()))
            {
                Contour3D merged;
                Pedestal newPedest= pedest;
                if (algPtr)
                {
                    if (newPedest.linkL.size() == 1)
                    {
                        Pillar linkPillar = pillar(newPedest.linkL[0].id);
                        double r_top = newPedest.r_top;
                        double r_bottom = newPedest.r_bottom;
                        Vec3d pedStartPt = newPedest.pos;
                        pedStartPt(Z) += newPedest.height;
                        auto pedhit = algPtr->pinhead_mesh_intersect(pedStartPt,
                            DOWN, r_top,r_bottom, newPedest.height);
                        if (pedhit.is_hit())
                        {
                            newPedest.r_top = linkPillar.r;
                            newPedest.r_bottom = linkPillar.r;
                        }
                    }
                }

                merged.merge(get_mesh(newPedest, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = pedest.id;
                    meshlinks.emplace_back(meshlink);
                    pedest.setCreatedFlg(false);
                    ////////
                    //mergedforPad.merge(merged);
                    m_meshPadCache_valid = false;
                }
            }
        }

        //if (!mergedforPad.empty())
        //{
        //    TriangleMesh meshcache = to_triangle_mesh(mergedforPad);
        //    if (!meshcache.empty())
        //    {
        //        meshcache.require_shared_vertices();
        //        m_meshPadCacheNew.merge(meshcache);
        //        m_meshPadCacheNew.require_shared_vertices();
        //        m_meshPadCache_valid = true;
        //    }
        //}
    }
    break;

    case MeshTypeElement::Anchor:
    {
        for (auto& anch : m_anchors) 
        {
            if (ctl().stopcondition()) 
                break;
            if (anch.is_valid() && (anch.is_created()))
            {
                Contour3D merged;
                if (algPtr != nullptr)
                {
                    Vec3d newpos = anch.pos + EPSILON * (-anch.dir);
                    IndexedMesh::hit_result hit;
                    hit = algPtr->ray_mesh_intersect(newpos, -anch.dir);

                    if (hit.is_inside())
                    {
                        double len = hit.distance();
                        //std::cout << "len==" << len << std::endl;
                        if (len < anch.penetration_mm)
                        {
                            len = len > EPSILON ? len : EPSILON;
                            anch.width_mm -= (anch.penetration_mm - len);
                            anch.penetration_mm = len;
                        }
                    }
                }
                merged.merge(get_mesh(anch, steps));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;

                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = anch.id;
                    meshlinks.emplace_back(meshlink);
                    anch.setCreatedFlg(false);
                }
            }
        }
    }
    break;
    case MeshTypeElement::Pad:
    {
        if (pad().showEnable)
        {
                SupportTreeNode2MeshLink meshlink;
                std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                *ptr = pad().tmesh;
                meshlink.triangleMesh = std::move(ptr);
                meshlink.type = meshtype;
                meshlink.id = 0;
                meshlinks.emplace_back(meshlink);
                m_pad.showEnable = false;
        }
    }
    break;
    case MeshTypeElement::SquarePedestal:
    {
        for (auto& pedest : m_squarePedestals)
        {
            if (ctl().stopcondition()) 
                break;
            if (pedest.is_valid() && (pedest.is_created()))
            {
                Contour3D merged;
                SquarePedestal newsqarePedest = pedest;
                if (algPtr)
                {
                    if (newsqarePedest.linkL.size() == 1)
                    {
                        Pedestal linkPedstal = pedestal(newsqarePedest.linkL[0].id);
                        double r_top = newsqarePedest.r_top;
                        double r_bottom = newsqarePedest.r_bottom;
                        Vec3d pedStartPt = newsqarePedest.pos;
                        pedStartPt(Z) += newsqarePedest.height;
                        auto pedhit = algPtr->square_mesh_intersect(pedStartPt,
                            DOWN, r_top, r_bottom, newsqarePedest.height, algPtr->m_cfg.safety_distance_mm);
                        if (pedhit.is_hit())
                        {
                            newsqarePedest.r_top = linkPedstal.r_top;
                            newsqarePedest.r_bottom = linkPedstal.r_top;
                        }
                    }
                }
                merged.merge(get_mesh(newsqarePedest));
                TriangleMesh meshcache = to_triangle_mesh(merged);

                // The mesh will be passed by const-pointer to TriangleMeshSlicer,
                // which will need this.
                if (!meshcache.empty())
                {
                    SupportTreeNode2MeshLink meshlink;
                    std::unique_ptr<TriangleMesh> ptr(new TriangleMesh());
                    meshcache.require_shared_vertices();
                    *ptr = meshcache;
                    meshlink.triangleMesh = std::move(ptr);
                    meshlink.type = meshtype;
                    meshlink.id = pedest.id;
                    meshlinks.emplace_back(meshlink);
                    pedest.setCreatedFlg(false);
                    m_meshPadCache_valid = false;
                }
            }
        }

    }
    break;
	default:
        std::cout << "error!!!unknow mesh type!meshtype=="<<(int)meshtype << std::endl;

    }
    
}
static Contour3D pinheadInner(double r_pin, double r_back, double length, size_t steps)
{
    assert(steps > 0);
    assert(length >= 0.);
    assert(r_back > 0.);
    assert(r_pin > 0.);
    static int printertimes = 0;
    if (printertimes == 0)
    {
        std::cout << "pinhead paramas" << std::endl;
        std::cout << "steps=" << steps << std::endl;
        std::cout << "length=" << length << std::endl;
        std::cout << "r_back=" << r_back << std::endl;
        std::cout << "r_pin=" << r_pin << std::endl;
        printertimes += 1;
    }
    Contour3D mesh;

    // We create two spheres which will be connected with a robe that fits
    // both circles perfectly.

    // Set up the model detail level
    const double detail = 2 * PI / steps;

    // We don't generate whole circles. Instead, we generate only the
    // portions which are visible (not covered by the robe) To know the
    // exact portion of the bottom and top circles we need to use some
    // rules of tangent circles from which we can derive (using simple
    // triangles the following relations:

    // The height of the whole mesh
    const double h = r_back + r_pin + length;
    double       phi = PI / 2. - std::acos((r_back - r_pin) / h);

    // To generate a whole circle we would pass a portion of (0, Pi)
    // To generate only a half horizontal circle we can pass (0, Pi/2)
    // The calculated phi is an offset to the half circles needed to smooth
    // the transition from the circle to the robe geometry

    auto&& s1 = sphere(r_back, make_portion(0, PI), detail);


    mesh.merge(s1);

    return mesh;
}
static  Contour3D get_headmesh(const Head& h, size_t steps)
{

    Contour3D mesh = pinheadInner(h.r_pin_mm, h.r_back_mm, h.width_mm, steps);

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

bool SupportTreeBuilder::addHead(const std::vector<Head>& heads, int add_status, std::vector<SupportTreeNode2MeshLink>& meshlink)
{
    size_t steps = CONTOUR_STEPS;//45;

    SupportTreeBuildsteps* algPtr = m_supportTreeBuildstepsPtr;
    HistoryData historyDataAdd;

    bool ret = true;
    for (const Head& head : heads)
    {
        algPtr->m_addHeadsize += 1;
        ret = algPtr->add_pinheadSupport(head, algPtr->m_addHeadsize);
        if (add_status == 0)//临时增加
        {
            algPtr->m_addHeadsize -= 1;
        }

        if (ret)
        {
            long nearestID = SupportTreeNode::ID_UNSET;
            for (int typeindex = (int)MeshTypeElement::BaseType; typeindex < (int)MeshTypeElement::TypeMax; typeindex++)
            {
                std::vector< SupportTreeNode2MeshLink> newmeshlink;
                if (typeindex == (int)MeshTypeElement::BaseType || typeindex == (int)MeshTypeElement::Pad)
                    continue;

                retrieve_meshLinkElements((MeshTypeElement)typeindex, newmeshlink);
                meshlink.insert(meshlink.end(), newmeshlink.begin(), newmeshlink.end());
            }

            for (SupportTreeNode2MeshLink& newlk : meshlink)
            {
                if (newlk.type != MeshTypeElement::Pillar)
                    continue;
                {
                    unsigned neighbors = algPtr->m_cfg.pillar_cascade_neighbors;
                    std::vector<SupportTreeNode2MeshLink> outintermeshlink;
                    std::vector< SupportTreeNode2MeshLink> newmeshlink;

                    Pillar& pillarobj = pillar(newlk.id);
                    nearestID = searchNearObjInterconnect(pillarobj, outintermeshlink);

                    retrieve_meshLinkElements(MeshTypeElement::CrossBridge, newmeshlink);
                    meshlink.insert(meshlink.end(), newmeshlink.begin(), newmeshlink.end());

                    newmeshlink.clear();
                    retrieve_meshLinkElements(MeshTypeElement::DiffBridge, newmeshlink);
                    meshlink.insert(meshlink.end(), newmeshlink.begin(), newmeshlink.end());
                }
            }

            if (add_status == 0)//临时增加
            {
                invalidateLinkRelationShip(meshlink);
                invalidateMeshLinks(meshlink, true);
                m_meshPadCacheNew.clear();
            }
            else
            {

                std::vector<HistoryData> srcdataV;
                std::vector < SUPPORT_AJUST_EVENT> eventV;
                for (SupportTreeNode2MeshLink& lk : meshlink)
                {
                    extractSupportTreeNodeChanged(lk, historyDataAdd);
                }
                srcdataV.emplace_back(historyDataAdd);
                eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_ADD);
                pushSupportTreeNodeChanged(srcdataV, eventV);
            }

        }
    }

    return true;
}

void SupportTreeBuilder::searchLeftTreeElement(SupportTreeNode& src, std::vector < SupportTreeNode2MeshLink>& outmeshlinks)
{
    SupportTreeNode2MeshLink meshlinktemp;
    MeshTypeElement type = src.treeNodeType;
    std::vector<SupportTreeNode2MeshLink>  linkL = src.linkL;
    SupportTreeNode oldsrc = src;
    long id = src.id;
    meshlinktemp.type = type;
    meshlinktemp.id = id;
    if (id == SupportTreeNode::ID_UNSET|| !src.is_valid())
    {
        return;
    }
    outmeshlinks.emplace_back(meshlinktemp);
    //std::cout << "Left Sub linkNodes****************************size=="<<linkL.size() << std::endl;
    for (int index=0; index<linkL.size(); index++)
    {
        SupportTreeNode *srctemp=NULL;
        SupportTreeNode2MeshLink linknd = linkL[index];
        MeshTypeElement linktype = linknd.type;
        long linkid = linknd.id;
        //printMeshType(linknd.type);
        if (linkid < 0)
            continue;
        switch (linktype)
        {
        case MeshTypeElement::Head:
            srctemp=&head(linkid);
            break;
        case MeshTypeElement::Pillar:
            srctemp = &pillar(linkid);
            break;
        case MeshTypeElement::Junction:
        {
            if (junction(linkid).linkR.size() == 2)
            {
                bool bridgeIDfirst = true;
                bool bridgeIDsecond = true;
                bool sameflg = (src.treeNodeType == junction(linkid).linkR[0].type) && (src.id == junction(linkid).linkR[0].id);
                if (sameflg)
                {
                    bridgeIDsecond = supportTreeNodeIsValid(junction(linkid).linkR[1]);
                    if (bridgeIDsecond == false)//另一半不可用,才继续搜索左侧
                    {
                        meshlinktemp.type = linktype;
                        meshlinktemp.id = linkid;
                        outmeshlinks.emplace_back(meshlinktemp);

                        linkL.insert(linkL.end(), junction(linkid).linkL.begin(), junction(linkid).linkL.end());

                    }
                }
                else
                {
                    sameflg = (src.treeNodeType == junction(linkid).linkR[1].type) && (src.id == junction(linkid).linkR[1].id);
                    if (sameflg)
                    {
                        bridgeIDsecond = supportTreeNodeIsValid(junction(linkid).linkR[0]);
                        if (bridgeIDsecond == false)//另一半不可用,才继续搜索左侧
                        {
                            meshlinktemp.type = linktype;
                            meshlinktemp.id = linkid;
                            outmeshlinks.emplace_back(meshlinktemp);
                            linkL.insert(linkL.end(), junction(linkid).linkL.begin(), junction(linkid).linkL.end());

                        }
                    }
                    else
                    {
                        std::cout << "jounction link error!!!!!" << std::endl;
                    }
                }

            }
            else
            {
                meshlinktemp.type = linktype;
                meshlinktemp.id = linkid;
                outmeshlinks.emplace_back(meshlinktemp);
                linkL.insert(linkL.end(), junction(linkid).linkL.begin(), junction(linkid).linkL.end());
            }

        }
        break;
        case MeshTypeElement::Bridge:
            srctemp = &bridge(linkid);
            break;
        case MeshTypeElement::CrossBridge:
            srctemp = &crossbridge(linkid);
            break;
        case MeshTypeElement::DiffBridge:
            srctemp = &diffbridge(linkid);
            break;
        case MeshTypeElement::Pedestal:
            srctemp = &pedestal(linkid);
            break;
        case MeshTypeElement::SquarePedestal:
            srctemp = &m_squarePedestals[linkid];
            break;
        default:
            std::cout << "MeshTypeElement unkown type===" << (int)type << std::endl;
            break;
        }
        if(srctemp)
        searchLeftTreeElement(*srctemp, outmeshlinks);
    }

}
void SupportTreeBuilder::searchRightTreeElement(SupportTreeNode& src, std::vector < SupportTreeNode2MeshLink>& outmeshlinks)
{
    SupportTreeNode2MeshLink meshlinktemp;
    SupportTreeNode oldsrc = src;
    MeshTypeElement type = src.treeNodeType;
    std::vector<SupportTreeNode2MeshLink>  linkR = src.linkR;
    long id = src.id;
    meshlinktemp.type = type;
    meshlinktemp.id = id;
    if (id == SupportTreeNode::ID_UNSET)
    {
        return;
    }
    {
        meshlinktemp.type = src.treeNodeType;;
        meshlinktemp.id = src.id;
        outmeshlinks.emplace_back(meshlinktemp);
    }

    //std::cout << "Right Sub linkNodes*************" << std::endl;

    for (const SupportTreeNode2MeshLink &linknd : linkR)
    {
       // printMeshType(linknd.type);
        SupportTreeNode* srctemp = NULL;
        type = linknd.type;
        id = linknd.id;
        if (id <0)
            continue;
        switch (type)
        {
        case MeshTypeElement::Head:
            srctemp = &head(id);
            break;
        case MeshTypeElement::Pillar:
        {
            std::vector<SupportTreeNode2MeshLink> linkNodes;
            linkNodes.insert(linkNodes.end(), pillar(id).linkL.begin(), pillar(id).linkL.end());
            auto item = std::find_if(linkNodes.begin(), linkNodes.end(), 
                [src](SupportTreeNode2MeshLink i)->bool {
                    return (src.treeNodeType == i.type) && (src.id == i.id);
                });

            if (item != linkNodes.end())//排除结点自身，同时确保该信息的来源
            {
                linkNodes.erase(item);
                item = std::find_if(linkNodes.begin(), linkNodes.end(), 
                [](SupportTreeNode2MeshLink linknd)
                {
                    return ((linknd.id >= 0) && (linknd.type == MeshTypeElement::Bridge || linknd.type == MeshTypeElement::DiffBridge));
                });
                if (item == linkNodes.end())//Pillar上没有Bridge链接才删除
                {
                    bool deleteflg = true;
                    for (const SupportTreeNode2MeshLink &lknd : linkNodes)
                    {
                        switch(lknd.type)
                        {
                            case MeshTypeElement::Junction :
                            {
                                std::vector < SupportTreeNode2MeshLink> meshlinksR;
                                searchLeftTreeElement(m_junctions[id], meshlinksR);
                                item = std::find_if(meshlinksR.begin(), meshlinksR.end(),
                                    [](SupportTreeNode2MeshLink linknd)
                                    {
                                        return ((linknd.id >= 0) && (linknd.type == MeshTypeElement::Head));
                                    });
                                if (item != meshlinksR.end())//Pillar上还存有Head链接,标识不删除
                                {
                                    deleteflg = false;
                                }
                            }
                            break;
                            default:
                                break;
                        }

                    }
                    if (deleteflg == true)
                    {
                        srctemp = &pillar(id);
                        for (const SupportTreeNode2MeshLink &lknd : linkNodes)
                        {
                            meshlinktemp.type = lknd.type;
                            meshlinktemp.id = lknd.id;
                            outmeshlinks.emplace_back(meshlinktemp);

                        }

                    }
                }
            }

        }
            break;
        case MeshTypeElement::Junction:
            srctemp = &m_junctions[id];
            break;
        case MeshTypeElement::Bridge:
            srctemp = &m_bridges[id];
            break;
        case MeshTypeElement::CrossBridge:
            srctemp = &m_crossbridges[id];
            break;
        case MeshTypeElement::DiffBridge:
            srctemp = &m_diffbridges[id];
            break;
        case MeshTypeElement::Pedestal:
            srctemp = &m_pedestals[id];
            break;
        case MeshTypeElement::Anchor:
            srctemp = &m_anchors[id];
            break;
        case MeshTypeElement::SquarePedestal:
            srctemp = &m_squarePedestals[id];
            break;
        default:
            std::cout << "MeshTypeElement unkown type===" << (int)type << std::endl;
            break;
        }
        if (srctemp)
        {
            searchRightTreeElement(*srctemp, outmeshlinks);
        }
    }


}


void SupportTreeBuilder::searchMeshLinks(Slic3r::sla::SupportTreeNode2MeshLink source, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
    MeshTypeElement meshtype = source.type;
    long id = source.id;
    if (id < 0)
    {
        std::cout << "invalidateTreeElement obj ===NULL" << std::endl;
        return;
    }
    switch (meshtype)
    {
    case MeshTypeElement::Head:
    {
        Head& obj = head(id);
        searchRightTreeElement(obj, outmeshlinks);
    }
    break;
    case MeshTypeElement::Pillar:
    {
        Pillar& pill = pillar(id);
        bool pillDeleteFlg = true;
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> pillarMeshlinks;
        //for (SupportTreeNode2MeshLink linknd : pill.linkL)
        //{
        //    printMeshType(linknd.type);
        //}
        //std::cout << "Sub linkNodes" << std::endl;
        searchLeftTreeElement(pill, pillarMeshlinks);
        if(pillarMeshlinks.size()>0)
            outmeshlinks.insert(outmeshlinks.end(), pillarMeshlinks.begin(), pillarMeshlinks.end());

        pillarMeshlinks.clear();
        searchRightTreeElement(pill, pillarMeshlinks);
        if (pillarMeshlinks.size() > 1)
            outmeshlinks.insert(outmeshlinks.end(), pillarMeshlinks.begin()+1, pillarMeshlinks.end());
        //for (SupportTreeNode2MeshLink linknd : pill.linkR)
        //{
        //    printMeshType(linknd.type);
        //}
        //std::cout << "Sub linkNodes" << std::endl;

    }
    break;
    case MeshTypeElement::CrossBridge:
    {
        CrossBridge& bri = crossbridge(id);
        if (bri.is_valid())
        {
            SupportTreeNode2MeshLink meshlinktemp;
            meshlinktemp.type = MeshTypeElement::CrossBridge;
            meshlinktemp.id = bri.id;
            outmeshlinks.emplace_back(meshlinktemp);
        }
    }
    break;
    case MeshTypeElement::Anchor:
    {
        Anchor& obj = anchor(id);
        searchLeftTreeElement(obj, outmeshlinks);
    }
    break;
    case MeshTypeElement::Pedestal:
    {
        Pedestal& obj = pedestal(id);
        searchLeftTreeElement(obj, outmeshlinks);
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> searchMeshlinks;

        searchRightTreeElement(obj, searchMeshlinks);
        if (searchMeshlinks.size() > 1)
            outmeshlinks.insert(outmeshlinks.end(), searchMeshlinks.begin() + 1, searchMeshlinks.end());

    }
    break;
    case MeshTypeElement::SquarePedestal:
    {
        SquarePedestal& obj = squarePedestal(id);
        searchLeftTreeElement(obj, outmeshlinks);
    }
    break;
    default:
    {
        std::cout << "not search type==";
        printMeshType(meshtype);
    }
    }

    //std::unique(outmeshlinks.begin(), outmeshlinks.end(),
    //    [this](Slic3r::sla::SupportTreeNode2MeshLink i, Slic3r::sla::SupportTreeNode2MeshLink j) {
    //        return (i.type == j.type) && (i.id == j.id);
    //    });
}

void SupportTreeBuilder::invalidateMeshLinks(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inMeshlinks, bool pop_up)
{
    //////////////////////////////////////////
    for (const SupportTreeNode2MeshLink& linkout : inMeshlinks)
    {
       invalidateMeshLink(linkout, pop_up);
    }
}
void SupportTreeBuilder::invalidateMeshLink(const SupportTreeNode2MeshLink& linkout, bool pop_up)
{
    //////////////////////////////////////////
        long id = linkout.id;
        if (id < 0)
        {
            return;
        }
        if (pop_up)
        {
            std::cout << "invalidateMeshLinks start  pop_up value==" << pop_up << std::endl;
            printMeshType(linkout.type);
        }
        SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
        if (algPtr == NULL)
        {
            return;
        }
        switch (linkout.type)
        {
        case MeshTypeElement::Head:
        {
            if (pop_up)
            {
                auto item1 = std::find_if(m_heads.begin(), m_heads.end(),
                    [linkout](Head i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item1 != m_heads.end())
                {
                    m_heads.erase(item1);
                    {
                        m_head_indices.clear();
                        algPtr->m_iheads.clear();

                        for (long index = 0; index < m_heads.size(); index++)
                        {
                            Head& hd = m_heads[index];
                            //add_head(hd.id, hd);
                            if (hd.id > SupportTreeNode::ID_UNSET)
                            {
                                if (hd.id >= m_head_indices.size())
                                    m_head_indices.resize(hd.id + 1);
                                m_head_indices[hd.id] = index;
                                algPtr->m_iheads.emplace_back(hd.id);
                            }
                        }
                    }

                }
            }
            else
            {
                Head& hd = head(id);
                hd.invalidate();
            }

        }
        break;
        case MeshTypeElement::Pillar:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_pillars.begin(), m_pillars.end(),
                    [linkout](Pillar i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_pillars.end())
                {
                    algPtr->m_pillar_index.guarded_remove(item->endpoint(), item->id);
                    algPtr->m_pillar_index_start.guarded_remove(item->startpoint(), item->id);
                    m_pillars.erase(item);
                }

            }
            else
            {
                m_pillars[id].invalidate();
            }

        }
        break;
        case MeshTypeElement::Junction:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_junctions.begin(), m_junctions.end(),
                    [linkout](Junction i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_junctions.end())
                    m_junctions.erase(item);
            }
            else
                m_junctions[id].invalidate();


        }
        break;
        case MeshTypeElement::Bridge:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_bridges.begin(), m_bridges.end(),
                    [linkout](Bridge i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_bridges.end())
                    m_bridges.erase(item);
            }
            else
                m_bridges[id].invalidate();

        }
        break;
        case MeshTypeElement::CrossBridge:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_crossbridges.begin(), m_crossbridges.end(),
                    [linkout](CrossBridge i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_crossbridges.end())
                    m_crossbridges.erase(item);
            }
            else
                m_crossbridges[id].invalidate();

        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_diffbridges.begin(), m_diffbridges.end(),
                    [linkout](DiffBridge i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_diffbridges.end())
                    m_diffbridges.erase(item);
            }
            else
                m_diffbridges[id].invalidate();

        }
        break;
        case MeshTypeElement::Pedestal:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_pedestals.begin(), m_pedestals.end(),
                    [linkout](Pedestal i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_pedestals.end())
                    m_pedestals.erase(item);
            }
            else
            {
                m_meshPadCache_valid = false;
                m_pedestals[id].invalidate();
            }


        }
        break;
        case MeshTypeElement::Anchor:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_anchors.begin(), m_anchors.end(),
                    [linkout](Anchor i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_anchors.end())
                    m_anchors.erase(item);
            }
            else
                m_anchors[id].invalidate();


        }
        break;
        case MeshTypeElement::SquarePedestal:
        {
            if (pop_up)
            {
                auto item = std::find_if(m_squarePedestals.begin(), m_squarePedestals.end(),
                    [linkout](SquarePedestal i)->bool {
                        return (i.treeNodeType == linkout.type) && (i.id == linkout.id);
                    });
                if (item != m_squarePedestals.end())
                    m_squarePedestals.erase(item);
            }
            else
            {
                m_meshPadCache_valid = false;
                m_squarePedestals[id].invalidate();
            }


        }
        break;
        }

}
void SupportTreeBuilder::popupUndoHistorydataInner()
{
    m_EventHistoryData.clear();
    if (!m_EventHistoryDatasV.empty())
    {
        m_EventHistoryData = m_EventHistoryDatasV.back();
        m_EventHistoryDatasV.pop_back();

        pushRedoHistoryData(m_EventHistoryData);
    }
}
void SupportTreeBuilder::popupRedoHistorydataInner()
{
    m_EventHistoryData.clear();
    if (!m_RedoEventHistoryDatasV.empty())
    {
        m_EventHistoryData = m_RedoEventHistoryDatasV.back();
        m_RedoEventHistoryDatasV.pop_back();
        {
            std::vector<HistoryDataPair> sameHistoryData;
            for (int index = 0; index < m_EventHistoryData.size(); index++)
            {
                HistoryData srcdata;
                SUPPORT_AJUST_EVENT event = SUPPORT_AJUST_EVENT::EVENT_NONE;
                switch (m_EventHistoryData[index].first)
                {
                case SUPPORT_AJUST_EVENT::EVENT_ADD:
                {
                    srcdata = m_EventHistoryData[index].second;
                    event = SUPPORT_AJUST_EVENT::EVENT_ADD;
                    sameHistoryData.emplace_back(std::make_pair(event, srcdata));
                }
                break;
                case SUPPORT_AJUST_EVENT::EVENT_MOVE:
                {
                    HistoryData srcdatatemp = m_EventHistoryData[index].second;
                    for (const auto& obj : srcdatatemp.m_heads)
                    {
                        srcdata.m_heads.emplace_back(head(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_pillars)
                    {
                        srcdata.m_pillars.emplace_back(pillar(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_junctions)
                    {
                        srcdata.m_junctions.emplace_back(junction(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_bridges)
                    {
                        srcdata.m_bridges.emplace_back(bridge(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_crossbridges)
                    {
                        srcdata.m_crossbridges.emplace_back(crossbridge(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_diffbridges)
                    {
                        srcdata.m_diffbridges.emplace_back(diffbridge(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_pedestals)
                    {
                        srcdata.m_pedestals.emplace_back(pedestal(obj.id));
                    }
                    for (const auto& obj : srcdatatemp.m_anchors)
                    {
                        srcdata.m_anchors.emplace_back(anchor(obj.id));
                    }
                    event = SUPPORT_AJUST_EVENT::EVENT_MOVE;
                    sameHistoryData.emplace_back(std::make_pair(event, srcdata));
                }
                break;
                case SUPPORT_AJUST_EVENT::EVENT_DELETE:
                {
                    srcdata = m_EventHistoryData[index].second;
                    event = SUPPORT_AJUST_EVENT::EVENT_DELETE;
                    sameHistoryData.emplace_back(std::make_pair(event, srcdata));

                }
                break;
                }

            }
            pushUndoHistoryData(sameHistoryData);
        }
    }
}
void SupportTreeBuilder::undoHistoryDataMeshLinks(std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_AJUST_EVENT& event)
{
    if (m_EventHistoryData.size() > 0)
    {
        auto item = m_EventHistoryData.begin();
        undoConstructMeshLink(item->second, item->first, outmeshlinks);
        event = item->first;
        m_EventHistoryData.erase(item);

    }
    else
    {
        outmeshlinks.clear();
        event = SUPPORT_AJUST_EVENT::EVENT_NONE;
    }

}
void SupportTreeBuilder::redoHistoryDataMeshLinks(std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_AJUST_EVENT& event)
{
    if (m_EventHistoryData.size() > 0)
    {
        auto item = m_EventHistoryData.begin();
        redoConstructMeshLink(item->second, item->first, outmeshlinks);
        event = item->first;
        m_EventHistoryData.erase(item);

    }
    else
    {
        outmeshlinks.clear();
        event = SUPPORT_AJUST_EVENT::EVENT_NONE;
    }

}
void SupportTreeBuilder::invalidateTreeElement(Slic3r::sla::SupportTreeNode2MeshLink source, std::vector < SupportTreeNode2MeshLink>& outmeshlinks)
{
    searchMeshLinks(source, outmeshlinks);
    pushSupportTreeHistoryData(outmeshlinks, SUPPORT_AJUST_EVENT::EVENT_DELETE);
    invalidateMeshLinks(outmeshlinks);
    
}

void SupportTreeBuilder::moveTreeElement(Slic3r::sla::SupportTreeNode2MeshLink source, Slic3r::sla::MoveOffSet inOffset, std::vector < SupportTreeNode2MeshLink>& oldmeshlinks)
{
    MeshTypeElement meshtype = source.type;
    long id = source.id;
    SUPPORT_EVENT_FLAG srceventflg = source.eventFlg;
    Vec3d offset = inOffset.ptEnd - inOffset.ptStart;
    std::cout << "moveTreeElement offset==" << offset << std::endl;
    HistoryData historyDataMove;
    HistoryData historyDataAdd;
    HistoryData historyDataDelete;
    bool historyDataEnable = false;
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    if (algPtr == NULL)
        return;
    if (id < 0)
        return;
    if (supportTreeNodeIsValid(source)==false)
    {
        return;
    }
    switch (meshtype)
    {
    case MeshTypeElement::Pedestal:
    {
        assert(pedestal(id).linkL.size() == 1);
        if (pedestal(id).linkL[0].type == MeshTypeElement::Pillar)
        {
            Pillar& pill = pillar(pedestal(id).linkL[0].id);
            if (pill.selfType == 1 || pill.bridgePillar == true)
            {
                return;
            }
            meshtype = pill.treeNodeType;
            id = pill.id;
        }
    }
    break;
    }
    switch(meshtype)
    {
        case MeshTypeElement::Pillar:
        {
            std::vector < SupportTreeNode2MeshLink> meshlinksR;
            Pillar& pill = pillar(id);
            if (pill.selfType == 1)
            {
                return;
            }
            if (!pill.bridgePillar)
            {
                searchRightTreeElement(pill, meshlinksR);
                for (SupportTreeNode2MeshLink lkR : meshlinksR)
                {
                    if (reconstructTreeNode(lkR, offset, oldmeshlinks, srceventflg))
                    {
                        return;
                    }

                }
            }
        }
            break;
    
    }
    switch (meshtype)
    {
    case MeshTypeElement::Head:
    {
        Head& hd = head(id);
        offset = inOffset.ptEnd;

        SupportTreeNode2MeshLink meshlinktemp;
        Vec3d jpt = hd.junction_point();
        Vec3d dir = jpt- offset;
        IndexedMesh::hit_result hit = algPtr->m_mesh.query_ray_hit(jpt, -dir.normalized());

        //if (!std::isinf(hit.distance()))
        if (hit.is_hit()&&
            !hit.is_inside()
            )
        {
             double len = 0.0;
             offset = hit.position();
             dir = jpt - offset;
             len = dir.norm() + hd.penetration_mm - 2 * hd.r_pin_mm - hd.r_back_mm;

            std::cout << "*************************hit face id==" << hit.face() << std::endl;
            auto [polar, azimuth] = dir_to_spheric(-dir.normalized());

            auto [polarBefore, azimuthBefore] = dir_to_spheric(hd.dir);

            // skip if the tilt is not sane
            if (polar > PI - algPtr->m_cfg.normal_move_cutoff_angle)
            {
                if (polar > polarBefore)
                {
                    std::cout << __FILE__ << __LINE__ << "cut off angle polar==" << polar * 180.0 / PI << std::endl;
                    return;
                }
            }
            if (len> algPtr->m_cfg.max_bridge_length_mm)
            {
                return;
            }
            if (len > 0)
            {
                meshlinktemp.type = MeshTypeElement::Head;
                meshlinktemp.id = hd.id;
                oldmeshlinks.emplace_back(meshlinktemp);
                extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                hd.setCreatedFlg(true);
                hd.pos = offset;
                hd.dir = dir.normalized();
                hd.width_mm = len;
            }
        }


    }
    break;
    case MeshTypeElement::Pillar:
    {
        Pillar pill = pillar(id);
        long   pillId = pill.id;
        std::vector<long> crossBridgeIDs;
        Slic3r::Vec3d pill_endptold = pill.endpt;
        {
            if (!pill.is_valid())
                return;
            for (const SupportTreeNode2MeshLink& linkl : pill.linkL)
            {
                if (linkl.type == MeshTypeElement::Junction)
                {
                    Junction& jun = junction(linkl.id);
                    for (SupportTreeNode2MeshLink& lk : jun.linkL)
                    {
                        switch (lk.type)
                        {
                        case MeshTypeElement::Head:
                        {
                            Vec3d dir(0, 0, 1);
                            double len = 0.0;
                            Head& hd = head(lk.id);
                            Vec3d jpt = hd.junction_point();
                            jpt(X) += offset(X);
                            jpt(Y) += offset(Y);
                            dir = jpt - hd.pos;
                            len = dir.norm() + hd.penetration_mm - 2 * hd.r_pin_mm - hd.r_back_mm;
                            auto [polar, azimuth] = dir_to_spheric(-dir.normalized());
                            auto [polarBefore, azimuthBefore] = dir_to_spheric(hd.dir);

                            // skip if the tilt is not sane
                            if ((polar > PI - algPtr->m_cfg.normal_move_cutoff_angle))
                            {
                                if (polar > polarBefore)
                                {
                                    std::cout << __FILE__ << __LINE__ << "cut off angle polar==" << polar * 180.0 / PI << std::endl;
                                    return;
                                }
                            }
                            if (len > algPtr->m_cfg.max_bridge_length_mm)
                            {
                                return;
                            }


                        }
                        break;
                        case MeshTypeElement::Bridge:
                        {
                            Vec3d dir(0, 0, 1);
                            Bridge& bri = bridge(lk.id);
                            bool startpMove = true;
                            if(bri.briStartplink.size() == 0)
                            return;
                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                            {
                                if (bri.briStartplink[0].type == jun.treeNodeType && bri.briStartplink[0].id == jun.id)
                                    startpMove = true;
                                else
                                    startpMove = false;
                            }
                            if (startpMove)
                            {
                                Vec3d jpt = bri.startp;
                                jpt(X) += offset(X);
                                jpt(Y) += offset(Y);
                                dir = bri.endp - jpt;

                            }
                            else
                            {
                                Vec3d jpt = bri.endp;
                                jpt(X) += offset(X);
                                jpt(Y) += offset(Y);
                                dir = bri.startp - jpt;
                            }
                            auto [polar, azimuth] = dir_to_spheric(dir.normalized());
                            auto [polarBefore, azimuthBefore] = dir_to_spheric(bri.get_dir());

                            // skip if the tilt is not sane
                            if (polar > PI - algPtr->m_cfg.normal_move_cutoff_angle)
                            {
                                if (polar > polarBefore)
                                {
                                    std::cout << __FILE__ << __LINE__ << "cut off angle polar==" << polar * 180.0 / PI << std::endl;
                                    return;
                                }
                            }
                            if (dir.norm() > algPtr->m_cfg.max_bridge_length_mm)
                            {
                                return;
                            }

                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            Vec3d dir(0, 0, 1);
                            DiffBridge& bri = diffbridge(lk.id);
                            bool startpMove = true;
                            if(bri.briStartplink.size() == 0)
                            return;
                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                            {
                                if (bri.briStartplink[0].type == jun.treeNodeType && bri.briStartplink[0].id == jun.id)
                                    startpMove = true;
                                else
                                    startpMove = false;
                            }
                            if (startpMove)
                            {
                                Vec3d jpt = bri.startp;
                                jpt(X) += offset(X);
                                jpt(Y) += offset(Y);
                                dir = bri.endp - jpt;

                            }
                            else
                            {
                                Vec3d jpt = bri.endp;
                                jpt(X) += offset(X);
                                jpt(Y) += offset(Y);
                                dir = bri.startp - jpt;
                            }
                            auto [polar, azimuth] = dir_to_spheric(dir.normalized());

                            auto [polarBefore, azimuthBefore] = dir_to_spheric(bri.get_dir());

                            // skip if the tilt is not sane
                            if (polar > PI - algPtr->m_cfg.normal_move_cutoff_angle)
                            {
                                if (polar > polarBefore)
                                {
                                    std::cout << __LINE__ << " cut off angle polar==" << polar * 180.0 / PI << std::endl;
                                    return;
                                }
                            }
                            if (dir.norm() > algPtr->m_cfg.max_bridge_length_mm)
                            {
                                return;
                            }

                        }
                        break;
                        default:
                            std::cout << "should move obj type==" << (int)jun.linkL[0].type << std::endl;
                        }
                    }
                }
            }
            for (const SupportTreeNode2MeshLink& linkl : pill.linkR)
            {
                switch (linkl.type)
                {
                    case MeshTypeElement::Pedestal:
                    {
                        Vec3d pts = pill.startpoint();
                        pts(X) += offset(X);
                        pts(Y) += offset(Y);
                        auto hitnew = algPtr->bridge_mesh_intersect(pts, DOWN, pill.r);
                        if (!std::isinf(hitnew.distance()))
                        {
                            std::cout  << __LINE__ << "  " << " need reconstruct ,but not " << std::endl;
                            return;
                        }

                    }
                    break;
                    case MeshTypeElement::Anchor:
                    {
                        Vec3d pts = pill.startpoint();
                        pts(X) += offset(X);
                        pts(Y) += offset(Y);
                        auto hitnew = algPtr->bridge_mesh_intersect(pts, DOWN, pill.r);
                        if (std::isinf(hitnew.distance()))
                        {
                            std::cout << __LINE__ << "  " << " need reconstruct ,but not " << std::endl;
                            return;
                        }
                        else
                        {
                            Vec3d hitp = hitnew.position();
                            double width_mm = 2 * anchor(linkl.id).r_pin_mm + anchor(linkl.id).r_back_mm + algPtr->m_cfg.head_penetration_mm;
                            double zangle = std::asin(hitnew.direction()(Z));
                            zangle = std::max(zangle, PI / 4);
                            double h = std::sin(zangle) * width_mm;
                            h = std::min(hitnew.distance() - anchor(linkl.id).r_back_mm, h);
                            if (h < EPSILON)
                            {
                                return;
                            }
                        }

                    }
                    break;
                }
            }
        }
        {
            SupportTreeNode2MeshLink meshlinktemp;
            meshlinktemp.type = MeshTypeElement::Pillar;
            meshlinktemp.id = pillId;
            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
            pillar(pillId).endpt(X) += offset(X);
            pillar(pillId).endpt(Y) += offset(Y);
            pillar(pillId).setCreatedFlg(true);
            oldmeshlinks.emplace_back(meshlinktemp);
        }
        ////main pillar left side
        for (const SupportTreeNode2MeshLink &linknd : pill.linkL)
        {
            if (supportTreeNodeIsValid(linknd))
            {
                //printMeshType(linknd.type);
                switch (linknd.type)
                {
                case MeshTypeElement::Bridge:
                {
                    Bridge& bri = bridge(linknd.id);
                    SupportTreeNode2MeshLink meshlinktemp;
                    bool startpMove = true;
                    meshlinktemp.type = MeshTypeElement::Bridge;
                    meshlinktemp.id = bri.id;
                    oldmeshlinks.emplace_back(meshlinktemp);
                    extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                    if (bri.briStartplink.size() == 0)
                        startpMove = false;
                    else
                    {
                        if (supportTreeNodeIsValid(bri.briStartplink[0]))
                        {
                            if (bri.briStartplink[0].type == pill.treeNodeType && bri.briStartplink[0].id == pill.id)
                                startpMove = true;
                            else
                                startpMove = false;
                        }
                    }
                    if (startpMove)
                    {
                        bri.startp(X) = pillar(pillId).endpt(X);
                        bri.startp(Y) = pillar(pillId).endpt(Y);
                    }
                    else
                    {
                        bri.endp(X) = pillar(pillId).endpt(X);
                        bri.endp(Y) = pillar(pillId).endpt(Y);
                    }

                    bri.setCreatedFlg(true);
                }
                break;
                case MeshTypeElement::CrossBridge:
                {
                    CrossBridge& bri = crossbridge(linknd.id);
                    if (bri.is_valid())
                        crossBridgeIDs.emplace_back(linknd.id);
                }
                break;
                case MeshTypeElement::DiffBridge:
                {
                    DiffBridge& bri = diffbridge(linknd.id);
                    SupportTreeNode2MeshLink meshlinktemp;
                    bool startpMove = false;
                    if (bri.briStartplink.size() == 0)
                        startpMove = false;
                    else
                    {
                        if (supportTreeNodeIsValid(bri.briStartplink[0]))
                        {
                            if (bri.briStartplink[0].type == pill.treeNodeType && bri.briStartplink[0].id == pill.id)
                                startpMove = true;
                            else
                                startpMove = false;
                        }
                    }
                    extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                    if (startpMove)
                    {
                        bri.startp(X) = pillar(pillId).endpt(X);
                        bri.startp(Y) = pillar(pillId).endpt(Y);
                    }
                    else
                    {
                        bri.endp(X) = pillar(pillId).endpt(X);
                        bri.endp(Y) = pillar(pillId).endpt(Y);
                    }
                    bri.setCreatedFlg(true);
                    meshlinktemp.type = MeshTypeElement::CrossBridge;
                    meshlinktemp.id = bri.id;
                    oldmeshlinks.emplace_back(meshlinktemp);
                }
                break;
                case MeshTypeElement::Junction:
                {
                    Junction& junc = junction(linknd.id);
                    //
                    if(junc.is_valid())
                    {
                        SupportTreeNode2MeshLink meshlinktemp;
                        meshlinktemp.type = MeshTypeElement::Junction;
                        meshlinktemp.id = junc.id;
                        extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                        //junc.pos += offset;
                        junc.pos(X) = pillar(pillId).endpt(X);
                        junc.pos(Y) = pillar(pillId).endpt(Y);

                        junc.setCreatedFlg(true);
                        oldmeshlinks.emplace_back(meshlinktemp);

                    }
                    for (const SupportTreeNode2MeshLink &lk : junc.linkL)
                    {
                        switch (lk.type)
                        {
                        case MeshTypeElement::Head:
                        {
                            Head& hd = head(lk.id);
                            Vec3d jpt;
                            Vec3d dir;
                            SupportTreeNode2MeshLink meshlinktemp;
                            meshlinktemp.type = MeshTypeElement::Head;
                            meshlinktemp.id = hd.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                            jpt = junc.pos;
                            dir = jpt - hd.pos;
                            //hd.width_mm = dir.norm() - hd.r_pin_mm - hd.r_back_mm;
                            hd.width_mm = dir.norm() + hd.penetration_mm - hd.r_pin_mm - hd.r_back_mm;
                            hd.dir = dir.normalized();
                            hd.setCreatedFlg(true);

                        }
                        break;
                        case MeshTypeElement::Bridge:
                        {
                            Bridge& bri = bridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::Bridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        case MeshTypeElement::CrossBridge:
                        {
                            CrossBridge& bri = crossbridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::CrossBridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            DiffBridge& bri = diffbridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::DiffBridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);

                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        default:
                            std::cout << "should move obj type==" << (int)junc.linkL[0].type << std::endl;
                        }
                    }

                }
                break;
                default:
                    break;

                }
            }
        }

        ////main pillar right side
        for (const SupportTreeNode2MeshLink &linknd : pill.linkR)
        {
            if (supportTreeNodeIsValid(linknd))
            {
                //printMeshType(linknd.type);
                switch (linknd.type)
                {
                case MeshTypeElement::Junction:
                {
                    Junction& junc = junction(linknd.id);
                    if(junc.is_valid())
                    {
                        SupportTreeNode2MeshLink meshlinktemp;
                        meshlinktemp.type = MeshTypeElement::Junction;
                        meshlinktemp.id = junc.id;
                        extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                        //junc.pos += offset;
                        junc.pos(X) = pillar(pillId).endpt(X);
                        junc.pos(Y) = pillar(pillId).endpt(Y);

                        junc.setCreatedFlg(true);
                        oldmeshlinks.emplace_back(meshlinktemp);
                    }
                    for (const SupportTreeNode2MeshLink &lk : junc.linkR)
                    {
                        switch (lk.type)
                        {
                        case MeshTypeElement::Bridge:
                        {
                            Bridge& bri = bridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::Bridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        case MeshTypeElement::CrossBridge:
                        {
                            CrossBridge& bri = crossbridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::CrossBridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            DiffBridge& bri = diffbridge(lk.id);
                            SupportTreeNode2MeshLink meshlinktemp;
                            bool startpMove = true;
                            meshlinktemp.type = MeshTypeElement::DiffBridge;
                            meshlinktemp.id = bri.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);

                            if (bri.briStartplink.size() == 0)
                                startpMove = false;
                            else
                            {
                                if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                {
                                    if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                        startpMove = true;
                                    else
                                        startpMove = false;
                                }
                            }
                            if (startpMove)
                            {
                                bri.startp = junc.pos;
                            }
                            else
                            {
                                bri.endp = junc.pos;
                            }
                            bri.setCreatedFlg(true);
                        }
                        break;
                        default:
                            std::cout << "should move obj type==" << (int)junc.linkL[0].type << std::endl;
                        }
                    }

                }
                break;
                case MeshTypeElement::Pedestal:
                {
                    SupportTreeNode2MeshLink meshlinktemp;
                    meshlinktemp.type = MeshTypeElement::Pedestal;
                    meshlinktemp.id = linknd.id;
                    oldmeshlinks.emplace_back(meshlinktemp);
                    extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);

                    pedestal(linknd.id).pos(X) = pillar(pillId).endpt(X);
                    pedestal(linknd.id).pos(Y) = pillar(pillId).endpt(Y);

                    pedestal(linknd.id).setCreatedFlg(true);

                    if (pedestal(linknd.id).linkR.size()==0)
                    {
                            Vec3d startpt = pedestal(linknd.id).pos;
                            startpt(Z) += algPtr->m_cfg.squarebase_height_mm;
                            IndexedMesh::hit_result hitret = algPtr->pinhead_mesh_intersect(startpt, Vec3d(0.0, 0.0, -1.0), algPtr->m_cfg.squarebase_radius_mm, algPtr->m_cfg.squarebase_radius_mm, algPtr->m_cfg.squarebase_height_mm);
                            if (!hitret.is_hit())
                            {
                                m_squarePedestals.emplace_back(pedestal(linknd.id).pos, algPtr->m_cfg.squarebase_height_mm, algPtr->m_cfg.squarebase_radius_mm, algPtr->m_cfg.squarebase_top_radius_mm);

                                m_squarePedestals.back().id = m_squarePedestals.size() - 1;

                                buildMeshLinkRelation(pedestal(linknd.id), m_squarePedestals.back());
                                meshlinktemp.type = MeshTypeElement::SquarePedestal;
                                meshlinktemp.id = m_squarePedestals.back().id;
                                oldmeshlinks.emplace_back(meshlinktemp);

                            }
                    }
                    else
                    {
                        for (const SupportTreeNode2MeshLink& lk : pedestal(linknd.id).linkR)
                        {
                            switch (lk.type)
                            {
                            case MeshTypeElement::SquarePedestal:
                            {
                                SquarePedestal& sPedestal = squarePedestal(lk.id);
                                SupportTreeNode2MeshLink meshlinktemp;
                                bool startpMove = true;
                                meshlinktemp.type = MeshTypeElement::SquarePedestal;
                                meshlinktemp.id = sPedestal.id;
                                oldmeshlinks.emplace_back(meshlinktemp);
                                extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);

                                sPedestal.pos(X) = pedestal(linknd.id).pos(X);
                                sPedestal.pos(Y) = pedestal(linknd.id).pos(Y);
                                sPedestal.setCreatedFlg(true);
                            }
                            break;
                            default:
                                std::cout << __LINE__ << "should move obj type SquarePedestal" << std::endl;
                            }
                        }
                    }


                }
                break;
                case MeshTypeElement::Anchor:
                {
                    Anchor& ach = anchor(linknd.id);
                    Vec3d startpt = pill.startpoint();
                    startpt(X) = pillar(pillId).endpt(X);
                    startpt(Y) = pillar(pillId).endpt(Y);
                    auto hit = algPtr->bridge_mesh_intersect(startpt, DOWN, pill.r);
                    if (hit.is_hit())
                    {

                        Vec3d hitp = hit.position();
                        double width_mm = 2 * ach.r_pin_mm + ach.r_back_mm + algPtr->m_cfg.head_penetration_mm;
                        double zangle = std::asin(hit.direction()(Z));
                        zangle = std::max(zangle, PI / 4);
                        double h = std::sin(zangle) * width_mm;
                        h = std::min(hit.distance() - ach.r_back_mm, h);

                       // std::cout << __FUNCTION__ << " " << __LINE__ << " pillar len==" << hit.distance() << std::endl;
                        if (h > 0)
                        {
                            Vec3d endpt{ startpt(X), startpt(Y), startpt(Z) - hit.distance() + h };
                            Vec3d taildir = endpt - hitp;
                            double dist = (hitp - endpt).norm() + algPtr->m_cfg.head_penetration_mm;
                            double w = dist - 2 * ach.r_pin_mm - ach.r_back_mm;
                            SupportTreeNode2MeshLink meshlinktemp;
                            if (w < 0)
                                w = 0;
                            meshlinktemp.type = MeshTypeElement::Anchor;
                            meshlinktemp.id = ach.id;
                            oldmeshlinks.emplace_back(meshlinktemp);
                            extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);

                            pillar(pillId).endpt = endpt;
                            pillar(pillId).height = startpt(Z) - endpt(Z);

                            ach.pos = hitp;
                            ach.width_mm = w;
                            ach.dir = taildir.normalized();
                            ach.setCreatedFlg(true);
                        }
                        else//强制合成在主支撑中
                        {
                            pillar(pillId).endpt = hit.position();
                            pillar(pillId).height = startpt(Z) - pillar(id).endpt(Z);

                        }

                    }


                }
                break;
                default:
                    break;

                }
            }
        }

        {

            if (crossBridgeIDs.size()==0)//由于重建后再次产生链接
            {
                std::cout << "**************************searchInterconnected=="<< std::endl;
                std::vector<SupportTreeNode2MeshLink> outintermeshlink;
                pill.links = 0;
                long nearestID = searchNearObjInterconnect(pill, outintermeshlink);
                for (SupportTreeNode2MeshLink& lk : outintermeshlink)
                {
                    extractSupportTreeNodeChanged(lk, historyDataAdd);
                    oldmeshlinks.emplace_back(lk);
                    historyDataEnable = true;
                }
            }
            else
            {
                bool crossbridgeLenOut = false;
                for (auto& crossBridgeid : crossBridgeIDs)
                {
                    CrossBridge& bri = crossbridge(crossBridgeid);
                    double brilen = bri.get_length();
                    if (srceventflg == SUPPORT_EVENT_FLAG::EVENT_FLAG_MOVE_START)
                        break;
                    if (brilen > algPtr->m_cfg.max_bridge_length_mm)
                    {
                        crossbridgeLenOut = true;
                        break;
                    }

                }

                if (crossbridgeLenOut)
                {
                    std::cout << "*****************************crossbridgeLenOut==" << crossbridgeLenOut << std::endl;

                    std::vector< Slic3r::sla::SupportTreeNode2MeshLink> meshlinksDeleted;
                    for (auto& crossBridgeid : crossBridgeIDs)
                    {
                        CrossBridge& bri = crossbridge(crossBridgeid);
                        SupportTreeNode2MeshLink meshlinktemp;
                        meshlinktemp.type = MeshTypeElement::CrossBridge;
                        meshlinktemp.id = bri.id;
                        oldmeshlinks.emplace_back(meshlinktemp);
                        meshlinksDeleted.emplace_back(meshlinktemp);
                        extractSupportTreeNodeChanged(meshlinktemp, historyDataDelete);
                        invalidateRelationShip(&crossbridge(crossBridgeid));
                        bri.invalidate();
                        historyDataEnable = true;
                    }
                    //invalidateLinkRelationShip(meshlinksDeleted);
                    ///////
                    {
                        std::vector<SupportTreeNode2MeshLink> outintermeshlink;
                        pill.links = 0;
                        long nearestID = searchNearObjInterconnect(pill, outintermeshlink);
                        for (SupportTreeNode2MeshLink& lk : outintermeshlink)
                        {
                            extractSupportTreeNodeChanged(lk, historyDataAdd);
                            oldmeshlinks.emplace_back(lk);
                            historyDataEnable = true;
                        }
                    }


                }
                else
                {
                    for (auto& crossBridgeid : crossBridgeIDs)
                    {
                        CrossBridge& bri = m_crossbridges[crossBridgeid];
                        SupportTreeNode2MeshLink meshlinktemp;
                        meshlinktemp.type = MeshTypeElement::CrossBridge;
                        meshlinktemp.id = bri.id;
                        oldmeshlinks.emplace_back(meshlinktemp);

                        bool startpMove = true;
                        if (bri.briStartplink.size() == 0)
                            startpMove = false;
                        else
                        {
                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                            {
                                if (bri.briStartplink[0].type == pill.treeNodeType && bri.briStartplink[0].id == pill.id)
                                    startpMove = true;
                                else
                                    startpMove = false;
                            }
                        }
                        if (startpMove)
                        {
                            if (bri.startp(Z) > pillar(pillId).endpt(Z))
                            {
                                extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                                bri.startp(X) = pillar(pillId).endpt(X);
                                bri.startp(Y) = pillar(pillId).endpt(Y);
                                bri.setCreatedFlg(true);
                            }
                            else
                            {
                                extractSupportTreeNodeChanged(meshlinktemp, historyDataDelete);
                                invalidateRelationShip(&crossbridge(crossBridgeid));

                            }
                        }
                        else
                        {
                            if (bri.endp(Z) > pillar(pillId).endpt(Z))
                            {
                                extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                                bri.endp(X) = pillar(pillId).endpt(X);
                                bri.endp(Y) = pillar(pillId).endpt(Y);
                                bri.setCreatedFlg(true);
                            }
                            else
                            {
                                extractSupportTreeNodeChanged(meshlinktemp, historyDataDelete);
                                invalidateRelationShip(&crossbridge(crossBridgeid));

                            }

                        }
                    }
                }

            }

        }


    }
    break;
    case MeshTypeElement::Anchor:
    {
        Anchor& anch = anchor(id);
        Vec3d dir;
        assert(anch.linkL.size() == 1);
        Vec3d jpt = pillar(anch.linkL[0].id).endpt;
        offset = inOffset.ptEnd;
        dir = jpt - offset;
        IndexedMesh::hit_result hit = algPtr->m_mesh.query_ray_hit(jpt, -dir.normalized());
        
        if (hit.is_hit()&&
            !hit.is_inside())
        {
            double len = 0.0;
            SupportTreeNode2MeshLink meshlinktemp;
            offset = hit.position();
            dir = jpt - offset;
            len = dir.norm() + anch.penetration_mm - 2 * anch.r_pin_mm - anch.r_back_mm ;
            meshlinktemp.type = MeshTypeElement::Anchor;
            meshlinktemp.id = anch.id;
            oldmeshlinks.emplace_back(meshlinktemp);
            if (len > 0.0)
            {
                extractSupportTreeNodeChanged(meshlinktemp, historyDataMove);
                anch.setCreatedFlg(true);
                anch.pos = offset;
                anch.dir = dir.normalized();
                anch.width_mm = len;
            }
        }

    }
    break;
    default:
    {
        std::cout << __LINE__ << " not move mesh type=" << (int)meshtype << std::endl;
        std::cout << __LINE__ << " not move mesh type ID=" << id << std::endl;
    }

    }
    switch (srceventflg)
    {
    case SUPPORT_EVENT_FLAG::EVENT_FLAG_MOVE_MOVE:
    case SUPPORT_EVENT_FLAG::EVENT_FLAG_MOVE_END:
        break;
    case SUPPORT_EVENT_FLAG::EVENT_FLAG_MOVE_START:
        historyDataEnable = true;
        break;
    default:
        std::cout << "SUPPORT_EVENT_FLAG Unknow!!!!== " << (int)srceventflg << std::endl;
        break;
    }
    if ( historyDataEnable == true)
    {
        std::vector<HistoryData> srcdataV;
        std::vector < SUPPORT_AJUST_EVENT> eventV;
        std::cout << "historyDataEnable == " << historyDataEnable << std::endl;

        srcdataV.emplace_back(historyDataMove);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_MOVE);

        srcdataV.emplace_back(historyDataAdd);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_ADD);

        srcdataV.emplace_back(historyDataDelete);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_DELETE);

        pushSupportTreeNodeChanged(srcdataV, eventV);

    }

}

bool SupportTreeBuilder::supportTreeNodeIsValid(SupportTreeNode2MeshLink linknd, SupportTreeNode** outtreeNodePtr)
{
    bool validflg = false;
    MeshTypeElement type = linknd.type;
    long id = linknd.id;
    SupportTreeNode* treeNodePtr = nullptr;
    switch (type)
    {
    case MeshTypeElement::Head:
        treeNodePtr = &head(id);
        break;
    case MeshTypeElement::Pillar:
        treeNodePtr = &pillar(id);
    break;
    case MeshTypeElement::Junction:
        treeNodePtr = &junction(id);
        break;
    case MeshTypeElement::Bridge:
        treeNodePtr = &bridge(id);
        break;
    case MeshTypeElement::CrossBridge:
        treeNodePtr = &crossbridge(id);
        break;
    case MeshTypeElement::DiffBridge:
        treeNodePtr = &diffbridge(id);
        break;
    case MeshTypeElement::Pedestal:
        treeNodePtr = &pedestal(id);
        break;
    case MeshTypeElement::Anchor:
        treeNodePtr = &anchor(id);
        break;
    case MeshTypeElement::SquarePedestal:
        treeNodePtr = &squarePedestal(id);
        break;
    default:
        std::cout << __LINE__<<" error!!! unkown type===" << (int)type << std::endl;
        std::cout << __LINE__<<" error!!! unknow type id=" << id << std::endl;
        break;
    }
    if (treeNodePtr)
    {
        validflg = treeNodePtr->is_valid();
        if(outtreeNodePtr !=nullptr)
            *outtreeNodePtr = treeNodePtr;
    }
    //if (validflg == false)
    //{
    //    std::cout << __LINE__ << " error!!!not valid unkown type===" << (int)type << std::endl;
    //    std::cout << __LINE__ << " error!!!not valid unknow type id=" << id << std::endl;
    //}
    return validflg;
}

void SupportTreeBuilder::extractSupportTreeNodeChanged(SupportTreeNode2MeshLink link, HistoryData &srcdata)
{
        switch (link.type)
        {
        case MeshTypeElement::Head:
            srcdata.m_heads.push_back(head(link.id));
            break;
        case MeshTypeElement::Pillar:
            srcdata.m_pillars.push_back(pillar(link.id));
            break;
        case MeshTypeElement::Junction:
            srcdata.m_junctions.push_back(junction(link.id));
            break;
        case MeshTypeElement::Bridge:
            srcdata.m_bridges.push_back(bridge(link.id));
            break;
        case MeshTypeElement::CrossBridge:
            srcdata.m_crossbridges.push_back(crossbridge(link.id));
            break;
        case MeshTypeElement::DiffBridge:
            srcdata.m_diffbridges.push_back(diffbridge(link.id));
            break;
        case MeshTypeElement::Pedestal:
            srcdata.m_pedestals.push_back(pedestal(link.id));
            break;
        case MeshTypeElement::Anchor:
            srcdata.m_anchors.push_back(anchor(link.id));
            break;
        case MeshTypeElement::SquarePedestal:
            srcdata.m_squarePedestals.push_back(squarePedestal(link.id));
            break;
        default:
            std::cout << "MeshTypeElement unkown type===" << (int)link.type << std::endl;
            break;
        }


}
void SupportTreeBuilder::pushSupportTreeNodeChanged(const std::vector<HistoryData>& srcdata, std::vector < SUPPORT_AJUST_EVENT> event,bool sameTimeAction)
{
    if (m_EventHistoryDatasV.size() > 20)
    {
        m_EventHistoryDatasV.erase(m_EventHistoryDatasV.begin());
    }
    assert(srcdata.size() == event.size());
    std::vector<HistoryDataPair> sameHistoryData;
    for (int srcindex = 0; srcindex < event.size(); srcindex++)
    {
        sameHistoryData.emplace_back(std::make_pair(event[srcindex], srcdata[srcindex]));
    }
    pushUndoHistoryData(sameHistoryData);
    m_RedoEventHistoryDatasV.clear();
}
void SupportTreeBuilder::pushUndoHistoryData(const std::vector<HistoryDataPair>& historyData)
{
    if (historyData.size() == 0)
        return;
    if (m_EventHistoryDatasV.size() > 20)
    {
        m_EventHistoryDatasV.erase(m_EventHistoryDatasV.begin());
    }
    m_EventHistoryDatasV.emplace_back(historyData);
}
void SupportTreeBuilder::pushRedoHistoryData(const std::vector<HistoryDataPair> &historyData)
{
    if (historyData.size() == 0)
        return;
    if (m_RedoEventHistoryDatasV.size() > 20)
    {
        m_RedoEventHistoryDatasV.erase(m_RedoEventHistoryDatasV.begin());
    }
    std::vector<HistoryDataPair> sameHistoryData;
    for (int index = 0; index < historyData.size(); index++)
    {
        HistoryData srcdata;
        SUPPORT_AJUST_EVENT event= SUPPORT_AJUST_EVENT::EVENT_NONE;
        switch (historyData[index].first)
        {
        case SUPPORT_AJUST_EVENT::EVENT_ADD:
        {
            srcdata = historyData[index].second;
            event = SUPPORT_AJUST_EVENT::EVENT_ADD;
            sameHistoryData.emplace_back(std::make_pair(event, srcdata));
        }
        break;
        case SUPPORT_AJUST_EVENT::EVENT_MOVE:
        {
            HistoryData srcdatatemp = historyData[index].second;
            for (const auto &obj : srcdatatemp.m_heads)
            {
                srcdata.m_heads.emplace_back(head(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_pillars)
            {
                srcdata.m_pillars.emplace_back(pillar(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_junctions)
            {
                srcdata.m_junctions.emplace_back(junction(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_bridges)
            {
                srcdata.m_bridges.emplace_back(bridge(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_crossbridges)
            {
                srcdata.m_crossbridges.emplace_back(crossbridge(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_diffbridges)
            {
                srcdata.m_diffbridges.emplace_back(diffbridge(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_pedestals)
            {
                srcdata.m_pedestals.emplace_back(pedestal(obj.id));
            }
            for (const auto &obj : srcdatatemp.m_anchors)
            {
                srcdata.m_anchors.emplace_back(anchor(obj.id));
            }
            event = SUPPORT_AJUST_EVENT::EVENT_MOVE;
            sameHistoryData.emplace_back(std::make_pair(event, srcdata));
        }
        break;
        case SUPPORT_AJUST_EVENT::EVENT_DELETE:
        {
            srcdata = historyData[index].second;
            event = SUPPORT_AJUST_EVENT::EVENT_DELETE;
            sameHistoryData.emplace_back(std::make_pair(event, srcdata));

        }
        break;
        }

    }
    if(sameHistoryData.size()>0)
        m_RedoEventHistoryDatasV.emplace_back(sameHistoryData);
}
void SupportTreeBuilder::buildMeshLinkRelation(SupportTreeNode& treeNodeL, SupportTreeNode& treeNodeR)
{
    SupportTreeNode2MeshLink relation;
    if (!treeNodeL.is_valid()||!treeNodeR.is_valid())
        return;
    relation.type = treeNodeR.treeNodeType;
    relation.id = treeNodeR.id;
    treeNodeL.linkR.emplace_back(relation);

    relation.type = treeNodeL.treeNodeType;
    relation.id = treeNodeL.id;
    treeNodeR.linkL.emplace_back(relation);

}
void SupportTreeBuilder::invalidateRelationShip(const SupportTreeNode* treeNdptr)
{
    bool bridgeflg = false;
    if (!treeNdptr->is_valid())
    {
        return;
    }

    switch (treeNdptr->treeNodeType)
    {
    case MeshTypeElement::Bridge:
    case MeshTypeElement::DiffBridge:
        bridgeflg = true;
        break;
    }
    std::vector<SupportTreeNode2MeshLink> sidelink;
    {
        sidelink = treeNdptr->linkL;
        for (SupportTreeNode2MeshLink link : sidelink)
        {
            if (link.id < 0)
                continue;
            switch (link.type)
            {
            case MeshTypeElement::Head:
            {
                Head& hd = head(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(hd.linkR.begin(), hd.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != hd.linkR.end())
                {
                    hd.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::Pillar:
            {
                Pillar& pillarobj = pillar(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(pillarobj.linkR.begin(), pillarobj.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != pillarobj.linkR.end())
                {
                    pillarobj.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::Junction:
            {
                Junction& j = junction(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(j.linkR.begin(), j.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != j.linkR.end())
                {
                    j.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::Bridge:
            {
                Bridge& bri = bridge(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(bri.linkR.begin(), bri.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != bri.linkR.end())
                {
                    bri.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::CrossBridge:
            {
                CrossBridge& bri = crossbridge(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(bri.linkR.begin(), bri.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != bri.linkR.end())
                {
                    bri.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::DiffBridge:
            {
                DiffBridge& bri = diffbridge(link.id);
                /////////////////////////////////////////////////////
                auto itemR = std::find_if(bri.linkR.begin(), bri.linkR.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemR != bri.linkR.end())
                {
                    bri.linkR.erase(itemR);
                }

            }
            break;
            case MeshTypeElement::Pedestal:
            {
                Pedestal& pedestalobj = pedestal(link.id);
                pedestalobj.linkR.clear();
            }
            break;
            case MeshTypeElement::Anchor:
            {
                Anchor& achr = anchor(link.id);
                achr.linkR.clear();
            }
            break;
            case MeshTypeElement::SquarePedestal:
            {
                SquarePedestal& obj = squarePedestal(link.id);
                obj.linkR.clear();
            }
            break;

            }
        }
    }
    ////Right side 
   {
         sidelink = treeNdptr->linkR;
        for (SupportTreeNode2MeshLink link : sidelink)
        {
            if (link.id < 0)
                continue;
            switch (link.type)
            {
            case MeshTypeElement::Head:
            {
                Head& hd = head(link.id);
                auto itemL = std::find_if(hd.linkL.begin(), hd.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != hd.linkL.end())
                {
                    hd.linkL.erase(itemL);
                }

            }
            break;
            case MeshTypeElement::Pillar:
            {
                Pillar& pillarobj = pillar(link.id);
                auto itemL = std::find_if(pillarobj.linkL.begin(), pillarobj.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != pillarobj.linkL.end())
                {
                    pillarobj.linkL.erase(itemL);
                    if (bridgeflg && pillarobj.bridges > 0)
                    {
                        pillarobj.bridges -= 1;
                    }
                }
            }
            break;
            case MeshTypeElement::Junction:
            {
                Junction& j = junction(link.id);
                auto itemL = std::find_if(j.linkL.begin(), j.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != j.linkL.end())
                {
                    j.linkL.erase(itemL);
                }
            }
            break;
            case MeshTypeElement::Bridge:
            {
                Bridge& bri = bridge(link.id);
                auto itemL = std::find_if(bri.linkL.begin(), bri.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != bri.linkL.end())
                {
                    bri.linkL.erase(itemL);
                }

            }
            break;
            case MeshTypeElement::CrossBridge:
            {
                CrossBridge& bri = crossbridge(link.id);
                auto itemL = std::find_if(bri.linkL.begin(), bri.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != bri.linkL.end())
                {
                    bri.linkL.erase(itemL);
                }

            }
            break;
            case MeshTypeElement::DiffBridge:
            {
                DiffBridge& bri = diffbridge(link.id);
                auto itemL = std::find_if(bri.linkL.begin(), bri.linkL.end(),
                    [treeNdptr](SupportTreeNode2MeshLink i)->bool {
                        return (treeNdptr->treeNodeType == i.type) && (treeNdptr->id == i.id);
                    });
                if (itemL != bri.linkL.end())
                {
                    bri.linkL.erase(itemL);
                }
            }
            break;
            case MeshTypeElement::Pedestal:
            {
                Pedestal& pedestalobj = pedestal(link.id);
                pedestalobj.linkL.clear();
            }
            break;
            case MeshTypeElement::Anchor:
            {
                Anchor& achr = anchor(link.id);
                achr.linkL.clear();
            }
            break;
            case MeshTypeElement::SquarePedestal:
            {
                SquarePedestal& obj = squarePedestal(link.id);
                obj.linkL.clear();
            }
            break;

            }
        }
    }
}
void SupportTreeBuilder::invalidateLinkRelationShip(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inmeshlinks)
{

    for (SupportTreeNode2MeshLink linkout : inmeshlinks)
    {
        long id = linkout.id;
        if (id < 0)
        {
            continue;
        }
        switch (linkout.type)
        {
        case MeshTypeElement::Head:
        {
            Head& hd = head(id);
            invalidateRelationShip(&hd);
        }
        break;
        case MeshTypeElement::Pillar:
        {
            Pillar& pillarobj = pillar(id);
            invalidateRelationShip(&pillarobj);
        }
        break;
        case MeshTypeElement::Junction:
        {

            Junction& j = junction(id);
            invalidateRelationShip(&j);

        }
        break;
        case MeshTypeElement::Bridge:
        {

            Bridge& bri = bridge(id);
            invalidateRelationShip(&bri);

        }
        break;
        case MeshTypeElement::CrossBridge:
        {
            CrossBridge& bri = crossbridge(id);
            invalidateRelationShip(&bri);

        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            DiffBridge& bri = diffbridge(id);
            invalidateRelationShip(&bri);

        }
        break;
        case MeshTypeElement::Pedestal:
        {
            Pedestal &pedestalobj=pedestal(id);
            invalidateRelationShip(&pedestalobj);
        }
        break;
        case MeshTypeElement::Anchor:
        {
            Anchor &anchorobj=anchor(id);
            invalidateRelationShip(&anchorobj);
        }
        break;
        case MeshTypeElement::SquarePedestal:
        {
            SquarePedestal&anchorobj= squarePedestal(id);
            invalidateRelationShip(&anchorobj);
        }
        break;
        }

    }
}
void SupportTreeBuilder::reconstructRelationShip(SupportTreeNode& treenodesrc)
{
    std::vector<SupportTreeNode2MeshLink> sidelink ;
    SupportTreeNode2MeshLink srclink;
    srclink.type = treenodesrc.treeNodeType;
    srclink.id = treenodesrc.id;
    auto findFun= [treenodesrc](SupportTreeNode2MeshLink i)->bool {
        return (treenodesrc.treeNodeType == i.type) && (treenodesrc.id == i.id)&& (i.id>SupportTreeNode::ID_UNSET);
    };
    sidelink = treenodesrc.linkL;
    for (SupportTreeNode2MeshLink link : sidelink)
    {
        SupportTreeNode* nearbytreenode = nullptr;
        bool reconstructFlg = false;
        if (link.id < 0)
            continue;
        switch (link.type)
        {
        case MeshTypeElement::Head:
        {
            nearbytreenode =& head(link.id);
            /////////////////////////////////////////////////////
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::Pillar:
        {
            nearbytreenode =&pillar(link.id);
            /////////////////////////////////////////////////////
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::Junction:
        {
            nearbytreenode = &junction(link.id);
            /////////////////////////////////////////////////////
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::Bridge:
        {
            nearbytreenode = &bridge(link.id);
            /////////////////////////////////////////////////////
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::CrossBridge:
        {
            nearbytreenode = &crossbridge(link.id);
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            nearbytreenode = &diffbridge(link.id);
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::Pedestal:
        {
            nearbytreenode = &pedestal(link.id);
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::Anchor:
        {
            nearbytreenode = &anchor(link.id);
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::SquarePedestal:
        {
            nearbytreenode = &squarePedestal(link.id);
            auto itemR = std::find_if(nearbytreenode->linkR.begin(), nearbytreenode->linkR.end(), findFun);
            if (itemR == nearbytreenode->linkR.end())
            {
                reconstructFlg = true;
            }
        }
        break;

        }
        if ((reconstructFlg == true) && (nearbytreenode != nullptr))
        {
            if(nearbytreenode->is_valid())
                nearbytreenode->linkR.emplace_back(srclink);
        }
    }
    ////
    sidelink = treenodesrc.linkR;
    for (SupportTreeNode2MeshLink link : sidelink)
    {
        SupportTreeNode* nearbytreenode = nullptr;
        bool reconstructFlg = false;
        if (link.id < 0)
            continue;
        switch (link.type)
        {
        case MeshTypeElement::Pillar:
        {
            nearbytreenode = &pillar(link.id);
            /////////////////////////////////////////////////////
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::Junction:
        {
            nearbytreenode = &junction(link.id);
            /////////////////////////////////////////////////////
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::Bridge:
        {
            nearbytreenode = &bridge(link.id);
            /////////////////////////////////////////////////////
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::CrossBridge:
        {
            nearbytreenode = &crossbridge(link.id);
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            nearbytreenode = &diffbridge(link.id);
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }

        }
        break;
        case MeshTypeElement::Pedestal:
        {
            nearbytreenode = &pedestal(link.id);
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::Anchor:
        {
            nearbytreenode = &anchor(link.id);
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }
        }
        break;
        case MeshTypeElement::SquarePedestal:
        {
            nearbytreenode = &squarePedestal(link.id);
            auto itemL = std::find_if(nearbytreenode->linkL.begin(), nearbytreenode->linkL.end(), findFun);
            if (itemL == nearbytreenode->linkL.end())
            {
                reconstructFlg = true;
            }
        }
        break;

        }
        if ((reconstructFlg == true)&&(nearbytreenode !=nullptr))
        {
            if (nearbytreenode->is_valid())
                nearbytreenode->linkL.emplace_back(srclink);
        }
    }
}
void SupportTreeBuilder::updateSupportParams(const SupportTreeConfig &cfg)
{
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    if (algPtr)
    {
        algPtr->m_cfg = cfg;
    }
}
void SupportTreeBuilder::redoConstructMeshLink(const HistoryData& srcdata, SUPPORT_AJUST_EVENT event, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
    switch (event)
    {
    case SUPPORT_AJUST_EVENT::EVENT_ADD:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        recoverDeletedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    case SUPPORT_AJUST_EVENT::EVENT_MOVE:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        recoverMovedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    case SUPPORT_AJUST_EVENT::EVENT_DELETE:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        DeletedAddedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    }
}

void SupportTreeBuilder::undoConstructMeshLink(const HistoryData& srcdata, SUPPORT_AJUST_EVENT event, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
    switch (event)
    {
    case SUPPORT_AJUST_EVENT::EVENT_DELETE:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        recoverDeletedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    case SUPPORT_AJUST_EVENT::EVENT_MOVE:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        recoverMovedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    case SUPPORT_AJUST_EVENT::EVENT_ADD:
    {
        std::vector< Slic3r::sla::SupportTreeNode2MeshLink> outmeshlinkstemp;
        DeletedAddedMeshLinks(srcdata, outmeshlinkstemp);
        outmeshlinks.insert(outmeshlinks.end(), outmeshlinkstemp.begin(), outmeshlinkstemp.end());
    }
    break;
    }
}
void SupportTreeBuilder::redoConstructMeshLinkRelationShip(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& meshlinks)
{
    for (const SupportTreeNode2MeshLink& link : meshlinks)
    {
        if (link.id < 0)
            continue;
        switch (link.type)
        {
        case MeshTypeElement::Head:
        {
            reconstructRelationShip(head(link.id));
        }
        break;
        case MeshTypeElement::Pillar:
        {
            reconstructRelationShip(pillar(link.id));
        }
        break;
        case MeshTypeElement::Junction:
        {
            reconstructRelationShip(junction(link.id));
        }
        break;
        case MeshTypeElement::Bridge:
        {
            reconstructRelationShip(bridge(link.id));
        }
        break;
        case MeshTypeElement::CrossBridge:
        {
            reconstructRelationShip(crossbridge(link.id));
        }
        break;
        case MeshTypeElement::DiffBridge:
        {
            reconstructRelationShip(diffbridge(link.id));
        }
        break;
        case MeshTypeElement::Pedestal:
        {
            reconstructRelationShip(pedestal(link.id));
        }
        break;
        case MeshTypeElement::Anchor:
        {
            reconstructRelationShip(anchor(link.id));
        }
        break;
        case MeshTypeElement::SquarePedestal:
        {
            reconstructRelationShip(squarePedestal(link.id));
        }
        break;

        }
    }
}
void SupportTreeBuilder::recoverDeletedMeshLinks(const HistoryData& srcdata,std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
      for (auto&  obj: srcdata.m_heads)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Head;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        head(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_pillars)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pillar;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        pillar(obj.id).reConstructe(obj);
        if (m_supportTreeBuildstepsPtr)
        {
            m_supportTreeBuildstepsPtr->m_pillar_index.guarded_insert(pillar(obj.id).endpoint(), pillar(obj.id).id);
            m_supportTreeBuildstepsPtr->m_pillar_index_start.guarded_insert(pillar(obj.id).startpoint(), pillar(obj.id).id);
        }
      }
      for (auto& obj : srcdata.m_junctions)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Junction;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        junction(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_bridges)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Bridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        bridge(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_crossbridges)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::CrossBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        crossbridge(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_diffbridges)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::DiffBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        diffbridge(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_pedestals)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        pedestal(obj.id).reConstructe(obj);
        m_meshPadCache_valid = false;

      }
      for (auto& obj : srcdata.m_anchors)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Anchor;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        anchor(obj.id).reConstructe(obj);
      }
      for (auto& obj : srcdata.m_squarePedestals)
      {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::SquarePedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        squarePedestal(obj.id).reConstructe(obj);
      }
}
void SupportTreeBuilder::DeletedAddedMeshLinks(const HistoryData& srcdata, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
    for (Head obj : srcdata.m_heads)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Head;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&head(obj.id));
        head(obj.id).invalidate();
    }
    for (Pillar obj : srcdata.m_pillars)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pillar;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&pillar(obj.id));
        pillar(obj.id).invalidate();
        if (m_supportTreeBuildstepsPtr)
        {
            m_supportTreeBuildstepsPtr->m_pillar_index.guarded_remove(pillar(obj.id).endpoint(), pillar(obj.id).id);
            m_supportTreeBuildstepsPtr->m_pillar_index_start.guarded_remove(pillar(obj.id).startpoint(), pillar(obj.id).id);
        }
    }
    for (Junction obj : srcdata.m_junctions)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Junction;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&junction(obj.id));
        junction(obj.id).invalidate();
    }
    for (Bridge obj : srcdata.m_bridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Bridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&bridge(obj.id));
        bridge(obj.id).invalidate();
    }
    for (CrossBridge obj : srcdata.m_crossbridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::CrossBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&crossbridge(obj.id));
        crossbridge(obj.id).invalidate();
    }
    for (DiffBridge obj : srcdata.m_diffbridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::DiffBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&diffbridge(obj.id));
        diffbridge(obj.id).invalidate();
    }
    for (Pedestal obj : srcdata.m_pedestals)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&pedestal(obj.id));
        pedestal(obj.id).invalidate();
        m_meshPadCache_valid = false;

    }
    for (Anchor obj : srcdata.m_anchors)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Anchor;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&anchor(obj.id));
        anchor(obj.id).invalidate();
    }
    for (SquarePedestal obj : srcdata.m_squarePedestals)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::SquarePedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        invalidateRelationShip(&squarePedestal(obj.id));
        squarePedestal(obj.id).invalidate();
    }

}

void SupportTreeBuilder::recoverMovedMeshLinks(const HistoryData& srcdata, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks)
{
    for (Head obj : srcdata.m_heads)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Head;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        head(obj.id) = obj;
        head(obj.id).setCreatedFlg(true);
    }
    for (Pillar obj : srcdata.m_pillars)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pillar;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        pillar(obj.id) = obj;
        pillar(obj.id).setCreatedFlg(true);
    }
    for (Junction obj : srcdata.m_junctions)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Junction;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        junction(obj.id) = obj;
        junction(obj.id).setCreatedFlg(true);
    }
    for (Bridge obj : srcdata.m_bridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Bridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        bridge(obj.id) = obj;
        bridge(obj.id).setCreatedFlg(true);
    }
    for (CrossBridge obj : srcdata.m_crossbridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::CrossBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        crossbridge(obj.id) = obj;
        crossbridge(obj.id).setCreatedFlg(true);
    }
    for (DiffBridge obj : srcdata.m_diffbridges)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::DiffBridge;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        diffbridge(obj.id) = obj;
        diffbridge(obj.id).setCreatedFlg(true);
    }
    for (Pedestal obj : srcdata.m_pedestals)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Pedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        pedestal(obj.id) = obj;
        pedestal(obj.id).setCreatedFlg(true);
        m_meshPadCache_valid = false;

    }
    for (Anchor obj : srcdata.m_anchors)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::Anchor;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        anchor(obj.id) = obj;
        anchor(obj.id).setCreatedFlg(true);
    }
    for (SquarePedestal obj : srcdata.m_squarePedestals)
    {
        SupportTreeNode2MeshLink linktemp;
        linktemp.type = MeshTypeElement::SquarePedestal;
        linktemp.id = obj.id;
        outmeshlinks.emplace_back(linktemp);
        squarePedestal(obj.id) = obj;
        squarePedestal(obj.id).setCreatedFlg(true);
    }

}

bool SupportTreeBuilder::reconstructTreeNode(SupportTreeNode2MeshLink meshlinkSrc, Vec3d offset, std::vector < SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_EVENT_FLAG srceventflg)
{
    HistoryData historyData_Add;
    HistoryData historyData_Delete;
    HistoryData historyData_Move;
    MeshTypeElement meshtype = meshlinkSrc.type;
    long id = meshlinkSrc.id;
    bool ret = false;
    SupportTreeNode2MeshLink meshlinktemp;
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    if (algPtr == NULL)
    {
        return false;
    }

    switch (meshtype)
    {
    case MeshTypeElement::Pedestal:
    {
        Pedestal& pedestal = SupportTreeBuilder::pedestal(id);
        if (pedestal.linkL.size() != 1)
            return false;
        Pillar pillar = SupportTreeBuilder::pillar(pedestal.linkL[0].id);
        if (!pillar.is_valid())
            return false;
        SupportTreeNode2MeshLink topJuncMeshLink;
        for (SupportTreeNode2MeshLink& nd : pillar.linkL)
        {
            switch (nd.type)
            {
            case MeshTypeElement::Junction:
            {
                    topJuncMeshLink = nd;
            }
            break;
            }
        }

        if (junction(topJuncMeshLink.id).is_valid())
        {
            Vec3d pts = pillar.startpoint();
            IndexedMesh::hit_result hit;
            pts(X) += offset(X);
            pts(Y) += offset(Y);
            hit = algPtr->bridge_mesh_intersect(pts, DOWN, pillar.r);
            if (hit.is_hit() && !hit.is_inside())
            {
                std::vector<SupportTreeNode2MeshLink> newTreeNodelink;
                if (algPtr->connect_to_model_body(pts, pillar.r, newTreeNodelink))
                {
                    auto pillarlinkL = pillar.linkL;
                    auto pillarlinkR = pillar.linkR;

                    meshlinktemp.type = pillar.treeNodeType;
                    meshlinktemp.id = pillar.id;
                    extractSupportTreeNodeChanged(meshlinktemp, historyData_Delete);
                    outmeshlinks.emplace_back(meshlinktemp);
                    SupportTreeBuilder::pillar(pillar.id).invalidate();
                    algPtr->m_pillar_index.guarded_remove(pillar.endpoint(), pillar.id);
                    algPtr->m_pillar_index_start.guarded_remove(pillar.startpoint(), pillar.id);

                    for (const SupportTreeNode2MeshLink& nd : newTreeNodelink)
                    {
                        meshlinktemp = nd;
                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Add);
                        outmeshlinks.emplace_back(meshlinktemp);
                        if (nd.type == MeshTypeElement::Pillar)
                        {
                            std::vector<SupportTreeNode2MeshLink> newCrossTreeNodelink;
                            searchNearObjInterconnect(SupportTreeBuilder::pillar(nd.id), newCrossTreeNodelink);
                            for (const SupportTreeNode2MeshLink& crosslk : newCrossTreeNodelink)
                            {
                                meshlinktemp = crosslk;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Add);
                                outmeshlinks.emplace_back(meshlinktemp);
                            }
                        }
                    }
                    for (const SupportTreeNode2MeshLink& nd : pillarlinkL)
                    {
                        switch (nd.type)
                        {
                        case MeshTypeElement::Bridge:
                        {
                            if (bridge(nd.id).is_valid())
                            {
                                std::vector<SupportTreeNode2MeshLink> nodelinkLefttemp;
                                searchLeftTreeElement(m_bridges[nd.id], nodelinkLefttemp);
                                for (SupportTreeNode2MeshLink lkleft : nodelinkLefttemp)
                                {
                                    extractSupportTreeNodeChanged(lkleft, historyData_Delete);
                                }
                                invalidateMeshLinks(nodelinkLefttemp);
                                outmeshlinks.insert(outmeshlinks.end(), nodelinkLefttemp.begin(), nodelinkLefttemp.end());
                            }

                        }
                        break;
                        case MeshTypeElement::CrossBridge:
                        {
                            if (crossbridge(nd.id).is_valid())
                            {
                                extractSupportTreeNodeChanged(nd, historyData_Delete);
                                m_crossbridges[nd.id].invalidate();
                                outmeshlinks.emplace_back(nd);
                            }
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            if (diffbridge(nd.id).is_valid())
                            {
                                std::vector<SupportTreeNode2MeshLink> nodelinkLefttemp;
                                searchLeftTreeElement(m_diffbridges[nd.id], nodelinkLefttemp);
                                for (SupportTreeNode2MeshLink lkleft : nodelinkLefttemp)
                                {
                                    extractSupportTreeNodeChanged(lkleft, historyData_Delete);
                                }
                                invalidateMeshLinks(nodelinkLefttemp);
                                outmeshlinks.insert(outmeshlinks.end(), nodelinkLefttemp.begin(), nodelinkLefttemp.end());
                            }

                        }
                        break;
                        case MeshTypeElement::Junction:
                        {
                            Junction& junc = junction(nd.id);
                            if (junc.is_valid())
                            {
                                SupportTreeNode2MeshLink meshlinktemp;
                                meshlinktemp.type = MeshTypeElement::Junction;
                                meshlinktemp.id = junc.id;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                junc.pos(X) += offset(X);
                                junc.pos(Y) += offset(Y);
                                junc.setCreatedFlg(true);
                                outmeshlinks.emplace_back(meshlinktemp);
                                {
                                    junc.linkR.clear();
                                    SupportTreeNode* newtreeNodePtr = nullptr;
                                    if (supportTreeNodeIsValid(newTreeNodelink[0], &newtreeNodePtr))
                                    {
                                        buildMeshLinkRelation(junc, *newtreeNodePtr);
                                    }

                                }

                                for (const SupportTreeNode2MeshLink& lk : junc.linkL)
                                {
                                    switch (lk.type)
                                    {
                                    case MeshTypeElement::Head:
                                    {
                                        Vec3d jpt;
                                        Vec3d dir;
                                        Head& hd = head(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        meshlinktemp.type = MeshTypeElement::Head;
                                        meshlinktemp.id = hd.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                        jpt = junc.pos;
                                        dir = jpt - hd.pos;
                                        hd.width_mm = dir.norm() + hd.penetration_mm - hd.r_pin_mm - hd.r_back_mm;
                                        hd.dir = dir.normalized();
                                        hd.setCreatedFlg(true);
                                    }
                                    break;
                                    case MeshTypeElement::Bridge:
                                    {
                                        Bridge& bri = bridge(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        bool startpMove = true;
                                        meshlinktemp.type = MeshTypeElement::Bridge;
                                        meshlinktemp.id = bri.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                        if (bri.briStartplink.size() == 0)
                                            startpMove = false;
                                        else
                                        {
                                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                            {
                                                if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                                    startpMove = true;
                                                else
                                                    startpMove = false;
                                            }
                                        }
                                        if (startpMove)
                                        {
                                            bri.startp = junc.pos;
                                        }
                                        else
                                        {
                                            bri.endp = junc.pos;
                                        }
                                        bri.setCreatedFlg(true);
                                    }
                                    break;
                                    case MeshTypeElement::DiffBridge:
                                    {
                                        DiffBridge& bri = diffbridge(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        bool startpMove = true;
                                        meshlinktemp.type = MeshTypeElement::DiffBridge;
                                        meshlinktemp.id = bri.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);

                                        if (bri.briStartplink.size() == 0)
                                            startpMove = false;
                                        else
                                        {
                                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                            {
                                                if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                                    startpMove = true;
                                                else
                                                    startpMove = false;
                                            }
                                        }
                                        if (startpMove)
                                        {
                                            bri.startp = junc.pos;
                                        }
                                        else
                                        {
                                            bri.endp = junc.pos;
                                        }
                                        bri.setCreatedFlg(true);
                                    }
                                    break;
                                    default:
                                        std::cout << "should move obj type==" << (int)junc.linkL[0].type << std::endl;
                                    }
                                }
                            }
                        }
                        break;
                        default:
                        {
                            std::cout << __LINE__ << "not support  tyep==" << (int)nd.type << std::endl;
                        }
                        break;
                        }
                    }
                    for (const SupportTreeNode2MeshLink& nd : pillarlinkR)
                    {
                        switch (nd.type)
                        {
                        case MeshTypeElement::Pedestal:
                        {
                            if (SupportTreeBuilder::pedestal(nd.id).is_valid())
                            {
                                auto pedlinkR = SupportTreeBuilder::pedestal(nd.id).linkR;
                                meshlinktemp.type = nd.type;
                                meshlinktemp.id = nd.id;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Delete);
                                outmeshlinks.emplace_back(meshlinktemp);
                                invalidateMeshLink(meshlinktemp);

                                {
                                    for (const SupportTreeNode2MeshLink& pednd : pedlinkR)
                                    {
                                        switch (pednd.type)
                                        {
                                        case MeshTypeElement::SquarePedestal:
                                        {
                                            if (SupportTreeBuilder::squarePedestal(pednd.id).is_valid())
                                            {
                                                meshlinktemp.type = pednd.type;
                                                meshlinktemp.id = pednd.id;
                                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Delete);
                                                outmeshlinks.emplace_back(meshlinktemp);
                                                invalidateMeshLink(meshlinktemp);
                                            }
                                        }
                                        }
                                    }
                                }
                            }


                        }
                        break;
                        default:
                        {
                            std::cout << __LINE__ << "not support  tyep==" << (int)nd.type << std::endl;
                        }
                        break;
                        }
                    }
                    ///////////////////////////
                    ret = true;
                }
            }

        }
    }
    break;
    case MeshTypeElement::Anchor:
    {
        Anchor& ahr = anchor(id);
        if (ahr.linkL.size() != 1)
            return false;
        Pillar pillar = SupportTreeBuilder::pillar(ahr.linkL[0].id);
        SupportTreeNode2MeshLink topJuncMeshLink;
        for (SupportTreeNode2MeshLink& nd : pillar.linkL)
        {
            switch (nd.type)
            {
            case MeshTypeElement::Junction:
            {
                    topJuncMeshLink = nd;
            }
            break;
            }
        }
        if (junction(topJuncMeshLink.id).is_valid())
        {
            Vec3d pts = pillar.startpoint();
            pts(X) += offset(X);
            pts(Y) += offset(Y);

            auto hit = algPtr->bridge_mesh_intersect(pts, DOWN, pillar.r);
            if (std::isinf(hit.distance()))
            {

                std::vector<SupportTreeNode2MeshLink> newTreeNodelink;
                if (algPtr->create_direct_ground_pillar(pts, DOWN, pillar.r, newTreeNodelink))
                {
                    auto pillarlinkL = pillar.linkL;
                    auto pillarlinkR = pillar.linkR;
                    meshlinktemp.type = pillar.treeNodeType;
                    meshlinktemp.id = pillar.id;
                    extractSupportTreeNodeChanged(meshlinktemp, historyData_Delete);
                    outmeshlinks.emplace_back(meshlinktemp);
                    SupportTreeBuilder::pillar(pillar.id).invalidate();
                    algPtr->m_pillar_index.guarded_remove(pillar.endpoint(), pillar.id);
                    algPtr->m_pillar_index_start.guarded_remove(pillar.startpoint(), pillar.id);

                    for (const SupportTreeNode2MeshLink& nd : pillarlinkL)
                    {
                        switch (nd.type)
                        {
                        case MeshTypeElement::Bridge:
                        {
                            if (bridge(nd.id).is_valid())
                            {
                                std::vector<SupportTreeNode2MeshLink> nodelinkLefttemp;
                                searchLeftTreeElement(m_bridges[nd.id], nodelinkLefttemp);
                                for (SupportTreeNode2MeshLink lkleft : nodelinkLefttemp)
                                {
                                    extractSupportTreeNodeChanged(lkleft, historyData_Delete);
                                }
                                invalidateMeshLinks(nodelinkLefttemp);
                                outmeshlinks.insert(outmeshlinks.end(), nodelinkLefttemp.begin(), nodelinkLefttemp.end());
                            }

                        }
                        break;
                        case MeshTypeElement::CrossBridge:
                        {
                            if (crossbridge(nd.id).is_valid())
                            {
                                extractSupportTreeNodeChanged(nd, historyData_Delete);
                                m_crossbridges[nd.id].invalidate();
                                outmeshlinks.emplace_back(nd);
                            }
                        }
                        break;
                        case MeshTypeElement::DiffBridge:
                        {
                            if (diffbridge(nd.id).is_valid())
                            {
                                std::vector<SupportTreeNode2MeshLink> nodelinkLefttemp;
                                searchLeftTreeElement(m_diffbridges[nd.id], nodelinkLefttemp);
                                for (SupportTreeNode2MeshLink lkleft : nodelinkLefttemp)
                                {
                                    extractSupportTreeNodeChanged(lkleft, historyData_Delete);
                                }
                                invalidateMeshLinks(nodelinkLefttemp);
                                outmeshlinks.insert(outmeshlinks.end(), nodelinkLefttemp.begin(), nodelinkLefttemp.end());
                            }

                        }
                        break;
                        case MeshTypeElement::Junction:
                        {
                            Junction& junc = junction(nd.id);
                            if (junc.is_valid())
                            {
                                SupportTreeNode2MeshLink meshlinktemp;
                                meshlinktemp.type = MeshTypeElement::Junction;
                                meshlinktemp.id = junc.id;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                junc.pos(X) += offset(X);
                                junc.pos(Y) += offset(Y);
                                junc.setCreatedFlg(true);
                                outmeshlinks.emplace_back(meshlinktemp);
                                {
                                    junc.linkR.clear();
                                    SupportTreeNode* newtreeNodePtr = nullptr;
                                    if (supportTreeNodeIsValid(newTreeNodelink[0], &newtreeNodePtr))
                                    {
                                        buildMeshLinkRelation(junc, *newtreeNodePtr);
                                    }

                                }

                                for (const SupportTreeNode2MeshLink& lk : junc.linkL)
                                {
                                    switch (lk.type)
                                    {
                                    case MeshTypeElement::Head:
                                    {
                                        Vec3d jpt;
                                        Vec3d dir;
                                        Head& hd = head(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        meshlinktemp.type = MeshTypeElement::Head;
                                        meshlinktemp.id = hd.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                        jpt = junc.pos;
                                        dir = jpt - hd.pos;
                                        hd.width_mm = dir.norm() + hd.penetration_mm - hd.r_pin_mm - hd.r_back_mm;
                                        hd.dir = dir.normalized();
                                        hd.setCreatedFlg(true);
                                    }
                                    break;
                                    case MeshTypeElement::Bridge:
                                    {
                                        Bridge& bri = bridge(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        bool startpMove = true;
                                        meshlinktemp.type = MeshTypeElement::Bridge;
                                        meshlinktemp.id = bri.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);
                                        if (bri.briStartplink.size() == 0)
                                            startpMove = false;
                                        else
                                        {
                                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                            {
                                                if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                                    startpMove = true;
                                                else
                                                    startpMove = false;
                                            }
                                        }
                                        if (startpMove)
                                        {
                                            bri.startp = junc.pos;
                                        }
                                        else
                                        {
                                            bri.endp = junc.pos;
                                        }
                                        bri.setCreatedFlg(true);
                                    }
                                    break;
                                    case MeshTypeElement::DiffBridge:
                                    {
                                        DiffBridge& bri = diffbridge(lk.id);
                                        SupportTreeNode2MeshLink meshlinktemp;
                                        bool startpMove = true;
                                        meshlinktemp.type = MeshTypeElement::DiffBridge;
                                        meshlinktemp.id = bri.id;
                                        outmeshlinks.emplace_back(meshlinktemp);
                                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Move);

                                        if (bri.briStartplink.size() == 0)
                                            startpMove = false;
                                        else
                                        {
                                            if (supportTreeNodeIsValid(bri.briStartplink[0]))
                                            {
                                                if (bri.briStartplink[0].type == junc.treeNodeType && bri.briStartplink[0].id == junc.id)
                                                    startpMove = true;
                                                else
                                                    startpMove = false;
                                            }
                                        }
                                        if (startpMove)
                                        {
                                            bri.startp = junc.pos;
                                        }
                                        else
                                        {
                                            bri.endp = junc.pos;
                                        }
                                        bri.setCreatedFlg(true);
                                    }
                                    break;
                                    default:
                                        std::cout << "should move obj type==" << (int)junc.linkL[0].type << std::endl;
                                    }
                                }
                            }
                        }
                        break;
                        default:
                        {
                            std::cout << __LINE__ << "not support  tyep==" << (int)nd.type << std::endl;
                        }
                        break;
                        }
                    }
                    for (const SupportTreeNode2MeshLink& nd : pillarlinkR)
                    {
                        switch (nd.type)
                        {
                        case MeshTypeElement::Anchor:
                        {
                            if (SupportTreeBuilder::anchor(nd.id).is_valid())
                            {
                                meshlinktemp.type = nd.type;
                                meshlinktemp.id = nd.id;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Delete);
                                outmeshlinks.emplace_back(meshlinktemp);
                                invalidateMeshLink(meshlinktemp);
                            }


                        }
                        break;
                        default:
                        {
                            std::cout << __LINE__ << "not support  tyep==" << (int)nd.type << std::endl;
                        }
                        break;
                        }
                    }

                    for (const SupportTreeNode2MeshLink& nd : newTreeNodelink)
                    {
                        meshlinktemp = nd;
                        extractSupportTreeNodeChanged(meshlinktemp, historyData_Add);
                        outmeshlinks.emplace_back(meshlinktemp);
                        if (nd.type == MeshTypeElement::Pillar)
                        {
                            std::vector<SupportTreeNode2MeshLink> newCrossTreeNodelink;
                            searchNearObjInterconnect(SupportTreeBuilder::pillar(nd.id), newCrossTreeNodelink);
                            for (const SupportTreeNode2MeshLink& crosslk : newCrossTreeNodelink)
                            {
                                meshlinktemp = crosslk;
                                extractSupportTreeNodeChanged(meshlinktemp, historyData_Add);
                                outmeshlinks.emplace_back(meshlinktemp);
                            }
                        }
                    }

                    ///////////////////////////
                    ret = true;
                }
            }

        }
        break;
    }
    }
    if (ret == true)
    {
        std::vector<HistoryData> srcdataV;
        std::vector < SUPPORT_AJUST_EVENT> eventV;

        srcdataV.emplace_back(historyData_Delete);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_DELETE);

        srcdataV.emplace_back(historyData_Move);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_MOVE);

        srcdataV.emplace_back(historyData_Add);
        eventV.emplace_back(SUPPORT_AJUST_EVENT::EVENT_ADD);

        pushSupportTreeNodeChanged(srcdataV, eventV);

    }
    return ret;
}

void SupportTreeBuilder::pushSupportTreeHistoryData(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inMeshlinks, SUPPORT_AJUST_EVENT event)
{
     HistoryData srcdata;
     std::vector<HistoryData> srcdataV;
     std::vector < SUPPORT_AJUST_EVENT> eventV;

     for (const Slic3r::sla::SupportTreeNode2MeshLink &inlk : inMeshlinks)
     {
         extractSupportTreeNodeChanged(inlk, srcdata);
     }
     if (inMeshlinks.size())
     {
         srcdataV.emplace_back(srcdata);
         eventV.emplace_back(event);
         pushSupportTreeNodeChanged(srcdataV, eventV);
     }
}

/**
    获取支撑的各项数据
    index:      int&,    代表对应的支撑数据,首次调用必需为0;
    filename:   std::string&, 获取到的支撑数据对应的名称;
    data:       char*,    支撑数据, 最大不能超过2*1024*1024 - 1;
    length:     int&,     支撑数据的长度  
*/
int SupportTreeBuilder::writeSupportTreeData(int& index, std::string& filename, char* data, int& length)
{
    std::string pad;
    char* src = 0;
    int size = 0;
    length = 0;
    filename = std::string("error_happened");

    rapidjson::StringBuffer buf;
    PrettyWriter<rapidjson::StringBuffer> writer(buf);

    writer.StartObject();
    switch (index++)
    {
    case 0:
    { 
        filename = std::string("0Heads");
        writeHeadsData(writer, this);
        break;
    }
    case 1:
    {
        filename = std::string("1Pillars");
        writePillarsData(writer, this);
        break;
    }
    case 2:
    {
        filename = std::string("2Junctions");
        writeJunctionsData(writer, this);
        break;
    }
    case 3:
    {
        filename = std::string("3Bridges");
        writeBridgesData(writer, this);
        break;
    }
    case 4:
    {
        filename = std::string("4CrossBridges");
        writeCrossBridgesData(writer, this);
        break;
    }
    case 5:
    {
        filename = std::string("5DiffBridges");
        writeDiffBridgesData(writer, this);
        break;
    }
    case 6:
    {
        filename = std::string("6Pedestals");
        writePedestalsData(writer, this);
        break;
    }
    case 7:
    {
        filename = std::string("7Anchors");
        writeAnchorsData(writer, this);
        break;
    }
    case 8:
    {
#if 0
        filename = std::string("8Pad");
        writePadData(pad, this);
        src = (char*)pad.c_str();
        size = pad.size();
        index = -1;//结束
        break;
#else
        filename = std::string("8Pad");
        writePadData(writer, this);
        //index = -1;//结束
        break;

#endif
    }
    case 9:
    {
        filename = std::string("9SquarePedestals");
        writeSquarePedestalsData(writer, this);
    }
    break;

    default:
        index = -1;//结束
        return index;
    }
    writer.EndObject();

    //if (0 == size)
    {
        src = (char*)buf.GetString();
        size = buf.GetSize();
    }
    memcpy(data, src, size);
    data[size] = 0;
    size += 1;
    length = size;

    return index;
}

void SupportTreeBuilder::readSupportTreeData(std::string &filename, const char* data, int& length)
{
#if 0
    std::string file_name = filepath + std::string("/Heads.json");
    readHeadsData(file_name,this);

    file_name = filepath + std::string("/Pillars.json");
    readPillarsData(file_name,this);

    file_name = filepath + std::string("/Junctions.json");
    readJunctionsData(file_name,this);

    file_name = filepath + std::string("/Bridges.json");
    readBridgesData(file_name, this);

    file_name = filepath + std::string("/CrossBridges.json");
    readCrossBridgesData(file_name, this);

    file_name = filepath + std::string("/DiffBridges.json");
    readDiffBridgesData(file_name, this);

    file_name = filepath + std::string("/Pedestals.json");
    readPedestalsData(file_name, this);

    file_name = filepath + std::string("/Anchors.json");
    readAnchorsData(file_name, this);

    file_name = filepath + std::string("/Pad.json");
    readPadData(file_name, this);
#endif

    //filelist << "0Heads" << "1Pillars" << "2Junctions" << "3Bridges" << "4CrossBridges"<< "5DiffBridges" << "6Pedestals" << "7Anchors" << "8Pad";
    int index = filename.at(0) - '0';
    switch (index)
    {
    case 0:
    {
        readHeadsData(filename, data, this);
        break;
    }
    case 1:
    {
        readPillarsData(filename, data, this);
        break;
    }
    case 2:
    {
        readJunctionsData(filename, data, this);
        break;
    }
    case 3:
    {
        readBridgesData(filename, data, this);
        break;
    }
    case 4:
    {
        readCrossBridgesData(filename, data, this);
        break;
    }
    case 5:
    {
        readDiffBridgesData(filename, data, this);
        break;
    }
    case 6:
    {
        readPedestalsData(filename, data, this);
        break;
    }
    case 7:
    {
        readAnchorsData(filename, data, this);
        break;
    }
    case 8:
    {
        readPadData(filename, data, this);
        break;
    }
    case 9:
    {
        readSquarePedestalsData(filename, data, this);
        break;
    }
    default:
        break;
    }
}

void SupportTreeBuilder::cloneSupportTreeData( const SupportTreeBuilder* srcPtr, const Slic3r::Transform3d transf3d)
{
    SupportTreeBuilder *srcBuilderPtr =(SupportTreeBuilder*)srcPtr;
    SupportTreeBuildsteps* srcalgPtr = (SupportTreeBuildsteps*)srcBuilderPtr->m_supportTreeBuildstepsPtr;
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
   assert(algPtr != NULL|| srcalgPtr != NULL);
   Matrix3d m;
   for (int i = 0; i < 3; ++i)
   {
       for (int j = 0; j < 3; ++j)
       {
           m(i, j) = transf3d(i, j);
       }
   }

    for (Head &obj : srcBuilderPtr->m_heads)
    {
        m_heads.emplace_back(obj);
       // m_heads.back().pos= m * obj.pos;
        m_heads.back().pos += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        //m_heads.back().setCreatedFlg(true);
    }
    for (size_t&obj : srcBuilderPtr->m_head_indices)
    {
        m_head_indices.emplace_back(obj);
    }

    for (Pillar&obj : srcBuilderPtr->m_pillars)
    {
        long id = obj.id;
        if (id >= m_pillars.size())
            m_pillars.resize(id + 1);
        m_pillars[id] = obj;
        //m_pillars.back().endpt= m * obj.endpt;
        m_pillars[id].endpt += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        algPtr->m_pillar_index.guarded_insert(m_pillars[id].endpt, unsigned(id));
        algPtr->m_pillar_index_start.guarded_insert(m_pillars[id].startpoint(), unsigned(id));

        //m_pillars.back().setCreatedFlg(true);

    }

    for (Junction& obj : srcBuilderPtr->m_junctions)
    {
        long id = obj.id;
        if (id >= m_junctions.size())
            m_junctions.resize(id + 1);
        m_junctions[id] = obj;
        //m_junctions.back().pos = m * obj.pos;
        m_junctions[id].pos += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        //m_junctions.back().setCreatedFlg(true);
    }
    for (Bridge&obj : srcBuilderPtr->m_bridges)
    {
        long id = obj.id;
        if (id >= m_bridges.size())
            m_bridges.resize(id + 1);
        m_bridges[id] = obj;

        m_bridges[id].startp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        m_bridges[id].endp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));

    }
    for (CrossBridge&obj : srcBuilderPtr->m_crossbridges)
    {
        long id = obj.id;
        if (id >= m_crossbridges.size())
            m_crossbridges.resize(id + 1);
        m_crossbridges[id] = obj;

        m_crossbridges[id].startp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        m_crossbridges[id].endp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));

    }
    for (DiffBridge&obj : srcBuilderPtr->m_diffbridges)
    {
        long id = obj.id;
        if (id >= m_diffbridges.size())
            m_diffbridges.resize(id + 1);
        m_diffbridges[id] = obj;

        m_diffbridges[id].startp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        m_diffbridges[id].endp += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));

    }
    for (Pedestal& obj : srcBuilderPtr->m_pedestals)
    {
        long id = obj.id;
        if (id >= m_pedestals.size())
            m_pedestals.resize(id + 1);
        m_pedestals[id] = obj;

        m_pedestals[id].pos += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
        //m_pedestals.back().setCreatedFlg(true);

    }
    for (Anchor& obj : srcBuilderPtr->m_anchors)
    {
        long id = obj.id;
        if (id >= m_anchors.size())
            m_anchors.resize(id + 1);
        m_anchors[id] = obj;

        m_anchors[id].pos  += Vec3d(transf3d.translation()(0), transf3d.translation()(1), transf3d.translation()(2));
       // m_anchors.back().setCreatedFlg(true);

    }

    algPtr->m_iheads = srcalgPtr->m_iheads;
    algPtr->m_iheads_onmodel = srcalgPtr->m_iheads_onmodel;
    algPtr->m_iheadless = srcalgPtr->m_iheadless;
    algPtr->m_addHeadsize = srcalgPtr->m_addHeadsize;
    ground_level = srcBuilderPtr->ground_level;

    m_pad.tmesh.clone(srcBuilderPtr->m_pad.tmesh);
}

void SupportTreeBuilder::releaseSupportTreeData()
{
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    if (algPtr)
    {
        algPtr->releaseSupportdata();
    }

    m_heads.clear();
    m_head_indices.clear();
    m_pillars.clear();
    m_junctions.clear();
    m_bridges.clear();
    m_crossbridges.clear();
    m_diffbridges.clear();
    m_pedestals.clear();
    m_anchors.clear();
    m_meshPadCache_valid = false;
    m_pad.tmesh.clear();
    m_EventHistoryDatasV.clear();
    m_RedoEventHistoryDatasV.clear();
}
long SupportTreeBuilder::searchNearObjInterconnect(Pillar &pillar, std::vector<SupportTreeNode2MeshLink> &outintermeshlink)
{
    SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)m_supportTreeBuildstepsPtr;
    long nearest_id = SupportTreeNode::ID_UNSET;
    if (algPtr)
    {
        unsigned neighbors = algPtr->m_cfg.pillar_cascade_neighbors;
        // connections are already enough for the pillar
        PointIndex spindex = algPtr->m_pillar_index.guarded_clone();
        Vec3d querypt = pillar.endpoint();
        if (pillar.links >= neighbors) return nearest_id;
        if (pillar.height < algPtr->m_cfg.max_solo_pillar_height_mm) return nearest_id;
        while (nearest_id < 0 && !spindex.empty()) {
            // loop until a suitable head is not found
            // if there is a pillar closer than the cluster center
            // (this may happen as the clustering is not perfect)
            // than we will bridge to this closer pillar

            //Vec3d qp(querypt(X), querypt(Y), m_builder.ground_level);
            Vec3d qp = querypt;
            auto qres = spindex.nearest(qp, 1);
            if (qres.empty())
                break;

            auto ne = qres.front();
            nearest_id = ne.second;

            if (!m_pillars[nearest_id].is_valid() || m_pillars[nearest_id].height < algPtr->m_cfg.max_solo_pillar_height_mm || nearest_id == pillar.id)
            {
                nearest_id = SupportTreeNode::ID_UNSET;    // continue searching
                spindex.remove(ne);       // without the current pillar
            }
        }
        if (nearest_id != SupportTreeNode::ID_UNSET)
        {
            algPtr->interconnect(pillar, m_pillars[nearest_id], outintermeshlink);
        }
    }
    return nearest_id;
}
} //namespace sla 
} // namespace Slic3r::sla
