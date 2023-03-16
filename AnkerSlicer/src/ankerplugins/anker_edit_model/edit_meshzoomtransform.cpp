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

//#include <meshlab/glarea.h>
#include "edit_meshzoomtransform.h"
#include <wrap/qt/gl_label.h>
#include <wrap/gui/trackball.h>
#include "wrap/gl/space.h"
#include "common/GeoAndShow/TransformPack.h"
#include "common/GeoAndShow/CHLine3D.h"
#include "common/GeoAndShow/CHBaseAlg.h"
#include "common/GeoAndShow/sphere.h"
//#include <minwindef.h>
#include "edit_meshtransform_factory.h"
#include "common/GeoAndShow/support/SupportAssemblyMeshes.h"
//#include "BoxBorder.h"
#include "common/utilities/tlogger.h"

using namespace vcg;


EditMeshZoomTransformTool::EditMeshZoomTransformTool()
{
    AkUtil::TFunction("");
    m_stepFlag = 0;
    m_operateMoveZ = 0;

    originMove[0] = originMove[1] = originMove[2] = 0.0;
}

void EditMeshZoomTransformTool::initInMainUI()
{
    m_paramUI = new CHModelZoomTransformParamsSetUI();

    EditMeshTransformFactory::m_conInt->addWidgetToModelTransForm(m_paramUI, AkConst::FDMMeshTransForm::Zoom);

    connect(this, &EditMeshZoomTransformTool::boxSizeParamsChanged,     m_paramUI, &CHModelZoomTransformParamsSetUI::boxSizeValuesChanged);
    connect(this, &EditMeshZoomTransformTool::scaleParamsChanged,       m_paramUI, &CHModelZoomTransformParamsSetUI::scaleValuesChanged);

    connect(m_paramUI, &CHModelZoomTransformParamsSetUI::viewValuesChanged, this, &EditMeshZoomTransformTool::viewValuesChanged);
    connect(m_paramUI, &CHModelZoomTransformParamsSetUI::scaleToFitSignal,  this, &EditMeshZoomTransformTool::scaleToFitClicked);
    connect(m_paramUI, &CHModelZoomTransformParamsSetUI::resetSignal,       this, &EditMeshZoomTransformTool::resetBtnClicked);
    //connect(m_paramUI, &CHModelZoomTransformParamsSetUI::stateChanged,    this, &EditMeshZoomTransformTool::updateLock);
    m_paramUI->hide();
}

