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
#include "edit_meshrotationtransform.h"
#include <wrap/qt/gl_label.h>
#include <wrap/gui/trackball.h>
#include "wrap/gl/space.h"
#include "common/GeoAndShow/TransformPack.h"
#include "common/GeoAndShow/CHLine3D.h"
#include "common/GeoAndShow/CHBaseAlg.h"
#include "edit_meshtransform_factory.h"
#include "common/GeoAndShow/support/SupportAssemblyMeshes.h"
#include "common/utilities/tlogger.h"

using namespace vcg;


CHLocalCoordinateAxis::CHLocalCoordinateAxis()
{
    AkUtil::TFunction("");
}

CHLocalCoordinateAxis::~CHLocalCoordinateAxis()
{
    AkUtil::TFunction("");
}

void CHLocalCoordinateAxis::create(QVector3D center, double lenth)
{
    AkUtil::TFunction("");
    setDirty(true);

    m_axisX = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_axisX->create(center, center + QVector3D(lenth, 0, 0));
    m_axisX->setColor(QColor(125, 0, 0));

    m_axisY = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_axisY->create(center, center + QVector3D(0, lenth, 0));
    m_axisY->setColor(QColor(0, 125, 0));

    m_axisZ = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_axisZ->create(center, center + QVector3D(0, 0, lenth));
    m_axisZ->setColor(QColor(0, 0, 125));

    m_baseShowObjs.push_back(m_axisX);
    m_baseShowObjs.push_back(m_axisY);
    m_baseShowObjs.push_back(m_axisZ);
}

CHAdjustCircle::CHAdjustCircle()
{
    AkUtil::TFunction("");
}

CHAdjustCircle::~CHAdjustCircle()
{
}

void CHAdjustCircle::create(QVector3D center, double rad, QVector3D nor, QVector3D refVec)
{
    AkUtil::TFunction("");
    setDirty(true);

    m_circleBody = CHCircleShowObjPtr(new CHCircleShowObj);
    m_circleBody->create(center, rad, nor, refVec);

    m_lineBody = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_lineBody->create(center, center + refVec * rad);

    m_adjustPointBody = CHPointShowObjPtr(new CHPointShowObj);
    m_adjustPointBody->create(center + refVec * rad);
    m_adjustPointBody->setSize(8);

    m_baseShowObjs.push_back(m_circleBody);
    m_baseShowObjs.push_back(m_lineBody);
    m_baseShowObjs.push_back(m_adjustPointBody);
}

EditMeshRotationTransformTool::EditMeshRotationTransformTool()
{
    m_paramUI = 0;
    m_stepFlag = 0;
    m_adjustShowCurve = 0;
    m_operateMoveZ = 0;
}

