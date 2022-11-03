#include "CHLineSegment3DShowObj.h"
#include "TransformPack.h"
#include "CHLine3D.h"
#include "CHBaseAlg.h"
#include "CHScene.h"


CHLineSegment3DShowObj::CHLineSegment3DShowObj()
{
	m_st = QVector3D(0, 0, 0);
	m_et = QVector3D(0, 0, 0);
}

CHLineSegment3DShowObj::~CHLineSegment3DShowObj()
{
}

void CHLineSegment3DShowObj::create(QVector3D st, QVector3D et)
{
    m_tran.setToIdentity();
	m_st = st;
	m_et = et;

	setDirty(true);
	m_showPoints.clear();
	m_showPoints.push_back(m_st);
	m_showPoints.push_back(m_et);

    calBaseAABB();
}

bool CHLineSegment3DShowObj::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
	float res1[3], res2[3];
	QVector3D coord1 = TransformPack::pRot(m_tran, m_st);
	QVector3D coord2 = TransformPack::pRot(m_tran, m_et);
	curScene->worldToPixel(coord1, res1[0], res1[1], res1[2]);
	curScene->worldToPixel(coord2, res2[0], res2[1], res2[2]);
	CHLineSegment3DShowObjPtr lineSegment(new CHLineSegment3DShowObj);
	lineSegment->create(QVector3D(res1[0], res1[1], res1[2]), QVector3D(res2[0], res2[1], res2[2]));
	CHLine3DPtr ray(new CHLine3D);
	ray->create(QVector3D(pixelX, pixelY, 1), QVector3D(pixelX, pixelY, -1));
	LineLineExtremeResult eres;
	CHBaseAlg::instance()->extremeBetweenLineSegmentAndLine(lineSegment, ray, eres);
	if (!eres.m_parallel && (eres.m_pt1 - eres.m_pt2).lengthSquared() < pickTol * pickTol)
	{
		QVector3D ppt;
		curScene->pixelToWorld(eres.m_pt1[0], eres.m_pt1[1], eres.m_pt1[2], ppt);
		result.m_uOnObj = getClosestPoint(ppt);
		result.m_ptOnObj = pointAt(result.m_uOnObj);
		result.m_deep = eres.m_pt2[2];
		result.m_pickObj = shared_from_this();
		return true;
	}
	return false;
}

CHAABB3D CHLineSegment3DShowObj::calBaseAABB()
{
	m_baseAABB.makeEmpty();
	m_baseAABB.add(m_st);
	m_baseAABB.add(m_et);
	return m_baseAABB;
}

CHAABB3D CHLineSegment3DShowObj::calRealAABB()
{
	m_realAABB.makeEmpty();
	m_realAABB.add(TransformPack::pRot(m_tran, m_st));
	m_realAABB.add(TransformPack::pRot(m_tran, m_et));
	return m_realAABB;
}

double CHLineSegment3DShowObj::getClosestPoint(const QVector3D& pt)
{
    
	QVector3D st = TransformPack::pRot(m_tran, m_st);
	QVector3D et = TransformPack::pRot(m_tran, m_et);

	QVector3D vecAB = et - st;
	QVector3D vecAC = pt - st;
	double u = QVector3D::dotProduct(vecAC, vecAB) / vecAB.lengthSquared();
	if (u < 0)
	{
		u = 0;
	}
	if (u > 1)
	{
		u = 1;
	}
    return u;
}

double CHLineSegment3DShowObj::getLineLength()
{
    return (double)m_et.distanceToPoint(m_st);
}

QVector3D CHLineSegment3DShowObj::pointAt(double u)
{
    
    return TransformPack::pRot(m_tran, m_st * (1 - u) + m_et * u);
}

QVector3D CHLineSegment3DShowObj::getCurrentStPoint()
{
    return (m_tran * QVector4D(m_st, 1)).toVector3D();
}










