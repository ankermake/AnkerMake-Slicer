#ifndef SUPPORTASSEMBLYMESHES_H
#define SUPPORTASSEMBLYMESHES_H
#include "SupportMesh.h"


class SupportAssemblyMeshes: public SupportMesh
{
public:
    SupportAssemblyMeshes();
    virtual ~SupportAssemblyMeshes();

public:

    virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);
    virtual void updateToScene();
    virtual CHAABB3D calBaseAABB();
    virtual CHAABB3D calRealAABB();

    virtual void setColor(QColor color);
    virtual void setTransform(QMatrix4x4 tran);
    virtual void setVisuable(bool show);
    virtual void setDelete(bool del);
    virtual void setDirty(bool dirty);
    virtual void setLightTest(bool value);
    virtual void setCalLight(bool value);
    virtual void setStatus(ObjStatus status);
    void append(const SupportMeshPtr supportPtr);

    virtual void copy(CHShowObjPtr& obj);

public:
    
    std::vector<SupportMeshPtr> m_baseShowObjs;
};

#endif // SUPPORTASSEMBLYMESHES_H
