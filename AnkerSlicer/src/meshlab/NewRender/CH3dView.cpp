#include "CHChildDocWindow.h"
#include "CH3dView.h"
#include "QMatrix4x4"
#include "qevent.h"
#include "common/GeoAndShow/TransformPack.h"
#include "QCursor"
#include "common/ml_document/mesh_model.h"
#include "common/utilities/tlogger.h"
#include <QPainter>
#include <QPen>
#define EPSINON 0.000001
#define OutOfRangeColor QColor(255,0,0)


CH3dView::CH3dView(QWidget* parent)
    : QOpenGLWidget(parent), m_viewType(NORMAL_VIEW_TYPE)
{
    AkUtil::TFunction("");
    m_w = parent->width();
    m_h = parent->height();
    setMouseTracking(true);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    hide();

    m_verticalAngle = 30;
    m_near = 10;
    m_far = 10000;

    m_ViewVec[0].eye = QVector3D(106.066, -938.46, 870.233);
    m_ViewVec[0].front = QVector3D(0.00884618, 0.816991, -0.576583);
    m_ViewVec[0].up = QVector3D(-0.0115033, 0.57665, 0.81691);

    initView();
    refTranPoint = QVector3D(0, 0, 0);
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
        tan(m_verticalAngle / 360.0 * CH_PI);
    m_bMove = true;

    m_controlKeyDown = false;
    m_viewTransforming = false;
    m_viewRotating = false;
    m_viewMoving = false;

#ifdef _WIN32
    
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(128);
    setFormat(surfaceFormat);
#elif __APPLE__
#endif
}

CH3dView::~CH3dView()
{
    makeCurrent();

    delete texture1;
    delete texture2;

    doneCurrent();
}


void CH3dView::initView()
{
    m_eye = m_ViewVec[0].eye;
    m_front = m_ViewVec[0].front;
    m_up = m_ViewVec[0].up;
}

void CH3dView::initializeGL()
{
    initializeOpenGLFunctions();
#ifdef _WIN32
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
#elif __APPLE__
#endif
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    AkUtil::TFunction(QString("initializeGL"));

    QOpenGLShaderProgram& m_shaderProgram = qobject_cast<CHChildDocWindow*>(parentWidget())->getShaderProgram();
    bool success = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/NewRender/colortriangle.vert");
    if (!success) {
        AkUtil::TDebug(QString("shaderProgram addShaderFromSourceFile failed!") + m_shaderProgram.log());
        return;
    }

    success = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/NewRender/colortriangle.frag");
    if (!success) {
        AkUtil::TDebug(QString("shaderProgram addShaderFromSourceFile failed!") + m_shaderProgram.log());
        return;
    }

    success = m_shaderProgram.link();
    if (!success) {
        AkUtil::TDebug(QString("shaderProgram link failed!") + m_shaderProgram.log());
    }

    qobject_cast<CHChildDocWindow*>(parentWidget())->generateBasePointBuffer();

}

void CH3dView::resizeGL(int w, int h)
{
    m_w = w;
    m_h = h;
    glViewport(0, 0, w, h);
}

