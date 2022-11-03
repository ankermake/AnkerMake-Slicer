#ifndef SUPPORTSPHERE_H
#define SUPPORTSPHERE_H
#include "SupportMesh.h"

class SupportSphere : public SupportMesh
{
public:
    SupportSphere();
    virtual ~SupportSphere();

public:
    
    
    bool create(QVector3D center, float rad = 1.0, int hDivide = 30, int vDivide = 15);


public:
    QVector3D m_center;
    float m_rad;
};

#endif // SUPPORTSPHERE_H
