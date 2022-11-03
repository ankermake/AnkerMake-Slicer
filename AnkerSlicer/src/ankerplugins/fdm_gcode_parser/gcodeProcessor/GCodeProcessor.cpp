//#include "libslic3r/libslic3r.h"
//#include "libslic3r/Utils.hpp"
//#include "libslic3r/Print.hpp"
#include "GCodeProcessor.hpp"

//#include <boost/algorithm/string/case_conv.hpp>
//#include <boost/log/trivial.hpp>
//#include <boost/format.hpp>
//#include <boost/nowide/fstream.hpp>
//#include <boost/nowide/cstdio.hpp>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <float.h>
#include <assert.h>

#if __has_include(<charconv>)
#include <charconv>
#include <utility>
#endif

#include <chrono>

#include <QDebug>

#if 0 // test for CL
#include "common/utilities/tlogger.h"
using namespace AkUtil;
#else
#define TDebug(msg)
#endif

static const float INCHES_TO_MM = 25.4f;
static const float MMMIN_TO_MMSEC = 1.0f / 60.0f;

static const float DEFAULT_ACCELERATION = 1500.0f; // Prusa Firmware 1_75mm_MK2

namespace Anker {

const std::string GCodeProcessor::Extrusion_Role_Tag = "TYPE:";
const std::string GCodeProcessor::Wipe_Start_Tag     = "WIPE_START";
const std::string GCodeProcessor::Wipe_End_Tag       = "WIPE_END";
const std::string GCodeProcessor::Height_Tag         = "HEIGHT:";
const std::string GCodeProcessor::Layer_Change_Tag   = "LAYER_CHANGE";
const std::string GCodeProcessor::Color_Change_Tag   = "COLOR_CHANGE";
const std::string GCodeProcessor::Pause_Print_Tag    = "PAUSE_PRINT";
const std::string GCodeProcessor::Custom_Code_Tag    = "CUSTOM_GCODE";

const std::string GCodeProcessor::First_Line_M73_Placeholder_Tag          = "; _GP_FIRST_LINE_M73_PLACEHOLDER";
const std::string GCodeProcessor::Last_Line_M73_Placeholder_Tag           = "; _GP_LAST_LINE_M73_PLACEHOLDER";
const std::string GCodeProcessor::Estimated_Printing_Time_Placeholder_Tag = "; _GP_ESTIMATED_PRINTING_TIME_PLACEHOLDER";

const float GCodeProcessor::Wipe_Width = 0.05f;
const float GCodeProcessor::Wipe_Height = 0.05f;

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
const std::string GCodeProcessor::Width_Tag = "WIDTH:";
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
#if !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
const std::string GCodeProcessor::Width_Tag      = "WIDTH:";
#endif // !ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
const std::string GCodeProcessor::Mm3_Per_Mm_Tag = "MM3_PER_MM:";
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

static bool is_valid_extrusion_role(int value)
{
    return (static_cast<int>(Anker::erNone) <= value) && (value <= static_cast<int>(Anker::erMixed));
}

static void set_option_value(Anker::ConfigOption<double>& option, size_t id, float value)
{
    if (id < option.values.size())
        option.values[id] = static_cast<double>(value);
}

static float get_option_value(const Anker::ConfigOption<double>& option, size_t id)
{
    return option.values.empty() ? 0.0f :
                                   ((id < option.values.size()) ? static_cast<float>(option.values[id]) : static_cast<float>(option.values.back()));
}

static float estimated_acceleration_distance(float initial_rate, float target_rate, float acceleration)
{
    return (acceleration == 0.0f) ? 0.0f : (sqr(target_rate) - sqr(initial_rate)) / (2.0f * acceleration);
}

static float intersection_distance(float initial_rate, float final_rate, float acceleration, float distance)
{
    return (acceleration == 0.0f) ? 0.0f : (2.0f * acceleration * distance - sqr(initial_rate) + sqr(final_rate)) / (4.0f * acceleration);
}

static float speed_from_distance(float initial_feedrate, float distance, float acceleration)
{
    // to avoid invalid negative numbers due to numerical errors
    float value = std::max(0.0f, sqr(initial_feedrate) + 2.0f * acceleration * distance);
    return ::sqrt(value);
}

// Calculates the maximum allowable speed at this point when you must be able to reach target_velocity using the 
// acceleration within the allotted distance.
static float max_allowable_speed(float acceleration, float target_velocity, float distance)
{
    // to avoid invalid negative numbers due to numerical errors
    float value = std::max(0.0f, sqr(target_velocity) - 2.0f * acceleration * distance);
    return std::sqrt(value);
}

static float acceleration_time_from_distance(float initial_feedrate, float distance, float acceleration)
{
    return (acceleration != 0.0f) ? (speed_from_distance(initial_feedrate, distance, acceleration) - initial_feedrate) / acceleration : 0.0f;
}

void GCodeProcessor::CachedPosition::reset()
{
    std::fill(position.begin(), position.end(), FLT_MAX);
    feedrate = FLT_MAX;
}

void GCodeProcessor::CpColor::reset()
{
    counter = 0;
    current = 0;
}

float GCodeProcessor::Trapezoid::acceleration_time(float entry_feedrate, float acceleration) const
{
    return acceleration_time_from_distance(entry_feedrate, accelerate_until, acceleration);
}

float GCodeProcessor::Trapezoid::cruise_time() const
{
    return (cruise_feedrate != 0.0f) ? cruise_distance() / cruise_feedrate : 0.0f;
}

float GCodeProcessor::Trapezoid::deceleration_time(float distance, float acceleration) const
{
    return acceleration_time_from_distance(cruise_feedrate, (distance - decelerate_after), -acceleration);
}

float GCodeProcessor::Trapezoid::cruise_distance() const
{
    return decelerate_after - accelerate_until;
}

void GCodeProcessor::TimeBlock::calculate_trapezoid()
{
    trapezoid.cruise_feedrate = feedrate_profile.cruise;

    float accelerate_distance = std::max(0.0f, estimated_acceleration_distance(feedrate_profile.entry, feedrate_profile.cruise, acceleration));
    float decelerate_distance = std::max(0.0f, estimated_acceleration_distance(feedrate_profile.cruise, feedrate_profile.exit, -acceleration));
    float cruise_distance = distance - accelerate_distance - decelerate_distance;

    // Not enough space to reach the nominal feedrate.
    // This means no cruising, and we'll have to use intersection_distance() to calculate when to abort acceleration
    // and start braking in order to reach the exit_feedrate exactly at the end of this block.
    if (cruise_distance < 0.0f) {
        accelerate_distance = std::clamp(intersection_distance(feedrate_profile.entry, feedrate_profile.exit, acceleration, distance), 0.0f, distance);
        cruise_distance = 0.0f;
        trapezoid.cruise_feedrate = speed_from_distance(feedrate_profile.entry, accelerate_distance, acceleration);
        //feedrate_profile.cruise = trapezoid.cruise_feedrate;
        /*
    const float accelerate_steps_float = CEIL(intersection_distance(initial_rate, final_rate, accel, block->step_event_count));
    accelerate_steps = _MIN(uint32_t(_MAX(accelerate_steps_float, 0)), block->step_event_count);
    plateau_steps = 0;
*/
    }

    trapezoid.accelerate_until = accelerate_distance;
    trapezoid.decelerate_after = accelerate_distance + cruise_distance;
}

float GCodeProcessor::TimeBlock::time() const
{
    return trapezoid.acceleration_time(feedrate_profile.entry, acceleration)
            + trapezoid.cruise_time()
            + trapezoid.deceleration_time(distance, acceleration);
}

void GCodeProcessor::TimeMachine::State::reset()
{
    feedrate = 0.0f;
    safe_feedrate = 0.0f;
    axis_feedrate = { 0.0f, 0.0f, 0.0f, 0.0f };
    abs_axis_feedrate = { 0.0f, 0.0f, 0.0f, 0.0f };
}

void GCodeProcessor::TimeMachine::CustomGCodeTime::reset()
{
    needed = false;
    cache = 0.0f;
    times = std::vector<std::pair<Anker::CustomGCode::Type, float>>();
}

void GCodeProcessor::TimeMachine::reset()
{
    enabled = false;
    acceleration = 0.0f;
    max_acceleration = 0.0f;
    extrude_factor_override_percentage = 1.0f;
    time = 0.0f;
    time_acceleration = 1.0f;
    curr.reset();
    prev.reset();
    gcode_time.reset();
    blocks = std::vector<TimeBlock>();
    g1_times_cache = std::vector<G1LinesCacheItem>();
    std::fill(moves_time.begin(), moves_time.end(), 0.0f);
    std::fill(roles_time.begin(), roles_time.end(), 0.0f);
    layers_time = std::vector<float>();
}

void GCodeProcessor::TimeMachine::simulate_st_synchronize(float additional_time)
{
    if (!enabled)
        return;

    time += additional_time;
    gcode_time.cache += additional_time;
    calculate_time();
}

static void planner_forward_pass_kernel(GCodeProcessor::TimeBlock& prev, GCodeProcessor::TimeBlock& curr)
{
    // If the previous block is an acceleration block, but it is not long enough to complete the
    // full speed change within the block, we need to adjust the entry speed accordingly. Entry
    // speeds have already been reset, maximized, and reverse planned by reverse planner.
    // If nominal length is true, max junction speed is guaranteed to be reached. No need to recheck.
    if (!prev.flags.nominal_length) {
        if (prev.feedrate_profile.entry < curr.feedrate_profile.entry) {
            float prev_exit = max_allowable_speed(-prev.acceleration, prev.feedrate_profile.entry, prev.distance);
            float entry_speed = std::min(curr.feedrate_profile.entry, max_allowable_speed(-prev.acceleration, prev.feedrate_profile.entry, prev.distance));

            // Check for junction speed change
            if (curr.feedrate_profile.entry != entry_speed) {
                curr.feedrate_profile.entry = entry_speed;
                curr.flags.recalculate = true;
            }
        }
    }
}

void planner_reverse_pass_kernel(GCodeProcessor::TimeBlock& curr, GCodeProcessor::TimeBlock& next)
{
    // If entry speed is already at the maximum entry speed, no need to recheck. Block is cruising.
    // If not, block in state of acceleration or deceleration. Reset entry speed to maximum and
    // check for maximum allowable speed reductions to ensure maximum possible planned speed.
    if (curr.feedrate_profile.entry != curr.max_entry_speed) {
        // If nominal length true, max junction speed is guaranteed to be reached. Only compute
        // for max allowable speed if block is decelerating and nominal length is false.
        if (!curr.flags.nominal_length && curr.max_entry_speed > next.feedrate_profile.entry)
            curr.feedrate_profile.entry = std::min(curr.max_entry_speed, max_allowable_speed(-curr.acceleration, next.feedrate_profile.entry, curr.distance));
        else
            curr.feedrate_profile.entry = curr.max_entry_speed;

        curr.flags.recalculate = true;
    }
}

static void recalculate_trapezoids(std::vector<GCodeProcessor::TimeBlock>& blocks)
{
    GCodeProcessor::TimeBlock* curr = nullptr;
    GCodeProcessor::TimeBlock* next = nullptr;

    for (size_t i = 0; i < blocks.size(); ++i) {
        GCodeProcessor::TimeBlock& b = blocks[i];

        curr = next;
        next = &b;

        /*

            const float current_nominal_speed = SQRT(block->nominal_speed_sqr),
                        nomr = 1.0f / current_nominal_speed;
            calculate_trapezoid_for_block(block, current_entry_speed * nomr, next_entry_speed * nomr);
            #if true //ENABLED(LIN_ADVANCE)
              if (block->use_advance_lead) {
                const float comp = block->e_D_ratio * extruder_advance_K[active_extruder] * settings.axis_steps_per_mm[E_AXIS];





              }
            #endif
*/

        if (curr != nullptr) {
            // Recalculate if current block entry or exit junction speed has changed.
            if (curr->flags.recalculate || next->flags.recalculate) {
                // NOTE: Entry and exit factors always > 0 by all previous logic operations.
                GCodeProcessor::TimeBlock& block = *curr;
                block.feedrate_profile.exit = next->feedrate_profile.entry;
                block.calculate_trapezoid();
                //curr->trapezoid = block.trapezoid;
                curr->flags.recalculate = false; // Reset current only to ensure next trapezoid is computed
            }
        }
    }

    // Last/newest block in buffer. Always recalculated.
    if (next != nullptr) {
        GCodeProcessor::TimeBlock& block = *next;
        block.feedrate_profile.exit = next->safe_feedrate;
        block.calculate_trapezoid();
        //next->trapezoid = block.trapezoid;
        next->flags.recalculate = false;
    }
}

void GCodeProcessor::TimeMachine::calculate_time(size_t keep_last_n_blocks)
{
    if (!enabled || blocks.size() < 2)
        return;

    assert(keep_last_n_blocks <= blocks.size());

    // forward_pass
    for (size_t i = 0; i + 1 < blocks.size(); ++i) {
        planner_forward_pass_kernel(blocks[i], blocks[i + 1]);
    }

    // reverse_pass
    for (int i = static_cast<int>(blocks.size()) - 1; i > 0; --i)
        planner_reverse_pass_kernel(blocks[i - 1], blocks[i]);

    recalculate_trapezoids(blocks);

    size_t n_blocks_process = blocks.size() - keep_last_n_blocks;
    for (size_t i = 0; i < n_blocks_process; ++i) {
        const TimeBlock& block = blocks[i];

#ifdef false // CL_OPT
        {
            //block.distance;
            //block.acceleration;
            //block.trapezoid.accelerate_until;
            //block.trapezoid.decelerate_after;
            //block.trapezoid.cruise_feedrate;
            time_acceleration = 1.0012f;
            float cruise_distance = block.trapezoid.decelerate_after - block.trapezoid.accelerate_until;
            float distance = block.distance;
            block.acceleration;
            if(distance < 0.1)          time_acceleration = 1.008f;
            if(cruise_distance < 0.1)   time_acceleration = 1.04f;

        }
#endif
        float block_time = block.time() * time_acceleration;
        time += block_time;
        gcode_time.cache += block_time;
        moves_time[static_cast<size_t>(block.move_type)] += block_time;
        roles_time[static_cast<size_t>(block.role)] += block_time;
        if (block.layer_id > 0) {
            if (block.layer_id >= layers_time.size()) {
                size_t curr_size = layers_time.size();
                layers_time.resize(block.layer_id);
                for (size_t i = curr_size; i < layers_time.size(); ++i) {
                    layers_time[i] = 0.0f;
                }
            }
            layers_time[block.layer_id - 1] += block_time;
        }
        g1_times_cache.push_back({ block.g1_line_id, time });
    }

    {
        {   //  add @2022-05-29 by CL
            /*
                FeedrateProfile feedrate_profile;
                Trapezoid trapezoid;

                std::vector<TimeBlock>  & blocks = GCodeProcessor::m_time_processor.machines[0].blocks;
                std::vector<MoveVertex> & moves  = GCodeProcessor::m_result.moves;
            */
            //std::vector<TimeBlock>  & blocks = gCodeProcessor->m_time_processor.machines[0].blocks;
            std::vector<MoveVertex> & moves  = gCodeProcessor->m_result.moves;
            //qDebug() << __LINE__ << __FUNCTION__ << "MoveVertex" << blocks.size() << moves.size();
            //size_t i = 0, j = 0;
            for (int i = (int)n_blocks_process-1; i >= 0; --i) {
                const TimeBlock& block = blocks[i];
                unsigned int id_in_block = block.g1_line_id;

                for (int j = (int)moves.size()-1; j >= 0; --j) {
                    unsigned int id_in_move = moves[j].g1_line_id;

                    if(id_in_block == id_in_move){
                        moves[j].motor_process.entry            = block.feedrate_profile.entry;
                        moves[j].motor_process.cruise           = block.feedrate_profile.cruise;
                        moves[j].motor_process.exit             = block.feedrate_profile.exit;
                        moves[j].motor_process.accelerate_until = block.trapezoid.accelerate_until;
                        moves[j].motor_process.decelerate_after = block.trapezoid.decelerate_after;
                        moves[j].motor_process.cruise           = block.trapezoid.cruise_feedrate;
                        //moves[j].feedrate = block.trapezoid.cruise_feedrate;
                        if(0 && block.feedrate_profile.cruise != block.trapezoid.cruise_feedrate || moves[j].motor_process.entry > moves[j].motor_process.cruise)
                            qDebug() << __LINE__ << "Id=" << id_in_block << i << j << "\t"
                                     << block.acceleration      << "\t"
                                     << block.distance          << "\t"
                                     << block.trapezoid.accelerate_until    << "\t"
                                     << block.trapezoid.decelerate_after    << "\t"
                                     << block.trapezoid.cruise_feedrate     << "\t"
                                     << block.feedrate_profile.entry        << "\t"
                                     << block.feedrate_profile.cruise       << "\t"
                                     << block.feedrate_profile.exit         << "\t"
                                        ;

                        break; //  end of for (int j = (int)moves.size()-1; j >= 0; --j)
                    }
                }

            }
        }//  add @2022-05-27 by CL
    }

    if (keep_last_n_blocks)
        blocks.erase(blocks.begin(), blocks.begin() + n_blocks_process);
    else
        blocks.clear();
}

void GCodeProcessor::TimeProcessor::reset()
{
    extruder_unloaded = true;
    export_remaining_time_enabled = false;
    machine_envelope_processing_enabled = false;
    machine_limits = Anker::MachineEnvelopeConfig();
    filament_load_times = std::vector<float>();
    filament_unload_times = std::vector<float>();
    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
        machines[i].reset();
    }
    machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Normal)].enabled = true;
}

