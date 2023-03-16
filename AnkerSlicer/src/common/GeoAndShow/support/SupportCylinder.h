#ifndef SUPPORTCYLINDER_H
#define SUPPORTCYLINDER_H
#include "SupportMesh.h"

class COMMONLIB_EXPORT SupportCylinder : public SupportMesh
{
public:
    SupportCylinder();
    virtual ~SupportCylinder();

public:
    
    bool create(float topRad, float downRad, float height, int sliceNum = 30);

public:
    float m_topRad;
    float m_downRad;
    float m_height;
};

#endif // SUPPORTCYLINDER_H
