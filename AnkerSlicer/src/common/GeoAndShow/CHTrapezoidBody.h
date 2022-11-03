/*



*/

#ifndef CHTrapezoidBody_H
#define CHTrapezoidBody_H

#include "CHMeshShowObj.h"


DEF_PTR(CHTrapezoidBody)
class CHTrapezoidBody :public CHMeshShowObj
{
public:
	CHTrapezoidBody();
	virtual ~CHTrapezoidBody();

public:
	
	bool create(QVector3D topcenter, QVector3D downcenter, float toplenth, float topwide, float downlenth, float downwide);


private:
	QVector3D m_topCenter;
	QVector3D m_downCenter;
	float m_topLenth;
	float m_topWide;
	float m_downLenth;
	float m_downWide;
	
};


#endif
