#ifndef GCODEFILTER_AIPRODUCER_H
#define GCODEFILTER_AIPRODUCER_H
#include "common/utilities/GCodeConstant.hpp"
#include "QFileMemoryMap.h"
#include "GCodeFilter_AIBase.h"


//  ================================== BasedOn_Cura ==================================
class GCodeFilter_BasedOn_Cura : public GCodeFilter_AIBase
{
protected:  // Direct construction is not allowed
    using SuperClass::SuperClass;
    using CategoryClass = GCodeFilter_BasedOn_Cura;

    virtual void parseGCodeParams() override;
};

//  ---------------- GCodeFilter_AIAnkerMake ----------------
class GCodeFilter_AIAnkerMake : public GCodeFilter_BasedOn_Cura
{
public:
    using CategoryClass::CategoryClass;
};

//  ---------------- GCodeFilter_AICura ----------------
class GCodeFilter_AICura : public GCodeFilter_BasedOn_Cura
{
public:
    using CategoryClass::CategoryClass;
};




//  ================================== BasedOn_Slic3r ==================================
class GCodeFilter_BasedOn_Slic3r : public GCodeFilter_AIBase
{
protected:  // Direct construction is not allowed
    using SuperClass::SuperClass;
    using CategoryClass = GCodeFilter_BasedOn_Slic3r;

    //delete 2line after layernum
    virtual void adjustDoc(QList<MemRefDecorate>& segments) override;
};

//  ---------------- GCodeFilter_AIPrusa ----------------
class GCodeFilter_AIPrusa : public GCodeFilter_BasedOn_Slic3r
{
public:
    using CategoryClass::CategoryClass;
};


//  ---------------- GCodeFilter_AISuper ----------------
class GCodeFilter_AISuper : public GCodeFilter_BasedOn_Slic3r
{
public:
    using CategoryClass::CategoryClass;
};

//  ---------------- GCodeFilter_AISuper ----------------
class GCodeFilter_AIAnkerPrusa : public GCodeFilter_BasedOn_Slic3r
{
public:
    using CategoryClass::CategoryClass;
};


//  ================================== BasedOn_Simplify3D ==================================
class GCodeFilterAI_Simplify3D : public GCodeFilter_AIBase
{
public:
    using SuperClass::SuperClass;
protected:
//    virtual bool running() override{
//        fileWriter.toBeDelete = true;
//        gcodeInfo.dstGcodeFilePath = gcodeInfo.srcGcodeFilePath;
//        return true;
//    }
};




#endif // GCODEFILTER_AIPRODUCER_H