bool EditMeshZoomTransformTool::startAnkerEdit(ActionEditTool* action, void* arg1, void* arg2)
{
    AkUtil::TFunction("");
    if (m_isActivated) { return false; }
    m_isActivated = true;

    curScene = static_cast<CHScene*>(arg1);
    if (curScene->m_pickCommand->m_selectedObjs.size() == 0)
    {
        return false;
    }

    m_editMeshModels = curScene->m_pickCommand->m_selectedObjs;
    m_firstMesh = *m_editMeshModels.begin();

    m_stepFlag = 0;
    m_pickedObj = 0;
    m_operateMoveZ = 0;


    if(m_paramUI){
        m_paramUI->show();
    }

    m_currentType = ZoomChangedType_View;
    
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        EditMeshTransformFactory::initMeshModelEulerParams(*it);
    }

    m_values.resize(m_editMeshModels.size());
    m_initValues.resize(m_editMeshModels.size());

    int p = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        m_values[p].resize(6);
        m_initValues[p].resize(6);
        m_initValues[p][0] = (*it)->m_params[0];
        m_initValues[p][1] = (*it)->m_params[1];
        m_initValues[p][2] = (*it)->m_params[2];
        m_initValues[p][3] = (*it)->m_params[6];
        m_initValues[p][4] = (*it)->m_params[7];
        m_initValues[p][5] = (*it)->m_params[8];
        p++;
    }

    
    CHAABB3D aabb;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        aabb.add((*it)->calRealAABB());
    }
    m_operationCenter = aabb.getCenterPoint();

    m_initBox.clear();
    if(m_editMeshModels.size() == 1)
    {
        m_initBox.push_back((*m_editMeshModels.begin())->m_baseAABB.getLenX() * (*m_editMeshModels.begin())->m_params[0]);
        m_initBox.push_back((*m_editMeshModels.begin())->m_baseAABB.getLenY() * (*m_editMeshModels.begin())->m_params[1]);
        m_initBox.push_back((*m_editMeshModels.begin())->m_baseAABB.getLenZ() * (*m_editMeshModels.begin())->m_params[2]);
        m_paramUI->initBoxSize((*m_editMeshModels.begin())->m_baseAABB.getLenX(),
                               (*m_editMeshModels.begin())->m_baseAABB.getLenY(),
                               (*m_editMeshModels.begin())->m_baseAABB.getLenZ());
    }
    else if(m_editMeshModels.size() > 1)
    {
        m_initBox.push_back(aabb.getLenX());
        m_initBox.push_back(aabb.getLenY());
        m_initBox.push_back(aabb.getLenZ());
        m_paramUI->initBoxSize(aabb.getLenX(), aabb.getLenY(), aabb.getLenZ());
    }

    emit boxSizeParamsChanged(m_initBox, ZoomChangedType_Init);

    
    //QVector3D originCreateCenter(m_meshModel->m_rotCenter[0], m_meshModel->m_rotCenter[1], m_meshModel->m_rotCenter[2]);
    QVector3D originCreateCenter = m_operationCenter;
    QColor xcolor(125, 0, 0);
    QColor ycolor(0, 125, 0);
    QColor zcolor(0, 0, 125);
    double marksize = 4;
    double adjustsize = 8;

    int len = (m_initBox[0] + m_initBox[1] + m_initBox[2]) / 3.0;//(aabb.getLenX() > aabb.getLenY() ? aabb.getLenX() : aabb.getLenY());
    m_adjustAxisX = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_adjustAxisX->create(originCreateCenter, originCreateCenter + QVector3D(len, 0, 0));
    m_adjustAxisX->setColor(xcolor);
    m_adjustAxisY = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_adjustAxisY->create(originCreateCenter, originCreateCenter + QVector3D(0, len, 0));
    m_adjustAxisY->setColor(ycolor);
    m_adjustAxisZ = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_adjustAxisZ->create(originCreateCenter, originCreateCenter + QVector3D(0, 0, len));
    m_adjustAxisZ->setColor(zcolor);

    m_markXPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_markXPoint->create(m_adjustAxisX->m_et);
    m_markXPoint->setColor(QColor(0, 0, 0));
    m_markXPoint->setSize(marksize);
    m_markYPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_markYPoint->create(m_adjustAxisY->m_et);
    m_markYPoint->setColor(QColor(0, 0, 0));
    m_markYPoint->setSize(marksize);
    m_markZPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_markZPoint->create(m_adjustAxisZ->m_et);
    m_markZPoint->setColor(QColor(0, 0, 0));
    m_markZPoint->setSize(marksize);

    m_adjustXPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustXPoint->create(m_adjustAxisX->m_et);
    m_adjustXPoint->setColor(xcolor);
    m_adjustXPoint->setSize(adjustsize);
    m_adjustYPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustYPoint->create(m_adjustAxisY->m_et);
    m_adjustYPoint->setColor(ycolor);
    m_adjustYPoint->setSize(adjustsize);
    m_adjustZPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustZPoint->create(m_adjustAxisZ->m_et);
    m_adjustZPoint->setColor(zcolor);
    m_adjustZPoint->setSize(adjustsize);
    m_adjustXYZPoint = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustXYZPoint->create(m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
    m_adjustXYZPoint->setColor(QColor(125, 0, 125));
    m_adjustXYZPoint->setSize(adjustsize);

    m_middleshowcurve1 = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_middleshowcurve1->create(m_adjustAxisY->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
    m_middleshowcurve1->setColor(xcolor);
    m_middleshowcurve2 = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_middleshowcurve2->create(m_adjustAxisX->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
    m_middleshowcurve2->setColor(ycolor);

    
    refreshScaleFrame();

    m_allPickObjs.push_back(m_adjustXPoint);
    m_allPickObjs.push_back(m_adjustYPoint);
    m_allPickObjs.push_back(m_adjustZPoint);
    m_allPickObjs.push_back(m_adjustXYZPoint);

    m_allShowObjs.push_back(m_adjustAxisX);
    m_allShowObjs.push_back(m_adjustAxisY);
    m_allShowObjs.push_back(m_adjustAxisZ);
    m_allShowObjs.push_back(m_middleshowcurve1);
    m_allShowObjs.push_back(m_middleshowcurve2);
    m_allShowObjs.push_back(m_markXPoint);
    m_allShowObjs.push_back(m_markYPoint);
    m_allShowObjs.push_back(m_markZPoint);
    m_allShowObjs.push_back(m_adjustXPoint);
    m_allShowObjs.push_back(m_adjustYPoint);
    m_allShowObjs.push_back(m_adjustZPoint);
    m_allShowObjs.push_back(m_adjustXYZPoint);

    for (int i = 0; i < m_allShowObjs.size(); i++)
    {
        m_allShowObjs[i]->setLightTest(false);
        m_allShowObjs[i]->updateToScene();
    }

    return true;
}

void EditMeshZoomTransformTool::endAnkerEdit(ActionEditTool*, void*, void*)
{
    AkUtil::TFunction("");
    if (!m_isActivated) { return; }
    m_isActivated = false;

    if (m_paramUI)
    {
        m_paramUI->hide();
    }
    for (int i = 0; i < m_allShowObjs.size(); i++)
    {
        m_allShowObjs[i]->setDelete(true);
        m_allShowObjs[i]->updateToScene();
    }

    m_allPickObjs.clear();
    m_allShowObjs.clear();
}

void EditMeshZoomTransformTool::scaleToFitClicked()
{
    AkUtil::TFunction("");
    std::vector<float> params(3);
    {
        CHAABB3D platformAabb = getDoc()->m_machineBox->getBaseAABB();
        QVector3D platformCenter = platformAabb.getCenterPoint();
        platformCenter.setZ(0.0);
        CHAABB3D aabb1;
        
        for(std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            aabb1.add((*it)->calRealAABB());
        }

        QVector3D modelCenter = aabb1.getCenterPoint();
        modelCenter.setZ(modelCenter.z() - aabb1.getLenZ() / 2.0);
        QVector3D modelOffset = platformCenter - modelCenter;
        CHAABB3D aabb2;
        int p = 0;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            //EditMeshTransformFactory::initMeshModelEulerParams(*it);
            (*it)->m_params[6] += modelOffset[0];
            (*it)->m_params[7] += modelOffset[1];
            (*it)->m_params[8] += modelOffset[2];
            m_initValues[p][0] = (*it)->m_params[0];
            m_initValues[p][1] = (*it)->m_params[1];
            m_initValues[p][2] = (*it)->m_params[2];
            m_initValues[p][3] = (*it)->m_params[6];
            m_initValues[p][4] = (*it)->m_params[7];
            m_initValues[p][5] = (*it)->m_params[8];
            p++;
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);
            (*it)->updateToScene();
            aabb2.add((*it)->calRealAABB());
        }
        m_operationCenter = aabb2.getCenterPoint();
        float ratio = 1.0;
        ratio = getTimes(aabb2, platformAabb);
        if(ratio == 1.0)
        {
//            emit getDoc()->modelCheckSceneInChanged();
//            return;
        }
        else
        {
            params[0] = ratio;
            params[1] = ratio;
            params[2] = ratio;
            m_currentType = ZoomChangedType_ScaleToFit;
            receiveParams(params, ZoomAxisType_X);
        }
        CHAABB3D aabb3;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            (*it)->calRealAABB();
            (*it)->updateToScene();
            aabb3.add((*it)->m_realAABB);
        }

        float moveZ = -aabb3.m_Zmin;
        m_operationCenter[2] += moveZ;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            (*it)->m_params[8] += moveZ;

            
            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);
        }

        //m_operationCenter = platformAabb.getCenterPoint();
        QVector3D originCreateCenter = aabb3.getCenterPoint();
        originCreateCenter[2] += moveZ;
        m_initBox.clear();
        m_initBox.push_back(aabb3.getLenX());
        m_initBox.push_back(aabb3.getLenY());
        m_initBox.push_back(aabb3.getLenZ());
        p = 0;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            m_initValues[p][0] = (*it)->m_params[0];
            m_initValues[p][1] = (*it)->m_params[1];
            m_initValues[p][2] = (*it)->m_params[2];
            m_initValues[p][3] = (*it)->m_params[6];
            m_initValues[p][4] = (*it)->m_params[7];
            p++;
        }
        float len = (aabb3.getLenX() > aabb3.getLenY() ? aabb3.getLenX() : aabb3.getLenY());
        len = (len > aabb3.getLenZ() ? len : aabb3.getLenZ()) * 0.5;
        m_adjustAxisX->create(originCreateCenter, originCreateCenter + QVector3D(len , 0, 0));

        m_adjustAxisY->create(originCreateCenter, originCreateCenter + QVector3D(0, len, 0));
        m_adjustAxisZ->create(originCreateCenter, originCreateCenter + QVector3D(0, 0, len));

        m_markXPoint->create(m_adjustAxisX->m_et);

        m_markYPoint->create(m_adjustAxisY->m_et);

        m_markZPoint->create(m_adjustAxisZ->m_et);

        m_adjustXPoint->create(m_adjustAxisX->m_et);
        m_adjustYPoint->create(m_adjustAxisY->m_et);
        m_adjustZPoint->create(m_adjustAxisZ->m_et);
        m_adjustXYZPoint->create(m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
        m_middleshowcurve1->create(m_adjustAxisY->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
        m_middleshowcurve2->create(m_adjustAxisX->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);

        
        refreshScaleFrame();
        m_adjustXPoint->updateToScene();
        m_adjustYPoint->updateToScene();
        m_adjustZPoint->updateToScene();
        m_adjustXYZPoint->updateToScene();
        m_adjustAxisX->updateToScene();
        m_adjustAxisY->updateToScene();
        m_adjustAxisZ->updateToScene();
        m_middleshowcurve1->updateToScene();
        m_middleshowcurve2->updateToScene();
        m_markXPoint->updateToScene();
        m_markYPoint->updateToScene();
        m_markZPoint->updateToScene();
        m_adjustXPoint->updateToScene();
        m_adjustYPoint->updateToScene();
        m_adjustZPoint->updateToScene();
        m_adjustXYZPoint->updateToScene();

        //createFrame(m_operationCenter, m_originAABB.getLenX() * 0.5, m_originAABB.getLenY() * 0.5, m_originAABB.getLenZ() * 0.5);
        emit getDoc()->modelCheckSceneInChanged();
        emit restartEditToolSignal();
    }
}

