#include <GL/glew.h>
#include "scene3d.h"
#include "common/utilities/tlogger.h"

Scene3D::Scene3D(QWidget* widget, const SceneParam& param) : m_widget(widget), m_sceneParam(param) , m_viewType()
{
    AkUtil::TFunction("");
    m_w = m_widget->width();
    m_h = m_widget->height();
    //reset();
    float max_x = param.m_printMachineBox.m_length;
    float max_y = param.m_printMachineBox.m_width;
    float max_z = param.m_printMachineBox.m_height;

    float min_x = 0.0;
    float min_y = 0.0;
    float min_z = 0.0;
    calcSceneParamsFromBox(CHAABB3D(min_x, min_y, min_z, max_x, max_y, max_z));
}

Scene3D::~Scene3D()
{

}

void Scene3D::reset()
{
    AkUtil::TFunction("");
    m_verticalAngle = 30;//28;
    m_near = 10;
    m_far = 10000;
    m_bMove = true;
    m_controlKeyDown = false;
    m_mousepress = false;
    m_front = QVector3D(0, 1.0, 0);
    m_up = QVector3D(0, 0, 1.0);
    refTranPoint = m_mainBox.getCenterPoint();
    //refTranPoint[2] = 0.0;

    float length = m_sceneParam.m_printMachineBox.m_length;
    float width = m_sceneParam.m_printMachineBox.m_width;
    float height = m_sceneParam.m_printMachineBox.m_height;

    m_mainBox = CHAABB3D(-length / 2, -width / 2, 0.0, length / 2, width / 2, height);

    setCenter();
}

void Scene3D::calcSceneParamsFromBox(const CHAABB3D& aabb)
{
    AkUtil::TFunction("");
    m_verticalAngle = 30;
    m_near = 10;
    m_far = 10000;

    m_mainBox = aabb;
    QVector3D center = aabb.getCenterPoint();

    //    m_eye = QVector3D(center.x(), -5 * aabb.getLenY(), center.z());
    //    m_up = QVector3D(0, 0, 1.0);
    //    m_front = QVector3D(0, 1.0, 0);
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
    setCenter();
}

ViewType Scene3D::getVIew()
{
   return m_viewType;
};

void Scene3D::setView(const ViewType& _type)
{
    m_verticalAngle = 30;
    QVector3D center = refTranPoint = m_mainBox.getCenterPoint();
    //refTranPoint[2] = 0.0;
    m_viewType = _type;
    switch (_type)
    {
    case ViewType::FRONT_VIEW_TYPE:
    {
        m_front = QVector3D(0, 1.0, 0);
        m_up = QVector3D(0, 0, 1);
        m_eye = QVector3D(center.x(), -800, 100.0);
        //m_eye = QVector3D(center.x(), -5 * m_mainBox.getLenY(), center.z());
    }
    break;
    case ViewType::BACK_VIEW_TYPE:
    {
        m_front = QVector3D(0, -1.0, 0);
        m_up = QVector3D(0, 0, 1.0);
        m_eye = QVector3D(center.x(), 800 + m_mainBox.getLenY(), 100.0);
        //m_eye = QVector3D(center.x(), 5 * m_mainBox.getLenY(), center.z());
    }
    break;
    case ViewType::LEFT_VIEW_TYPE:
    {
        m_front = QVector3D(1.0, 0, 0);
        m_up = QVector3D(0, 0, 1);
        m_eye = QVector3D(-800, center.y(), 100);
        //m_eye = QVector3D(-5 * m_mainBox.getLenX(), center.y(), center.z());
    }
    break;
    case ViewType::RIGHT_VIEW_TYPE:
    {
        m_front = QVector3D(-1.0, 0, 0);
        m_up = QVector3D(0, 0, 1);
        m_eye = QVector3D(800 + m_mainBox.getLenX(), center.y(), 100.0);
        //m_eye = QVector3D(5 * m_mainBox.getLenX(), center.y(), center.z());
    }
    break;
    case ViewType::TOP_VIEW_TYPE:
    {
        m_front = QVector3D(0, 0, -1);
        m_up = QVector3D(0, 1, 0);
        m_eye = QVector3D(center.x(), center.y(), 800 + m_mainBox.getLenZ());
        //m_eye = QVector3D(center.x(), center.y(), 5 * m_mainBox.getLenZ());
    }
    break;
    case ViewType::BOTTOM_VIEW_TYPE:
    {
        m_front = QVector3D(0, 0, 1);
        m_up = QVector3D(0, -1, 0);
        m_eye = QVector3D(center.x(), center.y(), -800);
        //m_eye = QVector3D(center.x(), center.y(), -5 * m_mainBox.getLenZ());
    }
    break;
    case ViewType::JUST_VIEW_TYPE:
    {
        m_front = QVector3D(1.0, 1.0, 0);
        m_up = QVector3D(0, 0, 1);
        m_eye = QVector3D(-800 / sqrt(2), -800 / sqrt(2), 100.0);
        //m_eye = QVector3D(-5 * sqrtf(m_mainBox.getLenY() * m_mainBox.getLenY() + m_mainBox.getLenX() * m_mainBox.getLenX()) / sqrtf(2),
        //    -5 * sqrtf(m_mainBox.getLenY() * m_mainBox.getLenY() + m_mainBox.getLenX() * m_mainBox.getLenX()) / sqrtf(2), center.z());
        break;
    }
    break;
    default:
        break;
    }
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) * (float)(m_w) / (float)(m_h)*
        tan(m_verticalAngle / 360.0 * CH_PI);
}

