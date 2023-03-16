#include "WipeTower.hpp"

#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>

#include "GCodeProcessor.hpp"
#include "BoundingBox.hpp"


// Experimental "Peter's wipe tower" feature was partially implemented, inspired by
// PJR's idea of alternating two perpendicular wiping directions on a square tower.
// It is probably never going to be finished, there are multiple remaining issues
// and there is probably no need to go down this way. m_peters_wipe_tower variable
// turns this on, maybe it should just be removed. Anyway, the issues are
// - layer's are not exactly square
// - variable width for higher levels
// - make sure it is not too sparse (apply max_bridge_distance and make last wipe longer)
// - enable enhanced first layer adhesion


namespace Slic3r
{

class WipeTowerWriter
{
public:
	WipeTowerWriter(float layer_height, float line_width, GCodeFlavor flavor, const std::vector<WipeTower::FilamentParameters>& filament_parameters) :
		m_current_pos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
		m_current_z(0.f),
		m_current_feedrate(0.f),
		m_layer_height(layer_height),
		m_extrusion_flow(0.f),
		m_preview_suppressed(false),
		m_elapsed_time(0.f),
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        m_default_analyzer_line_width(line_width),
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
        m_gcode_flavor(flavor),
        m_filpar(filament_parameters)
        {
            // adds tag for analyzer:
            char buf[64];
            sprintf(buf, ";%s%f\n", GCodeProcessor::Height_Tag.c_str(), m_layer_height); // don't rely on GCodeAnalyzer knowing the layer height - it knows nothing at priming
            m_gcode += buf;
            sprintf(buf, ";%s%s\n", GCodeProcessor::Extrusion_Role_Tag.c_str(), ExtrusionEntity::role_to_string(erWipeTower).c_str());
            m_gcode += buf;
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING
            change_analyzer_line_width(line_width);
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING
        }

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    WipeTowerWriter& change_analyzer_line_width(float line_width) {
        // adds tag for analyzer:
        char buf[64];
        sprintf(buf, ";%s%f\n", GCodeProcessor::Width_Tag.c_str(), line_width);
        m_gcode += buf;
        return *this;
    }
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
#if !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    WipeTowerWriter& change_analyzer_line_width(float line_width) {
        // adds tag for analyzer:
        char buf[64];
        sprintf(buf, ";%s%f\n", GCodeProcessor::Width_Tag.c_str(), line_width);
        m_gcode += buf;
        return *this;
    }
#endif // !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

    WipeTowerWriter& change_analyzer_mm3_per_mm(float len, float e) {
        static const float area = float(M_PI) * 1.75f * 1.75f / 4.f;
        float mm3_per_mm = (len == 0.f ? 0.f : area * e / len);
        // adds tag for processor:
        char buf[64];
        sprintf(buf, ";%s%f\n", GCodeProcessor::Mm3_Per_Mm_Tag.c_str(), mm3_per_mm);
        m_gcode += buf;
        return *this;
    }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

	WipeTowerWriter& 			 set_initial_position(const Vec2f &pos, float width = 0.f, float depth = 0.f, float internal_angle = 0.f) {
        m_wipe_tower_width = width;
        m_wipe_tower_depth = depth;
        m_internal_angle = internal_angle;
		m_start_pos = this->rotate(pos);
		m_current_pos = pos;
		return *this;
	}

    WipeTowerWriter&				 set_initial_tool(size_t tool) { m_current_tool = tool; return *this; }

	WipeTowerWriter&				 set_z(float z) 
		{ m_current_z = z; return *this; }

	WipeTowerWriter& 			 set_extrusion_flow(float flow)
		{ m_extrusion_flow = flow; return *this; }

	WipeTowerWriter&				 set_y_shift(float shift) {
        m_current_pos.y() -= shift-m_y_shift;
        m_y_shift = shift;
        return (*this);
    }

    WipeTowerWriter&            disable_linear_advance() {
        m_gcode += (m_gcode_flavor == gcfRepRapSprinter || m_gcode_flavor == gcfRepRapFirmware
                        ? (std::string("M572 D") + std::to_string(m_current_tool) + " S0\n")
                        : std::string("M900 K0\n"));
        return *this;
    }

	// Suppress / resume G-code preview in Slic3r. Slic3r will have difficulty to differentiate the various
	// filament loading and cooling moves from normal extrusion moves. Therefore the writer
	// is asked to suppres output of some lines, which look like extrusions.
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
    WipeTowerWriter& suppress_preview() { change_analyzer_line_width(0.f); m_preview_suppressed = true; return *this; }
    WipeTowerWriter& resume_preview() { change_analyzer_line_width(m_default_analyzer_line_width); m_preview_suppressed = false; return *this; }
#else
    WipeTowerWriter& 			 suppress_preview() { m_preview_suppressed = true; return *this; }
	WipeTowerWriter& 			 resume_preview()   { m_preview_suppressed = false; return *this; }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

	WipeTowerWriter& 			 feedrate(float f)
	{
		if (f != m_current_feedrate)
			m_gcode += "G1" + set_format_F(f) + "\n";
		return *this;
	}

	const std::string&   gcode() const { return m_gcode; }
	const std::vector<WipeTower::Extrusion>& extrusions() const { return m_extrusions; }
	float                x()     const { return m_current_pos.x(); }
	float                y()     const { return m_current_pos.y(); }
	const Vec2f& 		 pos()   const { return m_current_pos; }
	const Vec2f	 		 start_pos_rotated() const { return m_start_pos; }
	const Vec2f  		 pos_rotated() const { return this->rotate(m_current_pos); }
	float 				 elapsed_time() const { return m_elapsed_time; }
    float                get_and_reset_used_filament_length() { float temp = m_used_filament_length; m_used_filament_length = 0.f; return temp; }

	// Extrude with an explicitely provided amount of extrusion.
	WipeTowerWriter& extrude_explicit(float x, float y, float e, float f = 0.f, bool record_length = false, bool limit_volumetric_flow = true)
	{
		if (x == m_current_pos.x() && y == m_current_pos.y() && e == 0.f && (f == 0.f || f == m_current_feedrate))
			// Neither extrusion nor a travel move.
			return *this;

		float dx = x - m_current_pos.x();
		float dy = y - m_current_pos.y();
        float len = std::sqrt(dx*dx+dy*dy);
        if (record_length)
            m_used_filament_length += e;

		// Now do the "internal rotation" with respect to the wipe tower center
		Vec2f rotated_current_pos(this->pos_rotated());
		Vec2f rot(this->rotate(Vec2f(x,y)));                               // this is where we want to go

        if (! m_preview_suppressed && e > 0.f && len > 0.f) {
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
            change_analyzer_mm3_per_mm(len, e);
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
            // Width of a squished extrusion, corrected for the roundings of the squished extrusions.
			// This is left zero if it is a travel move.
            float width = e * m_filpar[0].filament_area / (len * m_layer_height);
			// Correct for the roundings of a squished extrusion.
			width += m_layer_height * float(1. - M_PI / 4.);
			if (m_extrusions.empty() || m_extrusions.back().pos != rotated_current_pos)
				m_extrusions.emplace_back(WipeTower::Extrusion(rotated_current_pos, 0, m_current_tool));
			m_extrusions.emplace_back(WipeTower::Extrusion(rot, width, m_current_tool));
		}

		m_gcode += "G1";
        if (std::abs(rot.x() - rotated_current_pos.x()) > (float)EPSILON)
			m_gcode += set_format_X(rot.x());

        if (std::abs(rot.y() - rotated_current_pos.y()) > (float)EPSILON)
			m_gcode += set_format_Y(rot.y());


		if (e != 0.f)
			m_gcode += set_format_E(e);

		if (f != 0.f && f != m_current_feedrate) {
            if (limit_volumetric_flow) {
                float e_speed = e / (((len == 0.f) ? std::abs(e) : len) / f * 60.f);
                f /= std::max(1.f, e_speed / m_filpar[m_current_tool].max_e_speed);
            }
			m_gcode += set_format_F(f);
        }

        m_current_pos.x() = x;
        m_current_pos.y() = y;

		// Update the elapsed time with a rough estimate.
        m_elapsed_time += ((len == 0.f) ? std::abs(e) : len) / m_current_feedrate * 60.f;
		m_gcode += "\n";
		return *this;
	}

