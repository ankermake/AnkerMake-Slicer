/****************************************************************************
Revision 0.1  2021/12/18 11:57:03  Aden Hu
****************************************************************************/

#ifndef GCODE_SCENE3D_H
#define GCODE_SCENE3D_H

#include <QWidget>
#include <QMatrix4x4>

#include <vector>
#include "common/GeoAndShow/SceneParam.h"
#include "renderscene.h"
#define PI 3.14159265f
#define EPSINON 0.0001
#define MAXNUM 999999999
#define MINNUM -999999999

struct BoxType
{
    QVector3D iMax = QVector3D(235, 235, 250);
    QVector3D iMin = QVector3D(0, 0, 0);
    float iTick = 10.0f;
    float iLineWidth = 0.1f;
    float iDiv = 20.0f;
    QColor iColor = QColor(0.0, 0.0, 0.0);
    QColor iBottomTriColor = QColor((int)(0.376 * 255), (int)(0.376 * 255), (int)(0.376 * 255), (int)(0.5 * 255));

    inline BoxType& operator=(const BoxType& type)
    {
        iMax = type.iMax;
        iMin = type.iMin;
        iTick = type.iTick;
        iLineWidth = type.iLineWidth;
        iDiv = type.iDiv;
        iColor = type.iColor;
        iBottomTriColor = type.iBottomTriColor;
        return *this;
    }
};

struct CoordinateType
{
    QMatrix4x4 m_matrix;
    float m_size = 10.0f;  
    QVector3D m_center = QVector3D(0, 0, 0);
    float m_linewidth = 1.5f;  

    inline CoordinateType& operator=(const CoordinateType& coord)
    {
        m_matrix = coord.m_matrix;
        m_size = coord.m_size;
        m_center = coord.m_center;
        m_linewidth = coord.m_linewidth;
        return *this;
    }
};

enum ViewType
{
    NORMAL_VIEW_TYPE = 0,
    JUST_VIEW_TYPE,
    FRONT_VIEW_TYPE, 
    BACK_VIEW_TYPE,
    LEFT_VIEW_TYPE,
    RIGHT_VIEW_TYPE,
    TOP_VIEW_TYPE,
    BOTTOM_VIEW_TYPE,
    OTHER_VIEW_TYPE
};

class Scene3D
{
public:
    Scene3D(QWidget* widget, const SceneParam& param);
    ~Scene3D();

public:
    void showObjectsCentral(std::vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp,
        QVector3D& outEye, QVector3D& outUP, QVector3D& outFront, double scale = 1.0);
    void resizeWidget(int w, int h);

    void scale(int x, int y, float d);
    void xyTranslate(int dx, int dy);
    void xyRotate(int dx, int dy);
    void getNearFarPoint(int x, int y, QVector3D& np, QVector3D& fp);
    void setPerspectiveTransformationRef(QVector3D rotCenter);
    void getPointWorldLocationOnFixedProjMatrix(int dx, int dy, QMatrix4x4& invertMatrix, QVector3D& outpt, float js = 1.0);
    void getMoveVec(int dx, int dy, QVector3D& move);
    void getZoomVec(int dx, int dy, QVector3D& move);

    
    void getCurrentProjMat_ssp(QMatrix4x4& matrix);
    void getCurrentProjMat_sp(QMatrix4x4& matrix);
    void getCurrentProjMat(QMatrix4x4& matrix);
    void getCurrentViewMat(QMatrix4x4& matrix);
    void getZoomViewMat(QMatrix4x4& matrix);

    QMatrix4x4 rotMat(const QVector3D& pos, const QVector3D& axis, double angle);
    QMatrix4x4 rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos);
    QVector3D pRot(QMatrix4x4 matrix, QVector3D pt);
    QVector3D vRot(QMatrix4x4 matrix, QVector3D vec);
    double vecAngle(QVector3D vec1, QVector3D vec2);

    void calcSceneParamsFromBox(const CHAABB3D& aabb);

    bool getIsMousePress() const;
    bool getIsControlKeyDown() const;

    void setBMove(bool is);
    void setMousePress(bool is);
    void setControlKeyDown(bool is);

    void reset();
    void setBBox(const BoxType& _box);
    void setCenter();
    void drawBBox();

    void setView(const ViewType& _type);
    ViewType getVIew();
    void drawScene();
    void drawWorldCoordinate();
    void drawCoordinate(const CoordinateType& _coord);

    void getBoxPoints(const BoxType& box, std::vector<QVector3D>& points);
    void getBoxPoints(const CHAABB3D& box, std::vector<QVector3D>& points);

    CHAABB3D getMainbox() const;

    void setEye(float x, float y, float z);
    void setUp(float x, float y, float z);
    void setRefTranPoint(float x, float y, float z);
    void setFront(float x, float y, float z);
    void setIsAiView();
    void setVerticalAngle(double setAngle);
    double getVerticalAngle();

    void setSceneParams(const SceneParam& param);
    QVector3D getEye() const;
    QVector3D getFront() const;
    QVector3D getUp() const;
    double getVerticalAngle() const;
    int getW() const;
    int getH() const;
private:
    QVector3D m_eye;
    QVector3D m_up;
    QVector3D m_front;//
    QVector3D m_viewCenter;
    bool isAiView = false;
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
    bool m_controlKeyDown;
    bool m_viewTransforming;
    bool m_mousepress;

    QWidget* m_widget;
    BoxType m_bbox;  
    CoordinateType m_worldCoordinate;    

    SceneParam m_sceneParam;
    CHAABB3D m_mainBox;

    ViewType m_viewType;
};

#endif // !GCODE_SCENE3D_H
