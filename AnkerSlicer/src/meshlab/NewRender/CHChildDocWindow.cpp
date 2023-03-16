#include "CHChildDocWindow.h"
#include "QOpenGLFunctions"
#include "common/GeoAndShow/CHLineSegment3DShowObj.h"
#include "common/GeoAndShow/CHPrintMachineBox.h"
#include "QCoreApplication"
#include "common/utilities/tlogger.h"


CHChildDocWindow::CHChildDocWindow(QWidget* parent) : QWidget(parent)
{
    AkUtil::TFunction("");
    connect(m_doc.get(), &CHDoc::modelObjsStatusChanged, m_pickCommand.get(), &CHPickOperationCommand::doWithDocMeshModelsChanged);
    m_view = new CH3dView(this);
    connect(this, &CHChildDocWindow::machinePlatformSizeSignal, m_view, &CH3dView::machinePlatformSizeChanged);
    m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_view->show();

    m_renderData = CHRenderDataPtr(new CHRenderData);
    setAcceptDrops(true);
}

CHChildDocWindow::~CHChildDocWindow()
{

}

void CHChildDocWindow::refresh()
{
    m_view->update();
}

void CHChildDocWindow::getCurNearFarPoint(int x, int y, QVector3D& np, QVector3D& fp)
{
    m_view->GetNearFarPoint(x, y, np, fp);
}

void CHChildDocWindow::pixelToWorld(float x, float y, float z, QVector3D& worldCoord)
{
    m_view->pixelToWorld(x, y, z, worldCoord);
}

void CHChildDocWindow::worldToPixel(QVector3D worldCoord, float& x, float& y, float& z)
{
    m_view->worldToPixel(worldCoord, x, y, z);
}

bool CHChildDocWindow::getCurMouseInWidget(int x, int y)
{
    QRect rect = geometry();
    return rect.contains(x, y);
}

bool CHChildDocWindow::isActivteAppWindow()
{
    return isActiveWindow();
}

void CHChildDocWindow::getMechineBoxSize(float &length, float &width, float &height)
{
    m_view->getMechineBoxSize(length, width, height);
}

QOpenGLShaderProgram& CHChildDocWindow::getShaderProgram()
{
    return m_renderData->m_shaderProgram;
}

CHRenderDataPtr CHChildDocWindow::getRenderData()
{
    return m_renderData;
}

void CHChildDocWindow::generateBasePointBuffer()
{
    CHMeshShowObjPtr omesh = m_renderData->getSphereMesh();
    writeToGPUBUffer(omesh, m_renderData->m_baseShowPoint->m_vbo, m_renderData->m_baseShowPoint->m_norbo,
        m_renderData->m_baseShowPoint->m_vao, m_renderData->m_baseShowPoint->m_nIBOId, m_renderData->m_baseShowPoint->m_fNum);

    CHLineSegment3DShowObjPtr line(new CHLineSegment3DShowObj);
    line->create(QVector3D(0, 0, 0), QVector3D(1, 0, 0));
    writeToGPUBUffer(line, m_renderData->m_unitLine);

    if (0)
    {
        
        QString path = QCoreApplication::applicationDirPath().toLocal8Bit().data();
        path += QString("/models/logo.stl");
        //QString path("Z:/share/logo.stl");
        m_doc->m_machineBox = CHPrintMachineBoxPtr(new CHPrintMachineBox);
        m_doc->m_machineBox->create(path, 230, 230, 300);
        m_doc->m_machineBox->setColor(QColor(229, 229, 229)/*QColor(80, 80, 80)*/);
        m_doc->m_machineBox->updateToScene();
    }


    m_doc->m_showPickedAABB->updateToScene();
}

void CHChildDocWindow::createMachineBox(const QString& logPath, float x, float y, float z, QColor color)
{
    AkUtil::TFunction(QString(""));
    
    QString path = QCoreApplication::applicationDirPath().toLocal8Bit().data();
    path += logPath;
    if (m_doc->m_machineBox == NULL)
    {
        m_doc->m_machineBox = CHPrintMachineBoxPtr(new CHPrintMachineBox);
    }

    m_doc->m_machineBox->create(path, x, y, z);
    m_doc->m_machineBox->setColor(color/*QColor(80, 80, 80)*/);
    m_doc->m_machineBox->updateToScene();

    m_view->calcSceneParamsFromBox(m_doc->m_machineBox->getBaseAABB());

    m_doc->m_showPickedAABB->updateToScene();
}

