#include "GCodeFilter_AIProducer.h"




void GCodeFilter_BasedOn_Cura::parseGCodeParams()
{
    MemDocuRef docFileRef = fileReader.getMemRef();
    MemDocuRef docHeadRef = docFileRef.left(docFileRef.minSize(512));
    const TokenMapType gcodeToken = gcodeInfo.getTokenMap();

    MemLineRef line;
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_TIME            ]) )) sscanf(line.m_data, ";TIME:%f"            , &gcodeInfo.printTime          );
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_Filament_used   ]) )) sscanf(line.m_data, ";Filament used: %f"  , &gcodeInfo.printFilamenLength );
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_Filament_weight ]) )) sscanf(line.m_data, ";Filament weight: %f", &gcodeInfo.printFilamenWeight );
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_Layer_height    ]) )) sscanf(line.m_data, ";Layer height: %f"   , &gcodeInfo.layerHeight        );
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MINX            ]) )) sscanf(line.m_data, ";MINX:%f"            , &gcodeInfo.printObjectBox_minX);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MINY            ]) )) sscanf(line.m_data, ";MINY:%f"            , &gcodeInfo.printObjectBox_minY);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MINZ            ]) )) sscanf(line.m_data, ";MINZ:%f"            , &gcodeInfo.printObjectBox_minZ);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MAXX            ]) )) sscanf(line.m_data, ";MAXX:%f"            , &gcodeInfo.printObjectBox_maxX);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MAXY            ]) )) sscanf(line.m_data, ";MAXY:%f"            , &gcodeInfo.printObjectBox_maxY);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MAXZ            ]) )) sscanf(line.m_data, ";MAXZ:%f"            , &gcodeInfo.printObjectBox_maxZ);
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_MAXSPEED        ]) )) sscanf(line.m_data, ";MAXSPEED:%f"        , &gcodeInfo.printMaxSpeed      );

    docHeadRef = docFileRef.left(docFileRef.minSize(100*1000));
    if(!! (line = docHeadRef.docFindLine(gcodeToken[(int)GC::E_LAYER_COUNT     ]) )) sscanf(line.m_data, ";LAYER_COUNT:%d"     , &gcodeInfo.layerCount         );

    gcodeInfo.relative_extrusion = false;
    if (!!(line = docHeadRef.docFindLine(MemRef(gcodeInfo.newLine + QLatin1String("M83"))))) gcodeInfo.relative_extrusion = true;
}



//delete 2line after layernum
void GCodeFilter_BasedOn_Slic3r::adjustDoc(QList<MemRefDecorate> &segments)
{
    if (segments.size() == 0)
    {
         return;
    }

    SuperClass::adjustDoc(segments);

    bool startAdjust = false;
    MemRef* lastLayerLine = nullptr;
    for(int i = 0;i <segments.size();i++)
    {
        if (!startAdjust && segments[i].willAddM1024)
        {
            startAdjust = true;
        }
        if (segments[i].willAddM1024){lastLayerLine = &segments[i].oriSegment;}

        if (startAdjust)
        {
            if (!segments[i].willAddM1024)
            {
                //cutdown 2 line
                for(;;)
                {
                    auto nextLine =  segments[i].oriSegment.docFindLineDown(MemFileRef::NewLine);
                    if (!!nextLine && (nextLine.startWith(';') || nextLine.startsWith(MemFileRef::NewLine)))
                    {
                        (*lastLayerLine)       = (*lastLayerLine) + nextLine.m_size       ;   //  add line to isLayerLine
                        segments[i].oriSegment = nextLine.m_size  - segments[i].oriSegment;
                    }
                    else{
                        break;
                    }
                }
            }
        }
    }

}



