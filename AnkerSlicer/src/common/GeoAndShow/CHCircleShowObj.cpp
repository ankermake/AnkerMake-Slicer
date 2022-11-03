#include "CHCircleShowObj.h"
#include "TransformPack.h"


CHCircleShowObj::CHCircleShowObj()
{
	m_center = QVector3D(0, 0, 0);
	m_radius = 0;
	m_nor = QVector3D(0, 0, 0);
	m_refVec = QVector3D(0, 0, 0);
	m_priority = Curve;
}

CHCircleShowObj::~CHCircleShowObj()
{
}

void CHCircleShowObj::create(QVector3D center, double radius, QVector3D nor, QVector3D refVec)
{
	m_center = center;
	m_radius = radius;
	m_nor = nor;
	m_refVec = refVec;

    
    
    
	QMatrix4x4 tran1, tran2, tran3, tran4, sumtran;
	tran1.scale(m_radius);
	QVector3D axisZ(0, 0, 1);
	QVector3D axisX(1, 0, 0);
	tran2 = TransformPack::rotMat(axisZ, m_nor, QVector3D(0, 0, 0));
	axisX = TransformPack::vRot(tran2, axisX);
	float angle = TransformPack::vecAngle(axisX, m_refVec);
	if (QVector3D::dotProduct(m_nor, QVector3D::crossProduct(axisX, m_refVec)) < 0)
	{
		angle = -angle;
	}
	tran3.rotate(angle / CH_PI * 180.0, m_nor);
	tran4.translate(center);
	sumtran = tran4 * tran3 * tran2 * tran1;


	int nside = 64;
	const double pi2 = CH_PI * 2.0;
	m_showPoints.resize(nside);
	for (double i = 0; i < nside; i++)
	{
		m_showPoints[i].setX(cos(i * pi2 / nside));
		m_showPoints[i].setY(sin(i * pi2 / nside));
		m_showPoints[i].setZ(0);
		m_showPoints[i] = TransformPack::pRot(sumtran, m_showPoints[i]);
	}

    
	m_showPoints.push_back(m_showPoints[0]);
}

CHAABB3D CHCircleShowObj::calBaseAABB()
{
    


	return m_baseAABB;
}

CHAABB3D CHCircleShowObj::calRealAABB()
{
    


	return m_realAABB;
}










