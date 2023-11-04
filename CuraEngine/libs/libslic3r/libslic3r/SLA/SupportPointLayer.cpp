#include "SupportPointLayer.hpp"
#include "Concurrency.hpp"

namespace Slic3r {
	namespace sla {
        void make_layers_w(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers, 
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb )
        {
            assert(slices.size() == heights.size());
            std::function<void(void)> throw_on_cancel = ctrlcb.throw_on_cancel;
            std::function<void(float)> statuscb = ctrlcb.status_cb;
            float delta_height = 2.0f * config.layer_height;
            layers.reserve(slices.size());
            for (size_t i = 0; i < slices.size(); ++i)
                layers.emplace_back(i, heights[i]);

            // FIXME: calculate actual pixel area from printer config:
            //const float pixel_area = pow(wxGetApp().preset_bundle->project_config.option<ConfigOptionFloat>("display_width") / wxGetApp().preset_bundle->project_config.option<ConfigOptionInt>("display_pixels_x"), 2.f); //
            const float pixel_area = pow(0.047f, 2.f);
            //const float pixel_area = pow(0.001f, 2.f);
            statuscb(0.2);
            std::cout << "step1" <<std::endl;
            ccr_par::for_each(size_t(0), layers.size(),
                [&layers, &slices, &heights, pixel_area, delta_height, throw_on_cancel](size_t layer_id)
                {
                    if ((layer_id % 8) == 0)
                        // Don't call the following function too often as it flushes
                        // CPU write caches due to synchronization primitves.
                        throw_on_cancel();

                    ICFLayer& layer = layers[layer_id];
                    const ExPolygons& islands = slices[layer_id];

                    // FIXME WTF?
                    const float height = heights[layer_id] - delta_height;
                    layer.islands.reserve(islands.size());
                    for (const ExPolygon& island : islands) {
                        float area = float(island.area() * SCALING_FACTOR * SCALING_FACTOR);
                        if (area >= pixel_area)
                        {
                            // FIXME this is not a correct centroid of a polygon with holes.
                            layer.islands.emplace_back(layer, island, get_extents(island.contour),
                                unscaled<float>(island.contour.centroid()), area, height);
                        }
                    }

                    if (0 == layer_id)
                    {
                        for (ICFStructure& layer : layer.islands)
                        {
                            layer.isFirstLyer = true;
                        }
                    }

                }, 32 /*gransize*/);

            statuscb(0.3);
            std::cout << "step2" << std::endl;

            // Calculate overlap of successive layers. Link overlapping islands.
            ccr_par::for_each(size_t(1), layers.size(),
                [&layers, &heights, &config, throw_on_cancel](size_t layer_id)
                {
                    if ((layer_id % 2) == 0)
                        // Don't call the following function too often as it flushes CPU write caches due to synchronization primitves.
                        throw_on_cancel();
                    ICFLayer& layer_above = layers[layer_id];
                    ICFLayer& layer_below = layers[layer_id - 1];
                    //FIXME WTF?
                    const float layer_height = (layer_id != 0 ? heights[layer_id] - heights[layer_id - 1] : heights[0]);
                    const float safe_angle = config.safe_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float between_layers_offset = scaled<float>(layer_height * std::tan(safe_angle));
                    const float slope_angle = config.slope_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float slope_offset = scaled<float>(layer_height * std::tan(slope_angle));
                    //FIXME This has a quadratic time complexity, it will be excessively slow for many tiny islands.
                    for (ICFStructure& top : layer_above.islands) {
                        //float overlapArea = 0.0f;
                        for (ICFStructure& bottom : layer_below.islands) {
                            float overlap_area = top.overlap_area(bottom);
                            //overlapArea += overlap_area;
                            if (overlap_area > 0) {
                                top.islands_below.emplace_back(&bottom, overlap_area);
                                bottom.islands_above.emplace_back(&top, overlap_area);  
                            }
                        }
                        
                        if (!top.islands_below.empty()) {
                            Polygons top_polygons = to_polygons(*top.polygon);
                            Polygons bottom_polygons = top.polygons_below();

                            //float new_island_offset = config.new_island_offset;
                            float new_island_offset = config.head_diameter;
                           
                            Polygons top_polygons_origin = top_polygons;
                            if (new_island_offset < 0.3f )
                            {
                                top_polygons = offset(top_polygons, -scaled<float>(new_island_offset), Clipper3r::jtMiter);
                            }
                            else
                            {
                                auto top_polygons1 = offset(top_polygons, -scaled<float>(new_island_offset), Clipper3r::jtMiter);
                                auto top_polygons2 = offset(top_polygons, -scaled<float>(0.3f), Clipper3r::jtMiter);
                                //top.top2_offset_areas = union_ex(top_polygons1);


                                auto top_polygons3 = offset(top_polygons1, scaled<float>(new_island_offset), Clipper3r::jtMiter);
                                top_polygons3 = diff(top_polygons2, top_polygons3);

                                top_polygons = union_(top_polygons1, top_polygons3);   
                            }

                            //get mini areas
                            Slic3r::Polygons top2overhangs;
                            if (layer_id < layers.size() - 1)
                            {
                                ICFLayer& layer_top2 = layers[layer_id + 1];
                                top2overhangs.reserve(layer_top2.islands.size());
                                for (ICFStructure& top2ploygons : layer_top2.islands) {
                                    float overlap_area = top2ploygons.overlap_area(top);
                                    if (overlap_area > 0) {
                                        top.islands_above.emplace_back(&top2ploygons, overlap_area);
                                    }
                                }
                            }

                            float soffset = scaled < float>(0.05f * tanf(5.0f * M_PI / 180.0f));
                            //float soffset = 1000.0f * 0.05f * tanf(1.0f * M_PI / 180.0f);
                            if (!top_polygons.empty())
                            {  
                                auto top_area = offset(top_polygons, scaled<float>(new_island_offset), Clipper3r::jtMiter);
                                top_area = diff(top_polygons_origin, top_area);
                                
                                //top.top2_offset_sharp = union_ex(top_area);  //test
                                //top_area = offset(top_area, -scaled<float>(0.05f), Clipper3r::jtMiter);
                                //top_area = offset(top_area, scaled<float>(0.05f), Clipper3r::jtMiter);

                                top_area = offset(top_area, -soffset, Clipper3r::jtMiter);
                                top_area = offset(top_area, soffset, Clipper3r::jtMiter);
                                
                                top.mini_overhangs_areas = diff_ex(top_area, bottom_polygons);

                                //top.top2_offset_areas = top.mini_overhangs_areas; //test
                                //top.top2_offset_sharp = top.mini_overhangs_areas;
                                auto iter = top.mini_overhangs_areas.begin();
                                for (; iter != top.mini_overhangs_areas.end();)
                                {
                                    float s = iter->area() / 1000000;
                                    if (s < 200000)
                                    //if (s < 0.5)
                                    {
                                        iter = top.mini_overhangs_areas.erase(iter);
                                    }
                                    else
                                        iter++;
                                }
                                //top.top2_offset_sharp = top.mini_overhangs_areas;//test
                                if (top.mini_overhangs_areas.size())
                                {
                                    top.belows_support_areas = union_(top.belows_support_areas, top_polygons_origin);
                                } 

                                //top.top2_offset_areas = top.mini_overhangs_areas;
                            }   
                            
                            top.overhangs = diff_ex(top_polygons, bottom_polygons);
                            auto interTopBottom = diff(top_polygons, bottom_polygons); //next and cur diff

                            if (!top.overhangs.empty()) {
                                // Produce 2 bands around the island, a safe band for dangling overhangs
                                // and an unsafe band for sloped overhangs.
                                // These masks include the original island

                                top.belows_support_areas = union_(top.belows_support_areas, top_polygons);  

                                auto dangl_mask = offset(bottom_polygons, between_layers_offset, Clipper3r::jtMiter);//dangl_mask form next
                                //
                                auto overh_slope_mask = offset(dangl_mask, slope_offset, Clipper3r::jtMiter);
                                auto overh_slope_mask1 = offset(dangl_mask, slope_offset*2.0, Clipper3r::jtMiter);
                                //overh_slope_mask = intersection(overh_slope_mask, interTopBottom);

                               auto overh_mask = diff(interTopBottom, overh_slope_mask);

/*                                auto dangl_mask = offset(bottom_polygons, between_layers_offset, Clipper3r::jtSquare);
                                auto overh_slope_mask = offset(bottom_polygons, slope_offset, Clipper3r::jtSquare);
                                dangl_mask = intersection(dangl_mask, interTopBottom);
                                overh_slope_mask = intersection(overh_slope_mask, interTopBottom);
                                overh_slope_mask = diff(overh_slope_mask, dangl_mask);
                                auto overh_mask = diff(interTopBottom, overh_slope_mask);
                                overh_mask = diff(overh_mask, dangl_mask); */     

                                top.overhangs = union_ex(overh_mask);


                                if (!top.islands_below.empty() && !overh_mask.empty()) {
                                    Polygons top2_polygons = top.polygons_above();

                                    if (top2_polygons.size())
                                    {
                                        auto top2_polygons_offset = offset(top2_polygons, -scaled<float>(new_island_offset*1.5f), Clipper3r::jtMiter);
                                        //auto ployTop1 = offset(overh_mask, top.soffset, Clipper3r::jtMiter);
                                        auto ployTop1 = offset(overh_mask, scaled<float>(new_island_offset*1.5f), Clipper3r::jtMiter);

                                        ployTop1 = diff(ployTop1, top2_polygons_offset, true);
                                        ployTop1 = intersection(ployTop1, overh_mask, true);
                                        ployTop1 = diff(ployTop1, overh_slope_mask1, true);   
                                        //
                                        //sharp
                                        Polygons top2_offset_areas;
                                        Polygons top2_offset_sharp;
                                        getSharpArea(ployTop1, overh_mask, top2_offset_areas, top2_offset_sharp, scaled<float>(0.3f));
                                        top.top2_offset_areas = union_ex(top2_offset_areas);
                                        top.top2_offset_sharp = union_ex(top2_offset_sharp);
                                    }
                                }
                               
                                dangl_mask = intersection(dangl_mask, interTopBottom);
                                overh_slope_mask = diff(overh_slope_mask, dangl_mask);
                                overh_slope_mask = intersection(overh_slope_mask, interTopBottom);
                                top.dangling_areas = union_ex(dangl_mask);
                                top.overhangs_slopes = union_ex(overh_slope_mask);
                            }
                        }
                    }
                }, 8 /* gransize */);
        
                // Calculate support area
                std::cout << "step3" << std::endl;

                statuscb(0.4);
                //ccr_par::for_each(size_t(1), layers.size(),
                //    [&layers, &heights, &config, throw_on_cancel](size_t layer_id)
                size_t layer_size = layers.size();
                int times = layers.size() / 10;
                for (size_t layer_id = 5; layer_id < layers.size(); layer_id +=5)
                {
                    if ((layer_id % 5) == 0)
                        throw_on_cancel();
                    ICFLayer& layer_above = layers[layer_id];
                    ICFLayer& layer_below = layers[layer_id -5];

                    if ((layer_id % times) == 0)
                    {
                        float status = (float)layer_id / (float)layer_size;
                        statuscb(0.4+status*0.6);
                    }

                    //FIXME WTF?
                    //const float layer_height = (layer_id != 0 ? heights[layer_id] - heights[layer_id - 1] : heights[0]);
                    //const float safe_angle = config.safe_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    //const float between_layers_offset = scaled<float>(layer_height * std::tan(safe_angle));
                    //const float slope_angle = config.slope_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    //const float slope_offset = scaled<float>(layer_height * std::tan(slope_angle));
                    //FIXME This has a quadratic time complexity, it will be excessively slow for many tiny islands.

                    for (ICFStructure& top : layer_above.islands) {
                        for (ICFStructure& bottom : layer_below.islands) {
                            float overlap_area = top.overlap_area(bottom);
                            if (overlap_area > 0) {
                                if (layer_id == 5)
                                {
                                    top.belows_support_areas = union_(top.belows_support_areas, to_polygons(*bottom.polygon));
                                }
                                else
                                {
									//修复异常现象
                                    if (top.belows_support_areas.empty() && !bottom.belows_support_areas.empty())
                                        top.belows_support_areas = bottom.belows_support_areas;
                                    else if(!top.belows_support_areas.empty() && !bottom.belows_support_areas.empty())
                                        top.belows_support_areas = union_(top.belows_support_areas, bottom.belows_support_areas);
                                }
                            }
                        }

                        if (!top.islands_below.empty()) {
                            Polygons top_polygons = to_polygons(*top.polygon);
                            Polygons bottom_polygons = top.polygons_below();

                            //float new_island_offset = config.new_island_offset;
                            float new_island_offset = config.head_diameter;

                            if (top.overhangs.size() == 0
                                && top.overhangs_slopes.size() == 0
                                && top.dangling_areas.size() == 0
                                && top.top2_offset_areas.size() == 0
                                && top.mini_overhangs_areas.size() == 0
                                ) {
                                auto currentAreas = diff(top_polygons, bottom_polygons);
                                auto  belows_support_areas = offset(top.belows_support_areas, scaled<float>(new_island_offset)*5, Clipper3r::jtMiter);
                                top.overhangs = diff_ex(top_polygons, belows_support_areas);
                                if (!top.overhangs.empty() && top.islands_above.size())
                                {
                                    top.belows_support_areas = union_(top.belows_support_areas, top_polygons);
                                }
                                
                            }
                        }
                        if (!top.overhangs_slopes.empty())
                        {
                            top.overhangs_area = 0.f;
                            std::vector<std::pair<ExPolygon*, float>> expolys_with_areas;
                            for (ExPolygon& ex : top.overhangs) {
                                float area = float(ex.area());
                                expolys_with_areas.emplace_back(&ex, area);
                                top.overhangs_area += area;
                            }

                            if (top.top2_offset_areas.size() == 0 && top.top2_offset_sharp.size() == 0)
                            {
                                float area = 0.0f;
                                for (Slic3r::ExPolygon& ex : top.overhangs_slopes)
                                {
                                    area += float(ex.area());
                                }

                                for (Slic3r::ExPolygon& ex : top.dangling_areas)
                                {
                                    area += float(ex.area());
                                }

                                if (top.overhangs_area < area)
                                {
                                    top.top2_offset_areas = top.overhangs;
                                }
                            }
                            std::sort(expolys_with_areas.begin(), expolys_with_areas.end(),
                                [](const std::pair<ExPolygon*, float>& p1, const std::pair<ExPolygon*, float>& p2)
                                { return p1.second > p2.second; });
                            ExPolygons overhangs_sorted;
                            for (auto& p : expolys_with_areas)
                                overhangs_sorted.emplace_back(std::move(*p.first));
                            top.overhangs = std::move(overhangs_sorted);
                            top.overhangs_area *= float(SCALING_FACTOR * SCALING_FACTOR);
                        }
                    }
                }
                //, 1 /* gransize */);
        }