void GCodeProcessor::TimeProcessor::post_process(const std::string& filename)
{

    QString qfileName = QString::fromStdString(filename.c_str());
    QFile f(qfileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    QTextStream in(&f);
    if (in.status() != QTextStream::Ok)
        throw Slic3r::RuntimeError(std::string("Time estimator post process export failed.\nCannot open file for reading.\n"));

    // temporary file to contain modified gcode
    std::string out_path = filename + ".postprocess";
    QString outfile = QString::fromStdString(out_path.c_str());
    QFile out(outfile);
    if (!out.open(QIODevice::WriteOnly))
    {
        return ;
        throw Slic3r::RuntimeError(std::string("Time estimator post process export failed.\nCannot open file for writing.\n"));
    }
    //    FILE* out = boost::nowide::fopen(out_path.c_str(), "wb");
    //    if (out == nullptr)
    //        return;
    //        throw Slic3r::RuntimeError(std::string("Time estimator post process export failed.\nCannot open file for writing.\n"));

    auto time_in_minutes = [](float time_in_seconds) {
        return int(::roundf(time_in_seconds / 60.0f));
    };

    GCodeReader parser;
    std::string gcode_line;
    size_t g1_lines_counter = 0;
    // keeps track of last exported pair <percent, remaining time>
    std::array<std::pair<int, int>, static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count)> last_exported;
    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
        last_exported[i] = { 0, time_in_minutes(machines[i].time) };
    }

    // buffer line to export only when greater than 64K to reduce writing calls
    std::string export_line;

    // replace placeholder lines with the proper final value
    auto process_placeholders = [&](const std::string& gcode_line) {
        // remove trailing '\n'
        std::string line = gcode_line.substr(0, gcode_line.length() - 1);

        std::string ret;
        
        if (export_remaining_time_enabled && (line == First_Line_M73_Placeholder_Tag || line == Last_Line_M73_Placeholder_Tag)) {
            for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
                const TimeMachine& machine = machines[i];
                if (machine.enabled) {
                    if (machine.remaining_times_type == Anker::rtM73) {
                        ret += "M73 P" + std::to_string((line == First_Line_M73_Placeholder_Tag) ? 0 : 100) +" R" + std::to_string((line == First_Line_M73_Placeholder_Tag) ? time_in_minutes(machine.time) : 0) +"\n";
                        //ret += (boost::format("M73 P%1% R%2%\n")
                        //    % std::to_string((line == First_Line_M73_Placeholder_Tag) ? 0 : 100)
                        //    % std::to_string((line == First_Line_M73_Placeholder_Tag) ? time_in_minutes(machine.time) : 0)).str();
                    } else if (machine.remaining_times_type == Anker::rtM117) {
                        if ((line == First_Line_M73_Placeholder_Tag))
                            ret += "M117 Time Left " + std::to_string(int32_t(machine.time) / 3600) + "h" + std::to_string(int32_t(machine.time / 60) % 60) + "m" + std::to_string(int32_t(machine.time) % 60) + "s\n";
                        //ret += (boost::format("M117 Time Left %1%h%2%m%3%s\n")
                        //    % std::to_string(int32_t(machine.time) / 3600) % std::to_string(int32_t(machine.time / 60) % 60) % std::to_string(int32_t(machine.time) % 60)
                        //    ).str();
                        else
                            ret += "M117 Time Left 0s\n";
                    }
                }
            }
        }
        else if (line == Estimated_Printing_Time_Placeholder_Tag) {
            for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
                const TimeMachine& machine = machines[i];
                PrintEstimatedTimeStatistics::ETimeMode mode = static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i);
                if (mode == PrintEstimatedTimeStatistics::ETimeMode::Normal || machine.enabled) {
                    char buf[128];
                    sprintf(buf, "; estimated printing time (%s mode) = %s\n",
                            (mode == PrintEstimatedTimeStatistics::ETimeMode::Normal) ? "normal" : "silent",
                            Anker::get_time_dhms(machine.time).c_str());
                    ret += buf;
                }
            }
        }

        return std::make_pair(!ret.empty(), ret.empty() ? gcode_line : ret);
    };

    // check for temporary lines
    auto is_temporary_decoration = [](const std::string_view gcode_line) {
        // remove trailing '\n'
        assert(! gcode_line.empty());
        assert(gcode_line.back() == '\n');

        // return true for decorations which are used in processing the gcode but that should not be exported into the final gcode
        // i.e.:
        // bool ret = gcode_line.substr(0, gcode_line.length() - 1) == ";" + Layer_Change_Tag;
        // ...
        // return ret;
        return false;
    };

    // Iterators for the normal and silent cached time estimate entry recently processed, used by process_line_G1.
    auto g1_times_cache_it = reserve_vector<std::vector<TimeMachine::G1LinesCacheItem>::const_iterator>(machines.size());
    for (const auto& machine : machines)
        g1_times_cache_it.emplace_back(machine.g1_times_cache.begin());
    // add lines M73 to exported gcode
    auto process_line_G1 = [&]() {
        if (export_remaining_time_enabled) {
            for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
                const TimeMachine& machine = machines[i];
                if (machine.enabled) {
                    // Skip all machine.g1_times_cache below g1_lines_counter.
                    auto& it = g1_times_cache_it[i];
                    while (it != machine.g1_times_cache.end() && it->id < g1_lines_counter)
                        ++it;
                    if (it != machine.g1_times_cache.end() && it->id == g1_lines_counter) {
                        float elapsed_time = it->elapsed_time;
                        std::pair<int, int> to_export = { int(100.0f * elapsed_time / machine.time),
                                                          time_in_minutes(machine.time - elapsed_time) };
                        if (last_exported[i] != to_export) {
                            if (machine.remaining_times_type == Anker::rtM73) {
                                export_line += "M73 P" + std::to_string(to_export.first) + " R" + std::to_string(to_export.second) + "\n";
                                //export_line += (boost::format("M73 P%1% R%2%\n")
                                //    % std::to_string(to_export.first)
                                //    % std::to_string(to_export.second)).str();
                            } else if (machine.remaining_times_type == Anker::rtM117) {
                                export_line += "M117 Time Left " + std::to_string(int32_t(machine.time - elapsed_time) / 3600) + "h" + std::to_string((int32_t(machine.time - elapsed_time) / 60) % 60) + "m" + std::to_string(int32_t(machine.time - elapsed_time) % 60) + "s\n";
                                //export_line += (boost::format("M117 Time Left %1%h%2%m%3%s\n")
                                //    % std::to_string(int32_t(machine.time - elapsed_time) / 3600) % std::to_string((int32_t(machine.time - elapsed_time) / 60) % 60) % std::to_string(int32_t(machine.time - elapsed_time) % 60)
                                //    ).str();
                            }
                            last_exported[i] = to_export;
                        }
                    }
                }
            }
        }
    };

    // helper function to write to disk
    auto write_string = [&](const std::string& str) {
        //fwrite((const void*)export_line.c_str(), 1, export_line.length(), out);
        qint64 re = out.write(export_line.c_str(),export_line.length());
        if (re == -1) {
            f.close();
            out.close();
            out.remove();
            throw Slic3r::RuntimeError(std::string("Time estimator post process export failed.\nIs the disk full?\n"));
        }
        export_line.clear();
    };

    while (!in.atEnd()) {
        gcode_line = in.readLine().toStdString();
        if (in.status() != QTextStream::Ok) {
            out.close();
            throw Slic3r::RuntimeError(std::string("Time estimator post process export failed.\nError while reading from file.\n"));
        }

        gcode_line += "\n";
        // replace placeholder lines
        auto [processed, result] = process_placeholders(gcode_line);
                gcode_line = result;
                if (!processed) {
            // remove temporary lines
            if (is_temporary_decoration(gcode_line))
                continue;

            // add lines M73 where needed
            parser.parse_line(gcode_line,
                              [&](GCodeReader& reader, const GCodeReader::GCodeLine& line) {
                if (line.cmd_is("G1")) {
                    process_line_G1();
                    ++g1_lines_counter;
                }
            });
        }

        export_line += gcode_line;
        if (export_line.length() > 65535)
            write_string(export_line);
    }

    if (!export_line.empty())
        write_string(export_line);

    out.close();
    f.close();

    std::error_code err_code;
    if (err_code = Anker::rename_file(out_path, filename)) {
        std::string err_msg = (std::string("Failed to rename the output G-code file from ") + out_path + " to " + filename + '\n' +
                               "Is " + out_path + " locked? (gcp)" + err_code.message() + '\n');
        if (Anker::copy_file(out_path, filename, err_msg, true) != Anker::SUCCESS)
            throw Slic3r::RuntimeError(std::string("Failed to rename the output G-code file from ") + out_path + " to " + filename + '\n' +
                                       "Is " + out_path + " locked? (gcp)" + err_code.message() + '\n');
    }
}

const std::vector<std::pair<GCodeProcessor::EProducer, std::string>> GCodeProcessor::Producers = {
    { EProducer::PrusaSlicer, "PrusaSlicer" },
    { EProducer::SuperSlicer, "SuperSlicer" },
    { EProducer::Slic3rPE,    "Slic3r Prusa Edition" },
    { EProducer::Slic3r,      "Slic3r" },
    { EProducer::Cura,        "Cura_SteamEngine" },
    { EProducer::Simplify3D,  "Simplify3D" },
    { EProducer::CraftWare,   "CraftWare" },
    { EProducer::ideaMaker,   "ideaMaker" },
    { EProducer::KissSlicer,  "KISSlicer" },
    { EProducer::AnkerSlicer,  "AnkerSlicer" } //
};

unsigned int GCodeProcessor::s_result_id = 0;

GCodeProcessor::GCodeProcessor()
{
    reset();
}

