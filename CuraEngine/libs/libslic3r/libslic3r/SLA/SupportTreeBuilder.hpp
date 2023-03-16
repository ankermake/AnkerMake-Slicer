#ifndef SLA_SUPPORTTREEBUILDER_HPP
#define SLA_SUPPORTTREEBUILDER_HPP

#include <libslic3r/SLA/Concurrency.hpp>
#include <libslic3r/SLA/JobController.hpp>
#include <libslic3r/SLA/SupportTreeHeader.h>
#include <libslic3r/SLA/SupportTreeNode.h>
#include <libslic3r/SLA/Contour3D.hpp>
#include <libslic3r/MTUtils.hpp>

namespace Slic3r {

    class TriangleMesh;
    class Model;
    class ModelInstance;
    class Polygon;
    class ExPolygon;

    using Polygons = std::vector<Polygon>;
    using ExPolygons = std::vector<ExPolygon>;
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#define CONTOUR_STEPS 12

namespace sla {
// This class will hold the support tree meshes with some additional
// bookkeeping as well. Various parts of the support geometry are stored
// separately and are merged when the caller queries the merged mesh. The
// merged result is cached for fast subsequent delivery of the merged mesh
// which can be quite complex. The support tree creation algorithm can use an
// instance of this class as a somewhat higher level tool for crafting the 3D
// support mesh. Parts can be added with the appropriate methods such as
// add_head or add_pillar which forwards the constructor arguments and fills
// the IDs of these substructures. The IDs are basically indices into the
// arrays of the appropriate type (heads, pillars, etc...). One can later query
// e.g. a pillar for a specific head...
//
// The support pad is considered an auxiliary geometry and is not part of the
// merged mesh. It can be retrieved using a dedicated method (pad())
class SupportTreeBuildsteps;
class SupportTreeBuilder{
    // For heads it is beneficial to use the same IDs as for the support points.
public:
    JobController m_ctl;
    int m_supportType=0x00;//default value 0x00.value 0x00:normale support ,value 0x01:vertical support
    std::vector<Head>       m_heads;
    std::vector<size_t>     m_head_indices;
    std::vector<Pillar>     m_pillars;
    std::vector<Junction>   m_junctions;
    std::vector<Bridge>     m_bridges;
    std::vector<CrossBridge>m_crossbridges;
    std::vector<DiffBridge> m_diffbridges;
    std::vector<Pedestal>   m_pedestals;
    std::vector<Anchor>     m_anchors;
    std::vector<SquarePedestal>   m_squarePedestals;
    Pad m_pad;

private:
    typedef struct DO_UNDO_DATA
    {
        std::vector<Head>       m_heads;
        std::vector<Pillar>     m_pillars;
        std::vector<Junction>   m_junctions;
        std::vector<Bridge>     m_bridges;
        std::vector<CrossBridge>m_crossbridges;
        std::vector<DiffBridge> m_diffbridges;
        std::vector<Pedestal>   m_pedestals;
        std::vector<Anchor>     m_anchors;
        std::vector<SquarePedestal>     m_squarePedestals;
        void release()
        {
            m_heads.clear();
            m_pillars.clear();
            m_junctions.clear();
            m_bridges.clear();
            m_crossbridges.clear();
            m_diffbridges.clear();
            m_pedestals.clear();
            m_anchors.clear();
            m_squarePedestals.clear();
       }
    }HistoryData;
    #define HistoryDataPair std::pair < SUPPORT_AJUST_EVENT, HistoryData>
    std::vector< std::vector<HistoryDataPair> >m_EventHistoryDatasV;
    std::vector< std::vector<HistoryDataPair> >m_RedoEventHistoryDatasV;
    std::vector<HistoryDataPair> m_EventHistoryData;
    using Mutex = ccr::SpinningMutex;
    mutable TriangleMesh m_meshPadCache;
    mutable TriangleMesh m_meshPadCacheNew;
    mutable bool m_meshPadCache_valid = false;
    
    mutable TriangleMesh m_meshcache;
    mutable Mutex m_mutex;
    mutable bool m_meshcache_valid = false;
    mutable double m_model_height = 0; // the full height of the model
    
    template<class BridgeT, class...Args>
     BridgeT& _add_bridge(std::vector<BridgeT> &br, Args&&... args)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        br.emplace_back(std::forward<Args>(args)...);
        br.back().id = long(br.size() - 1);
        m_meshcache_valid = false;
        return br.back();
    }
public:
    double ground_level = 0;
    SupportTreeBuildsteps* m_supportTreeBuildstepsPtr;

