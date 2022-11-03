#include "sphere.h"

Sphere::Sphere()
{

}

void Sphere::create(const QVector3D &center, float radious)
{
    m_center = center;
    m_radious = radious;
}

void Sphere::add(const Sphere &sphere)
{

}

float Sphere::getRadious() const
{
    return m_radious;
}

QVector3D Sphere::getCenter() const
{
    return m_center;
}

QVector3D Sphere::getLeftPoint() const
{
    return QVector3D(m_center.x() - m_radious, m_center.y(), m_center.z());
}

QVector3D Sphere::getRightPoint() const
{
    return QVector3D(m_center.x() + m_radious, m_center.y(), m_center.z());
}

QVector3D Sphere::getUpPoint() const
{
    return QVector3D(m_center.x(), m_center.y(), m_center.z() + m_radious);
}

QVector3D Sphere::getDownPoint() const
{
    return QVector3D(m_center.x(), m_center.y(), m_center.z() - m_radious);
}

QVector3D Sphere::getFrontPoint() const
{
    return QVector3D(m_center.x(), m_center.y() + m_radious, m_center.z());
}

QVector3D Sphere::getRearPoint() const
{
    return QVector3D(m_center.x(), m_center.y() - m_radious, m_center.z());
}