void EditMeshZoomTransformTool::resetSelectedObjsClicked()
{
    AkUtil::TFunction("");
    getGlobalPick()->resetSelectedObjs();
    CHAABB3D aabb;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        aabb.add((*it)->calRealAABB());
    }
//    m_operationCenter = aabb.getCenterPoint();
    submitToUI();
}

void EditMeshZoomTransformTool::updateLock(bool check)
{
    m_lockScaleRatio = check;
    vector<float> params(3);
    params[0] = (*m_editMeshModels.begin())->m_params[0];
    params[1] = (*m_editMeshModels.begin())->m_params[1];
    params[2] = (*m_editMeshModels.begin())->m_params[2];
    emit sendParams(params);
}


void EditMeshZoomTransformTool::resetBtnClicked()
{
    AkUtil::TFunction("");
    //bool lockToPrintPlatform = m_lockToPrintPlatform;
    if(m_editMeshModels.size() == 1)
    {
        CHAABB3D baseAABB;
        baseAABB.add((*m_editMeshModels.begin())->m_baseAABB);
        //(*m_editMeshModels.begin())->resetZoom();
        (*m_editMeshModels.begin())->reset3x3Transform();
        //m_lockToPrintPlatform = true;
        int dir[3] = {1};
        dir[0] = ((*m_editMeshModels.begin())->m_params[0] > 0 ? 1: -1);
        dir[1] = ((*m_editMeshModels.begin())->m_params[1] > 0 ? 1: -1);
        dir[2] = ((*m_editMeshModels.begin())->m_params[2] > 0 ? 1: -1);
        vector<double> params(3);
        params[0] = baseAABB.getLenX() * dir[0];
        params[1] = baseAABB.getLenY() * dir[1];
        params[2] = baseAABB.getLenZ() * dir[2];

        emit boxSizeParamsChanged(params, ZoomChangedType_Reset);
        //m_lockToPrintPlatform = lockToPrintPlatform;
    }
    else if(m_editMeshModels.size() > 1)
    {
        //m_lockToPrintPlatform = true;
        resetSelectedScale();
        submitToUI();
        refreshScaleFrame();
        curScene->refresh();
        emit getDoc()->modelCheckSceneInChanged();
        emit getDoc()->modelObjsStatusChanged(ModelStatusChangedType::ResetMesh);
        emit restartEditToolSignal();
        //m_lockToPrintPlatform = lockToPrintPlatform;
    }

}

