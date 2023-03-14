/*

*

*

*

*/

#ifndef CHAABB3D_H
#define CHAABB3D_H


#include "QVector3D"
#include <QVector4D>
#include <QMatrix4x4>
#include "CHPlane.h"


class CHAABB3D
{
public:
	CHAABB3D();
	CHAABB3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	virtual ~CHAABB3D();

public:
	
	void add(const QVector3D& pt);

	
	void add(const float& x, const float& y, const float& z);

	
	void add(const CHAABB3D& aabb);

	
	QVector3D getLeftDownPoint() const;

	
	QVector3D getRightTopPoint() const;

	
	QVector3D getCenterPoint() const;

	
	float getLenX() const;

	
	float getLenY() const;

	
	float getLenZ() const;

	
	bool isContain(const CHAABB3D& aabb) const;

	
	void makeEmpty();

	
	bool isZero() const;

	
	bool isEmpty() const;

	
	bool isIntersectRay(const QVector3D& sp, const QVector3D& v);


	bool isPtInBox(QVector3D p, float eps = 1e-4) const;

	
	int getMaxLenthDim() const;

	
	float getArea() const;

	
	float getMinSquareDis(const QVector3D& pt) const;

	
	bool collideAABB(const CHAABB3D& aabb) const;

	
	bool intersectWithPlane(const CHPlanePtr& plane) const;

    
    CHAABB3D getTransformCHAABB3D(const QMatrix4x4 &trans);


	CHAABB3D operator * (float d)const;
	CHAABB3D operator / (float  d)const;
	
	float operator[](int dim)const;
	const CHAABB3D& operator=(const CHAABB3D& aabb);

private:
	float getDimDis(float xmin, float xmax, float a) const;

public:
	float m_Xmin;
	float m_Ymin;
	float m_Zmin;
	float m_Xmax;
	float m_Ymax;
	float m_Zmax;
};

#endif

