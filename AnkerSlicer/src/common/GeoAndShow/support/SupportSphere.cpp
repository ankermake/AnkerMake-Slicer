#include "SupportSphere.h"

//SupportSphere::SupportSphere()
//{

//}

SupportSphere::SupportSphere()
{
    m_rad = 1.0;

}

SupportSphere::~SupportSphere()
{
}

bool SupportSphere::create(QVector3D center, float rad, int hDivide, int vDivide)
{
    m_center = center;
    m_rad = rad;

    QMatrix4x4 tran1, tran2;
    tran1.scale(m_rad);
    tran2.translate(m_center);
    QMatrix4x4 sumTran = tran2 * tran1;

    
    m_nors.push_back(QVector3D(0, 0, 1));
    m_vertices.push_back(TransformPack::pRot(sumTran, QVector3D(0, 0, 1)));
    float angleStep1 = CH_PI / (vDivide + 1);
    float angleStep2 = 2.0 * CH_PI / hDivide;
    for (int i = 0; i < vDivide; i++)
    {
        float tAngle1 = angleStep1 * (i + 1);
        float tz = cos(tAngle1);
        float txy = sin(tAngle1);
        for (int j = 0; j < hDivide; j++)
        {
            float tAngle2 = angleStep2 * j;
            QVector3D tpt(txy * cos(tAngle2), txy * sin(tAngle2), tz);
            m_nors.push_back(tpt);
            m_vertices.push_back(TransformPack::pRot(sumTran, tpt));
        }
    }
    m_nors.push_back(QVector3D(0, 0, -1));
    m_vertices.push_back(TransformPack::pRot(sumTran, QVector3D(0, 0, -1)));

    
    for (int j = 0; j < hDivide; j++)
    {
        MyTriangle ttrian;
        ttrian.m_index1 = 0;
        ttrian.m_index2 = j + 1;
        ttrian.m_index3 = j + 2;
        if (j == hDivide - 1)
        {
            ttrian.m_index3 = 1;
        }
        m_trians.push_back(ttrian);
    }
    for (int i = 1; i < vDivide; i++)
    {
        for (int j = 0; j < hDivide; j++)
        {
            int tindex = i * hDivide + 1 + j;
            if (j == hDivide - 1)
            {
                m_trians.push_back(MyTriangle(tindex, tindex + 1 - hDivide, tindex - hDivide));
                m_trians.push_back(MyTriangle(tindex + 1 - 2 * hDivide, tindex - hDivide, tindex + 1 - hDivide));
            }
            else
            {
                m_trians.push_back(MyTriangle(tindex, tindex + 1, tindex - hDivide));
                m_trians.push_back(MyTriangle(tindex - hDivide + 1, tindex - hDivide, tindex + 1));
            }
        }
    }
    int num = (int)m_vertices.size();
    for (int j = 0; j < hDivide; j++)
    {
        MyTriangle ttrian;
        ttrian.m_index1 = num - 1;
        ttrian.m_index2 = ttrian.m_index1 - (j + 1);
        ttrian.m_index3 = ttrian.m_index1 - (j + 2);
        if (j == hDivide - 1)
        {
            ttrian.m_index3 = ttrian.m_index1 - 1;
        }
        m_trians.push_back(ttrian);
    }

    return true;
}



