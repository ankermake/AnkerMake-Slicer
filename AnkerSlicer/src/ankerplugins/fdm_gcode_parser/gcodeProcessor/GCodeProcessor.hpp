#ifndef slic3r_GCodeProcessor_hpp_
#define slic3r_GCodeProcessor_hpp_

#include "GCodeReader.hpp"
//#include "libslic3r/Point.hpp"
//#include "libslic3r/PrintConfig.hpp"
//#include "libslic3r/CustomGCode.hpp"
//#include "libslic3r/ExtrusionEntity.hpp"
#include <system_error>
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <Eigen/Geometry>
#include "Exception.hpp"
#define L(s) (s)
#define M_PI       3.14159265358979323846   // pi

namespace Anker {
    static constexpr double EPSILON = 1e-4;

    typedef Eigen::Matrix<double, 2, 1, Eigen::DontAlign> Vec2d;
    typedef std::vector<Vec2d>                              Pointfs;
    typedef Eigen::Matrix<float, 3, 1, Eigen::DontAlign> Vec3f;
    typedef Eigen::Matrix<int32_t, 3, 1, Eigen::DontAlign> Vec3i32;
    typedef Eigen::Transform<double, 3, Eigen::Affine, Eigen::DontAlign> Transform3d;
    typedef Eigen::Matrix<double,   3, 1, Eigen::DontAlign> Vec3d;  //enum &
    namespace CustomGCode {
    enum Type{
        ColorChange,
        PausePrint,
        ToolChange,
        Template,
        Custom
       };
    }
    enum ExtrusionRole : uint8_t {
        erNone,
        erPerimeter,
        erExternalPerimeter,
        erOverhangPerimeter,
        erInternalInfill,
        erSolidInfill,
        erTopSolidInfill,
        erIroning,
        erBridgeInfill,
        erInternalBridgeInfill,
        erThinWall,
        erGapFill,
        erSkirt,
        erSupportMaterial,
        erSupportMaterialInterface,
        erWipeTower,
        erMilling,
        erCustom,
        // Extrusion role for a collection with multiple extrusion roles.
        erMixed,
        erCount
    };
    enum GCodeFlavor : uint8_t {
        gcfRepRap,
        gcfSprinter,
        gcfRepetier,
        gcfTeacup,
        gcfMakerWare,
        gcfMarlin,
        gcfLerdge,
        gcfKlipper,
        gcfSailfish,
        gcfMach3,
        gcfMachinekit,
        gcfSmoothie,
        gcfNoExtrusion,
    };
    enum RemainingTimeType {
        rtM117,
        rtM73,
    };

    enum class MachineLimitsUsage : uint8_t {
        EmitToGCode,
        TimeEstimateOnly,
        Limits,
        Ignore,
        Count,
    };
    enum CopyFileResult {
        SUCCESS = 0,
        FAIL_COPY_FILE,
        FAIL_FILES_DIFFERENT,
        FAIL_RENAMING,
        FAIL_CHECK_ORIGIN_NOT_OPENED,
        FAIL_CHECK_TARGET_NOT_OPENED
    };
    //func
    Anker::ExtrusionRole string_to_role(const std::string_view role);
    bool is_bridge(Anker::ExtrusionRole role);

    //function for utils.hpp
    std::string get_time_dhms(float time_in_secs);
    std::string short_time(const std::string& time);
    std::error_code rename_file(const std::string& from, const std::string& to);
    CopyFileResult copy_file(const std::string& from, const std::string& to, std::string& error_message, const bool with_check);
    //class for printcongfig
    template <class T>
    class ConfigOption {
    public:
        std::vector<T> values;

    public:
        void   clear() { this->values.clear(); }
        size_t size()  const { return this->values.size(); }
        bool   empty() const { return this->values.empty(); }
    };