float EditMeshZoomTransformTool::getRadio(const CHAABB3D& aabb, const CHAABB3D& platformAabb)
{
    float radio = 0.0;
        float radioX = 0.0;
        float radioY = 0.0;
        float radioZ = 0.0;

        if (aabb.getLenX() < platformAabb.getLenX() &&
            aabb.getLenY() < platformAabb.getLenY() &&
            aabb.getLenZ() < platformAabb.getLenZ())
        {
            radioX = 2 * aabb.getCenterPoint().x() / aabb.getLenX();
            radioX = (radioX < (2 * (platformAabb.m_Xmax - aabb.getCenterPoint().x()) / aabb.getLenX()) ? radioX :
                (2 * (platformAabb.m_Xmax - aabb.getCenterPoint().x()) / aabb.getLenX()));

            radioY = 2 * aabb.getCenterPoint().y() / aabb.getLenY();
            radioY = (radioY < (2 * (platformAabb.m_Ymax - aabb.getCenterPoint().y()) / aabb.getLenY()) ? radioY :
                (2 * (platformAabb.m_Ymax - aabb.getCenterPoint().y()) / aabb.getLenY()));

            radioZ = 2 * (platformAabb.m_Zmax - aabb.getCenterPoint().z()) / aabb.getLenZ();

            radio = (radio = (radioX < radioY ? radioX : radioY)) < radioZ ? radio : radioZ; 
            //radio -= 0.1;
        }
        else
        {
            radio = 1.0;
        }
        return radio;
}

float EditMeshZoomTransformTool::getTimes(const CHAABB3D &aabb, const CHAABB3D &platformAabb)
{
    float ratio = 1.0;
    float ratioX = (platformAabb.getLenX() * 0.99)/ aabb.getLenX(); 
    float ratioY = (platformAabb.getLenY() * 0.99)/ aabb.getLenY();
    float ratioZ = (platformAabb.getLenZ() * 0.99)/ aabb.getLenZ();
    ratio = ((ratioX < ratioY) ? ratioX : ratioY);
    ratio = ((ratio < ratioZ) ? ratio : ratioZ);
    if(fabs(ratio - 1.0) < 0.01)
    {
        ratio = 1.0;
    }
    return ratio;
}




void EditMeshZoomTransformTool::mousePressEvent(QMouseEvent* event, void*, void*)
{
    AkUtil::TFunction("");
    if (event->button() == Qt::LeftButton && m_pickedObj)
    {
        event->accept();
        m_stepFlag = 1;
        int i = 0;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            m_values[i][0] = (*it)->m_params[0];
            m_values[i][1] = (*it)->m_params[1];
            m_values[i][2] = (*it)->m_params[2];
            m_values[i][3] = (*it)->m_params[6];
            m_values[i][4] = (*it)->m_params[7];
            m_values[i][5] = (*it)->m_params[8];
            (*it)->calRealAABB();

            i++;
        }
        refreshScaleFrame();
    }
}

