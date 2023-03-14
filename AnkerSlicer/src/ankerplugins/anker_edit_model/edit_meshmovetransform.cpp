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
#include "edit_meshmovetransform.h"
#include <wrap/qt/gl_label.h>
#include <wrap/gui/trackball.h>
#include "wrap/gl/space.h"
#include "common/GeoAndShow/TransformPack.h"
#include "common/GeoAndShow/CHLine3D.h"
#include "common/GeoAndShow/CHBaseAlg.h"
#include "QPalette"
#include "edit_meshtransform_factory.h"

#include "QOpenGLFramebufferObject"
#include "common/GeoAndShow/AkTransformMath.h"
#include "common/GeoAndShow/support/SupportAssemblyMeshes.h"
#include "common/utilities/tlogger.h"

using namespace vcg;

CHAxisWithArrow::CHAxisWithArrow()
{
    AkUtil::TFunction("");
}

CHAxisWithArrow::~CHAxisWithArrow()
{
}

void CHAxisWithArrow::create(QVector3D center, QVector3D axisDir, float axisLenth, float rad, float height)
{
    AkUtil::TFunction("");
    setDirty(true);

    QVector3D et = center + axisDir * axisLenth;
    m_axis = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_axis->create(center, et);

    m_arrow = CHConePtr(new CHCone);
    m_arrow->create(rad, height);
    QMatrix4x4 matrix1 = TransformPack::rotMat(QVector3D(0, 0, 1), axisDir, QVector3D(0, 0, 0));
    QMatrix4x4 matrix2;
    matrix2.translate(et);
    m_arrow->transform(matrix2 * matrix1);

    m_baseShowObjs.push_back(m_axis);
    m_baseShowObjs.push_back(m_arrow);
}

bool CHAxisWithArrow::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
    m_axis->setTransform(getTransform());
    if (!m_axis->pick(pixelX, pixelY, result, pickTol))
    {
        return false;
    }
    result.m_pickObj = shared_from_this();
    return true;
}

EditMeshMoveTransformTool::EditMeshMoveTransformTool()
{
    AkUtil::TFunction("");
    m_paramUI = 0;
    m_stepFlag = 0;
    //m_meshModel = 0;
    //m_CoordZToZero = true;
}