void Scene3D::getBoxPoints(const BoxType& box, std::vector<QVector3D>& points)
{

    points.push_back(QVector3D(box.iMin.x(), box.iMin.y(), box.iMin.z()));
    points.push_back(QVector3D(box.iMax.x(), box.iMin.y(), box.iMin.z()));
    points.push_back(QVector3D(box.iMax.x(), box.iMax.y(), box.iMin.z()));
    points.push_back(QVector3D(box.iMin.x(), box.iMax.y(), box.iMin.z()));
    points.push_back(QVector3D(box.iMin.x(), box.iMin.y(), box.iMax.z()));
    points.push_back(QVector3D(box.iMin.x(), box.iMax.y(), box.iMax.z()));
    points.push_back(QVector3D(box.iMax.x(), box.iMax.y(), box.iMax.z()));
    points.push_back(QVector3D(box.iMax.x(), box.iMin.y(), box.iMax.z()));
}

void Scene3D::getBoxPoints(const CHAABB3D& box, std::vector<QVector3D>& points)
{

    box.m_Xmax, box.m_Ymax, box.m_Zmax, box.m_Xmin, box.m_Ymin, box.m_Zmin;
    points.push_back(QVector3D(box.m_Xmin, box.m_Ymin, box.m_Zmin));
    points.push_back(QVector3D(box.m_Xmax, box.m_Ymin, box.m_Zmin));
    points.push_back(QVector3D(box.m_Xmax, box.m_Ymax, box.m_Zmin));
    points.push_back(QVector3D(box.m_Xmin, box.m_Ymax, box.m_Zmin));
    points.push_back(QVector3D(box.m_Xmin, box.m_Ymin, box.m_Zmax));
    points.push_back(QVector3D(box.m_Xmin, box.m_Ymax, box.m_Zmax));
    points.push_back(QVector3D(box.m_Xmax, box.m_Ymax, box.m_Zmax));
    points.push_back(QVector3D(box.m_Xmax, box.m_Ymin, box.m_Zmax));
}

CHAABB3D Scene3D::getMainbox() const
{
    return m_mainBox;
}

void Scene3D::setCenter()
{

    QVector3D center = m_mainBox.getCenterPoint(); //(m_bbox.iMax + m_bbox.iMin) / 2.0;
    m_worldCoordinate.m_size = center.length() / 5;
    m_worldCoordinate.m_matrix = QMatrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    refTranPoint = center;
    //refTranPoint[2] = 0.0;
    /*td::vector<QVector3D> points;
    getBoxPoints(m_mainBox, points);
    showObjectsCentral(points, m_front, m_up, m_eye, m_up, m_front);  */

    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) *
        (float)(m_w) / (float)(m_h)*tan(m_verticalAngle / 360.0 * PI);
}