void EditMeshZoomTransformTool::mouseMoveEvent(QMouseEvent* event, void*, void*)
{
    if (m_stepFlag == 0)
    {
        
        CHShowObjPtr curPickedObj = 0;
        double pickTol = 10;
        vector<PickResult> curPickerObjs;
        for (int i = 0; i < m_allPickObjs.size(); i++)
        {
            PickResult result;
            if (m_allPickObjs[i]->pick(event->pos().x(), event->pos().y(), result, pickTol))
            {
                curPickerObjs.push_back(result);
            }
        }
        int num = (int)curPickerObjs.size();
        if (num == 1)
        {
            event->accept();
            curPickedObj = curPickerObjs[0].m_pickObj;

            m_pickCoord = curPickerObjs[0].m_ptOnObj;
        }
        else if (num > 1)
        {
            event->accept();
            std::sort(curPickerObjs.begin(), curPickerObjs.end(), CHShowObj::pred1);
            double ttol = 0.001;
            vector<PickResult> minDeepObjs;
            minDeepObjs.push_back(curPickerObjs[0]);
            for (int i = 1; i < num; i++)
            {
                if (fabs(curPickerObjs[i].m_deep - curPickerObjs[0].m_deep) < ttol)
                {
                    minDeepObjs.push_back(curPickerObjs[i]);
                }
            }

            std::sort(minDeepObjs.begin(), minDeepObjs.end(), CHShowObj::pred2);
            curPickedObj = minDeepObjs[0].m_pickObj;

            m_pickCoord = minDeepObjs[0].m_ptOnObj;
        }

        if (m_pickedObj != curPickedObj)
        {
            for (int i = 0; i < m_allPickObjs.size(); i++)
            {
                m_allPickObjs[i]->setStatus(general);
            }
            m_pickedObj = curPickedObj;
            if (m_pickedObj)
            {
                m_pickedObj->setStatus(canPicked);
            }
            curScene->refresh();
        }
    }
    else if (m_stepFlag == 1) 
    {
        event->accept();
        bool isWidget = curScene->getCurMouseInWidget(event->pos().x(), event->pos().y());
        if(!isWidget)
        {
            m_stepFlag = 0;
            m_pickedObj = 0;
            return;
        }
        QVector3D np, fp;
        curScene->getCurNearFarPoint(event->pos().x(), event->pos().y(), np, fp);
        CHLine3DPtr ray(new CHLine3D);
        ray->create(np, fp);

        
        QVector3D origin = TransformPack::pRot(m_adjustAxisX->getTransform(), m_adjustAxisX->m_st);

        
        CHLine3DPtr line(new CHLine3D);
        int index = 0;
        if (m_pickedObj == m_adjustXPoint)
        {
            line->create(origin, TransformPack::pRot(m_markXPoint->getTransform(), m_markXPoint->m_coord));
            index = 0;
        }
        else if (m_pickedObj == m_adjustYPoint)
        {
            line->create(origin, TransformPack::pRot(m_markYPoint->getTransform(), m_markYPoint->m_coord));
            index = 1;
        }
        else if (m_pickedObj == m_adjustZPoint)
        {
            line->create(origin, TransformPack::pRot(m_markZPoint->getTransform(), m_markZPoint->m_coord));
            index = 2;
        }
        else
        {
            line->create(origin, m_pickCoord);
            index = 3;
        }
        //line->create(origin, m_pickCoord);
        LineLineExtremeResult result;
        CHBaseAlg::instance()->extremeBetweenLineAndLine(line, ray, result);
        double minRatio = 0.05;
        if (result.m_u1 < minRatio)
        {
            result.m_u1 = minRatio;
        }
        else if (fabs(result.m_u1 - 1) < minRatio)
        {
            result.m_u1 = 1;
        }

        {
            int i = 0;
            for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            {
                
                if (m_lockScaleRatio || index == 3)
                {
                    (*it)->m_params[0] = m_values[i][0] * result.m_u1;
                    (*it)->m_params[1] = m_values[i][1] * result.m_u1;
                    (*it)->m_params[2] = m_values[i][2] * result.m_u1;
                }
                else
                {
                    (*it)->m_params[index] = m_values[i][index] * result.m_u1;
                }
//                if(index == 3)
//                {
//                    index = 0;
//                }
                
                QVector3D vecDir = (*it)->m_rotCenter + QVector3D(m_initValues[i][3], m_initValues[i][4], m_initValues[i][5]) - m_operationCenter;
                QVector3D newCenter;
                if(index == 3)
                {
                    newCenter = vecDir /** fabs((*it)->m_params[0] / m_initValues[i][0])*/ + m_operationCenter + QVector3D(0, 0, m_operateMoveZ);
                }
                else
                {
                    newCenter = vecDir /** fabs((*it)->m_params[index] / m_initValues[i][index]) */+ m_operationCenter + QVector3D(0, 0, m_operateMoveZ);
                }
                (*it)->m_params[6] = newCenter[0] - (*it)->m_rotCenter[0];
                (*it)->m_params[7] = newCenter[1] - (*it)->m_rotCenter[1];
                (*it)->m_params[8] = newCenter[2] - (*it)->m_rotCenter[2];

                i++;
            }
        }

        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
            }
            CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
            if (printModel != nullptr)
            {
                for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
                {
                    SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                    QVector3D position, scale;
                    QQuaternion orientation;
                    double pitch, yaw, roll;
                    if (supportMesh != nullptr)
                    {
                        supportMesh->setTransform(printModel->getTransform() * supportMesh->getLocalTransform());
                        
                        AkTransformMath::decomposeQMatrix4x4(supportMesh->getTransform(), position, orientation, scale);
                        QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
                        CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
                        
                        pitch = pitch / CH_PI * 180.0;
                        yaw = yaw / CH_PI * 180.0;
                        roll = roll / CH_PI * 180.0;
                        supportMesh->m_params[0] = scale[0];
                        supportMesh->m_params[1] = scale[1];
                        supportMesh->m_params[2] = scale[2];
                        supportMesh->m_params[3] = pitch;
                        supportMesh->m_params[4] = yaw;
                        supportMesh->m_params[5] = roll;

                        QVector3D newCenter = supportMesh->getTransform() * supportMesh->m_rotCenter;
                        QVector3D offset = newCenter - supportMesh->m_rotCenter;
                        supportMesh->m_params[6] = offset[0];
                        supportMesh->m_params[7] = offset[1];
                        supportMesh->m_params[8] = offset[2];
                    }
                }
            }
        }

        
        refreshScaleFrame();
        curScene->refresh();
        vector<double > params(3);
        params[0] = (*m_editMeshModels.begin())->m_params[0] / m_initValues[0][0];
        params[1] = (*m_editMeshModels.begin())->m_params[1] / m_initValues[0][1];
        params[2] = (*m_editMeshModels.begin())->m_params[2] / m_initValues[0][2];
        vector<double > boxSize(3);
        boxSize[0] = m_initBox[0] * params[0];
        boxSize[1] = m_initBox[1] * params[1];
        boxSize[2] = m_initBox[2] * params[2];
        emit boxSizeParamsChanged(boxSize, ZoomChangedType_View);
    }
}

void EditMeshZoomTransformTool::mouseReleaseEvent(QMouseEvent* event, void*, void*)
{
    AkUtil::TFunction("");
    if (event->button() == Qt::LeftButton/* && m_pickedObj*/)
    {
       stickOnBottom();

       refreshScaleFrame();
    }
}

void EditMeshZoomTransformTool::receiveBoxParams(std::vector<float> params, ZoomAxisType axisType)
{
    vector<float> scaleParams(3);
    for (int i = 0; i < scaleParams.size(); i++)
    {
        scaleParams[i] = params[i] / m_initBox[i];
    }

    receiveParams(scaleParams, axisType);
}

