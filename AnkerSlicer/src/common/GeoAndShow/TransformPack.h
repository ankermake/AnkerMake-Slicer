/*



*/

#ifndef TransformPack_H
#define TransformPack_H

#include "QMatrix4x4"
#include "QVector3D"


class TransformPack
{
public:

	static QMatrix4x4 rotMat(const QVector3D& pos, const QVector3D& axis, float angle);
	static QMatrix4x4 rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos);
	static QMatrix4x4 scale(const QVector3D& pos, float scaleX, float scaleY, float scaleZ);
	static QVector3D pRot(QMatrix4x4 matrix, QVector3D pt);
	static QVector3D vRot(QMatrix4x4 matrix, QVector3D vec);
	static float vecAngle(QVector3D vec1, QVector3D vec2);

};

#endif
