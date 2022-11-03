#ifndef SUPPORTCUBOID_H
#define SUPPORTCUBOID_H
#include "SupportMesh.h"

class SupportCuboid :public SupportMesh
{
public:
    bool create(float lenth, float wide, float height);
public:
    float m_lenth;
    float m_wide;
    float m_height;

};
#endif // SUPPORTCUBOID_H
