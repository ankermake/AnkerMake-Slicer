/*



*/

#ifndef CH3DPrintModel_H
#define CH3DPrintModel_H

#include "CHMeshShowObj.h"
#include "../ml_document/cmesh.h"
#include "CHCuboid.h"
#include "support/SupportMesh.h"


//DEF_PTR(CHSupportMesh)

DEF_PTR(CH3DPrintModel)
class CH3DPrintModel :public CHMeshShowObj
{
public:
    CH3DPrintModel();
    virtual ~CH3DPrintModel();


public:

    virtual void updateToScene();

    virtual void copy(CHShowObjPtr& obj);

    virtual void setTransform(QMatrix4x4 tran);
    virtual void setVisuable(bool show);
    virtual void setDelete(bool del);

    
    void addSupportMesh(CHMeshShowObjPtr supportModel);
    void deleteSupportMesh(const CHMeshShowObjPtr supportModel);
    void deleteAllSupportMeshes();

    void cmeshToMyModel();

    bool getIsSceneIn()	const;
    void setIsSceneIn(bool value);
    bool isSceneIn(const CH3DPrintModel& model);
    bool isSceneIn(const CHAABB3D& modelBox, const CHAABB3D& baseBox);
    bool isBoxIn(const CHAABB3D& box, const QVector3D& point);
    void getBoxPoints(const CHAABB3D& box, std::vector<QVector3D>& points);

    
    bool matrixEqual(QMatrix4x4 tran1, QMatrix4x4 tran2);

    void updateSupportMeshes();

public:

    bool m_isSceneIn;
    QString m_filePath;
    CMeshO* m_oldMesh;

    
    
    
    //QVector<CHMeshShowObjPtr> m_supportMeshes;
    QVector<SupportMeshPtr> m_supportMeshes;
};


//class CHSupportMesh : public CH3DPrintModel
//{
//public:
//    virtual QString getType() { return ""; }
//    virtual void calculate() {}
//    virtual void clone(CHMeshShowObjPtr& obj) {}
//    //virtual void create();
//};


//DEF_PTR(CHSupportCuboid)
//class CHSupportCuboid : public CHSupportMesh
//{
//public:
//    CHSupportCuboid()
//    {
//        m_cuboidPtr = CHCuboidPtr(new CHCuboid);
//    }
//    void clone(CHSupportMeshPtr& obj)
//    {
//        if (!obj)
//        {
//            obj = CHSupportMeshPtr(new CHSupportMesh);
//        }

//        CHMeshShowObjPtr out = std::dynamic_pointer_cast<CHMeshShowObj>(obj);
//        if (!out)
//        {
//            return;
//        }

//        out->setObjectName(m_name);

//        out->m_baseAABB = m_baseAABB;
//        out->m_realAABB = m_realAABB;
//        out->setColor(m_color);
//        out->setLightTest(m_lightTest);
//        out->setCalLight(m_calLight);
//        out->setTransform(m_tran);

//        out->m_vertices = m_vertices;
//        out->m_nors = m_nors;
//        out->m_uvs = m_uvs;
//        out->m_trians = m_trians;
//        out->m_rotCenter[0] = m_rotCenter[0];
//        out->m_rotCenter[1] = m_rotCenter[1];
//        out->m_rotCenter[2] = m_rotCenter[2];
//        out->m_params = m_params;

//        /*   CHShowObjPtr out = std::dynamic_pointer_cast<CHShowObj>(obj);
//           m_cuboidPtr->copy(out);
//           obj = std::dynamic_pointer_cast<CHSupportMesh>(out);*/
//    }
//    bool create(float lenth, float wide, float height) {
//        return m_cuboidPtr->create(lenth, wide, height);
//    }
//    void calculate() override
//    {
//        if (m_vertices.size() <= 0)
//        {
//            return;
//        }
//        m_trians.clear();
//        m_trians.push_back(MyTriangle(0, 1, 2));
//        m_trians.push_back(MyTriangle(0, 2, 3));
//        m_trians.push_back(MyTriangle(4, 5, 6));
//        m_trians.push_back(MyTriangle(4, 6, 7));
//        m_trians.push_back(MyTriangle(8, 9, 10));
//        m_trians.push_back(MyTriangle(8, 10, 11));
//        m_trians.push_back(MyTriangle(12, 13, 14));
//        m_trians.push_back(MyTriangle(12, 14, 15));
//        m_trians.push_back(MyTriangle(16, 17, 18));
//        m_trians.push_back(MyTriangle(16, 18, 19));
//        m_trians.push_back(MyTriangle(20, 21, 22));
//        m_trians.push_back(MyTriangle(20, 22, 23));
//    }
//    QString getType() override
//    {
//        return "CHSupportCuboid";
//    }

//private:
//    CHCuboidPtr m_cuboidPtr;
//};




#endif