void Scene3D::setBBox(const BoxType& _box)
{

    m_bbox = _box;
    setCenter();
}

void Scene3D::drawScene()
{
    drawWorldCoordinate();
    drawBBox();
}

void Scene3D::drawCoordinate(const CoordinateType& _coord)
{
    float linewidth = _coord.m_linewidth;
    QMatrix4x4 matrix = _coord.m_matrix;
    QVector3D center = _coord.m_center;
    float size = _coord.m_size;

    glLineWidth(linewidth);
    glPushMatrix();
    glMultMatrixf(matrix.constData());
    glBegin(GL_LINES);
    glColor4b(125, 0, 0, 255);
    glVertex3f(center.x(), center.y(), center.z());  glVertex3f(size + center.x(), center.y(), center.z());
    glColor4b(0, 125, 0, 255);
    glVertex3f(center.x(), center.y(), center.z());  glVertex3f(center.x(), size + center.y(), center.z());
    glColor4b(0, 0, 125, 255);
    glVertex3f(center.x(), center.y(), center.z());  glVertex3f(center.x(), center.y(), size + center.z());
    glEnd();
    glPopMatrix();
}

void Scene3D::drawWorldCoordinate()
{
    drawCoordinate(m_worldCoordinate);
}

void Scene3D::drawBBox()
{
    qreal rgb3[3] = { 0.0 };
    qreal rgb4[4] = { 0.0 };
    m_bbox.iColor.getRgbF(&rgb3[0], &rgb3[1], &rgb3[2]);
    m_bbox.iBottomTriColor.getRgbF(&rgb4[0], &rgb4[1], &rgb4[2], &rgb4[3]);

    QVector3D mi = m_bbox.iMin;
    QVector3D ma = m_bbox.iMax;
    float div = m_bbox.iDiv;

    QVector3D d3 = (ma - mi) / 2.0;
    QVector3D zz(0, 0, 0);

    // setup
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(m_bbox.iLineWidth);
    glDepthMask(GL_FALSE);

    glPushMatrix();
    glBegin(GL_LINES);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], mi[2]); glVertex3f(mi[0] + d3[0], mi[1] + zz[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], mi[2]); glVertex3f(mi[0] + zz[0], mi[1] + d3[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], mi[2]); glVertex3f(mi[0] + zz[0], mi[1] + zz[1], mi[2] + d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], mi[2]); glVertex3f(ma[0] - d3[0], mi[1] + zz[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], mi[2]); glVertex3f(ma[0] + zz[0], mi[1] + d3[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], mi[2]); glVertex3f(ma[0] + zz[0], mi[1] + zz[1], mi[2] + d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], mi[2]); glVertex3f(mi[0] + d3[0], ma[1] + zz[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], mi[2]); glVertex3f(mi[0] + zz[0], ma[1] - d3[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], mi[2]); glVertex3f(mi[0] + zz[0], ma[1] + zz[1], mi[2] + d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], mi[2]); glVertex3f(ma[0] - d3[0], ma[1] + zz[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], mi[2]); glVertex3f(ma[0] + zz[0], ma[1] - d3[1], mi[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], mi[2]); glVertex3f(ma[0] + zz[0], ma[1] + zz[1], mi[2] + d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], ma[2]); glVertex3f(mi[0] + d3[0], mi[1] + zz[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], ma[2]); glVertex3f(mi[0] + zz[0], mi[1] + d3[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], mi[1], ma[2]); glVertex3f(mi[0] + zz[0], mi[1] + zz[1], ma[2] - d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], ma[2]); glVertex3f(ma[0] - d3[0], mi[1] + zz[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], ma[2]); glVertex3f(ma[0] + zz[0], mi[1] + d3[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], mi[1], ma[2]); glVertex3f(ma[0] + zz[0], mi[1] + zz[1], ma[2] - d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], ma[2]); glVertex3f(mi[0] + d3[0], ma[1] + zz[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], ma[2]); glVertex3f(mi[0] + zz[0], ma[1] - d3[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(mi[0], ma[1], ma[2]); glVertex3f(mi[0] + zz[0], ma[1] + zz[1], ma[2] - d3[2]);

    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], ma[2]); glVertex3f(ma[0] - d3[0], ma[1] + zz[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], ma[2]); glVertex3f(ma[0] + zz[0], ma[1] - d3[1], ma[2] + zz[2]);
    glColor3f(rgb3[0], rgb3[1], rgb3[2]); glVertex3f(ma[0], ma[1], ma[2]); glVertex3f(ma[0] + zz[0], ma[1] + zz[1], ma[2] - d3[2]);

    glEnd();


    
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, -1.0f);
    
    glBegin(GL_TRIANGLES);
    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(mi[0], mi[1], mi[2]); glVertex3f(ma[0], mi[1], mi[2]); glVertex3f(mi[0], mi[1] + div, mi[2]);
    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(ma[0], mi[1] + div, mi[2]); glVertex3f(ma[0], mi[1], mi[2]); glVertex3f(mi[0], mi[1] + div, mi[2]);

    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(mi[0], mi[1] + div, mi[2]); glVertex3f(mi[0] + div, mi[1] + div, mi[2]); glVertex3f(mi[0], ma[1], mi[2]);
    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(mi[0] + div, ma[1], mi[2]); glVertex3f(mi[0] + div, mi[1] + div, mi[2]); glVertex3f(mi[0], ma[1], mi[2]);

    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(mi[0] + div, ma[1], mi[2]); glVertex3f(mi[0] + div, ma[1] - div, mi[2]); glVertex3f(ma[0], ma[1] - div, mi[2]);
    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(ma[0], ma[1], mi[2]); glVertex3f(mi[0] + div, ma[1], mi[2]); glVertex3f(ma[0], ma[1] - div, mi[2]);

    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(ma[0] - div, ma[1] - div, mi[2]); glVertex3f(ma[0], ma[1] - div, mi[2]); glVertex3f(ma[0], mi[1] + div, mi[2]);
    glColor4f(rgb4[0], rgb4[1], rgb4[2], rgb4[3]); glVertex3f(ma[0] - div, mi[1] + div, mi[2]); glVertex3f(ma[0], mi[1] + div, mi[2]); glVertex3f(ma[0] - div, ma[1] - div, mi[2]);


    glColor4f(1.0f, 1.0f, 1.0f, 0.5f); glVertex3f(mi[0] + div, mi[1] + div, mi[2]); glVertex3f(ma[0] - div, mi[1] + div, mi[2]); glVertex3f(mi[0] + div, ma[1] - div, mi[2]);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f); glVertex3f(ma[0] - div, ma[1] - div, mi[2]); glVertex3f(ma[0] - div, mi[1] + div, mi[2]); glVertex3f(mi[0] + div, ma[1] - div, mi[2]);
    glEnd();

    glColor3f(rgb3[0], rgb3[1], rgb3[2]);
    glLineWidth(m_bbox.iLineWidth);

    
    for (int i = 0; i <= (ma[0] - mi[0]) / m_bbox.iTick; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(mi[0] + i * m_bbox.iTick, mi[1], mi[2]);
        glVertex3f(mi[0] + i * m_bbox.iTick, ma[1], mi[2]);
        glEnd();
    }

    glColor3f(rgb3[0], rgb3[1], rgb3[2]);

    for (int i = 0; i <= (ma[1] - mi[1]) / m_bbox.iTick; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(mi[0], mi[1] + i * m_bbox.iTick, mi[2]);
        glVertex3f(ma[0], mi[1] + i * m_bbox.iTick, mi[2]);
        glEnd();
    }

    glDepthMask(GL_TRUE);
    glPopMatrix();
    glPopAttrib();
}



