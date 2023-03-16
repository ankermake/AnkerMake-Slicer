#include "CHAABB3D.h"


CHAABB3D::CHAABB3D()
{
	makeEmpty();
}

CHAABB3D::CHAABB3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	m_Xmin = minX;
	m_Ymin = minY;
	m_Zmin = minZ;
	m_Xmax = maxX;
	m_Ymax = maxY;
	m_Zmax = maxZ;
}

CHAABB3D::~CHAABB3D()
{
}

void CHAABB3D::add(const QVector3D& pt)
{
	return add(pt.x(), pt.y(), pt.z());
}

void CHAABB3D::add(const CHAABB3D& aabb)
{
	if (m_Xmin > aabb.m_Xmin)
	{
		m_Xmin = aabb.m_Xmin;
	}
	if (m_Xmax < aabb.m_Xmax)
	{
		m_Xmax = aabb.m_Xmax;
	}
	if (m_Ymin > aabb.m_Ymin)
	{
		m_Ymin = aabb.m_Ymin;
	}
	if (m_Ymax < aabb.m_Ymax)
	{
		m_Ymax = aabb.m_Ymax;
	}
	if (m_Zmin > aabb.m_Zmin)
	{
		m_Zmin = aabb.m_Zmin;
	}
	if (m_Zmax < aabb.m_Zmax)
	{
		m_Zmax = aabb.m_Zmax;
	}
}

void CHAABB3D::add(const float& x, const float& y, const float& z)
{
	if (m_Xmin > x)
	{
		m_Xmin = x;
	}
	if (m_Xmax < x)
	{
		m_Xmax = x;
	}
	if (m_Ymin > y)
	{
		m_Ymin = y;
	}
	if (m_Ymax < y)
	{
		m_Ymax = y;
	}
	if (m_Zmin > z)
	{
		m_Zmin = z;
	}
	if (m_Zmax < z)
	{
		m_Zmax = z;
	}
}

QVector3D CHAABB3D::getLeftDownPoint() const
{
	return QVector3D(m_Xmin, m_Ymin, m_Zmin);
}

QVector3D CHAABB3D::getRightTopPoint() const
{
	return QVector3D(m_Xmax, m_Ymax, m_Zmax);
}

QVector3D CHAABB3D::getCenterPoint() const
{
	return QVector3D((m_Xmin + m_Xmax) / 2, (m_Ymin + m_Ymax) / 2, (m_Zmin + m_Zmax) / 2);
}

float CHAABB3D::getLenX() const
{
	return m_Xmax - m_Xmin;
}

float CHAABB3D::getLenY() const
{
	return m_Ymax - m_Ymin;
}

float CHAABB3D::getLenZ() const
{
	return m_Zmax - m_Zmin;
}

bool CHAABB3D::isContain(const CHAABB3D& aabb) const
{
	return m_Xmin <= aabb.m_Xmin && m_Ymin <= aabb.m_Ymin && m_Zmin <= aabb.m_Zmin &&
		m_Xmax >= aabb.m_Xmax && m_Ymax >= aabb.m_Ymax && m_Zmax >= aabb.m_Zmax;
}

void CHAABB3D::makeEmpty()
{
    m_Xmin = m_Ymin = m_Zmin = + ANKER_FLT_MAX;
    m_Xmax = m_Ymax = m_Zmax = - ANKER_FLT_MAX;
}

bool CHAABB3D::isZero() const
{
	return m_Xmin == m_Xmax && m_Ymin == m_Ymax && m_Zmin == m_Zmax;
}

bool CHAABB3D::isEmpty() const
{
	return m_Xmin > m_Xmax || m_Ymin > m_Ymax || m_Zmin > m_Zmax;
}

float CHAABB3D::operator[](int dim) const
{
	if (dim == 0)
	{
		return (m_Xmin + m_Xmax) / 2.0;
	}
	else if (dim == 1)
	{
		return (m_Ymin + m_Ymax) / 2.0;
	}
	else if (dim == 2)
	{
		return (m_Zmin + m_Zmax) / 2.0;
	}
	else
	{
		return 0;
	}
}

