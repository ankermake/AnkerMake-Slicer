#ifndef COORDINATE_H
#define COORDINATE_H


#include "CHAssembly.h"
#include "CHLineSegment3DShowObj.h"
#include "CHMeshShowObj.h"
#include "CHCone.h"
#include <QColor>

DEF_PTR(CoordinateAxis)
class CoordinateAxis : public CHAssembly
{
public:
    CoordinateAxis();
    virtual ~CoordinateAxis();
    virtual void setColor(QColor color);
    void setWidth(float width );
    void setLightTest(bool lightTest);

    void create(const QVector3D& center, const QVector3D& axisDir, float axisLenth, float rad, float height);
    void updateCoordinateAxis(const QVector3D& center, const QVector3D& axisDir, float axisLenth, float rad, float height);
    virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);
private:
    CHLineSegment3DShowObjPtr m_axis;
    CHConePtr m_arrow;
};

#endif
