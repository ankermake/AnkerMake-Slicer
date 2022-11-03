#include "CHPointShowObj.h"
#include "TransformPack.h"
#include "QVector2D"
#include "CHScene.h"


CHPointShowObj::CHPointShowObj()
{
	m_coord = QVector3D(0, 0, 0);
	m_priority = Point;
	m_size = 4;
}

CHPointShowObj::~CHPointShowObj()
{
}

void CHPointShowObj::create(QVector3D coord)
{
    m_tran.setToIdentity();
	m_coord = coord;

	m_baseAABB.makeEmpty();
	m_baseAABB.add(m_coord);
}


bool CHPointShowObj::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
    
	QVector3D coord = TransformPack::pRot(m_tran, m_coord);
	float x, y, z;
	curScene->worldToPixel(coord, x, y, z);
	if (QVector2D(x - pixelX, y - pixelY).lengthSquared() < pickTol * pickTol)
	{
		result.m_ptOnObj = coord;
		result.m_deep = z;
		result.m_pickObj = shared_from_this();
		return true;
	}
	return false;
}

void CHPointShowObj::updateToScene()
{
	if (curScene)
	{
		curScene->updateShowObj(shared_from_this());
	}
}

CHAABB3D CHPointShowObj::calBaseAABB()
{
	m_baseAABB.makeEmpty();
	m_baseAABB.add(m_coord);
	return m_baseAABB;
}

CHAABB3D CHPointShowObj::calRealAABB()
{
	m_realAABB.makeEmpty();
	m_realAABB.add(TransformPack::pRot(m_tran, m_coord));
	return m_realAABB;
}

void CHPointShowObj::setSize(double tsize)
{
	m_size = tsize;
}











