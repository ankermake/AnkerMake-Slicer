/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005 - 2020                                          \/)\/   *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#include "cmesh.h"
#include "QMatrix4x4"
#include "GeoAndShow/CHLineSegment3DShowObj.h"
#include "GeoAndShow/CHBaseAlg.h"
#include "GeoAndShow/TransformPack.h"

CMeshO::CMeshO() :
	vcgTriMesh(),
	sfn(0), svn(0), pvn(0), pfn(0), Tr(Matrix44m::Identity())
{
	
	/*for (int i = 0; i < 3; i++)
	{
		m_rotCenter[i] = 0;
	}
	m_params.resize(9);
	for (int i = 0; i < 3; i++)
	{
		m_params[i] = 1;
	}
	for (int i = 3; i < 9; i++)
	{
		m_params[i] = 0;
	}*/
}

CMeshO::CMeshO(const CMeshO& oth) :
	vcgTriMesh(), sfn(oth.sfn), svn(oth.svn), 
	pvn(oth.pvn), pfn(oth.pfn), Tr(oth.Tr)
{
	enableComponentsFromOtherMesh(oth);
    vcg::tri::Append<vcgTriMesh, vcgTriMesh>::MeshAppendConst(*this, oth);
	textures = oth.textures;
	normalmaps = oth.normalmaps;
	imark = oth.imark;

	for (int i = 0; i < 3; i++)
	{
		m_rotCenter[i] = oth.m_rotCenter[i];
	}
	m_params = oth.m_params;
}

CMeshO::CMeshO(CMeshO&& oth)
	: CMeshO()
{
	swap(*this, oth);
}

CMeshO::~CMeshO()
{
	//no need to call base class destructor. It is called automatically
}

CMeshO& CMeshO::operator=(CMeshO oth)
{
	swap(*this, oth);
	return *this;
}

Box3m CMeshO::trBB() const
{
	Box3m bb;
	bb.Add(Tr,bbox);
	return bb;
}

bool CMeshO::pick(CHLineSegment3DShowObjPtr lineSegment, CHPointOnMesh& result)
{	
	std::vector<CHPointOnMesh> allpoints;
	if (CHBaseAlg::instance()->LineSegmentMeshIntersect(*this, lineSegment, allpoints))
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
				result = allpoints[i];
			}
		}
		return true;
	}
	return false;
}

Box3m CMeshO::calCurrentAABB()
{
	Box3m bb;
	float vv = 999999999;
	bb.min = vcg::Point3f(vv, vv, vv);
	bb.max = vcg::Point3f(-vv, -vv, -vv);
	for (VertexIterator vi = vert.begin(); vi != vert.end(); ++vi)
	{
		bb.Add(Tr * Point3m(vi->P().V(0), vi->P().V(1), vi->P().V(2)));
	}
	return bb;
}

void CMeshO::adjustToSuitableSize(Box3m workPlat, double ratio)
{
	std::vector<float> objLenths;
	objLenths.push_back(bbox.DimX());
	objLenths.push_back(bbox.DimY());
	objLenths.push_back(bbox.DimZ());
	std::sort(objLenths.begin(), objLenths.end());

	std::vector<float> platLenths;
	platLenths.push_back(workPlat.DimX());
	platLenths.push_back(workPlat.DimY());
	platLenths.push_back(workPlat.DimZ());
	std::sort(platLenths.begin(), platLenths.end());

	float scale = std::min(std::min(platLenths[0] / objLenths[0], platLenths[1] / objLenths[1]),
		platLenths[2] / objLenths[2]) * ratio;

	
	vcg::Point3f pt = bbox.Center();
	m_rotCenter[0] = pt.X();
	m_rotCenter[1] = pt.Y();
	m_rotCenter[2] = pt.Z();
	m_params.resize(9);
	for (int i = 0; i < 3; i++)
	{
		m_params[i] = scale;
	}
	for (int i = 3; i < 9; i++)
	{
		m_params[i] = 0;
	}

	
	QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6, tran7;
	tran1.translate(-QVector3D(m_rotCenter[0], m_rotCenter[1], m_rotCenter[2]));
	tran2.scale(m_params[0], m_params[1], m_params[2]);
	tran3.rotate(m_params[3], QVector3D(1, 0, 0));
	tran4.rotate(m_params[4], QVector3D(0, 1, 0));
	tran5.rotate(m_params[5], QVector3D(0, 0, 1));
	tran6.translate(QVector3D(m_params[6], m_params[7], m_params[8]));
	tran7.translate(QVector3D(m_rotCenter[0], m_rotCenter[1], m_rotCenter[2]));
	Tr = Matrix44m((tran7 * tran6 * tran5 * tran4 * tran3 * tran2 * tran1).transposed().data());
}

