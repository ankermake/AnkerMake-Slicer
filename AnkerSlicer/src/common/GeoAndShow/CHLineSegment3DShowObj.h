/*



*/

#ifndef CHLineSegment3DShowObj_H
#define CHLineSegment3DShowObj_H

#include "CHCurveShowObj.h"


DEF_PTR(CHLineSegment3DShowObj)
class COMMONLIB_EXPORT CHLineSegment3DShowObj :public CHCurveShowObj
{
public:
	CHLineSegment3DShowObj();
	virtual ~CHLineSegment3DShowObj();

public:
	void create(QVector3D st, QVector3D et);

	virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);

	virtual CHAABB3D calBaseAABB();
	virtual CHAABB3D calRealAABB();

	double getClosestPoint(const QVector3D& pt);
    double getLineLength();
	//????u????
	QVector3D pointAt(double u);

    QVector3D getCurrentStPoint();


public:
	QVector3D m_st;
	QVector3D m_et;
};


#endif