	WipeTowerWriter& extrude_explicit(const Vec2f &dest, float e, float f = 0.f, bool record_length = false, bool limit_volumetric_flow = true)
		{ return extrude_explicit(dest.x(), dest.y(), e, f, record_length); }

	// Travel to a new XY position. f=0 means use the current value.
	WipeTowerWriter& travel(float x, float y, float f = 0.f)
		{ return extrude_explicit(x, y, 0.f, f); }

	WipeTowerWriter& travel(const Vec2f &dest, float f = 0.f) 
		{ return extrude_explicit(dest.x(), dest.y(), 0.f, f); }

	// Extrude a line from current position to x, y with the extrusion amount given by m_extrusion_flow.
	WipeTowerWriter& extrude(float x, float y, float f = 0.f)
	{
		float dx = x - m_current_pos.x();
		float dy = y - m_current_pos.y();
        return extrude_explicit(x, y, std::sqrt(dx*dx+dy*dy) * m_extrusion_flow, f, true);
	}

	WipeTowerWriter& extrude(const Vec2f &dest, const float f = 0.f) 
		{ return extrude(dest.x(), dest.y(), f); }
        
    WipeTowerWriter& rectangle(const Vec2f& ld,float width,float height,const float f = 0.f)
    {
        Vec2f corners[4];
        corners[0] = ld;
        corners[1] = ld + Vec2f(width,0.f);
        corners[2] = ld + Vec2f(width,height);
        corners[3] = ld + Vec2f(0.f,height);
        int index_of_closest = 0;
        if (x()-ld.x() > ld.x()+width-x())    // closer to the right
            index_of_closest = 1;
        if (y()-ld.y() > ld.y()+height-y())   // closer to the top
            index_of_closest = (index_of_closest==0 ? 3 : 2);

        travel(corners[index_of_closest].x(), y());      // travel to the closest corner
        travel(x(),corners[index_of_closest].y());

        int i = index_of_closest;
        do {
            ++i;
            if (i==4) i=0;
            extrude(corners[i], f);
        } while (i != index_of_closest);
        return (*this);
    }

	WipeTowerWriter& load(float e, float f = 0.f)
	{
		if (e == 0.f && (f == 0.f || f == m_current_feedrate))
			return *this;
		m_gcode += "G1";
		if (e != 0.f)
			m_gcode += set_format_E(e);
		if (f != 0.f && f != m_current_feedrate)
			m_gcode += set_format_F(f);
		m_gcode += "\n";
		return *this;
	}

	WipeTowerWriter& retract(float e, float f = 0.f)
		{ return load(-e, f); }

// Loads filament while also moving towards given points in x-axis (x feedrate is limited by cutting the distance short if necessary)
    WipeTowerWriter& load_move_x_advanced(float farthest_x, float loading_dist, float loading_speed, float max_x_speed = 50.f)
    {
        float time = std::abs(loading_dist / loading_speed); // time that the move must take
        float x_distance = std::abs(farthest_x - x());       // max x-distance that we can travel
        float x_speed = x_distance / time;                   // x-speed to do it in that time

        if (x_speed > max_x_speed) {
            // Necessary x_speed is too high - we must shorten the distance to achieve max_x_speed and still respect the time.
            x_distance = max_x_speed * time;
            x_speed = max_x_speed;
        }

        float end_point = x() + (farthest_x > x() ? 1.f : -1.f) * x_distance;
        return extrude_explicit(end_point, y(), loading_dist, x_speed * 60.f, false, false);
    }

	// Elevate the extruder head above the current print_z position.
	WipeTowerWriter& z_hop(float hop, float f = 0.f)
	{ 
		m_gcode += std::string("G1") + set_format_Z(m_current_z + hop);
		if (f != 0 && f != m_current_feedrate)
			m_gcode += set_format_F(f);
		m_gcode += "\n";
		return *this;
	}

	// Lower the extruder head back to the current print_z position.
	WipeTowerWriter& z_hop_reset(float f = 0.f) 
		{ return z_hop(0, f); }

	// Move to x1, +y_increment,
	// extrude quickly amount e to x2 with feed f.
	WipeTowerWriter& ram(float x1, float x2, float dy, float e0, float e, float f)
	{
		extrude_explicit(x1, m_current_pos.y() + dy, e0, f, true, false);
		extrude_explicit(x2, m_current_pos.y(), e, 0.f, true, false);
		return *this;
	}

	// Let the end of the pulled out filament cool down in the cooling tube
	// by moving up and down and moving the print head left / right
	// at the current Y position to spread the leaking material.
	WipeTowerWriter& cool(float x1, float x2, float e1, float e2, float f)
	{
		extrude_explicit(x1, m_current_pos.y(), e1, f, false, false);
		extrude_explicit(x2, m_current_pos.y(), e2, false, false);
		return *this;
	}

    WipeTowerWriter& set_tool(size_t tool)
	{
		m_current_tool = tool;
		return *this;
	}

	// Set extruder temperature, don't wait by default.
	WipeTowerWriter& set_extruder_temp(int temperature, bool wait = false)
	{
        m_gcode += "M" + std::to_string(wait ? 109 : 104) + " S" + std::to_string(temperature) + "\n";
        return *this;
    }

    // Wait for a period of time (seconds).
	WipeTowerWriter& wait(float time)
	{
        if (time==0.f)
            return *this;
		char buf[128];
		sprintf(buf, "G4 S%.3f\n", time);
		m_gcode += buf;
		return *this;
    }

	// Set speed factor override percentage.
	WipeTowerWriter& speed_override(int speed)
	{
        m_gcode += "M220 S" + std::to_string(speed) + "\n";
		return *this;
    }

	// Let the firmware back up the active speed override value.
	WipeTowerWriter& speed_override_backup()
    {
        // This is only supported by Prusa at this point (https://github.com/prusa3d/PrusaSlicer/issues/3114)
        if (m_gcode_flavor == gcfMarlin)
            m_gcode += "M220 B\n";
		return *this;
    }

	// Let the firmware restore the active speed override value.
	WipeTowerWriter& speed_override_restore()
	{
        if (m_gcode_flavor == gcfMarlin)
            m_gcode += "M220 R\n";
		return *this;
    }

	// Set digital trimpot motor
	WipeTowerWriter& set_extruder_trimpot(int current)
	{
        if (m_gcode_flavor == gcfRepRapSprinter || m_gcode_flavor == gcfRepRapFirmware)
            m_gcode += "M906 E";
        else
            m_gcode += "M907 E";
        m_gcode += std::to_string(current) + "\n";
		return *this;
    }

	WipeTowerWriter& flush_planner_queue()
	{ 
		m_gcode += "G4 S0\n"; 
		return *this;
	}

	// Reset internal extruder counter.
	WipeTowerWriter& reset_extruder()
	{ 
		m_gcode += "G92 E0\n";
		return *this;
	}

	WipeTowerWriter& comment_with_value(const char *comment, int value)
    {
        m_gcode += std::string(";") + comment + std::to_string(value) + "\n";
		return *this;
    }


    WipeTowerWriter& set_fan(unsigned speed)
	{
		if (speed == m_last_fan_speed)
			return *this;
		if (speed == 0)
			m_gcode += "M107\n";
        else
            m_gcode += "M106 S" + std::to_string(unsigned(255.0 * speed / 100.0)) + "\n";
		m_last_fan_speed = speed;
		return *this;
	}

	WipeTowerWriter& append(const std::string& text) { m_gcode += text; return *this; }

    std::vector<Vec2f> wipe_path() const
    {
        return m_wipe_path;
    }

    WipeTowerWriter& add_wipe_point(const Vec2f& pt)
    {
        m_wipe_path.push_back(rotate(pt));
        return *this;
    }