bool EditMeshRotationTransformTool::startAnkerEdit(ActionEditTool * action, void * arg1, void *arg2)
{
    AkUtil::TFunction("");
    if(m_isActivated){return false;}
    m_isActivated = true;

    
    curScene = static_cast<CHScene*>(arg1);
    if (curScene->m_pickCommand->m_selectedObjs.size() == 0)
    {
        return false;
    }

    m_editMeshModels = curScene->m_pickCommand->m_selectedObjs;
    m_firstMesh = *m_editMeshModels.begin();

    m_stepFlag = 0;
    m_adjustShowCurve = 0;
    m_pickedObj = 0;
    m_operateMoveZ = 0;

    m_paramUI = new CHModelRotationTransformParamsSetUI();
    EditMeshTransformFactory::m_conInt->addWidgetToModelTransForm(m_paramUI, AkConst::FDMMeshTransForm::Rotate);
    connect(this, &EditMeshRotationTransformTool::sendParamsToGui, m_paramUI, &CHModelRotationTransformParamsSetUI::receiveParams);
    connect(m_paramUI, &CHModelRotationTransformParamsSetUI::sendParams, this, &EditMeshRotationTransformTool::receiveParamsFromGui);
    connect(m_paramUI, &CHModelRotationTransformParamsSetUI::reset, this, &EditMeshRotationTransformTool::resetBtnClicked);
    connect(this, &EditMeshRotationTransformTool::sendParam, m_paramUI, &CHModelRotationTransformParamsSetUI::receiveParam);

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
        m_initValues[p][0] = (*it)->m_params[3];
        m_initValues[p][1] = (*it)->m_params[4];
        m_initValues[p][2] = (*it)->m_params[5];
        m_initValues[p][3] = (*it)->m_params[6];
        m_initValues[p][4] = (*it)->m_params[7];
        m_initValues[p][5] = (*it)->m_params[8];

        qDebug() << "initValues: " << (*it)->m_params[3] << ", " << (*it)->m_params[4] << ", " <<(*it)->m_params[5];
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
    submitToUI();


    
    double ratio = 0.5;
    double maxLenth = max(max(aabb.getLenX(), aabb.getLenY()), aabb.getLenZ()) * ratio;
    QVector3D originCreateCenter = m_operationCenter;
    m_CoordAxis1 = CHLocalCoordinateAxisPtr(new CHLocalCoordinateAxis);
    m_CoordAxis1->create(originCreateCenter, maxLenth);
    m_CoordAxis1->setVisuable(false);
    m_CoordAxis2 = CHLocalCoordinateAxisPtr(new CHLocalCoordinateAxis);
    m_CoordAxis2->create(originCreateCenter, maxLenth);
    m_CoordAxis2->setVisuable(false);
    //m_CoordAxis2->setTransform(CHBaseAlg::instance()->calTransformFromParams(QVector3D(m_meshModel->m_rotCenter[0],
        //m_meshModel->m_rotCenter[1], m_meshModel->m_rotCenter[2]), m_meshModel->m_params));//??????????

    
    double rad = sqrt(aabb.getLenX() * aabb.getLenX() + aabb.getLenY() * aabb.getLenY() + aabb.getLenZ() * aabb.getLenZ()) / 2;
    m_adjustCircleX = CHAdjustCirclePtr(new CHAdjustCircle);
    m_adjustCircleX->create(originCreateCenter, rad, QVector3D(1, 0, 0), QVector3D(0, -1, 0));
    m_adjustCircleX->setColor(QColor(125, 0, 0));
    m_adjustCircleY = CHAdjustCirclePtr(new CHAdjustCircle);
    m_adjustCircleY->create(originCreateCenter, rad, QVector3D(0, 1, 0), QVector3D(0, 0, 1));
    m_adjustCircleY->setColor(QColor(0, 125, 0));
    m_adjustCircleZ = CHAdjustCirclePtr(new CHAdjustCircle);
    m_adjustCircleZ->create(originCreateCenter, rad, QVector3D(0, 0, 1), QVector3D(1, 0, 0));
    m_adjustCircleZ->setColor(QColor(0, 0, 125));

    
    refreshRotationFrame();

    m_allPickObjs.push_back(m_adjustCircleX);
    m_allPickObjs.push_back(m_adjustCircleY);
    m_allPickObjs.push_back(m_adjustCircleZ);

    m_allShowObjs.push_back(m_CoordAxis1);
    m_allShowObjs.push_back(m_CoordAxis2);
    m_allShowObjs.push_back(m_adjustCircleX);
    m_allShowObjs.push_back(m_adjustCircleY);
    m_allShowObjs.push_back(m_adjustCircleZ);
    m_adjustShowCurve = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_adjustShowCurve->setVisuable(false);
    m_allShowObjs.push_back(m_adjustShowCurve);
    for (int i = 0; i < m_allShowObjs.size(); i++)
    {
        m_allShowObjs[i]->updateToScene();
    }

    return true;
}

void EditMeshRotationTransformTool::endAnkerEdit  (ActionEditTool *, void *, void *)
{
    AkUtil::TFunction("");
    if(!m_isActivated){return;}
    m_isActivated = false;

    if (m_paramUI)
    {
        delete m_paramUI;
        m_paramUI = 0;
    }

    for (int i = 0; i < m_allShowObjs.size(); i++)
    {
        m_allShowObjs[i]->setDelete(true);
        /*if (dynamic_pointer_cast<CHAssembly>(m_allShowObjs[i]))
        {
            dynamic_pointer_cast<CHAssembly>(m_allShowObjs[i])->injectPropertiesToChildren();
        }*/
        m_allShowObjs[i]->updateToScene();
    }

    m_allPickObjs.clear();
    m_allShowObjs.clear();
}


