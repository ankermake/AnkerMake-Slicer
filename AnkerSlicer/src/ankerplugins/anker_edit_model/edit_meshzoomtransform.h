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
#ifndef EditMeshZoomTransformTool_H
#define EditMeshZoomTransformTool_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <common/plugins/interfaces/anker_edit_plugin.h>
#include <common/plugins/interfaces/edit_plugin.h>
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHPointShowObj.h"
#include "CHModelZoomTransformParamsSetUI.h"



class EditMeshZoomTransformTool : public QObject, public AnkerEditTool
{
    Q_OBJECT

public:

    EditMeshZoomTransformTool();
    virtual ~EditMeshZoomTransformTool() {}
    bool startAnkerEdit(ActionEditTool* action, void* arg1 = nullptr, void* arg2 = nullptr) override;
    void endAnkerEdit(ActionEditTool* action, void* arg1 = nullptr, void* arg2 = nullptr) override;

    void mousePressEvent(QMouseEvent* event, void* arg1 = nullptr, void* arg2 = nullptr) override;
    void mouseMoveEvent(QMouseEvent* event, void* arg1 = nullptr, void* arg2 = nullptr) override;
    void mouseReleaseEvent(QMouseEvent* event, void* arg1 = nullptr, void* arg2 = nullptr) override;

public Q_SLOTS:
    void receiveParams(std::vector<float> params);
    void receiveBoxParams(std::vector<float> params);
    void scaleToFitClicked();
    void resetBtnClicked();
    void resetSelectedObjsClicked();
    void updateLock(bool);

    void viewValuesChanged(std::vector<double> params, ZoomChangedType type);

Q_SIGNALS:
    void sendParams(std::vector<float> params);
    void sendBoxSizeParam(std::vector<float > params);
    void boxSizeParamsChanged(std::vector<double> params, ZoomChangedType type);
    void scaleParamsChanged(std::vector<double> params, ZoomChangedType type);


private:

    
    void refreshScaleFrame();
    float getRadio(const CHAABB3D& aabb, const CHAABB3D& platformAabb);
    float getTimes(const CHAABB3D& aabb, const CHAABB3D& platformAabb); 
    void submitToUI();
    void refreshBoxSizeUI();
    void createFrame(const QVector3D& origin, float len, float width, float height);
    void stickOnBottom(); 

private:
    CHModelZoomTransformParamsSetUI* m_paramUI;
    std::set<CHMeshShowObjPtr> m_editMeshModels;
    CHMeshShowObjPtr m_firstMesh;
    QVector3D m_operationCenter;

    CHLineSegment3DShowObjPtr m_adjustAxisX;
    CHLineSegment3DShowObjPtr m_adjustAxisY;
    CHLineSegment3DShowObjPtr m_adjustAxisZ;
    CHPointShowObjPtr m_markXPoint;
    CHPointShowObjPtr m_markYPoint;
    CHPointShowObjPtr m_markZPoint;
    CHPointShowObjPtr m_adjustXPoint;
    CHPointShowObjPtr m_adjustYPoint;
    CHPointShowObjPtr m_adjustZPoint;
    CHPointShowObjPtr m_adjustXYZPoint;

    CHLineSegment3DShowObjPtr m_middleshowcurve1;
    CHLineSegment3DShowObjPtr m_middleshowcurve2;

    //CHPointShowObjPtr m_adjustOrigin;

    QVector3D m_pickCoord;
    std::vector<CHShowObjPtr> m_allPickObjs;

    std::vector<CHShowObjPtr> m_allShowObjs;

    CHShowObjPtr m_pickedObj;
    int m_stepFlag;
    std::vector<std::vector<float>> m_values;
    std::vector<std::vector<float>> m_initValues;

    std::vector<double > m_initBox;

    float m_operateMoveZ;

    
    GLdouble mm[16];
    GLdouble pm[16];
    GLint vp[4];
    float originMove[3];

    std::vector<float> lastParams;
    bool m_scaleToFit = false;
    QMatrix4x4 m_moveToCenterTrans;
    ZoomChangedType m_currentType;
};

#endif
