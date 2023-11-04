#ifndef PRINT_FEATURE
#define PRINT_FEATURE

#include <map>
namespace cura
{

enum class PrintFeatureType: unsigned char
{
    NoneType = 0, // used to mark unspecified jumps in polygons. libArcus depends on it
    OuterWall = 1,
    InnerWall = 2,
    Skin = 3,
    Support = 4,
    SkirtBrim = 5,
    Infill = 6,
    SupportInfill = 7,
    MoveCombing = 8,
    MoveRetraction = 9,
    SupportInterface = 10,
    PrimeTower = 11,
    NumPrintFeatureTypes = 12 // this number MUST be the last one because other modules will
                              // use this symbol to get the total number of types, which can
                              // be used to create an array or so
};

static std::map<PrintFeatureType, const char *> PrintFeatureTypeMap(){
    static std::map<PrintFeatureType, const char *> map;
    if(map.size() == 0){
        map = {
            {PrintFeatureType::NoneType               , " --> [NoneType            ]"},
            {PrintFeatureType::OuterWall              , " --> [OuterWall           ]"},
            {PrintFeatureType::InnerWall              , " --> [InnerWall           ]"},
            {PrintFeatureType::Skin                   , " --> [Skin                ]"},
            {PrintFeatureType::Support                , " --> [Support             ]"},
            {PrintFeatureType::SkirtBrim              , " --> [SkirtBrim           ]"},
            {PrintFeatureType::Infill                 , " --> [Infill              ]"},
            {PrintFeatureType::SupportInfill          , " --> [SupportInfill       ]"},
            {PrintFeatureType::MoveCombing            , " --> [MoveCombing         ]"},
            {PrintFeatureType::MoveRetraction         , " --> [MoveRetraction      ]"},
            {PrintFeatureType::SupportInterface       , " --> [SupportInterface    ]"},
            {PrintFeatureType::PrimeTower             , " --> [PrimeTower          ]"},
            {PrintFeatureType::NumPrintFeatureTypes   , " --> [NumPrintFeatureTypes]"}
        };
    }
    return map;
}


} // namespace cura

#endif // PRINT_FEATURE
