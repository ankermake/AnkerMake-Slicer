/****************************************************************************
 * MeshLab                                                           o o     *
 * A versatile mesh processing toolbox                             o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2008                                                \/)\/    *
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

#include <meshlab/glarea.h>
#include "edit_meshAnyFaceToPlatformTransform.h"
#include <wrap/qt/gl_label.h>
#include <wrap/gui/trackball.h>
#include "wrap/gl/space.h"
#include "common/GeoAndShow/TransformPack.h"
#include "common/GeoAndShow/CHLine3D.h"
#include "common/GeoAndShow/CHBaseAlg.h"
#include "edit_meshtransform_factory.h"
#include "vcg/complex/algorithms/convex_hull.h"


using namespace vcg;


CHTriangleShow::CHTriangleShow()
{
}

CHTriangleShow::~CHTriangleShow()
{
}

void CHTriangleShow::create(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3)
{
	m_pt1 = pt1;
	m_pt2 = pt2;
	m_pt3 = pt3;

	QVector3D v0 = pt3 - pt1;
	QVector3D v1 = pt2 - pt1;
	m_nor = QVector3D::crossProduct(v1, v0).normalized();//???
}

QVector3D CHTriangleShow::realNormal()
{
	QVector3D pt1 = TransformPack::pRot(m_tran, m_pt1);
	QVector3D pt2 = TransformPack::pRot(m_tran, m_pt2);
	QVector3D pt3 = TransformPack::pRot(m_tran, m_pt3);

	QVector3D v0 = pt3 - pt1;
	QVector3D v1 = pt2 - pt1;
	return QVector3D::crossProduct(v1, v0).normalized();//???
}

void CHTriangleShow::draw()
{
	glPushMatrix();

	glMultMatrixf(m_tran.data());
	if (m_status == general)
	{
		glColor4f((float)m_color.red() / 255.0, (float)m_color.green() / 255.0,
			(float)m_color.blue() / 255.0, (float)m_color.alpha() / 255.0);
	}
	else
	{
		glColor4f((float)canPickedColor.red() / 255.0, (float)canPickedColor.green() / 255.0,
			(float)canPickedColor.blue() / 255.0, (float)canPickedColor.alpha() / 255.0);
	}
	glBegin(GL_TRIANGLES);
	glNormal3d(m_nor[0], m_nor[1], m_nor[2]);
	glVertex3f(m_pt1[0], m_pt1[1], m_pt1[2]);

	glNormal3d(m_nor[0], m_nor[1], m_nor[2]);
	glVertex3f(m_pt2[0], m_pt2[1], m_pt2[2]);

	glNormal3d(m_nor[0], m_nor[1], m_nor[2]);
	glVertex3f(m_pt3[0], m_pt3[1], m_pt3[2]);
	glEnd();

	glPopMatrix();
}

bool CHTriangleShow::pick(int pixelX, int pixelY, double* mm, double* pm, int* vp, PickResult& result, int pickTol)
{
	return false;
}

bool CHTriangleShow::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
	return false;
}

void CHTriangleShow::updateToScene()
{
}

CHAABB3D CHTriangleShow::calBaseAABB()
{
	return CHAABB3D();
}

CHAABB3D CHTriangleShow::calRealAABB()
{
	return CHAABB3D();
}


EditMeshAnyFaceTransformTool::EditMeshAnyFaceTransformTool()
{
	m_glView = 0;
	m_meshModel = 0;
	m_testFace = 0;
}

bool EditMeshAnyFaceTransformTool::startEdit(ActionEditTool * action, void * arg1, void *arg2)
{
    if(isActivated){return false;}
    isActivated = true;

	curScene = gla->m_scene;
	if (curScene->m_pickCommand->m_selectedObjs.size() == 0)
	{
		return false;
	}
	//m_meshModel = *(curScene->m_pickCommand->m_selectedObjs.begin());

	m_glView = gla;


	//????
	/*CMeshO convexhull;
	tri::UpdateTopology<CMeshO>::FaceFace(convexhull);
	tri::UpdateNormal<CMeshO>::PerFaceNormalized(convexhull);
	vcg::tri::ConvexHull<CMeshO, CMeshO>::ComputeConvexHull(m_meshModel->cm, convexhull);
	int num1 = convexhull.vert.size();
	int num2 = convexhull.face.size();*/

	Box3f aabb = m_meshModel->cm.bbox;
	if ((int)m_meshModel->cm.m_params.size() == 0)//??????
	{
		Point3f pt = aabb.Center();

		m_meshModel->cm.m_rotCenter[0] = pt.X();
		m_meshModel->cm.m_rotCenter[1] = pt.Y();
		m_meshModel->cm.m_rotCenter[2] = pt.Z();
		m_meshModel->cm.m_params.resize(9);
		for (int i = 0; i < 3; i++)
		{
			m_meshModel->cm.m_params[i] = 1;
		}
		for (int i = 3; i < 9; i++)
		{
			m_meshModel->cm.m_params[i] = 0;
		}
	}

	m_testFace = CHTriangleShowPtr(new CHTriangleShow);
	m_testFace->setVisuable(false);
	m_allShowObjs.push_back(m_testFace);
	
	return true;
}

void EditMeshAnyFaceTransformTool::endEdit  (ActionEditTool *, void *, void *)
{
    if(!isActivated){return;}
    isActivated = false;

	m_allShowObjs.clear();

}