void CH3dView::paintGL()
{
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
    {
        QString dLog = QString("width:%1, height:%2").arg(w).arg(h);
        AkUtil::TDebug(dLog);
        return;
    }

    //glClearColor(147 / 255.0, 147 / 255.0, 147 / 255.0, 0.0);
    glClearColor(26 / 255.0, 26 / 255.0, 28 / 255.0, 0.0);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    QOpenGLShaderProgram& m_shaderProgram = qobject_cast<CHChildDocWindow*>(parentWidget())->getShaderProgram();
    m_shaderProgram.bind();

    QMatrix4x4 pMatrix;
    getCurrentProjMat(pMatrix);
    QMatrix4x4 vMatrix;
    getCurrentViewMat(vMatrix);
    QMatrix4x4 pvMatrix = pMatrix * vMatrix;

    m_shaderProgram.setUniformValue("useTexture", false);

    
    QVector3D lightColor(1.0f, 1.0f, 1.0f);
    QVector3D lightPos(0.0f, 0.0f, 200.0f);
    m_shaderProgram.setUniformValue("viewPos", m_eye);

    
    QVector3D lightscolor[10];
    QVector3D lightsPos[10];
    lightscolor[0] = QVector3D(1.0f, 1.0f, 1.0f);
    lightsPos[0] = m_eye;
    lightscolor[1] = QVector3D(1.0f, 1.0f, 1.0f);
    lightsPos[1] = QVector3D(0, -300, 200);
    m_shaderProgram.setUniformValueArray("lightColor", lightscolor, 10);
    m_shaderProgram.setUniformValueArray("lightPos", lightsPos, 10);
    m_shaderProgram.setUniformValue("lightNum", 1);

    
    
    CHAABB3D aabb;
    if (!inAnkerEditMode() && getGlobalPick()->m_selectedObjs.size() > 0)
    {
        aabb = getGlobalPick()->getPickedObjsAABB();

        QMatrix4x4 matrix1, matrix2;
        matrix1.scale(aabb.getLenX(), aabb.getLenY(), aabb.getLenZ());
        matrix2.translate(aabb.getLeftDownPoint());
        getDoc()->m_showPickedAABB->setTransform(matrix2 * matrix1);
        getDoc()->m_showPickedAABB->setVisuable(true);
    }
    else
    {
        getDoc()->m_showPickedAABB->setVisuable(false);
    }
    //getDoc()->m_showPickedAABB->injectPropertiesToChildren();

    CHRenderDataPtr m_renderData = qobject_cast<CHChildDocWindow*>(parentWidget())->getRenderData();

    
    std::vector<std::pair<CHCurveShowObjPtr, CHRenderCurvePtr>> otherCurves;
    std::vector<std::pair<CHMeshShowObjPtr, CHRenderMeshPtr>> otherMeshes;
    std::vector<CHPointShowObjPtr> otherPoints;

    
    {
        glEnable(GL_DEPTH_TEST);
        m_shaderProgram.setUniformValue("twoSideRender", false);
        //qDebug() << "m_allmeshes size: " << m_renderData->m_allmeshes.size();
        
        for (map<CHMeshShowObjPtr, CHRenderMeshPtr>::iterator mit = m_renderData->m_allmeshes.begin();
            mit != m_renderData->m_allmeshes.end(); mit++)
        {
            if (!mit->first->getVisuable())
            {
                continue;
            }
            if (mit->first->getLightTest())
            {
                paintMeshVbo(m_shaderProgram, mit->first, mit->second, pvMatrix);
            }
            else
            {
                otherMeshes.push_back(std::make_pair(mit->first, mit->second));
            }
        }
        
        m_shaderProgram.setUniformValue("calLight", false);
        for (map<CHCurveShowObjPtr, CHRenderCurvePtr>::iterator cit = m_renderData->m_allcurves.begin();
            cit != m_renderData->m_allcurves.end(); cit++)
        {
            if (!cit->first->getVisuable())
            {
                continue;
            }

            if (cit->first->getLightTest())
            {
                paintCurveVbo(m_shaderProgram, cit->first, cit->second, pvMatrix);
            }
            else
            {
                otherCurves.push_back(std::make_pair(cit->first, cit->second));
            }
        }
        
#ifdef _WIN32
        glLineWidth(1);
#elif __APPLE__
        //glLineWidth(1);
#endif

        
        set<CHPointShowObjPtr>::iterator pit = m_renderData->m_allpoints.begin();
        for (; pit != m_renderData->m_allpoints.end(); pit++)
        {
            if (!(*pit)->getVisuable())
            {
                continue;
            }

            if ((*pit)->getLightTest())
            {
                paintPointVbo(m_shaderProgram, *pit, m_renderData->m_baseShowPoint, pvMatrix);
            }
            else
            {
                otherPoints.push_back((*pit));
            }
        }
    }

    
    {
        glDisable(GL_DEPTH_TEST);
        m_shaderProgram.setUniformValue("twoSideRender", true);

        
        m_shaderProgram.setUniformValue("calLight", false);
        for (int i = 0; i < otherCurves.size(); i++)
        {
            paintCurveVbo(m_shaderProgram, otherCurves[i].first, otherCurves[i].second, pvMatrix);
        }
        
#ifdef _WIN32
        glLineWidth(1);
#elif __APPLE__
        //glLineWidth(1);
#endif

        
        for (int i = 0; i < otherMeshes.size(); i++)
        {
            paintMeshVbo(m_shaderProgram, otherMeshes[i].first, otherMeshes[i].second, pvMatrix);
        }
        
        for (int i = 0; i < otherPoints.size(); i++)
        {
            paintPointVbo(m_shaderProgram, otherPoints[i], m_renderData->m_baseShowPoint, pvMatrix);
        }
    }

    m_shaderProgram.release();


    
    QPainter painter;
    painter.begin(this);
    QPen pen;
    pen.setColor(QColor(102, 102, 102));

    //pen.setWidthF((double)(pixelWide) / height() * m_height);

    painter.setPen(pen);

    QFont font;

    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    painter.setFont(this->font());
    //QFontMetrics

    if (inAnkerEditMode() || getGlobalPick()->m_selectedObjs.size() == 0)
    {
        aabb = getGlobalPick()->getPickedObjsAABB();
    }
    if (getGlobalPick()->m_selectedObjs.size() > 0)
    {
        QString text = QString("Model Size: ") + QString::number(aabb.getLenX(), 'f', 2) + QString(" * ") +
            QString::number(aabb.getLenY(), 'f', 2) + QString(" * ") + QString::number(aabb.getLenZ(), 'f', 2) + QString(" mm");
        painter.drawText(this->width() - 246 - QFontMetrics(this->font()).width(text), 50, text);
    }

    painter.end();
}