void EditMeshZoomTransformTool::receiveParams(std::vector<float> params, ZoomAxisType axisType)
{
    if (m_stepFlag == 1)
    {
        return;
    }

    //qDebug() << "axisType: " << axisType << ", m_operationCenter: " << m_operationCenter << ", m_operateMoveZ: " << m_operateMoveZ;
    CHAABB3D aabb;
    CHAABB3D baseAabb; 
    int k = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        for (int p = 0; p < 3; p++)
        {
            (*it)->m_params[p] = m_initValues[k][p] * params[p];
        }
        
        QVector3D vecDir = QVector3D((*it)->m_rotCenter[0], (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]) +
            QVector3D(m_initValues[k][3], m_initValues[k][4], m_initValues[k][5]) - m_operationCenter;
        QVector3D newCenter = vecDir  + m_operationCenter + QVector3D(0, 0, m_operateMoveZ);
        (*it)->m_params[6] = newCenter[0] - (*it)->m_rotCenter[0];
        (*it)->m_params[7] = newCenter[1] - (*it)->m_rotCenter[1];
        (*it)->m_params[8] = newCenter[2] - (*it)->m_rotCenter[2];

        
        (*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams((*it)->m_rotCenter, (*it)->m_params));

        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
        if (printModel != nullptr)
        {
            for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                if (supportMesh != nullptr)
                {
                    supportMesh->setTransform(printModel->getTransform() * supportMesh->getLocalTransform());
                    
                    AkTransformMath::decomposeQMatrix4x4(supportMesh->getTransform(), position, orientation, scale);
                    QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
                    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
                    
                    pitch = pitch / CH_PI * 180.0;
                    yaw = yaw / CH_PI * 180.0;
                    roll = roll / CH_PI * 180.0;
                    supportMesh->m_params[0] = scale[0];
                    supportMesh->m_params[1] = scale[1];
                    supportMesh->m_params[2] = scale[2];
                    supportMesh->m_params[3] = pitch;
                    supportMesh->m_params[4] = yaw;
                    supportMesh->m_params[5] = roll;

                    QVector3D newCenter = supportMesh->getTransform() * supportMesh->m_rotCenter;
                    QVector3D offset = newCenter - supportMesh->m_rotCenter;
                    supportMesh->m_params[6] = offset[0];
                    supportMesh->m_params[7] = offset[1];
                    supportMesh->m_params[8] = offset[2];
                }
            }
        }
        aabb.add((*it)->calRealAABB());
        baseAabb.add((*it)->m_baseAABB);
        k++;
    }
    
    if (m_lockToPrintPlatform)
    {
        float moveZ = -aabb.m_Zmin;
        m_operateMoveZ += moveZ;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                continue;
            }
            (*it)->m_params[8] = (*it)->m_params[8] + moveZ;

            
            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams((*it)->m_rotCenter, (*it)->m_params);

            
            (*it)->setTransform(sumtran1);

            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
            }
            CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
            if (printModel != nullptr)
            {
                for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
                {
                    SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                    QVector3D position, scale;
                    QQuaternion orientation;
                    double pitch, yaw, roll;
                    if (supportMesh != nullptr)
                    {
                        supportMesh->setTransform(printModel->getTransform() * supportMesh->getLocalTransform());
                        
                        AkTransformMath::decomposeQMatrix4x4(supportMesh->getTransform(), position, orientation, scale);
                        QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
                        CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
                        
                        pitch = pitch / CH_PI * 180.0;
                        yaw = yaw / CH_PI * 180.0;
                        roll = roll / CH_PI * 180.0;
                        supportMesh->m_params[0] = scale[0];
                        supportMesh->m_params[1] = scale[1];
                        supportMesh->m_params[2] = scale[2];
                        supportMesh->m_params[3] = pitch;
                        supportMesh->m_params[4] = yaw;
                        supportMesh->m_params[5] = roll;

                        QVector3D newCenter = supportMesh->getTransform() * supportMesh->m_rotCenter;
                        QVector3D offset = newCenter - supportMesh->m_rotCenter;
                        supportMesh->m_params[6] = offset[0];
                        supportMesh->m_params[7] = offset[1];
                        supportMesh->m_params[8] = offset[2];
                    }
                }
            }

        }
    }
    //m_operationCenter = aabb.getCenterPoint();


    AkUtil::TDebug("Calc Current: (" + QString::number(aabb.getLenX()) + ", " +
                   QString::number(aabb.getLenY()) + ", " + QString::number(aabb.getLenZ()));
    if(m_currentType == ZoomChangedType_Scale || m_currentType == ZoomChangedType_ScaleToFit)
    {
        vector<double > boxSize(3);
        if(m_editMeshModels.size() > 1)
        {
            CHAABB3D realAABB;
            for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            {
                realAABB.add((*it)->calRealAABB());
            }

            boxSize[0] = realAABB.getLenX() * ((*m_editMeshModels.begin())->m_params[0] > 0 ? 1 : -1);
            boxSize[1] = realAABB.getLenY() * ((*m_editMeshModels.begin())->m_params[1] > 0 ? 1 : -1);
            boxSize[2] = realAABB.getLenZ() * ((*m_editMeshModels.begin())->m_params[2] > 0 ? 1 : -1);
        }
        else if(m_editMeshModels.size() == 1)
        {
//            std::vector<double> tmpParams(3);
//            for(int i = 0; i < 3; i++)
//            {
//                double tmp = (*m_editMeshModels.begin())->m_params[i];
//                tmpParams[i] = tmp > 0 ? tmp : -tmp;
//            }

            boxSize[0] = (*m_editMeshModels.begin())->m_baseAABB.getLenX() * (*m_editMeshModels.begin())->m_params[0];
            boxSize[1] = (*m_editMeshModels.begin())->m_baseAABB.getLenY() * (*m_editMeshModels.begin())->m_params[1];
            boxSize[2] = (*m_editMeshModels.begin())->m_baseAABB.getLenZ() * (*m_editMeshModels.begin())->m_params[2];
        }
        //qDebug() << "boxSize: " << boxSize[0] << ", " << boxSize[1] << ", " << boxSize[2];
        emit boxSizeParamsChanged(boxSize, m_currentType);

        if(m_currentType == ZoomChangedType_ScaleToFit)
        {
            if(m_editMeshModels.size() == 1)
            {
                std::vector<double> scaleParams(3);
                scaleParams[0] = (*m_editMeshModels.begin())->m_params[0];
                scaleParams[1] = (*m_editMeshModels.begin())->m_params[1];
                scaleParams[2] = (*m_editMeshModels.begin())->m_params[2];

                emit scaleParamsChanged(scaleParams, ZoomChangedType_ScaleToFit);
            }
            else if(m_editMeshModels.size() > 1)
            {
                std::vector<double> scaleParams(3);
                scaleParams[0] = params[0];
                scaleParams[1] = params[1];
                scaleParams[2] = params[2];

                emit scaleParamsChanged(scaleParams, ZoomChangedType_ScaleToFit);
            }
        }
    }
    emit getDoc()->modelCheckSceneInChanged();
    
    refreshScaleFrame();
    curScene->refresh();
}