//void GCodeProcessor::apply_config(const PrintConfig& config)
//{
//    m_parser.apply_config(config);
//
//    m_flavor = config.gcode_flavor;
//
//    size_t extruders_count = config.nozzle_diameter.values.size();
//    m_result.extruders_count = extruders_count;
//
//    m_extruder_offsets.resize(extruders_count);
//    for (size_t i = 0; i < extruders_count; ++i) {
//        Vec2f offset = config.extruder_offset.get_at(i).cast<float>();
//        m_extruder_offsets[i] = { offset(0), offset(1), 0.0f };
//    }
//
//    m_extruder_names.resize(extruders_count);
//    for (size_t i = 0; i < extruders_count; ++i) {
//        m_extruder_names[i] = config.tool_name.get_at(i);
//    }
//
//    m_extruder_colors.resize(extruders_count);
//    for (size_t i = 0; i < extruders_count; ++i) {
//        m_extruder_colors[i] = static_cast<unsigned char>(i);
//    }
//
//    m_filament_diameters.resize(config.filament_diameter.values.size());
//    for (size_t i = 0; i < config.filament_diameter.values.size(); ++i) {
//        m_filament_diameters[i] = static_cast<float>(config.filament_diameter.values[i]);
//    }
//
//    if (config.machine_limits_usage.value < MachineLimitsUsage::Limits)
//        m_time_processor.machine_limits = reinterpret_cast<const MachineEnvelopeConfig&>(config);
//
//    // Filament load / unload times are not specific to a firmware flavor. Let anybody use it if they find it useful.
//    // As of now the fields are shown at the UI dialog in the same combo box as the ramming values, so they
//    // are considered to be active for the single extruder multi-material printers only.
//    m_time_processor.filament_load_times.resize(config.filament_load_time.values.size());
//    for (size_t i = 0; i < config.filament_load_time.values.size(); ++i) {
//        m_time_processor.filament_load_times[i] = static_cast<float>(config.filament_load_time.values[i]);
//    }
//    m_time_processor.filament_unload_times.resize(config.filament_unload_time.values.size());
//    for (size_t i = 0; i < config.filament_unload_time.values.size(); ++i) {
//        m_time_processor.filament_unload_times[i] = static_cast<float>(config.filament_unload_time.values[i]);
//    }
//
//    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
//        float max_acceleration = get_option_value(m_time_processor.machine_limits.machine_max_acceleration_extruding, i);
//        m_time_processor.machines[i].max_acceleration = max_acceleration;
//        m_time_processor.machines[i].acceleration = (max_acceleration > 0.0f) ? max_acceleration : DEFAULT_ACCELERATION;
//    }
//
//    m_time_processor.export_remaining_time_enabled = config.remaining_times.value;
//
//#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
//    m_use_volumetric_e = config.use_volumetric_e;
//#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
//
//    if (m_flavor != Anker::gcfMarlin) {
//        double time_estimation_compensation = config.get_computed_value("time_estimation_compensation");
//        for (auto& machine : this->m_time_processor.machines) {
//            machine.time_acceleration = float(time_estimation_compensation);
//        }
//    }
//
//    m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Normal)].remaining_times_type = config.remaining_times_type.value;
//    m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].remaining_times_type = config.remaining_times_type.value;
//
//}
//
//void GCodeProcessor::apply_config(const DynamicPrintConfig& config)
//{
//    m_parser.apply_config(config);
//
//    const ConfigOptionEnum<GCodeFlavor>* gcode_flavor = config.option<ConfigOptionEnum<GCodeFlavor>>("gcode_flavor");
//    if (gcode_flavor != nullptr)
//        m_flavor = gcode_flavor->value;
//
//    const ConfigOptionPoints* bed_shape = config.option<ConfigOptionPoints>("bed_shape");
//    if (bed_shape != nullptr)
//        m_result.bed_shape = bed_shape->values;
//
//    const ConfigOptionString* print_settings_id = config.option<ConfigOptionString>("print_settings_id");
//    if (print_settings_id != nullptr)
//        m_result.settings_ids.print = print_settings_id->value;
//
//    const ConfigOptionStrings* filament_settings_id = config.option<ConfigOptionStrings>("filament_settings_id");
//    if (filament_settings_id != nullptr)
//        m_result.settings_ids.filament = filament_settings_id->values;
//
//    const ConfigOptionString* printer_settings_id = config.option<ConfigOptionString>("printer_settings_id");
//    if (printer_settings_id != nullptr)
//        m_result.settings_ids.printer = printer_settings_id->value;
//
//    const Anker::ConfigOption<double>* filament_diameters = config.option<Anker::ConfigOption<double>>("filament_diameter");
//    if (filament_diameters != nullptr) {
//        for (double diam : filament_diameters->values) {
//            m_filament_diameters.push_back(static_cast<float>(diam));
//        }
//    }
//
//    m_result.extruders_count = config.option<Anker::ConfigOption<double>>("nozzle_diameter")->values.size();
//
//    const ConfigOptionPoints* extruder_offset = config.option<ConfigOptionPoints>("extruder_offset");
//    if (extruder_offset != nullptr) {
//        m_extruder_offsets.resize(extruder_offset->values.size());
//        for (size_t i = 0; i < extruder_offset->values.size(); ++i) {
//            Vec2f offset = extruder_offset->values[i].cast<float>();
//            m_extruder_offsets[i] = { offset(0), offset(1), 0.0f };
//        }
//    }
//
//    std::string default_color = "#FF8000";
//    m_result.filament_colors = std::vector<std::string>(1, default_color);
//    const ConfigOptionStrings* extruder_colour = config.option<ConfigOptionStrings>("extruder_colour");
//    const ConfigOptionStrings* filament_colour = config.option<ConfigOptionStrings>("filament_colour");
//    // takes colors from config
//    if (extruder_colour != nullptr)
//        m_result.extruder_colors = extruder_colour->values;
//    if (filament_colour != nullptr)
//        m_result.filament_colors = filament_colour->values;
//
//    // try to replace missing values with filament colors
//    if (filament_colour != nullptr && filament_colour != nullptr && m_result.extruder_colors.size() == m_result.filament_colors.size()) {
//        for (size_t i = 0; i < m_result.extruder_colors.size(); ++i) {
//            if (m_result.extruder_colors[i].empty())
//                m_result.extruder_colors[i] = m_result.filament_colors[i];
//        }
//    }
//
//    // replace missing values with default
//    for (size_t i = 0; i < m_result.extruder_colors.size(); ++i) {
//        if (m_result.extruder_colors[i].empty())
//            m_result.extruder_colors[i] = default_color;
//    }
//    for (size_t i = 0; i < m_result.filament_colors.size(); ++i) {
//        if (m_result.filament_colors[i].empty())
//            m_result.filament_colors[i] = default_color;
//    }
//
//    m_extruder_colors.resize(m_result.extruder_colors.size());
//    for (size_t i = 0; i < m_result.extruder_colors.size(); ++i) {
//        m_extruder_colors[i] = static_cast<unsigned char>(i);
//    }
//
//    const Anker::ConfigOption<double>* filament_load_time = config.option<Anker::ConfigOption<double>>("filament_load_time");
//    if (filament_load_time != nullptr) {
//        m_time_processor.filament_load_times.resize(filament_load_time->values.size());
//        for (size_t i = 0; i < filament_load_time->values.size(); ++i) {
//            m_time_processor.filament_load_times[i] = static_cast<float>(filament_load_time->values[i]);
//        }
//    }
//
//    const Anker::ConfigOption<double>* filament_unload_time = config.option<Anker::ConfigOption<double>>("filament_unload_time");
//    if (filament_unload_time != nullptr) {
//        m_time_processor.filament_unload_times.resize(filament_unload_time->values.size());
//        for (size_t i = 0; i < filament_unload_time->values.size(); ++i) {
//            m_time_processor.filament_unload_times[i] = static_cast<float>(filament_unload_time->values[i]);
//        }
//    }
//
//    const ConfigOptionEnum<MachineLimitsUsage>* machine_limits_usage = config.option<ConfigOptionEnum<MachineLimitsUsage>>("machine_limits_usage");
//    if (machine_limits_usage && machine_limits_usage->value != MachineLimitsUsage::Ignore) {
//        const Anker::ConfigOption<double>* machine_max_acceleration_x = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_x");
//        if (machine_max_acceleration_x != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_x.values = machine_max_acceleration_x->values;
//
//        const Anker::ConfigOption<double>* machine_max_acceleration_y = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_y");
//        if (machine_max_acceleration_y != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_y.values = machine_max_acceleration_y->values;
//
//        const Anker::ConfigOption<double>* machine_max_acceleration_z = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_z");
//        if (machine_max_acceleration_z != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_z.values = machine_max_acceleration_z->values;
//
//        const Anker::ConfigOption<double>* machine_max_acceleration_e = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_e");
//        if (machine_max_acceleration_e != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_e.values = machine_max_acceleration_e->values;
//
//        const Anker::ConfigOption<double>* machine_max_feedrate_x = config.option<Anker::ConfigOption<double>>("machine_max_feedrate_x");
//        if (machine_max_feedrate_x != nullptr)
//            m_time_processor.machine_limits.machine_max_feedrate_x.values = machine_max_feedrate_x->values;
//
//        const Anker::ConfigOption<double>* machine_max_feedrate_y = config.option<Anker::ConfigOption<double>>("machine_max_feedrate_y");
//        if (machine_max_feedrate_y != nullptr)
//            m_time_processor.machine_limits.machine_max_feedrate_y.values = machine_max_feedrate_y->values;
//
//        const Anker::ConfigOption<double>* machine_max_feedrate_z = config.option<Anker::ConfigOption<double>>("machine_max_feedrate_z");
//        if (machine_max_feedrate_z != nullptr)
//            m_time_processor.machine_limits.machine_max_feedrate_z.values = machine_max_feedrate_z->values;
//
//        const Anker::ConfigOption<double>* machine_max_feedrate_e = config.option<Anker::ConfigOption<double>>("machine_max_feedrate_e");
//        if (machine_max_feedrate_e != nullptr)
//            m_time_processor.machine_limits.machine_max_feedrate_e.values = machine_max_feedrate_e->values;
//
//        const Anker::ConfigOption<double>* machine_max_jerk_x = config.option<Anker::ConfigOption<double>>("machine_max_jerk_x");
//        if (machine_max_jerk_x != nullptr)
//            m_time_processor.machine_limits.machine_max_jerk_x.values = machine_max_jerk_x->values;
//
//        const Anker::ConfigOption<double>* machine_max_jerk_y = config.option<Anker::ConfigOption<double>>("machine_max_jerk_y");
//        if (machine_max_jerk_y != nullptr)
//            m_time_processor.machine_limits.machine_max_jerk_y.values = machine_max_jerk_y->values;
//
//        const Anker::ConfigOption<double>* machine_max_jerk_z = config.option<Anker::ConfigOption<double>>("machine_max_jerkz");
//        if (machine_max_jerk_z != nullptr)
//            m_time_processor.machine_limits.machine_max_jerk_z.values = machine_max_jerk_z->values;
//
//        const Anker::ConfigOption<double>* machine_max_jerk_e = config.option<Anker::ConfigOption<double>>("machine_max_jerk_e");
//        if (machine_max_jerk_e != nullptr)
//            m_time_processor.machine_limits.machine_max_jerk_e.values = machine_max_jerk_e->values;
//
//        const Anker::ConfigOption<double>* machine_max_acceleration_extruding = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_extruding");
//        if (machine_max_acceleration_extruding != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_extruding.values = machine_max_acceleration_extruding->values;
//
//        const Anker::ConfigOption<double>* machine_max_acceleration_retracting = config.option<Anker::ConfigOption<double>>("machine_max_acceleration_retracting");
//        if (machine_max_acceleration_retracting != nullptr)
//            m_time_processor.machine_limits.machine_max_acceleration_retracting.values = machine_max_acceleration_retracting->values;
//
//        if (std::set<uint8_t>{Anker::gcfMarlin, gcfLerdge, Anker::gcfRepetier, Anker::gcfRepRap}.count(m_flavor) > 0) {
//            const Anker::ConfigOption<double>* machine_min_extruding_rate = config.option<Anker::ConfigOption<double>>("machine_min_extruding_rate");
//            if (machine_min_extruding_rate != nullptr)
//                m_time_processor.machine_limits.machine_min_extruding_rate.values = machine_min_extruding_rate->values;
//        }
//
//        const Anker::ConfigOption<double>* machine_min_travel_rate = config.option<Anker::ConfigOption<double>>("machine_min_travel_rate");
//        if (machine_min_travel_rate != nullptr)
//            m_time_processor.machine_limits.machine_min_travel_rate.values = machine_min_travel_rate->values;
//    }
//
//    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
//        float max_acceleration = get_option_value(m_time_processor.machine_limits.machine_max_acceleration_extruding, i);
//        m_time_processor.machines[i].max_acceleration = max_acceleration;
//        m_time_processor.machines[i].acceleration = (max_acceleration > 0.0f) ? max_acceleration : DEFAULT_ACCELERATION;
//    }
//
//    if (m_time_processor.machine_limits.machine_max_acceleration_x.values.size() > 1)
//        enable_stealth_time_estimator(true);
//
//#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
//    const ConfigOptionBool* use_volumetric_e = config.option<ConfigOptionBool>("use_volumetric_e");
//    if (use_volumetric_e != nullptr)
//        m_use_volumetric_e = use_volumetric_e->value;
//#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
//
//    //adapt to firmware units
//    if (m_flavor != Anker::gcfMarlin && m_flavor != gcfLerdge && m_flavor != Anker::gcfSmoothie) {
//        //change some fields from mm/min to mm/sec as intended
//        for (double& val : m_time_processor.machine_limits.machine_max_feedrate_x.values) val /= 60;
//        for (double& val : m_time_processor.machine_limits.machine_max_feedrate_y.values) val /= 60;
//        for (double& val : m_time_processor.machine_limits.machine_max_feedrate_z.values) val /= 60;
//        for (double& val : m_time_processor.machine_limits.machine_min_extruding_rate.values) val /= 60;
//        for (double& val : m_time_processor.machine_limits.machine_min_travel_rate.values) val /= 60;
//    }
//
//    if (m_flavor != Anker::gcfMarlin) {
//        double time_estimation_compensation = config.get_computed_value("time_estimation_compensation");
//        for (auto& machine : this->m_time_processor.machines) {
//            machine.time_acceleration = float(time_estimation_compensation);
//        }
//    }
//
//
//    const ConfigOptionEnum<RemainingTimeType>* remaining_times_type = config.option<ConfigOptionEnum<RemainingTimeType>>("remaining_times_type");
//    if (remaining_times_type) {
//        m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Normal)].remaining_times_type = remaining_times_type->value;
//        m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].remaining_times_type = remaining_times_type->value;
//    }
//
//}

void GCodeProcessor::enable_stealth_time_estimator(bool enabled)
{
    m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].enabled = enabled;
}

void GCodeProcessor::reset()
{
    static const size_t Min_Extruder_Count = 5;

    m_units = EUnits::Millimeters;
    m_global_positioning_type = EPositioningType::Absolute;
    m_e_local_positioning_type = EPositioningType::Absolute;
    m_extruder_offsets = std::vector<Vec3f>(Min_Extruder_Count, Vec3f::Zero());
    m_flavor = Anker::gcfSprinter;

    m_start_position = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_end_position = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_origin = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_cached_position.reset();
    m_wiping = false;

    m_feedrate = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    m_forced_width = 0.0f;
    m_forced_height = 0.0f;
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    m_mm3_per_mm = 0.0f;
    m_fan_speed = 0.0f;
    m_temperature = 0.0f;

    m_extrusion_role = Anker::erNone;
    m_extruder_id = 0;
    m_extruder_colors.resize(Min_Extruder_Count);
    for (size_t i = 0; i < Min_Extruder_Count; ++i) {
        m_extruder_colors[i] = static_cast<unsigned char>(i);
    }

    m_filament_diameters = std::vector<float>(Min_Extruder_Count, 1.75f);
    m_extruded_last_z = 0.0;
    m_g1_line_id = 0;
    m_layer_id = 0;
    m_cp_color.reset();

    m_producer = EProducer::Unknown;
    m_producers_enabled = false;

    m_time_processor.reset();
    //  add @2022-05-29 by CL
    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
        m_time_processor.machines[i].gCodeProcessor = this;
    }

    m_result.reset();
    m_result.id = ++s_result_id;

#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
    m_use_volumetric_e = false;
#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
    m_mm3_per_mm_compare.reset();
    m_height_compare.reset();
    m_width_compare.reset();
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
}
void GCodeProcessor::process_string(const std::string& gcode, std::function<void()> cancel_callback)
{
    std::stringstream ss(gcode);
    std::string strline;
    while ( std::getline(ss, strline))
        m_parser.parse_line(strline, [this](GCodeReader& reader, const GCodeReader::GCodeLine& line) {
            process_gcode_line(line);
        });
}

void GCodeProcessor::process_file(const std::string& filename, bool apply_postprocess, std::function<void()> cancel_callback)
{
    auto last_cancel_callback_time = std::chrono::high_resolution_clock::now();

#if ENABLE_GCODE_VIEWER_STATISTICS
    auto start_time = std::chrono::high_resolution_clock::now();
#endif // ENABLE_GCODE_VIEWER_STATISTICS

    // pre-processing
    // parse the gcode file to detect its producer
    if (m_producers_enabled) {
        m_parser.parse_file(filename, [this](GCodeReader& reader, const GCodeReader::GCodeLine& line) {
            const std::string_view cmd = line.cmd();
            if (cmd.length() == 0) {
                const std::string_view comment = line.comment();
                if (comment.length() > 1 && detect_producer(comment))
                    m_parser.quit_parsing_file();
            }
        });

        // if the gcode was produced by this slicer,
        // extract the config from it
        try {
            if (m_producer == EProducer::AnkerSlicer||m_producer == EProducer::PrusaSlicer || m_producer == EProducer::SuperSlicer || m_producer == EProducer::Slic3rPE || m_producer == EProducer::Slic3r) {
                //TODO:
                //DynamicPrintConfig config;
                //config.apply(FullPrintConfig::defaults());
                // Silently substitute unknown values by new ones for loading configurations from PrusaSlicer's own G-code.
                // Showing substitution log or errors may make sense, but we are not really reading many values from the G-code config,
                // thus a probability of incorrect substitution is low and the G-code viewer is a consumer-only anyways.
                //config.load_from_gcode_file(filename, ForwardCompatibilitySubstitutionRule::EnableSilent);
                //apply_config(config);
                for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
                    float max_acceleration = get_option_value(m_time_processor.machine_limits.machine_max_acceleration_extruding, i);
                    m_time_processor.machines[i].max_acceleration = max_acceleration;
                    m_time_processor.machines[i].acceleration = (max_acceleration > 0.0f) ? max_acceleration : DEFAULT_ACCELERATION;
                }
            }
        }
        catch (Slic3r::Exception ex) {
            m_producer = EProducer::Unknown;
        }
    }

    // process gcode
    m_result.id = ++s_result_id;
    // 1st move must be a dummy move
    m_result.moves.emplace_back(MoveVertex());
    m_parser.parse_file(filename, [this, cancel_callback, &last_cancel_callback_time](GCodeReader& reader, const GCodeReader::GCodeLine& line) {
        if (cancel_callback != nullptr) {
            // call the cancel callback every 100 ms
            auto curr_time = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - last_cancel_callback_time).count() > 100) {
                cancel_callback();
                last_cancel_callback_time = curr_time;
            }
        }
        process_gcode_line(line);
    });

    // update width/height of wipe moves
    for (MoveVertex& move : m_result.moves) {
        if (move.type == EMoveType::Wipe) {
            move.width = Wipe_Width;
            move.height = Wipe_Height;
        }
    }

    // process the time blocks
    for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
        TimeMachine& machine = m_time_processor.machines[i];
        TimeMachine::CustomGCodeTime& gcode_time = machine.gcode_time;
        machine.calculate_time();
        if (gcode_time.needed && gcode_time.cache != 0.0f)
            gcode_time.times.push_back({ Anker::CustomGCode::ColorChange, gcode_time.cache });
    }

    update_estimated_times_stats();

    // post-process to add M73 lines into the gcode
    if (apply_postprocess)
        m_time_processor.post_process(filename);

    //update times for results
    for (size_t i = 0; i < m_result.moves.size(); i++) {
        //field layer_duration contains the layer id for the move in which the layer_duration has to be set.
        size_t layer_id = size_t(m_result.moves[i].layer_duration);
        std::vector<float>& layer_times = m_result.time_statistics.modes[0].layers_times;
        if (layer_times.size() > layer_id - 1 && layer_id > 0)
            m_result.moves[i].layer_duration = layer_times[layer_id - 1];
        else
            m_result.moves[i].layer_duration = 0;
    }
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
    m_mm3_per_mm_compare.output();
    m_height_compare.output();
    m_width_compare.output();
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

#if ENABLE_GCODE_VIEWER_STATISTICS
    m_result.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
#endif // ENABLE_GCODE_VIEWER_STATISTICS
}

