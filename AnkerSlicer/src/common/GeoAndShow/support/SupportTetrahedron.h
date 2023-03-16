#ifndef SUPPORTTETRAHEDRON_H
#define SUPPORTTETRAHEDRON_H
#include "SupportMesh.h"
class COMMONLIB_EXPORT SupportTetrahedron : public SupportMesh
{
public:
    SupportTetrahedron();
    virtual ~SupportTetrahedron();

public:

    
    bool create(QVector3D pt1, QVector3D pt2, QVector3D pt3, QVector3D pt4);


public:
    QVector3D m_pt1;
    QVector3D m_pt2;
    QVector3D m_pt3;
    QVector3D m_pt4;
};

#endif // SUPPORTTETRAHEDRON_H
