#include "CHRenderData.h"
//#include <QOpenGLFunctions>
//#include <QOpenGLWidget>
#include "common/GeoAndShow/CHSphere.h"

CHRenderMesh::CHRenderMesh()
{
}

CHRenderMesh::~CHRenderMesh()
{
	
}

CHRenderPointSphere::CHRenderPointSphere()
{
}

CHRenderPointSphere::~CHRenderPointSphere()
{
}

CHRenderCurve::CHRenderCurve()
{
}

CHRenderCurve::~CHRenderCurve()
{
}

CHRenderData::CHRenderData()
{
	m_baseShowPoint = CHRenderPointSpherePtr(new CHRenderPointSphere);
	m_unitLine = CHRenderCurvePtr(new CHRenderCurve);
}

CHRenderData::~CHRenderData()
{
}

CHMeshShowObjPtr CHRenderData::getSphereMesh()
{
    CHSpherePtr out(new CHSphere);
    out->create(QVector3D());
    return out;
}


