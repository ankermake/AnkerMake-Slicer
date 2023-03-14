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
#ifndef EditMeshMirrorTransformTool_H
#define EditMeshMirrorTransformTool_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <common/plugins/interfaces/anker_edit_plugin.h>
#include <common/plugins/interfaces/edit_plugin.h>
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "CHModelMirrorTransformParamsSetUI.h"



class EditMeshMirrorTransformTool : public QObject, public AnkerEditTool
{
	Q_OBJECT

public:

	EditMeshMirrorTransformTool();
	virtual ~EditMeshMirrorTransformTool() {}

    bool startAnkerEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;
    void endAnkerEdit  (ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;

    void mousePressEvent  (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseMoveEvent   (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseReleaseEvent(QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;


  public
Q_SLOTS:
	void receiveButtonClicked(int index);
	void reset();

private:
	void adjustSingleAngle(float& angle);

private:
	CHModelMirrorTransformParamsSetUI* m_paramUI;
	std::set<CHMeshShowObjPtr> m_editMeshModels;
    CHMeshShowObjPtr m_firstMesh;
	QVector3D m_operationCenter;

    std::vector<std::vector<float>> m_initValues;

	float m_operateMoveZ;
};

#endif
