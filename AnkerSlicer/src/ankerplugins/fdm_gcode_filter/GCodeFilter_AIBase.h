#ifndef GCODEFILTER_AIBASE_H
#define GCODEFILTER_AIBASE_H
#include "common/utilities/GCodeConstant.hpp"
#include "QFileMemoryMap.h"


class MemRefDecorate
{
public:
    bool willAddM1024 = false;
    MemRef decorateSegment;
    MemRef oriSegment;
};

class GCodeFilter_AIBase
{
public:
    GCodeFilter_AIBase(QFileMemoryMapRead & reader, QFileMemoryMapWrite & writer, GC::GCodeInfo & info)
        :fileReader(reader), fileWriter(writer), gcodeInfo(info){}
    using SuperClass = GCodeFilter_AIBase;
    virtual ~GCodeFilter_AIBase(){}

    virtual bool running() ExceptionsMayBeThrown;
protected:
    virtual QLatin1String getProducer    () { return gcodeInfo.getProducer    (); }
    virtual QLatin1String getLayerKeyWord() { return gcodeInfo.getLayerKeyWord(); }
    virtual bool needNotInsertAiCodeAfterFirstNLayers(){return false;}

    virtual void parseGCodeParams(){}
    //split gcode to segments by express
    virtual QList<MemRefDecorate> splitGcode(QLatin1String express);

    virtual void adjustDoc(QList<MemRefDecorate>& segments);

    virtual bool infillAiGcode(QList<MemRefDecorate>& segments);
    virtual void writeLayerComment(MemRefDecorate& layerLine,int layerNumber, int layerCount, QByteArray* storeLocal = nullptr);

    virtual QString getFileEndInfo();

    virtual void writeFile(QList<MemRefDecorate>& segments);

protected:
    QFileMemoryMapRead & fileReader;
    QFileMemoryMapWrite& fileWriter;
    GC::GCodeInfo      & gcodeInfo ;

    QVector<QByteArray> decorateStore;
    //all data
    QList<MemRefDecorate> segments;
    void getMachineSize();
    QVector3D machineSize;
};

#endif // GCODEFILTER_AIBASE_H
