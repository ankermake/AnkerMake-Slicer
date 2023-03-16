#ifndef AKTRANSFORM_H
#define AKTRANSFORM_H

#include <QObject>
#include <QQuaternion>
#include <QMatrix4x4>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

/*================ class AkTransformData ================*/
struct COMMONLIB_EXPORT AkTransformData
{
    //#ifdef _WIN32
    //    constexpr static QVector3D Zero3D = QVector3D(0,0,0);
    //    constexpr static QVector3D Ones3D = QVector3D(1,1,1);

    //#elif __APPLE__
    //    static QVector3D Zero3D;
    //    static QVector3D Ones3D;
    //#endif

    static QVector3D Zero3D;
    static QVector3D Ones3D;

    struct Param{
        QVector3D scale;
        QVector3D eulerRotationAngles;   
        QVector3D translation;
    }matParam;



    union{ QQuaternion r4d, rotation; };
    mutable QMatrix4x4 matrix;
    mutable bool matrixDirty;

    /// --------
    AkTransformData();
    inline void clean(){ *this = AkTransformData();}
    QMatrix4x4  getMatrix4x4();
    void        setMatrix4x4(const QMatrix4x4 & mat44);
    QString     toString();
};


/*================ class AkTransform : QObject ================*/
class COMMONLIB_EXPORT AkTransform : public QObject
{
    Q_OBJECT
    AkTransformData m_data;
public:
    QMatrix4x4   matrix() const;
    float        scale() const;
    QVector3D    scale3D() const;
    QVector3D    translation() const;

    QQuaternion  rotationQuaternion()  const;
    QVector3D    rotationEulerAngles() const;
    inline float rotationEulerXAngle() const {return rotationEulerAngles()[Qt::XAxis];}
    inline float rotationEulerYAngle() const {return rotationEulerAngles()[Qt::YAxis];}
    inline float rotationEulerZAngle() const {return rotationEulerAngles()[Qt::ZAxis];}

public Q_SLOTS:
    
    void setMatrix      (const QMatrix4x4 &matrix);
    void setScale       (float scale);
    void setScale3D     (const QVector3D &scale);
    void setTranslation (const QVector3D &translation);

    void setRotationQuaternion (const QQuaternion &quater);
    void setRotationEulerAngles(const QVector3D   &euler );
    void setRotationEulerX(float angle) { set_RotationEulerXYZAngles(angle, Qt::XAxis);}
    void setRotationEulerY(float angle) { set_RotationEulerXYZAngles(angle, Qt::YAxis);}
    void setRotationEulerZ(float angle) { set_RotationEulerXYZAngles(angle, Qt::ZAxis);}

    
    void setDeltaMatrix     (const QMatrix4x4 &m){ setMatrix     (m * matrix())     ;}
    void setDeltaScale      (float s)            { setScale      (s * scale())      ;}
    void setDeltaScale3D    (const QVector3D &s) { setScale3D    (s * scale3D())    ;}
    void setDeltaTranslation(const QVector3D &t) { setTranslation(t + translation());}

    void setDeltaRotationQuaternion(const QQuaternion &quater) { setRotationQuaternion (quater * rotationQuaternion()) ;}
    void setDeltaRotationEulerAngles(const QVector3D  &euler ) { setRotationEulerAngles(euler  + rotationEulerAngles());}
    void setDeltaRotationEulerX(float angle)     { setRotationEulerX(angle + rotationEulerXAngle());}
    void setDeltaRotationEulerY(float angle)     { setRotationEulerX(angle + rotationEulerYAngle());}
    void setDeltaRotationEulerZ(float angle)     { setRotationEulerX(angle + rotationEulerZAngle());}

Q_SIGNALS:
    void matrixChanged();
    void scaleChanged(float scale);
    void scale3DChanged(const QVector3D &scale);
    void translationChanged(const QVector3D &translation);

    void rotationQuaternionChanged(const QQuaternion & quater);
    void rotationEulerAnglesChanged(const QVector3D  & euler);
    void rotationEulerXChanged(float angle);   //
    void rotationEulerYChanged(float angle);   //
    void rotationEulerZChanged(float angle);   //

private:
    void set_RotationEulerXYZAngles(float angle, Qt::Axis axis);
    void emit_EulerChanged(const QVector3D &oldEuler);
};

#endif // AKTRANSFORM_H