bool EditMeshMoveTransformTool::startAnkerEdit(ActionEditTool * action, void * arg1, void *arg2)
{
    AkUtil::TFunction("");
    if(m_isActivated){return false;}
    m_isActivated = true;

#if 0  
    curScene = gla->m_scene;
    SupportMeshFactory inst;
    QList<CHShowObjPtr> objs = inst.GenerateSupportMeshes(SupportData::instance()->GetSupportLines());
    for (int i = 0; i < objs.size(); i++)
    {
        objs[i]->updateToScene();
    }
#endif

    curScene = static_cast<CHScene*>(arg1);
    if (curScene->m_pickCommand->m_selectedObjs.size() == 0)
    {
        return false;
    }

    m_editMeshModels = curScene->m_pickCommand->m_selectedObjs;
    m_firstMesh = *m_editMeshModels.begin();

    m_stepFlag = 0;
    m_pickedObj = 0;

    m_paramUI = new CHModelMoveTransformParamsSetUI();
    EditMeshTransformFactory::m_conInt->addWidgetToModelTransForm(m_paramUI, AkConst::FDMMeshTransForm::Move);
    connect(this, &EditMeshMoveTransformTool::sendParamsToGui, m_paramUI, &CHModelMoveTransformParamsSetUI::receiveParams);
    connect(m_paramUI, &CHModelMoveTransformParamsSetUI::sendParams, this, &EditMeshMoveTransformTool::receiveParamsFormGui);
    connect(m_paramUI, &CHModelMoveTransformParamsSetUI::reset, this, &EditMeshMoveTransformTool::resetBtnClicked);
    connect(m_paramUI, &CHModelMoveTransformParamsSetUI::lockToPrintPlatformStatusChanged, this, &EditMeshMoveTransformTool::lockToPrintPlatformStatusChanged);
    
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        EditMeshTransformFactory::initMeshModelEulerParams(*it);
    }
    m_values.resize(m_editMeshModels.size());
    m_initValues.resize(m_editMeshModels.size());
    int p = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        m_values[p].resize(3);
        m_initValues[p].resize(3);
        m_initValues[p][0] = (*it)->m_params[6];
        m_initValues[p][1] = (*it)->m_params[7];
        m_initValues[p][2] = (*it)->m_params[8];
        p++;
    }

    
    CHAABB3D aabb;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        if (std::dynamic_pointer_cast<SupportAssemblyMeshes>(*it) != nullptr)
        {
            aabb.add((*it)->calRealAABB());
        }
        else
        {
            aabb.add((*it)->calSimilarAABB());
        }
    }
    m_operationCenter = aabb.getCenterPoint();

    if(m_editMeshModels.size() == 1)
    {
        
        
        std::vector<double> params(3);
        params[0] = m_firstMesh->m_params[6] + m_firstMesh->m_rotCenter[0];
        params[1] = m_firstMesh->m_params[7] + m_firstMesh->m_rotCenter[1];
        params[2] = m_firstMesh->m_params[8] + m_firstMesh->m_rotCenter[2] - m_firstMesh->m_baseAABB.getLenZ() / 2;
        emit sendParamsToGui(params);
    }
    else
    {
        std::vector<double> params(3);
        params[0] = m_firstMesh->m_params[6] - m_initValues[0][0];
        params[1] = m_firstMesh->m_params[7] - m_initValues[0][1];
        params[2] = m_firstMesh->m_params[8] - m_initValues[0][2];
        emit sendParamsToGui(params);
    }

    lockToPrintPlatformStatusChanged();

    
    double ratio = 0.5;
    double maxLenth = max(max(aabb.getLenX(), aabb.getLenY()), aabb.getLenZ()) * ratio;
    //QVector3D originCreateCenter(m_meshModel->m_rotCenter[0], m_meshModel->m_rotCenter[1], m_meshModel->m_rotCenter[2]);
    QVector3D originCreateCenter = m_operationCenter;
    float trad = 1.5;
    float theight = 11;
    m_adjustAxisX = CHAxisWithArrowPtr(new CHAxisWithArrow);
    m_adjustAxisX->create(originCreateCenter, QVector3D(1, 0, 0), maxLenth, trad, theight);
    m_adjustAxisX->setColor(QColor(125, 0, 0));
    m_adjustAxisX->setCalLight(false);
    m_adjustAxisY = CHAxisWithArrowPtr(new CHAxisWithArrow);
    m_adjustAxisY->create(originCreateCenter, QVector3D(0, 1, 0), maxLenth, trad, theight);
    m_adjustAxisY->setColor(QColor(0, 125, 0));
    m_adjustAxisY->setCalLight(false);
    m_adjustAxisZ = CHAxisWithArrowPtr(new CHAxisWithArrow);
    m_adjustAxisZ->create(originCreateCenter, QVector3D(0, 0, 1), maxLenth, trad, theight);
    m_adjustAxisZ->setColor(QColor(0, 0, 125));
    m_adjustAxisZ->setCalLight(false);
    m_adjustOrigin = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustOrigin->create(originCreateCenter);
    m_adjustOrigin->setColor(QColor(125, 0, 125));
    m_adjustOrigin->setSize(8);

    
    refreshMoveFrame();

    m_allPickObjs.push_back(m_adjustAxisX);
    m_allPickObjs.push_back(m_adjustAxisY);
    m_allPickObjs.push_back(m_adjustAxisZ);
    m_allPickObjs.push_back(m_adjustOrigin);

    for (int i = 0; i < m_allPickObjs.size(); i++)
    {
        m_allPickObjs[i]->setLightTest(false);
        /*if (dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i]))
        {
            dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i])->injectPropertiesToChildren();
        }*/
        m_allPickObjs[i]->updateToScene();
    }

    return true;
}

void EditMeshMoveTransformTool::endAnkerEdit  (ActionEditTool *, void *, void *)
{
    AkUtil::TFunction("");
    if(!m_isActivated){return;}
    m_isActivated = false;

    if (m_paramUI)
    {
        delete m_paramUI;
        m_paramUI = 0;
    }

    for (int i = 0; i < m_allPickObjs.size(); i++)
    {
        m_allPickObjs[i]->setDelete(true);
        /*if (dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i]))
        {
            dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i])->injectPropertiesToChildren();
        }*/
        m_allPickObjs[i]->updateToScene();
    }

    m_allPickObjs.clear();
}