float GCodeProcessor::get_time(PrintEstimatedTimeStatistics::ETimeMode mode) const
{
    return (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) ? m_time_processor.machines[static_cast<size_t>(mode)].time : 0.0f;
    }

    std::string GCodeProcessor::get_time_dhm(PrintEstimatedTimeStatistics::ETimeMode mode) const
    {
    return (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) ? Anker::short_time(Anker::get_time_dhms(m_time_processor.machines[static_cast<size_t>(mode)].time)) : std::string("N/A");
}

std::vector<std::pair<Anker::CustomGCode::Type, std::pair<float, float>>> GCodeProcessor::get_custom_gcode_times(PrintEstimatedTimeStatistics::ETimeMode mode, bool include_remaining) const
{
    std::vector<std::pair<Anker::CustomGCode::Type, std::pair<float, float>>> ret;
    if (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) {
        const TimeMachine& machine = m_time_processor.machines[static_cast<size_t>(mode)];
        float total_time = 0.0f;
        for (const auto& [type, time] : machine.gcode_time.times) {
            float remaining = include_remaining ? machine.time - total_time : 0.0f;
            ret.push_back({ type, { time, remaining } });
            total_time += time;
        }
    }
    return ret;
}

std::vector<std::pair<EMoveType, float>> GCodeProcessor::get_moves_time(PrintEstimatedTimeStatistics::ETimeMode mode) const
{
    std::vector<std::pair<EMoveType, float>> ret;
    if (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) {
        for (size_t i = 0; i < m_time_processor.machines[static_cast<size_t>(mode)].moves_time.size(); ++i) {
            float time = m_time_processor.machines[static_cast<size_t>(mode)].moves_time[i];
            if (time > 0.0f)
                ret.push_back({ static_cast<EMoveType>(i), time });
        }
    }
    return ret;
}

std::vector<std::pair<Anker::ExtrusionRole, float>> GCodeProcessor::get_roles_time(PrintEstimatedTimeStatistics::ETimeMode mode) const
{
    std::vector<std::pair<Anker::ExtrusionRole, float>> ret;
    if (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) {
        for (size_t i = 0; i < m_time_processor.machines[static_cast<size_t>(mode)].roles_time.size(); ++i) {
            float time = m_time_processor.machines[static_cast<size_t>(mode)].roles_time[i];
            if (time > 0.0f)
                ret.push_back({ static_cast<Anker::ExtrusionRole>(i), time });
        }
    }
    return ret;
}

std::vector<float> GCodeProcessor::get_layers_time(PrintEstimatedTimeStatistics::ETimeMode mode) const
{
    return (mode < PrintEstimatedTimeStatistics::ETimeMode::Count) ?
                m_time_processor.machines[static_cast<size_t>(mode)].layers_time :
        std::vector<float>();
    }

    std::string get_klipper_param(std::string key, std::string line) {
    size_t key_pos = line.find(key);
    if (key_pos == std::string::npos) {
        QString strTemp = QString::fromStdString(std::string(key).c_str());
        std::string key_lower =strTemp.toLower().toStdString();
        //boost::to_lower(key);
        key_pos = line.find(key_lower);
    }
    if (key_pos != std::string::npos) {
        size_t data_pos = key_pos + key.size();
        while (data_pos < line.size() && (line[data_pos] == ' ' || line[data_pos] == '='))
            data_pos++;
        if (data_pos < line.size()) {
            size_t end_pos = line.find(" ", data_pos);
            if (end_pos == std::string::npos)
                end_pos = line.size();
            return line.substr(data_pos, end_pos - data_pos);
        }
    }
    return "";
}

void GCodeProcessor::process_klipper_ACTIVATE_EXTRUDER(const GCodeReader::GCodeLine& line) {
    uint8_t extruder_id = 0;
    //check the config
    std::string raw_value = get_klipper_param(" EXTRUDER", line.raw());
    auto it = std::find(m_extruder_names.begin(), m_extruder_names.end(), raw_value);
    if ( it != m_extruder_names.end()) {
        process_T(uint8_t(it - m_extruder_names.begin()));
        return;
    }
    std::string trsf;
    while (raw_value.back() >= '0' && raw_value.back() <= '9') {
        trsf = raw_value.back() + trsf;
        raw_value.resize(raw_value.size() - 1);
    }
    if (trsf.empty())
        trsf = "0";
    process_T(uint8_t(std::stoi(trsf)));
}

void GCodeProcessor::process_gcode_line(const GCodeReader::GCodeLine& line)
{

    // update start position
    m_start_position = m_end_position;

    const std::string_view cmd = line.cmd();
    if (cmd.length() > 10 && m_flavor == Anker::GCodeFlavor::gcfKlipper) {
        try {
            QString strTemp = QString::fromStdString(std::string(cmd).c_str());
            std::string cmd_up =strTemp.toUpper().toStdString();
            //klipper extendt comands
            if (cmd_up == "TURN_OFF_HEATERS")
                m_temperature = 0;
            else if (cmd_up == "ACTIVATE_EXTRUDER")
                process_klipper_ACTIVATE_EXTRUDER(line);
        }
        catch (...) {
            //TODO:
            //BOOST_LOG_TRIVIAL(error) << "GCodeProcessor failed to parse the klipper command '" << line.raw() << "'.";
        }
    } else if (cmd.length() > 1) {
        // process command lines
        switch (::toupper(cmd[0]))
        {
        case 'G':
        {
            switch (::atoi(&cmd[1]))
            {
            case 0:  { process_G0(line); break; }  // Move
            case 1:  { process_G1(line); break; }  // Move
            case 2:  { process_G2_G3(line, false); break; }  // Move
            case 3:  { process_G2_G3(line, true); break; }  // Move
            case 10: { process_G10(line); break; } // Retract
            case 11: { process_G11(line); break; } // Unretract
            case 20: { process_G20(line); break; } // Set Units to Inches
            case 21: { process_G21(line); break; } // Set Units to Millimeters
            case 22: { process_G22(line); break; } // Firmware controlled retract
            case 23: { process_G23(line); break; } // Firmware controlled unretract
            case 90: { process_G90(line); break; } // Set to Absolute Positioning
            case 91: { process_G91(line); break; } // Set to Relative Positioning
            case 92: { process_G92(line); break; } // Set Position
            default: { break; }
            }
            break;
        }
        case 'M':
        {
            switch (::atoi(&cmd[1]))
            {
            case 1:   { process_M1(line); break; }   // Sleep or Conditional stop
            case 82:  { process_M82(line); break; }  // Set extruder to absolute mode
            case 83:  { process_M83(line); break; }  // Set extruder to relative mode
            case 104: { process_M104_M109(line); break; } // Set extruder temp
            case 106: { process_M106(line); break; } // Set fan speed
            case 107: { process_M107(line); break; } // Disable fan
            case 108: { process_M108(line); break; } // Set tool (Sailfish)
            case 109: { process_M104_M109(line); break; } // Set extruder temp
            case 132: { process_M132(line); break; } // Recall stored home offsets
            case 135: { process_M135(line); break; } // Set tool (MakerWare)
            case 201: { process_M201(line); break; } // Set max printing acceleration
            case 203: { process_M203(line); break; } // Set maximum feedrate
            case 204: { process_M204(line); break; } // Set default acceleration
            case 205: { process_M205(line); break; } // Advanced settings
            case 221: { process_M221(line); break; } // Set extrude factor override percentage
            case 401: { process_M401(line); break; } // Repetier: Store x, y and z position
            case 402: { process_M402(line); break; } // Repetier: Go to stored position
            case 566: { process_M566(line); break; } // Set allowable instantaneous speed change
            case 702: { process_M702(line); break; } // Unload the current filament into the MK3 MMU2 unit at the end of print.
            case 900: { process_M900(line); break; } 
            case 1024:{ process_M1024(line); break; }
            default: { break; }
            }
            break;
        }
        case 'T':
        {
            process_T(line); // Select Tool
            break;
        }
        default: { break; }
        }
    } else {
        const std::string &comment = line.raw();
        if (comment.length() > 2 && comment.front() == ';')
            // Process tags embedded into comments. Tag comments always start at the start of a line
            // with a comment and continue with a tag without any whitespace separator.
            process_tags(comment.substr(1));
    }
}

static inline bool starts_with(const std::string_view comment, const std::string_view tag)
{
    size_t tag_len = tag.size();
    return comment.size() >= tag_len && comment.substr(0, tag_len) == tag;
}

#if __has_include(<charconv>)
template <typename T, typename = void>
struct is_from_chars_convertible : std::false_type {};
template <typename T>
struct is_from_chars_convertible<T, std::void_t<decltype(std::from_chars(std::declval<const char*>(), std::declval<const char*>(), std::declval<T&>()))>> : std::true_type {};
#endif

