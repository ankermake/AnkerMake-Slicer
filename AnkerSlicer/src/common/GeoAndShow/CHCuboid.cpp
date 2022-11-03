#include "CHCuboid.h"


CHCuboid::CHCuboid()
{

}

CHCuboid::~CHCuboid()
{
}

bool CHCuboid::create(float lenth, float wide, float height)
{
    m_lenth = lenth;
    m_wide = wide;
    m_height = height;

    QVector3D pt1(-m_lenth / 2, -m_wide / 2, height);
    QVector3D pt2(m_lenth / 2, -m_wide / 2, height);
    QVector3D pt3(m_lenth / 2, m_wide / 2, height);
    QVector3D pt4(-m_lenth / 2, m_wide / 2, height);
    QVector3D pt5(-m_lenth / 2, -m_wide / 2, 0);
    QVector3D pt6(m_lenth / 2, -m_wide / 2, 0);
    QVector3D pt7(m_lenth / 2, m_wide / 2, 0);
    QVector3D pt8(-m_lenth / 2, m_wide / 2, 0);

    
    m_vertices.push_back(pt5);
    m_vertices.push_back(pt6);
    m_vertices.push_back(pt2);
    m_vertices.push_back(pt1);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(0, -1, 0));
    }
    m_trians.push_back(MyTriangle(0, 1, 2));
    m_trians.push_back(MyTriangle(0, 2, 3));

    m_vertices.push_back(pt7);
    m_vertices.push_back(pt8);
    m_vertices.push_back(pt4);
    m_vertices.push_back(pt3);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(0, 1, 0));
    }
    m_trians.push_back(MyTriangle(4, 5, 6));
    m_trians.push_back(MyTriangle(4, 6, 7));

    
    m_vertices.push_back(pt8);
    m_vertices.push_back(pt5);
    m_vertices.push_back(pt1);
    m_vertices.push_back(pt4);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(-1, 0, 0));
    }
    m_trians.push_back(MyTriangle(8, 9, 10));
    m_trians.push_back(MyTriangle(8, 10, 11));

    m_vertices.push_back(pt6);
    m_vertices.push_back(pt7);
    m_vertices.push_back(pt3);
    m_vertices.push_back(pt2);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(1, 0, 0));
    }
    m_trians.push_back(MyTriangle(12, 13, 14));
    m_trians.push_back(MyTriangle(12, 14, 15));

    
    m_vertices.push_back(pt1);
    m_vertices.push_back(pt2);
    m_vertices.push_back(pt3);
    m_vertices.push_back(pt4);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(0, 0, 1));
    }
    m_trians.push_back(MyTriangle(16, 17, 18));
    m_trians.push_back(MyTriangle(16, 18, 19));

    m_vertices.push_back(pt8);
    m_vertices.push_back(pt7);
    m_vertices.push_back(pt6);
    m_vertices.push_back(pt5);
    for (int i = 0; i < 4; i++)
    {
        m_nors.push_back(QVector3D(0, 0, -1));
    }
    m_trians.push_back(MyTriangle(20, 21, 22));
    m_trians.push_back(MyTriangle(20, 22, 23));

    return true;
}

void CHCuboid::setLWH(float lenth, float wide, float height)
{
    m_lenth = lenth;
    m_wide = wide;
    m_height = height;
}

QMatrix4x4 CHCuboid::getLocalTransform()  const
{
    return m_localTransform;
}

void CHCuboid::loadProjectInitSupportTri()
{
    if (m_vertices.size() <= 0)
    {
        return;
    }
    m_trians.push_back(MyTriangle(0, 1, 2));
    m_trians.push_back(MyTriangle(0, 2, 3));
    m_trians.push_back(MyTriangle(4, 5, 6));
    m_trians.push_back(MyTriangle(4, 6, 7));
    m_trians.push_back(MyTriangle(8, 9, 10));
    m_trians.push_back(MyTriangle(8, 10, 11));
    m_trians.push_back(MyTriangle(12, 13, 14));
    m_trians.push_back(MyTriangle(12, 14, 15));
    m_trians.push_back(MyTriangle(16, 17, 18));
    m_trians.push_back(MyTriangle(16, 18, 19));
    m_trians.push_back(MyTriangle(20, 21, 22));
    m_trians.push_back(MyTriangle(20, 22, 23));
}

void CHCuboid::cmeshoToMyModel(const CMeshO& mesh)
{
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


















