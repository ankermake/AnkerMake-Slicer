/*



*/


#ifndef CHNFPAlg_H
#define CHNFPAlg_H

#include "CHAABB3D.h"
#include "CHLine3D.h"
#include "CHLineSegment3DShowObj.h"
#include "../ml_document/cmesh.h"
#include "CHMeshShowObj.h"
#include "AkTransformMath.h"

//inline QVector3D operator*(const QMatrix3x3& matrix, const QVector3D& vector) {
//	float x, y, z;
//	x = vector.x() * matrix(0, 0) + vector.y() * matrix(1, 0) + vector.z() * matrix(2, 0);
//	y = vector.x() * matrix(0, 1) + vector.y() * matrix(1, 1) + vector.z() * matrix(2, 1);
//	z = vector.x() * matrix(0, 2) + vector.y() * matrix(1, 2) + vector.z() * matrix(2, 2);
//	return QVector3D(x, y, z);
//}

struct COMMONLIB_EXPORT LineLineExtremeResult
{
    bool m_parallel;
	double m_u1;
	double m_u2;
	QVector3D m_pt1;
	QVector3D m_pt2;
};

class COMMONLIB_EXPORT CHBaseAlg
{
public:
	CHBaseAlg();
	~CHBaseAlg();

	static CHBaseAlg* instance(); 

public:

/*------------------------------------------------------IO------------------------------------------------------*/

	bool writeMeshesToStlFile(CHMeshShowObjPtr meshPtr, const QString& path, bool binary = true);

	
	
	
	bool writeMeshesToStlFile(const std::vector<CHMeshShowObjPtr>& meshes, const QString& path, bool binary = true);





/*------------------------------------------------------Alg------------------------------------------------------*/

    
	void extremeBetweenLineAndLine(const CHLine3DPtr& line1, const CHLine3DPtr& line2, LineLineExtremeResult& res);

    
	void extremeBetweenLineSegmentAndLine(const CHLineSegment3DShowObjPtr& lineSegment1, const CHLine3DPtr& line2, LineLineExtremeResult& res);

	QMatrix4x4 calTransformFromParams(const QVector3D& rotCenter, const std::vector<float>& params);
	void calEulerAnglesFromRotMatrix(const QMatrix4x4& rotTran, double& angleX, double& angleY, double& angleZ);//????????????????

    
	bool LineMeshIntersect(CMeshO& mesh, const CHLine3DPtr& line, std::vector<CHPointOnMesh>& intersections);

    
	bool LineSegmentMeshIntersect(CMeshO& mesh, const CHLineSegment3DShowObjPtr& lineSegment, std::vector<CHPointOnMesh>& intersections);


    
	bool LineMeshIntersect(const CHMeshShowObjPtr& mesh, const CHLine3DPtr& line, std::vector<CHPointOnMesh>& intersections);

    
	bool LineSegmentMeshIntersect(const CHMeshShowObjPtr& mesh, const CHLineSegment3DShowObjPtr& lineSegment, std::vector<CHPointOnMesh>& intersections);


    
	bool existIntersectionBetweenLineAndTriangle(const CHLine3DPtr& line, const QVector3D& pt1,
		const QVector3D& pt2, const QVector3D& pt3, CHPointOnMesh& outpt);

    
    bool existIntersectionBetweenLineAndRectangle(
            const CHLine3DPtr& line,
            const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& pt4);

    int existIntersectionBetweenLineAndBox(const CHLine3DPtr& line, const QVector3D& boxMin, const QVector3D& boxMax);

    int existIntersectionBetweenLineAndBox(const CHLine3DPtr& line, const CHAABB3D& box, const QMatrix4x4& boxMat44 = QMatrix4x4());
};

#endif