void CH3dView::mouseMoveEvent(QMouseEvent* event)
{
    QPoint qPoint = event->pos();// - lastPos; // mouse move right x+, down y+
    if(getGlobalPick()->rightBtnPressed)
    {
        getGlobalPick()->mouseRightMoved = true;
        m_viewMoving = true;
    }
    if (m_viewRotating)
    {
        refTranPoint = m_sceneBox.getCenterPoint();
        //refTranPoint[2] = 0.0;
        xyRotate(qPoint.x(), qPoint.y());
        update();
    }
    else if (m_viewMoving)
    {
        refTranPoint = m_sceneBox.getCenterPoint();
        //refTranPoint[2] = 0.0;
        xyTranslate(qPoint.x(), qPoint.y());
        update();
    }
    else
    {
        getGlobalPick()->mouseMoveEvent(event);
    }
    return QOpenGLWidget::mouseMoveEvent(event);
}

void CH3dView::mouseReleaseEvent(QMouseEvent* event)
{
    m_bMove = true;

    QWidget::mouseReleaseEvent(event);
    if(m_viewMoving && event->button() == Qt::RightButton)
    {
        m_viewMoving = false;
    }
    if (m_viewMoving && event->button() == Qt::MiddleButton)
    {
        m_viewMoving = false;
    }
    else if (m_viewRotating && event->button() == Qt::LeftButton)
    {
        m_viewRotating = false;
    }
    else
    {
        getGlobalPick()->mouseReleaseEvent(event);
    }
    update();
    getGlobalPick()->rightBtnPressed = false; 
    getGlobalPick()->mouseRightMoved = false; 
    return QOpenGLWidget::mouseReleaseEvent(event);
}

void CH3dView::mousePressEvent(QMouseEvent* event)
{
    setFocus();

    if(Qt::RightButton == event->button())
    {
        getGlobalPick()->rightBtnPressed = true;
    }
    if ((getGlobalPick()->haveCanPickedObj() || getGlobalPick()->havePickedObjs()) || (event->button() == Qt::RightButton))
    {
        getGlobalPick()->mousePressEvent(event);
    }
    else
    {
        m_viewMoving = (event->button() == Qt::MiddleButton);
        m_viewRotating = (event->button() == Qt::LeftButton);
        update();
    }
    return QOpenGLWidget::mousePressEvent(event);
}

void CH3dView::mouseDoubleClickEvent(QMouseEvent* event)
{
    QWidget::mouseDoubleClickEvent(event);
    getGlobalPick()->mouseDoubleClickEvent(event);
    return QOpenGLWidget::mouseDoubleClickEvent(event);
}

void CH3dView::wheelEvent(QWheelEvent* event)
{
    setFocus();
    refTranPoint = m_sceneBox.getCenterPoint();
    int qwheel = event->delta();  // positive when wheel up, delta +/- 120 when wheel +/- 1
    double zoomscale = (double)(qwheel) / 2000.0;
    Scale(event->x(), event->y(), zoomscale);
    update();
    return QOpenGLWidget::wheelEvent(event);
}

void CH3dView::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);

    if (event->key() == Qt::Key::Key_Control)
    {
        m_controlKeyDown = true;
    }
    getGlobalPick()->keyPressEvent(event);
    update();
    return QOpenGLWidget::keyPressEvent(event);
}

void CH3dView::keyReleaseEvent(QKeyEvent* event)
{
    QWidget::keyReleaseEvent(event);

    if (event->key() == Qt::Key::Key_Control)
    {
        m_controlKeyDown = false;
    }

    getGlobalPick()->keyReleaseEvent(event);
    update();
    return QOpenGLWidget::keyReleaseEvent(event);
}

void CH3dView::calcSceneParamsFromBox(const CHAABB3D& aabb)
{
    m_verticalAngle = 30;
    m_near = 10;
    m_far = 10000;

    m_sceneBox = aabb;
    QVector3D center = aabb.getCenterPoint();

    m_eye = QVector3D(center.x(), -6 * center.y(), 5 * center.z());
    m_front = QVector3D(0.00884618, 0.816991, -0.576583);
    m_up = QVector3D(-0.0115033, 0.57665, 0.81691);

    refTranPoint = center;
    //refTranPoint[2] = 0.0;
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
        tan(m_verticalAngle / 360.0 * CH_PI);
    m_bMove = true;

    m_controlKeyDown = false;
    m_viewTransforming = false;
    m_viewRotating = false;
    m_viewMoving = false;
}

