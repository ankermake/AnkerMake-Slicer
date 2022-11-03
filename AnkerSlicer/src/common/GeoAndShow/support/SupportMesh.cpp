#include "SupportMesh.h"
#include "../common/GeoAndShow/AkTransformMath.h"
#include "../common/GeoAndShow/CHBaseAlg.h"
SupportMesh::SupportMesh(): m_type(Cuboid)
{

}

void SupportMesh::copy(CHShowObjPtr &obj)
{
    if (!obj)
    {
        obj = SupportMeshPtr(new SupportMesh);
    }

    SupportMeshPtr out = std::dynamic_pointer_cast<SupportMesh>(obj);
    if (!out)
    {
        return;
    }
    out->setParent(m_parent);
    out->setObjectName(m_name);

    out->m_baseAABB = m_baseAABB;
    out->m_realAABB = m_realAABB;
    out->setColor(m_color);
    out->setLightTest(m_lightTest);
    out->setCalLight(m_calLight);
    out->setTransform(m_tran);

    out->m_vertices = m_vertices;
    out->m_nors = m_nors;
    out->m_uvs = m_uvs;
    out->m_trians = m_trians;
    out->m_rotCenter[0] = m_rotCenter[0];
    out->m_rotCenter[1] = m_rotCenter[1];
    out->m_rotCenter[2] = m_rotCenter[2];
    out->m_params = m_params;
}

QString SupportMesh::serialize(QString path)
{
    return "";
}

QMatrix4x4 SupportMesh::getLocalTransform() const
{
    return m_localTransform;
}

void SupportMesh::setLocalTransform(const QMatrix4x4& mat)
{
    m_localTransform = mat;
}

void SupportMesh::initSupportParams()
{
    //m_baseAABB = calSimilarAABB();
    m_rotCenter = m_realAABB.getCenterPoint();
    QVector3D position, scale;
    QQuaternion  orientation;
    double pitch, yaw, roll;
    AkTransformMath::decomposeQMatrix4x4(getTransform(), position, orientation, scale);
    //QVector4D move = m_tran.column(3);
    m_params.clear();
    m_params.push_back(scale[0]);
    m_params.push_back(scale[1]);
    m_params.push_back(scale[2]);

    QMatrix4x4 tmpRotMat(orientation.toRotationMatrix());
    CHBaseAlg::instance()->calEulerAnglesFromRotMatrix(tmpRotMat, pitch, yaw, roll);
    
    pitch = pitch / CH_PI * 180.0;
    yaw = yaw / CH_PI * 180.0;
    roll = roll / CH_PI * 180.0;
    m_params.push_back(pitch);
    m_params.push_back(yaw);
    m_params.push_back(roll);

    QVector3D newCenter = getTransform() * m_rotCenter;
    QVector3D offset = newCenter - m_rotCenter;
    m_params.push_back(offset[0]);
    m_params.push_back(offset[1]);
    m_params.push_back(offset[2]);

    m_initParams = m_params;
    m_initTransform = getTransform();
}


CHMeshShowObjPtr SupportMesh::deserialize(QString xmlContent)
{
    return nullptr;
}

//QMatrix4x4 SupportMesh::getInitTransform() const
//{
//    return m_initTransform;
//}
//
//void SupportMesh::setInitTransform(const QMatrix4x4& mat)
//{
//    m_initTransform = mat;
//}