void CMeshO::adjustRotCenterXYToPrintCenter(double moveToX, double moveToY)
{
	m_params[6] = moveToX - m_rotCenter[0];
	m_params[7] = moveToY - m_rotCenter[1];

	
	QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6, tran7;
	tran1.translate(-QVector3D(m_rotCenter[0], m_rotCenter[1], m_rotCenter[2]));
	tran2.scale(m_params[0], m_params[1], m_params[2]);
	tran3.rotate(m_params[3], QVector3D(1, 0, 0));
	tran4.rotate(m_params[4], QVector3D(0, 1, 0));
	tran5.rotate(m_params[5], QVector3D(0, 0, 1));
	tran6.translate(QVector3D(m_params[6], m_params[7], m_params[8]));
	tran7.translate(QVector3D(m_rotCenter[0], m_rotCenter[1], m_rotCenter[2]));
	Tr = Matrix44m((tran7 * tran6 * tran5 * tran4 * tran3 * tran2 * tran1).transposed().data());
}

/**
 * @brief When copying a vcg mesh, it is first necessary to enable
 * all the optional fields that are enabled on the other mesh, otherwise
 * they won't be copied on this mesh...........
 */
void CMeshO::enableComponentsFromOtherMesh(const CMeshO& oth)
{
	//vertex
	if (oth.vert.IsVFAdjacencyEnabled())
		this->vert.EnableVFAdjacency();
	if (oth.vert.IsMarkEnabled())
		this->vert.EnableMark();
	if (oth.vert.IsTexCoordEnabled())
		this->vert.EnableTexCoord();
	if (oth.vert.IsCurvatureEnabled())
		this->vert.EnableCurvature();
	if (oth.vert.IsCurvatureDirEnabled())
		this->vert.EnableCurvatureDir();
	if (oth.vert.IsRadiusEnabled())
		this->vert.EnableRadius();

	//face
	if (oth.face.IsQualityEnabled())
		this->face.EnableQuality();
	if (oth.face.IsMarkEnabled())
		this->face.EnableMark();
	if (oth.face.IsColorEnabled())
		this->face.EnableColor();
	if (oth.face.IsFFAdjacencyEnabled())
		this->face.EnableFFAdjacency();
	if (oth.face.IsVFAdjacencyEnabled())
		this->face.EnableVFAdjacency();
	if (oth.face.IsCurvatureDirEnabled())
		this->face.EnableCurvatureDir();
	if (oth.face.IsWedgeTexCoordEnabled())
		this->face.EnableWedgeTexCoord();

	std::vector<std::string> perVScalarAttrs, perVPointAttrs;
	std::vector<std::string> perFScalarAttrs, perFPointAttrs;

	vcg::tri::Allocator<CMeshO>::GetAllPerVertexAttribute<Scalarm>(oth, perVScalarAttrs);
	vcg::tri::Allocator<CMeshO>::GetAllPerVertexAttribute<Point3m>(oth, perVPointAttrs);
	vcg::tri::Allocator<CMeshO>::GetAllPerFaceAttribute<Scalarm>(oth, perFScalarAttrs);
	vcg::tri::Allocator<CMeshO>::GetAllPerFaceAttribute<Point3m>(oth, perFPointAttrs);

	for(const std::string& attr : perVScalarAttrs)
		vcg::tri::Allocator<CMeshO>::AddPerVertexAttribute<Scalarm>(*this, attr);
	for(const std::string& attr : perVPointAttrs)
		vcg::tri::Allocator<CMeshO>::AddPerVertexAttribute<Point3m>(*this, attr);
	for(const std::string& attr : perFScalarAttrs)
		vcg::tri::Allocator<CMeshO>::AddPerFaceAttribute<Scalarm>(*this, attr);
	for(const std::string& attr : perFPointAttrs)
		vcg::tri::Allocator<CMeshO>::AddPerFaceAttribute<Point3m>(*this, attr);
}