void EditMeshAnyFaceTransformTool::mousePressEvent(QMouseEvent* event, MeshModel&, GLArea*)
{
	if (m_testFace->getVisuable())
	{
		m_testFace->setVisuable(false);

		//???????
		QMatrix4x4 tran1, tran2, tran3, tran4;
		tran1.rotate(m_meshModel->cm.m_params[3], QVector3D(1, 0, 0));
		tran2.rotate(m_meshModel->cm.m_params[4], QVector3D(0, 1, 0));
		tran3.rotate(m_meshModel->cm.m_params[5], QVector3D(0, 0, 1));
		QVector3D axis1 = m_testFace->realNormal();
		QVector3D axis2(0, 0, -1);
		float angle = TransformPack::vecAngle(axis1, axis2);	
		QVector3D axis = QVector3D::crossProduct(axis1, axis2);
		if (fabs(angle - CH_PI) < 0.0000001)
		{
			axis = QVector3D(1, 0, 0);
		}
		tran4.rotate(angle / CH_PI * 180.0, axis);

		double angleX, angleY, angleZ;
		CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tran4 * tran3 * tran2 * tran1, angleX, angleY, angleZ);

		//???0-2pi??
		m_meshModel->cm.m_params[3] = angleX / CH_PI * 180.0;
		m_meshModel->cm.m_params[4] = angleY / CH_PI * 180.0;
		m_meshModel->cm.m_params[5] = angleZ / CH_PI * 180.0;
		adjustSingleAngle(m_meshModel->cm.m_params[3]);
		adjustSingleAngle(m_meshModel->cm.m_params[4]);
		adjustSingleAngle(m_meshModel->cm.m_params[5]);

		//??????
		QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D(m_meshModel->cm.m_rotCenter[0],
			m_meshModel->cm.m_rotCenter[1], m_meshModel->cm.m_rotCenter[2]), m_meshModel->cm.m_params);

		//????
		m_meshModel->cm.Tr = Matrix44m(sumtran1.transposed().data());

		//??z=0????
		Box3m aabb = m_meshModel->cm.calCurrentAABB();
		m_meshModel->cm.m_params[8] = m_meshModel->cm.m_params[8] - aabb.min.Z();

		//??????
		QMatrix4x4 sumtran2 = CHBaseAlg::instance()->calTransformFromParams(QVector3D(m_meshModel->cm.m_rotCenter[0],
			m_meshModel->cm.m_rotCenter[1], m_meshModel->cm.m_rotCenter[2]), m_meshModel->cm.m_params);

		//????
		m_meshModel->cm.Tr = Matrix44m(sumtran2.transposed().data());

		m_glView->update();
	}
}

void EditMeshAnyFaceTransformTool::mouseMoveEvent(QMouseEvent* event, MeshModel& meshmodel, GLArea* gla)
{
	//????
	double nres[3];
	double fres[3];
	gluUnProject(event->pos().x(), vp[3] - event->pos().y(), -1.0, mm, pm, vp, &nres[0], &nres[1], &nres[2]);
	gluUnProject(event->pos().x(), vp[3] - event->pos().y(), 1.0, mm, pm, vp, &fres[0], &fres[1], &fres[2]);
	CHLineSegment3DShowObjPtr ray(new CHLineSegment3DShowObj);
	ray->create(QVector3D(nres[0], nres[1], nres[2]), QVector3D(fres[0], fres[1], fres[2]));

	CHPointOnMesh pom;
	if (m_meshModel->cm.pick(ray, pom))
	{
		CFaceO* tface = (CFaceO*)m_meshModel->cm.face.data() + pom.m_faceIndex;
		vcgTriMesh::VertexType* pv0 = tface->V(0);
		vcgTriMesh::VertexType* pv1 = tface->V(1);
		vcgTriMesh::VertexType* pv2 = tface->V(2);
		//QVector3D nor = pom.m_nor.normalized();
		//float dis = 1.0;
		m_testFace->create(QVector3D(pv0->P().V()[0], pv0->P().V()[1], pv0->P().V()[2]) /*+ nor * dis*/,
			QVector3D(pv1->P().V()[0], pv1->P().V()[1], pv1->P().V()[2]) /*+ nor * dis*/,
			QVector3D(pv2->P().V()[0], pv2->P().V()[1], pv2->P().V()[2]) /*+ nor * dis*/);
		m_testFace->setTransform(QMatrix4x4(m_meshModel->cm.Tr.ElementAt(0, 0), m_meshModel->cm.Tr.ElementAt(0, 1), m_meshModel->cm.Tr.ElementAt(0, 2), m_meshModel->cm.Tr.ElementAt(0, 3),
			m_meshModel->cm.Tr.ElementAt(1, 0), m_meshModel->cm.Tr.ElementAt(1, 1), m_meshModel->cm.Tr.ElementAt(1, 2), m_meshModel->cm.Tr.ElementAt(1, 3),
			m_meshModel->cm.Tr.ElementAt(2, 0), m_meshModel->cm.Tr.ElementAt(2, 1), m_meshModel->cm.Tr.ElementAt(2, 2), m_meshModel->cm.Tr.ElementAt(2, 3),
			m_meshModel->cm.Tr.ElementAt(3, 0), m_meshModel->cm.Tr.ElementAt(3, 1), m_meshModel->cm.Tr.ElementAt(3, 2), m_meshModel->cm.Tr.ElementAt(3, 3)));
		m_testFace->setVisuable(true);

	}
	else
	{
		m_testFace->setVisuable(false);
	}

	gla->update();	
}

void EditMeshAnyFaceTransformTool::mouseReleaseEvent(QMouseEvent* event, MeshModel&, GLArea*)
{
	
}

void EditMeshAnyFaceTransformTool::adjustSingleAngle(float& angle)
{
	int n = (int)(angle / 360.0);
	if (angle < 0)
	{
		n--;
	}
	angle = angle - 360.0 * n;
}