    class MachineEnvelopeConfig
    {
    public:
        MachineEnvelopeConfig();
    public:
        // Allowing the machine limits to be completely ignored or used just for time estimator.
        ConfigOption<MachineLimitsUsage> machine_limits_usage;
        // M201 X... Y... Z... E... [mm/sec^2]
        ConfigOption<double>              machine_max_acceleration_x;
        ConfigOption<double>              machine_max_acceleration_y;
        ConfigOption<double>              machine_max_acceleration_z;
        ConfigOption<double>              machine_max_acceleration_e;
        // M203 X... Y... Z... E... [mm/sec]
        ConfigOption<double>              machine_max_feedrate_x;
        ConfigOption<double>              machine_max_feedrate_y;
        ConfigOption<double>              machine_max_feedrate_z;
        ConfigOption<double>              machine_max_feedrate_e;
        // M204 S... [mm/sec^2]
        ConfigOption<double>              machine_max_acceleration_extruding;
        // M204 R... [mm/sec^2]
        ConfigOption<double>              machine_max_acceleration_retracting;
        // M204 T... [mm/sec^2]
        ConfigOption<double>              machine_max_acceleration_travel;
        // M205 X... Y... Z... E... [mm/sec]
        ConfigOption<double>              machine_max_jerk_x;
        ConfigOption<double>              machine_max_jerk_y;
        ConfigOption<double>              machine_max_jerk_z;
        ConfigOption<double>              machine_max_jerk_e;
        // M205 T... [mm/sec]
        ConfigOption<double>              machine_min_travel_rate;
        // M205 S... [mm/sec]
        ConfigOption<double>              machine_min_extruding_rate;

        // M900
        ConfigOption<double>              machine_lin_advance_k;
    };

    template<typename T>
    static inline T sqr(T x)
    {
        return x * x;
    }


    template<class T, class O = T>
    using IntegerOnly = std::enable_if_t<std::is_integral<T>::value, O>;
    template<class T, class I, class... Args> // Arbitrary allocator can be used
    IntegerOnly<I, std::vector<T, Args...>> reserve_vector(I capacity)
    {
        std::vector<T, Args...> ret;
        if (capacity > I(0)) ret.reserve(size_t(capacity));

        return ret;
    }
}

namespace Anker {

    enum class EMoveType : unsigned char
    {
        Noop,
        Retract,
        Unretract,
        Tool_change,
        Color_change,
        Pause_Print,
        Custom_GCode,
        Travel,
        Wipe,
        Zlap,
        Extrude,
        Count
    };

    struct PrintEstimatedTimeStatistics
    {
        enum class ETimeMode : unsigned char
        {
            Normal,
            Stealth,
            Count
        };

        struct Mode
        {
            float time;
            std::vector<std::pair<Anker::CustomGCode::Type, std::pair<float, float>>> custom_gcode_times;
            std::vector<std::pair<EMoveType, float>> moves_times;
            std::vector<std::pair<Anker::ExtrusionRole, float>> roles_times;
            std::vector<float> layers_times;

            void reset() {
                time = 0.0f;
                custom_gcode_times.clear();
                moves_times.clear();
                roles_times.clear();
                layers_times.clear();
            }
        };

        std::array<Mode, static_cast<size_t>(ETimeMode::Count)> modes;

        PrintEstimatedTimeStatistics() { reset(); }

        void reset() {
            for (auto m : modes) {
                m.reset();
            }
        }
    };

    class GCodeProcessor
    {
    public:
        static const std::string Extrusion_Role_Tag;
        static const std::string Wipe_Start_Tag;
        static const std::string Wipe_End_Tag;
        static const std::string Height_Tag;
        static const std::string Layer_Change_Tag;
        static const std::string Color_Change_Tag;
        static const std::string Pause_Print_Tag;
        static const std::string Custom_Code_Tag;
        static const std::string First_Line_M73_Placeholder_Tag;
        static const std::string Last_Line_M73_Placeholder_Tag;
        static const std::string Estimated_Printing_Time_Placeholder_Tag;

        static const float Wipe_Width;
        static const float Wipe_Height;

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        static const std::string Width_Tag;
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
#if !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        static const std::string Width_Tag;
#endif // !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        static const std::string Mm3_Per_Mm_Tag;
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

    private:
        using AxisCoords = std::array<double, 4>;
        using ExtruderColors = std::vector<unsigned char>;

        enum class EUnits : unsigned char
        {
            Millimeters,
            Inches
        };

        enum class EPositioningType : unsigned char
        {
            Absolute,
            Relative
        };

        struct CachedPosition
        {
            AxisCoords position; // mm
            float feedrate; // mm/s

            void reset();
        };

        struct CpColor
        {
            unsigned char counter;
            unsigned char current;

            void reset();
        };

    public:
        struct FeedrateProfile
        {
            float entry{ 0.0f }; // mm/s
            float cruise{ 0.0f }; // mm/s
            float exit{ 0.0f }; // mm/s
        };