// Returns true if the number was parsed correctly into out and the number spanned the whole input string.
template<typename T>
[[nodiscard]] static inline bool parse_number(const std::string_view sv, T &out)
{
    // https://www.bfilipek.com/2019/07/detect-overload-from-chars.html#example-stdfromchars
#if __has_include(<charconv>)
    // Visual Studio 19 supports from_chars all right.
    // OSX compiler that we use only implements std::from_chars just for ints.
    // GCC that we compile on does not provide <charconv> at all.
    if constexpr (is_from_chars_convertible<T>::value) {
        auto str_end = sv.data() + sv.size();
        auto [end_ptr, error_code] = std::from_chars(sv.data(), str_end, out);
                return error_code == std::errc() && end_ptr == str_end;
    }
                else
        #endif
        {
            // Legacy conversion, which is costly due to having to make a copy of the string before conversion.
            try {
                assert(sv.size() < 1024);
                assert(sv.data() != nullptr);
                std::string str { sv };
                size_t read = 0;
                if constexpr (std::is_same_v<T, int>)
                        out = std::stoi(str, &read);
                else if constexpr (std::is_same_v<T, long>)
                        out = std::stol(str, &read);
                else if constexpr (std::is_same_v<T, float>)
                        out = std::stof(str, &read);
                else if constexpr (std::is_same_v<T, double>)
                        out = std::stod(str, &read);
                return str.size() == read;
            } catch (...) {
                return false;
            }
        }
    }

    void GCodeProcessor::process_tags(const std::string_view comment)
    {
        // producers tags
        if (m_producers_enabled && process_producers_tags(comment))
            return;

        // extrusion role tag
        if (starts_with(comment, Extrusion_Role_Tag)) {
            m_extrusion_role = Anker::string_to_role(comment.substr(Extrusion_Role_Tag.length()));
            return;
        }

        // wipe start tag
        if (starts_with(comment, Wipe_Start_Tag)) {
            m_wiping = true;
            return;
        }

        // wipe end tag
        if (starts_with(comment, Wipe_End_Tag)) {
            m_wiping = false;
            return;
        }

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        if (!m_producers_enabled || m_producer == EProducer::PrusaSlicer || m_producer == EProducer::Slic3r || m_producer == EProducer::SuperSlicer) {
            // height tag
            if (starts_with(comment, Height_Tag)) {
                if (!parse_number(comment.substr(Height_Tag.size()), m_forced_height))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
                return;
            }
            // width tag
            if (starts_with(comment, Width_Tag)) {
                if (!parse_number(comment.substr(Width_Tag.size()), m_forced_width))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
                return;
            }
        }
#else
        if ((!m_producers_enabled || m_producer == EProducer::PrusaSlicer || m_producer == EProducer::Slic3r) &&
                starts_with(comment, Height_Tag)) {
            // height tag
            if (!parse_number(comment.substr(Height_Tag.size()), m_height))
                //TODO:
                //BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
                return;
        }

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        // width tag
        if (starts_with(comment, Width_Tag)) {
            if (! parse_number(comment.substr(Width_Tag.size()), m_width_compare.last_tag_value))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
            return;
        }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

        // color change tag
        if (starts_with(comment, Color_Change_Tag)) {
            unsigned char extruder_id = 0;
            if (starts_with(comment.substr(Color_Change_Tag.size()), ",T")) {
                int eid;
                if (! parse_number(comment.substr(Color_Change_Tag.size() + 2), eid) || eid < 0 || eid > 255) {
                    //TODO:
                    //BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Color_Change (" << comment << ").";
                    return;
                }
                extruder_id = static_cast<unsigned char>(eid);
            }

            m_extruder_colors[extruder_id] = static_cast<unsigned char>(m_extruder_offsets.size()) + m_cp_color.counter; // color_change position in list of color for preview
            ++m_cp_color.counter;
            if (m_cp_color.counter == UCHAR_MAX)
                m_cp_color.counter = 0;

            if (m_extruder_id == extruder_id) {
                m_cp_color.current = m_extruder_colors[extruder_id];
                store_move_vertex(EMoveType::Color_change);
            }

            process_custom_gcode_time(Anker::CustomGCode::ColorChange);

            return;
        }

        // pause print tag
        if (comment == Pause_Print_Tag) {
            store_move_vertex(EMoveType::Pause_Print);
            process_custom_gcode_time(Anker::CustomGCode::PausePrint);
            return;
        }

        // custom code tag
        if (comment == Custom_Code_Tag) {
            store_move_vertex(EMoveType::Custom_GCode);
            return;
        }

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        // mm3_per_mm print tag
        if (starts_with(comment, Mm3_Per_Mm_Tag)) {
            if (! parse_number(comment.substr(Mm3_Per_Mm_Tag.size()), m_mm3_per_mm_compare.last_tag_value))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Mm3_Per_Mm (" << comment << ").";
            return;
        }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

        // layer change tag
        if (comment == Layer_Change_Tag) {
            ++m_layer_id;
            return;
        }


    }

    bool GCodeProcessor::process_producers_tags(const std::string_view comment)
    {
        switch (m_producer)
        {
        case EProducer::Slic3rPE:
        case EProducer::Slic3r:
        case EProducer::PrusaSlicer: { return process_prusaslicer_tags(comment); }
        case EProducer::SuperSlicer: { return process_prusaslicer_tags(comment); }
        case EProducer::Cura:        { return process_cura_tags(comment); }
        case EProducer::Simplify3D:  { return process_simplify3d_tags(comment); }
        case EProducer::CraftWare:   { return process_craftware_tags(comment); }
        case EProducer::ideaMaker:   { return process_ideamaker_tags(comment); }
        case EProducer::KissSlicer:  { return process_kissslicer_tags(comment); }
        case EProducer::AnkerSlicer: { return process_cura_tags(comment); }
        default:                     { return false; }
        }
    }

    bool GCodeProcessor::process_prusaslicer_tags(const std::string_view comment)
    {
        return false;
    }

    bool GCodeProcessor::process_cura_tags(const std::string_view comment)
    {
        // TYPE -> extrusion role
        std::string tag = "TYPE:";
        size_t pos = comment.find(tag);
        if (pos != comment.npos) {
            const std::string_view type = comment.substr(pos + tag.length());
            if (type == "SKIRT")
                m_extrusion_role = Anker::erSkirt;
            else if (type == "WALL-OUTER"){
                m_extrusion_role = Anker::erExternalPerimeter;
                m_z_lap_flag = true;
            }
            else if (type == "WALL-INNER")
                m_extrusion_role = Anker::erPerimeter;
            else if (type == "SKIN")
                m_extrusion_role = Anker::erSolidInfill;
            else if (type == "FILL")
                m_extrusion_role = Anker::erInternalInfill;
            else if (type == "SUPPORT")
                m_extrusion_role = Anker::erSupportMaterial;
            else if (type == "SUPPORT-INTERFACE")
                m_extrusion_role = Anker::erSupportMaterialInterface;
            else if (type == "PRIME-TOWER")
                m_extrusion_role = Anker::erWipeTower;
            else {
                m_extrusion_role = Anker::erNone;
                //TODO:
                //BOOST_LOG_TRIVIAL(warning) << "GCodeProcessor found unknown extrusion role: " << type;
            }

            return true;
        }

        // flavor
        tag = "FLAVOR:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            const std::string_view flavor = comment.substr(pos + tag.length());
            if (flavor == "BFB")
                m_flavor = Anker::gcfMarlin; // << ???????????????????????
            else if (flavor == "Mach3")
                m_flavor = Anker::gcfMach3;
            else if (flavor == "Makerbot")
                m_flavor = Anker::gcfMakerWare;
            else if (flavor == "UltiGCode")
                m_flavor = Anker::gcfMarlin; // << ???????????????????????
            else if (flavor == "Marlin(Volumetric)")
                m_flavor = Anker::gcfMarlin; // << ???????????????????????
            else if (flavor == "Griffin")
                m_flavor = Anker::gcfMarlin; // << ???????????????????????
            else if (flavor == "Repetier")
                m_flavor = Anker::gcfRepetier;
            else if (flavor == "RepRap")
                m_flavor = Anker::gcfRepRap;
            else if (flavor == "Sprinter")
                m_flavor = Anker::gcfSprinter;
            else if (flavor == "Marlin")
                m_flavor = Anker::gcfMarlin;
            else

                //TODO:
                //BOOST_LOG_TRIVIAL(warning) << "GCodeProcessor found unknown flavor: " << flavor;

                return true;
        }

        // layer
        tag = "LAYER:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            ++m_layer_id;
            return true;
        }

        return false;
    }

    bool GCodeProcessor::process_simplify3d_tags(const std::string_view comment)
    {
        // extrusion roles

        // ; skirt
        size_t pos = comment.find(" skirt");
        if (pos == 0) {
            m_extrusion_role = Anker::erSkirt;
            return true;
        }

        // ; outer perimeter
        pos = comment.find(" outer perimeter");
        if (pos == 0) {
            m_extrusion_role = Anker::erExternalPerimeter;
            return true;
        }

        // ; inner perimeter
        pos = comment.find(" inner perimeter");
        if (pos == 0) {
            m_extrusion_role = Anker::erPerimeter;
            return true;
        }

        // ; gap fill
        pos = comment.find(" gap fill");
        if (pos == 0) {
            m_extrusion_role = Anker::erGapFill;
            return true;
        }

        // ; infill
        pos = comment.find(" infill");
        if (pos == 0) {
            m_extrusion_role = Anker::erInternalInfill;
            return true;
        }

        // ; solid layer
        pos = comment.find(" solid layer");
        if (pos == 0) {
            m_extrusion_role = Anker::erSolidInfill; //  change @2022-04-21 by CL
            return true;
        }

        // ; bridge (& internal bridge)
        pos = comment.find(" bridge");
        if (pos == 0) {
            m_extrusion_role = Anker::erBridgeInfill;
            return true;
        }

        // ; support
        pos = comment.find(" support");
        if (pos == 0) {
            m_extrusion_role = Anker::erSupportMaterial;
            return true;
        }

        // ; prime pillar
        pos = comment.find(" prime pillar");
        if (pos == 0) {
            m_extrusion_role = Anker::erWipeTower;
            return true;
        }

        // ; ooze shield
        pos = comment.find(" ooze shield");
        if (pos == 0) {
            m_extrusion_role = Anker::erWipeTower; // change @2022-04-21 by CL
            return true;
        }

        // ; raft
        pos = comment.find(" raft");
        if (pos == 0) {
            m_extrusion_role = Anker::erSkirt;
            return true;
        }

        // geometry
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        // ; tool
        std::string tag = " tool";
        pos = comment.find(tag);
        if (pos == 0) {
            const std::string_view data = comment.substr(pos + tag.length());
            std::string h_tag = "H";
            size_t h_start = data.find(h_tag);
            size_t h_end = data.find_first_of(' ', h_start);
            std::string w_tag = "W";
            size_t w_start = data.find(w_tag);
            size_t w_end = data.find_first_of(' ', w_start);
            if (h_start != data.npos) {
                if (!parse_number(data.substr(h_start + 1, (h_end != data.npos) ? h_end - h_start - 1 : h_end), m_forced_height))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
            }
            if (w_start != data.npos) {
                if (!parse_number(data.substr(w_start + 1, (w_end != data.npos) ? w_end - w_start - 1 : w_end), m_forced_width))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
            }

            return true;
        }

        // ; layer
        tag = " layer";
#else
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        // ; tool
        std::string tag = " tool";
        pos = comment.find(tag);
        if (pos == 0) {
            const std::string_view data = comment.substr(pos + tag.length());
            std::string h_tag = "H";
            size_t h_start = data.find(h_tag);
            size_t h_end = data.find_first_of(' ', h_start);
            std::string w_tag = "W";
            size_t w_start = data.find(w_tag);
            size_t w_end = data.find_first_of(' ', w_start);
            if (h_start != data.npos) {
                if (! parse_number(data.substr(h_start + 1, (h_end != data.npos) ? h_end - h_start - 1 : h_end), m_height_compare.last_tag_value))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
            }
            if (w_start != data.npos) {
                if (! parse_number(data.substr(w_start + 1, (w_end != data.npos) ? w_end - w_start - 1 : w_end), m_width_compare.last_tag_value))
                    BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
            }

            return true;
        }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

        // ; layer
        std::string tag = " layer";
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        pos = comment.find(tag);
        if (pos == 0) {
            // skip lines "; layer end"
            const std::string_view data = comment.substr(pos + tag.length());
            size_t end_start = data.find("end");
            if (end_start == data.npos)
                ++m_layer_id;

            return true;
        }

        return false;
    }

    bool GCodeProcessor::process_craftware_tags(const std::string_view comment)
    {
        // segType -> extrusion role
        std::string tag = "segType:";
        size_t pos = comment.find(tag);
        if (pos != comment.npos) {
            const std::string_view type = comment.substr(pos + tag.length());
            if (type == "Skirt")
                m_extrusion_role = Anker::erSkirt;
            else if (type == "Perimeter")
                m_extrusion_role = Anker::erExternalPerimeter;
            else if (type == "HShell")
                m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            else if (type == "InnerHair")
                m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            else if (type == "Loop")
                m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            else if (type == "Infill")
                m_extrusion_role = Anker::erInternalInfill;
            else if (type == "Raft")
                m_extrusion_role = Anker::erSkirt;
            else if (type == "Support")
                m_extrusion_role = Anker::erSupportMaterial;
            else if (type == "SupportTouch")
                m_extrusion_role = Anker::erSupportMaterial;
            else if (type == "SoftSupport")
                m_extrusion_role = Anker::erSupportMaterialInterface;
            else if (type == "Pillar")
                m_extrusion_role = Anker::erWipeTower;
            else {
                m_extrusion_role = Anker::erNone;
                //TODO:
                //BOOST_LOG_TRIVIAL(warning) << "GCodeProcessor found unknown extrusion role: " << type;
            }

            return true;
        }

        // layer
        pos = comment.find(" Layer #");
        if (pos == 0) {
            ++m_layer_id;
            return true;
        }

        return false;
    }

    bool GCodeProcessor::process_ideamaker_tags(const std::string_view comment)
    {
        // TYPE -> extrusion role
        std::string tag = "TYPE:";
        size_t pos = comment.find(tag);
        if (pos != comment.npos) {
            const std::string_view type = comment.substr(pos + tag.length());
            if (type == "RAFT")
                m_extrusion_role = Anker::erSkirt;
            else if (type == "WALL-OUTER")
                m_extrusion_role = Anker::erExternalPerimeter;
            else if (type == "WALL-INNER")
                m_extrusion_role = Anker::erPerimeter;
            else if (type == "SOLID-FILL")
                m_extrusion_role = Anker::erSolidInfill;
            else if (type == "FILL")
                m_extrusion_role = Anker::erInternalInfill;
            else if (type == "BRIDGE")
                m_extrusion_role = Anker::erBridgeInfill;
            else if (type == "SUPPORT")
                m_extrusion_role = Anker::erSupportMaterial;
            else {
                m_extrusion_role = Anker::erNone;
                //TODO:
                //BOOST_LOG_TRIVIAL(warning) << "GCodeProcessor found unknown extrusion role: " << type;
            }
            return true;
        }

        // geometry
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        // width
        tag = "WIDTH:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            if (!parse_number(comment.substr(pos + tag.length()), m_forced_width))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
            return true;
        }

        // height
        tag = "HEIGHT:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            if (!parse_number(comment.substr(pos + tag.length()), m_forced_height))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
            return true;
        }
