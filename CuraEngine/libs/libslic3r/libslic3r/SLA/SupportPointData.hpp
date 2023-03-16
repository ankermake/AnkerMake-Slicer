#ifndef SLIC3R_SUPPORTPOINTDATA_1634225497208_H
#define SLIC3R_SUPPORTPOINTDATA_1634225497208_H
#include <libslic3r/SLA/SupportPoint.hpp>
#include <libslic3r/BoundingBox.hpp>
#include <libslic3r/ClipperUtils.hpp>
#include <libslic3r/Point.hpp>

#include <boost/container/small_vector.hpp>

#define M_PI       3.14159265358979323846   // pi

namespace Slic3r { namespace sla {

	struct SupportPointGeneratorConfig {
		float density_relative{ 0.5f };
		float minimal_distance{ 1.f };
		float head_diameter{ 0.4f };

		float closing_radius{ 0.005f };  //for slice
		float layer_height{ 0.05f };

		float safe_angle{ 35.0f };
		float slope_angle{ 45.0f };

		//debug
		bool sample_only_overhang{ true };

		float new_island_offset{ 0.3f };
		float support_min_area{ 0.01f };

		bool use_raw_sample_points{ false };
		bool use_userdef_layer{ true };
		int debug_area_type { 3 };
		//no use
		bool project_only_up{ false };
		bool use_norandom_sample{ false };
		bool use_user_density{ false };

		bool only_first_overhangs{ false };
		bool only_first_layer{ false };

		// Originally calibrated to 7.7f, reduced density by Tamas to 70% which is 11.1 (7.7 / 0.7) to adjust for new algorithm changes in tm_suppt_gen_improve
		inline float support_force() const { return 11.1f / density_relative; } // a force one point can support       (arbitrary force unit)
		inline float tear_pressure() const { return 1.f; }  // pressure that the display exerts    (the force unit per mm2)
	};

	enum class IslandType
	{
		isTypeNone = -1,
		isTypeNew = 0,
		isTypeOverhangs = 1,
		isTypeOverhangsSlope = 2,
		isTypeDangling = 3,
		isTypeOverhangs_minArea = 4
	};

	const int OverhangsFlag = 1;
	const int OverhangsSlopeFlag = 2;
	const int DanglingFlag = 4;

	struct PointGrid3D {
		struct GridHash {
			std::size_t operator()(const Vec3i& cell_id) const {
				return std::hash<int>()(cell_id.x()) ^ std::hash<int>()(cell_id.y() * 593) ^ std::hash<int>()(cell_id.z() * 7919);
			}
		};
		typedef std::unordered_multimap<Vec3i, Vec3f, GridHash> Grid;

		Vec3f   cell_size;
		Grid    grid;

		Vec3i cell_id(const Vec3f& pos) {
			return Vec3i(int(floor(pos.x() / cell_size.x())),
				int(floor(pos.y() / cell_size.y())),
				int(floor(pos.z() / cell_size.z())));
		}

		void insert(const Vec2f& pos, float z) {
			Vec3f position = Vec3f(pos.x(), pos.y(), z);
			//pt.island   = island;
			grid.emplace(cell_id(position), position);
		}

		bool collides_with(const Vec2f& pos, float print_z, float radius) {
			Vec3f pos3d(pos.x(), pos.y(), print_z);
			Vec3i cell = cell_id(pos3d);
			std::pair<Grid::const_iterator, Grid::const_iterator> it_pair = grid.equal_range(cell);
			if (collides_with(pos3d, radius, it_pair.first, it_pair.second))
				return true;
			for (int i = -1; i < 2; ++i)
				for (int j = -1; j < 2; ++j)
					for (int k = -1; k < 1; ++k) {
						if (i == 0 && j == 0 && k == 0)
							continue;
						it_pair = grid.equal_range(cell + Vec3i(i, j, k));
						if (collides_with(pos3d, radius, it_pair.first, it_pair.second))
							return true;
					}
			return false;
		}

	private:
		bool collides_with(const Vec3f& pos, float radius, Grid::const_iterator it_begin, Grid::const_iterator it_end) {
			for (Grid::const_iterator it = it_begin; it != it_end; ++it) {
				float dist2 = (it->second - pos).squaredNorm();
				if (dist2 < radius * radius)
					return true;
			}
			return false;
		}
	};

