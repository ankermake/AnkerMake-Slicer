/*



*/


#ifndef CHCircleShowObj_H
#define CHCircleShowObj_H

#include "CHCurveShowObj.h"



DEF_PTR(CHCircleShowObj)
class COMMONLIB_EXPORT CHCircleShowObj :public CHCurveShowObj
{
public:
	CHCircleShowObj();
	virtual ~CHCircleShowObj();

public:
	void create(QVector3D center, double radius, QVector3D nor, QVector3D refVec);

	virtual CHAABB3D calBaseAABB();
	virtual CHAABB3D calRealAABB();

public:
	QVector3D m_center;
	double m_radius;
	QVector3D m_nor;
	QVector3D m_refVec;
};


#endif