    WipeTowerWriter& add_wipe_point(float x, float y)
    {
        return add_wipe_point(Vec2f(x, y));
    }

private:
	Vec2f         m_start_pos;
	Vec2f         m_current_pos;
    std::vector<Vec2f>  m_wipe_path;
	float    	  m_current_z;
	float 	  	  m_current_feedrate;
    size_t        m_current_tool;
	float 		  m_layer_height;
	float 	  	  m_extrusion_flow;
	bool		  m_preview_suppressed;
	std::string   m_gcode;
	std::vector<WipeTower::Extrusion> m_extrusions;
	float         m_elapsed_time;
	float   	  m_internal_angle = 0.f;
	float		  m_y_shift = 0.f;
	float 		  m_wipe_tower_width = 0.f;
	float		  m_wipe_tower_depth = 0.f;
    unsigned      m_last_fan_speed = 0;
    int           current_temp = -1;
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
    const float   m_default_analyzer_line_width;
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
    float         m_used_filament_length = 0.f;
    GCodeFlavor   m_gcode_flavor;
    const std::vector<WipeTower::FilamentParameters>& m_filpar;

	std::string   set_format_X(float x)
	{
		char buf[64];
		sprintf(buf, " X%.3f", x);
		m_current_pos.x() = x;
		return buf;
	}

	std::string   set_format_Y(float y) {
		char buf[64];
		sprintf(buf, " Y%.3f", y);
		m_current_pos.y() = y;
		return buf;
	}

	std::string   set_format_Z(float z) {
		char buf[64];
		sprintf(buf, " Z%.3f", z);
		return buf;
	}

	std::string   set_format_E(float e) {
		char buf[64];
		sprintf(buf, " E%.4f", e);
		return buf;
	}

	std::string   set_format_F(float f) {
		char buf[64];
		sprintf(buf, " F%d", int(floor(f + 0.5f)));
		m_current_feedrate = f;
		return buf;
	}

	WipeTowerWriter& operator=(const WipeTowerWriter &rhs);

	// Rotate the point around center of the wipe tower about given angle (in degrees)
	Vec2f rotate(Vec2f pt) const
	{
		pt.x() -= m_wipe_tower_width / 2.f;
		pt.y() += m_y_shift - m_wipe_tower_depth / 2.f;
	    double angle = m_internal_angle * float(M_PI/180.);
	    double c = cos(angle);
	    double s = sin(angle);
	    return Vec2f(float(pt.x() * c - pt.y() * s) + m_wipe_tower_width / 2.f, float(pt.x() * s + pt.y() * c) + m_wipe_tower_depth / 2.f);
	}

}; // class WipeTowerWriter



WipeTower::ToolChangeResult WipeTower::construct_tcr(WipeTowerWriter& writer,
                                                     bool priming,
                                                     size_t old_tool) const
{
    ToolChangeResult result;
    result.priming      = priming;
    result.initial_tool = int(old_tool);
    result.new_tool     = int(this->m_current_tool);
    result.print_z      = this->m_z_pos;
    result.layer_height = this->m_layer_height;
    result.elapsed_time = writer.elapsed_time();
    result.start_pos    = writer.start_pos_rotated();
    result.end_pos      = priming ? writer.pos() : writer.pos_rotated();
    result.gcode        = std::move(writer.gcode());
    result.extrusions   = std::move(writer.extrusions());
    result.wipe_path    = std::move(writer.wipe_path());
    return result;
}



WipeTower::WipeTower(const PrintConfig& config, const std::vector<std::vector<float>>& wiping_matrix, size_t initial_tool) :
    m_semm(config.single_extruder_multi_material.value),
    m_wipe_tower_pos(config.wipe_tower_x, config.wipe_tower_y),
    m_wipe_tower_width(float(config.wipe_tower_width)),
    m_wipe_tower_rotation_angle(float(config.wipe_tower_rotation_angle)),
    m_y_shift(0.f),
    m_z_pos(0.f),
    m_is_first_layer(false),
    m_bridging(float(config.wipe_tower_bridging)),
    m_no_sparse_layers(config.wipe_tower_no_sparse_layers),
    m_gcode_flavor(config.gcode_flavor),
    m_current_tool(initial_tool),
    wipe_volumes(wiping_matrix)
{
    // If this is a single extruder MM printer, we will use all the SE-specific config values.
    // Otherwise, the defaults will be used to turn off the SE stuff.
    if (m_semm) {
        m_cooling_tube_retraction = float(config.cooling_tube_retraction);
        m_cooling_tube_length     = float(config.cooling_tube_length);
        m_parking_pos_retraction  = float(config.parking_pos_retraction);
        m_extra_loading_move      = float(config.extra_loading_move);
        m_set_extruder_trimpot    = config.high_current_on_filament_swap;
    }
    // Calculate where the priming lines should be - very naive test not detecting parallelograms or custom shapes
    const std::vector<Vec2d>& bed_points = config.bed_shape.values;
    m_bed_shape = (bed_points.size() == 4 ? RectangularBed : CircularBed);
    m_bed_width = float(BoundingBoxf(bed_points).size().x());
    m_bed_bottom_left = m_bed_shape == RectangularBed
                  ? Vec2f(bed_points.front().x(), bed_points.front().y())
                  : Vec2f::Zero();
}



void WipeTower::set_extruder(size_t idx, const PrintConfig& config)
{
    //while (m_filpar.size() < idx+1)   // makes sure the required element is in the vector
    m_filpar.push_back(FilamentParameters());

    m_filpar[idx].material = config.filament_type.get_at(idx);
    m_filpar[idx].temperature = config.temperature.get_at(idx);
    m_filpar[idx].first_layer_temperature = config.first_layer_temperature.get_at(idx);

    // If this is a single extruder MM printer, we will use all the SE-specific config values.
    // Otherwise, the defaults will be used to turn off the SE stuff.
    if (m_semm) {
        m_filpar[idx].loading_speed           = float(config.filament_loading_speed.get_at(idx));
        m_filpar[idx].loading_speed_start     = float(config.filament_loading_speed_start.get_at(idx));
        m_filpar[idx].unloading_speed         = float(config.filament_unloading_speed.get_at(idx));
        m_filpar[idx].unloading_speed_start   = float(config.filament_unloading_speed_start.get_at(idx));
        m_filpar[idx].delay                   = float(config.filament_toolchange_delay.get_at(idx));
        m_filpar[idx].cooling_moves           = config.filament_cooling_moves.get_at(idx);
        m_filpar[idx].cooling_initial_speed   = float(config.filament_cooling_initial_speed.get_at(idx));
        m_filpar[idx].cooling_final_speed     = float(config.filament_cooling_final_speed.get_at(idx));
    }

    m_filpar[idx].filament_area = float((M_PI/4.f) * pow(config.filament_diameter.get_at(idx), 2)); // all extruders are assumed to have the same filament diameter at this point
    float nozzle_diameter = float(config.nozzle_diameter.get_at(idx));
    m_filpar[idx].nozzle_diameter = nozzle_diameter; // to be used in future with (non-single) multiextruder MM

    float max_vol_speed = float(config.filament_max_volumetric_speed.get_at(idx));
    if (max_vol_speed!= 0.f)
        m_filpar[idx].max_e_speed = (max_vol_speed / filament_area());

    m_perimeter_width = nozzle_diameter * Width_To_Nozzle_Ratio; // all extruders are now assumed to have the same diameter

    if (m_semm) {
        std::istringstream stream{config.filament_ramming_parameters.get_at(idx)};
        float speed = 0.f;
        stream >> m_filpar[idx].ramming_line_width_multiplicator >> m_filpar[idx].ramming_step_multiplicator;
        m_filpar[idx].ramming_line_width_multiplicator /= 100;
        m_filpar[idx].ramming_step_multiplicator /= 100;
        while (stream >> speed)
            m_filpar[idx].ramming_speed.push_back(speed);
    }

    m_used_filament_length.resize(std::max(m_used_filament_length.size(), idx + 1)); // makes sure that the vector is big enough so we don't have to check later
}



