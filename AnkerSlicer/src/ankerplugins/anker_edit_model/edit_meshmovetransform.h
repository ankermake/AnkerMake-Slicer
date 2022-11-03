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
#ifndef EditMeshMoveTransformTool_H
#define EditMeshMoveTransformTool_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <common/plugins/interfaces/anker_edit_plugin.h>
#include <common/plugins/interfaces/edit_plugin.h>
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "CHModelMoveTransformParamsSetUI.h"
#include "common/GeoAndShow/CHAssembly.h"
#include "common/GeoAndShow/CHCone.h"


DEF_PTR(CHAxisWithArrow)
class CHAxisWithArrow :public CHAssembly
{
public:
    CHAxisWithArrow();
    virtual ~CHAxisWithArrow();

public:
    void create(QVector3D center, QVector3D axisDir, float axisLenth, float rad, float height);

    virtual bool pick(int pixelX, int pixelY, PickResult& result, int pickTol = 10);//?????????m_axis?pick?????


public:
    CHLineSegment3DShowObjPtr m_axis;
    CHConePtr m_arrow;

};

class EditMeshMoveTransformTool : public QObject, public AnkerEditTool
{
    Q_OBJECT

public:

    EditMeshMoveTransformTool();
    virtual ~EditMeshMoveTransformTool() {}

    bool startAnkerEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;
    void endAnkerEdit  (ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;

    void mousePressEvent  (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseMoveEvent   (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseReleaseEvent(QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;


    public
Q_SLOTS:
    void receiveParams(vector<float> params);
    void receiveParamsFormGui(std::vector<double> params, MoveChangedType type = MoveChangedType_BoxMove);
    void resetBtnClicked();
    void resetSelectedObjsClicked();
    void lockToPrintPlatformStatusChanged();


Q_SIGNALS:
    void sendParams(vector<float> params);
    void sendParamsToGui(std::vector<double> params, MoveChangedType type = MoveChangedType_ViewMove);

private:
    
    void refreshMoveFrame();

    void submitToUI();


private:
    CHModelMoveTransformParamsSetUI* m_paramUI;
    std::set<CHMeshShowObjPtr> m_editMeshModels;
    CHMeshShowObjPtr m_firstMesh;//???????????????
    QVector3D m_operationCenter;

    CHAxisWithArrowPtr m_adjustAxisX;
    CHAxisWithArrowPtr m_adjustAxisY;
    CHAxisWithArrowPtr m_adjustAxisZ;
    CHPointShowObjPtr m_adjustOrigin;
    std::vector<CHShowObjPtr> m_allPickObjs;

    QVector3D m_pickCoord;
    CHShowObjPtr m_pickedObj;
    int m_stepFlag;
    std::vector<std::vector<float>> m_values;
    std::vector<std::vector<float>> m_initValues;

    
    GLdouble mm[16];
    GLdouble pm[16];
    GLint vp[4];

};

#endif
