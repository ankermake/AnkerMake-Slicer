/*



*/

#ifndef CHTetrahedron_H
#define CHTetrahedron_H

#include "CHMeshShowObj.h"


DEF_PTR(CHTetrahedron)
class CHTetrahedron :public CHMeshShowObj
{
public:
	CHTetrahedron();
	virtual ~CHTetrahedron();

public:

	
	bool create(QVector3D pt1, QVector3D pt2, QVector3D pt3, QVector3D pt4);


private:
	QVector3D m_pt1;
	QVector3D m_pt2;
	QVector3D m_pt3;
	QVector3D m_pt4;
	
};


#endif