// Returns gcode to prime the nozzles at the front edge of the print bed.
std::vector<WipeTower::ToolChangeResult> WipeTower::prime(
	// print_z of the first layer.
	float 						first_layer_height, 
	// Extruder indices, in the order to be primed. The last extruder will later print the wipe tower brim, print brim and the object.
	const std::vector<unsigned int> &tools,
	// If true, the last priming are will be the same as the other priming areas, and the rest of the wipe will be performed inside the wipe tower.
	// If false, the last priming are will be large enough to wipe the last extruder sufficiently.
    bool 						/*last_wipe_inside_wipe_tower*/)
{
	this->set_layer(first_layer_height, first_layer_height, tools.size(), true, false);
	this->m_current_tool 		= tools.front();
    
    // The Prusa i3 MK2 has a working space of [0, -2.2] to [250, 210].
    // Due to the XYZ calibration, this working space may shrink slightly from all directions,
    // therefore the homing position is shifted inside the bed by 0.2 in the firmware to [0.2, -2.0].
//	box_coordinates cleaning_box(xy(0.5f, - 1.5f), m_wipe_tower_width, wipe_area);

    float prime_section_width = std::min(0.9f * m_bed_width / tools.size(), 60.f);
    box_coordinates cleaning_box(Vec2f(0.02f * m_bed_width, 0.01f + m_perimeter_width/2.f), prime_section_width, 100.f);
    // In case of a circular bed, place it so it goes across the diameter and hope it will fit
    if (m_bed_shape == CircularBed)
        cleaning_box.translate(-m_bed_width/2 + m_bed_width * 0.03f, -m_bed_width * 0.12f);
    if (m_bed_shape == RectangularBed)
        cleaning_box.translate(m_bed_bottom_left);

    std::vector<ToolChangeResult> results;

    // Iterate over all priming toolchanges and push respective ToolChangeResults into results vector.
    for (size_t idx_tool = 0; idx_tool < tools.size(); ++ idx_tool) {
        size_t old_tool = m_current_tool;

        WipeTowerWriter writer(m_layer_height, m_perimeter_width, m_gcode_flavor, m_filpar);
        writer.set_extrusion_flow(m_extrusion_flow)
              .set_z(m_z_pos)
              .set_initial_tool(m_current_tool);

        // This is the first toolchange - initiate priming
        if (idx_tool == 0) {
            writer.append(";--------------------\n"
                          "; CP PRIMING START\n")
                  .append(";--------------------\n")
                  .speed_override_backup()
                  .speed_override(100)
                  .set_initial_position(Vec2f::Zero())	// Always move to the starting position
                  .travel(cleaning_box.ld, 7200);
            if (m_set_extruder_trimpot)
                writer.set_extruder_trimpot(750); 			// Increase the extruder driver current to allow fast ramming.
        }
        else
            writer.set_initial_position(results.back().end_pos);


        unsigned int tool = tools[idx_tool];
        m_left_to_right = true;
        toolchange_Change(writer, tool, m_filpar[tool].material); // Select the tool, set a speed override for soluble and flex materials.
        toolchange_Load(writer, cleaning_box); // Prime the tool.
        if (idx_tool + 1 == tools.size()) {
            // Last tool should not be unloaded, but it should be wiped enough to become of a pure color.
            toolchange_Wipe(writer, cleaning_box, wipe_volumes[tools[idx_tool-1]][tool]);
        } else {
            // Ram the hot material out of the melt zone, retract the filament into the cooling tubes and let it cool.
            //writer.travel(writer.x(), writer.y() + m_perimeter_width, 7200);
            toolchange_Wipe(writer, cleaning_box , 20.f);
            box_coordinates box = cleaning_box;
            box.translate(0.f, writer.y() - cleaning_box.ld.y() + m_perimeter_width);
            toolchange_Unload(writer, box , m_filpar[m_current_tool].material, m_filpar[tools[idx_tool + 1]].first_layer_temperature);
            cleaning_box.translate(prime_section_width, 0.f);
            writer.travel(cleaning_box.ld, 7200);
        }
        ++ m_num_tool_changes;


        // Ask our writer about how much material was consumed:
        if (m_current_tool < m_used_filament_length.size())
            m_used_filament_length[m_current_tool] += writer.get_and_reset_used_filament_length();

        // This is the last priming toolchange - finish priming
        if (idx_tool+1 == tools.size()) {
            // Reset the extruder current to a normal value.
            if (m_set_extruder_trimpot)
                writer.set_extruder_trimpot(550);
            writer.speed_override_restore()
                  .feedrate(6000)
                  .flush_planner_queue()
                  .reset_extruder()
                  .append("; CP PRIMING END\n"
                          ";------------------\n"
                          "\n\n");
        }

        results.emplace_back(construct_tcr(writer, true, old_tool));
    }

    m_old_temperature = -1; // If the priming is turned off in config, the temperature changing commands will not actually appear
                            // in the output gcode - we should not remember emitting them (we will output them twice in the worst case)

	// so that tool_change() will know to extrude the wipe tower brim:
	m_print_brim = true;

	return results;
}

WipeTower::ToolChangeResult WipeTower::tool_change(size_t tool)
{
	if ( m_print_brim )
		return toolchange_Brim();

    size_t old_tool = m_current_tool;

	float wipe_area = 0.f;
	bool last_change_in_layer = false;
	float wipe_volume = 0.f;
	
	// Finds this toolchange info
	if (tool != (unsigned int)(-1))
	{
		for (const auto &b : m_layer_info->tool_changes)
			if ( b.new_tool == tool ) {
				wipe_volume = b.wipe_volume;
				if (tool == m_layer_info->tool_changes.back().new_tool)
					last_change_in_layer = true;
				wipe_area = b.required_depth * m_layer_info->extra_spacing;
				break;
			}
	}
	else {
		// Otherwise we are going to Unload only. And m_layer_info would be invalid.
	}

	box_coordinates cleaning_box(
		Vec2f(m_perimeter_width / 2.f, m_perimeter_width / 2.f),
		m_wipe_tower_width - m_perimeter_width,
        (tool != (unsigned int)(-1) ? /*m_layer_info->depth*/wipe_area+m_depth_traversed-0.5f*m_perimeter_width
                                    : m_wipe_tower_depth-m_perimeter_width));

	WipeTowerWriter writer(m_layer_height, m_perimeter_width, m_gcode_flavor, m_filpar);
	writer.set_extrusion_flow(m_extrusion_flow)
		.set_z(m_z_pos)
		.set_initial_tool(m_current_tool)
		.set_y_shift(m_y_shift + (tool!=(unsigned int)(-1) && (m_current_shape == SHAPE_REVERSED && !m_peters_wipe_tower) ? m_layer_info->depth - m_layer_info->toolchanges_depth(): 0.f))
		.append(";--------------------\n"
				"; CP TOOLCHANGE START\n")
		.comment_with_value(" toolchange #", m_num_tool_changes + 1); // the number is zero-based

    if (tool != (unsigned)(-1))
        writer.append(std::string("; material : " + (m_current_tool < m_filpar.size() ? m_filpar[m_current_tool].material : "(NONE)") + " -> " + m_filpar[tool].material + "\n").c_str())
              .append(";--------------------\n");

    writer.speed_override_backup();
	writer.speed_override(100);

	Vec2f initial_position = cleaning_box.ld + Vec2f(0.f, m_depth_traversed);
    writer.set_initial_position(initial_position, m_wipe_tower_width, m_wipe_tower_depth, m_internal_rotation);

    // Increase the extruder driver current to allow fast ramming.
	if (m_set_extruder_trimpot)
		writer.set_extruder_trimpot(750);

    // Ram the hot material out of the melt zone, retract the filament into the cooling tubes and let it cool.
    if (tool != (unsigned int)-1){ 			// This is not the last change.
        toolchange_Unload(writer, cleaning_box, m_filpar[m_current_tool].material,
                          m_is_first_layer ? m_filpar[tool].first_layer_temperature : m_filpar[tool].temperature);
        toolchange_Change(writer, tool, m_filpar[tool].material); // Change the tool, set a speed override for soluble and flex materials.
        toolchange_Load(writer, cleaning_box);
        writer.travel(writer.x(), writer.y()-m_perimeter_width); // cooling and loading were done a bit down the road
        toolchange_Wipe(writer, cleaning_box, wipe_volume);     // Wipe the newly loaded filament until the end of the assigned wipe area.
        ++ m_num_tool_changes;
    } else
        toolchange_Unload(writer, cleaning_box, m_filpar[m_current_tool].material, m_filpar[m_current_tool].temperature);

    m_depth_traversed += wipe_area;

    if (last_change_in_layer) {// draw perimeter line
        writer.set_y_shift(m_y_shift);
        if (m_peters_wipe_tower)
            writer.rectangle(Vec2f::Zero(), m_layer_info->depth + 3*m_perimeter_width, m_wipe_tower_depth);
        else {
            writer.rectangle(Vec2f::Zero(), m_wipe_tower_width, m_layer_info->depth + m_perimeter_width);
            if (layer_finished()) { // no finish_layer will be called, we must wipe the nozzle
                writer.add_wipe_point(writer.x(), writer.y())
                      .add_wipe_point(writer.x()> m_wipe_tower_width / 2.f ? 0.f : m_wipe_tower_width, writer.y());

            }
        }
    }

	if (m_set_extruder_trimpot)
		writer.set_extruder_trimpot(550);    // Reset the extruder current to a normal value.
	writer.speed_override_restore();
    writer.feedrate(6000)
          .flush_planner_queue()
          .reset_extruder()
          .append("; CP TOOLCHANGE END\n"
                  ";------------------\n"
                  "\n\n");

    // Ask our writer about how much material was consumed:
    if (m_current_tool < m_used_filament_length.size())
        m_used_filament_length[m_current_tool] += writer.get_and_reset_used_filament_length();

   return construct_tcr(writer, false, old_tool);
}

