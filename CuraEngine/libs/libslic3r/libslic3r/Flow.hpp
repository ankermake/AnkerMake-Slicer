#ifndef slic3r_Flow_hpp_
#define slic3r_Flow_hpp_

#include "libslic3r.h"
#include "Config.hpp"
#include "Exception.hpp"
#include "ExtrusionEntity.hpp"

namespace Slic3r {

class PrintObject;

// Extra spacing of bridge threads, in mm.
#define BRIDGE_EXTRA_SPACING 0.05

// Overlap factor of perimeter lines. Currently no overlap.
#ifdef HAS_PERIMETER_LINE_OVERLAP
    #define PERIMETER_LINE_OVERLAP_FACTOR 1.0
#endif

enum FlowRole {
    frExternalPerimeter,
    frPerimeter,
    frInfill,
    frSolidInfill,
    frTopSolidInfill,
    frSupportMaterial,
    frSupportMaterialInterface,
};

class FlowError : public Slic3r::InvalidArgument
{
public:
	FlowError(const std::string& what_arg) : Slic3r::InvalidArgument(what_arg) {}
	FlowError(const char* what_arg) : Slic3r::InvalidArgument(what_arg) {}
};

class FlowErrorNegativeSpacing : public FlowError
{
public:
    FlowErrorNegativeSpacing();
};

class FlowErrorNegativeFlow : public FlowError
{
public:
    FlowErrorNegativeFlow();
};

class FlowErrorMissingVariable : public FlowError
{
public:
    FlowErrorMissingVariable(const std::string& what_arg) : FlowError(what_arg) {}
};

class Flow
{
public:
    // Non bridging flow: Maximum width of an extrusion with semicircles at the ends.
    // Bridging flow: Bridge thread diameter.
    float width;
    // Non bridging flow: Layer height.
    // Bridging flow: Bridge thread diameter = layer height.
    float height;
    // Nozzle diameter. 
    float nozzle_diameter;
    // Is it a bridge?
    bool  bridge;
    
    Flow(float _w, float _h, float _nd, bool _bridge = false) :
        width(_w), height(_h), nozzle_diameter(_nd), bridge(_bridge) {}

    float   spacing() const;
    float   spacing(const Flow &other) const;
    double  mm3_per_mm() const;
    coord_t scaled_width() const { return coord_t(scale_(this->width)); }
    coord_t scaled_spacing() const { return coord_t(scale_(this->spacing())); }
    coord_t scaled_spacing(const Flow &other) const { return coord_t(scale_(this->spacing(other))); }

    // Elephant foot compensation spacing to be used to detect narrow parts, where the elephant foot compensation cannot be applied.
    // To be used on frExternalPerimeter only.
    // Enable some perimeter squish (see INSET_OVERLAP_TOLERANCE).
    // Here an overlap of 0.2x external perimeter spacing is allowed for by the elephant foot compensation.
    coord_t scaled_elephant_foot_spacing() const { return coord_t(0.5f * float(this->scaled_width() + 0.6f * this->scaled_spacing())); }

    bool operator==(const Flow &rhs) const { return this->width == rhs.width && this->height == rhs.height && this->nozzle_diameter == rhs.nozzle_diameter && this->bridge == rhs.bridge; }
    
    static Flow new_from_config_width(FlowRole role, const ConfigOptionFloatOrPercent &width, float nozzle_diameter, float height, float bridge_flow_ratio);
    // Create a flow from the spacing of extrusion lines.
    // This method is used exclusively to calculate new flow of 100% infill, where the extrusion width was allowed to scale
    // to fit a region with integer number of lines.
    static Flow new_from_spacing(float spacing, float nozzle_diameter, float height, bool bridge);

    // Sane extrusion width defautl based on nozzle diameter.
    // The defaults were derived from manual Prusa MK3 profiles.
    static float auto_extrusion_width(FlowRole role, float nozzle_diameter);

    // Extrusion width from full config, taking into account the defaults (when set to zero) and ratios (percentages).
    // Precise value depends on layer index (1st layer vs. other layers vs. variable layer height),
    // on active extruder etc. Therefore the value calculated by this function shall be used as a hint only.
	static double extrusion_width(const std::string &opt_key, const ConfigOptionFloatOrPercent *opt, const ConfigOptionResolver &config, const unsigned int first_printing_extruder = 0);
	static double extrusion_width(const std::string &opt_key, const ConfigOptionResolver &config, const unsigned int first_printing_extruder = 0);
};

extern Flow support_material_flow(const PrintObject *object, float layer_height = 0.f);
extern Flow support_material_1st_layer_flow(const PrintObject *object, float layer_height = 0.f);
extern Flow support_material_interface_flow(const PrintObject *object, float layer_height = 0.f);

}

#endif