        struct Trapezoid
        {
            float accelerate_until{ 0.0f }; // mm
            float decelerate_after{ 0.0f }; // mm
            float cruise_feedrate{ 0.0f }; // mm/sec

            float acceleration_time(float entry_feedrate, float acceleration) const;
            float cruise_time() const;
            float deceleration_time(float distance, float acceleration) const;
            float cruise_distance() const;
        };

        struct TimeBlock
        {
            struct Flags
            {
                bool recalculate{ false };
                bool nominal_length{ false };
            };

            EMoveType move_type{ EMoveType::Noop };
            Anker::ExtrusionRole role{ Anker::ExtrusionRole::erNone };
            unsigned int g1_line_id{ 0 };
            unsigned int layer_id{ 0 };
            float distance{ 0.0f }; // mm
            float acceleration{ 0.0f }; // mm/s^2
            float max_entry_speed{ 0.0f }; // mm/s
            float safe_feedrate{ 0.0f }; // mm/s
            Flags flags;
            FeedrateProfile feedrate_profile;
            Trapezoid trapezoid;
            AxisCoords axis_feedrate; // mm/s; add @2022-05-29 by CL

            // Calculates this block's trapezoid
            void calculate_trapezoid();

            float time() const;
        };

    private:
        friend class TimeMachine;
        struct TimeMachine
        {
            struct State
            {
                float feedrate; // mm/s
                float safe_feedrate; // mm/s
                AxisCoords axis_feedrate; // mm/s
                AxisCoords abs_axis_feedrate; // mm/s

                void reset();
            };

            struct CustomGCodeTime
            {
                bool needed;
                float cache;
                std::vector<std::pair<Anker::CustomGCode::Type, float>> times;

                void reset();
            };

            struct G1LinesCacheItem
            {
                unsigned int id;
                float elapsed_time;
            };

            bool enabled;
            float acceleration; // mm/s^2
            // hard limit for the acceleration, to which the firmware will clamp.
            float max_acceleration; // mm/s^2
            float extrude_factor_override_percentage;
            float time; // s
            float time_acceleration;
            Anker::RemainingTimeType remaining_times_type;
            State curr;
            State prev;
            CustomGCodeTime gcode_time;
            std::vector<TimeBlock> blocks;
            std::vector<G1LinesCacheItem> g1_times_cache;
            std::array<float, static_cast<size_t>(EMoveType::Count)> moves_time;
            std::array<float, static_cast<size_t>(Anker::ExtrusionRole::erCount)> roles_time;
            std::vector<float> layers_time;

            GCodeProcessor * gCodeProcessor{nullptr}; //  add @2022-05-29 by CL
            void reset();

            // Simulates firmware st_synchronize() call
            void simulate_st_synchronize(float additional_time = 0.0f);
            void calculate_time(size_t keep_last_n_blocks = 0);
        };

        struct TimeProcessor
        {
            struct Planner
            {
                // Size of the firmware planner queue. The old 8-bit Marlins usually just managed 16 trapezoidal blocks.
                // Let's be conservative and plan for newer boards with more memory.
                // The firmware recalculates last planner_queue_size trapezoidal blocks each time a new block is added.
                // We are not simulating the firmware exactly, we calculate a sequence of blocks once a reasonable number of blocks accumulate.
#if CL_OPT      //  add for test Marlin @2022-09-16 by CL
                static constexpr size_t queue_size = 16; // 64;
                static constexpr size_t refresh_threshold = 16; //  queue_size * 4
#else
                static constexpr size_t queue_size = 64;
                static constexpr size_t refresh_threshold = queue_size * 4;
#endif
            };

            // extruder_id is currently used to correctly calculate filament load / unload times into the total print time.
            // This is currently only really used by the MK3 MMU2:
            // extruder_unloaded = true means no filament is loaded yet, all the filaments are parked in the MK3 MMU2 unit.
            bool extruder_unloaded;
            // whether or not to export post-process the gcode to export lines M73 in it
            bool export_remaining_time_enabled;
            // allow to skip the lines M201/M203/M204/M205 generated by GCode::print_machine_envelope() for non-Normal time estimate mode
            bool machine_envelope_processing_enabled;
            Anker::MachineEnvelopeConfig machine_limits;
            // Additional load / unload times for a filament exchange sequence.
            std::vector<float> filament_load_times;
            std::vector<float> filament_unload_times;
            std::array<TimeMachine, static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count)> machines;

            void reset();

            // post process the file with the given filename to add remaining time lines M73
            void post_process(const std::string& filename);
        };