void Scene3D::resizeWidget(int w, int h)
{

    m_w = w;
    m_h = h;
}

bool Scene3D::getIsMousePress() const
{

    return m_mousepress;
}

bool Scene3D::getIsControlKeyDown() const
{
    return m_controlKeyDown;
}

void Scene3D::setBMove(bool is)
{
    m_bMove = is;
}
void Scene3D::setMousePress(bool is)
{
    m_mousepress = is;
}

void Scene3D::setControlKeyDown(bool is)
{
    m_controlKeyDown = is;
}


void Scene3D::showObjectsCentral(std::vector<QVector3D> points, const QVector3D& inFront, const QVector3D& inUp, QVector3D& outEye, QVector3D& outUp, QVector3D& outFront, double scale)
{
    outUp = inUp;
    outFront = inFront;

    
    QMatrix4x4 mat1 = rotMat(outFront, QVector3D(0, 0, 1), QVector3D(0, 0, 0));
    QVector3D curAxisY = -outUp;
    curAxisY = vRot(mat1, curAxisY);
    double rotAngle = vecAngle(curAxisY, QVector3D(0, 1, 0));
    QVector3D rotAxis = QVector3D::crossProduct(curAxisY, QVector3D(0, 1, 0));
    if (fabs(rotAngle - PI) < EPSINON)   
    {
        rotAxis = QVector3D(0, 0, 1);
    }
    QMatrix4x4 mat2 = rotMat(QVector3D(0, 0, 0), rotAxis, rotAngle);

    for (int i = 0; i < points.size(); i++)
    {
        points[i] = pRot(mat2 * mat1, points[i]);
    }

    double xMin, yMin, xMax, yMax;
    xMin = yMin = MAXNUM;
    xMax = yMax = MINNUM;

    double reductionZ = m_near;
    double ratio1 = tan(m_verticalAngle / 180.0 * PI / 2.0);
    double ratio2 = ratio1 * m_w / m_h;
    for (int i = 0; i < points.size(); i++)
    {
        double x1 = points[i].x() + (points[i].z() - reductionZ) * ratio2;
        double x2 = points[i].x() - (points[i].z() - reductionZ) * ratio2;
        double y1 = points[i].y() + (points[i].z() - reductionZ) * ratio1;
        double y2 = points[i].y() - (points[i].z() - reductionZ) * ratio1;

        if (xMin > x1)
        {
            xMin = x1;
        }
        if (xMax < x2)
        {
            xMax = x2;
        }
        if (yMin > y1)
        {
            yMin = y1;
        }
        if (yMax < y2)
        {
            yMax = y2;
        }
    }

    double h1 = (yMax - yMin) / 2.0 / ratio1;
    double h2 = (xMax - xMin) / 2.0 / ratio2;
    double h = h1;
    if (h < h1)
    {
        h = h2;
    }

    m_eye = QVector3D((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, reductionZ - h / scale);
    m_eye = pRot((mat2 * mat1).inverted(), m_eye);
}

void Scene3D::scale(int x, int y, float d)
{

    QVector3D move;
    getZoomVec(x, y, move);
    m_eye = m_eye + move * d;

    
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) *
        (float)(m_w) / (float)(m_h)*tan(m_verticalAngle / 360.0 * PI) ;
}

