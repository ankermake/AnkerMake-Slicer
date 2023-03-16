#include "coordinate.h"
#include "TransformPack.h"

CoordinateAxis::CoordinateAxis()
{

}

CoordinateAxis::~CoordinateAxis()
{
}

void CoordinateAxis::setColor(QColor color)
{
    m_axis->setColor(color);
    m_arrow->setColor(color);
}

void CoordinateAxis::setWidth(float width)
{
    m_axis->setLineWide(width);
}

void CoordinateAxis::setLightTest(bool lightTest)
{
    m_axis->setLightTest(lightTest);
    m_axis->updateToScene();
}

void CoordinateAxis::create(const QVector3D& center, const QVector3D& axisDir, float axisLenth, float rad, float height)
{
    setDirty(true);

    QVector3D et = center + axisDir * axisLenth;
    m_axis = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    m_axis->create(center, et);
    //m_axis->setLineWide(5.0);

    m_arrow = CHConePtr(new CHCone);
    m_arrow->create(rad, height);

    QMatrix4x4 matrix1 = TransformPack::rotMat(QVector3D(0, 0, 1), axisDir, QVector3D(0, 0, 0));
    QMatrix4x4 matrix2;
    matrix2.translate(et);
    m_arrow->transform(matrix2 * matrix1);

    m_baseShowObjs.push_back(m_axis);
    m_baseShowObjs.push_back(m_arrow);
}

void CoordinateAxis::updateCoordinateAxis(const QVector3D& center, const QVector3D& axisDir, float axisLenth, float rad, float height)
{
    if(m_axis == nullptr || m_arrow == nullptr) 
    {
        return;
    }
    setDirty(true);
    QVector3D et = center + axisDir * axisLenth;
    m_axis->create(center, et);
    m_arrow->create(rad, height);

    QMatrix4x4 matrix1 = TransformPack::rotMat(QVector3D(0, 0, 1), axisDir, QVector3D(0, 0, 0));
    QMatrix4x4 matrix2;
    matrix2.translate(et);
    m_arrow->transform(matrix2 * matrix1);
}

bool CoordinateAxis::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
    m_axis->setTransform(getTransform());
    if (!m_axis->pick(pixelX, pixelY, result, pickTol))
    {
        return false;
    }
    result.m_pickObj = shared_from_this();
    return true;
}