        struct TrapezoidFeedrateProfile
        {
            
            
            

            float accelerate_until{ 0.0f }; 
            float decelerate_after{ 0.0f }; 

            float entry{ 0.0f };     
            float cruise{ 0.0f };    
            float exit{ 0.0f };      
        };
    public:
        struct MoveVertex
        {
            EMoveType type{ EMoveType::Noop };
            Anker::ExtrusionRole extrusion_role{ Anker::ExtrusionRole::erNone };
            unsigned char extruder_id{ 0 };
            unsigned char cp_color_id{ 0 };
            Vec3f position{ Vec3f::Zero() }; // mm
            float delta_extruder{ 0.0f }; // mm
            float feedrate{ 0.0f }; // mm/s
            float width{ 0.0f }; // mm
            float height{ 0.0f }; // mm
            float mm3_per_mm{ 0.0f };
            float fan_speed{ 0.0f }; // percentage
            float layer_duration{ 0.0f }; // s
            float time{ 0.0f }; // s
            float temperature{ 0.0f }; // deg
            float bed_temperature{ 0.0f }; //deg
            TrapezoidFeedrateProfile motor_process;
            unsigned int g1_line_id{ 0 };
            float volumetric_rate() const { return feedrate * mm3_per_mm; }
        };

        struct Result
        {
            struct SettingsIds
            {
                std::string print;
                std::vector<std::string> filament;
                std::string printer;

                void reset()
                {
                    print = "";
                    filament = std::vector<std::string>();
                    printer = "";
                }
            };
            unsigned int id;
            std::vector<MoveVertex> moves;
            Pointfs bed_shape;
            SettingsIds settings_ids;
            size_t extruders_count;
            std::vector<std::string> extruder_colors;
            std::vector<std::string> filament_colors;
            PrintEstimatedTimeStatistics time_statistics;
            std::vector<int> ai_pic_Layer;
            std::vector<std::array<double,4>> layer_offset;

            std::vector<float> freedrate_range;
            std::vector<float> flow_range;
#if ENABLE_GCODE_VIEWER_STATISTICS
            int64_t time{ 0 };
            void reset()
            {
                time = 0;
                moves = std::vector<MoveVertex>();
                bed_shape = Pointfs();
                extruder_colors = std::vector<std::string>();
                extruders_count = 0;
                settings_ids.reset();
            }
#else
            void reset()
            {
                moves = std::vector<MoveVertex>();
                bed_shape = Pointfs();
                extruder_colors = std::vector<std::string>();
                filament_colors = std::vector<std::string>();
                extruders_count = 0;
                settings_ids.reset();
                freedrate_range = {-1.0f, -1.0f};
                flow_range = {-1.0f,-1.0f};
                std::vector<int>().swap(ai_pic_Layer);
                std::vector<std::array<double,4>>().swap(layer_offset);
//                std::vector<double>().swap(layer_z_offset);
//                ai_pic_Layer.swap();
//                layer_y_offset.swap(std::vector<double>());
            }
#endif // ENABLE_GCODE_VIEWER_STATISTICS
        };

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        struct DataChecker
        {
            struct Error
            {
                float value;
                float tag_value;
                Anker::ExtrusionRole role;
            };

            std::string type;
            float threshold{ 0.01f };
            float last_tag_value{ 0.0f };
            unsigned int count{ 0 };
            std::vector<Error> errors;

            DataChecker(const std::string& type, float threshold)
                : type(type), threshold(threshold)
            {}

            void update(float value, Anker::ExtrusionRole role) {
                if (role != erCustom) {
                    ++count;
                    if (last_tag_value != 0.0f) {
                        if (std::abs(value - last_tag_value) / last_tag_value > threshold)
                            errors.push_back({ value, last_tag_value, role });
                    }
                }
            }

            void reset() { last_tag_value = 0.0f; errors.clear(); count = 0; }

            std::pair<float, float> get_min() const {
                float delta_min = FLT_MAX;
                float perc_min = 0.0f;
                for (const Error& e : errors) {
                    if (delta_min > e.value - e.tag_value) {
                        delta_min = e.value - e.tag_value;
                        perc_min = 100.0f * delta_min / e.tag_value;
                    }
                }
                return { delta_min, perc_min };
            }

            std::pair<float, float> get_max() const {
                float delta_max = -FLT_MAX;
                float perc_max = 0.0f;
                for (const Error& e : errors) {
                    if (delta_max < e.value - e.tag_value) {
                        delta_max = e.value - e.tag_value;
                        perc_max = 100.0f * delta_max / e.tag_value;
                    }
                }
                return { delta_max, perc_max };
            }

            void output() const {
                if (!errors.empty()) {
                    std::cout << type << ":\n";
                    std::cout << "Errors: " << errors.size() << " (" << 100.0f * float(errors.size()) / float(count) << "%)\n";
                    auto [min, perc_min] = get_min();
                    auto [max, perc_max] = get_max();
                    std::cout << "min: " << min << "(" << perc_min << "%) - max: " << max << "(" << perc_max << "%)\n";
                }
            }
        };
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

