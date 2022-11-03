#include "AkTransform.h"
#include "AkTransformMath.h"

template <typename T>
static QString Qobj2Str(T q){
    QString str;
    QDebug dbg(&str);
    dbg << q;
    return str;
}

/*================ class AkTransformData ================*/

QVector3D AkTransformData::Zero3D = QVector3D(0,0,0);
QVector3D AkTransformData::Ones3D = QVector3D(1,1,1);

AkTransformData::AkTransformData(){
    matParam.scale = Zero3D;
    matParam.eulerRotationAngles = matParam.translation = Zero3D;
    r4d = QQuaternion();
    matrix = QMatrix4x4();
}

QMatrix4x4 AkTransformData::getMatrix4x4() {
    r4d=QQuaternion::fromEulerAngles(matParam.eulerRotationAngles);
    AkTransformMath::composeQMatrix4x4(matParam.translation, r4d, matParam.scale, matrix);
    return matrix;
}

void AkTransformData::setMatrix4x4(const QMatrix4x4 &mat44){
    matrix = mat44;
    AkTransformMath::decomposeQMatrix4x4(mat44, matParam.translation, r4d, matParam.scale);
    matParam.eulerRotationAngles = r4d.toEulerAngles();
}

QString AkTransformData::toString(){
    return QString("S %1, E %2, T %3, R %4").arg(Qobj2Str(matParam.scale),-24).arg(Qobj2Str(matParam.eulerRotationAngles),-24).arg(Qobj2Str(matParam.translation),-24).arg(Qobj2Str(r4d),-24);
}



/*================ class AkTransform : QObject ================*/
/// matrix
QMatrix4x4 AkTransform::matrix() const
{
    if (m_data.matrixDirty) {
        AkTransformMath::composeQMatrix4x4(m_data.matParam.translation, m_data.rotation, m_data.matParam.scale, m_data.matrix);
        m_data.matrixDirty = false;
    }
    return m_data.matrix;
}

void AkTransform::setMatrix(const QMatrix4x4 &m)
{
    if (m != matrix()) {
        m_data.matrix = m;
        m_data.matrixDirty = false;

        const QVector3D oldEuler = m_data.matParam.eulerRotationAngles;
        QVector3D s;
        QVector3D t;
        QQuaternion r;
        AkTransformMath::decomposeQMatrix4x4(m, t, r, s);
        m_data.matParam.scale = s;
        m_data.rotation = r;
        m_data.matParam.translation = t;
        m_data.matParam.eulerRotationAngles = m_data.rotation.toEulerAngles();
        emit scale3DChanged(s);
        emit rotationQuaternionChanged(r);
        emit translationChanged(t);
        emit_EulerChanged(oldEuler);
        emit matrixChanged();
    }
}



//  get
float AkTransform::scale() const
{
    return m_data.matParam.scale.x();
}
QVector3D AkTransform::scale3D() const
{
    return m_data.matParam.scale;
}
//  set
void AkTransform::setScale3D(const QVector3D &s)
{
    if (s != m_data.matParam.scale) {
        m_data.matParam.scale = s;
        m_data.matrixDirty = true;
        emit scale3DChanged(s);
        emit matrixChanged();
    }
}

void AkTransform::setScale(float s)
{
    if (s != m_data.matParam.scale.x()) {
        setScale3D(QVector3D(s, s, s));
        emit scaleChanged(s);
    }
}



QVector3D AkTransform::translation() const
{
    return m_data.matParam.translation;
}
void AkTransform::setTranslation(const QVector3D &t)
{
    if (t != m_data.matParam.translation) {
        m_data.matParam.translation = t;
        m_data.matrixDirty = true;

        emit translationChanged(t);
        emit matrixChanged();
    }
}



QQuaternion AkTransform::rotationQuaternion() const
{
    return m_data.rotation;
}

QVector3D AkTransform::rotationEulerAngles() const
{
    return m_data.matParam.eulerRotationAngles;
}


void AkTransform::setRotationQuaternion(const QQuaternion &quater)
{
    if (quater != m_data.rotation) {
        m_data.rotation = quater;
        const QVector3D oldEuler = m_data.matParam.eulerRotationAngles;
        m_data.matParam.eulerRotationAngles = m_data.rotation.toEulerAngles();  
        m_data.matrixDirty = true;

        emit rotationQuaternionChanged(quater);
        emit_EulerChanged(oldEuler);
        emit matrixChanged();
    }
}

void AkTransform::setRotationEulerAngles(const QVector3D &euler)
{
    if (m_data.matParam.eulerRotationAngles == euler) { return; }

    const QVector3D oldEuler = m_data.matParam.eulerRotationAngles;
    m_data.matParam.eulerRotationAngles = euler;
    QQuaternion rotation = QQuaternion::fromEulerAngles(m_data.matParam.eulerRotationAngles); 
    if (rotation != m_data.rotation) {
        m_data.rotation = rotation;
        m_data.matrixDirty = true;
        emit rotationQuaternionChanged(rotation);
    }

    emit_EulerChanged(oldEuler);
    emit matrixChanged();
}


void AkTransform::set_RotationEulerXYZAngles(float angle, Qt::Axis axis)
{
    if (m_data.matParam.eulerRotationAngles[axis] == angle)
        return;

    QVector3D newEuler = m_data.matParam.eulerRotationAngles;
    newEuler[axis] = angle;

    setRotationEulerAngles(newEuler);
}

void AkTransform::emit_EulerChanged(const QVector3D &oldEuler)
{
    if(m_data.matParam.eulerRotationAngles != oldEuler){
        emit rotationEulerAnglesChanged(m_data.matParam.eulerRotationAngles);
        if (m_data.matParam.eulerRotationAngles.x() != oldEuler.x())
            emit rotationEulerXChanged(m_data.matParam.eulerRotationAngles.x());
        if (m_data.matParam.eulerRotationAngles.y() != oldEuler.y())
            emit rotationEulerYChanged(m_data.matParam.eulerRotationAngles.y());
        if (m_data.matParam.eulerRotationAngles.z() != oldEuler.z())
            emit rotationEulerZChanged(m_data.matParam.eulerRotationAngles.z());
    }
}










