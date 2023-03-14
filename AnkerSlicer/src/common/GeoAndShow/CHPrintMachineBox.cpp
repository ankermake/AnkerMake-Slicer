#include "CHPrintMachineBox.h"
#include "../ml_document/cmesh.h"
#include "../../vcglib/wrap/io_trimesh/import_stl.h"
#include "TransformPack.h"



CHPrintMachineBox::CHPrintMachineBox()
{

}

CHPrintMachineBox::~CHPrintMachineBox()
{
}

void CHPrintMachineBox::setColor(QColor color)
{
    for(int i = 0; i < m_baseShowObjs.size(); i++)
    {
        if(std::dynamic_pointer_cast<CHLineSegment3DShowObj>(m_baseShowObjs[i]) != nullptr)
        {
            m_baseShowObjs[i]->setColor(color);
        }
        else if(std::dynamic_pointer_cast<CHMeshShowObj>(m_baseShowObjs[i]) != nullptr &&
                std::dynamic_pointer_cast<CHMeshShowObj>(m_baseShowObjs[i])->isLogo())
        {
             m_baseShowObjs[i]->setColor(color);
        }
    }
}

void CHPrintMachineBox::create(QString logoPath, float lenth, float wide, float height)
{
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setDelete(true);
        m_baseShowObjs[i]->updateToScene();
    }
    m_baseShowObjs.clear();

    
    /*m_baseAABB.makeEmpty();
    m_baseAABB.add(QVector3D(-lenth / 2, -wide / 2, 0));
    m_baseAABB.add(QVector3D(lenth / 2, wide / 2, height));
    m_realAABB = m_baseAABB;

    CHLineSegment3DShowObjPtr line1(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line2(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line3(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line4(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line5(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line6(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line7(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line8(new CHLineSegment3DShowObj);
    line1->create(QVector3D(lenth / 2, wide / 2, 0), QVector3D(lenth / 2, wide / 2, height));
    line2->create(QVector3D(lenth / 2, -wide / 2, 0), QVector3D(lenth / 2, -wide / 2, height));
    line3->create(QVector3D(-lenth / 2, wide / 2, 0), QVector3D(-lenth / 2, wide / 2, height));
    line4->create(QVector3D(-lenth / 2, -wide / 2, 0), QVector3D(-lenth / 2, -wide / 2, height));
    line5->create(QVector3D(lenth / 2, wide / 2, height), QVector3D(-lenth / 2, wide / 2, height));
    line6->create(QVector3D(-lenth / 2, wide / 2, height), QVector3D(-lenth / 2, -wide / 2, height));
    line7->create(QVector3D(-lenth / 2, -wide / 2, height), QVector3D(lenth / 2, -wide / 2, height));
    line8->create(QVector3D(lenth / 2, -wide / 2, height), QVector3D(lenth / 2, wide / 2, height));  */

    m_baseAABB.makeEmpty();
    m_baseAABB.add(QVector3D(0, 0, 0));
    m_baseAABB.add(QVector3D(lenth, wide, height));
    m_realAABB = m_baseAABB;

    CHLineSegment3DShowObjPtr line1(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line2(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line3(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line4(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line5(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line6(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line7(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line8(new CHLineSegment3DShowObj);
    line1->create(QVector3D(0, 0, 0), QVector3D(0, 0, height));
    line2->create(QVector3D(lenth, 0, 0), QVector3D(lenth, 0, height));
    line3->create(QVector3D(0, wide, 0), QVector3D(0, wide, height));
    line4->create(QVector3D(lenth, wide, 0), QVector3D(lenth, wide, height));
    line5->create(QVector3D(0, 0, height), QVector3D(0, wide, height));
    line6->create(QVector3D(0, wide, height), QVector3D(lenth, wide, height));
    line7->create(QVector3D(lenth, wide, height), QVector3D(lenth, 0, height));
    line8->create(QVector3D(lenth, 0, height), QVector3D(0, 0, height));
    line1->setLineWide(2);
    line2->setLineWide(2);
    line3->setLineWide(2);
    line4->setLineWide(2);
    line5->setLineWide(2);
    line6->setLineWide(2);
    line7->setLineWide(2);
    line8->setLineWide(2);

    
    /*m_platform = CHMeshShowObjPtr(new CHMeshShowObj);
    m_platform->m_vertices.push_back(QVector3D(lenth / 2, wide / 2, 0));
    m_platform->m_vertices.push_back(QVector3D(-lenth / 2, wide / 2, 0));
    m_platform->m_vertices.push_back(QVector3D(-lenth / 2, -wide / 2, 0));
    m_platform->m_vertices.push_back(QVector3D(lenth / 2, -wide / 2, 0));
    m_platform->m_nors.push_back(QVector3D(0, 0, 1));
    m_platform->m_nors.push_back(QVector3D(0, 0, 1));
    m_platform->m_nors.push_back(QVector3D(0, 0, 1));
    m_platform->m_nors.push_back(QVector3D(0, 0, 1));
    m_platform->m_trians.push_back(MyTriangle(0, 1, 3));
    m_platform->m_trians.push_back(MyTriangle(1, 2, 3));*/

    m_baseShowObjs.push_back(line1);
    m_baseShowObjs.push_back(line2);
    m_baseShowObjs.push_back(line3);
    m_baseShowObjs.push_back(line4);
    m_baseShowObjs.push_back(line5);
    m_baseShowObjs.push_back(line6);
    m_baseShowObjs.push_back(line7);
    m_baseShowObjs.push_back(line8);
    //m_baseShowObjs.push_back(m_platform);

   
    int num = 30;
    for (int i = 0; i < num + 1; i++)
    {
        CHLineSegment3DShowObjPtr tline(new CHLineSegment3DShowObj);
        // tline->create(QVector3D(-lenth / 2, wide / num * i - wide / 2, 0), QVector3D(lenth / 2, wide / num * i - wide / 2, 0));
        tline->create(QVector3D(0, wide / num * i, 0), QVector3D(lenth, wide / num * i, 0));
        tline->setLineWide(1);
        m_baseShowObjs.push_back(tline);
    }
    for (int i = 0; i < num + 1; i++)
    {
        CHLineSegment3DShowObjPtr tline(new CHLineSegment3DShowObj);
        //tline->create(QVector3D(lenth / num * i - lenth / 2, -wide / 2, 0), QVector3D(lenth / num * i - lenth / 2, wide / 2, 0));
        tline->create(QVector3D(lenth / num * i, 0, 0), QVector3D(lenth / num * i, wide, 0));
        tline->setLineWide(1);
        m_baseShowObjs.push_back(tline);
    }

    
    QVector3D originPoint = QVector3D(0, 0, 0);
    //float trad = 1.5;
    //float theight = 10;
    CoordinateAxisPtr adjustAxisX;
    CoordinateAxisPtr adjustAxisY;
    CoordinateAxisPtr adjustAxisZ;
    CHPointShowObjPtr adjustOrigin;
    adjustAxisX = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisX->create(originPoint, QVector3D(1, 0, 0), lenth / 5, lenth/ 100.0, lenth/ 20.0);
    adjustAxisX->setColor(QColor(125, 0, 0));
    adjustAxisX->setVisuable(true);
    adjustAxisX->setCalLight(false);
    adjustAxisX->setWidth(5.0);
    //adjustAxisX->setLightTest(false);
    adjustAxisY = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisY->create(originPoint, QVector3D(0, 1, 0), wide / 5,  wide / 100.0, wide / 20.0);
    adjustAxisY->setColor(QColor(0, 125, 0));
    adjustAxisY->setVisuable(true);
    adjustAxisY->setCalLight(false);
    adjustAxisY->setWidth(5.0);
    //adjustAxisY->setLightTest(false);
    adjustAxisZ = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisZ->create(originPoint, QVector3D(0, 0, 1), height / 5, height / 200.0, height / 20.0);
    adjustAxisZ->setColor(QColor(0, 0, 125));
    adjustAxisZ->setVisuable(true);
    adjustAxisZ->setCalLight(false);
    //adjustAxisZ->setLightTest(false);
    adjustAxisZ->setWidth(5.0);
    adjustOrigin = CHPointShowObjPtr(new CHPointShowObj);
    adjustOrigin->create(originPoint);
    adjustOrigin->setColor(QColor(125, 0, 125));
    adjustOrigin->setCalLight(false);
    adjustOrigin->setSize(5);
    m_baseShowObjs.push_back(adjustAxisX);
    m_baseShowObjs.push_back(adjustAxisY);
    m_baseShowObjs.push_back(adjustAxisZ);
    m_baseShowObjs.push_back(adjustOrigin);
    
    CMeshO vcgmesh;
    int mask;
    //QString path("Z:/share/logo.stl");
    /*QString path = QCoreApplication::applicationDirPath();
    path += QString("/models/logo.stl");*/
    vcg::tri::io::ImporterSTL<CMeshO>::Open(vcgmesh, logoPath.toStdString().c_str(), mask);
    //qDebug() << "Open Result: " << openResult;
    vcg::tri::UpdateNormal<CMeshO>::PerFace(vcgmesh); 
    
    vcg::tri::UpdateNormal<CMeshO>::PerVertexFromCurrentFaceNormal(vcgmesh);

    m_logoCMesh = vcgmesh; 

    CHMeshShowObjPtr m_logo(new CHMeshShowObj);
    m_logo->setLogo(true);
    m_logo->setVisuable(true);
    //m_logo->setLightTest(true);
    m_logo->setCalLight(false);
    m_logo->m_showMode = ShowMode::MeshShowFace;
    //qDebug() << "points: " << vcgmesh.vert.size();

    std::vector<QVector3D>& points = m_logo->m_vertices;
    std::vector<QVector3D>& nors = m_logo->m_nors;
    points.resize(vcgmesh.vert.size());
    nors.resize(vcgmesh.vert.size());
    int i = 0;
    for (vcgTriMesh::VertexIterator it = vcgmesh.vert.begin(); it != vcgmesh.vert.end(); it++)
    {
        points[i][0] = it->P().X();
        points[i][1] = it->P().Y();
        points[i][2] = it->P().Z();

        nors[i][0] = it->N().X();
        nors[i][1] = it->N().Y();
        nors[i][2] = it->N().Z();
        i++;
    }

    std::vector<MyTriangle>& faces = m_logo->m_trians;
    faces.resize(vcgmesh.face.size());
    int p = 0;
    for (vcgTriMesh::FaceIterator it = vcgmesh.face.begin(); it != vcgmesh.face.end(); it++)
    {
        faces[p].m_index1 = it->V(0) - vcgmesh.vert.data();
        faces[p].m_index2 = it->V(1) - vcgmesh.vert.data();
        faces[p].m_index3 = it->V(2) - vcgmesh.vert.data();
        p++;
    }

    
    QMatrix4x4 tran1;
    tran1.rotate(-90, QVector3D(1, 0, 0));
    m_logo->transform(tran1);

    
    CHAABB3D aabb;
    for (int i = 0; i < points.size(); i++)
    {
        aabb.add(points[i]);
    }
    float scaleValue = lenth / aabb.getLenX() * 0.5;
    QMatrix4x4 tran2, tran3;
    tran2 = TransformPack::scale(aabb.getCenterPoint(), scaleValue, scaleValue, scaleValue);
    // tran3.translate(QVector3D(0, -wide / 4, 0) - aabb.getCenterPoint());
    tran3.translate(QVector3D(lenth / 2, wide / 2 - wide / 4, 0) - aabb.getCenterPoint());
    m_logo->transform(tran3 * tran2);
    for (int i = 0; i < points.size(); i++)
    {
        points[i][2] = 0.5;
    }
    m_baseShowObjs.push_back(m_logo);
}

void CHPrintMachineBox::create(const CMeshO& logoMesh, float lenth, float wide, float height, const QColor& boxColor)
{
    for (int i = 0; i < m_baseShowObjs.size(); i++)
    {
        m_baseShowObjs[i]->setDelete(true);
        m_baseShowObjs[i]->updateToScene();
    }
    m_baseShowObjs.clear();

    
    m_baseAABB.makeEmpty();
    m_baseAABB.add(QVector3D(0, 0, 0));
    m_baseAABB.add(QVector3D(lenth, wide, height));
    m_realAABB = m_baseAABB;

    CHLineSegment3DShowObjPtr line1(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line2(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line3(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line4(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line5(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line6(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line7(new CHLineSegment3DShowObj);
    CHLineSegment3DShowObjPtr line8(new CHLineSegment3DShowObj);
    line1->create(QVector3D(0, 0, 0), QVector3D(0, 0, height));
    line2->create(QVector3D(lenth, 0, 0), QVector3D(lenth, 0, height));
    line3->create(QVector3D(0, wide, 0), QVector3D(0, wide, height));
    line4->create(QVector3D(lenth, wide, 0), QVector3D(lenth, wide, height));
    line5->create(QVector3D(0, 0, height), QVector3D(0, wide, height));
    line6->create(QVector3D(0, wide, height), QVector3D(lenth, wide, height));
    line7->create(QVector3D(lenth, wide, height), QVector3D(lenth, 0, height));
    line8->create(QVector3D(lenth, 0, height), QVector3D(0, 0, height));
    line1->setColor(boxColor);
    line2->setColor(boxColor);
    line3->setColor(boxColor);
    line4->setColor(boxColor);
    line5->setColor(boxColor);
    line6->setColor(boxColor);
    line7->setColor(boxColor);
    line8->setColor(boxColor);
    line1->setLineWide(2);
    line2->setLineWide(2);
    line3->setLineWide(2);
    line4->setLineWide(2);
    line5->setLineWide(2);
    line6->setLineWide(2);
    line7->setLineWide(2);
    line8->setLineWide(2);

    m_baseShowObjs.push_back(line1);
    m_baseShowObjs.push_back(line2);
    m_baseShowObjs.push_back(line3);
    m_baseShowObjs.push_back(line4);
    m_baseShowObjs.push_back(line5);
    m_baseShowObjs.push_back(line6);
    m_baseShowObjs.push_back(line7);
    m_baseShowObjs.push_back(line8);

    
    int num = 30;
    for (int i = 0; i < num + 1; i++)
    {
        CHLineSegment3DShowObjPtr tline(new CHLineSegment3DShowObj);
        // tline->create(QVector3D(-lenth / 2, wide / num * i - wide / 2, 0), QVector3D(lenth / 2, wide / num * i - wide / 2, 0));
        tline->create(QVector3D(0, wide / num * i, 0), QVector3D(lenth, wide / num * i, 0));
        tline->setColor(boxColor);
        tline->setLineWide(1);
        m_baseShowObjs.push_back(tline);
    }
    for (int i = 0; i < num + 1; i++)
    {
        CHLineSegment3DShowObjPtr tline(new CHLineSegment3DShowObj);
        //tline->create(QVector3D(lenth / num * i - lenth / 2, -wide / 2, 0), QVector3D(lenth / num * i - lenth / 2, wide / 2, 0));
        tline->create(QVector3D(lenth / num * i, 0, 0), QVector3D(lenth / num * i, wide, 0));
        tline->setColor(boxColor);
        tline->setLineWide(1);
        m_baseShowObjs.push_back(tline);
    }

    
    QVector3D originPoint = QVector3D(0, 0, 0);
    float trad = 1.5;
    float theight = 10;
    CoordinateAxisPtr adjustAxisX;
    CoordinateAxisPtr adjustAxisY;
    CoordinateAxisPtr adjustAxisZ;
    CHPointShowObjPtr adjustOrigin;
    adjustAxisX = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisX->create(originPoint, QVector3D(1, 0, 0), lenth / 5, trad, theight);
    adjustAxisX->setColor(QColor(125, 0, 0));
    adjustAxisX->setVisuable(true);
    adjustAxisX->setCalLight(false);
    adjustAxisX->setWidth(5.0);
    //adjustAxisX->setLightTest(false);
    adjustAxisY = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisY->create(originPoint, QVector3D(0, 1, 0), wide / 5, trad, theight);
    adjustAxisY->setColor(QColor(0, 125, 0));
    adjustAxisY->setVisuable(true);
    adjustAxisY->setCalLight(false);
    adjustAxisY->setWidth(5.0);
    //adjustAxisY->setLightTest(false);
    adjustAxisZ = CoordinateAxisPtr(new CoordinateAxis);
    adjustAxisZ->create(originPoint, QVector3D(0, 0, 1), height / 5, trad, theight);
    adjustAxisZ->setColor(QColor(0, 0, 125));
    adjustAxisZ->setVisuable(true);
    adjustAxisZ->setCalLight(false);
    //adjustAxisZ->setLightTest(false);
    adjustAxisZ->setWidth(5.0);
    adjustOrigin = CHPointShowObjPtr(new CHPointShowObj);
    adjustOrigin->create(originPoint);
    adjustOrigin->setColor(QColor(125, 0, 125));
    adjustOrigin->setCalLight(false);
    adjustOrigin->setSize(5);
    m_baseShowObjs.push_back(adjustAxisX);
    m_baseShowObjs.push_back(adjustAxisY);
    m_baseShowObjs.push_back(adjustAxisZ);
    m_baseShowObjs.push_back(adjustOrigin);

    
    CHMeshShowObjPtr m_logo(new CHMeshShowObj);
    m_logo->setLogo(true);
    m_logo->setCalLight(false);
    //m_logo->setLightTest(true);
    m_logo->m_showMode = ShowMode::MeshShowFace;
    std::vector<QVector3D>& points = m_logo->m_vertices;
    std::vector<QVector3D>& nors = m_logo->m_nors;
   //m_logo->setColor(m_logo->getColor());
    points.resize(logoMesh.vert.size());
    nors.resize(logoMesh.vert.size());
    int i = 0;
    
    //PerVertexFromCurrentFaceNormal
    for (auto it = logoMesh.vert.begin(); it != logoMesh.vert.end(); it++)
    {
        points[i][0] = it->P().X();
        points[i][1] = it->P().Y();
        points[i][2] = it->P().Z();

        nors[i][0] = it->N().X();
        nors[i][1] = it->N().Y();
        nors[i][2] = it->N().Z();

       // qDebug() << "nors2: " << i << nors[i];
        i++;
    }

    std::vector<MyTriangle>& faces = m_logo->m_trians;
    faces.resize(logoMesh.face.size());
    int p = 0;
    for (auto it = logoMesh.face.begin(); it != logoMesh.face.end(); it++)
    {
        faces[p].m_index1 = it->V(0) - logoMesh.vert.data();
        faces[p].m_index2 = it->V(1) - logoMesh.vert.data();
        faces[p].m_index3 = it->V(2) - logoMesh.vert.data();

        p++;
    }

    
    QMatrix4x4 tran1;
    tran1.rotate(-90, QVector3D(1, 0, 0));
    m_logo->transform(tran1);

    
    CHAABB3D aabb;
    for (int i = 0; i < points.size(); i++)
    {
        aabb.add(points[i]);
    }
    float scaleValue = lenth / aabb.getLenX() * 0.5;
    QMatrix4x4 tran2, tran3;
    tran2 = TransformPack::scale(aabb.getCenterPoint(), scaleValue, scaleValue, scaleValue);
    //tran3.translate(QVector3D(0, -wide / 4.0, 0) - aabb.getCenterPoint());
    tran3.translate(QVector3D(lenth / 2, wide / 2 - wide / 4, 0) - aabb.getCenterPoint());
    m_logo->transform(tran3 * tran2);
    for (int i = 0; i < points.size(); i++)
    {
        points[i][2] = 0.5;
    }
    m_baseShowObjs.push_back(m_logo);

    qDebug() << "Base Show Size: " << m_baseShowObjs.size();
}

CMeshO CHPrintMachineBox::getLogoCMeshO() const
{
    return m_logoCMesh;
}














