#ifndef SLA_SUPPORTPOINTGENERATOR_HPP
#define SLA_SUPPORTPOINTGENERATOR_HPP

#include <random>

#include <libslic3r/SLA/SupportPointData.hpp>
#include <libslic3r/SLA/IndexedMesh.hpp>

namespace Slic3r { namespace sla {
	class SupportPointGeneratorTracer;

class SupportPointGenerator {
public:
    SupportPointGenerator(const IndexedMesh& emesh, const std::vector<ExPolygons>& slices,
                    const std::vector<float>& heights, const SupportPointGeneratorConfig& config, std::function<void(void)> throw_on_cancel, std::function<void(float)> statusfn);
    
    SupportPointGenerator(const IndexedMesh& emesh, const SupportPointGeneratorConfig& config, std::function<void(void)> throw_on_cancel, std::function<void(float)> statusfn);
    
	void setType(int type);

    const std::vector<SupportPoint>& output() const { return m_output; }
    std::vector<SupportPoint>& output() { return m_output; }
        
    void execute(const std::vector<ExPolygons> &slices,
                 const std::vector<float> &     heights);
    
    void seed(std::mt19937::result_type s) { m_rng.seed(s); }

	void setSupportPointGeneratorTracer(SupportPointGeneratorTracer* tracer) { m_tracer = tracer; }
private:
    std::vector<SupportPoint> m_output;
	SupportPointGeneratorTracer* m_tracer;
    SupportPointGeneratorConfig m_config;
    
    void process(const std::vector<ExPolygons>& slices, const std::vector<float>& heights);
private:
    void uniformly_cover(const ExPolygons& islands, ICFStructure& structure, float deficit,
        PointGrid3D &grid3d, IslandType type = IslandType::isTypeNone, bool sampleBoundary = false);
    void uniformly_cover_min(const ExPolygons& islands, ICFStructure& structure, float deficit,
        PointGrid3D& grid3d, IslandType type = IslandType::isTypeNone, bool sampleBoundary = false);
    void uniformly_cover_min1(const ExPolygons& islands, ICFStructure& structure, float deficit,
        PointGrid3D& grid3d, IslandType type = IslandType::isTypeNone, bool sampleBoundary = false);
    void uniformly_cover_min2(const ExPolygons& islands, ICFStructure& structure, float poisson_radius,
        PointGrid3D& grid3d, IslandType type, bool sampleBoundary);
    void add_support_points(ICFStructure& structure, PointGrid3D &grid3d);
    void project_onto_mesh(std::vector<SupportPoint>& points) const;

    const IndexedMesh& m_emesh;
    std::function<void(void)> m_throw_on_cancel;
    std::function<void(float)>  m_statusfn;
    
    std::mt19937 m_rng;

	int m_type;
};

void remove_bottom_points(std::vector<SupportPoint> &pts, float lvl);

std::vector<Vec2f> sample_expolygon(const ExPolygon &expoly, float samples_per_mm2, std::mt19937 &rng);
void sample_expolygon_boundary(const ExPolygon &expoly, float samples_per_mm, std::vector<Vec2f> &out, std::mt19937 &rng);

class SupportPointGeneratorTracer
{
public:
	virtual ~SupportPointGeneratorTracer() {}

	virtual void onMylayers(std::vector<ICFLayer>& layers) = 0;
    virtual void onPlanePoints(const std::vector<SupportPoint>& points) = 0;
    virtual void onModelPoints(const std::vector<SupportPoint>& points) = 0;
};

}} // namespace Slic3r::sla

#endif // SUPPORTPOINTGENERATOR_HPP
