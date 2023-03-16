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
#include "edit_meshmirrortransform.h"
#include <wrap/qt/gl_label.h>
#include <wrap/gui/trackball.h>
#include "wrap/gl/space.h"
#include "common/GeoAndShow/TransformPack.h"
#include "common/GeoAndShow/CHLine3D.h"
#include "common/GeoAndShow/CHBaseAlg.h"
#include "common/GeoAndShow/CHScene.h"
#include "QPalette"
#include "edit_meshtransform_factory.h"
#include "common/utilities/tlogger.h"

using namespace vcg;

EditMeshMirrorTransformTool::EditMeshMirrorTransformTool()
{
    AkUtil::TFunction("");
    m_operateMoveZ = 0;
    m_paramUI = 0;
}

bool EditMeshMirrorTransformTool::startAnkerEdit(ActionEditTool* action, void* arg1, void* arg2)
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
    if (std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()) != nullptr)
    {
        return false;
    }
    m_operateMoveZ = 0;

    m_paramUI = new CHModelMirrorTransformParamsSetUI();
    EditMeshTransformFactory::m_conInt->addWidgetToModelTransForm(m_paramUI, AkConst::FDMMeshTransForm::Mirror);
    connect(m_paramUI, SIGNAL(sendWhichButtonClicked(int)), this, SLOT(receiveButtonClicked(int)));
    connect(m_paramUI->m_resetButton, SIGNAL(clicked()), this, SLOT(reset()));

    //disconnect(getGlobalPick().get(), SIGNAL(resetSeletedObjsSig()), getGlobalPick().get(), SLOT(resetSelectedObjs()));
    //connect(getGlobalPick().get(), SIGNAL(resetSeletedObjsSig()), this, SLOT(reset()));

    
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        EditMeshTransformFactory::initMeshModelEulerParams(*it);
    }
    m_initValues.resize(m_editMeshModels.size());
    int p = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        m_initValues[p] = (*it)->m_params;
        p++;
    }

    //    if(m_editMeshModels.size() == 1)
    //    {
    //        m_operationCenter = (*m_editMeshModels.begin())->calSimilarAABB().getCenterPoint();
    //    }

    
    //    if (m_editMeshModels.size() > 1)
    {
        CHAABB3D aabb;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            aabb.add((*it)->calSimilarAABB());
        }
        m_operationCenter = aabb.getCenterPoint();
    }

    return true;
}

void EditMeshMirrorTransformTool::endAnkerEdit(ActionEditTool*, void*, void*)
{
    AkUtil::TFunction("");
    if (!m_isActivated) { return; }
    m_isActivated = false;

    //disconnect(getGlobalPick().get(), SIGNAL(resetSeletedObjsSig()), this, SLOT(reset()));
    //connect(getGlobalPick().get(), SIGNAL(resetSeletedObjsSig()), getGlobalPick().get(), SLOT(resetSelectedObjs()));
    if (m_paramUI)
    {
        delete m_paramUI;
        m_paramUI = 0;
    }

    emit getDoc()->modelCheckSceneInChanged();
    curScene->refresh();
}

void EditMeshMirrorTransformTool::mousePressEvent(QMouseEvent* event, void*, void*)
{

}

void EditMeshMirrorTransformTool::mouseMoveEvent(QMouseEvent* event, void*, void*)
{
}

void EditMeshMirrorTransformTool::mouseReleaseEvent(QMouseEvent* event, void*, void*)
{
}

