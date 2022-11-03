#include "AkTransformMath.h"


void AkTransformMath::composeQMatrix4x4(const QVector3D &position, const QQuaternion &orientation, const QVector3D &scale, QMatrix4x4 &m)
{
    const QMatrix3x3 rot3x3(orientation.toRotationMatrix());

    // set up final matrix with scale, rotation and translation
    m(0, 0) = scale.x() * rot3x3(0, 0); m(0, 1) = scale.y() * rot3x3(0, 1); m(0, 2) = scale.z() * rot3x3(0, 2); m(0, 3) = position.x();
    m(1, 0) = scale.x() * rot3x3(1, 0); m(1, 1) = scale.y() * rot3x3(1, 1); m(1, 2) = scale.z() * rot3x3(1, 2); m(1, 3) = position.y();
    m(2, 0) = scale.x() * rot3x3(2, 0); m(2, 1) = scale.y() * rot3x3(2, 1); m(2, 2) = scale.z() * rot3x3(2, 2); m(2, 3) = position.z();
    // no projection term
    m(3, 0) = 0.0f; m(3, 1) = 0.0f; m(3, 2) = 0.0f; m(3, 3) = 1.0f;
}

void AkTransformMath::decomposeQMatrix3x3(const QMatrix3x3 &m, QMatrix3x3 &Q, QVector3D &D, QVector3D &U)
{
    // build orthogonal matrix Q
    float invLen = 1.0f / std::sqrt(m(0, 0) * m(0, 0) + m(1, 0) * m(1, 0) + m(2, 0) * m(2, 0));
    Q(0, 0) = m(0, 0) * invLen;
    Q(1, 0) = m(1, 0) * invLen;
    Q(2, 0) = m(2, 0) * invLen;

    float dot = Q(0, 0) * m(0, 1) + Q(1, 0) * m(1, 1) + Q(2, 0) * m(2, 1);
    Q(0, 1) = m(0, 1) - dot * Q(0, 0);
    Q(1, 1) = m(1, 1) - dot * Q(1, 0);
    Q(2, 1) = m(2, 1) - dot * Q(2, 0);
    invLen = 1.0f / std::sqrt(Q(0, 1) * Q(0, 1) + Q(1, 1) * Q(1, 1) + Q(2, 1) * Q(2, 1));
    Q(0, 1) *= invLen;
    Q(1, 1) *= invLen;
    Q(2, 1) *= invLen;

    dot = Q(0, 0) * m(0, 2) + Q(1, 0) * m(1, 2) + Q(2, 0) * m(2, 2);
    Q(0, 2) = m(0, 2) - dot * Q(0, 0);
    Q(1, 2) = m(1, 2) - dot * Q(1, 0);
    Q(2, 2) = m(2, 2) - dot * Q(2, 0);
    dot = Q(0, 1) * m(0, 2) + Q(1, 1) * m(1, 2) + Q(2, 1) * m(2, 2);
    Q(0, 2) -= dot * Q(0, 1);
    Q(1, 2) -= dot * Q(1, 1);
    Q(2, 2) -= dot * Q(2, 1);
    invLen = 1.0f / std::sqrt(Q(0, 2) * Q(0, 2) + Q(1, 2) * Q(1, 2) + Q(2, 2) * Q(2, 2));
    Q(0, 2) *= invLen;
    Q(1, 2) *= invLen;
    Q(2, 2) *= invLen;

    // guarantee that orthogonal matrix has determinant 1 (no reflections)
    const float det = Q(0, 0) * Q(1, 1) * Q(2, 2) + Q(0, 1) * Q(1, 2) * Q(2, 0) +
            Q(0, 2) * Q(1, 0) * Q(2, 1) - Q(0, 2) * Q(1, 1) * Q(2, 0) -
            Q(0, 1) * Q(1, 0) * Q(2, 2) - Q(0, 0) * Q(1, 2) * Q(2, 1);
    if (det < 0.0f)
        Q *= -1.0f;

    // build "right" matrix R
    QMatrix3x3 R(Qt::Uninitialized);
    R(0, 0) = Q(0, 0) * m(0, 0) + Q(1, 0) * m(1, 0) + Q(2, 0) * m(2, 0);
    R(0, 1) = Q(0, 0) * m(0, 1) + Q(1, 0) * m(1, 1) + Q(2, 0) * m(2, 1);
    R(1, 1) = Q(0, 1) * m(0, 1) + Q(1, 1) * m(1, 1) + Q(2, 1) * m(2, 1);
    R(0, 2) = Q(0, 0) * m(0, 2) + Q(1, 0) * m(1, 2) + Q(2, 0) * m(2, 2);
    R(1, 2) = Q(0, 1) * m(0, 2) + Q(1, 1) * m(1, 2) + Q(2, 1) * m(2, 2);
    R(2, 2) = Q(0, 2) * m(0, 2) + Q(1, 2) * m(1, 2) + Q(2, 2) * m(2, 2);

    // the scaling component
    D[0] = R(0, 0);
    D[1] = R(1, 1);
    D[2] = R(2, 2);

    // the shear component
    U[0] = R(0, 1) / D[0];
    U[1] = R(0, 2) / D[0];
    U[2] = R(1, 2) / D[1];
}