void CH3dView::resizeShowWindow(vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp, QVector3D& outEye,
    QVector3D& outUP, QVector3D& outFront, double scale)
{
    outUP = inUp;
    outFront = inFront;

    
    QMatrix4x4 mat1 = TransformPack::rotMat(outFront, QVector3D(0, 0, 1), QVector3D(0, 0, 0));
    QVector3D curAxisY = -outUP;
    curAxisY = TransformPack::vRot(mat1, curAxisY);
    double rotAngle = TransformPack::vecAngle(curAxisY, QVector3D(0, 1, 0));
    QVector3D rotAxis = QVector3D::crossProduct(curAxisY, QVector3D(0, 1, 0));
    if (fabs(rotAngle - 3.1415926) < 0.00000001)   
    {
        rotAxis = QVector3D(0, 0, 1);
    }
    QMatrix4x4 mat2 = TransformPack::rotMat(QVector3D(0, 0, 0), rotAxis, rotAngle);

    for (int i = 0; i < points.size(); i++)
    {
        points[i] = TransformPack::pRot(mat2 * mat1, points[i]);
    }

    double Xmin;
    double Ymin;
    double Xmax;
    double Ymax;
    Xmin = Ymin = 999999999;
    Xmax = Ymax = -999999999;
    double reductionZ = m_near;
    double ratio1 = tan(m_verticalAngle / 180.0 * 3.1415926 / 2.0);
    double ratio2 = ratio1 * m_w / m_h;
    for (int i = 0; i < points.size(); i++)
    {
        double x1 = points[i].x() + (points[i].z() - reductionZ) * ratio2;
        double x2 = points[i].x() - (points[i].z() - reductionZ) * ratio2;
        double y1 = points[i].y() + (points[i].z() - reductionZ) * ratio1;
        double y2 = points[i].y() - (points[i].z() - reductionZ) * ratio1;
        if (Xmin > x1)
        {
            Xmin = x1;
        }
        if (Xmax < x2)
        {
            Xmax = x2;
        }
        if (Ymin > y1)
        {
            Ymin = y1;
        }
        if (Ymax < y2)
        {
            Ymax = y2;
        }
    }

    double h1 = (Ymax - Ymin) / 2.0 / ratio1;
    double h2 = (Xmax - Xmin) / 2.0 / ratio2;
    double h = h1;
    if (h < h2)
    {
        h = h2;
    }

    m_eye = QVector3D((Xmin + Xmax) / 2.0, (Ymin + Ymax) / 2.0, reductionZ - h / /*1.0*/scale);
    m_eye = TransformPack::pRot((mat2 * mat1).inverted(), m_eye);
}

void CH3dView::viewShowWindow(std::vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp, QVector3D& outEye,
    QVector3D& outUp, QVector3D& outFront, double scale)
{
    outUp = inUp;
    outFront = inFront;

    
    QMatrix4x4 mat1 = rotMat(outFront, QVector3D(0, 0, 1), QVector3D(0, 0, 0));
    QVector3D curAxisY = -outUp;
    curAxisY = TransformPack::vRot(mat1, curAxisY);
    double rotAngle = TransformPack::vecAngle(curAxisY, QVector3D(0, 1, 0));
    QVector3D rotAxis = QVector3D::crossProduct(curAxisY, QVector3D(0, 1, 0));
    if (fabs(rotAngle - CH_PI) < EPSINON) 
    {
        rotAxis = QVector3D(0, 0, 1);
    }
    QMatrix4x4 mat2 = TransformPack::rotMat(QVector3D(0, 0, 0), rotAxis, rotAngle);

    for (int i = 0; i < points.size(); i++)
    {
        points[i] = TransformPack::pRot(mat2 * mat1, points[i]);
    }

    double Xmin;
    double Ymin;
    double Xmax;
    double Ymax;
    Xmin = Ymin = 999999999;
    Xmax = Ymax = -999999999;
    double reductionZ = m_near;
    double ratio1 = tan(m_verticalAngle / 180.0 * 3.1415926 / 2.0);
    double ratio2 = ratio1 * m_w / m_h;

    for (int i = 0; i < points.size(); i++)
    {
        double x1 = points[i].x() + (points[i].z() - reductionZ) * ratio2;
        double x2 = points[i].x() - (points[i].z() - reductionZ) * ratio2;
        double y1 = points[i].y() + (points[i].z() - reductionZ) * ratio1;
        double y2 = points[i].y() - (points[i].z() - reductionZ) * ratio1;
        if (Xmin > x1)
        {
            Xmin = x1;
        }
        if (Xmax < x2)
        {
            Xmax = x2;
        }
        if (Ymin > y1)
        {
            Ymin = y1;
        }
        if (Ymax < y2)
        {
            Ymax = y2;
        }
    }

    double h1 = (Ymax - Ymin) / 2.0 / ratio1;
    double h2 = (Xmax - Xmin) / 2.0 / ratio2;
    double h = h1;
    if (h < h2)
    {
        h = h2;
    }

    m_eye = QVector3D((Xmin + Xmax) / 2.0, (Ymin + Ymax) / 2.0, reductionZ - h / /*1.0*/scale);
    m_eye = TransformPack::pRot((mat2 * mat1).inverted(), m_eye);
}

QMatrix4x4 CH3dView::rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos)
{
    double angle = TransformPack::vecAngle(axis1, axis2);
    QVector3D axis = QVector3D::crossProduct(axis1, axis2);
    if (axis == QVector3D(0, 0, 0))
    {
        QVector3D ref1(1.0, 1.5, 2.0);
        QVector3D ref2(-1.0, 1.5, -2.0);

        if (fabs(angle - CH_PI) < EPSINON) 
        {
            QVector3D ref3(1.0, 0.0, 0.0);
            axis = QVector3D::crossProduct(axis1, ref3);
        }
        else
        {
            axis = QVector3D::crossProduct(axis1, ref1);
        }

        if (axis == QVector3D(0, 0, 0))
        {
            axis = QVector3D::crossProduct(axis1, ref2);
        }
    }
    return TransformPack::rotMat(pos, axis, angle);
}

