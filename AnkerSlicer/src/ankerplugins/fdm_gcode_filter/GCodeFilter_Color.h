
#ifndef GCODEFILTER_COLOR_H
#define GCODEFILTER_COLOR_H

#include "common/utilities/GCodeConstant.hpp"
#include "QFileMemoryMap.h"

class GCodeFilter_Base{
public:
    using SuperClass = GCodeFilter_Base;
    GCodeFilter_Base(QFileMemoryMapRead & reader, QFileMemoryMapWrite & writer, GC::GCodeInfo & info)
        :fileReader(reader), fileWriter(writer), gcodeInfo(info){}
    virtual ~GCodeFilter_Base(){}

    virtual bool running() ExceptionsMayBeThrown{ return false; }
    virtual std::string getError(){ return std::string(gcodeInfo.errorStr.toLocal8Bit().data()); }
protected:
    QFileMemoryMapRead & fileReader;
    QFileMemoryMapWrite& fileWriter;
    GC::GCodeInfo      & gcodeInfo ;
};


class GCodeFilter_Color : GCodeFilter_Base
{
public:
    using SuperClass::SuperClass;
};



#endif // GCODEFILTER_COLOR_H
