/*



*/



#ifndef CHMeshShowObj_H
#define CHMeshShowObj_H

#include "CHShowObj.h"
#include "QVector2D"
#include "../common/ml_document/cmesh.h"

enum ShowMode
{
    MeshShowVertex = 0,
    MeshShowEdge,
    MeshShowFace
};


struct MyTriangle
{
    MyTriangle() {}
    MyTriangle(const int& index1, const int& index2, const int& index3)
    {
        m_index1 = index1;
        m_index2 = index2;
        m_index3 = index3;
    }
    int m_index1;
    int m_index2;
    int m_index3;
};


DEF_PTR(CHMeshShowObj)
class CHMeshShowObj :public CHShowObj
{
public:
    CHMeshShowObj();
    virtual ~CHMeshShowObj();

public:
    void setLogo(bool logo);
    bool isLogo() const;

    void transform(const QMatrix4x4& matrix);

    
    void resetTransform();
    void resetMove();
    void resetRot();
    void resetZoom();
    void resetMirr();

    virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);
    virtual bool pickAABB(int pixelX, int pixelY, int pickTol = 10);
    virtual void updateToScene();
    virtual CHAABB3D calBaseAABB();
    virtual CHAABB3D calRealAABB();

    virtual void copy(CHShowObjPtr& obj);

    void cmeshoToMeshShowObj(CMeshO& mesh);

    
    
    virtual QString serialize(QString path);
    virtual CHMeshShowObjPtr deserialize(QString xmlContent);

public:
    std::vector<QVector3D> m_vertices;
    std::vector<QVector3D> m_nors;
    std::vector<QVector2D> m_uvs;
    std::vector<MyTriangle> m_trians;

    ShowMode m_showMode;

    
    
    QVector3D m_rotCenter;
    std::vector<float> m_params;
    float m_lastRotParams[3] = {-1}; 
    int m_mirrAxis[3] = {1, 1, 1};

    std::vector<float> m_initParams;
    QMatrix4x4 m_initTransform;

    QString m_meshPath;

    bool m_logo; 

    bool m_mirror = false; 
};


#endif