void EditMeshRotationTransformTool::mousePressEvent(QMouseEvent  *event, void *, void *)
{
    AkUtil::TFunction("");
    //BoxBorder::getInstancePtr()->setParam(gla, meshmodel);
    if (event->button() == Qt::LeftButton && m_pickedObj)
    {
        event->accept();
        CHAdjustCirclePtr pickObj = dynamic_pointer_cast<CHAdjustCircle>(m_pickedObj);

        m_stepFlag = 1;
        int i = 0;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            m_values[i][0] = (*it)->m_params[3];
            m_values[i][1] = (*it)->m_params[4];
            m_values[i][2] = (*it)->m_params[5];
            m_values[i][3] = (*it)->m_params[6];
            m_values[i][4] = (*it)->m_params[7];
            m_values[i][5] = (*it)->m_params[8];
            i++;

//            CH3DPrintModelPtr meshPtr = dynamic_pointer_cast<CH3DPrintModel>(*it);
//            if (meshPtr != NULL)
//            {
//                meshPtr->isSceneIn(meshPtr->calRealAABB(), getDoc()->m_machineBox->getBaseAABB());
//            }
        }
        emit getDoc()->modelCheckSceneInChanged();
        QVector3D pt1 = pickObj->m_lineBody->m_st;
        QVector3D pt2 = pickObj->m_lineBody->m_et;
        QMatrix4x4 sumMat = pickObj->getTransform() /** pickObj->m_lineBody->getTransform()*/;
        pt1 = TransformPack::pRot(sumMat, pt1);
        pt2 = TransformPack::pRot(sumMat, pt2);

        //m_adjustShowCurve = CHLineSegmentShowObjPtr(new CHLineSegmentShowObj);
        m_adjustShowCurve->create(pt1, pt2);
        m_adjustShowCurve->setVisuable(true);
        m_adjustShowCurve->setTransform(QMatrix4x4());//??????????
        m_adjustShowCurve->setColor(pickObj->getColor());
        m_adjustShowCurve->updateToScene();

        curScene->refresh();
    }
}

