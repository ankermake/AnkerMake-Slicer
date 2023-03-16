#ifndef SLIC3R_SUPPORTPOINTLAYER_1634225497208_H
#define SLIC3R_SUPPORTPOINTLAYER_1634225497208_H
#include "libslic3r/SLA/SupportPointData.hpp"

namespace Slic3r {
    namespace sla {

        typedef struct __CTRL_CALLBACK_FUNCTION__
        {
            std::function<void(void)> throw_on_cancel;
            std::function<void(float)> status_cb;
        }CTRL_CALLBACK_FUNCTION;
        void make_layers(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers, 
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb);

        void make_layers_w(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers, 
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb);

        void make_layers_w1(
            const std::vector<ExPolygons>& slices, const std::vector<float>& heights, std::vector<ICFLayer>& layers,
            const SupportPointGeneratorConfig& config, CTRL_CALLBACK_FUNCTION ctrlcb, int type = 0);
    
        int testOffset(const Polygons& originMask,const float& offset, Polygons& outMask);
        void getSharpArea(const Polygons& originMask, Polygons& dangl_mask, Polygons& origin_mask, Polygons& sharp_mask, const float& offset);
        void getSharpArea_ex(const ExPolygons& originMask, const float& offset, ExPolygons& outMask, ExPolygons& sharpMask);
    }
}

#endif // SLIC3R_SUPPORTPOINTLAYER_1634225497208_H