void Scene3D::xyTranslate(int dx, int dy)
{
    QVector3D moveVec;
    getMoveVec(dx, dy, moveVec);
    m_eye = m_eye + moveVec * (-1);
}

void Scene3D::xyRotate(int dx, int dy)
{
    //getPointWorldLocationOnFixedProjMatrix(dx, dy, invertMatrix, refTranPoint);
    QVector3D moveVec;
    getMoveVec(dx, dy, moveVec);
    if (moveVec != QVector3D(0, 0, 0))
    {
        lastmousept.setX(lastmousept.x());
    }

    double H1 = QVector3D::dotProduct(lastmousept - refTranPoint, lockU);
    double H2 = QVector3D::dotProduct(lastmousept - refTranPoint, lockV);
    
    double L1 = QVector3D::dotProduct(moveVec, lockU) * 5.0 ;
    double L2 = QVector3D::dotProduct(moveVec, lockV) * 5.0 ;
    double angle1 = L2 / senseRadius;
    double angle2 = L1 / senseRadius;
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
    QVector3D axis2 = curV * sqrt(1 - A2 * A2) + curN * A2;//rotMat

    QMatrix4x4 mat;
    if(fabs(m_lastMousePts[0] - dx) > fabs(m_lastMousePts[1] - dy)) 
    {
        axis2 = QVector3D(0, 0, 1);
        mat = rotMat(refTranPoint, axis2, -angle2);
    }
    else if(fabs(m_lastMousePts[0] - dx) < fabs(m_lastMousePts[1] - dy))
    {
        axis1 = curU;
        mat = rotMat(refTranPoint, axis1, angle1);
    }
    else
    {
        mat = rotMat(refTranPoint, axis1, angle1) * rotMat(refTranPoint, axis2, -angle2);
    }
    //qDebug() << "dx: " << dx << ", dy: " << dy << ", lastmousept: " << lastmousept << ", moveVec: " << moveVec;
    m_lastMousePts[0] = dx;
    m_lastMousePts[1] = dy;
    m_eye = pRot(mat, m_eye);
    m_front = vRot(mat, m_front);
    m_up = vRot(mat, m_up);
}

