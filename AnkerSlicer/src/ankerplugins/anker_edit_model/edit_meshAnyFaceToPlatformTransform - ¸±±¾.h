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
#ifndef EditMeshAnyFaceTransformTool_H
#define EditMeshAnyFaceTransformTool_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <common/plugins/interfaces/anker_edit_plugin.h>
#include <common/plugins/interfaces/edit_plugin.h>
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHPointShowObj.h"


DEF_PTR(CHTriangleShow)
//??????
class CHTriangleShow :public CHShowObj
{
public:
	CHTriangleShow();
	virtual ~CHTriangleShow();

public:
	void create(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3);

	QVector3D realNormal();

	virtual void draw();
	virtual bool pick(int pixelX, int pixelY, double* mm, double* pm, int* vp, PickResult& result, int pickTol = 10);
	virtual bool pick(int pixelX, int pixelY,PickResult& result, int pickTol = 10);
	virtual void updateToScene();
	virtual CHAABB3D calBaseAABB();
	virtual CHAABB3D calRealAABB();

public:
	QVector3D m_pt1;
	QVector3D m_pt2;
	QVector3D m_pt3;
	QVector3D m_nor;
};


class EditMeshAnyFaceTransformTool : public QObject, public AnkerEditTool
{
	Q_OBJECT

public:

	EditMeshAnyFaceTransformTool();
	virtual ~EditMeshAnyFaceTransformTool() {}

    bool startEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;
    void endEdit  (ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;

    void mousePressEvent  (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseMoveEvent   (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseReleaseEvent(QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;

private:
	void adjustSingleAngle(float& angle);

private:
	GLArea* m_glView;
	MeshModel* m_meshModel;

	CHTriangleShowPtr m_testFace;	
	std::vector<CHShowObjPtr> m_allShowObjs;

    
	GLdouble mm[16];
	GLdouble pm[16];
	GLint vp[4];
};

#endif