#else
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
        // width
        tag = "WIDTH:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            if (! parse_number(comment.substr(pos + tag.length()), m_width_compare.last_tag_value))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Width (" << comment << ").";
            return true;
        }

        // height
        tag = "HEIGHT:";
        pos = comment.find(tag);
        if (pos != comment.npos) {
            if (! parse_number(comment.substr(pos + tag.length()), m_height_compare.last_tag_value))
                BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid value for Height (" << comment << ").";
            return true;
        }
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

        // layer
        pos = comment.find("LAYER:");
        if (pos == 0) {
            ++m_layer_id;
            return true;
        }

        return false;
    }

    bool GCodeProcessor::process_kissslicer_tags(const std::string_view comment)
    {
        // extrusion roles

        // ; 'Raft Path'
        size_t pos = comment.find(" 'Raft Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erSkirt;
            return true;
        }

        // ; 'Support Interface Path'
        pos = comment.find(" 'Support Interface Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erSupportMaterialInterface;
            return true;
        }

        // ; 'Travel/Ironing Path'
        pos = comment.find(" 'Travel/Ironing Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erIroning;
            return true;
        }

        // ; 'Support (may Stack) Path'
        pos = comment.find(" 'Support (may Stack) Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erSupportMaterial;
            return true;
        }

        // ; 'Perimeter Path'
        pos = comment.find(" 'Perimeter Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erExternalPerimeter;
            return true;
        }

        // ; 'Pillar Path'
        pos = comment.find(" 'Pillar Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            return true;
        }

        // ; 'Destring/Wipe/Jump Path'
        pos = comment.find(" 'Destring/Wipe/Jump Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            return true;
        }

        // ; 'Prime Pillar Path'
        pos = comment.find(" 'Prime Pillar Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            return true;
        }

        // ; 'Loop Path'
        pos = comment.find(" 'Loop Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            return true;
        }

        // ; 'Crown Path'
        pos = comment.find(" 'Crown Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            return true;
        }

        // ; 'Solid Path'
        pos = comment.find(" 'Solid Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erNone;
            return true;
        }

        // ; 'Stacked Sparse Infill Path'
        pos = comment.find(" 'Stacked Sparse Infill Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erInternalInfill;
            return true;
        }

        // ; 'Sparse Infill Path'
        pos = comment.find(" 'Sparse Infill Path'");
        if (pos == 0) {
            m_extrusion_role = Anker::erSolidInfill;
            return true;
        }

        // geometry

        // layer
        pos = comment.find(" BEGIN_LAYER_");
        if (pos == 0) {
            ++m_layer_id;
            return true;
        }

        return false;
    }

    bool GCodeProcessor::detect_producer(const std::string_view comment)
    {
        for (const auto& [id, search_string] : Producers) {
            size_t pos = comment.find(search_string);
            if (pos != comment.npos) {
                m_producer = id;
                //TODO:
                //BOOST_LOG_TRIVIAL(info) << "Detected gcode producer: " << search_string;
                return true;
            }
        }
        return false;
    }

    void GCodeProcessor::process_G0(const GCodeReader::GCodeLine& line)
    {
        process_G1(line);
    }

    void GCodeProcessor::process_G1(const GCodeReader::GCodeLine& line)
    {
#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
        float filament_diameter = (static_cast<size_t>(m_extruder_id) < m_filament_diameters.size()) ? m_filament_diameters[m_extruder_id] : m_filament_diameters.back();
        float filament_radius = 0.5f * filament_diameter;
        double area_filament_cross_section = M_PI * sqr(filament_radius);
        auto absolute_position = [this, area_filament_cross_section](Axis axis, const GCodeReader::GCodeLine& lineG1) {
#else
        auto absolute_position = [this](Axis axis, const GCodeReader::GCodeLine& lineG1) {
#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
            bool is_relative = (m_global_positioning_type == EPositioningType::Relative);
            if (axis == E)
                is_relative |= (m_e_local_positioning_type == EPositioningType::Relative);

            if (lineG1.has(Axis(axis))) {
                float lengthsScaleFactor = (m_units == EUnits::Inches) ? INCHES_TO_MM : 1.0f;
                double ret = lineG1.value(Axis(axis)) * lengthsScaleFactor;
#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
                if (axis == E && m_use_volumetric_e)
                    ret /= area_filament_cross_section;
#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
                return is_relative ? m_start_position[axis] + ret : m_origin[axis] + ret;
            }
            else
                return m_start_position[axis];
        };

        auto move_type = [this](const AxisCoords& delta_pos) {
            EMoveType type = EMoveType::Noop;

            if (m_wiping)
                type = EMoveType::Wipe;
            else if (delta_pos[E] < 0.0f)
                type = (delta_pos[X] != 0.0f || delta_pos[Y] != 0.0f || delta_pos[Z] != 0.0f) ? EMoveType::Travel : EMoveType::Retract;
            else if (delta_pos[E] > 0.0f) {
                if (delta_pos[X] == 0.0f && delta_pos[Y] == 0.0f)
                    type = (delta_pos[Z] == 0.0f) ? EMoveType::Unretract : EMoveType::Travel;
                else if (delta_pos[X] != 0.0f || delta_pos[Y] != 0.0f)
                    type = EMoveType::Extrude;
            }
            else if (delta_pos[X] != 0.0f || delta_pos[Y] != 0.0f || delta_pos[Z] != 0.0f)
                type = EMoveType::Travel;

            return type;
        };

        ++m_g1_line_id;

        //reprap thingy, ignore
        if (line.has('R') || line.has('H'))
            return;

        //record z_lap pos . 20220705 friva;
        if (line.has('E') && m_z_lap_flag)
        {
            m_z_lap_flag = false;
            store_move_vertex(EMoveType::Zlap);
        }

        // enable processing of lines M201/M203/M204/M205
        m_time_processor.machine_envelope_processing_enabled = true;

        // updates axes positions from line
        for (unsigned char a = X; a <= E; ++a) {
            m_end_position[a] = absolute_position((Axis)a, line);
        }

        // updates feedrate from line, if present
        if (line.has_f())
            m_feedrate = line.f() * MMMIN_TO_MMSEC;

        // calculates movement deltas
        double max_abs_delta = 0.0f;
        AxisCoords delta_pos;
        AxisCoords delta_abs_pos;   //  add @2022-09-07 by CL
        for (unsigned char a = X; a <= E; ++a) {
            delta_pos[a] = m_end_position[a] - m_start_position[a];
            delta_abs_pos[a] = std::abs(delta_pos[a]);
            max_abs_delta = std::max(max_abs_delta, delta_abs_pos[a]);
        }

        // no displacement, return
        if (max_abs_delta == 0.0f)
            return;

        EMoveType type = move_type(delta_pos);
        if (type == EMoveType::Extrude && m_end_position[Z] == 0.0f)
            type = EMoveType::Travel;

        float height_saved = -1;
        if (type == EMoveType::Extrude) {
            double delta_xyz = std::sqrt(sqr(delta_pos[X]) + sqr(delta_pos[Y]) + sqr(delta_pos[Z]));
#if !ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
            float filament_diameter = (static_cast<size_t>(m_extruder_id) < m_filament_diameters.size()) ? m_filament_diameters[m_extruder_id] : m_filament_diameters.back();
            float filament_radius = 0.5f * filament_diameter;
            double area_filament_cross_section = static_cast<float>(M_PI) * sqr(filament_radius);
#endif // !ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
            double volume_extruded_filament = area_filament_cross_section * delta_pos[E];
            double area_toolpath_cross_section = volume_extruded_filament / delta_xyz;

            // volume extruded filament / tool displacement = area toolpath cross section
            m_mm3_per_mm = float(area_toolpath_cross_section);
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
            m_mm3_per_mm_compare.update(area_toolpath_cross_section, m_extrusion_role);
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
            if (m_forced_height > 0.0f)
                m_height = m_forced_height;
            else {
                if (m_end_position[Z] > m_extruded_last_z + EPSILON) {
                    m_height = float(m_end_position[Z] - m_extruded_last_z);
                    m_extruded_last_z = m_end_position[Z];
                }
            }

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
            m_height_compare.update(m_height, m_extrusion_role);
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
#else
            if ((m_producers_enabled && m_producer != EProducer::PrusaSlicer || m_producer == EProducer::SuperSlicer || m_producer == EProducer::Slic3r) || m_height == 0.0f) {
                if (m_end_position[Z] > m_extruded_last_z + EPSILON) {
                    m_height = float(m_end_position[Z] - m_extruded_last_z);
#if ENABLE_GCODE_VIEWER_DATA_CHECKING
                    m_height_compare.update(m_height, m_extrusion_role);
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
                    m_extruded_last_z = m_end_position[Z];
                }
            }
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
            if (m_forced_width > 0.0f)
                m_width = m_forced_width;
            else {
                if (m_extrusion_role == Anker::erExternalPerimeter)
#else
            {
                if (m_extrusion_role == Anker::erExternalPerimeter)
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
                    // cross section: rectangle
                    m_width = float(delta_pos[E] * (M_PI * sqr(1.05f * filament_radius)) / (delta_xyz * m_height));
                else if (Anker::is_bridge(m_extrusion_role) || m_extrusion_role == Anker::erNone)
                    // cross section: circle
                    m_width = float(m_filament_diameters[m_extruder_id] * std::sqrt(delta_pos[E] / delta_xyz));
                else
                    // cross section: rectangle + 2 semicircles
                    m_width = float(delta_pos[E] * (M_PI * sqr(filament_radius)) / (delta_xyz * m_height) + (1.0 - 0.25 * M_PI) * m_height);

                // if the value seems wrong, fall back to circular extrusion from flow
                //change by friva.xie  220518
                if (m_width > m_height * 5 || m_width < m_height) {
                    //m_width = 2 * std::sqrt(m_mm3_per_mm / float(M_PI));
                    //height_saved = m_height;
                    //m_height = m_width;
                    m_width = m_height;
                }
            }

#if ENABLE_GCODE_VIEWER_DATA_CHECKING
            m_width_compare.update(m_width, m_extrusion_role);
#endif // ENABLE_GCODE_VIEWER_DATA_CHECKING
        }

        if (type == EMoveType::Extrude && (m_extrusion_role == Anker::erCustom || m_width == 0.0f || m_height == 0.0f))
            type = EMoveType::Travel;

        // time estimate section
        auto move_length = [](const AxisCoords& delta_pos) {
            float sq_xyz_length = (float)sqr(delta_pos[X]) + (float)sqr(delta_pos[Y]) + (float)sqr(delta_pos[Z]);
            return (sq_xyz_length > 0.0f) ? std::sqrt(sq_xyz_length) : std::abs(delta_pos[E]);
        };

        auto is_extrusion_only_move = [](const AxisCoords& delta_pos) {
            return delta_pos[X] == 0.0f && delta_pos[Y] == 0.0f && delta_pos[Z] == 0.0f && delta_pos[E] != 0.0f;
        };

        float distance = (float)move_length(delta_pos);
        assert(distance != 0.0f);
        float inv_distance = 1.0f / distance;

        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            TimeMachine& machine = m_time_processor.machines[i];
            if (!machine.enabled)
                continue;

            TimeMachine::State& curr = machine.curr;
            TimeMachine::State& prev = machine.prev;
            std::vector<TimeBlock>& blocks = machine.blocks;

            curr.feedrate = (delta_pos[E] == 0.0f) ?
                        minimum_travel_feedrate(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), m_feedrate) :
                        minimum_feedrate(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), m_feedrate);

            TimeBlock block;
            block.move_type = type;
            block.role = m_extrusion_role;
            block.distance = distance;
            block.g1_line_id = m_g1_line_id;
            block.layer_id = m_layer_id;

            // calculates block cruise feedrate
            double min_feedrate_factor = 1.0f;
            for (unsigned char a = X; a <= E; ++a) {
                curr.axis_feedrate[a] = curr.feedrate * delta_pos[a] * inv_distance;
                if (a == E)
                    curr.axis_feedrate[a] *= machine.extrude_factor_override_percentage;

                curr.abs_axis_feedrate[a] = std::abs(curr.axis_feedrate[a]);
                if (curr.abs_axis_feedrate[a] != 0.0f) {
                    double axis_max_feedrate = get_axis_max_feedrate(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), static_cast<Axis>(a));
                    if (axis_max_feedrate != 0.0f)
                        min_feedrate_factor = std::min(min_feedrate_factor, axis_max_feedrate / curr.abs_axis_feedrate[a]);
                }
            }

            block.feedrate_profile.cruise = float(min_feedrate_factor * curr.feedrate);
            curr.feedrate = block.feedrate_profile.cruise;  //  @2022-05-29 by CL

            if (min_feedrate_factor < 1.0f) {
                for (unsigned char a = X; a <= E; ++a) {
                    curr.axis_feedrate[a] *= min_feedrate_factor;
                    curr.abs_axis_feedrate[a] *= min_feedrate_factor;
                }
            }

            // calculates block acceleration
            float acceleration = is_extrusion_only_move(delta_pos) ?
                        get_retract_acceleration(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i)) :
                        get_acceleration(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i));

            
            do{
                //  const uint32_t max_accel_steps_per_s2 = MAX_E_JERK(extruder) / (extruder_advance_K[active_extruder] * block->e_D_ratio) * steps_per_mm;
                //  if (TERN0(LA_DEBUG, accel > max_accel_steps_per_s2))
                //    SERIAL_ECHOLNPGM("Acceleration limited.");
                //  NOMORE(accel, max_accel_steps_per_s2);

                // K=0.06, jerk_e = 3
                const double advance_K  = m_time_processor.machine_limits.machine_lin_advance_k.values[i];
                const double max_jerk_e = m_time_processor.machine_limits.machine_max_jerk_e.values[i];;

                double delta_xyz = std::sqrt(sqr(delta_pos[X]) + sqr(delta_pos[Y]) + sqr(delta_pos[Z]));
                double delta_e   = std::abs(delta_pos[E]);
                if(delta_e > 0){
                    if(delta_xyz > 0 && advance_K > 0){
                        double e_D_ratio  = delta_pos[E] / delta_xyz;

                        float max_acc_ek = max_jerk_e / (e_D_ratio * advance_K);
                        acceleration = acceleration < max_acc_ek ? acceleration : max_acc_ek;
                    }
                }

            }while(false);

            for (unsigned char a = X; a <= E; ++a) {
                float axis_max_acceleration = get_axis_max_acceleration(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), static_cast<Axis>(a));
                if (acceleration * std::abs(delta_pos[a]) * inv_distance > axis_max_acceleration)
                    acceleration = axis_max_acceleration;
            }

            block.acceleration = acceleration; 

            // calculates block exit feedrate
            curr.safe_feedrate = block.feedrate_profile.cruise;

            for (unsigned char a = X; a <= E; ++a) {
                float axis_max_jerk = get_axis_max_jerk(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), static_cast<Axis>(a));
                if (curr.abs_axis_feedrate[a] > axis_max_jerk)
                    curr.safe_feedrate = std::min(curr.safe_feedrate, axis_max_jerk);
            }

            block.feedrate_profile.exit = curr.safe_feedrate;

            static const float PREVIOUS_FEEDRATE_THRESHOLD = 0.0001f;

            // calculates block entry feedrate
            float vmax_junction = curr.safe_feedrate;
            if (!blocks.empty() && prev.feedrate > PREVIOUS_FEEDRATE_THRESHOLD) {
                bool prev_speed_larger = prev.feedrate > block.feedrate_profile.cruise;
                float smaller_speed_factor = prev_speed_larger ? (block.feedrate_profile.cruise / prev.feedrate) : (prev.feedrate / block.feedrate_profile.cruise);
                // Pick the smaller of the nominal speeds. Higher speed shall not be achieved at the junction during coasting.
                vmax_junction = prev_speed_larger ? block.feedrate_profile.cruise : prev.feedrate;

                float v_factor = 1.0f;
                bool limited = false;

                for (unsigned char a = X; a <= E; ++a) {
                    // Limit an axis. We have to differentiate coasting from the reversal of an axis movement, or a full stop.
                    double v_exit = prev.axis_feedrate[a];
                    double v_entry = curr.axis_feedrate[a];

                    if (prev_speed_larger)
                        v_exit *= smaller_speed_factor;

                    if (limited) {
                        v_exit *= v_factor;
                        v_entry *= v_factor;
                    }

                    // Calculate the jerk depending on whether the axis is coasting in the same direction or reversing a direction.
                    double jerk =
                            (v_exit > v_entry) ?
                                (((v_entry > 0.0f) || (v_exit < 0.0f)) ?
                                     // coasting
                                     (v_exit - v_entry) :
                                     // axis reversal
                                     std::max(v_exit, -v_entry)) :
                                // v_exit <= v_entry
                                (((v_entry < 0.0f) || (v_exit > 0.0f)) ?
                                     // coasting
                                     (v_entry - v_exit) :
                                     // axis reversal
                                     std::max(-v_exit, v_entry));

                    float axis_max_jerk = get_axis_max_jerk(static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i), static_cast<Axis>(a));
                    if (float(jerk) > axis_max_jerk) {
                        v_factor *= axis_max_jerk / float(jerk);
                        limited = true;
                    }
                }

                if (limited)
                    vmax_junction *= v_factor;

                // Now the transition velocity is known, which maximizes the shared exit / entry velocity while
                // respecting the jerk factors, it may be possible, that applying separate safe exit / entry velocities will achieve faster prints.
                float vmax_junction_threshold = vmax_junction * 0.99f;

                // Not coasting. The machine will stop and start the movements anyway, better to start the segment from start.
                if ((prev.safe_feedrate > vmax_junction_threshold) && (curr.safe_feedrate > vmax_junction_threshold))
                    vmax_junction = curr.safe_feedrate;
            }

            float v_allowable = max_allowable_speed(-acceleration, curr.safe_feedrate, block.distance);
            block.feedrate_profile.entry = std::min(vmax_junction, v_allowable);

            block.max_entry_speed = vmax_junction;      
            block.flags.nominal_length = (block.feedrate_profile.cruise <= v_allowable);
            block.flags.recalculate = true;
            block.safe_feedrate = curr.safe_feedrate;   
            block.axis_feedrate = curr.axis_feedrate;   //  add @2022-05-29 by CL
            
            

            // calculates block trapezoid
            block.calculate_trapezoid();

            // updates previous
            prev = curr;

            blocks.push_back(block);

            if (blocks.size() > TimeProcessor::Planner::refresh_threshold)
                machine.calculate_time(TimeProcessor::Planner::queue_size);
        }

        // store move
        store_move_vertex(type);

        //restore
        if(height_saved > 0)
            m_height = height_saved;
    }

    void GCodeProcessor::emit_G1_from_G2(Vec2d dest, float e, float f) {
        GCodeReader::FakeGCodeLine line_fake;
        line_fake.set_x(dest.x());
        line_fake.set_y(dest.y());
        //line_fake.set_z(dest.z());
        line_fake.set_e(e);
        if( f > 0)
            line_fake.set_f(f);
        process_G1(line_fake);
    }

    void GCodeProcessor::process_G2_G3(const GCodeReader::GCodeLine& line, bool direct)
    {
        //check it has everything
        float i = 0, j = 0;
        bool has_i = line.has_value('I', i);
        bool has_j = line.has_value('J', j);
        if(!((line.has_x() || line.has_y()) && (has_i || has_j)))
            return;
        //compute points
        //  compute mult factor
        float lengthsScaleFactor = (m_units == EUnits::Inches) ? INCHES_TO_MM : 1.0f;
        Vec2d p_start = { m_end_position[Axis::X], m_end_position[Axis::Y] };
        Vec2d p_end = { line.x() * lengthsScaleFactor, line.y() * lengthsScaleFactor };
        Vec2d p_center = { i * lengthsScaleFactor, j * lengthsScaleFactor };
        p_center += p_start;
        //  if relative positioning
        if ((m_global_positioning_type == EPositioningType::Relative)) {
            Vec2d p_to_add = { m_start_position[Axis::X], m_start_position[Axis::Y] };
            p_end += p_to_add;
            p_center += p_to_add;
        }
        // get missing values
        if (!line.has_x())
            p_end.x() = p_start.x();
        if (!line.has_y())
            p_end.y() = p_start.y();
        if (!has_i)
            p_center.x() = p_start.x();
        if (!has_j)
            p_center.y() = p_start.y();

        //compute angles
        double min_dist = m_width == 0 ? 1 : m_width * 4;
        const double pi2 = 2 * M_PI;
        const double radius = (p_start - p_center).norm();
        const double radius2 = (p_end - p_center).norm();
        if (std::abs(radius - radius2) > min_dist*0.1) {
            //TODO:
            //BOOST_LOG_TRIVIAL(error) << "error, radius from start & end are too different in command '" << line.raw() << "'.";
            return;
        }
        Vec2d p_start_rel = p_start - p_center;
        Vec2d p_end_rel = p_end - p_center;
        const double a1 = atan2(p_start_rel.y(), p_start_rel.x());
        const double a2 = atan2(p_end_rel.y(), p_end_rel.x());
        double adiff = a2 - a1;
        //if (a1 < 0)
        //    a1 += pi2;
        //if (a2 < 0)
        //    a2 += pi2;
        if (adiff > pi2)
            adiff -= pi2;
        if (adiff < -pi2)
            adiff += pi2;
        //check order
        if (direct) {
            if (adiff < 0)
                adiff += pi2;
        } else {
            if (adiff > 0)
                adiff -= pi2;
        }
        double distance = std::abs(adiff * radius);
        //get E
        float dE = 0;
        float start_e = m_start_position[E];
        bool e_relative = true;
        if (line.has_e()) {
            e_relative = (m_e_local_positioning_type == EPositioningType::Relative);
            double ret = line.e() * lengthsScaleFactor;
#if ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
            if (m_use_volumetric_e) {
                float filament_diameter = (static_cast<size_t>(m_extruder_id) < m_filament_diameters.size()) ? m_filament_diameters[m_extruder_id] : m_filament_diameters.back();
                float filament_radius = 0.5f * filament_diameter;
                double area_filament_cross_section = M_PI * sqr(filament_radius);
                ret /= area_filament_cross_section;
            }
#endif // ENABLE_VOLUMETRIC_EXTRUSION_PROCESSING
            dE = e_relative ? ret : m_origin[E] + ret - start_e;
        }

        //compute how much sections we need (~1 per 4 * width/nozzle)
        int nb_sections = std::min(30, 1 + int(distance / min_dist));
        Vec2d p_current = p_start;
        float angle_incr = adiff / nb_sections;
        float dE_incr = dE / nb_sections;
        float current_angle = a1;
        //create smaller sections
        for (int i = 1; i < nb_sections;i++) {
            current_angle += angle_incr;
            p_current = { std::cos(current_angle) * radius, std::sin(current_angle) * radius };
            p_current += p_center;
            emit_G1_from_G2(p_current, e_relative ? dE_incr : start_e + i * dE_incr, line.has_f() ? line.f() : -1);
            // update start position for next fake G1
            m_start_position[X] = p_current.x();
            m_start_position[Y] = p_current.y();
            m_start_position[E] += dE_incr;
        }
        //emit last
        emit_G1_from_G2(p_end, e_relative ? dE_incr : start_e + dE, line.has_f() ? line.f() : -1);

    }

    void GCodeProcessor::process_G10(const GCodeReader::GCodeLine& line)
    {
        // try to guess the meaning of this
        if (!line.has('P'))
            // stores retract move
            store_move_vertex(EMoveType::Retract);
        else if (line.has('S'))
            process_M104_M109(line);
    }

    void GCodeProcessor::process_G11(const GCodeReader::GCodeLine& line)
    {
        // stores unretract move
        store_move_vertex(EMoveType::Unretract);
    }

    void GCodeProcessor::process_G20(const GCodeReader::GCodeLine& line)
    {
        m_units = EUnits::Inches;
    }

    void GCodeProcessor::process_G21(const GCodeReader::GCodeLine& line)
    {
        m_units = EUnits::Millimeters;
    }

    void GCodeProcessor::process_G22(const GCodeReader::GCodeLine& line)
    {
        // stores retract move
        store_move_vertex(EMoveType::Retract);
    }

    void GCodeProcessor::process_G23(const GCodeReader::GCodeLine& line)
    {
        // stores unretract move
        store_move_vertex(EMoveType::Unretract);
    }

    void GCodeProcessor::process_G90(const GCodeReader::GCodeLine& line)
    {
        m_global_positioning_type = EPositioningType::Absolute;
    }

    void GCodeProcessor::process_G91(const GCodeReader::GCodeLine& line)
    {
        m_global_positioning_type = EPositioningType::Relative;
    }

    void GCodeProcessor::process_G92(const GCodeReader::GCodeLine& line)
    {
        float lengths_scale_factor = (m_units == EUnits::Inches) ? INCHES_TO_MM : 1.0f;
        bool any_found = false;

        if (line.has_x()) {
            m_origin[X] = m_end_position[X] - line.x() * lengths_scale_factor;
            any_found = true;
        }

        if (line.has_y()) {
            m_origin[Y] = m_end_position[Y] - line.y() * lengths_scale_factor;
            any_found = true;
        }

        if (line.has_z()) {
            m_origin[Z] = m_end_position[Z] - line.z() * lengths_scale_factor;
            any_found = true;
        }

        if (line.has_e()) {
            // extruder coordinate can grow to the point where its float representation does not allow for proper addition with small increments,
            // we set the value taken from the G92 line as the new current position for it
            m_end_position[E] = line.e() * lengths_scale_factor;
            any_found = true;
        }
        else
            simulate_st_synchronize();

        if (!any_found && !line.has_unknown_axis()) {
            // The G92 may be called for axes that PrusaSlicer does not recognize, for example see GH issue #3510,
            // where G92 A0 B0 is called although the extruder axis is till E.
            for (unsigned char a = X; a <= E; ++a) {
                m_origin[a] = m_end_position[a];
            }
        }
    }

    void GCodeProcessor::process_M1(const GCodeReader::GCodeLine& line)
    {
        simulate_st_synchronize();
    }

    void GCodeProcessor::process_M82(const GCodeReader::GCodeLine& line)
    {
        m_e_local_positioning_type = EPositioningType::Absolute;
    }

    void GCodeProcessor::process_M83(const GCodeReader::GCodeLine& line)
    {
        m_e_local_positioning_type = EPositioningType::Relative;
    }

    void GCodeProcessor::process_M104_M109(const GCodeReader::GCodeLine& line)
    {
        float new_temp;
        if (line.has_value('S', new_temp))
        {
            m_temperature = new_temp;
        }
    }

    void GCodeProcessor::process_M106(const GCodeReader::GCodeLine& line)
    {
        if (!line.has('P')) {
            // The absence of P means the print cooling fan, so ignore anything else.
            float new_fan_speed;
            if (line.has_value('S', new_fan_speed))
                m_fan_speed = (100.0f / 255.0f) * new_fan_speed;
            else
                m_fan_speed = 100.0f;
        }
    }

    void GCodeProcessor::process_M107(const GCodeReader::GCodeLine& line)
    {
        m_fan_speed = 0.0f;
    }

    void GCodeProcessor::process_M108(const GCodeReader::GCodeLine& line)
    {
        // These M-codes are used by Sailfish to change active tool.
        // They have to be processed otherwise toolchanges will be unrecognised
        // by the analyzer - see https://github.com/prusa3d/PrusaSlicer/issues/2566

        if (m_flavor != Anker::gcfSailfish)
            return;

        std::string cmd = line.raw();
        size_t pos = cmd.find("T");
        if (pos != std::string::npos)
            process_T(cmd.substr(pos));
    }

    void GCodeProcessor::process_M132(const GCodeReader::GCodeLine& line)
    {
        // This command is used by Makerbot to load the current home position from EEPROM
        // see: https://github.com/makerbot/s3g/blob/master/doc/GCodeProtocol.md
        // Using this command to reset the axis origin to zero helps in fixing: https://github.com/prusa3d/PrusaSlicer/issues/3082

        if (line.has_x())
            m_origin[X] = 0.0f;

        if (line.has_y())
            m_origin[Y] = 0.0f;

        if (line.has_z())
            m_origin[Z] = 0.0f;

        if (line.has_e())
            m_origin[E] = 0.0f;
    }

    void GCodeProcessor::process_M135(const GCodeReader::GCodeLine& line)
    {
        // These M-codes are used by MakerWare to change active tool.
        // They have to be processed otherwise toolchanges will be unrecognised
        // by the analyzer - see https://github.com/prusa3d/PrusaSlicer/issues/2566

        if (m_flavor != Anker::gcfMakerWare)
            return;

        std::string cmd = line.raw();
        size_t pos = cmd.find("T");
        if (pos != std::string::npos)
            process_T(cmd.substr(pos));
    }

    void GCodeProcessor::process_M201(const GCodeReader::GCodeLine& line)
    {
        // see http://reprap.org/wiki/G-code#M201:_Set_max_printing_acceleration
        float factor = ((m_flavor != Anker::gcfSprinter && m_flavor != Anker::gcfRepRap) && m_units == EUnits::Inches) ? INCHES_TO_MM : 1.0f;
        //TODO:
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i) == PrintEstimatedTimeStatistics::ETimeMode::Normal ||
                    m_time_processor.machine_envelope_processing_enabled) {
                if (line.has_x())
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_x, i, line.x() * factor);

                if (line.has_y())
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_y, i, line.y() * factor);

                if (line.has_z())
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_z, i, line.z() * factor);

                if (line.has_e())
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_e, i, line.e() * factor);
                TDebug(QString("M201_A_MAX: %1, X=%2, Y=%3, Z=%4, E=%5")
                       .arg(QString::fromStdString(line.raw()))
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_x.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_y.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_z.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_e.values[i])
                       );
            }
        }
    }

    void GCodeProcessor::process_M203(const GCodeReader::GCodeLine& line)
    {
        // see http://reprap.org/wiki/G-code#M203:_Set_maximum_feedrate
        if (m_flavor == Anker::gcfRepetier)
            return;

        // see http://reprap.org/wiki/G-code#M203:_Set_maximum_feedrate
        // http://smoothieware.org/supported-g-codes
        float factor = (m_flavor == Anker::gcfMarlin || m_flavor == Anker::gcfSmoothie) ? 1.0f : MMMIN_TO_MMSEC;
        //TODO:
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i) == PrintEstimatedTimeStatistics::ETimeMode::Normal ||
                    m_time_processor.machine_envelope_processing_enabled) {
                if (line.has_x())
                    set_option_value(m_time_processor.machine_limits.machine_max_feedrate_x, i, line.x() * factor);

                if (line.has_y())
                    set_option_value(m_time_processor.machine_limits.machine_max_feedrate_y, i, line.y() * factor);

                if (line.has_z())
                    set_option_value(m_time_processor.machine_limits.machine_max_feedrate_z, i, line.z() * factor);

                if (line.has_e())
                    set_option_value(m_time_processor.machine_limits.machine_max_feedrate_e, i, line.e() * factor);
                TDebug(QString("M203_V_MAX: %1, X=%2, Y=%3, Z=%4, E=%5")
                       .arg(QString::fromStdString(line.raw()))
                       .arg(m_time_processor.machine_limits.machine_max_feedrate_x.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_feedrate_y.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_feedrate_z.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_feedrate_e.values[i])
                       );
            }
        }
    }

    void GCodeProcessor::process_M204(const GCodeReader::GCodeLine& line)
    {
        float value;
        //TODO:
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i) == PrintEstimatedTimeStatistics::ETimeMode::Normal ||
                    m_time_processor.machine_envelope_processing_enabled) {
                if (line.has_value('S', value)) {
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_extruding, i, value);
                    set_option_value(m_time_processor.machine_limits.machine_max_acceleration_travel, i, value);
                }
                else {
                    // New acceleration format, compatible with the upstream Marlin.
                    if (line.has_value('P', value))
                        set_option_value(m_time_processor.machine_limits.machine_max_acceleration_extruding, i, value);
                    if (line.has_value('R', value))
                        set_option_value(m_time_processor.machine_limits.machine_max_acceleration_retracting, i, value);
                    if (line.has_value('T', value)) {
                        set_option_value(m_time_processor.machine_limits.machine_max_acceleration_travel, i, value);
                    }
                }

                TDebug(QString("M204_A_SET: %1, EX=%2, RE=%3, TR=%4")
                       .arg(QString::fromStdString(line.raw()))
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_extruding.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_retracting.values[i])
                       .arg(m_time_processor.machine_limits.machine_max_acceleration_travel.values[i])
                       );
            }
        }
    }

    void GCodeProcessor::process_M205(const GCodeReader::GCodeLine& line)
    {
        // Note: Sprinter / Marlin gcode
        //TODO:
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i) == PrintEstimatedTimeStatistics::ETimeMode::Normal ||
                    m_time_processor.machine_envelope_processing_enabled) {
                if (line.has_x()) {
                    float max_jerk = line.x();
                    set_option_value(m_time_processor.machine_limits.machine_max_jerk_x, i, max_jerk);
                    set_option_value(m_time_processor.machine_limits.machine_max_jerk_y, i, max_jerk);
                }

                if (line.has_y())
                    set_option_value(m_time_processor.machine_limits.machine_max_jerk_y, i, line.y());

                if (line.has_z())
                    set_option_value(m_time_processor.machine_limits.machine_max_jerk_z, i, line.z());

                if (line.has_e())
                    set_option_value(m_time_processor.machine_limits.machine_max_jerk_e, i, line.e());

                float value;
                if (line.has_value('S', value))
                    set_option_value(m_time_processor.machine_limits.machine_min_extruding_rate, i, value);

                if (line.has_value('T', value))
                    set_option_value(m_time_processor.machine_limits.machine_min_travel_rate, i, value);

                if(line.has_x() || line.has_y() || line.has_z() || line.has_e()){
                    TDebug(QString("M205_J_MAX: %1, X=%2, Y=%3, Z=%4, E=%5")
                           .arg(QString::fromStdString(line.raw()))
                           .arg(m_time_processor.machine_limits.machine_max_jerk_x.values[i])
                           .arg(m_time_processor.machine_limits.machine_max_jerk_y.values[i])
                           .arg(m_time_processor.machine_limits.machine_max_jerk_z.values[i])
                           .arg(m_time_processor.machine_limits.machine_max_jerk_e.values[i])
                           );
                }
                else{
                    TDebug(QString("M205_V_MIN: %1, EX=%2, TR=%3")
                           .arg(QString::fromStdString(line.raw()))
                           .arg(m_time_processor.machine_limits.machine_min_extruding_rate.values[i])
                           .arg(m_time_processor.machine_limits.machine_min_travel_rate.values[i])
                           );
                }
            }
        }
    }

    void GCodeProcessor::process_M221(const GCodeReader::GCodeLine& line)
    {
        float value_s;
        float value_t;
        if (line.has_value('S', value_s) && !line.has_value('T', value_t)) {
            value_s *= 0.01f;
            for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
                m_time_processor.machines[i].extrude_factor_override_percentage = value_s;
            }
        }
    }

    void GCodeProcessor::process_M401(const GCodeReader::GCodeLine& line)
    {
        if (m_flavor != Anker::gcfRepetier)
            return;

        for (unsigned char a = 0; a <= 3; ++a) {
            m_cached_position.position[a] = m_start_position[a];
        }
        m_cached_position.feedrate = m_feedrate;
    }

    void GCodeProcessor::process_M402(const GCodeReader::GCodeLine& line)
    {
        if (m_flavor != Anker::gcfRepetier)
            return;

        // see for reference:
        // https://github.com/repetier/Repetier-Firmware/blob/master/src/ArduinoAVR/Repetier/Printer.cpp
        // void Printer::GoToMemoryPosition(bool x, bool y, bool z, bool e, float feed)

        bool has_xyz = !(line.has_x() || line.has_y() || line.has_z());

        float p = FLT_MAX;
        for (unsigned char a = X; a <= Z; ++a) {
            if (has_xyz || line.has(a)) {
                p = float(m_cached_position.position[a]);
                if (p != FLT_MAX)
                    m_start_position[a] = p;
            }
        }

        p = float(m_cached_position.position[E]);
        if (p != FLT_MAX)
            m_start_position[E] = p;

        p = FLT_MAX;
        if (!line.has_value(4, p))
            p = m_cached_position.feedrate;

        if (p != FLT_MAX)
            m_feedrate = p;
    }

    void GCodeProcessor::process_M566(const GCodeReader::GCodeLine& line)
    {
        // RepRapFirmware gcode
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (line.has_x())
                set_option_value(m_time_processor.machine_limits.machine_max_jerk_x, i, line.x() * MMMIN_TO_MMSEC);

            if (line.has_y())
                set_option_value(m_time_processor.machine_limits.machine_max_jerk_y, i, line.y() * MMMIN_TO_MMSEC);

            if (line.has_z())
                set_option_value(m_time_processor.machine_limits.machine_max_jerk_z, i, line.z() * MMMIN_TO_MMSEC);

            if (line.has_e())
                set_option_value(m_time_processor.machine_limits.machine_max_jerk_e, i, line.e() * MMMIN_TO_MMSEC);
        }
    }

    void GCodeProcessor::process_M702(const GCodeReader::GCodeLine& line)
    {
        if (line.has('C')) {
            // MK3 MMU2 specific M code:
            // M702 C is expected to be sent by the custom end G-code when finalizing a print.
            // The MK3 unit shall unload and park the active filament into the MMU2 unit.
            m_time_processor.extruder_unloaded = true;
            simulate_st_synchronize(get_filament_unload_time(m_extruder_id));
        }
    }

    //  M900 [K<kfactor>] [L<kfactor>] [S<slot>] [T<index>]
    void GCodeProcessor::process_M900(const GCodeReader::GCodeLine &line)
    {
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            if (static_cast<PrintEstimatedTimeStatistics::ETimeMode>(i) == PrintEstimatedTimeStatistics::ETimeMode::Normal || m_time_processor.machine_envelope_processing_enabled) {
                float value;
                if (line.has_value('K', value))
                    set_option_value(m_time_processor.machine_limits.machine_lin_advance_k, i, value);

                if (line.has_value('L', value)){}
                if (line.has_value('S', value)){}
                if (line.has_value('T', value)){}

                TDebug(QString("M900_K_Value: %1, K=%2")
                       .arg(QString::fromStdString(line.raw()))
                       .arg(m_time_processor.machine_limits.machine_lin_advance_k.values[i]));
            }
        }
    }

    void GCodeProcessor::process_M1024(const GCodeReader::GCodeLine& line)
    {
        if (line.has('L')) {
            int layerValue = -1;
            if (line.get_ai_layer(layerValue))
            {
                m_result.ai_pic_Layer.push_back(layerValue);
                m_result.layer_offset.push_back(this->m_end_position);

            }

        }
    }

    void GCodeProcessor::process_T(const GCodeReader::GCodeLine& line)
    {
        process_T(line.cmd());
    }

    void GCodeProcessor::process_T(const std::string_view command)
    {
        if (command.length() > 1) {
            int eid;
            if (!parse_number(command.substr(1), eid) || eid < 0 || eid > 255) {
                // T-1 is a valid gcode line for RepRap Firmwares (used to deselects all tools) see https://github.com/prusa3d/PrusaSlicer/issues/5677
                if ((m_flavor != Anker::gcfRepRap && m_flavor != Anker::gcfSprinter) || eid != -1)
                    return;
                //TODO: return;
                //BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid toolchange (" << command << ").";
            } else {
                unsigned char id = static_cast<unsigned char>(eid);
                process_T(id);
            }
        }
    }

    void GCodeProcessor::process_T(uint8_t new_id)
    {
        if (m_extruder_id != new_id) {
            unsigned char extruders_count = static_cast<unsigned char>(m_extruder_offsets.size());
            if (new_id >= extruders_count)
                return;
            //TODO:
            //BOOST_LOG_TRIVIAL(error) << "GCodeProcessor encountered an invalid toolchange, maybe from a custom gcode.";
            else {
                unsigned char old_extruder_id = m_extruder_id;
                m_extruder_id = new_id;
                m_cp_color.current = m_extruder_colors[new_id];
                // Specific to the MK3 MMU2:
                // The initial value of extruder_unloaded is set to true indicating
                // that the filament is parked in the MMU2 unit and there is nothing to be unloaded yet.
                float extra_time = get_filament_unload_time(static_cast<size_t>(old_extruder_id));
                m_time_processor.extruder_unloaded = false;
                extra_time += get_filament_load_time(static_cast<size_t>(m_extruder_id));
                simulate_st_synchronize(extra_time);
            }

            // store tool change move
            store_move_vertex(EMoveType::Tool_change);
        }
    }

