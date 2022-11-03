#include "CHShowObj.h"
#include "TransformPack.h"


CHShowObj::CHShowObj()
{
    m_color = QColor(0, 0, 0);
	m_visuable = true;
	m_delete = false;
	m_dirty = false;
	m_lightTest = true;
	m_calLight = true;
	m_tran.setToIdentity();
	m_status = general;
}

CHShowObj::~CHShowObj()
{
}

CHAABB3D CHShowObj::calSimilarAABB()
{
	CHAABB3D aabb;
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmin, m_baseAABB.m_Ymin, m_baseAABB.m_Zmin)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmin, m_baseAABB.m_Ymin, m_baseAABB.m_Zmax)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmin, m_baseAABB.m_Ymax, m_baseAABB.m_Zmin)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmin, m_baseAABB.m_Ymax, m_baseAABB.m_Zmax)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmax, m_baseAABB.m_Ymin, m_baseAABB.m_Zmin)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmax, m_baseAABB.m_Ymin, m_baseAABB.m_Zmax)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmax, m_baseAABB.m_Ymax, m_baseAABB.m_Zmin)));
	aabb.add(TransformPack::pRot(m_tran, QVector3D(m_baseAABB.m_Xmax, m_baseAABB.m_Ymax, m_baseAABB.m_Zmax)));
	return aabb;
}

CHAABB3D CHShowObj::getBaseAABB()
{
	return m_baseAABB;
}

CHAABB3D CHShowObj::getRealAABB()
{
	return m_realAABB;
}

void CHShowObj::setColor(QColor color)
{
	m_color = color;
}

QColor CHShowObj::getColor()
{
	return m_color;
}

void CHShowObj::setTransform(QMatrix4x4 tran)
{
	m_tran = tran;
}

QMatrix4x4 CHShowObj::getTransform()
{
	return m_tran;
}

void CHShowObj::setVisuable(bool show)
{
	m_visuable = show;
}

bool CHShowObj::getVisuable()
{
	return m_visuable;
}

void CHShowObj::setDelete(bool del)
{
	m_delete = del;
}

bool CHShowObj::getDelete()
{
	return m_delete;
}

void CHShowObj::setDirty(bool dirty)
{
	m_dirty = dirty;
}

bool CHShowObj::getDirty()
{
	return m_dirty;
}

void CHShowObj::setLightTest(bool value)
{
	m_lightTest = value;
}

bool CHShowObj::getLightTest()
{
	return m_lightTest;
}

void CHShowObj::setCalLight(bool value)
{
	m_calLight = value;
}

bool CHShowObj::getCalLight()
{
	return m_calLight;
}

void CHShowObj::setStatus(ObjStatus status)
{
	m_status = status;
}

ObjStatus CHShowObj::getStatus()
{
	return m_status;
}

PickPriority CHShowObj::getPickPriority()
{
	return m_priority;
}

