#include "SupportCylinder.h"

//SupportCylinder::SupportCylinder()
//{

//}


SupportCylinder::SupportCylinder()
{
}

SupportCylinder::~SupportCylinder()
{
}

bool SupportCylinder::create(float topRad, float downRad, float height, int sliceNum)
{
    m_topRad = topRad;
    m_downRad = downRad;
    m_height = height;

    float stepAngle = CH_PI * 2.0 / sliceNum;

    
    //vert
    m_vertices.push_back(QVector3D(0, 0, m_height));
    m_nors.push_back(QVector3D(0, 0, 1));
    for (int i = 0; i < sliceNum; i++)
    {
        m_vertices.push_back(QVector3D(m_topRad * cos(stepAngle * i), m_topRad * sin(stepAngle * i), m_height));
        m_nors.push_back(QVector3D(0, 0, 1));
    }
    //face
    int startIndex = 1;
    int endIndex = startIndex + sliceNum;
    for (int i = startIndex; i < endIndex; i++)
    {
        int nextIndex = i + 1;
        if (i == endIndex - 1)
        {
            nextIndex = startIndex;
        }
        m_trians.push_back(MyTriangle(0, i, nextIndex));
    }

    
    //vert
    m_vertices.push_back(QVector3D(0, 0, 0));
    m_nors.push_back(QVector3D(0, 0, -1));
    for (int i = 0; i < sliceNum; i++)
    {
        m_vertices.push_back(QVector3D(m_downRad * cos(stepAngle * i), m_downRad * sin(stepAngle * i), 0));
        m_nors.push_back(QVector3D(0, 0, -1));
    }
    //face
    startIndex = sliceNum + 2;
    endIndex = startIndex + sliceNum;
    for (int i = startIndex; i < endIndex; i++)
    {
        int nextIndex = i + 1;
        if (i == endIndex - 1)
        {
            nextIndex = startIndex;
        }
        m_trians.push_back(MyTriangle(sliceNum + 1, i, nextIndex));
    }

    
    //vert
    for (int i = 0; i < sliceNum; i++)
    {
        QVector3D basePt(cos(stepAngle * i), sin(stepAngle * i), 0);
        QVector3D pt = basePt * m_topRad;
        m_vertices.push_back(QVector3D(pt[0], pt[1], m_height));
        m_nors.push_back(basePt);

        m_vertices.push_back(basePt * m_downRad);
        m_nors.push_back(basePt);
    }
    //face
    startIndex = 2 * (sliceNum + 1);
    endIndex = startIndex + 2 * sliceNum;
    for (int i = startIndex; i < endIndex; i += 2)
    {
        int nnextIndex = i + 2;
        if (i == endIndex - 2)
        {
            nnextIndex = startIndex;
        }
        m_trians.push_back(MyTriangle(i, i + 1, nnextIndex));
        m_trians.push_back(MyTriangle(nnextIndex, i + 1, nnextIndex + 1));
    }


    return true;
}

