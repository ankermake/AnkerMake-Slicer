/*



*/

#ifndef CHCone_H
#define CHCone_H

#include "CHMeshShowObj.h"


DEF_PTR(CHCone)
class CHCone :public CHMeshShowObj
{
public:
	CHCone();
	virtual ~CHCone();

public:
	bool create(float rad, float height, int sliceNum = 32);
	

private:
	float m_rad;
	float m_height;
	int m_sliceNum;
	
};


#endif