        void make_layers_w1(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers,
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb, int type)
        {
            assert(slices.size() == heights.size());
            std::function<void(void)> throw_on_cancel = ctrlcb.throw_on_cancel;
            std::function<void(float)> statuscb = ctrlcb.status_cb;
            float delta_height = 2.0f * config.layer_height;
            //layers.clear();
            std::cout << "step1" << std::endl;

            layers.reserve(slices.size());
            for (size_t i = 0; i < slices.size(); ++i)
                layers.emplace_back(i, heights[i]);

            // FIXME: calculate actual pixel area from printer config:
            //const float pixel_area = pow(wxGetApp().preset_bundle->project_config.option<ConfigOptionFloat>("display_width") / wxGetApp().preset_bundle->project_config.option<ConfigOptionInt>("display_pixels_x"), 2.f); //
            const float pixel_area = pow(0.047f, 2.f);

            //const float pixel_area = pow(0.001f, 2.f);
            statuscb(0.2);
            std::cout << "step2" << std::endl;

            ccr_par::for_each(size_t(0), layers.size(),
                [&layers, &slices, &heights, pixel_area, delta_height, throw_on_cancel](size_t layer_id)
                //for (int layer_id = 0; layer_id < layers.size(); layer_id++)
                {
                    if ((layer_id % 8) == 0)
                        //Don't call the following function too often as it flushes
                        //CPU write caches due to synchronization primitves.
                        throw_on_cancel();

                    ICFLayer& layer = layers[layer_id];
                    const ExPolygons& islands = slices[layer_id];

                    // FIXME WTF?
                    const float height = heights[layer_id] - delta_height;
                    layer.islands.reserve(islands.size());

                    // ccr_par::for_each(size_t(0), islands.size(),
                         //[&layer, &islands, &height, pixel_area, delta_height, throw_on_cancel](size_t id)
                    for (const ExPolygon& island : islands) {
                        float area = float(island.area() * SCALING_FACTOR * SCALING_FACTOR);
                        if (area >= pixel_area)
                        {
                            // FIXME this is not a correct centroid of a polygon with holes.
                            layer.islands.emplace_back(layer, island, get_extents(island.contour),
                                unscaled<float>(island.contour.centroid()), area, height);
                        }
                    }

                    if (0 == layer_id)
                    {
                        for (ICFStructure& layer : layer.islands)
                        {
                            layer.isFirstLyer = true;
                        }
                    }

                }, 32 /*gransize*/);

            statuscb(0.3);
            std::cout << "step3" << std::endl;

            //处理被过滤的小面积
            float new_island_offset = config.head_diameter;
            ccr_par::for_each(size_t(1), layers.size(),
                [&layers, &heights ,&config, throw_on_cancel](size_t layer_id)
                {
                    if (((layer_id - 1) % 3) != 0)
                        throw_on_cancel();
                    ICFLayer& layer_cur = layers[layer_id];
                    ICFLayer& layer_below = layers[layer_id - 1];
                    ICFLayer& layer_above = (layer_id >= layers.size() - 1) ? layers[layer_id] : layers[layer_id + 1];

                    float new_island_offset = config.head_diameter;
                    //const float layer_height = (layer_id != 0 ? heights[layer_id] - heights[layer_id - 1] : heights[0]);
                    const float layer_height = 0.05f;
                    const float safe_angle = config.safe_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float between_layers_offset = scaled<float>(layer_height * std::tan(safe_angle));
                    const float slope_angle = config.slope_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float slope_offset = scaled<float>(layer_height * std::tan(slope_angle));

                    //FIXME This has a quadratic time complexity, it will be excessively slow for many tiny islands.
                    for (ICFStructure& cur : layer_cur.islands) {
                        Polygons cur_polygons = to_polygons(*cur.polygon);
                        //ccr_par::for_each(size_t(0), layer_below.islands.size(),
                            //[&layer_below, &cur, throw_on_cancel](size_t id)
                        for (ICFStructure& below : layer_below.islands)
                        {
                            float overlap_area = cur.overlap_area(below);
                            if (overlap_area > 0) {
                                cur.islands_below.emplace_back(&below, overlap_area);
                                below.islands_above.emplace_back(&cur, overlap_area);
                            }
                        }//, 32 /*gransize*/);

                        //ccr_par::for_each(size_t(0), layer_above.islands.size(),
                            //[&layer_above, &cur, throw_on_cancel](size_t id)
                        for (ICFStructure& above : layer_above.islands)
                        {
                            float overlap_area = cur.overlap_area(above);
                            if (overlap_area > 0) {
                                cur.islands_above.emplace_back(&above, overlap_area);
                            }
                        }//, 32 /*gransize*/);

#if 1
                        if (!cur.islands_below.empty())
                        {
                            //Polygons cur_polygons = to_polygons(*cur.polygon);
                            Polygons bottom_polygons = cur.polygons_below();
                            Polygons top_polygons = cur.polygons_above();
                            Polygons cur_polygons_origin = cur_polygons;
                            cur_polygons = offset(cur_polygons, -scaled<float>(new_island_offset), Clipper3r::jtMiter);

                            //Area 1. min                          
                            Polygons min_cur_polygons = offset(cur_polygons, scaled<float>(new_island_offset), Clipper3r::jtMiter);
                            cur.mini_overhangs_areas = diff_ex(cur_polygons_origin, min_cur_polygons);
                            //cur.top2_offset_sharp = cur.mini_overhangs_areas; //test
                            auto iter = cur.mini_overhangs_areas.begin();
                            for (; iter != cur.mini_overhangs_areas.end();)
                            {
                                float area = float(iter->area() * SCALING_FACTOR * SCALING_FACTOR);
                                if (area < 2.0f)
                                {
                                    iter = cur.mini_overhangs_areas.erase(iter);
                                }
                                else
                                    iter++;
                            }

                            if (cur.mini_overhangs_areas.size())
                            {
                                cur.belows_support_areas = union_(cur.belows_support_areas, cur_polygons_origin);
                            }

                            //Area 2. below
                            //Area 3. above
                            //Area 4. sharp
                            auto overhangs = diff(cur_polygons, bottom_polygons);
                            auto interTopBottom = diff(cur_polygons, bottom_polygons); //next and cur diff
                            if (!overhangs.empty()) {
                                auto top_polygons_offset = offset(top_polygons, -scaled<float>(new_island_offset * 1.2f), Clipper3r::jtMiter);

                                auto curpoly = offset(cur_polygons, scaled<float>(new_island_offset / 2.0f), Clipper3r::jtMiter);
                                auto curploy2 = diff(curpoly, cur_polygons);
                                curploy2 = offset(curploy2, scaled<float>(new_island_offset / 2.0f), Clipper3r::jtMiter);
                                curploy2 = intersection(curploy2, cur_polygons);
                                curploy2 = diff(curploy2, top_polygons_offset);

                                //sharp
                                Polygons top2_offset_areas;
                                Polygons top2_offset_sharp;
                                Polygons p;
                                float s = scaled<float>(0.3f);
                                getSharpArea(curploy2, p, top2_offset_areas, top2_offset_sharp, s);

                                //Area 5. slope
                                auto dangl_mask = offset(bottom_polygons, between_layers_offset, Clipper3r::jtMiter);//dangl_mask form next
                                auto overh_slope_mask = offset(dangl_mask, slope_offset, Clipper3r::jtMiter);
                                cur.dangling_areas = intersection_ex(overhangs, dangl_mask);
                                overh_slope_mask = intersection(overhangs, overh_slope_mask);
                                cur.overhangs_slopes = diff_ex(overh_slope_mask, dangl_mask);
                                overhangs = diff(overhangs, overh_slope_mask);

                                //sharp
                                cur.top2_offset_areas = intersection_ex(top2_offset_areas, overhangs);
                                cur.top2_offset_sharp = intersection_ex(top2_offset_sharp, overhangs);

                                cur.belows_support_areas = union_(cur.belows_support_areas, cur_polygons);
                            }
                            cur.overhangs = union_ex(overhangs);


                            if (!cur.overhangs_slopes.empty())
                            {
                                cur.overhangs_area = 0.f;
                                std::vector<std::pair<ExPolygon*, float>> expolys_with_areas;
                                for (ExPolygon& ex : cur.overhangs) {
                                    float area = float(ex.area());
                                    expolys_with_areas.emplace_back(&ex, area);
                                    cur.overhangs_area += area;
                                }

                                if (cur.top2_offset_areas.size() == 0 && cur.top2_offset_sharp.size() == 0)
                                {
                                    float area = 0.0f;
                                    for (Slic3r::ExPolygon& ex : cur.overhangs_slopes)
                                    {
                                        area += float(ex.area());
                                    }
                                    for (Slic3r::ExPolygon& ex : cur.dangling_areas)
                                    {
                                        area += float(ex.area());
                                    }

                                    if (cur.overhangs_area < area)
                                    {
                                        cur.top2_offset_areas = cur.overhangs;
                                    }
                                }
                                std::sort(expolys_with_areas.begin(), expolys_with_areas.end(),
                                    [](const std::pair<ExPolygon*, float>& p1, const std::pair<ExPolygon*, float>& p2)
                                    { return p1.second > p2.second; });
                                ExPolygons overhangs_sorted;
                                for (auto& p : expolys_with_areas)
                                    overhangs_sorted.emplace_back(std::move(*p.first));
                                cur.overhangs = std::move(overhangs_sorted);
                                cur.overhangs_area *= float(SCALING_FACTOR * SCALING_FACTOR);
                            }
                        }
#endif
                    }
                }, 8 /* gransize */);

            size_t layer_size = layers.size();
            int  layeroffset = 6;
            int times = layers.size() / layeroffset ;
            //累加计算
            std::cout << "step4" << std::endl;

			if (type == 0)
			{
				for (int i = 1; i < layers.size(); i += layeroffset)
				{
					if (((i - 1) % times) == 0)
					{
						float status = (float)i / (float)layer_size;
						statuscb(0.4 + status * 0.6);
					}

					ICFLayer& layer_cur = layers[i];
					ICFLayer& layer_below = i == 1 ? layers[0] : layers[i - 6];

					ccr_par::for_each(size_t(0), layer_cur.islands.size(),
						[&layer_cur, &layer_below, &i, &pixel_area, &config, throw_on_cancel](size_t id) {
							//for (ICFStructure& cur : layer_cur.islands) {

							if ((id % 8) == 0)
								throw_on_cancel();

							ICFStructure & cur = layer_cur.islands[id];
							float new_island_offset = config.head_diameter;

							if (cur.overhangs.size() == 0)
							{
								for (ICFStructure& belowIslands : layer_below.islands)
								{
									float overlap_area = cur.overlap_area_outer(belowIslands);
									//overlapArea += overlap_area;
									if (overlap_area > pixel_area) {
										if (i == 1)
										{
											cur.belows_support_areas = to_polygons_outer(*belowIslands.polygon);
										}
										else
										{
											cur.belows_support_areas = union_(cur.belows_support_areas, belowIslands.belows_support_areas);
											//belowIslands.belows_support_areas.clear();
										}
									}
								}

								Polygons cur_polygons = to_polygons_outer(*cur.polygon);
								Polygons bottom_polygons = cur.polygons_below();
								if (cur.overhangs_slopes.size() == 0
									&& cur.dangling_areas.size() == 0
									&& cur.top2_offset_areas.size() == 0
									&& cur.mini_overhangs_areas.size() == 0
									)
								{
									auto currentAreas = diff(cur_polygons, bottom_polygons);

									if (cur.belows_support_areas.size() < 10)
									{
										auto  belows_support_areas = offset(cur.belows_support_areas, scaled<float>(new_island_offset) * 5, Clipper3r::jtMiter);
										cur.overhangs = diff_ex(currentAreas, belows_support_areas);
										if (!cur.overhangs.empty() && cur.islands_above.size())
										{
											cur.belows_support_areas = union_(cur.belows_support_areas, cur_polygons);
										}
									}
								}
							}

						}, 32);
				}
			}
        }

