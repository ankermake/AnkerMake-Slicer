#include "TransformPack.h"
#include "CHBase.h"

#include <QQuaternion>

QMatrix4x4 TransformPack::rotMat(const QVector3D& pos, const QVector3D& axis, float angle)
{
    QMatrix4x4 matrix;
    matrix.translate(pos);
    matrix.rotate(angle / CH_PI * 180.0, axis);
    matrix.translate(-pos);
    return matrix;

    //QMatrix4x4 mat1, mat2, mat3;
    //mat1.translate(-pos);
    //mat2.rotate(angle / CH_PI * 180.0, axis);
    //mat3.translate(pos);
    //return mat3 * mat2 * mat1;
}

QMatrix4x4 TransformPack::rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos)
{
    QMatrix4x4 matrix;
    matrix.translate(pos);
    matrix.rotate(QQuaternion::rotationTo(axis1, axis2));
    matrix.translate(-pos);
    return matrix;

    //float angle = vecAngle(axis1, axis2);
    //QVector3D axis = QVector3D::crossProduct(axis1, axis2);
    //if (axis == QVector3D(0, 0, 0))
    //{
    //	QVector3D ref1(1.0, 1.5, 2.0);
    //	QVector3D ref2(-1.0, 1.5, -2.0);
    
//
    //	if (axis == QVector3D(0, 0, 0))
    //	{
    //		axis = QVector3D::crossProduct(axis1, ref2);
    //	}
    //}
    //return rotMat(pos, axis, angle);
}

QMatrix4x4 TransformPack::scale(const QVector3D& pos, float scaleX, float scaleY, float scaleZ)
{
    QMatrix4x4 matrix;
    matrix.translate(pos);
    matrix.scale(scaleX, scaleY, scaleZ);
    matrix.translate(-pos);
    return matrix;

    //QMatrix4x4 mat1, mat2, mat3;
    //mat1.translate(-pos);
    //mat2.scale(scaleX, scaleY, scaleZ);
    //mat3.translate(pos);
    //return mat3 * mat2 * mat1;
}

QVector3D TransformPack::pRot(QMatrix4x4 matrix, QVector3D pt)
{
    return matrix * pt;
    //return matrix * QVector4D(pt, 1.0).toVector3DAffine();
}

QVector3D TransformPack::vRot(QMatrix4x4 matrix, QVector3D vec)
{
    return matrix.mapVector(vec);
    //QVector3D out1 = matrix * QVector4D(0, 0, 0, 1.0).toVector3DAffine();
    //QVector3D out2 = matrix * QVector4D(vec, 1.0).toVector3DAffine();
    //return out2 - out1;
}

float TransformPack::vecAngle(QVector3D vec1, QVector3D vec2)
{
    vec1.normalize();
    vec2.normalize();
    return acos(QVector3D::dotProduct(vec1, vec2));
}









