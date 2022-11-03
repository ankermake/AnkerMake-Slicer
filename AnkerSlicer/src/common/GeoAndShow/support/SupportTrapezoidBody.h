#ifndef SUPPORTTRAPEZOIDBODY_H
#define SUPPORTTRAPEZOIDBODY_H
#include "SupportMesh.h"

class SupportTrapezoidBody :public SupportMesh
{
public:
    SupportTrapezoidBody();
    virtual ~SupportTrapezoidBody();

public:
    bool create(QVector3D topcenter, QVector3D downcenter, float toplenth, float topwide, float downlenth, float downwide);

public:
    QVector3D m_topCenter;
    QVector3D m_downCenter;
    float m_topLenth;
    float m_topWide;
    float m_downLenth;
    float m_downWide;

};
#endif // SUPPORTTRAPEZOIDBODY_H
