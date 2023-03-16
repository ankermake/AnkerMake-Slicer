/*



*/


#ifndef CHLine3D_H
#define CHLine3D_H

#include "CHBase.h"
#include "QVector3D"

#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

DEF_PTR(CHLine3D)
class COMMONLIB_EXPORT CHLine3D
{
public:
	CHLine3D();
	~CHLine3D();


public:

	void create(QVector3D st, QVector3D et);

	void reverse();

    
	QVector3D getStartPoint();
	QVector3D getEndPoint();

    
	QVector3D getTangent();

    
	QVector3D pointAt(double u);

    
	double getClosestPoint(const QVector3D& pt);


public:
    QVector3D m_st;
    QVector3D m_et;

};


#endif