const CHAABB3D& CHAABB3D::operator=(const CHAABB3D& aabb)
{
	m_Xmin = aabb.m_Xmin;
	m_Ymin = aabb.m_Ymin;
	m_Zmin = aabb.m_Zmin;
	m_Xmax = aabb.m_Xmax;
	m_Ymax = aabb.m_Ymax;
	m_Zmax = aabb.m_Zmax;
	return *this;
}

float CHAABB3D::getDimDis(float xmin, float xmax, float a) const
{
	if (a < xmin)
	{
		return xmin - a;
	}
	else if (a <= xmax)
	{
		return 0;
	}
	else
	{
		return a - xmax;
	}
}

bool CHAABB3D::isIntersectRay(const QVector3D& sp, const QVector3D& v)
{
	const float kNoIntersection = 1e30f;

	bool inside = true;

	float xt, xn;
	if (sp.x() < m_Xmin)
	{
		xt = m_Xmin - sp.x();

		if (xt > v.x())
		{
			return false;
		}

		xt /= v.x();
		inside = false;
		xn = -1.0f;
	}
	else if (sp.x() > m_Xmax)
	{
		xt = m_Xmax - sp.x();

		if (xt < v.x())
		{
			return false;
		}

		xt /= v.x();
		inside = false;
		xn = 1.0f;
	}
	else
	{
		xt = -1.0f;
	}

	float yt, yn;
	if (sp.y() < m_Ymin)
	{
		yt = m_Ymin - sp.y();

		if (yt > v.y())
		{
			return false;
		}

		yt /= v.y();
		inside = false;
		yn = -1.0f;
	}
	else if (sp.y() > m_Ymax)
	{
		yt = m_Ymax - sp.y();

		if (yt < v.y())
		{
			return false;
		}

		yt /= v.y();
		inside = false;
		yn = 1.0f;
	}
	else
	{
		yt = -1.0f;
	}

	float zt, zn;
	if (sp.z() < m_Zmin)
	{
		zt = m_Zmin - sp.z();

		if (zt > v.z())
		{
			return false;
		}

		zt /= v.z();
		inside = false;
		zn = -1.0f;
	}
	else if (sp.z() > m_Zmax)
	{
		zt = m_Zmax - sp.z();

		if (zt < v.z())
		{
			return false;
		}

		zt /= v.z();
		inside = false;
		zn = 1.0f;
	}
	else
	{
		zt = -1.0f;
	}

	// Inside box?

	if (inside)
	{
		return true;
	}

	// Select farthest plane - this is
	// the plane of intersection.

	int which = 0;
	float t = xt;
	if (yt > t) {
		which = 1;
		t = yt;
	}
	if (zt > t) {
		which = 2;
		t = zt;
	}

	switch (which) {

	case 0: // intersect with yz plane
	{
		float y = sp.y() + v.y() * t;
		if (y < m_Ymin || y > m_Ymax) return false;
		float z = sp.z() + v.z() * t;
		if (z <m_Zmin || z > m_Zmax) return false;


	} break;

	case 1: // intersect with xz plane
	{
		float x = sp.x() + v.x() * t;
		if (x < m_Xmin || x > m_Xmax) return false;
		float z = sp.z() + v.z() * t;
		if (z <m_Zmin || z > m_Zmax) return false;


	} break;

	case 2: // intersect with xy plane
	{
		float x = sp.x() + v.x() * t;
		if (x < m_Xmin || x > m_Xmax) return false;
		float y = sp.y() + v.y() * t;
		if (y < m_Ymin || y > m_Ymax) return false;

	} break;
	}

	if (t > 1)
	{
		return false;
	}

	return true;
}

bool CHAABB3D::isPtInBox(QVector3D p, float eps) const
{
	if (m_Xmin - eps < p.x() && p.x() < m_Xmax + eps &&
		m_Ymin - eps < p.y() && p.y() < m_Ymax + eps &&
		m_Zmin - eps < p.z() && p.z() < m_Zmax + eps)
		return 1;

	return 0;
}

