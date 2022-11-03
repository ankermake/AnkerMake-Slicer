#include "SupportCone.h"

SupportCone::SupportCone()
{
    m_rad = 1.0;
    m_height = 1.0;
    m_sliceNum = 32;
}

SupportCone::~SupportCone()
{
}

bool SupportCone::create(float rad, float height, int sliceNum)
{
    m_rad = rad;
    m_height = height;
    m_sliceNum = sliceNum;

    setDirty(true);
    m_vertices.clear();
    m_nors.clear();
    m_uvs.clear();
    m_trians.clear();

    
    m_vertices.push_back(QVector3D(0, 0, 0));
    m_nors.push_back(QVector3D(0, 0, -1));
    float step = 2.0 * CH_PI / m_sliceNum;
    for (int i = 0; i < m_sliceNum; i++)
    {
        float angle = step * i;
        m_vertices.push_back(QVector3D(m_rad * cos(angle), m_rad * sin(angle), 0));
        m_nors.push_back(QVector3D(0, 0, -1));
    }
    for (int i = 0; i < m_sliceNum; i++)
    {
        float angle = step * i;
        m_vertices.push_back(QVector3D(m_rad * cos(angle), m_rad * sin(angle), 0));
        m_nors.push_back(QVector3D(cos(angle), sin(angle), m_rad / m_height).normalized());
    }
    m_vertices.push_back(QVector3D(0, 0, m_height));
    m_nors.push_back(QVector3D(0, 0, 1));

    
    for (int i = 0; i < m_sliceNum; i++)
    {
        int index1 = i + 1;
        int index2 = index1 + 1;
        if (i == m_sliceNum - 1)
        {
            index2 = 1;
        }
        m_trians.push_back(MyTriangle(0, index1, index2));
    }
    int num = (int)m_vertices.size();
    for (int i = 0; i < m_sliceNum; i++)
    {
        int index1 = num - 2 - i;
        int index2 = num - 3 - i;
        if (i == m_sliceNum - 1)
        {
            index2 = num - 2;
        }
        m_trians.push_back(MyTriangle(num - 1, index1, index2));
    }

    return true;
}