        void make_layers(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers,
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb)
        {
            assert(slices.size() == heights.size());
            std::function<void(void)> throw_on_cancel = ctrlcb.throw_on_cancel;
            std::function<void(float)> statuscb = ctrlcb.status_cb;

            float delta_height = 0.1f;
            layers.reserve(slices.size());
            for (size_t i = 0; i < slices.size(); ++i)
                layers.emplace_back(i, heights[i]);

            // FIXME: calculate actual pixel area from printer config:
            //const float pixel_area = pow(wxGetApp().preset_bundle->project_config.option<ConfigOptionFloat>("display_width") / wxGetApp().preset_bundle->project_config.option<ConfigOptionInt>("display_pixels_x"), 2.f); //
            const float pixel_area = pow(0.047f, 2.f);
            statuscb(0.2);
            ccr_par::for_each(size_t(0), layers.size(),
                [&layers, &slices, &heights, pixel_area, delta_height, throw_on_cancel](size_t layer_id)
                {
                    if ((layer_id % 8) == 0)
                        // Don't call the following function too often as it flushes
                        // CPU write caches due to synchronization primitves.
                        throw_on_cancel();

                    ICFLayer& layer = layers[layer_id];
                    const ExPolygons& islands = slices[layer_id];
                    // FIXME WTF?
                    const float height = heights[layer_id] - delta_height;
                    layer.islands.reserve(islands.size());
                    for (const ExPolygon& island : islands) {
                        float area = float(island.area() * SCALING_FACTOR * SCALING_FACTOR);
                        if (area >= pixel_area)
                            // FIXME this is not a correct centroid of a polygon with holes.
                            layer.islands.emplace_back(layer, island, get_extents(island.contour),
                                unscaled<float>(island.contour.centroid()), area, height);
                    }
                }, 32 /*gransize*/);
            
            statuscb(0.4);

            // Calculate overlap of successive layers. Link overlapping islands.
            ccr_par::for_each(size_t(1), layers.size(),
                [&layers, &heights, &config, throw_on_cancel](size_t layer_id)
                {
                    if ((layer_id % 2) == 0)
                        // Don't call the following function too often as it flushes CPU write caches due to synchronization primitves.
                        throw_on_cancel();
                    ICFLayer& layer_above = layers[layer_id];
                    ICFLayer& layer_below = layers[layer_id - 1];
                    //FIXME WTF?
                    const float layer_height = (layer_id != 0 ? heights[layer_id] - heights[layer_id - 1] : heights[0]);
                    const float safe_angle = config.safe_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float between_layers_offset = scaled<float>(layer_height * std::tan(safe_angle));
                    const float slope_angle = config.slope_angle * (float(M_PI) / 180.f); // smaller number - less supports
                    const float slope_offset = scaled<float>(layer_height * std::tan(slope_angle));
                    //FIXME This has a quadratic time complexity, it will be excessively slow for many tiny islands.
                    for (ICFStructure& top : layer_above.islands) {
                        for (ICFStructure& bottom : layer_below.islands) {
                            float overlap_area = top.overlap_area(bottom);
                            if (overlap_area > 0) {
                                top.islands_below.emplace_back(&bottom, overlap_area);
                                bottom.islands_above.emplace_back(&top, overlap_area);
                            }
                        }
                        if (!top.islands_below.empty()) {
                            Polygons top_polygons = to_polygons(*top.polygon);
                            Polygons bottom_polygons = top.polygons_below();
                            top.overhangs = diff_ex(top_polygons, bottom_polygons);
                            if (!top.overhangs.empty()) {

                                // Produce 2 bands around the island, a safe band for dangling overhangs
                                // and an unsafe band for sloped overhangs.
                                // These masks include the original island
                                auto dangl_mask = offset(bottom_polygons, between_layers_offset, Clipper3r::jtSquare);
                                auto overh_mask = offset(bottom_polygons, slope_offset, Clipper3r::jtSquare);

                                // Absolutely hopeless overhangs are those outside the unsafe band
                                top.overhangs = diff_ex(top_polygons, overh_mask);

                                // Now cut out the supported core from the safe band
                                // and cut the safe band from the unsafe band to get distinct
                                // zones.
                                overh_mask = diff(overh_mask, dangl_mask);
                                dangl_mask = diff(dangl_mask, bottom_polygons);

                                top.dangling_areas = intersection_ex(top_polygons, dangl_mask);
                                top.overhangs_slopes = intersection_ex(top_polygons, overh_mask);

                                top.overhangs_area = 0.f;
                                std::vector<std::pair<ExPolygon*, float>> expolys_with_areas;
                                for (ExPolygon& ex : top.overhangs) {
                                    float area = float(ex.area());
                                    expolys_with_areas.emplace_back(&ex, area);
                                    top.overhangs_area += area;
                                }
                                std::sort(expolys_with_areas.begin(), expolys_with_areas.end(),
                                    [](const std::pair<ExPolygon*, float>& p1, const std::pair<ExPolygon*, float>& p2)
                                    { return p1.second > p2.second; });
                                ExPolygons overhangs_sorted;
                                for (auto& p : expolys_with_areas)
                                    overhangs_sorted.emplace_back(std::move(*p.first));
                                top.overhangs = std::move(overhangs_sorted);
                                top.overhangs_area *= float(SCALING_FACTOR * SCALING_FACTOR);
                            }
                        }
                    }
                }, 8 /* gransize */);
            
            statuscb(0.6);

        }