void CHChildDocWindow::updateMechineBox(float x, float y, float z)
{
    if (m_doc->m_machineBox == NULL)
    {
        return;
    }
    m_doc->m_machineBox->updateMechineSize(x, y, z);
    m_doc->m_machineBox->updateToScene();
    m_view->calcSceneParamsFromBox(m_doc->m_machineBox->getBaseAABB());
    m_doc->m_showPickedAABB->updateToScene();
}

void CHChildDocWindow::updatePoint(CHPointShowObjPtr pb)
{
    set<CHPointShowObjPtr>::iterator it = m_renderData->m_allpoints.find(pb);
    if (it != m_renderData->m_allpoints.end())
    {
        if (pb->getDelete())
        {
            
            m_renderData->m_allpoints.erase(it);
        }
    }
    else   
    {
        m_renderData->m_allpoints.insert(pb);
    }
}

void CHChildDocWindow::updateCurve(CHCurveShowObjPtr showcurve){
    m_view->makeCurrent();

    std::map<CHCurveShowObjPtr, CHRenderCurvePtr>::iterator it = m_renderData->m_allcurves.find(showcurve);
    if (it != m_renderData->m_allcurves.end())
    {
        if (showcurve->getDelete())
        {
            
            it->second->m_vbo.destroy();
            it->second->m_vao.destroy();
            
            m_renderData->m_allcurves.erase(it);
        }
        else
        {
            if (showcurve->getDirty())
            {
                showcurve->setDirty(false);

                
                qDebug() << "destroy ago: " << it->second->m_vbo.bufferId();
                it->second->m_vbo.destroy();
                it->second->m_vao.destroy();
                writeToGPUBUffer(showcurve, it->second);
            }
        }
    }
    else 
    {
        if (!showcurve->getDelete())
        {
            showcurve->setDirty(false);

            CHRenderCurvePtr rc(new CHRenderCurve);
            writeToGPUBUffer(showcurve, rc);
            m_renderData->m_allcurves.insert(pair<CHCurveShowObjPtr, CHRenderCurvePtr>(showcurve, rc));
        }
    }

    m_view->doneCurrent();
}

void CHChildDocWindow::updateMesh(CHMeshShowObjPtr mb)
{
    m_view->makeCurrent();

    map<CHMeshShowObjPtr, CHRenderMeshPtr>::iterator it = m_renderData->m_allmeshes.find(mb);
    if (it != m_renderData->m_allmeshes.end())
    {
        if (mb->getDelete())
        {
            
            if(it->first != nullptr)
            {
                //qDebug() << "delete: " << it->first->getObjectName();
                it->second->m_vbo.destroy();
                it->second->m_norbo.destroy();
                it->second->m_vao.destroy();
                m_view->glDeleteBuffers(1, &it->second->m_nIBOId);

                
                m_renderData->m_allmeshes.erase(it);
            }
        }
        else
        {
            if (mb->getDirty())
            {
                mb->setDirty(false);

                it->second->m_vbo.destroy();
                it->second->m_norbo.destroy();
                it->second->m_vao.destroy();
                m_view->glDeleteBuffers(1, &it->second->m_nIBOId);

                writeToGPUBUffer(mb, it->second->m_vbo, it->second->m_norbo, it->second->m_vao,
                    it->second->m_nIBOId, it->second->m_fNum, false);
            }
        }
    }
    else 
    {
        if (!mb->getDelete())
        {
            mb->setDirty(false);

            CHRenderMeshPtr rm(new CHRenderMesh);
            rm->m_beBaseMesh = true;

            writeToGPUBUffer(mb, rm->m_vbo, rm->m_norbo, rm->m_vao, rm->m_nIBOId, rm->m_fNum, false);
            m_renderData->m_allmeshes.insert(pair<CHMeshShowObjPtr, CHRenderMeshPtr>(mb, rm));
        }
    }

    m_view->doneCurrent();
}

void CHChildDocWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->accept();
    }
    QWidget::dragEnterEvent(event);
}

void CHChildDocWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* data = event->mimeData();
    if (!data->hasUrls()) {
        return;
    }
    QList<QUrl> urlList = data->urls();
    QStringList fileList;

    for (auto url : urlList) {
        QString file = url.toLocalFile();
        if (!file.isEmpty()) {
            fileList.append(file);
        }
    }
    if (fileList.isEmpty()) {
        return;
    }
    emit  openFiles(fileList);
    QWidget::dropEvent(event);
}

void CHChildDocWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (width() != 0 && height() != 0)
    {
        m_view->resize(width(), height());
        //m_view->resizeGL(width(), height());
        //m_view->resize(width(), height());
    }

}

void CHChildDocWindow::writeToGPUBUffer(CHMeshShowObjPtr mesh, QOpenGLBuffer& outvbo, QOpenGLBuffer& outnorbo,
    QOpenGLVertexArrayObject& outvao, GLuint& outibo, int& fnum, bool normalFlip)
{
    int verNum = mesh->m_vertices.size();
    float* vertices = reinterpret_cast<float*>(mesh->m_vertices.data());
    float* nors = reinterpret_cast<float*>(mesh->m_nors.data());
    fnum = mesh->m_trians.size();
    int* tri = reinterpret_cast<int*>(mesh->m_trians.data());

    QOpenGLVertexArrayObject::Binder vaoBind(&outvao);
    outvbo.create();
    outvbo.bind();
    outvbo.allocate(vertices, sizeof(GLfloat) * verNum * 3);
    int attr = -1;
    attr = m_renderData->m_shaderProgram.attributeLocation("aPos");
    m_renderData->m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgram.enableAttributeArray(attr);
    outvbo.release();

    outnorbo.create();
    outnorbo.bind();
    outnorbo.allocate(nors, sizeof(GLfloat) * verNum * 3);
    attr = m_renderData->m_shaderProgram.attributeLocation("aNormal");
    m_renderData->m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgram.enableAttributeArray(attr);
    if (normalFlip)
    {
        float* nordata = (float*)outnorbo.map(QOpenGLBuffer::Access::ReadWrite);
        for (int i = 0; i < verNum * 3; i++)
        {
            nordata[i] = -nordata[i];
        }
        outnorbo.unmap();
    }
    outnorbo.release();

    
    m_view->glGenBuffers(1, &outibo);
    
    m_view->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outibo);
    m_view->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * fnum * 3, tri, GL_STATIC_DRAW);
    m_view->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void CHChildDocWindow::writeToGPUBUffer(CHCurveShowObjPtr curvebody, CHRenderCurvePtr rendercurve)
{
    vector<QVector3D>& points = curvebody->getShowPoints();
    rendercurve->m_Num = (int)points.size();
    GLfloat* vertices = new GLfloat[rendercurve->m_Num * 3];
    for (int i = 0; i < rendercurve->m_Num; i++)
    {
        vertices[3 * i] = points[i].x();
        vertices[3 * i + 1] = points[i].y();
        vertices[3 * i + 2] = points[i].z();
    }

    QOpenGLVertexArrayObject::Binder vaoBind(&(rendercurve->m_vao));
    rendercurve->m_vbo.create();
    //qDebug() << "create vbo: " << rendercurve->m_vbo.bufferId();
    rendercurve->m_vbo.bind();
    rendercurve->m_vbo.allocate(vertices, sizeof(GLfloat) * rendercurve->m_Num * 3);
    int attr = -1;
    attr = m_renderData->m_shaderProgram.attributeLocation("aPos");
    m_renderData->m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgram.enableAttributeArray(attr);
    rendercurve->m_vbo.release();

    delete [] vertices;
}

void CHChildDocWindow::setView(const CH3dView::ViewType& _type, const CHAABB3D& aabb)
{
    if (m_view)
        m_view->setView(_type, aabb);
}















