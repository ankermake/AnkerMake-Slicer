/*



*/

#ifndef CHCuboid_H
#define CHCuboid_H

#include "CHMeshShowObj.h"
#include "../common/ml_document/cmesh.h"


DEF_PTR(CHCuboid)
class CHCuboid :public CHMeshShowObj
{
public:
    CHCuboid();
    virtual ~CHCuboid();

public:

    
    bool create(float lenth, float wide, float height);
    void loadProjectInitSupportTri();
    void cmeshoToMyModel(const CMeshO& mesh);
    void setLWH(float lenth, float wide, float height);

    QMatrix4x4 getLocalTransform() const;
private:
    float m_lenth;
    float m_wide;
    float m_height;

    QMatrix4x4 m_localTransform;

};


#endif
