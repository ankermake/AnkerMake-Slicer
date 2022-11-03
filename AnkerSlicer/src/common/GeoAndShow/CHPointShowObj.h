/*



*/


#ifndef CHPointShowObj_H
#define CHPointShowObj_H

#include "CHShowObj.h"


DEF_PTR(CHPointShowObj)
class CHPointShowObj :public CHShowObj
{
public:
	CHPointShowObj();
	virtual ~CHPointShowObj();

public:
	void create(QVector3D coord);

	virtual bool pick(int pixelX, int pixelY,PickResult& result, int pickTol = 10);
	virtual void updateToScene();
	virtual CHAABB3D calBaseAABB();
	virtual CHAABB3D calRealAABB();

	void setSize(double tsize);


public:
	QVector3D m_coord;
	double m_size;
};


#endif
