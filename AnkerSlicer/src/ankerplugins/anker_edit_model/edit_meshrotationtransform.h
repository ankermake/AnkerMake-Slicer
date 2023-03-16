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
#ifndef EditMeshRotationTransformTool_H
#define EditMeshRotationTransformTool_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <common/plugins/interfaces/anker_edit_plugin.h>
#include <common/plugins/interfaces/edit_plugin.h>
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHCircleShowObj.h"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "CHModelRotationTransformParamsSetUI.h"
#include "common/GeoAndShow/CHAssembly.h"
#include "common/GeoAndShow/CHScene.h"


DEF_PTR(CHLocalCoordinateAxis)
class CHLocalCoordinateAxis :public CHAssembly
{
public:
    CHLocalCoordinateAxis();
    virtual ~CHLocalCoordinateAxis();

public:
    void create(QVector3D center, double lenth);

private:
    CHLineSegment3DShowObjPtr m_axisX;
    CHLineSegment3DShowObjPtr m_axisY;
    CHLineSegment3DShowObjPtr m_axisZ;
};


DEF_PTR(CHAdjustCircle)

class CHAdjustCircle :public CHAssembly
{
public:
    CHAdjustCircle();
    virtual ~CHAdjustCircle();

public:
    void create(QVector3D center, double rad, QVector3D nor, QVector3D refVec);
    QVector3D getOriginCenter() const;
    QVector3D getCurrentCenter() const;

public:
    
    CHCircleShowObjPtr m_circleBody;
    CHLineSegment3DShowObjPtr m_lineBody;
    CHPointShowObjPtr m_adjustPointBody;
};


class EditMeshRotationTransformTool : public QObject, public AnkerEditTool
{
    Q_OBJECT

public:

    EditMeshRotationTransformTool();
    virtual ~EditMeshRotationTransformTool() {}

    void initInMainUI  () override ; //  add  @2023-01-13 by ChunLian
    bool startAnkerEdit(ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;
    void endAnkerEdit  (ActionEditTool * action, void * arg1=nullptr, void *arg2=nullptr) override;

    void mousePressEvent  (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseMoveEvent   (QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;
    void mouseReleaseEvent(QMouseEvent  *event, void * arg1=nullptr, void *arg2=nullptr) override;

public Q_SLOTS:
    void showAxis(bool showed);
    void receiveParams(vector<float> params);
    void receiveParamsFromGui(std::vector<double> params, RotateChangedType type = RotateChangedType_BoxRotate);
    void resetBtnClicked();
    void resetSelectedObjsClicked();

Q_SIGNALS:
    void sendParams(vector<float> params);
    void sendParamsToGui(std::vector<double> params, RotateChangedType type = RotateChangedType_ViewRotate);
    void sendParam(double angle, RotateAxisType refAxis);

private:
    void adjustSingleAngle(float& angle);

    
    void refreshRotationFrame();

    void submitToUI();
    void resetSelectedRotate();

    void checkOnBottom();



private:
    CHModelRotationTransformParamsSetUI* m_paramUI{nullptr};
    std::set<CHMeshShowObjPtr> m_editMeshModels;
    CHMeshShowObjPtr m_firstMesh;//???????????????
    QVector3D m_operationCenter;

    
    CHLocalCoordinateAxisPtr m_CoordAxis1;
    CHLocalCoordinateAxisPtr m_CoordAxis2;

    
    CHAdjustCirclePtr m_adjustCircleX;
    CHAdjustCirclePtr m_adjustCircleY;
    CHAdjustCirclePtr m_adjustCircleZ;
    std::vector<CHShowObjPtr> m_allPickObjs;

    std::vector<CHShowObjPtr> m_allShowObjs;

    CHShowObjPtr m_pickedObj;
    int m_stepFlag;
    std::vector<std::vector<float>> m_values;
    std::vector<std::vector<float>> m_initValues;

    float m_operateMoveZ;

    CHLineSegment3DShowObjPtr m_adjustShowCurve;

    
    GLdouble mm[16];
    GLdouble pm[16];
    GLint vp[4];
};

#endif