void EditMeshMoveTransformTool::mousePressEvent(QMouseEvent  *event, void *, void *)
{
    AkUtil::TFunction("");
    if (event->button() == Qt::LeftButton && m_pickedObj)
    {
        event->accept();
        m_stepFlag = 1;

        int i = 0;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            m_values[i][0] = (*it)->m_params[6];
            m_values[i][1] = (*it)->m_params[7];
            m_values[i][2] = (*it)->m_params[8];

            i++;
        }
    }
}

void EditMeshMoveTransformTool::mouseMoveEvent(QMouseEvent  *event, void *, void *)
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
                /*if (dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i]))
                {
                    dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i])->injectPropertiesToChildren();
                }*/
            }
            m_pickedObj = curPickedObj;
            if (m_pickedObj)
            {
                m_pickedObj->setStatus(canPicked);
                /*if (dynamic_pointer_cast<CHAssembly>(m_pickedObj))
                {
                    dynamic_pointer_cast<CHAssembly>(m_pickedObj)->injectPropertiesToChildren();
                }*/
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

        if (dynamic_pointer_cast<CHPointShowObj>(m_pickedObj))
        {
            
            QVector3D dir = ray->m_et - ray->m_st;
            float m = QVector3D::dotProduct(dir, QVector3D(0, 0, 1));
            if (fabs(m) > 0.0000001)
            {
                float t = QVector3D::dotProduct(m_pickCoord - ray->m_st, QVector3D(0, 0, 1)) / m;
                QVector3D intersection = ray->m_st + dir * t;

                int i = 0;
                for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
                {
                    (*it)->m_params[6] = m_values[i][0] + intersection[0] - m_pickCoord[0];
                    (*it)->m_params[7] = m_values[i][1] + intersection[1] - m_pickCoord[1];
                    (*it)->m_params[8] = m_values[i][2] + intersection[2] - m_pickCoord[2];
                    i++;
                }
            }
        }
        else
        {
            CHLineSegment3DShowObjPtr moveLine = dynamic_pointer_cast<CHAxisWithArrow>(m_pickedObj)->m_axis;

            
            CHLine3DPtr line(new CHLine3D);
            line->create(TransformPack::pRot(moveLine->getTransform(), moveLine->m_st),
                TransformPack::pRot(moveLine->getTransform(), moveLine->m_et));

            LineLineExtremeResult result;
            CHBaseAlg::instance()->extremeBetweenLineAndLine(line, ray, result);

            int i = 0;
            for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            {
                (*it)->m_params[6] = m_values[i][0] + result.m_pt1[0] - m_pickCoord[0];
                (*it)->m_params[7] = m_values[i][1] + result.m_pt1[1] - m_pickCoord[1];
                (*it)->m_params[8] = m_values[i][2] + result.m_pt1[2] - m_pickCoord[2];
                i++;
            }
        }

        submitToUI();
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);
            qDebug() << "move params: " << (*it)->m_params[6] << ", " << (*it)->m_params[7] << ", " << (*it)->m_params[8];
            
            (*it)->setTransform(sumtran1);
            (*it)->calRealAABB();
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                operateType = 1;
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
            }
            CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
            if (printModel != nullptr)
            {
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
                {
                    SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
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

        
        refreshMoveFrame();

        curScene->refresh();
    }
}