WipeTower::ToolChangeResult WipeTower::toolchange_Brim(bool sideOnly, float y_offset)
{
    size_t old_tool = m_current_tool;

	const box_coordinates wipeTower_box(
		Vec2f::Zero(),
		m_wipe_tower_width,
		m_wipe_tower_depth);

	WipeTowerWriter writer(m_layer_height, m_perimeter_width, m_gcode_flavor, m_filpar);
	writer.set_extrusion_flow(m_extrusion_flow * 1.1f)
		  .set_z(m_z_pos) // Let the writer know the current Z position as a base for Z-hop.
		  .set_initial_tool(m_current_tool)
		  .append(";-------------------------------------\n"
				  "; CP WIPE TOWER FIRST LAYER BRIM START\n");

	Vec2f initial_position = wipeTower_box.lu - Vec2f(m_perimeter_width * 6.f, 0);
	writer.set_initial_position(initial_position, m_wipe_tower_width, m_wipe_tower_depth, m_internal_rotation);

    writer.extrude_explicit(wipeTower_box.ld - Vec2f(m_perimeter_width * 6.f, 0), // Prime the extruder left of the wipe tower.
        1.5f * m_extrusion_flow * (wipeTower_box.lu.y() - wipeTower_box.ld.y()), 2400);

    // The tool is supposed to be active and primed at the time when the wipe tower brim is extruded.
    // Extrude 4 rounds of a brim around the future wipe tower.
    box_coordinates box(wipeTower_box);
    // the brim shall have 'normal' spacing with no extra void space
    float spacing = m_perimeter_width - m_layer_height*float(1.-M_PI_4);
    for (size_t i = 0; i < 4; ++ i) {
        box.expand(spacing);
        writer.travel (box.ld, 7000)
              .extrude(box.lu, 2100).extrude(box.ru)
              .extrude(box.rd      ).extrude(box.ld);
    }

    box.expand(-spacing);
    writer.add_wipe_point(writer.x(), writer.y())
          .add_wipe_point(box.ld)
          .add_wipe_point(box.rd);

    writer.append("; CP WIPE TOWER FIRST LAYER BRIM END\n"
                  ";-----------------------------------\n");

    // Save actual brim width to be later passed to the Print object, which will use it
    // for skirt calculation and pass it to GLCanvas for precise preview box
    m_wipe_tower_brim_width = wipeTower_box.ld.x() - box.ld.x() + spacing/2.f;

    m_print_brim = false;  // Mark the brim as extruded

    // Ask our writer about how much material was consumed:
    if (m_current_tool < m_used_filament_length.size())
    	m_used_filament_length[m_current_tool] += writer.get_and_reset_used_filament_length();

    return construct_tcr(writer, false, old_tool);
}