void CH3dView::Scale(int x, int y, float d)
{
    QVector3D move;
    getZoomVec(x, y, move);
    m_eye = m_eye + move * d;

    
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
        tan(m_verticalAngle / 360.0 * 3.1415926);
}

void CH3dView::xyTranslate(int dx, int dy)
{
    QVector3D moveVec;
    getMoveVec(dx, dy, moveVec);
    m_eye = m_eye + moveVec * (-1);
}

void CH3dView::xyRotate(int dx, int dy)
{
    QVector3D moveVec;
    getMoveVec(dx, dy, moveVec);
    if (moveVec != QVector3D(0, 0, 0))
    {
        lastmousept.setX(lastmousept.x());
    }

    double H1 = QVector3D::dotProduct(lastmousept - refTranPoint, lockN);
    double H2 = QVector3D::dotProduct(lastmousept - refTranPoint, lockV);
    double L1 = QVector3D::dotProduct(moveVec, lockU) * 5.0;
    double L2 = QVector3D::dotProduct(moveVec, lockV) * 5.0;
    double Angle1 = L2 / senseRadius;
    double Angle2 = L1 / senseRadius;
    double A1 = H1 / senseRadius;
    double A2 = H2 / senseRadius;
    if (A1 > 1)
    {
        A1 = 1;
    }
    else if (A1 < -1)
    {
        A1 = -1;
    }
    if (A2 > 1)
    {
        A2 = 1;
    }
    else if (A2 < -1)
    {
        A2 = -1;
    }

    
    QVector3D curN = m_front;
    QVector3D curV = m_up;
    QVector3D curU = QVector3D::crossProduct(curN, curV);
    QVector3D axis1 = curU * sqrt(1 - A1 * A1) + curN * A1;
    QVector3D axis2 = curV * sqrt(1 - A2 * A2) + curN * A2;
    QMatrix4x4 mat;
    if(fabs(m_lastMousePts[0] - dx) > fabs(m_lastMousePts[1] - dy)) 
    {
        axis2 = QVector3D(0, 0, 1);
        mat = TransformPack::rotMat(refTranPoint, axis2, -Angle2);
    }
    else if(fabs(m_lastMousePts[0] - dx) < fabs(m_lastMousePts[1] - dy))
    {
        axis1 = curU;
        mat = TransformPack::rotMat(refTranPoint, axis1, Angle1);
    }
    else
    {
        mat = TransformPack::rotMat(refTranPoint, axis1, Angle1) * TransformPack::rotMat(refTranPoint, axis2, -Angle2);
    }
    //qDebug() << "dx: " << dx << ", dy: " << dy << ", lastmousept: " << lastmousept << ", moveVec: " << moveVec;
    m_lastMousePts[0] = dx;
    m_lastMousePts[1] = dy;
    m_eye = TransformPack::pRot(mat, m_eye);
    m_front = TransformPack::vRot(mat, m_front);
    m_up = TransformPack::vRot(mat, m_up);
}

void CH3dView::GetNearFarPoint(float x, float y, QVector3D& np, QVector3D& fp)
{
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    getCurrentProjMat(projMatrix);
    getCurrentViewMat(viewMatrix);
    invertMatrix = (projMatrix * viewMatrix).inverted();
    getPointWorldLocationOnFixedProjMatrix(x, y, invertMatrix, np, -1);
    getPointWorldLocationOnFixedProjMatrix(x, y, invertMatrix, fp, 1);
}

void CH3dView::pixelToWorld(float x, float y, float z, QVector3D& worldCoord)
{
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    getCurrentProjMat(projMatrix);
    getCurrentViewMat(viewMatrix);
    invertMatrix = (projMatrix * viewMatrix).inverted();
    getPointWorldLocationOnFixedProjMatrix(x, y, invertMatrix, worldCoord, z);
}

void CH3dView::worldToPixel(QVector3D worldCoord, float& x, float& y, float& z)
{
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    getCurrentProjMat(projMatrix);
    getCurrentViewMat(viewMatrix);

    QVector4D standardPixel = projMatrix * viewMatrix * QVector4D(worldCoord, 1.0);
    QVector3D out = standardPixel.toVector3DAffine()/* / standardPixel[3]*/;
    x = (1.0f + out.x()) * (float)m_w / 2.0f;
    y = (1.0f - out.y()) * (float)m_h / 2.0f;
    z = out.z();
}

/*
QVector3D globalEye = QVector3D(106.066, -938.46, 870.233);
QVector3D globalFront = QVector3D(0.00884618, 0.816991, -0.576583);
QVector3D globalUp = QVector3D(-0.0115033, 0.57665, 0.81691);
*/