#include <algorithm>

    void GCodeProcessor::store_move_vertex(EMoveType type)
    {
        TrapezoidFeedrateProfile temTF;
        temTF.accelerate_until = this->m_time_processor.machines[0].blocks.back().trapezoid.accelerate_until;
        temTF.decelerate_after = this->m_time_processor.machines[0].blocks.back().trapezoid.decelerate_after;
        temTF.cruise = this->m_time_processor.machines[0].blocks.back().feedrate_profile.cruise;
        temTF.entry = this->m_time_processor.machines[0].blocks.back().feedrate_profile.entry;
        temTF.exit = this->m_time_processor.machines[0].blocks.back().feedrate_profile.exit;
        unsigned int g1LineId = this->m_time_processor.machines[0].blocks.back().g1_line_id;

        
        
        if(type == Anker::EMoveType::Extrude )
        {
            if(m_result.freedrate_range[0] < 0 )
            {
                m_result.freedrate_range[0] = m_feedrate;
                m_result.freedrate_range[1] = m_feedrate;
            }else{
                //for debug
                m_result.freedrate_range[0] = m_feedrate < m_result.freedrate_range[0] ? m_feedrate : m_result.freedrate_range[0];
                m_result.freedrate_range[1] = m_feedrate > m_result.freedrate_range[1] ? m_feedrate : m_result.freedrate_range[1];

                //        if(std::find(m_result.freedrate_range.begin(),m_result.freedrate_range.end(),m_feedrate) == m_result.freedrate_range.end())
                //        {
                //            m_result.freedrate_range.push_back(m_feedrate);
                //        }
            }

            //        m_result.freedrate_range[0] = m_feedrate < m_result.freedrate_range[0] ? m_feedrate : m_result.freedrate_range[0];
            //        m_result.freedrate_range[1] = m_feedrate > m_result.freedrate_range[1] ? m_feedrate : m_result.freedrate_range[1];


            if(m_result.flow_range[0] < 0  & m_result.flow_range[1] <0 )
            {
                m_result.flow_range[0] = m_mm3_per_mm * m_feedrate;
                m_result.flow_range[1] = m_mm3_per_mm * m_feedrate;
            }else{
                m_result.flow_range[0] = m_mm3_per_mm * m_feedrate < m_result.flow_range[0] ? m_mm3_per_mm * m_feedrate : m_result.flow_range[0];
                m_result.flow_range[1] = m_mm3_per_mm * m_feedrate > m_result.flow_range[1] ? m_mm3_per_mm * m_feedrate : m_result.flow_range[1];
            }

        }
        MoveVertex vertex = {
            type,
            m_extrusion_role,
            m_extruder_id,
            m_cp_color.current,
            Vec3f(float(m_end_position[X]), float(m_end_position[Y]), float(m_end_position[Z])) + m_extruder_offsets[m_extruder_id],
            float(m_end_position[E] - m_start_position[E]),
            m_feedrate,
            m_width,
            m_height,
            m_mm3_per_mm,
            m_fan_speed,
            float(m_layer_id), //layer_duration: set later
            m_time_processor.machines[0].time, //time: set later
            m_temperature,
            std::move(temTF),
            g1LineId
        };
        m_result.moves.emplace_back(vertex);
    }

    float GCodeProcessor::minimum_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, float feedrate) const
    {
        if (m_time_processor.machine_limits.machine_min_extruding_rate.empty())
            return feedrate;

        return std::max(feedrate, get_option_value(m_time_processor.machine_limits.machine_min_extruding_rate, static_cast<size_t>(mode)));
    }

    float GCodeProcessor::minimum_travel_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, float feedrate) const
    {
        if (m_time_processor.machine_limits.machine_min_travel_rate.empty())
            return feedrate;

        return std::max(feedrate, get_option_value(m_time_processor.machine_limits.machine_min_travel_rate, static_cast<size_t>(mode)));
    }

    float GCodeProcessor::get_axis_max_feedrate(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const
    {
        switch (axis)
        {
        case X: { return get_option_value(m_time_processor.machine_limits.machine_max_feedrate_x, static_cast<size_t>(mode)); }
        case Y: { return get_option_value(m_time_processor.machine_limits.machine_max_feedrate_y, static_cast<size_t>(mode)); }
        case Z: { return get_option_value(m_time_processor.machine_limits.machine_max_feedrate_z, static_cast<size_t>(mode)); }
        case E: { return get_option_value(m_time_processor.machine_limits.machine_max_feedrate_e, static_cast<size_t>(mode)); }
        default: { return 0.0f; }
        }
    }

    float GCodeProcessor::get_axis_max_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const
    {
        switch (axis)
        {
        case X: { return get_option_value(m_time_processor.machine_limits.machine_max_acceleration_x, static_cast<size_t>(mode)); }
        case Y: { return get_option_value(m_time_processor.machine_limits.machine_max_acceleration_y, static_cast<size_t>(mode)); }
        case Z: { return get_option_value(m_time_processor.machine_limits.machine_max_acceleration_z, static_cast<size_t>(mode)); }
        case E: { return get_option_value(m_time_processor.machine_limits.machine_max_acceleration_e, static_cast<size_t>(mode)); }
        default: { return 0.0f; }
        }
    }

    float GCodeProcessor::get_axis_max_jerk(PrintEstimatedTimeStatistics::ETimeMode mode, Axis axis) const
    {
        switch (axis)
        {
        case X: { return get_option_value(m_time_processor.machine_limits.machine_max_jerk_x, static_cast<size_t>(mode)); }
        case Y: { return get_option_value(m_time_processor.machine_limits.machine_max_jerk_y, static_cast<size_t>(mode)); }
        case Z: { return get_option_value(m_time_processor.machine_limits.machine_max_jerk_z, static_cast<size_t>(mode)); }
        case E: { return get_option_value(m_time_processor.machine_limits.machine_max_jerk_e, static_cast<size_t>(mode)); }
        default: { return 0.0f; }
        }
    }

    float GCodeProcessor::get_retract_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode) const
    {
        return get_option_value(m_time_processor.machine_limits.machine_max_acceleration_retracting, static_cast<size_t>(mode));
    }

    float GCodeProcessor::get_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode) const
    {
        size_t id = static_cast<size_t>(mode);
        return (id < m_time_processor.machines.size()) ? m_time_processor.machines[id].acceleration : DEFAULT_ACCELERATION;
    }

    void GCodeProcessor::set_acceleration(PrintEstimatedTimeStatistics::ETimeMode mode, float value)
    {
        size_t id = static_cast<size_t>(mode);
        if (id < m_time_processor.machines.size()) {
            m_time_processor.machines[id].acceleration = (m_time_processor.machines[id].max_acceleration == 0.0f) ? value :
                                                                                                                    // Clamp the acceleration with the maximum.
                                                                                                                    std::min(value, m_time_processor.machines[id].max_acceleration);
        }
    }

    float GCodeProcessor::get_filament_load_time(size_t extruder_id)
    {
        return (m_time_processor.filament_load_times.empty() || m_time_processor.extruder_unloaded) ?
                    0.0f :
                    ((extruder_id < m_time_processor.filament_load_times.size()) ?
                         m_time_processor.filament_load_times[extruder_id] : m_time_processor.filament_load_times.front());
    }

    float GCodeProcessor::get_filament_unload_time(size_t extruder_id)
    {
        return (m_time_processor.filament_unload_times.empty() || m_time_processor.extruder_unloaded) ?
                    0.0f :
                    ((extruder_id < m_time_processor.filament_unload_times.size()) ?
                         m_time_processor.filament_unload_times[extruder_id] : m_time_processor.filament_unload_times.front());
    }

    void GCodeProcessor::process_custom_gcode_time(Anker::CustomGCode::Type code)
    {
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            TimeMachine& machine = m_time_processor.machines[i];
            if (!machine.enabled)
                continue;

            TimeMachine::CustomGCodeTime& gcode_time = machine.gcode_time;
            gcode_time.needed = true;
            //FIXME this simulates st_synchronize! is it correct?
            // The estimated time may be longer than the real print time.
            machine.simulate_st_synchronize();
            if (gcode_time.cache != 0.0f) {
                gcode_time.times.push_back({ code, gcode_time.cache });
                gcode_time.cache = 0.0f;
            }
        }
    }

    void GCodeProcessor::simulate_st_synchronize(float additional_time)
    {
        for (size_t i = 0; i < static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Count); ++i) {
            m_time_processor.machines[i].simulate_st_synchronize(additional_time);
        }
    }

    void GCodeProcessor::update_estimated_times_stats()
    {
        auto update_mode = [this](PrintEstimatedTimeStatistics::ETimeMode mode) {
            PrintEstimatedTimeStatistics::Mode& data = m_result.time_statistics.modes[static_cast<size_t>(mode)];
            data.time = get_time(mode);
            data.custom_gcode_times = get_custom_gcode_times(mode, true);
            data.moves_times = get_moves_time(mode);
            data.roles_times = get_roles_time(mode);
            data.layers_times = get_layers_time(mode);
        };

        update_mode(PrintEstimatedTimeStatistics::ETimeMode::Normal);
        if (m_time_processor.machines[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].enabled)
            update_mode(PrintEstimatedTimeStatistics::ETimeMode::Stealth);
        else
            m_result.time_statistics.modes[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Stealth)].reset();
    }

} /* namespace Slic3r */