// Ram the hot material out of the melt zone, retract the filament into the cooling tubes and let it cool.
void WipeTower::toolchange_Unload(
	WipeTowerWriter &writer,
	const box_coordinates 	&cleaning_box,
	const std::string&		 current_material,
	const int 				 new_temperature)
{
	float xl = cleaning_box.ld.x() + 1.f * m_perimeter_width;
	float xr = cleaning_box.rd.x() - 1.f * m_perimeter_width;
	
	const float line_width = m_perimeter_width * m_filpar[m_current_tool].ramming_line_width_multiplicator;       // desired ramming line thickness
	const float y_step = line_width * m_filpar[m_current_tool].ramming_step_multiplicator * m_extra_spacing; // spacing between lines in mm

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING
    writer.append("; CP TOOLCHANGE UNLOAD\n")
        .change_analyzer_line_width(line_width);
#else
    writer.append("; CP TOOLCHANGE UNLOAD\n");
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING

	unsigned i = 0;										// iterates through ramming_speed
	m_left_to_right = true;								// current direction of ramming
	float remaining = xr - xl ;							// keeps track of distance to the next turnaround
	float e_done = 0;									// measures E move done from each segment

	writer.travel(xl, cleaning_box.ld.y() + m_depth_traversed + y_step/2.f ); // move to starting position

    // if the ending point of the ram would end up in mid air, align it with the end of the wipe tower:
    if (m_layer_info > m_plan.begin() && m_layer_info < m_plan.end() && (m_layer_info-1!=m_plan.begin() || !m_adhesion )) {

        // this is y of the center of previous sparse infill border
        float sparse_beginning_y = 0.f;
        if (m_current_shape == SHAPE_REVERSED)
            sparse_beginning_y += ((m_layer_info-1)->depth - (m_layer_info-1)->toolchanges_depth())
                                      - ((m_layer_info)->depth-(m_layer_info)->toolchanges_depth()) ;
        else
            sparse_beginning_y += (m_layer_info-1)->toolchanges_depth() + m_perimeter_width;

        float sum_of_depths = 0.f;
        for (const auto& tch : m_layer_info->tool_changes) {  // let's find this toolchange
            if (tch.old_tool == m_current_tool) {
                sum_of_depths += tch.ramming_depth;
                float ramming_end_y = sum_of_depths;
                ramming_end_y -= (y_step/m_extra_spacing-m_perimeter_width) / 2.f;   // center of final ramming line

                if ( (m_current_shape == SHAPE_REVERSED   && ramming_end_y < sparse_beginning_y - 0.5f*m_perimeter_width  ) ||
                     (m_current_shape == SHAPE_NORMAL && ramming_end_y > sparse_beginning_y + 0.5f*m_perimeter_width  )  )
                {
                    writer.extrude(xl + tch.first_wipe_line-1.f*m_perimeter_width,writer.y());
                    remaining -= tch.first_wipe_line-1.f*m_perimeter_width;
                }
                break;
            }
            sum_of_depths += tch.required_depth;
        }
    }

    writer.disable_linear_advance();

    // now the ramming itself:
    while (i < m_filpar[m_current_tool].ramming_speed.size())
    {
        const float x = volume_to_length(m_filpar[m_current_tool].ramming_speed[i] * 0.25f, line_width, m_layer_height);
        const float e = m_filpar[m_current_tool].ramming_speed[i] * 0.25f / filament_area(); // transform volume per sec to E move;
        const float dist = std::min(x - e_done, remaining);		  // distance to travel for either the next 0.25s, or to the next turnaround
        const float actual_time = dist/x * 0.25f;
        writer.ram(writer.x(), writer.x() + (m_left_to_right ? 1.f : -1.f) * dist, 0.f, 0.f, e * (dist / x), dist / (actual_time / 60.f));
        remaining -= dist;

		if (remaining < WT_EPSILON)	{ // we reached a turning point
			writer.travel(writer.x(), writer.y() + y_step, 7200);
			m_left_to_right = !m_left_to_right;
			remaining = xr - xl;
		}
		e_done += dist; // subtract what was actually done
		if (e_done > x - WT_EPSILON) { // current segment finished
			++i;
			e_done = 0;
		}
	}
	Vec2f end_of_ramming(writer.x(),writer.y());
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING
    writer.change_analyzer_line_width(m_perimeter_width);   // so the next lines are not affected by ramming_line_width_multiplier
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE || ENABLE_GCODE_VIEWER_DATA_CHECKING

    // Retraction:
    float old_x = writer.x();
    float turning_point = (!m_left_to_right ? xl : xr );
    if (m_semm && (m_cooling_tube_retraction != 0 || m_cooling_tube_length != 0)) {
        float total_retraction_distance = m_cooling_tube_retraction + m_cooling_tube_length/2.f - 15.f; // the 15mm is reserved for the first part after ramming
        writer.suppress_preview()
              .retract(15.f, m_filpar[m_current_tool].unloading_speed_start * 60.f) // feedrate 5000mm/min = 83mm/s
              .retract(0.70f * total_retraction_distance, 1.0f * m_filpar[m_current_tool].unloading_speed * 60.f)
              .retract(0.20f * total_retraction_distance, 0.5f * m_filpar[m_current_tool].unloading_speed * 60.f)
              .retract(0.10f * total_retraction_distance, 0.3f * m_filpar[m_current_tool].unloading_speed * 60.f)
              .resume_preview();
    }
    // Wipe tower should only change temperature with single extruder MM. Otherwise, all temperatures should
    // be already set and there is no need to change anything. Also, the temperature could be changed
    // for wrong extruder.
    if (m_semm) {
        if (new_temperature != 0 && (new_temperature != m_old_temperature || m_is_first_layer) ) { 	// Set the extruder temperature, but don't wait.
            // If the required temperature is the same as last time, don't emit the M104 again (if user adjusted the value, it would be reset)
            // However, always change temperatures on the first layer (this is to avoid issues with priming lines turned off).
            writer.set_extruder_temp(new_temperature, false);
            m_old_temperature = new_temperature;
        }
    }

    // Cooling:
    const int& number_of_moves = m_filpar[m_current_tool].cooling_moves;
    if (number_of_moves > 0) {
        const float& initial_speed = m_filpar[m_current_tool].cooling_initial_speed;
        const float& final_speed   = m_filpar[m_current_tool].cooling_final_speed;

        float speed_inc = (final_speed - initial_speed) / (2.f * number_of_moves - 1.f);

        writer.suppress_preview()
              .travel(writer.x(), writer.y() + y_step);
        old_x = writer.x();
        turning_point = xr-old_x > old_x-xl ? xr : xl;
        for (int i=0; i<number_of_moves; ++i) {
            float speed = initial_speed + speed_inc * 2*i;
            writer.load_move_x_advanced(turning_point, m_cooling_tube_length, speed);
            speed += speed_inc;
            writer.load_move_x_advanced(old_x, -m_cooling_tube_length, speed);
        }
    }

    // let's wait is necessary:
    writer.wait(m_filpar[m_current_tool].delay);
    // we should be at the beginning of the cooling tube again - let's move to parking position:
    writer.retract(-m_cooling_tube_length/2.f+m_parking_pos_retraction-m_cooling_tube_retraction, 2000);

	// this is to align ramming and future wiping extrusions, so the future y-steps can be uniform from the start:
    // the perimeter_width will later be subtracted, it is there to not load while moving over just extruded material
	writer.travel(end_of_ramming.x(), end_of_ramming.y() + (y_step/m_extra_spacing-m_perimeter_width) / 2.f + m_perimeter_width, 2400.f);

	writer.resume_preview()
		  .flush_planner_queue();
}

// Change the tool, set a speed override for soluble and flex materials.
void WipeTower::toolchange_Change(
	WipeTowerWriter &writer,
    const size_t 	new_tool,
    const std::string&  new_material)
{
    // Ask the writer about how much of the old filament we consumed:
    if (m_current_tool < m_used_filament_length.size())
    	m_used_filament_length[m_current_tool] += writer.get_and_reset_used_filament_length();

    // This is where we want to place the custom gcodes. We will use placeholders for this.
    // These will be substituted by the actual gcodes when the gcode is generated.
    writer.append("[end_filament_gcode]\n");
    writer.append("[toolchange_gcode]\n");

    // Travel to where we assume we are. Custom toolchange or some special T code handling (parking extruder etc)
    // gcode could have left the extruder somewhere, we cannot just start extruding. We should also inform the
    // postprocessor that we absolutely want to have this in the gcode, even if it thought it is the same as before.
    Vec2f current_pos = writer.pos_rotated();
    writer.append(std::string("G1 X") + std::to_string(current_pos.x()) +  " Y" + std::to_string(current_pos.y()) + never_skip_tag() + "\n");

    // The toolchange Tn command will be inserted later, only in case that the user does
    // not provide a custom toolchange gcode.
	writer.set_tool(new_tool); // This outputs nothing, the writer just needs to know the tool has changed.
    writer.append("[start_filament_gcode]\n");

	writer.flush_planner_queue();
	m_current_tool = new_tool;
}

void WipeTower::toolchange_Load(
	WipeTowerWriter &writer,
	const box_coordinates  &cleaning_box)
{
    if (m_semm && (m_parking_pos_retraction != 0 || m_extra_loading_move != 0)) {
        float xl = cleaning_box.ld.x() + m_perimeter_width * 0.75f;
        float xr = cleaning_box.rd.x() - m_perimeter_width * 0.75f;
        float oldx = writer.x();	// the nozzle is in place to do the first wiping moves, we will remember the position

        // Load the filament while moving left / right, so the excess material will not create a blob at a single position.
        float turning_point = ( oldx-xl < xr-oldx ? xr : xl );
        float edist = m_parking_pos_retraction+m_extra_loading_move;

        writer.append("; CP TOOLCHANGE LOAD\n")
              .suppress_preview()
              .load(0.2f * edist, 60.f * m_filpar[m_current_tool].loading_speed_start)
              .load_move_x_advanced(turning_point, 0.7f * edist,        m_filpar[m_current_tool].loading_speed)  // Fast phase
              .load_move_x_advanced(oldx,          0.1f * edist, 0.1f * m_filpar[m_current_tool].loading_speed)  // Super slow*/

              .travel(oldx, writer.y()) // in case last move was shortened to limit x feedrate
              .resume_preview();

        // Reset the extruder current to the normal value.
        if (m_set_extruder_trimpot)
            writer.set_extruder_trimpot(550);
    }
}