void CH3dView::setView(const ViewType& _type, const CHAABB3D& aabb)
{
    //matrix.lookAt(m_eye, m_eye+m_front,m_up);
    QVector3D center = aabb.getCenterPoint();
    m_viewType = _type;
    switch (_type)
    {
    case ViewType::JUST_VIEW_TYPE:
        m_front = m_ViewVec[JUST_VIEW_TYPE].front;
        m_up = m_ViewVec[JUST_VIEW_TYPE].up;
        m_eye = m_ViewVec[JUST_VIEW_TYPE].eye;
        break;
    case ViewType::FRONT_VIEW_TYPE:
        m_front = m_ViewVec[FRONT_VIEW_TYPE].front;
        m_up = m_ViewVec[FRONT_VIEW_TYPE].up;
        m_eye = m_ViewVec[FRONT_VIEW_TYPE].eye;
        break;
    case ViewType::BACK_VIEW_TYPE:
        m_front = m_ViewVec[BACK_VIEW_TYPE].front;
        m_up = m_ViewVec[BACK_VIEW_TYPE].up;
        m_eye = m_ViewVec[BACK_VIEW_TYPE].eye;
        break;
    case ViewType::LEFT_VIEW_TYPE:
        m_front = m_ViewVec[LEFT_VIEW_TYPE].front;
        m_up = m_ViewVec[LEFT_VIEW_TYPE].up;
        m_eye = m_ViewVec[LEFT_VIEW_TYPE].eye;
        break;
    case ViewType::RIGHT_VIEW_TYPE:
        m_front = m_ViewVec[RIGHT_VIEW_TYPE].front;
        m_up = m_ViewVec[RIGHT_VIEW_TYPE].up;
        m_eye = m_ViewVec[RIGHT_VIEW_TYPE].eye;
        break;
    case ViewType::TOP_VIEW_TYPE:
        m_front = m_ViewVec[TOP_VIEW_TYPE].front;
        m_up = m_ViewVec[TOP_VIEW_TYPE].up;
        m_eye = m_ViewVec[TOP_VIEW_TYPE].eye;
        break;
    case ViewType::BOTTOM_VIEW_TYPE:
        m_front = m_ViewVec[BOTTOM_VIEW_TYPE].front;
        m_up = m_ViewVec[BOTTOM_VIEW_TYPE].up;
        m_eye = m_ViewVec[BOTTOM_VIEW_TYPE].eye;
        break;
    default:
        break;
    }
    update();
    //viewShowWindow(points, m_front, m_up, m_eye, m_up, m_front);
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
        tan(m_verticalAngle / 360.0 * CH_PI);
}

void CH3dView::getBoxPoints(const QVector3D& iMin, const QVector3D& iMax, std::vector<QVector3D>& points)
{
    points.push_back(QVector3D(iMin.x(), iMin.y(), iMin.z()));
    points.push_back(QVector3D(iMax.x(), iMin.y(), iMin.z()));
    points.push_back(QVector3D(iMax.x(), iMax.y(), iMin.z()));
    points.push_back(QVector3D(iMin.x(), iMax.y(), iMin.z()));
    points.push_back(QVector3D(iMin.x(), iMin.y(), iMax.z()));
    points.push_back(QVector3D(iMin.x(), iMax.y(), iMax.z()));
    points.push_back(QVector3D(iMax.x(), iMax.y(), iMax.z()));
    points.push_back(QVector3D(iMax.x(), iMin.y(), iMax.z()));
}

void CH3dView::machinePlatformSizeChanged(float length, float width, float height)
{
    m_length = length;
    m_width = width;
    m_height = height;

    m_ViewVec[NORMAL_VIEW_TYPE].eye = QVector3D(length / 2.0, -4 * width, height * 3.5);
    m_ViewVec[NORMAL_VIEW_TYPE].front = QVector3D(0.00884618, 0.816991, -0.576583);
    m_ViewVec[NORMAL_VIEW_TYPE].up = QVector3D(-0.0115033, 0.57665, 0.81691);

    m_ViewVec[JUST_VIEW_TYPE].eye = QVector3D(-2.3 * length, -2.3 * width, 1.63 * height);
    m_ViewVec[JUST_VIEW_TYPE].front = QVector3D(0.945542, 0.951188, -0.448531);
    m_ViewVec[JUST_VIEW_TYPE].up = QVector3D(0.228283, 0.220264, 0.948349);

    m_ViewVec[FRONT_VIEW_TYPE].eye = QVector3D(length / 2.0, -3.5 * width, height / 2.0);
    m_ViewVec[FRONT_VIEW_TYPE].front = QVector3D(0.0f, 1.0f, 0.0f);
    m_ViewVec[FRONT_VIEW_TYPE].up = QVector3D(0.0f, 0.0f, 1.0f);

    m_ViewVec[BACK_VIEW_TYPE].eye = QVector3D(length / 2.0, 3.5 * width + width,  height / 2.0);
    m_ViewVec[BACK_VIEW_TYPE].front = QVector3D(0, -1.0, 0);
    m_ViewVec[BACK_VIEW_TYPE].up = QVector3D(0, 0, 1.0);

    m_ViewVec[LEFT_VIEW_TYPE].eye = QVector3D(-length * 3.4, width / 2.0, height / 2.0);
    m_ViewVec[LEFT_VIEW_TYPE].front = QVector3D(1.0, 0, 0);
    m_ViewVec[LEFT_VIEW_TYPE].up = QVector3D(0, 0, 1);

    m_ViewVec[RIGHT_VIEW_TYPE].eye = QVector3D(length + 3.4 * length, width / 2.0, height / 2.0);
    m_ViewVec[RIGHT_VIEW_TYPE].front = QVector3D(-1.0, 0, 0);
    m_ViewVec[RIGHT_VIEW_TYPE].up = QVector3D(0, 0, 1);

    m_ViewVec[TOP_VIEW_TYPE].eye = QVector3D(length / 2.0, width / 2.0, height * 3.2 + height);
    m_ViewVec[TOP_VIEW_TYPE].front = QVector3D(0, 0, -1);
    m_ViewVec[TOP_VIEW_TYPE].up = QVector3D(0, 1, 0);

    m_ViewVec[BOTTOM_VIEW_TYPE].eye = QVector3D(length / 2.0,  width / 2.0, -height * 3.2);
    m_ViewVec[BOTTOM_VIEW_TYPE].front = QVector3D(0, 0, 1);
    m_ViewVec[BOTTOM_VIEW_TYPE].up = QVector3D(0, -1, 0);

    initView();
}