void EditMeshZoomTransformTool::viewValuesChanged(std::vector<double> params, ZoomChangedType type, ZoomAxisType axisType)
{
    m_currentType = type;
    if(params.size() == 3)
    {
        std::vector<float> values(3);
        if(m_editMeshModels.size() == 1)
        {
            values[0] = params[0] / m_initValues[0][0];
            values[1] = params[1] / m_initValues[0][1];
            values[2] = params[2] / m_initValues[0][2];
        }
        else if(m_editMeshModels.size() > 1)
        {
            values[0] = params[0];
            values[1] = params[1];
            values[2] = params[2];
        }
        //qDebug() << "m_currentType: " << m_currentType << ", params: " << params[0] << " " << params[1] << " " << params[2] <<
        //            ", values: " << values[0] << " " << values[1] << " " << values[2];
        receiveParams(values, axisType);
    }
}

void EditMeshZoomTransformTool::refreshBoxSizeUI()
{
    //submitToUI();
}

void EditMeshZoomTransformTool::createFrame(const QVector3D &originCreateCenter, float len, float width, float height)
{
    m_operationCenter = originCreateCenter;
    m_adjustAxisX->create(originCreateCenter, originCreateCenter + QVector3D(len, 0, 0));
    m_adjustAxisY->create(originCreateCenter, originCreateCenter + QVector3D(0, width, 0));
    m_adjustAxisZ->create(originCreateCenter, originCreateCenter + QVector3D(0, 0, height));
    m_markXPoint->create(m_adjustAxisX->m_et);
    m_markYPoint->create(m_adjustAxisY->m_et);
    m_markZPoint->create(m_adjustAxisZ->m_et);

    m_adjustXPoint->create(m_adjustAxisX->m_et);
    m_adjustYPoint->create(m_adjustAxisY->m_et);
    m_adjustZPoint->create(m_adjustAxisZ->m_et);
    m_adjustXYZPoint->create(m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
    m_middleshowcurve1->create(m_adjustAxisY->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);
    m_middleshowcurve2->create(m_adjustAxisX->m_et, m_adjustAxisX->m_et + m_adjustAxisY->m_et - originCreateCenter);

    
    refreshScaleFrame();
    m_adjustXPoint->updateToScene();
    m_adjustYPoint->updateToScene();
    m_adjustZPoint->updateToScene();
    m_adjustXYZPoint->updateToScene();
    m_adjustAxisX->updateToScene();
    m_adjustAxisY->updateToScene();
    m_adjustAxisZ->updateToScene();
    m_middleshowcurve1->updateToScene();
    m_middleshowcurve2->updateToScene();
    m_markXPoint->updateToScene();
    m_markYPoint->updateToScene();
    m_markZPoint->updateToScene();
}

void EditMeshZoomTransformTool::stickOnBottom()
{
    
    CHAABB3D aabb;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        aabb.add((*it)->calRealAABB());
    }
    
    if (m_lockToPrintPlatform&& !std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()))
    {
        float moveZ = -aabb.m_Zmin;
        m_operateMoveZ += moveZ;

        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            (*it)->m_params[8] += moveZ;

            
            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);

        }
    }
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
        if (printModel != nullptr)
        {
            for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                if (supportMesh != nullptr)
                {
                    supportMesh->setTransform(printModel->getTransform() * supportMesh->getLocalTransform());
                    
                    AkTransformMath::decomposeQMatrix4x4(supportMesh->getTransform(), position, orientation, scale);
                    QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
                    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
                    
                    pitch = pitch / CH_PI * 180.0;
                    yaw = yaw / CH_PI * 180.0;
                    roll = roll / CH_PI * 180.0;
                    supportMesh->m_params[0] = scale[0];
                    supportMesh->m_params[1] = scale[1];
                    supportMesh->m_params[2] = scale[2];
                    supportMesh->m_params[3] = pitch;
                    supportMesh->m_params[4] = yaw;
                    supportMesh->m_params[5] = roll;

                    QVector3D newCenter = supportMesh->getTransform() * supportMesh->m_rotCenter;
                    QVector3D offset = newCenter - supportMesh->m_rotCenter;
                    supportMesh->m_params[6] = offset[0];
                    supportMesh->m_params[7] = offset[1];
                    supportMesh->m_params[8] = offset[2];
                }
            }
            //printModel->isSceneIn(printModel->calRealAABB(), getDoc()->m_machineBox->getBaseAABB());
        }

    }

    refreshScaleFrame();
    emit getDoc()->modelCheckSceneInChanged();
    m_stepFlag = 0;
    m_pickedObj = 0;
    for (int i = 0; i < m_allPickObjs.size(); i++)
    {
        m_allPickObjs[i]->setStatus(general);
    }
    curScene->refresh();
}