void EditMeshMoveTransformTool::mouseReleaseEvent(QMouseEvent  *event, void *, void *)
{
    AkUtil::TFunction("");
    if (event->button() == Qt::LeftButton && m_pickedObj)
    {
        CHAABB3D aabb;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            aabb.add((*it)->calRealAABB());
        }
        
        if (m_lockToPrintPlatform && !std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()))
        {
            
            //for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            //{
            //	aabb.add((*it)->calRealAABB());
            //}
            float moveZ = -aabb.m_Zmin;

            for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            {
                (*it)->m_params[8] = (*it)->m_params[8] + moveZ;

                //?????????
                (*it)->m_realAABB.m_Zmin += moveZ;
                (*it)->m_realAABB.m_Zmax += moveZ;

                
                QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                    (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);
                
                (*it)->setTransform(sumtran1);
                if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
                {
                    SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                    supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
                }
            }

            submitToUI();

            
            refreshMoveFrame();
        }

        m_stepFlag = 0;
        m_pickedObj = 0;
        for (int i = 0; i < m_allPickObjs.size(); i++)
        {
            m_allPickObjs[i]->setStatus(general);
            /*if (dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i]))
            {
                dynamic_pointer_cast<CHAssembly>(m_allPickObjs[i])->injectPropertiesToChildren();
            }*/
        }
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
            if (printModel != nullptr)
            {
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
                {
                    SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
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
                //printModel->isSceneIn(printModel->getRealAABB(), getDoc()->m_machineBox->getBaseAABB());
            }
        }
        emit getDoc()->modelCheckSceneInChanged();
        curScene->refresh();
    }

}

void EditMeshMoveTransformTool::resetBtnClicked()
{
    AkUtil::TFunction("");
    for (auto bIt = getGlobalPick()->m_selectedObjs.begin(); bIt != getGlobalPick()->m_selectedObjs.end(); bIt++)
    {
        if (std::dynamic_pointer_cast<SupportMesh>(*bIt) != nullptr)
        {
            continue;
        }
        (*bIt)->resetMove();
    }
    bool lockToPrintPlatform = m_lockToPrintPlatform;
    std::vector<double> params(3);
    if(m_editMeshModels.size() == 1)
    {
        
        
        params[0] = m_firstMesh->m_params[6] + m_firstMesh->m_rotCenter[0];
        params[1] = m_firstMesh->m_params[7] + m_firstMesh->m_rotCenter[1];
        params[2] = m_firstMesh->m_params[8] + m_firstMesh->m_rotCenter[2] - m_firstMesh->m_baseAABB.getLenZ() / 2;
        qDebug() << "params: " << params[0] << ",  " << params[1] << ",  " << params[2];
        m_lockToPrintPlatform = true;
        emit sendParamsToGui(params, MoveChangedType_ResetMove);
        m_lockToPrintPlatform = lockToPrintPlatform;
    }
    else if(m_editMeshModels.size() > 1)
    {
//        params[0] = m_firstMesh->m_params[6] - m_initValues[0][0];
//        params[1] = m_firstMesh->m_params[7] - m_initValues[0][1];
//        params[2] = m_firstMesh->m_params[8] - m_initValues[0][2];
        m_lockToPrintPlatform = true;
        resetSelectedMoveObjsClicked();
        curScene->refresh();
        submitToUI();
        m_lockToPrintPlatform = lockToPrintPlatform;
    }

}

void EditMeshMoveTransformTool::resetSelectedObjsClicked()
{
    AkUtil::TFunction("");
    getGlobalPick()->resetSelectedObjs();
    submitToUI();
}

void EditMeshMoveTransformTool::lockToPrintPlatformStatusChanged()
{
    AkUtil::TFunction("");
    if (m_lockToPrintPlatform)
    {
        CHAABB3D aabb;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            aabb.add((*it)->calRealAABB());
        }
        float moveZ = -aabb.getLeftDownPoint().z();

        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                continue;
            }
            (*it)->m_params[8] = (*it)->m_params[8] + moveZ;

            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                                                                                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
            }
        }

        submitToUI();
        emit getDoc()->modelCheckSceneInChanged();
        curScene->refresh();
        refreshMoveFrame();
    }
}

void EditMeshMoveTransformTool::receiveParams(vector<float> params)
{
    if (m_stepFlag == 1)
    {
        return;
    }
    CHAABB3D aabb;
    int i = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        if(m_editMeshModels.size() > 1)
        {
            (*it)->m_params[6] = params[0] + m_initValues[i][0];
            (*it)->m_params[7] = params[1] + m_initValues[i][1];
            (*it)->m_params[8] = params[2] + m_initValues[i][2];
            i++;
        }
        else if (m_editMeshModels.size() == 1)
        {
            (*it)->m_params[6] = params[0];
            (*it)->m_params[7] = params[1];
            (*it)->m_params[8] = params[2];
        }

        
        (*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
            (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params));
        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
        if (printModel != nullptr)
        {
            QVector3D position, scale;
            QQuaternion orientation;
            double pitch, yaw, roll;
            for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
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

       aabb.add((*it)->calRealAABB());
    }
    }
    
    if (m_lockToPrintPlatform)
    {
        float moveZ = -aabb.getLeftDownPoint().z();

        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                continue;
            }
            (*it)->m_params[8] = (*it)->m_params[8] + moveZ;

            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            
            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
                (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params);

            
            (*it)->setTransform(sumtran1);
            if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
                supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
            }
        }

        submitToUI();
    }
