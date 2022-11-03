#include "CHMeshShowObj.h"
#include "TransformPack.h"
#include "CHScene.h"
#include "CHBaseAlg.h"



CHMeshShowObj::CHMeshShowObj()
{
    m_showMode = MeshShowFace;
    m_priority = Mesh;
    m_logo = false;
}

CHMeshShowObj::~CHMeshShowObj()
{
}

void CHMeshShowObj::setLogo(bool logo)
{
    m_logo = logo;
}

bool CHMeshShowObj::isLogo() const
{
    return m_logo;
}
void CHMeshShowObj::transform(const QMatrix4x4& matrix)
{
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i] = TransformPack::pRot(matrix, m_vertices[i]);
    }

    QMatrix3x3 norMatrix = matrix.normalMatrix();
    float* data = norMatrix.data();
    QMatrix4x4 tttt(data[0], data[3], data[6], 0,
        data[1], data[4], data[7], 0,
        data[2], data[5], data[8], 0,
        0, 0, 0, 1);
    for (int i = 0; i < m_nors.size(); i++)
    {
        m_nors[i] = tttt * QVector4D(m_nors[i], 1.0).toVector3D();
    }
}

bool CHMeshShowObj::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
    QVector3D np, fp;
    curScene->getCurNearFarPoint(pixelX, pixelY, np, fp);
    CHLineSegment3DShowObjPtr lineSegment(new CHLineSegment3DShowObj);
    lineSegment->create(np, fp);

    std::vector<CHPointOnMesh> allpoints;
    if (CHBaseAlg::instance()->LineSegmentMeshIntersect(std::dynamic_pointer_cast<CHMeshShowObj>(shared_from_this()), lineSegment, allpoints))
    {
        double minDis = 99999999999;
        QVector3D sssstt = TransformPack::pRot(lineSegment->getTransform(), lineSegment->m_st);
        for (int i = 0; i < (int)allpoints.size(); i++)
        {
            QVector3D fzCPOnBody = allpoints[i].m_pt;
            double tempDis = fzCPOnBody.distanceToPoint(sssstt);
            if (tempDis < minDis)
            {
                minDis = tempDis;
                result.m_ptOnObj = allpoints[i].m_pt;
                result.m_pickObj = shared_from_this();
                result.m_pointOnMesh = allpoints[i];

                
                float x, y, z;
                curScene->worldToPixel(result.m_ptOnObj, x, y, z);
                result.m_deep = z;
            }
        }
        return true;
    }
    return false;
}

bool CHMeshShowObj::pickAABB(int pixelX, int pixelY, int)
{
    QVector3D np, fp; curScene->getCurNearFarPoint(pixelX, pixelY, np, fp);
    CHLine3DPtr lineSegment(new CHLine3D); lineSegment->create(np, fp);
    return 0 < CHBaseAlg::instance()->existIntersectionBetweenLineAndBox(lineSegment, getBaseAABB(), getTransform());
}

void CHMeshShowObj::updateToScene()
{
    if (curScene)
    {
        curScene->updateShowObj(shared_from_this());
    }
}

void CHMeshShowObj::resetTransform()
{
    setTransform(m_initTransform);
    //m_realAABB = m_baseAABB;

    //?????
    m_rotCenter = getBaseAABB().getCenterPoint();
    m_params.resize(9);
    for (int i = 0; i < 9; i++)
    {
        m_params[i] = m_initParams[i];
    }
}
void CHMeshShowObj::resetMove()
{
    for (int i = 6; i < 9; i++)
    {
        m_params[i] = m_initParams[i];
    }
}
void CHMeshShowObj::resetRot()
{
    for (int i = 3; i < 6; i++)
    {
        m_params[i] = m_initParams[i];
    }
}
void CHMeshShowObj::resetZoom()
{
    for (int i = 0; i < 3; i++)
    {
        m_params[i] = m_initParams[i];
    }
}
void CHMeshShowObj::resetMirr()
{

}

CHAABB3D CHMeshShowObj::calBaseAABB()
{
    m_baseAABB.makeEmpty();
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_baseAABB.add(m_vertices[i]);
    }
    return m_baseAABB;
}

CHAABB3D CHMeshShowObj::calRealAABB()
{
    m_realAABB.makeEmpty();
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_realAABB.add(m_tran * m_vertices[i]);
    }
    return m_realAABB;
}

void CHMeshShowObj::copy(CHShowObjPtr& obj)
{
    if (!obj)
    {
        obj = CHMeshShowObjPtr(new CHMeshShowObj);
    }

    CHMeshShowObjPtr out = std::dynamic_pointer_cast<CHMeshShowObj>(obj);
    if (!out)
    {
        return;
    }
    out->setObjectName(m_name);
    out->m_baseAABB = m_baseAABB;
    out->m_realAABB = m_realAABB;
    out->setColor(m_color);
    out->setLightTest(m_lightTest);
    out->setCalLight(m_calLight);
    out->setTransform(m_tran);

    out->m_vertices = m_vertices;
    out->m_nors = m_nors;
    out->m_uvs = m_uvs;
    out->m_trians = m_trians;
    out->m_rotCenter[0] = m_rotCenter[0];
    out->m_rotCenter[1] = m_rotCenter[1];
    out->m_rotCenter[2] = m_rotCenter[2];
    out->m_params = m_params;
}

