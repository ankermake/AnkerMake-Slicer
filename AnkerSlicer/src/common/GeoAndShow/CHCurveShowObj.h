/*



*/



#ifndef CHCurveShowObj_H
#define CHCurveShowObj_H

#include "CHShowObj.h"


DEF_PTR(CHCurveShowObj)
class CHCurveShowObj :public CHShowObj
{
public:
	CHCurveShowObj();
	virtual ~CHCurveShowObj();

public:

	virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);
	virtual void updateToScene();

	std::vector<QVector3D>& getShowPoints();

	void setLineWide(float value);
	float getLineWide();

protected:
	std::vector<QVector3D> m_showPoints;

	float m_lineWide;

};


#endif