    private:
        GCodeReader m_parser;

        EUnits m_units;
        EPositioningType m_global_positioning_type;
        EPositioningType m_e_local_positioning_type;
        std::vector<Vec3f> m_extruder_offsets;
        std::vector<std::string> m_extruder_names;
        Anker::GCodeFlavor m_flavor;

        AxisCoords m_start_position; // mm
        AxisCoords m_end_position; // mm
        AxisCoords m_origin; // mm
        CachedPosition m_cached_position;
        bool m_wiping;

        float m_feedrate; // mm/s
        float m_width; // mm
        float m_height; // mm
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        float m_forced_width; // mm
        float m_forced_height; // mm
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        float m_mm3_per_mm;
        float m_fan_speed; // percentage
        Anker::ExtrusionRole m_extrusion_role;
        unsigned char m_extruder_id;
        ExtruderColors m_extruder_colors;
        std::vector<float> m_filament_diameters;
        double m_extruded_last_z;
        unsigned int m_g1_line_id;
        unsigned int m_layer_id;
        CpColor m_cp_color;
        float m_temperature;
        float m_bed_temperature;
#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
        bool m_use_volumetric_e;
#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING

        enum class EProducer
        {
            Unknown,
            PrusaSlicer,
            SuperSlicer,
            Slic3rPE,
            Slic3r,
            Cura,
            Simplify3D,
            CraftWare,
            ideaMaker,
            KissSlicer,
            AnkerMake
        };

        static const std::vector<std::pair<GCodeProcessor::EProducer, std::string>> Producers;
        EProducer m_producer;
        bool m_producers_enabled;

        TimeProcessor m_time_processor;

        Result m_result;
        static unsigned int s_result_id;

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        DataChecker m_mm3_per_mm_compare{ "mm3_per_mm", 0.01f };
        DataChecker m_height_compare{ "height", 0.01f };
        DataChecker m_width_compare{ "width", 0.01f };
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

    public:
        GCodeProcessor();