    SupportTreeBuilder();
    SupportTreeBuilder(SupportTreeBuilder &&o);
    SupportTreeBuilder(const SupportTreeBuilder &o);
    SupportTreeBuilder& operator=(SupportTreeBuilder &&o);
    SupportTreeBuilder& operator=(const SupportTreeBuilder &o);
    ~SupportTreeBuilder();

    template<class...Args> Head& add_head(unsigned id, Args&&... args)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        m_heads.emplace_back(std::forward<Args>(args)...);
        m_heads.back().id = id;
        
        if (id >= m_head_indices.size()) m_head_indices.resize(id + 1);
        m_head_indices[id] = m_heads.size() - 1;
        
        m_meshcache_valid = false;
        return m_heads.back();
    }
    
    template<class...Args> long add_pillar(long headid, double length)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (m_pillars.capacity() < m_heads.size())
            m_pillars.reserve(m_heads.size() * 10);
        
        assert(headid >= 0 && size_t(headid) < m_head_indices.size());
        Head &head = m_heads[m_head_indices[size_t(headid)]];
        
        Vec3d hjp = head.junction_point() - Vec3d{0, 0, length};
        m_pillars.emplace_back(hjp, length, head.r_back_mm);

        Pillar& pillar = m_pillars.back();
        pillar.id = long(m_pillars.size() - 1);
        head.pillar_id = pillar.id;
        pillar.start_junction_id = head.id;
        pillar.starts_from_head = true;
        
