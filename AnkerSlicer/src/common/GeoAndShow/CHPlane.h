/*



*/

#ifndef CHPlane_H
#define CHPlane_H

#include "CHBase.h"
#include "QVector3D"



DEF_PTR(CHPlane)
class CHPlane
{
public:
	CHPlane();
	~CHPlane();


public:

	void create(QVector3D center, QVector3D nor);



public:
	QVector3D m_center;
	QVector3D m_nor;

};


#endif