namespace Anker {

ExtrusionRole string_to_role(const std::string_view role)
{
    if (role == L("Perimeter") || role == L("Internal perimeter"))
        return erPerimeter;
    else if (role == L("External perimeter"))
        return erExternalPerimeter;
    else if (role == L("Overhang perimeter"))
        return erOverhangPerimeter;
    else if (role == L("Internal infill"))
        return erInternalInfill;
    else if (role == L("Solid infill"))
        return erSolidInfill;
    else if (role == L("Top solid infill"))
        return erTopSolidInfill;
    else if (role == L("Ironing"))
        return erIroning;
    else if (role == L("Bridge infill"))
        return erBridgeInfill;
    else if (role == L("Internal bridge infill"))
        return erInternalBridgeInfill;
    else if (role == L("Thin wall"))
        return erThinWall;
    else if (role == L("Gap fill"))
        return erGapFill;
    else if (role == L("Skirt"))
        return erSkirt;
    else if (role == L("Support material"))
        return erSupportMaterial;
    else if (role == L("Support material interface"))
        return erSupportMaterialInterface;
    else if (role == L("Wipe tower"))
        return erWipeTower;
    else if (role == L("Mill"))
        return erMilling;
    else if (role == L("Custom"))
        return Anker::erCustom;
    else if (role == L("Mixed"))
        return Anker::erMixed;
    else
        return erNone;
}
bool is_bridge(ExtrusionRole role)
{
    return role == erBridgeInfill
            || role == erInternalBridgeInfill
            || role == erOverhangPerimeter;
}
std::string get_time_dhms(float time_in_secs)
{
    int days = (int)(time_in_secs / 86400.0f);
    time_in_secs -= (float)days * 86400.0f;
    int hours = (int)(time_in_secs / 3600.0f);
    time_in_secs -= (float)hours * 3600.0f;
    int minutes = (int)(time_in_secs / 60.0f);
    time_in_secs -= (float)minutes * 60.0f;

    char buffer[64];
    if (days > 0)
        ::sprintf(buffer, "%dd %dh %dm %ds", days, hours, minutes, (int)time_in_secs);
    else if (hours > 0)
        ::sprintf(buffer, "%dh %dm %ds", hours, minutes, (int)time_in_secs);
    else if (minutes > 0)
        ::sprintf(buffer, "%dm %ds", minutes, (int)time_in_secs);
    else
        ::sprintf(buffer, "%ds", (int)time_in_secs);

    return buffer;
}
std::string short_time(const std::string& time)
{
    // Parse the dhms time format.
    int days = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    if (time.find('d') != std::string::npos)
        ::sscanf(time.c_str(), "%dd %dh %dm %ds", &days, &hours, &minutes, &seconds);
    else if (time.find('h') != std::string::npos)
        ::sscanf(time.c_str(), "%dh %dm %ds", &hours, &minutes, &seconds);
    else if (time.find('m') != std::string::npos)
        ::sscanf(time.c_str(), "%dm %ds", &minutes, &seconds);
    else if (time.find('s') != std::string::npos)
        ::sscanf(time.c_str(), "%ds", &seconds);
    // Round to full minutes.
    if (days + hours + minutes > 0 && seconds >= 30) {
        if (++minutes == 60) {
            minutes = 0;
            if (++hours == 24) {
                hours = 0;
                ++days;
            }
        }
    }
    // Format the dhm time.
    char buffer[64];
    if (days > 0)
        ::sprintf(buffer, "%dd%dh%dm", days, hours, minutes);
    else if (hours > 0)
        ::sprintf(buffer, "%dh%dm", hours, minutes);
    else if (minutes > 0)
        ::sprintf(buffer, "%dm", minutes);
    else
        ::sprintf(buffer, "%ds", seconds);
    return buffer;
}
std::error_code rename_file(const std::string& from, const std::string& to)
{
    //TODO:
    return std::error_code();
}

CopyFileResult copy_file(const std::string& from, const std::string& to, std::string& error_message, const bool with_check)
{
    //TODO:
    return Anker::SUCCESS;
}
//  change @2022-09-05 by CL
MachineEnvelopeConfig::MachineEnvelopeConfig()
{
    struct AxisDefault {
        std::string         name;
        std::vector<double> max_feedrate;
        std::vector<double> max_acceleration;
        std::vector<double> max_jerk;
    };
    std::vector<AxisDefault> axes{
        // name, max_feedrate,  max_acceleration, max_jerk
        { "x", { 500., 500. }, {  2500., 2500. }, { 15. , 15.  } },
        { "y", { 500., 500. }, {  2500., 2500. }, { 15. , 15.  } },
        { "z", {  50.,  50. }, {   100.,  100. }, {  0.3,  0.3 } },
        { "e", { 300., 300. }, {  2500., 2500. }, {  3. ,  3.  } }
    };

    
    
    this->machine_limits_usage.values.push_back(MachineLimitsUsage::TimeEstimateOnly);
    this->machine_max_acceleration_x.values = std::move(axes[0].max_acceleration);
    this->machine_max_acceleration_y.values = std::move(axes[1].max_acceleration);
    this->machine_max_acceleration_z.values = std::move(axes[2].max_acceleration);
    this->machine_max_acceleration_e.values = std::move(axes[3].max_acceleration);

    
    this->machine_max_feedrate_x.values = std::move(axes[0].max_feedrate);
    this->machine_max_feedrate_y.values = std::move(axes[1].max_feedrate);
    this->machine_max_feedrate_z.values = std::move(axes[2].max_feedrate);
    this->machine_max_feedrate_e.values = std::move(axes[3].max_feedrate);

    
    this->machine_max_jerk_x.values = std::move(axes[0].max_jerk);
    this->machine_max_jerk_y.values = std::move(axes[1].max_jerk);
    this->machine_max_jerk_z.values = std::move(axes[2].max_jerk);
    this->machine_max_jerk_e.values = std::move(axes[3].max_jerk);

    
    std::vector<double> machine_max_acceleration_extruding_temp{ 2500., 2500. };
    std::vector<double> machine_max_acceleration_retracting_temp{ 2500., 2500. };
    std::vector<double> machine_max_acceleration_travel_temp{ 2500., 2500. };
    this->machine_max_acceleration_extruding.values = std::move(machine_max_acceleration_extruding_temp);
    this->machine_max_acceleration_retracting.values = std::move(machine_max_acceleration_retracting_temp);
    this->machine_max_acceleration_travel.values = std::move(machine_max_acceleration_travel_temp);

    
    std::vector<double> machine_min_travel_rate_temp{0.05,0.05};
    std::vector<double> machine_min_extruding_rate_temp{ 0.,0. };
    this->machine_min_travel_rate.values = std::move(machine_min_travel_rate_temp);
    this->machine_min_extruding_rate.values = std::move(machine_min_extruding_rate_temp);

    
    std::vector<double> machine_lin_advance_k{ 0.06, 0.06 };
    this->machine_lin_advance_k.values = std::move(machine_lin_advance_k);
}
}