int CHAABB3D::getMaxLenthDim() const
{
	int dim = 0;
	float lenth = getLenX();
	if (getLenY() > lenth)
	{
		dim = 1;
		lenth = getLenY();
	}
	if (getLenZ() > lenth)
	{
		dim = 2;
		lenth = getLenZ();
	}
	return dim;
}

float CHAABB3D::getArea() const
{
	float a = getLenX();
	float b = getLenY();
	float c = getLenZ();
	return (a * b + a * c + b * c) * 2.0;
}

float CHAABB3D::getMinSquareDis(const QVector3D& pt) const
{
	float a = getDimDis(m_Xmin, m_Xmax, pt.x());
	float b = getDimDis(m_Ymin, m_Ymax, pt.y());
	float c = getDimDis(m_Zmin, m_Zmax, pt.z());
	return a * a + b * b + c * c;
}

bool CHAABB3D::collideAABB(const CHAABB3D& aabb) const
{
    
	return !(aabb.m_Xmin > m_Xmax || aabb.m_Xmax < m_Xmin || aabb.m_Ymin > m_Ymax || aabb.m_Ymax < m_Ymin ||
		aabb.m_Zmin > m_Zmax || aabb.m_Zmax < m_Zmin);
}

bool CHAABB3D::intersectWithPlane(const CHPlanePtr& plane) const
{
    
    
	QVector3D center = plane->m_center;
	QVector3D nor = plane->m_nor;
	QVector3D ptmin, ptmax;
	if (nor.x() > 0)
	{
		ptmin.setX(m_Xmin);
		ptmax.setX(m_Xmax);
	}
	else
	{
		ptmin.setX(m_Xmax);
		ptmax.setX(m_Xmin);
	}

	if (nor.y() > 0)
	{
		ptmin.setY(m_Ymin);
		ptmax.setY(m_Ymax);
	}
	else
	{
		ptmin.setY(m_Ymax);
		ptmax.setY(m_Ymin);
	}

	if (nor.z() > 0)
	{
		ptmin.setZ(m_Zmin);
		ptmax.setZ(m_Zmax);
	}
	else
	{
		ptmin.setZ(m_Zmax);
		ptmax.setZ(m_Zmin);
	}

	float dis1 = QVector3D::dotProduct(ptmin - center, nor);
	float dis2 = QVector3D::dotProduct(ptmax - center, nor);
    return dis1 * dis2 <= 0;
}

CHAABB3D CHAABB3D::getTransformCHAABB3D(const QMatrix4x4 &trans)
{
    CHAABB3D aabb = *this;
    QVector3D ptMin(aabb.m_Xmin, aabb.m_Ymin, aabb.m_Zmin);
    QVector3D ptMax(aabb.m_Xmax, aabb.m_Ymax, aabb.m_Zmax);
    ptMin = (trans * QVector4D(ptMin)).toVector3D();
    ptMax = (trans * QVector4D(ptMax)).toVector3D();
    aabb.m_Xmin = ptMin.x();
    aabb.m_Ymin = ptMin.y();
    aabb.m_Zmin = ptMin.z();

    aabb.m_Xmax = ptMax.x();
    aabb.m_Ymax = ptMax.y();
    aabb.m_Zmax = ptMax.z();
    return aabb;
}

CHAABB3D CHAABB3D::operator*(float d) const
{
	CHAABB3D rtn;
	rtn.m_Xmin = m_Xmin * d;
	rtn.m_Ymin = m_Ymin * d;
	rtn.m_Zmin = m_Zmin * d;
	rtn.m_Xmax = m_Xmax * d;
	rtn.m_Ymax = m_Ymax * d;
	rtn.m_Zmax = m_Zmax * d;
	return rtn;
}

CHAABB3D CHAABB3D::operator/(float d) const
{
	CHAABB3D rtn;
	if (fabs(d) > 0.000000001)
	{
		rtn.m_Xmin = m_Xmin / d;
		rtn.m_Ymin = m_Ymin / d;
		rtn.m_Zmin = m_Zmin / d;
		rtn.m_Xmax = m_Xmax / d;
		rtn.m_Ymax = m_Ymax / d;
		rtn.m_Zmax = m_Zmax / d;
	}
	else
	{
		rtn = *this;
	}
	return rtn;
}