void CH3dView::setPerspectiveTransformationRef(QVector3D rotCenter)
{
    refTranPoint = rotCenter;

    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
            tan(m_verticalAngle / 360.0 * 3.1415926);
}

void CH3dView::getMechineBoxSize(float &length, float &width, float &height)
{
    length = m_length;
    width = m_width;
    height = m_height;
}

void CH3dView::getPointWorldLocationOnFixedProjMatrix(float dx, float dy, QMatrix4x4& invertMatrix, QVector3D& outpt, float js)
{
    float x = (2.0f * (float)dx) / (float)m_w - 1.0f;
    float y = 1.0f - (2.0f * (float)dy) / (float)m_h;
    //float z = 1.0f;

    QVector4D ray_clip = QVector4D(x, y, js, 1/*js*/);
    QVector4D ray_world = invertMatrix * ray_clip;
    outpt = QVector3D(ray_world.x(), ray_world.y(), ray_world.z());
    if (ray_world.w() != 0.0)
    {
        outpt = outpt / ray_world.w();
    }
}

void CH3dView::getMoveVec(int dx, int dy, QVector3D& move)
{
    if (m_bMove)
    {
        m_bMove = false;
        cameralocation = m_eye;
        lockN = m_front;
        lockV = m_up;
        lockU = QVector3D::crossProduct(lockN, lockV);

        QMatrix4x4 projMatrix;
        QMatrix4x4 viewMatrix;
        getCurrentProjMat(projMatrix);
        getZoomViewMat(viewMatrix);
        //getCurrentViewMat(viewMatrix);
        invertMatrix = (projMatrix * viewMatrix).inverted();
        getPointWorldLocationOnFixedProjMatrix(dx, dy, invertMatrix, lastmousept);
        A = QVector3D::dotProduct(refTranPoint - cameralocation, lockN);
        double B = A / QVector3D::dotProduct(lastmousept - cameralocation, lockN);
        lastmousept = cameralocation * (1 - B) + lastmousept * B;
    }

    QVector3D  fp;
    getPointWorldLocationOnFixedProjMatrix(dx, dy, invertMatrix, fp);
    double C = A / QVector3D::dotProduct(fp - cameralocation, lockN);
    fp = cameralocation * (1 - C) + fp * C;
    move = fp - lastmousept;
    lastmousept = fp;  
}

void CH3dView::getZoomVec(int dx, int dy, QVector3D& move)
{
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    getCurrentProjMat(projMatrix);
    getZoomViewMat(viewMatrix);
    //getCurrentViewMat(viewMatrix);
    invertMatrix = (projMatrix * viewMatrix).inverted();
    QVector3D fp;
    getPointWorldLocationOnFixedProjMatrix(dx, dy, invertMatrix, fp);
    A = QVector3D::dotProduct(refTranPoint - m_eye, m_front);
    double B = A / QVector3D::dotProduct(fp - m_eye, m_front);
    fp = m_eye * (1 - B) + fp * B;
    move = fp - m_eye;
}

void CH3dView::getCurrentProjMat(QMatrix4x4& matrix)
{
    matrix.perspective(m_verticalAngle, (double)m_w / m_h, m_near, m_far);
    return;

    int w = width();
    int h = height();
    float verticalCot = 1.0 / tan(m_verticalAngle / 360.0 * 3.1415926);
    float horizCot = (float)(h) / (float)(w)*verticalCot;

    matrix = QMatrix4x4(horizCot, 0, 0, 0,
        0, verticalCot, 0, 0,
        0, 0, (m_near + m_far) / (m_near - m_far), 2.0 * m_near * m_far / (m_near - m_far),
        0, 0, -1, 0);
}