void CHMeshShowObj::cmeshoToMeshShowObj(CMeshO& mesh)
{
    m_baseAABB = CHAABB3D(mesh.bbox.min.X(), mesh.bbox.min.Y(), mesh.bbox.min.Z(),
        mesh.bbox.max.X(), mesh.bbox.max.Y(), mesh.bbox.max.Z());
    m_realAABB = m_baseAABB;

    if (true)
    {
        std::vector<QVector3D>& points = m_vertices;
        std::vector<QVector3D>& nors = m_nors;
        std::vector<MyTriangle>& faces = m_trians;
        points.resize(mesh.face.size() * 3);
        nors.resize(mesh.face.size() * 3);
        faces.resize(mesh.face.size());
        int p = 0;
        vcg::tri::UpdateNormal<CMeshO>::PerFace(mesh); 
        float maxZ = -9999;
        float maxX = -9999;
        float minZ = 9999;
        float minX = 9999;
        int reverse = 1; 
        std::vector<CFaceO>::iterator findUpFaceIt = mesh.face.end();
        std::vector<CFaceO>::iterator findBottomFaceIt = mesh.face.end();
        std::vector<CFaceO>::iterator findRightFaceIt = mesh.face.end(); 
        std::vector<CFaceO>::iterator findLeftFaceIt = mesh.face.end(); 
        for(auto it = mesh.face.begin(); it != mesh.face.end(); it++)
        {
            if(it->V(0)->P().Z() > maxZ)
            {
               maxZ = it->V(0)->P().Z();
               findUpFaceIt  = it;
            }

            if(it->V(0)->P().Z() < minZ)
            {
                minZ = it->V(0)->P().Z();
                findBottomFaceIt = it;
            }

            if(it->V(0)->P().X() > maxX)
            {
               maxX = it->V(0)->P().X();
               findRightFaceIt = it;
            }

            if(it->V(0)->P().X() < minX)
            {
                minX = it->V(0)->P().X();
                findLeftFaceIt = it;
            }
        }

        /*








            if(dir > 0)
            {

            }
            else
            {

            }
        */
        float dir1 = 0.0;
        if(findUpFaceIt != mesh.face.end())
        {
            QVector3D upVertex0(findUpFaceIt->V(0)->P().X(), findUpFaceIt->V(0)->P().Y(), findUpFaceIt->V(0)->P().Z());
            QVector3D upVertex1(findUpFaceIt->V(1)->P().X(), findUpFaceIt->V(1)->P().Y(), findUpFaceIt->V(1)->P().Z());
            QVector3D upVertex2(findUpFaceIt->V(2)->P().X(), findUpFaceIt->V(2)->P().Y(), findUpFaceIt->V(2)->P().Z());
            QVector3D refP1(findUpFaceIt->V(0)->P().X(), findUpFaceIt->V(0)->P().Y(), 10000); 
            QVector3D upN = QVector3D::crossProduct(upVertex1 - upVertex0, upVertex2 - upVertex0).normalized();
            float upD =  QVector3D::dotProduct(-upN, upVertex0);
            dir1 = QVector3D::dotProduct(upN, refP1) + upD;
        }

        float dir2 = 0.0;
        if(findBottomFaceIt != mesh.face.end())
        {
            QVector3D bottomVertex0(findBottomFaceIt->V(0)->P().X(), findBottomFaceIt->V(0)->P().Y(), findBottomFaceIt->V(0)->P().Z());
            QVector3D bottomVertex1(findBottomFaceIt->V(1)->P().X(), findBottomFaceIt->V(1)->P().Y(), findBottomFaceIt->V(1)->P().Z());
            QVector3D bottomVertex2(findBottomFaceIt->V(2)->P().X(), findBottomFaceIt->V(2)->P().Y(), findBottomFaceIt->V(2)->P().Z());
            QVector3D refP2(findBottomFaceIt->V(0)->P().X(), findBottomFaceIt->V(0)->P().Y(), -10000); 
            QVector3D bottomN = QVector3D::crossProduct(bottomVertex1 - bottomVertex0, bottomVertex2 - bottomVertex0).normalized();
            float bottomD =  QVector3D::dotProduct(-bottomN, bottomVertex0);
            dir2 = QVector3D::dotProduct(bottomN, refP2) + bottomD;
        }

        if(dir1 < 0 && dir2 < 0)
        {
            
            reverse = -reverse;
        }
        else  
        {
            float dir3 = 0.0;
            if(findRightFaceIt != mesh.face.end())
            {
                QVector3D rightVertex0(findRightFaceIt->V(0)->P().X(), findRightFaceIt->V(0)->P().Y(), findRightFaceIt->V(0)->P().Z());
                QVector3D rightVertex1(findRightFaceIt->V(1)->P().X(), findRightFaceIt->V(1)->P().Y(), findRightFaceIt->V(1)->P().Z());
                QVector3D rightVertex2(findRightFaceIt->V(2)->P().X(), findRightFaceIt->V(2)->P().Y(), findRightFaceIt->V(2)->P().Z());
                QVector3D refP3(10000, findRightFaceIt->V(0)->P().Y(), findRightFaceIt->V(0)->P().Z()); 
                QVector3D rightN = QVector3D::crossProduct(rightVertex1 - rightVertex0, rightVertex2 - rightVertex0).normalized();
                float rightD =  QVector3D::dotProduct(-rightN, rightVertex0);
                dir3 = QVector3D::dotProduct(rightN, refP3) + rightD;
            }

            float dir4 = 0.0;
            if(findLeftFaceIt != mesh.face.end())
            {
                QVector3D leftVertex0(findLeftFaceIt->V(0)->P().X(), findLeftFaceIt->V(0)->P().Y(), findLeftFaceIt->V(0)->P().Z());
                QVector3D leftVertex1(findLeftFaceIt->V(1)->P().X(), findLeftFaceIt->V(1)->P().Y(), findLeftFaceIt->V(1)->P().Z());
                QVector3D leftVertex2(findLeftFaceIt->V(2)->P().X(), findLeftFaceIt->V(2)->P().Y(), findLeftFaceIt->V(2)->P().Z());
                QVector3D refP4(-10000, findLeftFaceIt->V(0)->P().Y(), findLeftFaceIt->V(0)->P().Z()); 
                QVector3D leftN = QVector3D::crossProduct(leftVertex1 - leftVertex0, leftVertex2 - leftVertex0).normalized();
                float leftD =  QVector3D::dotProduct(-leftN, leftVertex0);
                dir4 = QVector3D::dotProduct(leftN, refP4) + leftD;
            }

            if(dir3 < 0/* && dir4 < 0*/)
            {
                reverse = -reverse;
            }
        }
        

        for (auto it = mesh.face.begin(); it != mesh.face.end(); it++)
        {
            points[3 * p][0] = it->V(0)->P().X();
            points[3 * p][1] = it->V(0)->P().Y();
            points[3 * p][2] = it->V(0)->P().Z();

            points[3 * p + 1][0] = it->V(1)->P().X();
            points[3 * p + 1][1] = it->V(1)->P().Y();
            points[3 * p + 1][2] = it->V(1)->P().Z();

            points[3 * p + 2][0] = it->V(2)->P().X();
            points[3 * p + 2][1] = it->V(2)->P().Y();
            points[3 * p + 2][2] = it->V(2)->P().Z();

            QVector3D vn(it->N().X(), it->N().Y(), it->N().Z());
            vn *= reverse;
            nors[3 * p][0] = vn.x();
            nors[3 * p][1] = vn.y();
            nors[3 * p][2] = vn.z();
            nors[3 * p + 1] = nors[3 * p];
            nors[3 * p + 2] = nors[3 * p];

            faces[p].m_index1 = 3 * p;
            faces[p].m_index2 = 3 * p + 1;
            faces[p].m_index3 = 3 * p + 2;
            p++;
        }
    }
    else
    {
        
        vcg::tri::UpdateNormal<CMeshO>::PerVertexFromCurrentFaceNormal(mesh);
        std::vector<QVector3D>& points = m_vertices;
        std::vector<QVector3D>& nors = m_nors;
        points.resize(mesh.vert.size());
        nors.resize(mesh.vert.size());
        int i = 0;
        for (auto it = mesh.vert.begin(); it != mesh.vert.end(); it++)
        {
            points[i][0] = it->P().X();
            points[i][1] = it->P().Y();
            points[i][2] = it->P().Z();

            nors[i][0] = it->N().X();
            nors[i][1] = it->N().Y();
            nors[i][2] = it->N().Z();

            i++;
        }

        std::vector<MyTriangle>& faces = m_trians;
        faces.resize(mesh.face.size());
        int p = 0;
        for (auto it = mesh.face.begin(); it != mesh.face.end(); it++)
        {
            faces[p].m_index1 = it->V(0) - mesh.vert.data();
            faces[p].m_index2 = it->V(1) - mesh.vert.data();
            faces[p].m_index3 = it->V(2) - mesh.vert.data();
            p++;
        }
    }
}


QString CHMeshShowObj::serialize(QString path)
{

    return "";
}


CHMeshShowObjPtr CHMeshShowObj::deserialize(QString xmlContent)
{
    return nullptr;
}