void EditMeshMirrorTransformTool::receiveButtonClicked(int index)
{
    if (std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()) != nullptr)
    {
        return;
    }
    CHAABB3D aabb;
    int p = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        //(*it)->m_params[index] = -(*it)->m_params[index];
        //if (m_editMeshModels.size() > 1)
        {
            //float t1 = (*it)->m_rotCenter[index] + (*it)->m_params[index + 6];
            //(*it)->m_params[index + 6] += 2 * ((m_operationCenter + QVector3D(0, 0, m_operateMoveZ))[index] - t1);

            QVector3D curCenter = m_operationCenter + QVector3D(0, 0, m_operateMoveZ);
            QMatrix4x4 tran1, tran2, tran3;
            tran1.translate(-curCenter);
            QVector3D scaleValue(1, 1, 1);
            scaleValue[index] = -1;
            (*it)->m_mirrAxis[index] = -(*it)->m_mirrAxis[index];

            tran2.scale(scaleValue);
            tran3.translate(curCenter);
            QMatrix4x4 newSumMatrix = tran3 * tran2 * tran1 * (*it)->getTransform();
            (*it)->setTransform(newSumMatrix);
            QVector3D position, scale;
            QQuaternion orientation;
            double pitch, yaw, roll;
            
            AkTransformMath::decomposeQMatrix4x4(newSumMatrix, position, orientation, scale);
            QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
            CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
            
            pitch = pitch / CH_PI * 180.0;
            yaw = yaw / CH_PI * 180.0;
            roll = roll / CH_PI * 180.0;
            (*it)->m_params[0] = scale[0];
            (*it)->m_params[1] = scale[1];
            (*it)->m_params[2] = scale[2];
            (*it)->m_params[3] = pitch;
            (*it)->m_params[4] = yaw;
            (*it)->m_params[5] = roll;

            QVector3D newCenter = (*it)->getTransform() * (*it)->m_rotCenter;
            QVector3D offset = newCenter - (*it)->m_rotCenter;
            (*it)->m_params[6] = offset[0];
            (*it)->m_params[7] = offset[1];
            (*it)->m_params[8] = offset[2];
            p++;
           // qDebug() << "scale: " << scale;
        }

        //(*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams((*it)->m_rotCenter, (*it)->m_params));
        if(std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }
        CH3DPrintModelPtr printModel = std::dynamic_pointer_cast<CH3DPrintModel>(*it);
        if(printModel != nullptr)
        {
            for(int i = 0; i < printModel->m_supportMeshes.size(); i++)
            {
                SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(printModel->m_supportMeshes[i]);
                QVector3D position, scale;
                QQuaternion orientation;
                double pitch, yaw, roll;
                if(supportMesh != nullptr)
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

        if (true/*m_lockToPrintPlatform*/)
        {
            aabb.add((*it)->calRealAABB());
        }
        p++;
    }

    if (m_lockToPrintPlatform)
    {
        float moveZ = -aabb.m_Zmin;
        m_operateMoveZ += moveZ;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
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
    emit getDoc()->modelCheckSceneInChanged();
    curScene->refresh();
}

void EditMeshMirrorTransformTool::reset()
{
    AkUtil::TFunction("");
    if (std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()) != nullptr)
    {
        return;
    }
    CHAABB3D aabb;
    int p = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            continue;
        }

        for (int i = 0; i < 3; i++)
        {
            if((*it)->m_mirrAxis[i] < 0)
            {
                receiveButtonClicked(i);
            }
        }
        if (true/*m_lockToPrintPlatform*/)
        {
            aabb.add((*it)->calRealAABB());
        }
        p++;
    }

    if (m_lockToPrintPlatform)
    {
        float moveZ = -aabb.m_Zmin;
        m_operateMoveZ += moveZ;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            (*it)->m_params[8] = (*it)->m_params[8] + moveZ;

            (*it)->m_realAABB.m_Zmin += moveZ;
            (*it)->m_realAABB.m_Zmax += moveZ;

            QMatrix4x4 sumtran1 = CHBaseAlg::instance()->calTransformFromParams((*it)->m_rotCenter, (*it)->m_params);

            (*it)->setTransform(sumtran1);
        }
    }
    //    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    //    {
    //        CH3DPrintModelPtr meshPtr = dynamic_pointer_cast<CH3DPrintModel>(*it);
    //        if (meshPtr != NULL)
    //        {
    //            meshPtr->isSceneIn(meshPtr->calRealAABB(), getDoc()->m_machineBox->getBaseAABB());
    //        }
    //    }
    emit getDoc()->modelCheckSceneInChanged();
    curScene->refresh();
}

void EditMeshMirrorTransformTool::adjustSingleAngle(float& angle)
{
    int n = (int)(angle / 360.0);
    if (angle < 0)
    {
        n--;
    }
    angle = angle - 360.0 * n;
}


