#include "SupportTetrahedron.h"

//SupportTetrahedron::SupportTetrahedron()
//{

//}



SupportTetrahedron::SupportTetrahedron()
{
}

SupportTetrahedron::~SupportTetrahedron()
{
}

bool SupportTetrahedron::create(QVector3D pt1, QVector3D pt2, QVector3D pt3, QVector3D pt4)
{
    m_pt1 = pt1;
    m_pt2 = pt2;
    m_pt3 = pt3;
    m_pt4 = pt4;
    
    if (QVector3D::dotProduct(pt4 - pt1, QVector3D::crossProduct(pt2 - pt1, pt3 - pt1)) > 0)
    {
        std::swap(m_pt2, m_pt3);
    }

    m_vertices.push_back(m_pt1);
    m_vertices.push_back(m_pt2);
    m_vertices.push_back(m_pt3);
    int num = (int)m_vertices.size();
    QVector3D tnor = QVector3D::crossProduct(m_vertices[num - 2] - m_vertices[num - 3], m_vertices[num - 1] - m_vertices[num - 3]);
    tnor.normalized();
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);

    m_vertices.push_back(m_pt1);
    m_vertices.push_back(m_pt3);
    m_vertices.push_back(m_pt4);
    num = (int)m_vertices.size();
    tnor = QVector3D::crossProduct(m_vertices[num - 2] - m_vertices[num - 3], m_vertices[num - 1] - m_vertices[num - 3]);
    tnor.normalized();
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);

    m_vertices.push_back(m_pt3);
    m_vertices.push_back(m_pt2);
    m_vertices.push_back(m_pt4);
    num = (int)m_vertices.size();
    tnor = QVector3D::crossProduct(m_vertices[num - 2] - m_vertices[num - 3], m_vertices[num - 1] - m_vertices[num - 3]);
    tnor.normalized();
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);

    m_vertices.push_back(m_pt2);
    m_vertices.push_back(m_pt1);
    m_vertices.push_back(m_pt4);
    num = (int)m_vertices.size();
    tnor = QVector3D::crossProduct(m_vertices[num - 2] - m_vertices[num - 3], m_vertices[num - 1] - m_vertices[num - 3]);
    tnor.normalized();
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);
    m_nors.push_back(tnor);

    m_trians.push_back(MyTriangle(0, 1, 2));
    m_trians.push_back(MyTriangle(3, 4, 5));
    m_trians.push_back(MyTriangle(6, 7, 8));
    m_trians.push_back(MyTriangle(9, 10, 11));

    return true;
}