//    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
//    {
//        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
//        if (printModel != nullptr)
//            printModel->isSceneIn(printModel->getRealAABB(), getDoc()->m_machineBox->getBaseAABB());
//    }
    emit getDoc()->modelCheckSceneInChanged();
    //????????
    refreshMoveFrame();

    curScene->refresh();
}

void EditMeshMoveTransformTool::receiveParamsFormGui(std::vector<double> params, MoveChangedType type)
{
    if(m_editMeshModels.size() == 1)
    {
        std::vector<float> tmpParams(3);
        tmpParams[0] = params[0] - m_firstMesh->m_rotCenter[0];
        tmpParams[1] = params[1] - m_firstMesh->m_rotCenter[1];
        tmpParams[2] = params[2] - m_firstMesh->m_rotCenter[2] + m_firstMesh->m_baseAABB.getLenZ() / 2;
        receiveParams(tmpParams);
    }
    else
    {
        std::vector<float> tmpParams(3);
        tmpParams[0] = params[0];
        tmpParams[1] = params[1];
        tmpParams[2] = params[2];
        receiveParams(tmpParams);
    }
}

void EditMeshMoveTransformTool::refreshMoveFrame()
{
    if (m_adjustAxisX == nullptr || m_adjustAxisY == nullptr || m_adjustAxisZ == nullptr || m_adjustOrigin == nullptr)
    {
        return;
    }
    CHAABB3D aabb;
    for(std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        aabb.add((*it)->m_realAABB);
    }
    QVector3D center = aabb.getCenterPoint();
    QVector3D curOrigin = m_adjustOrigin->getCurrentCoord();
    QVector3D offset = center - curOrigin;

    QMatrix4x4 trans;
//    tran.translate(QVector3D(m_firstMesh->m_params[6], m_firstMesh->m_params[7], m_firstMesh->m_params[8]) -
//        QVector3D(m_initValues[0][0], m_initValues[0][1], m_initValues[0][2]));
//    m_adjustAxisX->setTransform(tran);
//    m_adjustAxisY->setTransform(tran);
//    m_adjustAxisZ->setTransform(tran);
//    m_adjustOrigin->setTransform(tran);
    trans.translate(offset);
    m_adjustAxisX->setTransform(trans);
    m_adjustAxisY->setTransform(trans);
    m_adjustAxisZ->setTransform(trans);
    m_adjustOrigin->setTransform(trans);
}

void EditMeshMoveTransformTool::submitToUI()
{
    std::vector<double> params(3);
    if (m_editMeshModels.size() == 1)
    {
        params[0] = m_firstMesh->m_params[6] + m_firstMesh->m_rotCenter[0];
        params[1] = m_firstMesh->m_params[7] + m_firstMesh->m_rotCenter[1];
        params[2] = m_firstMesh->m_params[8] + m_firstMesh->m_rotCenter[2] - m_firstMesh->m_baseAABB.getLenZ() / 2;
    }
    else
    {
        params[0] = m_firstMesh->m_params[6] - m_initValues[0][0];
        params[1] = m_firstMesh->m_params[7] - m_initValues[0][1];
        params[2] = m_firstMesh->m_params[8] - m_initValues[0][2];
    }
    emit sendParamsToGui(params);
}

void EditMeshMoveTransformTool::resetSelectedMoveObjsClicked()
{
    for(std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        (*it)->resetMove();
        
        (*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
            (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params));
        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>((*it));
        if (printModel != nullptr)
        {
            QVector3D position, scale;
            QQuaternion orientation;
            double pitch, yaw, roll;
            for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
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
    refreshMoveFrame();
    emit getDoc()->modelCheckSceneInChanged();
    emit getDoc()->modelObjsStatusChanged(ModelStatusChangedType::ResetMesh);
}