void Scene3D::getNearFarPoint(int x, int y, QVector3D& np, QVector3D& fp)
{
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    getCurrentProjMat(projMatrix);
    getCurrentViewMat(viewMatrix);
    invertMatrix = (projMatrix * viewMatrix).inverted();
    getPointWorldLocationOnFixedProjMatrix(x, y, invertMatrix, np, -1);
    getPointWorldLocationOnFixedProjMatrix(x, y, invertMatrix, fp, 1);
}

void Scene3D::setPerspectiveTransformationRef(QVector3D rotCenter)
{
    refTranPoint = rotCenter;
    //refTranPoint[2] = 0.0;
    senseRadius = fabs(QVector3D::dotProduct(m_eye - refTranPoint, m_front)) *
        (float)(m_w / m_h) * tan(m_verticalAngle / 360.0 * PI);
}

void Scene3D::getPointWorldLocationOnFixedProjMatrix(int dx, int dy, QMatrix4x4& invertMatrix, QVector3D& outpt, float js)
{
    float x = (2.0f * (float)dx) / (float)m_w - 1.0f;
    float y = 1.0f - (2.0f * (float)dy) / (float)m_h;
    float z = 1.0f;

    QVector4D ray_clip = QVector4D(x, y, js, 1/*js*/);
    QVector4D ray_world = invertMatrix * ray_clip;
    outpt = QVector3D(ray_world.x(), ray_world.y(), ray_world.z());
    if (ray_world.w() != 0.0)
    {
        outpt = outpt / ray_world.w();
    }
}

void Scene3D::getMoveVec(int dx, int dy, QVector3D& move)
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
        //getCurrentViewMat(viewMatrix);
        getZoomViewMat(viewMatrix);
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

void Scene3D::getZoomVec(int dx, int dy, QVector3D& move)
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


void Scene3D::getCurrentProjMat_ssp(QMatrix4x4 &matrix)
{
    matrix.perspective(m_verticalAngle, (double)m_w / m_h, m_near, m_far);
    return;
}

void Scene3D::getCurrentProjMat_sp(QMatrix4x4 &matrix)
{
    matrix.perspective(m_verticalAngle, (double)m_w / m_h, m_near, m_far);
    return;
}

void Scene3D::getCurrentProjMat(QMatrix4x4& matrix)
{
    matrix.perspective(m_verticalAngle, (double)m_w / m_h, m_near, m_far);
    return;

    int w = m_widget->width();
    int h = m_widget->height();
    float verticalCot = 1.0 / tan(m_verticalAngle / 360.0 * PI);
    float horizCot = (float)(h) / (float)(w)*verticalCot;

    matrix = QMatrix4x4(horizCot, 0, 0, 0,
        0, verticalCot, 0, 0,
        0, 0, (m_near + m_far) / (m_near - m_far), 2.0 * m_near * m_far / (m_near - m_far),
        0, 0, -1, 0);
}

void Scene3D::getZoomViewMat(QMatrix4x4& matrix)
{
    matrix.lookAt(m_eye, m_eye + m_front, m_up);
}

