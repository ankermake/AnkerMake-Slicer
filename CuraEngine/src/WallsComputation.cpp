//Copyright (c) 2018 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include "ExtruderTrain.h"
#include "sliceDataStorage.h"
#include "WallsComputation.h"
#include "settings/types/Ratio.h"
#include "settings/EnumSettings.h"
#include "utils/polygonUtils.h"
#include "Application.h"
#include "Slice.h"

//#include"ExPolygon.hpp"

//#include"slic3rapi/slice3rapi.h"

#define MIN_SINGLE_WALL_AREA 3.0

namespace cura {

WallsComputation::WallsComputation(const Settings& settings, const LayerIndex layer_nr)
: settings(settings)
, layer_nr(layer_nr)
{
}

/*
 * This function is executed in a parallel region based on layer_nr.
 * When modifying make sure any changes does not introduce data races.
 *
 * generateInsets only reads and writes data for the current layer
 */
void WallsComputation::generateInsets(SliceLayerPart* part)
{
    size_t inset_count = settings.get<size_t>("wall_line_count");
    const bool spiralize = settings.get<bool>("magic_spiralize");
    if (spiralize && layer_nr < LayerIndex(settings.get<size_t>("initial_bottom_layers")) && ((layer_nr % 2) + 2) % 2 == 1) //Add extra insets every 2 layers when spiralizing. This makes bottoms of cups watertight.
    {
        inset_count += 5;
    }
    if (settings.get<bool>("alternate_extra_perimeter"))
    {
        inset_count += ((layer_nr % 2) + 2) % 2;
    }

    if (inset_count == 0)
    {
        part->insets.push_back(part->outline);
        part->print_outline = part->outline;
        return;
    }

    
    //if (layer_nr > 75 && layer_nr < 94)
    //{
    //    inset_count ++;
    //}

    const coord_t wall_0_inset = settings.get<coord_t>("wall_0_inset");
    coord_t line_width_0 = settings.get<coord_t>("wall_line_width_0");
    /// 2022/11/22 binary for small area use single wall to print
    coord_t thin_line_width_0 = settings.get<coord_t>("thin_wall_line_width_0");

    coord_t line_width_x = settings.get<coord_t>("wall_line_width_x");
    if (layer_nr == 0)
    {
        const ExtruderTrain& train_wall_0 = settings.get<ExtruderTrain&>("wall_0_extruder_nr");
        line_width_0 *= train_wall_0.settings.get<Ratio>("initial_layer_line_width_factor");
        const ExtruderTrain& train_wall_x = settings.get<ExtruderTrain&>("wall_x_extruder_nr");
        line_width_x *= train_wall_x.settings.get<Ratio>("initial_layer_line_width_factor");
    }

    const bool recompute_outline_based_on_outer_wall =
        settings.get<bool>("support_enable") &&
        !settings.get<bool>("fill_outline_gaps");

    
    bool  wall_ruler_concave_convex_enable = false && settings.get<bool>("wall_ruler_concave_convex_enable");    
    Ratio wall_ruler_concave_percent       = settings.get<Ratio>("wall_ruler_concave_percent");         
    Ratio wall_ruler_convex_percent        = settings.get<Ratio>("wall_ruler_convex_percent");          
    Ratio wall_ruler_separate_percent      = settings.get<Ratio>("wall_ruler_separate_percent");        

    Polygons outline_out_gap;   
    Polygons outline_in_gap ;   
    auto getOutlineOutGap1 = [&]() -> Polygons
    {
        Polygons  outline_out_keep   = part->outline.offset(line_width_0).offset(-line_width_0 - 10);
        Polygons  outline_out_gap    = outline_out_keep.difference(part->outline);
        outline_out_gap              = outline_out_gap.offset(10);
        return outline_out_gap;
    };
    auto getOutlineOutGap2 = [&]() -> Polygons
    {
        Polygons  outline_out_gap = getOutlineOutGap1();
        Polygons small_out_gap;
        for(ConstPolygonRef gap : outline_out_gap){
            double gap_area = gap.area();
            double gap_leng = gap.polygonLength();
            AABB   aabb(gap);
            Point  L_W = aabb.max - aabb.min;

            if(MM2INT(0.1) < L_W.X && MM2INT(0.1) < L_W.Y)          ; else{continue;}
            if( 1.0 * L_W.X / L_W.Y < 6 && 1.0 * L_W.Y / L_W.X < 6) ; else{continue;}
            if( gap_leng * gap_leng / gap_area < 30)                ; else{continue;}
            if( gap_area < MM2_2INT(0.6))                           ; else{continue;}  //  0.5*1.0  0.4*2.1

            small_out_gap.add(gap);
        }
        return small_out_gap;
    };
    if(wall_ruler_concave_convex_enable){
        outline_out_gap = getOutlineOutGap2();
    }
    //  end add @2022-07-20 by CL

    for(size_t i = 0; i < inset_count; i++)
    {
        part->insets.push_back(Polygons());
        if (i == 0)
        {
            part->insets[0] = part->outline.offset(-line_width_0 / 2 - wall_0_inset);

            //  add  @2022-07-20 by CL

            if(wall_ruler_concave_convex_enable){
                
                if( std::abs(wall_ruler_concave_percent) > 0.001 ){
                    Polygons small_out_gap = outline_out_gap.offset(line_width_0 * wall_ruler_concave_percent);
                    Polygons  outline_out  = part->outline.difference(small_out_gap);
                    part->insets[0] = outline_out.offset(-line_width_0 / 2 - wall_0_inset);
                }
            }
        }
        else if (i == 1)
        {
            
            //part->insets[1] = part->insets[0].offset(-line_width_0 / 2 + wall_0_inset - line_width_x / 2);
            part->insets[1] = part->insets[0].offset(-line_width_0 / 2 + wall_0_inset - line_width_x / 2 - line_width_0/2);
            if (part->insets[1].size() != 0)
            {
                part->insets[1] = part->insets[1].offset(line_width_0 / 2);
            }
            else
            {
                part->insets[1] = part->insets[0].offset(-line_width_0 / 2 + wall_0_inset - line_width_x / 2);
            }
            if(wall_ruler_concave_convex_enable)
            {
                Polygons  outerWall_0;

                
                if( std::abs(wall_ruler_separate_percent) > 0.001 ){
                    Polygons small_out_gap;
                    //  small_out_gap = getOutlineOutGap2();
                    
                    
                    

                    small_out_gap = outline_out_gap.offset(
                                +line_width_0 * wall_ruler_concave_percent
                                +line_width_0 / 2 + wall_0_inset
                                +line_width_0 * wall_ruler_separate_percent);

                    outerWall_0 = part->insets[0].difference(small_out_gap);
                }
                else{
                    outerWall_0 = part->insets[0];
                }

                coord_t offset_0_1 = -line_width_0 / 2 + wall_0_inset - line_width_x / 2;

                if( std::abs(wall_ruler_convex_percent) > 0.001 ){
                    Polygons  innerWall_in_1       = outerWall_0.offset(offset_0_1);
                    Polygons  innerWall_in_keep    = innerWall_in_1.offset( - line_width_x * wall_ruler_convex_percent).offset(line_width_x * wall_ruler_convex_percent);
                    Polygons  innerWall_in_gap     = innerWall_in_1.difference(innerWall_in_keep);
                    Polygons  small_gap;
                    for(ConstPolygonRef gap : innerWall_in_gap   ){
                        if(gap.area() < MM2_2INT(0.5)){ // 0.6*0.8
                            small_gap.add(gap);
                        }
                    }
                    part->insets[1] = innerWall_in_1.difference(small_gap);
                }
                else{
                    part->insets[1] = outerWall_0.offset(offset_0_1);
                }
            };
        }
        else
        {
            part->insets[i] = part->insets[i - 1].offset(-line_width_x);

        }

        const size_t inset_part_count = part->insets[i].size();
        constexpr size_t minimum_part_saving = 3; //Only try if the part has more pieces than the previous inset and saves at least this many parts.
        constexpr coord_t try_smaller = 10; //How many micrometres to inset with the try with a smaller inset.
        if (inset_part_count > minimum_part_saving + 1 && (i == 0 || (i > 0 && inset_part_count > part->insets[i - 1].size() + minimum_part_saving)))
        {
            //Try a different line thickness and see if this fits better, based on these criteria:
            // - There are fewer parts to the polygon (fits better in slim areas).
            // - The polygon area is largely unaffected.
            Polygons alternative_inset;
            if (i == 0)
            {
                alternative_inset = part->outline.offset(-(line_width_0 - try_smaller) / 2 - wall_0_inset);
            }
            else if (i == 1)
            {
                alternative_inset = part->insets[0].offset(-(line_width_0 - try_smaller) / 2 + wall_0_inset - line_width_x / 2);
            }
            else
            {
                alternative_inset = part->insets[i - 1].offset(-(line_width_x - try_smaller));
            }
            if (alternative_inset.size() < inset_part_count - minimum_part_saving) //Significantly fewer parts (saves more than 3 parts).
            {
                part->insets[i] = alternative_inset;
            }
        }

        //Finally optimize all the polygons. Every point removed saves time in the long run.
        int wall_extruder_nr = settings.get<int>(i == 0 ? "wall_0_extruder_nr" : "wall_x_extruder_nr");
        const Settings& resolution_settings = wall_extruder_nr < 0 ? settings : Application::getInstance().current_slice->scene.extruders[wall_extruder_nr].settings;
        coord_t maximum_resolution = resolution_settings.get<coord_t>("meshfix_maximum_resolution");
        coord_t maximum_deviation = resolution_settings.get<coord_t>("meshfix_maximum_deviation");

        if(resolution_settings.get<bool>("ak_meshfix_dynamic_resolution")){
            maximum_resolution = maximum_resolution * (i+1);
            maximum_deviation = maximum_resolution * (i+1);
        }

        part->insets[i].simplify(maximum_resolution, maximum_deviation);
        part->insets[i].removeDegenerateVerts();
        if (i == 0)
        {
            if (recompute_outline_based_on_outer_wall)
            {
                part->print_outline = part->insets[0].offset(line_width_0 / 2, ClipperLib::jtSquare);
            }
            else
            {
                part->print_outline = part->outline;
            }
        }
        if (part->insets[i].size() < 1)
        {
            part->insets.pop_back();
            break;
        }

        /// 2022/11/22 binary for small area use single wall to print
        //auto offset_wall = part->outline.offset(-line_width_0 / 2 - wall_0_inset);
        auto poly_after = part->insets[0].offset(line_width_0 / 2 + wall_0_inset);
        auto diff = part->outline.difference(poly_after);
       if (part->insets[0].size()>0 && (part->insets[0].size() == 0 || diff.size()>0) && INT2MM2(part->insets[0].area()) < MIN_SINGLE_WALL_AREA)
        {
            part->insets[0] = part->outline.offset(-(line_width_0 - thin_line_width_0) / 2 - wall_0_inset);
            part->real_wall0_line_width = thin_line_width_0;
            part->real_line_width_changed = true;

#if(0)  
            Slic3r::ExPolygons expolygons;
            for (auto& path : part->outline.getOutsidePolygons())
            {
                for (auto& path : part->outline.getOutsidePolygons())
                {
                    expolygons.push_back(Slic3r::ExPolygon(ClipperPath_to_Slic3rPolygon(path)));
                }
            }
            Polygons polygons;
            for (auto& ex : expolygons)
            {
                Slic3r::Polylines polylines;
                ex.medial_axis(771238, 0.01, &polylines);
                for (auto& line : polylines)
                {
                    Polygon poly;
                    for (auto& pp : line.points)
                    {
                        poly.add({ pp.x(), pp.y() });
                    }
                    polygons.add(poly);
                }
            }

            if (polygons.size() > 0)
            {
                part->insets[0].clear();
                part->insets[0].add(polygons);
            }
#endif
        }




    }
}

/*
 * This function is executed in a parallel region based on layer_nr.
 * When modifying make sure any changes does not introduce data races.
 *
 * generateInsets only reads and writes data for the current layer
 */
void WallsComputation::generateInsets(SliceLayer* layer)
{
    for(unsigned int partNr = 0; partNr < layer->parts.size(); partNr++)
    {
        generateInsets(&layer->parts[partNr]);
    }

    const bool remove_parts_with_no_insets = !settings.get<bool>("fill_outline_gaps");
    //Remove the parts which did not generate an inset. As these parts are too small to print,
    // and later code can now assume that there is always minimal 1 inset line.
    for (unsigned int part_idx = 0; part_idx < layer->parts.size(); part_idx++)
    {
        if (layer->parts[part_idx].insets.size() == 0 && remove_parts_with_no_insets)
        {
            if (part_idx != layer->parts.size() - 1)
            { // move existing part into part to be deleted
                layer->parts[part_idx] = std::move(layer->parts.back());
            }
            layer->parts.pop_back(); // always remove last element from array (is more efficient)
            part_idx -= 1; // check the part we just moved here
        }
    }
}

}//namespace cura
