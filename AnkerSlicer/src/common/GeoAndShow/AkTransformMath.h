#ifndef AKTRANSFORMMATH_H
#define AKTRANSFORMMATH_H

#include <QQuaternion>
#include <QMatrix4x4>
#include <QtMath>


inline QVector3D operator*(const QMatrix3x3 & matrix, const QVector3D & vector){
    float x, y, z;
    x = vector.x() * matrix(0, 0) + vector.y() * matrix(1, 0) + vector.z() * matrix(2, 0);
    y = vector.x() * matrix(0, 1) + vector.y() * matrix(1, 1) + vector.z() * matrix(2, 1);
    z = vector.x() * matrix(0, 2) + vector.y() * matrix(1, 2) + vector.z() * matrix(2, 2);
    return QVector3D(x, y, z);
};


class AkTransformMath
{
public:
    //  TRS <==> Matrix4x4
    static void composeQMatrix4x4(const QVector3D &position, const QQuaternion &orientation, const QVector3D &scale, QMatrix4x4 &m);
    static void decomposeQMatrix3x3(const QMatrix3x3 &m, QMatrix3x3 &Q, QVector3D &D, QVector3D &U);
    static bool hasScale(const QMatrix4x4 &m);
    static void decomposeQMatrix4x4(const QMatrix4x4 &m, QVector3D &position, QQuaternion &orientation, QVector3D &scale);

    
    static QQuaternion fromAxisAndAngle (float x, float y, float z, float angle);
    static QQuaternion fromAxisAndAngle (const QVector3D &axis , float angle );
    static QQuaternion fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                         const QVector3D &axis2, float angle2);
    static QQuaternion fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                         const QVector3D &axis2, float angle2,
                                         const QVector3D &axis3, float angle3);
    static QQuaternion fromAxes(const QVector3D &xAxis, const QVector3D &yAxis, const QVector3D &zAxis);

    
    static QQuaternion fromEulerAngles(const QVector3D &eulerAngles);
    static QQuaternion fromEulerAngles(float pitch, float yaw, float roll);

    static QMatrix4x4 rotateAround(const QVector3D &point, float angle, const QVector3D &axis);
    static QMatrix4x4 rotateFromAxes(const QVector3D &xAxis, const QVector3D &yAxis, const QVector3D &zAxis);

    
    static float vectorRadian(const QVector3D& vec1, const QVector3D& vec2);
    static float vectorAngle (const QVector3D& vec1, const QVector3D& vec2);

    
    static QMatrix4x4 fixedPointRotation();
    
    static QMatrix4x4 matrixBetweenTwoRirectionalRays(const QVector3D srcPos,  const QVector3D srcDir,
                                                      const QVector3D destPos, const QVector3D destDir,
                                                      const QVector3D *srcRight = nullptr)
    {

        QMatrix4x4 matrix;
        float angle = vectorAngle(srcDir, destDir);
        QVector3D axis = QVector3D::crossProduct(srcDir, destDir);

        
        if(qFuzzyCompare(angle, 0.0f)){
            matrix.translate(destPos - srcPos);
            return matrix;
        }

        
        if(!qFuzzyCompare(angle, 180.0f)){
            matrix.translate(destPos);
            matrix.rotate(angle, axis);
            matrix.translate(-srcPos);
            return matrix;
        }
        
        else{ // qFuzzyCompare(angle, 180.0f)
            if (axis.isNull()){     
                axis = QVector3D::crossProduct(srcDir, QVector3D(1.0, 1.5, 2.0));
                if (axis.isNull()){
                    axis = QVector3D::crossProduct(srcDir, QVector3D(-1.0, 1.5, -2.0));
                }
            }
            matrix.translate(destPos);
            matrix.rotate(angle, axis);
            matrix.translate(-srcPos);
            return matrix;
        }
    };
};

#endif // AKTRANSFORMMATH_H
