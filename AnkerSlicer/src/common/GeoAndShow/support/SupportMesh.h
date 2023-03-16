#ifndef SUPPORTMESH_H
#define SUPPORTMESH_H
#include "../CHBase.h"
#include "../CHMeshShowObj.h"
#include "../TransformPack.h"

DEF_PTR(SupportMesh)
DEF_PTR(SupportAssemblyMeshes)
DEF_PTR(SupportSphere)
DEF_PTR(SupportCone)
DEF_PTR(SupportTetrahedron)
DEF_PTR(SupportTrapezoidBody)
DEF_PTR(SupportCuboid)
DEF_PTR(SupportCylinder)

class COMMONLIB_EXPORT SupportMesh: public CHMeshShowObj
{
public:
    enum EMeshType
    {
        AssemblyMeshes = 0,
        Cuboid,
        Cone,
        Cylinder,
        Sphere,
        Tetrahedron,
        TrapezoidBody
    };

public:
    SupportMesh();
    virtual EMeshType getType() { return m_type; }
    virtual void setType(const EMeshType& type) { m_type = type; }
    virtual void calculate() {}
    virtual void copy(CHShowObjPtr& obj);
    
    virtual QString serialize(QString path);
    virtual CHMeshShowObjPtr deserialize(QString xmlContent);

    QMatrix4x4 getLocalTransform() const;
    void setLocalTransform(const QMatrix4x4& mat);

    //QMatrix4x4 getInitTransform() const;
    //void setInitTransform(const QMatrix4x4& mat);

    void initSupportParams();

    virtual inline void setParent(CHShowObj* parent) {m_parent = parent;}
    virtual inline CHShowObj* getParent() {return m_parent;}

protected:
    QMatrix4x4 m_localTransform;

    

    CHShowObj* m_parent;
    EMeshType m_type;
};


#endif // SUPPORTMESH_H
