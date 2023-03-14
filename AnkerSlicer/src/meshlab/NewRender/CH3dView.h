#ifndef CH3dView_H
#define CH3dView_H

#include "common/GeoAndShow/CHAABB3D.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include "CHRenderData.h"

using namespace std;



class CH3dView : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    CH3dView(QWidget* parent = Q_NULLPTR);
    ~CH3dView() Q_DECL_OVERRIDE;


public:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void calcSceneParamsFromBox(const CHAABB3D& aabb);

    
    void resizeShowWindow(vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp, QVector3D& outEye,
        QVector3D& outUP, QVector3D& outFront, double scale = 1.0);

    
    void viewShowWindow(std::vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp, QVector3D& outEye,
        QVector3D& outUp, QVector3D& outFront, double scale = 1.0);
    QMatrix4x4 rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos);

    void GetNearFarPoint(float x, float y, QVector3D& np, QVector3D& fp);
    void pixelToWorld(float x, float y, float z, QVector3D& worldCoord);
    void worldToPixel(QVector3D worldCoord, float& x, float& y, float& z);

    void setPerspectiveTransformationRef(QVector3D rotCenter);

    void getMechineBoxSize(float &length, float &width, float &height);

    
    enum ViewType
    {
        NORMAL_VIEW_TYPE = 0,
        JUST_VIEW_TYPE = 1,
        FRONT_VIEW_TYPE,
        BACK_VIEW_TYPE,
        LEFT_VIEW_TYPE,
        RIGHT_VIEW_TYPE,
        TOP_VIEW_TYPE,
        BOTTOM_VIEW_TYPE,
    };
    struct ViewVec
    {
        QVector3D eye;
        QVector3D up;
        QVector3D front;
    };
    void setView(const ViewType& _type, const CHAABB3D& aabb);
    void getBoxPoints(const QVector3D& iMin, const QVector3D& iMax, std::vector<QVector3D>& points);

public slots:
    void machinePlatformSizeChanged(float length, float width, float height);

private:

    void initView();
    void Scale(int x, int y, float d);
    void xyTranslate(int dx, int dy);
    void xyRotate(int dx, int dy);
    void getPointWorldLocationOnFixedProjMatrix(float dx, float dy, QMatrix4x4& invertMatrix,
        QVector3D& outpt, float js = 1.0);
    void getMoveVec(int dx, int dy, QVector3D& move);
    void getZoomVec(int dx, int dy, QVector3D& move);

    void getCurrentProjMat(QMatrix4x4& matrix);
    void getCurrentViewMat(QMatrix4x4& matrix);
    void getZoomViewMat(QMatrix4x4& matrix);

    void paintMeshVbo(QOpenGLShaderProgram& shaderProgram, CHMeshShowObjPtr mesh, CHRenderMeshPtr rm, QMatrix4x4& pvMatrix);
    void paintCurveVbo(QOpenGLShaderProgram& shaderProgram, CHCurveShowObjPtr curve, CHRenderCurvePtr rc, QMatrix4x4& pvMatrix);
    void paintPointVbo(QOpenGLShaderProgram& shaderProgram, CHPointShowObjPtr point, CHRenderPointSpherePtr rps, QMatrix4x4& pvMatrix);

private:
    QVector3D m_eye;
    QVector3D m_up;
    QVector3D m_front;
    double m_verticalAngle;
    float m_near;
    float m_far;
    int m_w;
    int m_h;

    QVector3D lastmousept;
    bool m_bMove;

    QVector3D cameralocation;
    QVector3D lockN;
    QVector3D lockV;
    QVector3D lockU;
    QVector3D refTranPoint;
    double senseRadius;

    double A;
    QMatrix4x4 invertMatrix;

    QOpenGLTexture* texture1 = nullptr;
    QOpenGLTexture* texture2 = nullptr;

    bool m_controlKeyDown;

    bool m_viewTransforming;
    bool m_viewRotating; 
    bool m_viewMoving; 

    CHAABB3D m_sceneBox;
    ViewType m_viewType;

    ViewVec m_ViewVec[8]; 

    float m_length;
    float m_width;
    float m_height;
};
#endif
