#include "CHTrapezoidBody.h"
#include "TransformPack.h"



CHTrapezoidBody::CHTrapezoidBody()
{
}

CHTrapezoidBody::~CHTrapezoidBody()
{
}

bool CHTrapezoidBody::create(QVector3D topcenter, QVector3D downcenter, float toplenth, float topwide, float downlenth, float downwide)
{
	m_topCenter = topcenter;
	m_downCenter = downcenter;
	m_topLenth = toplenth;
	m_topWide = topwide;
	m_downLenth = downlenth;
	m_downWide = downwide;

	QVector3D pt1(m_topCenter[0] - m_topLenth / 2, m_topCenter[1] - m_topWide / 2, m_topCenter[2]);
	QVector3D pt2(m_topCenter[0] + m_topLenth / 2, m_topCenter[1] - m_topWide / 2, m_topCenter[2]);
	QVector3D pt3(m_topCenter[0] + m_topLenth / 2, m_topCenter[1] + m_topWide / 2, m_topCenter[2]);
	QVector3D pt4(m_topCenter[0] - m_topLenth / 2, m_topCenter[1] + m_topWide / 2, m_topCenter[2]);
	QVector3D pt5(m_downCenter[0] - m_downLenth / 2, m_downCenter[1] - m_downWide / 2, m_downCenter[2]);
	QVector3D pt6(m_downCenter[0] + m_downLenth / 2, m_downCenter[1] - m_downWide / 2, m_downCenter[2]);
	QVector3D pt7(m_downCenter[0] + m_downLenth / 2, m_downCenter[1] + m_downWide / 2, m_downCenter[2]);
	QVector3D pt8(m_downCenter[0] - m_downLenth / 2, m_downCenter[1] + m_downWide / 2, m_downCenter[2]);

	
	//vert
	m_vertices.push_back(pt1);
	m_vertices.push_back(pt2);
	m_vertices.push_back(pt3);
	m_vertices.push_back(pt4);
	for (int i = 0; i < 4; i++)
	{
		m_nors.push_back(QVector3D(0, 0, 1));
	}
	//face
	m_trians.push_back(MyTriangle(0, 1, 2));
	m_trians.push_back(MyTriangle(0, 2, 3));

	
	//vert
	m_vertices.push_back(pt5);
	m_vertices.push_back(pt6);
	m_vertices.push_back(pt7);
	m_vertices.push_back(pt8);
	for (int i = 0; i < 4; i++)
	{
		m_nors.push_back(QVector3D(0, 0, -1));
	}
	//face
	m_trians.push_back(MyTriangle(4, 6, 5));
	m_trians.push_back(MyTriangle(4, 7, 6));

	
	
	m_vertices.push_back(pt5);
	m_vertices.push_back(pt6);
	m_vertices.push_back(pt2);
	m_vertices.push_back(pt1);
	QVector3D tnor = QVector3D::crossProduct(QVector3D(1, 0, 0), pt1 - pt5);
	tnor.normalized();
	for (int i = 0; i < 4; i++)
	{		
		m_nors.push_back(tnor);
	}
	m_trians.push_back(MyTriangle(8, 9, 10));
	m_trians.push_back(MyTriangle(8, 10, 11));

	
	m_vertices.push_back(pt6);
	m_vertices.push_back(pt7);
	m_vertices.push_back(pt3);
	m_vertices.push_back(pt2);
	tnor = QVector3D::crossProduct(QVector3D(0, 1, 0), pt2 - pt6);
	tnor.normalized();
	for (int i = 0; i < 4; i++)
	{
		m_nors.push_back(tnor);
	}
	m_trians.push_back(MyTriangle(12, 13, 14));
	m_trians.push_back(MyTriangle(12, 14, 15));

	
	m_vertices.push_back(pt7);
	m_vertices.push_back(pt8);
	m_vertices.push_back(pt4);
	m_vertices.push_back(pt3);
	tnor = QVector3D::crossProduct(QVector3D(-1, 0, 0), pt3 - pt7);
	tnor.normalized();
	for (int i = 0; i < 4; i++)
	{
		m_nors.push_back(tnor);
	}
	m_trians.push_back(MyTriangle(16, 17, 18));
	m_trians.push_back(MyTriangle(16, 18, 19));

	
	m_vertices.push_back(pt8);
	m_vertices.push_back(pt5);
	m_vertices.push_back(pt1);
	m_vertices.push_back(pt4);
	tnor = QVector3D::crossProduct(QVector3D(0, -1, 0), pt4 - pt8);
	tnor.normalized();
	for (int i = 0; i < 4; i++)
	{
		m_nors.push_back(tnor);
	}
	m_trians.push_back(MyTriangle(20, 21, 22));
	m_trians.push_back(MyTriangle(20, 22, 23));

	return true;
}



















