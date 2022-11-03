/****************************************************************************
Revision 0.1  2021/12/10 09:47:40  Aden Hu
****************************************************************************/
#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLTexture>
#include "camera.h"
#include "cube.h"
#include <QTimer>

class OpenGLShaderProgram;

class GLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = nullptr);
	~GLWidget();
	void horizonsliderMove(int value);
	void verticalsliderMove(int value);
protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	//void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;



public slots:
	void loadMesh(const QString& fileName);
	void cleanScene();
	void clearGL();
	void update();


private:
	QTimer* timer;

	Cube* m_cube;
	GLint u_position_id;
	GLint u_modelToWorld_id;
	GLint u_worldToCamera_id;
	GLint u_cameraToView_id;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_transform;
	//Transform m_transform;
	Camera m_camera;

	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLShaderProgram* m_program;
	QOpenGLTexture* m_texture;
};



#endif // !GLWIDGET_H