bool AkTransformMath::hasScale(const QMatrix4x4 &m)
{
    // If the columns are orthonormal and form a right-handed system, then there is no scale
    float t(m.determinant());
    if (!qFuzzyIsNull(t - 1.0f))
        return true;
    t = m(0, 0) * m(0, 0) + m(1, 0) * m(1, 0) + m(2, 0) * m(2, 0);
    if (!qFuzzyIsNull(t - 1.0f))
        return true;
    t = m(0, 1) * m(0, 1) + m(1, 1) * m(1, 1) + m(2, 1) * m(2, 1);
    if (!qFuzzyIsNull(t - 1.0f))
        return true;
    t = m(0, 2) * m(0, 2) + m(1, 2) * m(1, 2) + m(2, 2) * m(2, 2);
    if (!qFuzzyIsNull(t - 1.0f))
        return true;
    return false;
}

void AkTransformMath::decomposeQMatrix4x4(const QMatrix4x4 &m, QVector3D &position, QQuaternion &orientation, QVector3D &scale)
{
    Q_ASSERT(m.isAffine());

    const QMatrix3x3 m3x3(m.toGenericMatrix<3, 3>());

    QMatrix3x3 rot3x3(Qt::Uninitialized);
    if (hasScale(m)) {
        decomposeQMatrix3x3(m3x3, rot3x3, scale, position);
    } else {
        // we know there is no scaling part; no need for QDU decomposition
        scale = QVector3D(1.0f, 1.0f, 1.0f);
        rot3x3 = m3x3;
    }
    orientation = QQuaternion::fromRotationMatrix(rot3x3);
    position = QVector3D(m(0, 3), m(1, 3), m(2, 3));
}

QQuaternion AkTransformMath::fromAxisAndAngle(const QVector3D &axis, float angle)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

QQuaternion AkTransformMath::fromAxisAndAngle(float x, float y, float z, float angle)
{
    return QQuaternion::fromAxisAndAngle(x, y, z, angle);
}

QQuaternion AkTransformMath::fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                          const QVector3D &axis2, float angle2)
{
    const QQuaternion q1 = QQuaternion::fromAxisAndAngle(axis1, angle1);
    const QQuaternion q2 = QQuaternion::fromAxisAndAngle(axis2, angle2);
    return q2 * q1;
}

QQuaternion AkTransformMath::fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                          const QVector3D &axis2, float angle2,
                                          const QVector3D &axis3, float angle3)
{
    const QQuaternion q1 = QQuaternion::fromAxisAndAngle(axis1, angle1);
    const QQuaternion q2 = QQuaternion::fromAxisAndAngle(axis2, angle2);
    const QQuaternion q3 = QQuaternion::fromAxisAndAngle(axis3, angle3);
    return q3 * q2 * q1;
}

QQuaternion AkTransformMath::fromAxes(const QVector3D &xAxis, const QVector3D &yAxis, const QVector3D &zAxis)
{
    return QQuaternion::fromAxes(xAxis, yAxis, zAxis);
}


QQuaternion AkTransformMath::fromEulerAngles(const QVector3D &eulerAngles)
{
    return QQuaternion::fromEulerAngles(eulerAngles);
}

QQuaternion AkTransformMath::fromEulerAngles(float pitch, float yaw, float roll)
{
    return QQuaternion::fromEulerAngles(pitch, yaw, roll);
}

QMatrix4x4 AkTransformMath::rotateAround(const QVector3D &point, float angle, const QVector3D &axis)
{
    QMatrix4x4 m;
    m.translate(point);
    m.rotate(angle, axis);
    m.translate(-point);
    return m;
}

QMatrix4x4 AkTransformMath::rotateFromAxes(const QVector3D &xAxis, const QVector3D &yAxis, const QVector3D &zAxis)
{
    return QMatrix4x4(xAxis.x(), yAxis.x(), zAxis.x(), 0.0f,
                      xAxis.y(), yAxis.y(), zAxis.y(), 0.0f,
                      xAxis.z(), yAxis.z(), zAxis.z(), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);
}

float AkTransformMath::vectorRadian(const QVector3D& vec1, const QVector3D& vec2){
    return qAcos(QVector3D::dotProduct(vec1.normalized(), vec2.normalized()));;
}

float AkTransformMath::vectorAngle(const QVector3D& vec1, const QVector3D& vec2){
    return vectorRadian(vec1, vec2) * 180.0 / M_PI;
}