void EditMeshZoomTransformTool::resetSelectedScale()
{
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        //(*it)->resetZoom();
        (*it)->resetTransform();
        
        //(*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams((*it)->m_rotCenter, (*it)->m_params));

        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
        if (printModel != nullptr)
        {
            for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                if (supportMesh != nullptr)
                {
                    supportMesh->setTransform(printModel->getTransform() * supportMesh->getLocalTransform());
                    
                    AkTransformMath::decomposeQMatrix4x4(supportMesh->getTransform(), position, orientation, scale);
                    QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
                    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
                    
                    pitch = pitch / CH_PI * 180.0;
                    yaw = yaw / CH_PI * 180.0;
                    roll = roll / CH_PI * 180.0;
                    supportMesh->m_params[0] = scale[0];
                    supportMesh->m_params[1] = scale[1];
                    supportMesh->m_params[2] = scale[2];
                    supportMesh->m_params[3] = pitch;
                    supportMesh->m_params[4] = yaw;
                    supportMesh->m_params[5] = roll;

                    QVector3D newCenter = supportMesh->getTransform() * supportMesh->m_rotCenter;
                    QVector3D offset = newCenter - supportMesh->m_rotCenter;
                    supportMesh->m_params[6] = offset[0];
                    supportMesh->m_params[7] = offset[1];
                    supportMesh->m_params[8] = offset[2];
                }
            }
        }
        (*it)->calRealAABB();
    }
}

void EditMeshZoomTransformTool::refreshScaleFrame()
{
    if (m_adjustXPoint == nullptr || m_adjustYPoint == nullptr || m_adjustZPoint == nullptr)
        return;
    {
        CHAABB3D realAABB, baseAABB;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            realAABB.add((*it)->m_realAABB);
            baseAABB.add((*it)->m_baseAABB);
        }

        float ratioX = realAABB.getLenX() / m_initBox[0];
        float ratioY = realAABB.getLenY() / m_initBox[1];
        float ratioZ = realAABB.getLenZ() / m_initBox[2];

        float ratio = 1.0;
        ratio = (ratioX * ratioY * ratioZ) * 3.0;
        double zoomXValue = m_adjustAxisX->getLineLength() * ratio;
        double zoomYValue = m_adjustAxisY->getLineLength() * ratio;
        double zoomZValue = m_adjustAxisZ->getLineLength() * ratio;
        double machineBoxX = getDoc()->m_machineBox->m_realAABB.getLenX();
        double machineBoxY = getDoc()->m_machineBox->m_realAABB.getLenY();
        double machineBoxZ = getDoc()->m_machineBox->m_realAABB.getLenZ();
        if(fabs(zoomXValue) > machineBoxX ||
                fabs(zoomYValue) > machineBoxY ||
                fabs(zoomZValue) > machineBoxZ)
        {
            ratio = machineBoxX / m_adjustAxisX->getLineLength();
        }
        QVector3D position, scale;
        QQuaternion orientation;
        AkTransformMath::decomposeQMatrix4x4(m_firstMesh->getTransform(), position, orientation, scale);
        QVector3D center = realAABB.getCenterPoint();
        QMatrix4x4 tran1, tran2, tran3, tran4, tran5;
        tran1.translate(-center);
        tran2.scale(ratio, ratio, ratio);
         tran3.rotate(orientation);
        tran4.translate(center);
        QMatrix4x4 tmpTransform = tran4 * tran3  * tran2 * tran1;

        QMatrix4x4 sumtran1 = tmpTransform;
        m_adjustXPoint->setTransform(sumtran1);
        m_adjustYPoint->setTransform(sumtran1);
        m_adjustZPoint->setTransform(sumtran1);
        m_adjustXYZPoint->setTransform(sumtran1);
        m_middleshowcurve1->setTransform(sumtran1);
        m_middleshowcurve2->setTransform(sumtran1);

        m_adjustAxisX->setTransform(sumtran1);
        m_adjustAxisY->setTransform(sumtran1);
        m_adjustAxisZ->setTransform(sumtran1);

        m_markXPoint->setTransform(sumtran1);
        m_markYPoint->setTransform(sumtran1);
        m_markZPoint->setTransform(sumtran1);


        QVector3D offset = center - m_adjustAxisX->getCurrentStPoint(); 
        tran5.translate(offset);
        QMatrix4x4 sumtran2 = tran5 * sumtran1; 
        m_adjustXPoint->setTransform(sumtran2);
        m_adjustYPoint->setTransform(sumtran2);
        m_adjustZPoint->setTransform(sumtran2);
        m_adjustXYZPoint->setTransform(sumtran2);
        m_middleshowcurve1->setTransform(sumtran2);
        m_middleshowcurve2->setTransform(sumtran2);

        m_adjustAxisX->setTransform(sumtran2);
        m_adjustAxisY->setTransform(sumtran2);
        m_adjustAxisZ->setTransform(sumtran2);

        m_markXPoint->setTransform(sumtran2);
        m_markYPoint->setTransform(sumtran2);
        m_markZPoint->setTransform(sumtran2);
    }
}

void EditMeshZoomTransformTool::submitToUI()
{
    vector<float> params(3);
    CHMeshShowObjPtr mesh = *m_editMeshModels.begin();
    /*

    {

        params[0] = mesh->m_params[0];
        params[1] = mesh->m_params[1];
        params[2] = mesh->m_params[2];
    }
    else   */
    {
        params[0] = mesh->m_params[0] / m_initValues[0][0];
        params[1] = mesh->m_params[1] / m_initValues[0][1];
        params[2] = mesh->m_params[2] / m_initValues[0][2];
    }
    emit sendParams(params);

    vector<float > boxSize(3);
    boxSize[0] = m_initBox[0] * params[0];
    boxSize[1] = m_initBox[1] * params[1];
    boxSize[2] = m_initBox[2] * params[2];
    emit sendBoxSizeParam(boxSize);
}