void Scene3D::getCurrentViewMat(QMatrix4x4& matrix)
{
    //TODO:
//    if(isAiView)
//    {
//       matrix.lookAt(m_eye, m_viewCenter,m_up);
//       isAiView = false;
//       return;
//    }
//    if (m_viewType == NORMAL_VIEW_TYPE || m_viewType == JUST_VIEW_TYPE)
//    {
//        QMatrix4x4 rotXMat, trans1, trans2;
//        matrix.lookAt(m_eye, m_eye + m_front, m_up);
//        QVector4D tmpPos = matrix.column(3);
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
    mat1 = rotMat(curAxisZ, QVector3D(0, 0, 1), m_eye);
    curAxisY = vRot(mat1, curAxisY);
    mat2 = rotMat(curAxisY, axisY, m_eye);
    mat3.translate(-m_eye);
    matrix = mat3 * mat2 * mat1;
}

QMatrix4x4 Scene3D::rotMat(const QVector3D& pos, const QVector3D& axis, double angle)
{
    QMatrix4x4 mat1, mat2, mat3;
    mat1.translate(-pos);
    mat2.rotate(angle / PI * 180.0, axis);
    mat3.translate(pos);
    return mat3 * mat2 * mat1;
}

QMatrix4x4 Scene3D::rotMat(const QVector3D& axis1, const QVector3D& axis2, QVector3D pos)
{
    double angle = vecAngle(axis1, axis2);
    QVector3D axis = QVector3D::crossProduct(axis1, axis2);
    if (axis == QVector3D(0, 0, 0))
    {
        QVector3D ref1(1.0, 1.5, 2.0);
        QVector3D ref2(-1.0, 1.5, -2.0);

        if (fabs(angle - PI) < EPSINON)   
        {
            //angle = 0.0;
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
    return rotMat(pos, axis, angle);
}

QVector3D Scene3D::pRot(QMatrix4x4 matrix, QVector3D pt)
{
    return matrix * QVector4D(pt, 1.0).toVector3D();
}

QVector3D Scene3D::vRot(QMatrix4x4 matrix, QVector3D vec)
{
    QVector3D out1 = matrix * QVector4D(0, 0, 0, 1.0).toVector3D();
    QVector3D out2 = matrix * QVector4D(vec, 1.0).toVector3D();
    return out2 - out1;
}

double Scene3D::vecAngle(QVector3D vec1, QVector3D vec2)
{
    vec1.normalize();
    vec2.normalize();
    return acos(QVector3D::dotProduct(vec1, vec2));
}

void Scene3D::setEye(float x, float y, float z)
{

    this->m_eye.setX(x);
    this->m_eye.setY(y);
    this->m_eye.setZ(z);
}

void Scene3D::setFront(float x, float y, float z)
{

    this->m_front.setX(x);
    this->m_front.setY(y);
    this->m_front.setZ(z);
}

void Scene3D::setUp(float x, float y, float z)
{

    this->m_up.setX(x);
    this->m_up.setY(y);
    this->m_up.setZ(z);
}

void Scene3D::setRefTranPoint(float x, float y, float z)
{

    this->refTranPoint.setX(x);
    this->refTranPoint.setY(y);
    this->refTranPoint.setZ(z);
}

void Scene3D::setIsAiView()
{

	this->isAiView = true;
    this->m_verticalAngle = 90;//60.67;
}

void Scene3D::setVerticalAngle(double setAngle)
{

     m_verticalAngle = setAngle;
}

double Scene3D::getVerticalAngle()
{

     return m_verticalAngle;
}

void Scene3D::setSceneParams(const SceneParam& param)
{

    m_sceneParam.clear();
    m_sceneParam = param;
}

int Scene3D::getH() const
{
    return m_h;
}

int Scene3D::getW() const
{
    return m_w;
}

QVector3D Scene3D::getEye() const
{
    return m_eye;
}

QVector3D Scene3D::getUp() const
{
    return m_up;
}

QVector3D Scene3D::getFront() const
{
    return m_front;
}

double Scene3D::getVerticalAngle() const
{
    return m_verticalAngle;
}


