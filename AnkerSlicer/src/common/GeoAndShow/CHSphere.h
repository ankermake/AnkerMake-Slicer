/*



*/

#ifndef CHSphere_H
#define CHSphere_H

#include "CHMeshShowObj.h"


DEF_PTR(CHSphere)
class CHSphere :public CHMeshShowObj
{
public:
	CHSphere();
	virtual ~CHSphere();

public:
	
	
	bool create(QVector3D center, float rad = 1.0, int hDivide = 30, int vDivide = 15);
	

private:
	QVector3D m_center;
	float m_rad;	
	
};


#endif
