#include "CHLine3D.h"


CHLine3D::CHLine3D()
{	
	
}

CHLine3D::~CHLine3D()
{
	
}

void CHLine3D::create(QVector3D st, QVector3D et)
{
	m_st = st;
	m_et = et;
}

void CHLine3D::reverse()
{
	std::swap(m_st, m_et);
}

QVector3D CHLine3D::getStartPoint()
{
	return m_st;
}

QVector3D CHLine3D::getEndPoint()
{
	return m_et;
}

QVector3D CHLine3D::getTangent()
{
	return (m_et - m_st).normalized();
}


QVector3D CHLine3D::pointAt(double u)
{
	return m_st * (1 - u) + m_et * u;
}

double CHLine3D::getClosestPoint(const QVector3D& pt)
{
	QVector3D vecAB = m_et - m_st;
	QVector3D vecAC = pt - m_st;
	return QVector3D::dotProduct(vecAC, vecAB) / vecAB.lengthSquared();	
}













