#ifndef SUPPORTCONE_H
#define SUPPORTCONE_H
#include "SupportMesh.h"

class SupportCone : public SupportMesh
{

public:
    SupportCone();
    virtual ~SupportCone();

public:
    bool create(float rad, float height, int sliceNum = 32);

public:
    float m_rad;
    float m_height;
    int m_sliceNum;

};
#endif // SUPPORTCONE_H