        m_meshcache_valid = false;
        return pillar.id;
    }

    template<class...Args> long addPillarBasePt(Vec3d endpt, double length,double radius)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (m_pillars.capacity() < m_heads.size())
            m_pillars.reserve(m_heads.size() * 10);

        m_pillars.emplace_back(endpt, length, radius);

        Pillar& pillar = m_pillars.back();
        pillar.id = long(m_pillars.size() - 1);
        m_meshcache_valid = false;
        return pillar.id;
    }
    void add_pillar_base(long pid, double baseheight = 3, double top_radius=2.0, double bt_radius = 2);

    Anchor& add_anchor(double r_big_mm,
        double r_small_mm,
        double length_mm,
        double penetration,
         Vec3d direction = DOWN,  // direction (normal to the dull end)
         Vec3d offset = { 0, 0, 0 })
    {
        std::lock_guard<Mutex> lk(m_mutex);
        m_anchors.emplace_back( r_big_mm,
             r_small_mm,
             length_mm,
             penetration,
             direction,  // direction (normal to the dull end)
            offset );
        m_anchors.back().id = long(m_anchors.size() - 1);
        m_meshcache_valid = false;
        return m_anchors.back();

        //std::lock_guard<Mutex> lk(m_mutex);
        //m_heads.emplace_back(std::forward<Args>(args)...);
        //m_heads.back().id = id;

        //if (id >= m_head_indices.size()) m_head_indices.resize(id + 1);
        //m_head_indices[id] = m_heads.size() - 1;

        //m_meshcache_valid = false;
        //return m_heads.back();

    }
    
    void increment_bridges(const Pillar& pillar)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        assert(pillar.id >= 0 && size_t(pillar.id) < m_pillars.size());
        
        if(pillar.id >= 0 && size_t(pillar.id) < m_pillars.size())
            m_pillars[size_t(pillar.id)].bridges++;
    }

    void setCtl(JobController jc);
    
    void increment_links(const Pillar& pillar)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        assert(pillar.id >= 0 && size_t(pillar.id) < m_pillars.size());
        
        if(pillar.id >= 0 && size_t(pillar.id) < m_pillars.size()) 
            m_pillars[size_t(pillar.id)].links++;
    }

    unsigned bridgecount(const Pillar &pillar) const {
        std::lock_guard<Mutex> lk(m_mutex);
        assert(pillar.id >= 0 && size_t(pillar.id) < m_pillars.size());
        return pillar.bridges;
    }
    
    template<class...Args> long add_pillar(Args&&...args)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (m_pillars.capacity() < m_heads.size())
            m_pillars.reserve(m_heads.size() * 10);
        
        m_pillars.emplace_back(std::forward<Args>(args)...);
        Pillar& pillar = m_pillars.back();
        pillar.id = long(m_pillars.size() - 1);
        pillar.starts_from_head = false;
        m_meshcache_valid = false;
        return pillar.id;
    }
    
    template<class...Args> const Junction& add_junction(Args&&... args)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        m_junctions.emplace_back(std::forward<Args>(args)...);
        m_junctions.back().id = long(m_junctions.size() - 1);
        m_meshcache_valid = false;
        return m_junctions.back();
    }
    
    const Bridge& add_bridge(const Vec3d &s, const Vec3d &e, double r)
    {
        return _add_bridge(m_bridges, s, e, r);
    }
    
    const Bridge& add_bridge(long headid, const Vec3d &endp)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        assert(headid >= 0 && size_t(headid) < m_head_indices.size());
        
        Head &h = m_heads[m_head_indices[size_t(headid)]];
        m_bridges.emplace_back(h.junction_point(), endp, h.r_back_mm);
        m_bridges.back().id = long(m_bridges.size() - 1);
        
        h.bridge_id = m_bridges.back().id;
        m_meshcache_valid = false;

        m_bridges.back().starts_from_head = true; // Could start from a junction as well
        m_bridges.back().start_junction_id = h.id;

        return m_bridges.back();
    }

    template<class...Args>  CrossBridge& add_crossbridge(Args&&... args)
    {
        return _add_bridge(m_crossbridges, std::forward<Args>(args)...);
    }

    template<class...Args> const DiffBridge& add_diffbridge(Args&&... args)
    {
        return _add_bridge(m_diffbridges, std::forward<Args>(args)...);
    }
    
    Head &head(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (id >= m_head_indices.size()|| id<0)
        {
            static auto obj = Head();
            obj = Head();
            return obj;
        }
        
        m_meshcache_valid = false;
        return m_heads[m_head_indices[id]];
    }
    Pillar &pillar(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (id >= m_pillars.size() || id < 0)
        {
            static Pillar obj = Pillar();
            obj = Pillar();
            return obj;
        }

        m_meshcache_valid = false;
        return m_pillars[id];
    }
    Junction &junction(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (id >= m_junctions.size() || id < 0)
        {
            static Junction obj = Junction();
            obj = Junction();
            return obj;
        }

        m_meshcache_valid = false;
        return m_junctions[id];
    }
     Bridge & bridge(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if (id >= m_bridges.size() || id < 0)
        {
            static Bridge obj = Bridge();
            obj = Bridge();
            return obj;
        }

        m_meshcache_valid = false;
        return m_bridges[id];
    }
     CrossBridge& crossbridge(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if(id >= m_crossbridges.size() || id < 0)
        {
            static CrossBridge obj = CrossBridge();
            obj = CrossBridge();
            return obj;
        }
        m_meshcache_valid = false;
        return m_crossbridges[id];
    }
     DiffBridge & diffbridge(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if(id >= m_diffbridges.size() || id < 0)
        {
            static DiffBridge obj = DiffBridge();
            obj = DiffBridge();
            return obj;
        }
        m_meshcache_valid = false;
        return m_diffbridges[id];
    }
     Pedestal & pedestal(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if(id >= m_pedestals.size() || id < 0)
        {
            static Pedestal obj = Pedestal();
            obj = Pedestal();
            return obj;
        }
        m_meshcache_valid = false;
        return m_pedestals[id];
    }
     Anchor & anchor(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if(id >= m_anchors.size() || id < 0)
        {
            static Anchor obj = Anchor();
            obj = Anchor();
            return obj;
        }

        m_meshcache_valid = false;
        return m_anchors[id];
    }
  
     SquarePedestal& squarePedestal(unsigned id)
    {
        std::lock_guard<Mutex> lk(m_mutex);
        if(id >= m_squarePedestals.size() || id < 0)
        {
            static SquarePedestal obj = SquarePedestal();
            obj = SquarePedestal();
            return obj;
        }
        m_meshcache_valid = false;
        return m_squarePedestals[id];
    }
    inline size_t pillarcount() const {
        std::lock_guard<Mutex> lk(m_mutex);
        return m_pillars.size();
    }
    
    inline const std::vector<Pillar> &pillars() const { return m_pillars; }
    inline const std::vector<Head>   &heads() const { return m_heads; }
    inline const std::vector<Bridge> &bridges() const { return m_bridges; }
    inline const std::vector<CrossBridge> &crossbridges() const { return m_crossbridges; }
    inline const std::vector<DiffBridge> &diffbridges() const { return m_diffbridges; }
    inline const std::vector<Pedestal> & pedestals() const { return m_pedestals; }
    inline const std::vector<Junction> & junctions() const { return m_junctions; }

    inline const std::vector<Pillar> pillars_pop_back() { m_pillars.pop_back(); return m_pillars; }
  

    const Pad& pad() const { return m_pad; }
    
    // WITHOUT THE PAD!!!
    const TriangleMesh &merged_mesh(size_t steps = CONTOUR_STEPS) const;

    const TriangleMesh & merged_meshforPad(size_t steps = CONTOUR_STEPS) const;
    
    // WITH THE PAD
    double full_height() const;
    
    // WITHOUT THE PAD!!!
    inline double mesh_height() const
    {
        if (!m_meshcache_valid) merged_mesh();
        return m_model_height;
    }
    
    // Intended to be called after the generation is fully complete
    const TriangleMesh & merge_and_cleanup();
    
    // Implement SupportTree interface:

    const TriangleMesh &add_pad(const ExPolygons &modelbase,
                                const PadConfig & pcfg);
    
    void remove_pad(){ m_pad = Pad(); }
    const JobController& ctl() const { return m_ctl; }

    virtual const TriangleMesh &retrieve_mesh(
        MeshType meshtype = MeshType::Support) const;

    virtual void retrieve_meshElements(MeshTypeElement meshtype, std::vector<TriangleMesh*>& triangleMeshes);
    virtual void retrieve_meshLinkElements(MeshTypeElement meshtype , std::vector < SupportTreeNode2MeshLink>& meshlink);
    bool addHead(const std::vector<Head>& heads, int add_status, std::vector<SupportTreeNode2MeshLink>& meshlink);
    
    virtual void invalidateTreeElement(SupportTreeNode2MeshLink source, std::vector < SupportTreeNode2MeshLink>& outmeshlinks);
    virtual void moveTreeElement(SupportTreeNode2MeshLink source, Slic3r::sla::MoveOffSet inOffset, std::vector < SupportTreeNode2MeshLink>& oldmeshlinks);

    virtual void searchMeshLinks(Slic3r::sla::SupportTreeNode2MeshLink source, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    virtual void invalidateMeshLinks(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inMeshlinks, bool pop_up=false);
    virtual void invalidateMeshLink(const SupportTreeNode2MeshLink& linkout, bool pop_up = false);
    virtual void redoHistoryDataMeshLinks(std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_AJUST_EVENT& event);
    virtual void undoHistoryDataMeshLinks(std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_AJUST_EVENT& event);
    virtual void updateSupportParams(const SupportTreeConfig& cfg);
    virtual void pushSupportTreeHistoryData(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inMeshlinks, SUPPORT_AJUST_EVENT event);
    virtual void popupUndoHistorydataInner();
    virtual void popupRedoHistorydataInner();
    virtual int writeSupportTreeData(int& index, std::string& filename, char* data, int& length);
    virtual void readSupportTreeData(std::string& filename, const char* data, int& length);
    void cloneSupportTreeData(const SupportTreeBuilder* srcPtr, const Slic3r::Transform3d transf3d);

    virtual void releaseSupportTreeData();
    virtual void invalidateLinkRelationShip(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& inmeshlinks);
    ///////////////////////////////////////
    void searchLeftTreeElement(SupportTreeNode &src, std::vector < SupportTreeNode2MeshLink>& outmeshlinks);
    void searchRightTreeElement(SupportTreeNode &src, std::vector < SupportTreeNode2MeshLink>& outmeshlinks);
    bool supportTreeNodeIsValid(SupportTreeNode2MeshLink linknd, SupportTreeNode** treeNodePtr = NULL);
    void extractSupportTreeNodeChanged(SupportTreeNode2MeshLink link, HistoryData& srcdata);
    void pushSupportTreeNodeChanged(const std::vector<HistoryData>& srcdata, std::vector < SUPPORT_AJUST_EVENT> event,bool sameTimeAction=false);
    void pushUndoHistoryData(const std::vector<HistoryDataPair> &historyData);
    void pushRedoHistoryData(const std::vector<HistoryDataPair> &historyData);

    void invalidateRelationShip(const SupportTreeNode* treeNdptr);
    void buildMeshLinkRelation(SupportTreeNode& treeNodeL, SupportTreeNode& treeNodeR);
    void reconstructRelationShip(SupportTreeNode& treeNdptr);
    void redoConstructMeshLink(const HistoryData& srcdata, SUPPORT_AJUST_EVENT event, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    void undoConstructMeshLink(const HistoryData& srcdata, SUPPORT_AJUST_EVENT event, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    void redoConstructMeshLinkRelationShip(const std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& meshlinks);
    void recoverDeletedMeshLinks(const HistoryData& srcdata, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    void DeletedAddedMeshLinks(const HistoryData& srcdata, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    void recoverMovedMeshLinks(const HistoryData& srcdata, std::vector< Slic3r::sla::SupportTreeNode2MeshLink>& outmeshlinks);
    bool reconstructTreeNode(SupportTreeNode2MeshLink meshlinkSrc, Vec3d offset, std::vector < SupportTreeNode2MeshLink>& outmeshlinks, SUPPORT_EVENT_FLAG srceventflg);
    long searchNearObjInterconnect(Pillar& pillar, std::vector<SupportTreeNode2MeshLink>& outintermeshlink);
};

}} // namespace Slic3r::sla

#endif // SUPPORTTREEBUILDER_HPP