void CH3dView::getZoomViewMat(QMatrix4x4& matrix)
{
    matrix.lookAt(m_eye, m_eye + m_front, m_up);
}

void CH3dView::getCurrentViewMat(QMatrix4x4& matrix)
{
    //    if (m_viewType == NORMAL_VIEW_TYPE || m_viewType == JUST_VIEW_TYPE)
    //    {
    //        QMatrix4x4 rotXMat, trans1, trans2;
    //        matrix.lookAt(m_eye, m_eye + m_front, m_up);
    //        QVector4D tmpPos = matrix.column(3);
    //        QMatrix4x4 upMat;
    //        trans1.translate(-QVector3D(tmpPos.toVector3D()));
    //        rotXMat.rotate(30, 1, 0, 0);
    //        trans2.translate(QVector3D(tmpPos.toVector3D()));
    //        matrix = trans2 * rotXMat * trans1 * matrix;
    //    }
    //    else
    //    {
    //        matrix.lookAt(m_eye, m_eye + m_front, m_up);
    //    }
    matrix.lookAt(m_eye, m_eye + m_front, m_up);
    return;

    QVector3D curAxisZ = -m_front;
    QVector3D curAxisY = m_up;
    QVector3D axisY(0, 1, 0);
    QMatrix4x4 mat1, mat2, mat3;
    mat1 = TransformPack::rotMat(curAxisZ, QVector3D(0, 0, 1), m_eye);
    curAxisY = TransformPack::vRot(mat1, curAxisY);
    mat2 = TransformPack::rotMat(curAxisY, axisY, m_eye);
    mat3.translate(-m_eye);
    matrix = mat3 * mat2 * mat1;
}

void CH3dView::paintMeshVbo(QOpenGLShaderProgram& shaderProgram, CHMeshShowObjPtr mesh, CHRenderMeshPtr rm, QMatrix4x4& pvMatrix)
{
    
    QColor color = mesh->getColor();
    if (mesh->getStatus() == canPicked || mesh->getStatus() == selected)
    {
        color = canPickedColor;
    }
    if (std::dynamic_pointer_cast<CH3DPrintModel>(mesh) &&
        (!std::dynamic_pointer_cast<CH3DPrintModel>(mesh)->getIsSceneIn()))
    {
        color = OutOfRangeColor;
    }

    if (mesh->isLogo())
    {
        //qDebug() << mesh->m_meshPath << ", " << mesh->getTransform();
    }
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix = mesh->getTransform();
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

    QOpenGLVertexArrayObject::Binder vaoBind(&(rm->m_vao));
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    
    //shaderProgram.setUniformValue("calLight", mesh->getCalLight());
    if (mesh->m_showMode == MeshShowFace)
    {
        shaderProgram.setUniformValue("calLight", mesh->getCalLight());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (mesh->m_showMode == MeshShowEdge)
    {
        shaderProgram.setUniformValue("calLight", false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (mesh->m_showMode == MeshShowVertex)
    {
        shaderProgram.setUniformValue("calLight", false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rm->m_nIBOId);
    glDrawElements(GL_TRIANGLES, rm->m_fNum * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CH3dView::paintCurveVbo(QOpenGLShaderProgram& shaderProgram, CHCurveShowObjPtr curve, CHRenderCurvePtr rc, QMatrix4x4& pvMatrix)
{
    
    QColor color = curve->getColor();
    if (curve->getStatus() == canPicked || curve->getStatus() == selected)
    {
        color = canPickedColor;
    }
    //qDebug() << "Curve: Color: (" << color.red() << ", " << color.green() << ", " << color.blue() << ")" << "  Width: " << curve->getLineWide();
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix = curve->getTransform();
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

#ifdef _WIN32
    glLineWidth(curve->getLineWide());
#elif __APPLE__
        //glLineWidth(curve->getLineWide());
#endif
    QOpenGLVertexArrayObject::Binder vaoBind(&(rc->m_vao));
    glDrawArrays(GL_LINE_STRIP, 0, rc->m_Num);
}

void CH3dView::paintPointVbo(QOpenGLShaderProgram& shaderProgram, CHPointShowObjPtr point, CHRenderPointSpherePtr rps, QMatrix4x4& pvMatrix)
{
    shaderProgram.setUniformValue("calLight", point->getCalLight());

    
    QColor color = point->getColor();
    if (point->getStatus() == canPicked || point->getStatus() == selected)
    {
        color = canPickedColor;
    }
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix;
    QVector3D newCoord = TransformPack::pRot(point->getTransform(), point->m_coord);
    mMatrix.translate(newCoord);
    if (true)
    {
        QMatrix4x4 sMatrix;
        sMatrix.scale(QVector3D::dotProduct(newCoord - m_eye, m_front) * tan(m_verticalAngle / 360.0 * CH_PI)
            / height() * 2 * point->m_size);
        mMatrix = mMatrix * sMatrix;
    }
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

    QOpenGLVertexArrayObject::Binder vaoBind(&(rps->m_vao));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rps->m_nIBOId);
    glDrawElements(GL_TRIANGLES, rps->m_fNum * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

