/*



*/

#ifndef CHCylinder_H
#define CHCylinder_H

#include "CHMeshShowObj.h"


DEF_PTR(CHCylinder)
class CHCylinder :public CHMeshShowObj
{
public:
	CHCylinder();
	virtual ~CHCylinder();

public:
	
	bool create(float topRad, float downRad, float height, int sliceNum = 30);


private:
	float m_topRad;
	float m_downRad;
	float m_height;
	
};


#endif