        void getSharpArea(const Polygons& originMask, Polygons& dangl_mask, Polygons& origin_mask, Polygons& sharp_mask, const float& offset)
        {
            auto extend1 = Slic3r::offset(originMask, offset, Clipper3r::jtSquare);
            if (!dangl_mask.empty())
            {
                extend1 = diff(extend1, dangl_mask, true);
            }
            auto extend2 = Slic3r::offset(originMask, offset, Clipper3r::jtMiter);
            if (!dangl_mask.empty())
            {
                extend2 = diff(extend2, dangl_mask, true);
            }
            auto outMask = Slic3r::diff(extend2, extend1);
            outMask = Slic3r::offset(outMask, offset * 2, Clipper3r::jtMiter);
            sharp_mask = Slic3r::intersection(outMask, originMask);
            outMask = Slic3r::intersection(outMask, originMask);
            origin_mask = Slic3r::diff(originMask, outMask);
        }

        void getSharpArea_ex(const ExPolygons& originMask, const float& offset, ExPolygons& outMask, ExPolygons& sharpMask)
        {
            Slic3r::Polygons extend0 = Slic3r::offset(originMask, 0, Clipper3r::jtMiter);
            Slic3r::Polygons extend1 = Slic3r::offset(originMask, offset, Clipper3r::jtSquare);
            Slic3r::Polygons extend2 = Slic3r::offset(originMask, offset, Clipper3r::jtMiter);
            auto _outMask = Slic3r::diff(extend2, extend1);
            _outMask = Slic3r::offset(_outMask, offset * 2, Clipper3r::jtMiter);
            auto sharp=Slic3r::intersection(_outMask, extend0);
            outMask = Slic3r::diff_ex(extend0, sharp);
            sharpMask = Slic3r::intersection_ex(_outMask, extend0);
        }

        int testOffset(const Polygons& originMask,const float& offset, Polygons& outMask)
        {
            int cnt = 1;
            int cycleCount = 90;
            auto top_polygons1 = Slic3r::offset(originMask, -offset, Clipper3r::jtMiter);
            while (top_polygons1.size() == originMask.size() && cycleCount > 0)
            {
                cnt++;
                top_polygons1 = Slic3r::offset(originMask, -offset * cnt, Clipper3r::jtMiter);
                cycleCount--;
            }
            cnt = cnt > 1 ? (cnt - 1) : 1;
            top_polygons1 = Slic3r::offset(originMask, -offset * cnt, Clipper3r::jtMiter);
            outMask = Slic3r::offset(top_polygons1, offset * cnt, Clipper3r::jtMiter);
        
            return cnt;
        }
    }
}