	struct ICFLayer;
	struct ICFStructure {
		ICFStructure(ICFLayer& layer, const ExPolygon& poly, const BoundingBox& bbox, const Vec2f& centroid, float area, float h) :
			layer(&layer), polygon(&poly), bbox(bbox), centroid(centroid), area(area), zlevel(h)
		{
			soffset = scaled < float>( 0.05f * tanf(5.0f * M_PI / 180.0f));
		}
		ICFLayer* layer;
		const ExPolygon* polygon = nullptr;
		const BoundingBox bbox;
		const Vec2f centroid = Vec2f::Zero();
		const float area = 0.f;
		float zlevel = 0;
		// How well is this ExPolygon held to the print base?
		// Positive number, the higher the better.
		float supports_force_this_layer = 0.f;
		float supports_force_inherited = 0.f;
		float supports_force_total() const { return this->supports_force_this_layer + this->supports_force_inherited; }

		struct Link {
			Link(ICFStructure* island, float overlap_area) : island(island), overlap_area(overlap_area) {}
			ICFStructure* island;
			float        overlap_area;
		};

#ifdef NDEBUG
		// In release mode, use the optimized container.
		boost::container::small_vector<Link, 4> islands_above;
		boost::container::small_vector<Link, 4> islands_below;
#else
		// In debug mode, use the standard vector, which is well handled by debugger visualizer.
		std::vector<Link>					 	islands_above;
		std::vector<Link>						islands_below;
#endif
		// Overhangs, that are dangling considerably.
		ExPolygons                              dangling_areas;
		// Complete overhands.
		ExPolygons                              overhangs;
		// top2 
		ExPolygons                              top2_offset_areas;
		ExPolygons                              top2_offset_sharp;
		// Overhangs, where the surface must slope.
		ExPolygons                              overhangs_slopes;
		ExPolygons								mini_overhangs_areas;

		Polygons                              belows_support_areas;
		float                                   overhangs_area = 0.f;
		float                                   soffset = 0.f;

		bool                                    isFirstLyer = false;

		bool overlaps(const ICFStructure& rhs) const {
			return this->bbox.overlap(rhs.bbox) && (this->polygon->overlaps(*rhs.polygon) || rhs.polygon->overlaps(*this->polygon));
		}
		float overlap_area(const ICFStructure& rhs) const {
			double out = 0.;
			if (this->bbox.overlap(rhs.bbox)) {
				Polygons polys = intersection(to_polygons(*this->polygon), to_polygons(*rhs.polygon), false);
				for (const Polygon& poly : polys)
					out += poly.area();
			}
			return float(out);
		}
		float overlap_area_outer(const ICFStructure& rhs) const {
			double out = 0.;
			if (this->bbox.overlap(rhs.bbox)) {
				Polygons polys = intersection(to_polygons_outer(*this->polygon), to_polygons_outer(*rhs.polygon), false);
				for (const Polygon& poly : polys)
					out += poly.area();
			}
			return float(out);
		}
		float area_below() const {
			float area = 0.f;
			for (const Link& below : this->islands_below)
				area += below.island->area;
			return area;
		}
		Polygons polygons_below() const {
			size_t cnt = 0;
			for (const Link& below : this->islands_below)
				cnt += 1 + below.island->polygon->holes.size();
			Polygons out;
			out.reserve(cnt);
			for (const Link& below : this->islands_below) {
				out.emplace_back(below.island->polygon->contour);
				append(out, below.island->polygon->holes);
			}
			return out;
		}
		Polygons polygons_above() const {
			size_t cnt = 0;
			for (const Link& above : this->islands_above)
				cnt += 1 + above.island->polygon->holes.size();
			Polygons out;
			out.reserve(cnt);
			for (const Link& above : this->islands_above) {
				out.emplace_back(above.island->polygon->contour);
				append(out, above.island->polygon->holes);
			}
			return out;
		}
		ExPolygons expolygons_below() const {
			ExPolygons out;
			out.reserve(this->islands_below.size());
			for (const Link& below : this->islands_below)
				out.emplace_back(*below.island->polygon);
			return out;
		}
		// Positive deficit of the supports. If negative, this area is well supported. If positive, more supports need to be added.
		float support_force_deficit(const float tear_pressure) const { return this->area * tear_pressure - this->supports_force_total(); }
	};

	struct ICFLayer {
		ICFLayer(const size_t layer_id, coordf_t print_z) : layer_id(layer_id), print_z(print_z) {}
		size_t                  layer_id;
		coordf_t                print_z;
		std::vector<ICFStructure>  islands;
	};
}}

#endif // SLIC3R_SUPPORTPOINTDATA_1634225497208_H