// Wipe the newly loaded filament until the end of the assigned wipe area.
void WipeTower::toolchange_Wipe(
	WipeTowerWriter &writer,
	const box_coordinates  &cleaning_box,
	float wipe_volume)
{
	// Increase flow on first layer, slow down print.
	writer.set_extrusion_flow(m_extrusion_flow * (m_is_first_layer ? 1.18f : 1.f))
		  .append("; CP TOOLCHANGE WIPE\n");
	float wipe_coeff = m_is_first_layer ? 0.5f : 1.f;
	const float& xl = cleaning_box.ld.x();
	const float& xr = cleaning_box.rd.x();

	// Variables x_to_wipe and traversed_x are here to be able to make sure it always wipes at least
    //   the ordered volume, even if it means violating the box. This can later be removed and simply
    // wipe until the end of the assigned area.

	float x_to_wipe = volume_to_length(wipe_volume, m_perimeter_width, m_layer_height);
	float dy = m_extra_spacing*m_perimeter_width;
	float wipe_speed = 1600.f;

    // if there is less than 2.5*m_perimeter_width to the edge, advance straightaway (there is likely a blob anyway)
    if ((m_left_to_right ? xr-writer.x() : writer.x()-xl) < 2.5f*m_perimeter_width) {
        writer.travel((m_left_to_right ? xr-m_perimeter_width : xl+m_perimeter_width),writer.y()+dy);
        m_left_to_right = !m_left_to_right;
    }
    
    // now the wiping itself:
	for (int i = 0; true; ++i)	{
		if (i!=0) {
			if (wipe_speed < 1610.f) wipe_speed = 1800.f;
			else if (wipe_speed < 1810.f) wipe_speed = 2200.f;
			else if (wipe_speed < 2210.f) wipe_speed = 4200.f;
			else wipe_speed = std::min(4800.f, wipe_speed + 50.f);
		}

		float traversed_x = writer.x();
		if (m_left_to_right)
            writer.extrude(xr - (i % 4 == 0 ? 0 : 1.5f*m_perimeter_width), writer.y(), wipe_speed * wipe_coeff);
		else
            writer.extrude(xl + (i % 4 == 1 ? 0 : 1.5f*m_perimeter_width), writer.y(), wipe_speed * wipe_coeff);

        if (writer.y()+float(EPSILON) > cleaning_box.lu.y()-0.5f*m_perimeter_width)
            break;		// in case next line would not fit

		traversed_x -= writer.x();
        x_to_wipe -= std::abs(traversed_x);
		if (x_to_wipe < WT_EPSILON) {
            writer.travel(m_left_to_right ? xl + 1.5f*m_perimeter_width : xr - 1.5f*m_perimeter_width, writer.y(), 7200);
			break;
		}
		// stepping to the next line:
        writer.extrude(writer.x() + (i % 4 == 0 ? -1.f : (i % 4 == 1 ? 1.f : 0.f)) * 1.5f*m_perimeter_width, writer.y() + dy);
		m_left_to_right = !m_left_to_right;
	}

    // this is neither priming nor not the last toolchange on this layer - we are
    // going back to the model - wipe the nozzle.
    if (m_layer_info != m_plan.end() && m_current_tool != m_layer_info->tool_changes.back().new_tool) {
        m_left_to_right = !m_left_to_right;
        writer.add_wipe_point(writer.x(), writer.y())
              .add_wipe_point(writer.x(), writer.y() - dy)
              .add_wipe_point(m_left_to_right ? m_wipe_tower_width : 0.f, writer.y() - dy);

    }

	writer.set_extrusion_flow(m_extrusion_flow); // Reset the extrusion flow.
}




WipeTower::ToolChangeResult WipeTower::finish_layer()
{
	// This should only be called if the layer is not finished yet.
	// Otherwise the caller would likely travel to the wipe tower in vain.
	assert(! this->layer_finished());

    size_t old_tool = m_current_tool;

	WipeTowerWriter writer(m_layer_height, m_perimeter_width, m_gcode_flavor, m_filpar);
	writer.set_extrusion_flow(m_extrusion_flow)
		.set_z(m_z_pos)
		.set_initial_tool(m_current_tool)
		.set_y_shift(m_y_shift - (m_current_shape == SHAPE_REVERSED && !m_peters_wipe_tower ? m_layer_info->toolchanges_depth() : 0.f))
		.append(";--------------------\n"
				"; CP EMPTY GRID START\n")
		.comment_with_value(" layer #", m_num_layer_changes + 1);

	// Slow down on the 1st layer.
	float speed_factor = m_is_first_layer ? 0.5f : 1.f;
	float current_depth = m_layer_info->depth - m_layer_info->toolchanges_depth();
	box_coordinates fill_box(Vec2f(m_perimeter_width, m_depth_traversed + m_perimeter_width),
							 m_wipe_tower_width - 2 * m_perimeter_width, current_depth-m_perimeter_width);


    writer.set_initial_position((m_left_to_right ? fill_box.ru : fill_box.lu), // so there is never a diagonal travel
                                 m_wipe_tower_width, m_wipe_tower_depth, m_internal_rotation);

    bool toolchanges_on_layer = m_layer_info->toolchanges_depth() > WT_EPSILON;
	box_coordinates box = fill_box;
    for (int i=0;i<2;++i) {
        if (! toolchanges_on_layer) {
            if (i==0) box.expand(m_perimeter_width);
            else box.expand(-m_perimeter_width);
        }
        else i=2;	// only draw the inner perimeter, outer has been already drawn by tool_change(...)
        writer.rectangle(box.ld, box.rd.x()-box.ld.x(), box.ru.y()-box.rd.y(), 2900*speed_factor);
    }

    // we are in one of the corners, travel to ld along the perimeter:
    if (writer.x() > fill_box.ld.x()+EPSILON) writer.travel(fill_box.ld.x(),writer.y());
    if (writer.y() > fill_box.ld.y()+EPSILON) writer.travel(writer.x(),fill_box.ld.y());

    if (m_is_first_layer && m_adhesion) {
        // Extrude a dense infill at the 1st layer to improve 1st layer adhesion of the wipe tower.
        box.expand(-m_perimeter_width/2.f);
        int nsteps = int(floor((box.lu.y() - box.ld.y()) / (2*m_perimeter_width)));
        float step   = (box.lu.y() - box.ld.y()) / nsteps;
        writer.travel(box.ld - Vec2f(m_perimeter_width/2.f, m_perimeter_width/2.f));
        if (nsteps >= 0)
            for (int i = 0; i < nsteps; ++i)	{
                writer.extrude(box.ld.x()+m_perimeter_width/2.f, writer.y() + 0.5f * step);
                writer.extrude(box.rd.x() - m_perimeter_width / 2.f, writer.y());
                writer.extrude(box.rd.x() - m_perimeter_width / 2.f, writer.y() + 0.5f * step);
                writer.extrude(box.ld.x() + m_perimeter_width / 2.f, writer.y());
            }
        writer.add_wipe_point(writer.x(), writer.y())
              .add_wipe_point(box.rd.x()-m_perimeter_width/2.f,writer.y());
    }
    else {  // Extrude a sparse infill to support the material to be printed above.
        const float dy = (fill_box.lu.y() - fill_box.ld.y() - m_perimeter_width);
        const float left = fill_box.lu.x() + 2*m_perimeter_width;
        const float right = fill_box.ru.x() - 2 * m_perimeter_width;
        if (dy > m_perimeter_width)
        {
            // Extrude an inverse U at the left of the region.
            writer.travel(fill_box.ld + Vec2f(m_perimeter_width * 2, 0.f))
                  .extrude(fill_box.lu + Vec2f(m_perimeter_width * 2, 0.f), 2900 * speed_factor);

            const int n = 1+int((right-left)/m_bridging);
            const float dx = (right-left)/n;
            for (int i=1;i<=n;++i) {
                float x=left+dx*i;
                writer.travel(x,writer.y());
                writer.extrude(x,i%2 ? fill_box.rd.y() : fill_box.ru.y());
            }
            writer.add_wipe_point(Vec2f(writer.x(), writer.y()))
                  .add_wipe_point(Vec2f(left, writer.y()));
        }
        else {
            writer.add_wipe_point(Vec2f(writer.x(), writer.y()))
                  .add_wipe_point(Vec2f(right, writer.y()));
        }
    }
    writer.append("; CP EMPTY GRID END\n"
                  ";------------------\n\n\n\n\n\n\n");

    m_depth_traversed = m_wipe_tower_depth-m_perimeter_width;


    // Ask our writer about how much material was consumed.
    // Skip this in case the layer is sparse and config option to not print sparse layers is enabled.
    if (! m_no_sparse_layers || toolchanges_on_layer)
        if (m_current_tool < m_used_filament_length.size())
            m_used_filament_length[m_current_tool] += writer.get_and_reset_used_filament_length();

    return construct_tcr(writer, false, old_tool);
}