void EditMeshRotationTransformTool::mouseMoveEvent(QMouseEvent  *event, void *, void *)
{
    if (m_stepFlag == 0)
    {
        CHShowObjPtr curPickedObj = 0;
        double pickTol = 10;
        for (int i = 0; i < m_allPickObjs.size(); i++)
        {
            PickResult result;
            if (m_allPickObjs[i]->pick(event->pos().x(), event->pos().y(), result, pickTol))
            {
                event->accept();
                curPickedObj = m_allPickObjs[i];
                break;
            }
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
        CHAdjustCirclePtr pickObj = dynamic_pointer_cast<CHAdjustCircle>(m_pickedObj);

        /*QVector3D realCenter = pickObj->m_circleBody->m_center + QVector3D(m_meshModel->m_params[6],
            m_meshModel->m_params[7], m_meshModel->m_params[8]);*/
        QVector3D realCenter = m_operationCenter + QVector3D(0, 0, m_operateMoveZ);

        QVector3D st, et;
        curScene->getCurNearFarPoint(event->pos().x(), event->pos().y(), st, et);
        QVector3D dir = et - st;
        float m = QVector3D::dotProduct(dir, pickObj->m_circleBody->m_nor);
        if (fabs(m) > 0.0000001)
        {
            float t = QVector3D::dotProduct(realCenter - st, pickObj->m_circleBody->m_nor) / m;
            QVector3D intersection = st + dir * t;

            
            QVector3D tvec = intersection - realCenter;
            tvec.normalize();
            double angle = acos(QVector3D::dotProduct(tvec, pickObj->m_circleBody->m_refVec));
            QVector3D outvec = QVector3D::crossProduct(pickObj->m_circleBody->m_refVec, tvec);
            if (QVector3D::dotProduct(outvec, pickObj->m_circleBody->m_nor) < 0)
            {
                angle = -angle;
            }
            qDebug() << "angle: " << angle;

            
            QMatrix4x4 stran = TransformPack::rotMat(realCenter, pickObj->m_circleBody->m_nor, angle);
            m_adjustShowCurve->setTransform(stran);
//            if(m_editMeshModels.size() > 1)
//            {
//                QVector3D refVec = pickObj->m_circleBody->m_refVec;
//                RotateAxisType tmpAxis = RotateAxisType_Unknown;
//                if(fabs(refVec[0]) == 1 && refVec[1] == 0 && refVec[2] == 0)
//                {
//                    tmpAxis = RotateAxisType_Z;
//                }
//                else if(fabs(refVec[1]) == 1 && refVec[0] == 0 && refVec[2] == 0)
//                {
//                    tmpAxis = RotateAxisType_X;

//                }
//                else if(fabs(refVec[2]) == 1 && refVec[0] == 0 && refVec[1] == 0)
//                {
//                    tmpAxis = RotateAxisType_Y;
//                }
//                double angleX, angleY, angleZ, tmpAngle;

//                CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(stran, angleX, angleY, angleZ);

//                angleX = angleX / CH_PI * 180.0;
//                angleY = angleY / CH_PI * 180.0;
//                angleZ = angleZ / CH_PI * 180.0;
//                qDebug() << "refVec: " <<pickObj->m_circleBody->m_refVec
//                         << ", calEulerAnglesFromRotMatrix: " << angleX << ", " << angleY << ", " << angleZ;
//                if(tmpAxis == RotateAxisType_Z)
//                {
//                    tmpAngle = angleZ;
//                }
//                else if(tmpAxis == RotateAxisType_X)
//                {
//                    tmpAngle = angleX;
//                }
//                else if(tmpAxis == RotateAxisType_Y)
//                {
//                    tmpAngle = angleY;
//                }
//                emit sendParam(tmpAngle, tmpAxis);
//            }

            int i = 0;
            for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            {
                
                QMatrix4x4 tran1, tran2, tran3, tran4;
                tran1.rotate(m_values[i][0], QVector3D(1, 0, 0));
                tran2.rotate(m_values[i][1], QVector3D(0, 1, 0));
                tran3.rotate(m_values[i][2], QVector3D(0, 0, 1));
                tran4.rotate(angle / CH_PI * 180.0, pickObj->m_circleBody->m_nor);

                double angleX, angleY, angleZ;
                QMatrix4x4 rotTran = tran4 * tran3 * tran2 * tran1;
                CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(rotTran, angleX, angleY, angleZ);
                
                (*it)->m_params[3] = angleX / CH_PI * 180.0;
                (*it)->m_params[4] = angleY / CH_PI * 180.0;
                (*it)->m_params[5] = angleZ / CH_PI * 180.0;
                adjustSingleAngle((*it)->m_params[3]);
                adjustSingleAngle((*it)->m_params[4]);
                adjustSingleAngle((*it)->m_params[5]);
                //qDebug() << "real Params: " << (*it)->m_params[3] << ", " << (*it)->m_params[4] << ", " << (*it)->m_params[5];

//                if(m_editMeshModels.size() > 1)
//                {
//                    std::vector<double> vecParams(3);
//                    vecParams[0] = (*it)->m_params[3];
//                    vecParams[1] = (*it)->m_params[4];
//                    vecParams[2] = (*it)->m_params[5];
//                    emit sendParamsToGui(vecParams);
//                }
                
                QVector3D currentMeshCenter = (*it)->m_rotCenter + QVector3D(m_values[i][3], m_values[i][4], m_values[i][5]);
                QVector3D newCenter = TransformPack::pRot(stran, currentMeshCenter);
                QVector3D sumMove = newCenter - (*it)->m_rotCenter;
                (*it)->m_params[6] = sumMove[0];
                (*it)->m_params[7] = sumMove[1];
                (*it)->m_params[8] = sumMove[2];

                i++;
            }

            submitToUI();

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

            
            //m_CoordAxis2->setTransform(sumtran1);

            curScene->refresh();
        }
    }
}

void EditMeshRotationTransformTool::mouseReleaseEvent(QMouseEvent  *event, void *, void *)
{
    AkUtil::TFunction("");
    if (event->button() == Qt::LeftButton && m_pickedObj)
    {
        //???????????????????????????????????pick??????
        CHAABB3D aabb;
        for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
        {
            aabb.add((*it)->calRealAABB());
        }
        
        if (m_lockToPrintPlatform && !std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()))
        {
            //CHAABB3D aabb;//?????????
            //for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
            //{
            //	aabb.add((*it)->calRealAABB());
            //}
            float moveZ = -aabb.m_Zmin;
            m_operateMoveZ += moveZ;

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

            
            //m_CoordAxis2->setTransform(sumtran1);

            refreshRotationFrame();
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
        m_adjustShowCurve->setVisuable(false);
        curScene->refresh();
    }
    //BoxBorder::getInstancePtr()->setParam(gla, meshmodel);
}

void EditMeshRotationTransformTool::showAxis(bool showed)
{
    m_CoordAxis1->setVisuable(showed);
    m_CoordAxis2->setVisuable(showed);

    curScene->refresh();
}

void EditMeshRotationTransformTool::receiveParams(vector<float> params)
{
    CHAABB3D aabb;//?????????
    int i = 0;
    for (std::set<CHMeshShowObjPtr>::iterator it = m_editMeshModels.begin(); it != m_editMeshModels.end(); it++)
    {
        

        {
            
            
            
            QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6;
            tran1.rotate(m_initValues[i][0], QVector3D(1, 0, 0));
            tran2.rotate(m_initValues[i][1], QVector3D(0, 1, 0));
            tran3.rotate(m_initValues[i][2], QVector3D(0, 0, 1));
            tran4.rotate(params[0], QVector3D(1, 0, 0));
            tran5.rotate(params[1], QVector3D(0, 1, 0));
            tran6.rotate(params[2], QVector3D(0, 0, 1));

            double angleX, angleY, angleZ;
            QMatrix4x4 rotTran = tran6 * tran5 * tran4 * tran3 * tran2 * tran1;
            CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(rotTran, angleX, angleY, angleZ);
            
            (*it)->m_params[3] = angleX / CH_PI * 180.0;
            (*it)->m_params[4] = angleY / CH_PI * 180.0;
            (*it)->m_params[5] = angleZ / CH_PI * 180.0;

            qDebug() << "real params: " << (*it)->m_params[3] << ", " << (*it)->m_params[4] << ", " << (*it)->m_params[5];
            adjustSingleAngle((*it)->m_params[3]);
            adjustSingleAngle((*it)->m_params[4]);
            adjustSingleAngle((*it)->m_params[5]);

            
            
            QVector3D vecDir = (*it)->m_rotCenter + QVector3D(m_initValues[i][3], m_initValues[i][4], m_initValues[i][5]) - m_operationCenter;
            QMatrix4x4 ttran1, ttran2, ttran3;
            ttran1.rotate(params[0], QVector3D(1, 0, 0));
            ttran2.rotate(params[1], QVector3D(0, 1, 0));
            ttran3.rotate(params[2], QVector3D(0, 0, 1));
            QVector3D newCenter = TransformPack::vRot(ttran3 * ttran2 * ttran1, vecDir) + m_operationCenter +
                QVector3D(0, 0, m_operateMoveZ);
            (*it)->m_params[6] = newCenter[0] - (*it)->m_rotCenter[0];
            (*it)->m_params[7] = newCenter[1] - (*it)->m_rotCenter[1];
            (*it)->m_params[8] = newCenter[2] - (*it)->m_rotCenter[2];
        }
        
        (*it)->setTransform(CHBaseAlg::instance()->calTransformFromParams(QVector3D((*it)->m_rotCenter[0],
            (*it)->m_rotCenter[1], (*it)->m_rotCenter[2]), (*it)->m_params));
        if (std::dynamic_pointer_cast<SupportMesh>(*it) != nullptr)
        {
            SupportMeshPtr supportMesh = std::dynamic_pointer_cast<SupportMesh>(*it);
            supportMesh->setLocalTransform(supportMesh->getParent()->getTransform().inverted() * supportMesh->getTransform());
        }

        if (true/*m_lockToPrintPlatform*/)//??????????????????????????
        {
            aabb.add((*it)->calRealAABB());
        }
        i++;
    }
    
    if (m_lockToPrintPlatform && !std::dynamic_pointer_cast<SupportMesh>(*m_editMeshModels.begin()))
    {
        float moveZ = -aabb.m_Zmin;
        m_operateMoveZ += moveZ;
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
    }

    
    //m_CoordAxis2->setTransform(sumtran1);

    
    refreshRotationFrame();
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

void EditMeshRotationTransformTool::receiveParamsFromGui(std::vector<double> params, RotateChangedType type)
{
    if(type == RotateChangedType_BoxRotate)
    {
        std::vector<float> vecParams(3);
        if(m_editMeshModels.size() == 1)
        {
            vecParams[0] = params[0] - m_initValues[0][0];
            vecParams[1] = params[1] - m_initValues[0][1];
            vecParams[2] = params[2] - m_initValues[0][2];
        }
        else if(m_editMeshModels.size() > 1)
        {
            vecParams[0] = params[0];
            vecParams[1] = params[1];
            vecParams[2] = params[2];
        }
        receiveParams(vecParams);
    }
}

void EditMeshRotationTransformTool::resetBtnClicked()
{
    AkUtil::TFunction("");
    auto bIt = getGlobalPick()->m_selectedObjs.begin();
    auto eIt = getGlobalPick()->m_selectedObjs.end();
    for (bIt; bIt != eIt; bIt++)
    {
        if (std::dynamic_pointer_cast<SupportMesh>(*bIt) != nullptr)
        {
            continue;
        }
        (*bIt)->resetRot();
    }
    vector<float> params(3);
    QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6;
    tran1.rotate(m_initValues[0][0], QVector3D(1, 0, 0));
    tran2.rotate(m_initValues[0][1], QVector3D(0, 1, 0));
    tran3.rotate(m_initValues[0][2], QVector3D(0, 0, 1));
    tran4.rotate(m_firstMesh->m_params[3], QVector3D(1, 0, 0));
    tran5.rotate(m_firstMesh->m_params[4], QVector3D(0, 1, 0));
    tran6.rotate(m_firstMesh->m_params[5], QVector3D(0, 0, 1));
    
    QMatrix4x4 rotTran = tran6 * tran5 * tran4 * (tran3 * tran2 * tran1).inverted();

    double angleX, angleY, angleZ;
    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(rotTran, angleX, angleY, angleZ);

    
    params[0] = angleX / CH_PI * 180.0;
    params[1] = angleY / CH_PI * 180.0;
    params[2] = angleZ / CH_PI * 180.0;
    adjustSingleAngle(params[0]);
    adjustSingleAngle(params[1]);
    adjustSingleAngle(params[2]);

    std::vector<double> vecParams(3);
    if(m_editMeshModels.size() > 1)
    {
        vecParams[0] = params[0];
        vecParams[1] = params[1];
        vecParams[2] = params[2];
    }
    else if(m_editMeshModels.size() == 1)
    {
        vecParams[0] = params[0] + m_initValues[0][0];
        vecParams[1] = params[1] + m_initValues[0][1];
        vecParams[2] = params[2] + m_initValues[0][2];
    }

    qDebug() << "reset params: " << vecParams;
    emit sendParamsToGui(vecParams, RotateChangedType_ResetRotate);
}

void EditMeshRotationTransformTool::resetSelectedObjsClicked()
{
    AkUtil::TFunction("");
    getGlobalPick()->resetSelectedObjs();
    submitToUI();
}

void EditMeshRotationTransformTool::adjustSingleAngle(float& angle)
{
    int n = (int)(angle / 360.0);
    if (angle < 0)
    {
        n--;
    }
    angle = angle - 360.0 * n;
}

void EditMeshRotationTransformTool::refreshRotationFrame()
{
    if (m_adjustCircleX == nullptr || m_adjustCircleY == nullptr || m_adjustCircleZ == nullptr)
        return;
    QMatrix4x4 tran;
    tran.translate(QVector3D(0, 0, m_operateMoveZ));
    m_adjustCircleX->setTransform(tran);
    //m_adjustCircleX->injectPropertiesToChildren();
    m_adjustCircleY->setTransform(tran);
    //m_adjustCircleY->injectPropertiesToChildren();
    m_adjustCircleZ->setTransform(tran);
    //m_adjustCircleZ->injectPropertiesToChildren();
}

void EditMeshRotationTransformTool::submitToUI()
{
    vector<float> params(3);
    QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6;

    if(m_editMeshModels.size() > 1)
    {
        tran1.rotate(m_initValues[0][0], QVector3D(1, 0, 0));
        tran2.rotate(m_initValues[0][1], QVector3D(0, 1, 0));
        tran3.rotate(m_initValues[0][2], QVector3D(0, 0, 1));
    }
    else if(m_editMeshModels.size() == 1)
    {

    }

    tran4.rotate(m_firstMesh->m_params[3], QVector3D(1, 0, 0));
    tran5.rotate(m_firstMesh->m_params[4], QVector3D(0, 1, 0));
    tran6.rotate(m_firstMesh->m_params[5], QVector3D(0, 0, 1));
    
    QMatrix4x4 rotTran = tran6 * tran5 * tran4 * (tran3 * tran2 * tran1).inverted();

    double angleX, angleY, angleZ;
    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(rotTran, angleX, angleY, angleZ);

    
    params[0] = angleX / CH_PI * 180.0;
    params[1] = angleY / CH_PI * 180.0;
    params[2] = angleZ / CH_PI * 180.0;
    adjustSingleAngle(params[0]);
    adjustSingleAngle(params[1]);
    adjustSingleAngle(params[2]);

    std::vector<double> vecParams(3);
    vecParams[0] = params[0];
    vecParams[1] = params[1];
    vecParams[2] = params[2];
    emit sendParamsToGui(vecParams);
}



