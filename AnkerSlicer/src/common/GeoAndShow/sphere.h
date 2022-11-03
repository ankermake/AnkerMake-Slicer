#ifndef SPHERE_H
#define SPHERE_H

#include <QVector3D>

class Sphere
{
public:
    Sphere();
    void create(const QVector3D &center, float radious);
    void add(const Sphere &sphere);
    float getRadious() const;
    QVector3D getCenter() const;
    QVector3D getLeftPoint() const;
    QVector3D getRightPoint() const;
    QVector3D getUpPoint() const;
    QVector3D getDownPoint() const;
    QVector3D getFrontPoint() const;
    QVector3D getRearPoint() const;

private:
    QVector3D m_center;
    float m_radious;
};

#endif // SPHERE_H