// Appends a toolchange into m_plan and calculates neccessary depth of the corresponding box
void WipeTower::plan_toolchange(float z_par, float layer_height_par, unsigned int old_tool, unsigned int new_tool, bool brim, float wipe_volume)
{
	assert(m_plan.empty() || m_plan.back().z <= z_par + WT_EPSILON);	// refuses to add a layer below the last one

	if (m_plan.empty() || m_plan.back().z + WT_EPSILON < z_par) // if we moved to a new layer, we'll add it to m_plan first
		m_plan.push_back(WipeTowerInfo(z_par, layer_height_par));

	if (brim) {	// this toolchange prints brim - we must add it to m_plan, but not to count its depth
		m_plan.back().tool_changes.push_back(WipeTowerInfo::ToolChange(old_tool, new_tool));
		return;
	}

	if (old_tool==new_tool)	// new layer without toolchanges - we are done
		return;

	// this is an actual toolchange - let's calculate depth to reserve on the wipe tower
	float depth = 0.f;			
	float width = m_wipe_tower_width - 3*m_perimeter_width; 
	float length_to_extrude = volume_to_length(0.25f * std::accumulate(m_filpar[old_tool].ramming_speed.begin(), m_filpar[old_tool].ramming_speed.end(), 0.f),
										m_perimeter_width * m_filpar[old_tool].ramming_line_width_multiplicator,
										layer_height_par);
	depth = (int(length_to_extrude / width) + 1) * (m_perimeter_width * m_filpar[old_tool].ramming_line_width_multiplicator * m_filpar[old_tool].ramming_step_multiplicator);
    float ramming_depth = depth;
    length_to_extrude = width*((length_to_extrude / width)-int(length_to_extrude / width)) - width;
    float first_wipe_line = -length_to_extrude;
    length_to_extrude += volume_to_length(wipe_volume, m_perimeter_width, layer_height_par);
    length_to_extrude = std::max(length_to_extrude,0.f);

	depth += (int(length_to_extrude / width) + 1) * m_perimeter_width;
	depth *= m_extra_spacing;

	m_plan.back().tool_changes.push_back(WipeTowerInfo::ToolChange(old_tool, new_tool, depth, ramming_depth, first_wipe_line, wipe_volume));
}



void WipeTower::plan_tower()
{
	// Calculate m_wipe_tower_depth (maximum depth for all the layers) and propagate depths downwards
	m_wipe_tower_depth = 0.f;
	for (auto& layer : m_plan)
		layer.depth = 0.f;
	
    for (int layer_index = int(m_plan.size()) - 1; layer_index >= 0; --layer_index)
	{
		float this_layer_depth = std::max(m_plan[layer_index].depth, m_plan[layer_index].toolchanges_depth());
		m_plan[layer_index].depth = this_layer_depth;
		
		if (this_layer_depth > m_wipe_tower_depth - m_perimeter_width)
			m_wipe_tower_depth = this_layer_depth + m_perimeter_width;

		for (int i = layer_index - 1; i >= 0 ; i--)
		{
			if (m_plan[i].depth - this_layer_depth < 2*m_perimeter_width )
				m_plan[i].depth = this_layer_depth;
		}
	}
}

void WipeTower::save_on_last_wipe()
{
    for (m_layer_info=m_plan.begin();m_layer_info<m_plan.end();++m_layer_info) {
        set_layer(m_layer_info->z, m_layer_info->height, 0, m_layer_info->z == m_plan.front().z, m_layer_info->z == m_plan.back().z);
        if (m_layer_info->tool_changes.size()==0)   // we have no way to save anything on an empty layer
            continue;

        for (const auto &toolchange : m_layer_info->tool_changes)
            tool_change(toolchange.new_tool);

        float width = m_wipe_tower_width - 3*m_perimeter_width; // width we draw into
        float length_to_save = 2*(m_wipe_tower_width+m_wipe_tower_depth) + (!layer_finished() ? finish_layer().total_extrusion_length_in_plane() : 0.f);
        float length_to_wipe = volume_to_length(m_layer_info->tool_changes.back().wipe_volume,
                              m_perimeter_width,m_layer_info->height)  - m_layer_info->tool_changes.back().first_wipe_line - length_to_save;

        length_to_wipe = std::max(length_to_wipe,0.f);
        float depth_to_wipe = m_perimeter_width * (std::floor(length_to_wipe/width) + ( length_to_wipe > 0.f ? 1.f : 0.f ) ) * m_extra_spacing;

        //depth += (int(length_to_extrude / width) + 1) * m_perimeter_width;
        m_layer_info->tool_changes.back().required_depth = m_layer_info->tool_changes.back().ramming_depth + depth_to_wipe;
    }
}

// Processes vector m_plan and calls respective functions to generate G-code for the wipe tower
// Resulting ToolChangeResults are appended into vector "result"
void WipeTower::generate(std::vector<std::vector<WipeTower::ToolChangeResult>> &result)
{
	if (m_plan.empty())

        return;

    m_extra_spacing = 1.f;

	plan_tower();
    for (int i=0;i<5;++i) {
        save_on_last_wipe();
        plan_tower();
    }

	if (m_peters_wipe_tower)
			make_wipe_tower_square();

    m_layer_info = m_plan.begin();

    // we don't know which extruder to start with - we'll set it according to the first toolchange
    for (const auto& layer : m_plan) {
        if (!layer.tool_changes.empty()) {
            m_current_tool = layer.tool_changes.front().old_tool;
            break;
        }
    }

    for (auto& used : m_used_filament_length) // reset used filament stats
        used = 0.f;

    m_old_temperature = -1; // reset last temperature written in the gcode

    std::vector<WipeTower::ToolChangeResult> layer_result;
	for (auto layer : m_plan)
	{
		set_layer(layer.z,layer.height,0,layer.z == m_plan.front().z,layer.z == m_plan.back().z);
		if (m_peters_wipe_tower)
			m_internal_rotation += 90.f;
		else
            m_internal_rotation += 180.f;

		if (!m_peters_wipe_tower && m_layer_info->depth < m_wipe_tower_depth - m_perimeter_width)
			m_y_shift = (m_wipe_tower_depth-m_layer_info->depth-m_perimeter_width)/2.f;

		for (const auto &toolchange : layer.tool_changes)
            layer_result.emplace_back(tool_change(toolchange.new_tool));

		if (! layer_finished()) {
            auto finish_layer_toolchange = finish_layer();
            if ( ! layer.tool_changes.empty() ) { // we will merge it to the last toolchange
                auto& last_toolchange = layer_result.back();
                if (last_toolchange.end_pos != finish_layer_toolchange.start_pos) {
                    char buf[2048];     // Add a travel move from tc1.end_pos to tc2.start_pos.
					sprintf(buf, "G1 X%.3f Y%.3f F7200\n", finish_layer_toolchange.start_pos.x(), finish_layer_toolchange.start_pos.y());
					last_toolchange.gcode += buf;
				}
                last_toolchange.gcode += finish_layer_toolchange.gcode;
                last_toolchange.extrusions.insert(last_toolchange.extrusions.end(), finish_layer_toolchange.extrusions.begin(), finish_layer_toolchange.extrusions.end());
                last_toolchange.end_pos = finish_layer_toolchange.end_pos;
                last_toolchange.wipe_path = finish_layer_toolchange.wipe_path;
            }
            else
                layer_result.emplace_back(std::move(finish_layer_toolchange));
        }

		result.emplace_back(std::move(layer_result));
		m_is_first_layer = false;
	}
}

void WipeTower::make_wipe_tower_square()
{
	const float width = m_wipe_tower_width - 3 * m_perimeter_width;
	const float depth = m_wipe_tower_depth - m_perimeter_width;
	// area that we actually print into is width*depth
	float side = sqrt(depth * width);

	m_wipe_tower_width = side + 3 * m_perimeter_width;
	m_wipe_tower_depth = side + 2 * m_perimeter_width;
	// For all layers, find how depth changed and update all toolchange depths
	for (auto &lay : m_plan)
	{
		side = sqrt(lay.depth * width);
		float width_ratio = width / side;

		//lay.extra_spacing = width_ratio;
		for (auto &tch : lay.tool_changes)
			tch.required_depth *= width_ratio;
	}

	plan_tower();				// propagates depth downwards again (width has changed)
	for (auto& lay : m_plan)	// depths set, now the spacing
		lay.extra_spacing = lay.depth / lay.toolchanges_depth();
}


} // namespace Slic3r