        //void apply_config(const PrintConfig& config);
        //void apply_config(const DynamicPrintConfig& config);
        void enable_stealth_time_estimator(bool enabled);
        bool is_stealth_time_estimator_enabled() const {
            return m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].enabled;
        }
        void enable_machine_envelope_processing(bool enabled) { m_time_processor.machine_envelope_processing_enabled = enabled; }
        void enable_producers(bool enabled) { m_producers_enabled = enabled; }
        void reset();

        const Result& get_result() const { return m_result; }
        Result&& extract_result() { return std::move(m_result); }

        // Process the gcode contained in the file with the given filename
        // throws CanceledException through print->throw_if_canceled() (sent by the caller as callback).
        void process_file(const std::string& filename, bool apply_postprocess, std::function<void()> cancel_callback = nullptr);
        void process_string(const std::string& gcode, std::function<void()> cancel_callback = nullptr);

        float get_time(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        std::string get_time_dhm(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        std::vector<std::pair<Anker::CustomGCode::Type, std::pair<float, float>>> get_custom_gcode_times(PrintEstimatedTimeStatistics::ETimeMode mode, bool include_remaining) const;

        std::vector<std::pair<EMoveType, float>> get_moves_time(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        std::vector<std::pair<Anker::ExtrusionRole, float>> get_roles_time(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        std::vector<float> get_layers_time(PrintEstimatedTimeStatistics::ETimeMode mode) const;

    private:
        void process_gcode_line(const GCodeReader::GCodeLine& line);

        // Process tags embedded into comments
        void process_tags(const std::string_view comment);
        bool process_producers_tags(const std::string_view comment);
        bool process_prusaslicer_tags(const std::string_view comment);
        bool process_cura_tags(const std::string_view comment);
        bool process_simplify3d_tags(const std::string_view comment);
        bool process_craftware_tags(const std::string_view comment);
        bool process_ideamaker_tags(const std::string_view comment);
        bool process_kissslicer_tags(const std::string_view comment);

        bool detect_producer(const std::string_view comment);

        // Move
        void process_G0(const GCodeReader::GCodeLine& line);
        void process_G1(const GCodeReader::GCodeLine& line);
        void process_G2_G3(const GCodeReader::GCodeLine& line, bool direct);
        void emit_G1_from_G2(Vec2d dest, float de, float f);

        // Retract
        void process_G10(const GCodeReader::GCodeLine& line);

        // Unretract
        void process_G11(const GCodeReader::GCodeLine& line);

        // Set Units to Inches
        void process_G20(const GCodeReader::GCodeLine& line);

        // Set Units to Millimeters
        void process_G21(const GCodeReader::GCodeLine& line);

        // Firmware controlled Retract
        void process_G22(const GCodeReader::GCodeLine& line);

        // Firmware controlled Unretract
        void process_G23(const GCodeReader::GCodeLine& line);

        // Set to Absolute Positioning
        void process_G90(const GCodeReader::GCodeLine& line);

        // Set to Relative Positioning
        void process_G91(const GCodeReader::GCodeLine& line);

        // Set Position
        void process_G92(const GCodeReader::GCodeLine& line);

        // Sleep or Conditional stop
        void process_M1(const GCodeReader::GCodeLine& line);

        // Set extruder to absolute mode
        void process_M82(const GCodeReader::GCodeLine& line);

        // Set extruder to relative mode
        void process_M83(const GCodeReader::GCodeLine& line);

        // Set extruder temp
        void process_M104_M109(const GCodeReader::GCodeLine& line);

        // Set fan speed
        void process_M106(const GCodeReader::GCodeLine& line);

        // Disable fan
        void process_M107(const GCodeReader::GCodeLine& line);

        // Set tool (Sailfish)
        void process_M108(const GCodeReader::GCodeLine& line);

        // Recall stored home offsets
        void process_M132(const GCodeReader::GCodeLine& line);

        // Set tool (MakerWare)
        void process_M135(const GCodeReader::GCodeLine& line);

        // Set bed temp
        void process_M140(const GCodeReader::GCodeLine& line);

        // Set max printing acceleration
        void process_M201(const GCodeReader::GCodeLine& line);

        // Set maximum feedrate
        void process_M203(const GCodeReader::GCodeLine& line);

        // Set default acceleration
        void process_M204(const GCodeReader::GCodeLine& line);

        // Advanced settings
        void process_M205(const GCodeReader::GCodeLine& line);

        // Set extrude factor override percentage
        void process_M221(const GCodeReader::GCodeLine& line);

        // Repetier: Store x, y and z position
        void process_M401(const GCodeReader::GCodeLine& line);

        // Repetier: Go to stored position
        void process_M402(const GCodeReader::GCodeLine& line);

        // Set allowable instantaneous speed change
        void process_M566(const GCodeReader::GCodeLine& line);

        // Unload the current filament into the MK3 MMU2 unit at the end of print.
        void process_M702(const GCodeReader::GCodeLine& line);

        
        void process_M900(const GCodeReader::GCodeLine& line);

        void process_M1024(const GCodeReader::GCodeLine& line);
        // Processes T line (Select Tool)
        void process_T(const GCodeReader::GCodeLine& line);
        void process_T(const std::string_view command);
        void process_T(uint8_t command);
        void process_klipper_ACTIVATE_EXTRUDER(const GCodeReader::GCodeLine& line);

        void store_move_vertex(EMoveType type);

        float minimum_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, float feedrate) const;
        float minimum_travel_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, float feedrate) const;
        float get_axis_max_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const;
        float get_axis_max_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const;
        float get_axis_max_jerk(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const;
        float get_retract_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        float get_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode) const;
        void set_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode, float value);
        float get_filament_load_time(size_t extruder_id);
        float get_filament_unload_time(size_t extruder_id);

        void process_custom_gcode_time(Anker::CustomGCode::Type code);

        // Simulates firmware st_synchronize() call
        void simulate_st_synchronize(float additional_time = 0.0f);

        void update_estimated_times_stats();

        private:
        bool m_z_lap_flag = false;
   };

} /* namespace Slic3r */

#endif /* slic3r_GCodeProcessor_hpp_ */


